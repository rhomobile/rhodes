#include "SyncSource.h"
#include "SyncEngine.h"

#include "common/RhoFilePath.h"
#include "common/RhoTime.h"
//#include "common/RhoFile.h"
#include "common/StringConverter.h"
//#include "common/AutoPointer.h"
#include "json/JSONIterator.h"

namespace rho {
namespace sync {
IMPLEMENT_LOGCLASS(CSyncSource,"Sync");

using namespace rho::net;
using namespace rho::db;
using namespace rho::common;
using namespace rho::json;

CSyncSource::CSyncSource() : m_syncEngine( *new CSyncEngine(*new db::CDBAdapter()))
{
}

CSyncSource::CSyncSource(int id, const String& strUrl, uint64 token, CSyncEngine& syncEngine ) : m_syncEngine(syncEngine)
{
    m_nID = id;
    m_strUrl = strUrl;
    m_token = token;

    m_nCurPageCount = 0;
    m_nInserted = 0;
    m_nDeleted = 0;
    m_nTotalCount = 0;
    m_bGetAtLeastOnePage = false;
}

CDBAdapter& CSyncSource::getDB(){ return getSync().getDB(); }
INetRequest& CSyncSource::getNet(){ return getSync().getNet(); }

void CSyncSource::sync()
{
	LOG(INFO) + "Start syncing source ID :" + getID();
    CTimeInterval startTime = CTimeInterval::getCurrentTime();
	
    syncClientChanges();
    getAndremoveAsk();
    syncServerChanges();

    CTimeInterval endTime = CTimeInterval::getCurrentTime();
    getDB().executeSQL("UPDATE sources set last_updated=?,last_inserted_size=?,last_deleted_size=?, \
						 last_sync_duration=?,last_sync_success=? WHERE source_id=?", 
                         endTime.toULong(), getInsertedCount(), getDeletedCount(), (endTime-startTime).toULong(), m_bGetAtLeastOnePage, getID() );

	LOG(INFO) + "End syncing source ID :" + getID();
}

void CSyncSource::syncClientBlobs(const String& strBaseQuery)
{
    String strQuery;
    for( int i = 0; i < (int)m_arSyncBlobs.size(); i ++)
    {
        CSyncBlob& blob = *m_arSyncBlobs.elementAt(i);

        //CRhoFile oFile;
        //if ( !oFile.open(blob.getFilePath().c_str(),CRhoFile::OpenReadOnly) ) 
        //    continue;

        strQuery = strBaseQuery + "&" + blob.getBody();
        if ( !getNet().pushFile(strQuery, blob.getFilePath() ) ) //oFile.getInputStream()) )
        {
            getSync().setState(CSyncEngine::esStop);
            return;
        }

        getDB().executeSQL("DELETE FROM object_values WHERE source_id=? and attrib_type=? and value=?", getID(), "blob.file", blob.getFilePath() );
    }

    m_arSyncBlobs.clear();
}

void CSyncSource::syncClientChanges()
{
    const char* arUpdateTypes[] = {"update", "create", "delete"};
    for( int i = 0; i < 3 && getSync().isContinueSync(); i++ )
    {
        String strUrl = getUrl() + "/" + arUpdateTypes[i];
        strUrl += "objects";
        String strQuery = CSyncEngine::SYNC_SOURCE_FORMAT() + "&client_id=" + getSync().getClientID();

        String strBody;
        makePushBody(strBody, arUpdateTypes[i]);
        if ( strBody.length() > 0 )
        {
		    LOG(INFO) + "Push client changes to server. Source id: " + getID() + "Size :" + strBody.length();
		    LOG(TRACE) + "Push body: " + strBody;		

            if ( !getNet().pushData(strUrl+strQuery,strBody) )
            {
                getSync().setState(CSyncEngine::esStop);
                continue;
            }
        }

        if ( m_arSyncBlobs.size() )
        {
		    LOG(INFO) + "Push blobs to server. Source id: " + getID() + "Count :" + m_arSyncBlobs.size();

            getDB().executeSQL("DELETE FROM object_values WHERE source_id=? and update_type=? and (attrib_type ISNULL or attrib_type!=?)", getID(), arUpdateTypes[i], "blob.file" );
            syncClientBlobs(strUrl+strQuery);
        }else
            getDB().executeSQL("DELETE FROM object_values WHERE source_id=? and update_type=?", getID(), arUpdateTypes[i] );
    }
}

/*
 * Construct the body of the request by filtering 
 * the attr_filter string. The body format should
 * look like the following:
 * create: attrvals[][attrib]=<name|industry>&attrvals[][object]=<locallygeneratedid>&attrvals[][value]=<some value>
 * update: attrvals[][attrib]=<name|industry>&attrvals[][object]=<remoteid>&attrvals[][value]=<some new value>
 * delete: attrvals[][attrib]=<name|industry>&attrvals[][object]=<remoteid>
 */
void CSyncSource::makePushBody(String& strBody, const char* szUpdateType)
{
    //boolean bFirst = true;
    DBResult( res , getDB().executeSQL("SELECT attrib, object, value, attrib_type "
					 "FROM object_values where source_id=? and update_type =?", getID(), szUpdateType ) );
    for( ; !res.isEnd(); res.next() )
    {
        String strSrcBody = "attrvals[][attrib]=" + res.getStringByIdx(0);

        if ( res.getStringByIdx(1).length() > 0 ) 
            strSrcBody += "&attrvals[][object]=" + res.getStringByIdx(1);

        String value = res.getStringByIdx(2);
        String attribType = res.getStringByIdx(3);

        /*if ( bFirst )
        {
            value = "d:\\work\\blobtest.png";
            attribType = "blob.file";
            bFirst = false;
        }*/

        if ( value.length() > 0 )
        {
            if ( attribType == "blob.file" )
            {
                common::CFilePath oBlobPath(value);
                strSrcBody += "&attrvals[][value]=";
                strSrcBody += oBlobPath.getBaseName();
                strSrcBody += "&attrvals[][attrib_type]=blob";

                m_arSyncBlobs.addElement(new CSyncBlob(strSrcBody,value));
                continue;
            }else
                strSrcBody += "&attrvals[][value]=" + value;
        }

        if ( strBody.length() > 0 )
            strBody += "&";

        strBody += strSrcBody;
    }
}

void CSyncSource::getAndremoveAsk()
{
    String askParams = "";
    {
        DBResult( res , getDB().executeSQL("SELECT value from object_values where source_id=? and update_type=?", 
            getID(), "ask" ) );
        askParams = res.isEnd() ? "" : res.getStringByIdx(0);
    }

    getDB().executeSQL("DELETE FROM object_values WHERE source_id=? and update_type=?", getID(), "ask" );

    setAskParams(askParams);
}

void CSyncSource::syncServerChanges()
{
    while( getSync().isContinueSync() )
    {
        setCurPageCount(0);
        String strUrl = getUrl();
        String strQuery = getSync().SYNC_SOURCE_FORMAT() + "&client_id=" + getSync().getClientID() + 
                "&p_size=" + getSync().SYNC_PAGE_SIZE();

        if ( getAskParams().length() > 0 )
        {
            strUrl +=  getSync().SYNC_ASK_ACTION();
            strQuery += "&question=" + getAskParams();
        }

        if ( isEmptyToken() )
            processToken(1);
        else if ( getToken() > 1 )
            strQuery += "&ack_token=" + convertToStringA(getToken());

		LOG(INFO) + "Pull changes from server. Url: " + (strUrl+strQuery);
		
        NetRequestStr(szData,getNet().pullData(strUrl+strQuery));
        if ( szData == 0 )
        {
            getSync().stopSync();
            continue;
        }

        processServerData(szData);

        m_bGetAtLeastOnePage = true;

        if ( getAskParams().length() > 0 || getCurPageCount() == 0 )
            break;
    }
}

void CSyncSource::processServerData(const char* szData)
{
    CJSONArrayIterator oJsonArr(szData);
    if ( !oJsonArr.isEnd() )
    {
        setCurPageCount(oJsonArr.getCurItem().getInt("count"));
        oJsonArr.next();
    }
    if ( !oJsonArr.isEnd() && oJsonArr.getCurItem().hasName("total_count") )
    {
        setTotalCount(oJsonArr.getCurItem().getInt("total_count"));
        oJsonArr.next();
    }
    if ( getServerObjectsCount() == 0 )
    	m_syncEngine.fireNotification(*this, false);

    if ( !oJsonArr.isEnd() )
    {
        processToken(oJsonArr.getCurItem().getUInt64("token"));
        oJsonArr.next();
    }else if ( getCurPageCount() == 0 )
        processToken(0);

	LOG(INFO) + "Got " + getCurPageCount() + " records of " + getTotalCount() + " from server. Source ID: " + getID();
	
    //TODO: support DBExceptions
    getDB().startTransaction();
    for( ; !oJsonArr.isEnd() && getSync().isContinueSync(); oJsonArr.next() )
    {
        if ( getDB().isUnlockDB() )
        {
			LOG(INFO) + "Commit transaction because of UI request.";
            getDB().endTransaction();
            getDB().startTransaction();
        }

        CJSONEntry oJsonEntry = oJsonArr.getCurItem();

        CJSONEntry oJsonObject = oJsonEntry.getEntry("object_value");
        if ( !oJsonObject.isEmpty() )
            processSyncObject(oJsonObject);
    }
    getDB().endTransaction();

    if ( getServerObjectsCount() < getTotalCount() )
    	m_syncEngine.fireNotification(*this, false);
}

void CSyncSource::processSyncObject(CJSONEntry& oJsonEntry)
{
    const char* szDbOp = oJsonEntry.getString("db_operation");
    if ( szDbOp && strcmp(szDbOp,"insert")==0 )
    {
        getDB().executeSQL("INSERT INTO object_values \
            (id, attrib, source_id, object, value, update_type,attrib_type) VALUES(?,?,?,?,?,?,?)", 
            oJsonEntry.getUInt64("id"), oJsonEntry.getString("attrib"), getID(), oJsonEntry.getString("object"),
            oJsonEntry.getString("value"), oJsonEntry.getString("update_type"), oJsonEntry.getString("attrib_type") );

        m_nInserted++;
    }else if ( szDbOp && strcmp(szDbOp,"delete")==0 )
    {
        getDB().executeSQL("DELETE FROM object_values where id=?", oJsonEntry.getUInt64("id") );

        m_nDeleted++;
    }else{
        LOG(ERROR) + "Unknown DB operation: " + (szDbOp ? szDbOp : "");
    }
}

void CSyncSource::processToken(uint64 token)
{
    if ( token > 1 && getToken() == token ){
		//Delete non-confirmed records

        getDB().executeSQL("DELETE FROM object_values where source_id=? and token=?", getID(), token );
	}else
    {
        setToken( token );
        getDB().executeSQL("UPDATE sources SET token=? where source_id=?", token, getID() );
	}

}

}
}