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
/*
CSyncSource::CSyncSource() : m_syncEngine(CSyncEngine())
{
}*/

CDBAdapter& CSyncSource::getDB(){ return getSync().getDB(); }
INetRequest& CSyncSource::getNet(){ return getSync().getNet(); }

void CSyncSource::sync()
{
	LOG(INFO) + "Start syncing source ID :" + getID();
	
    syncClientChanges();
    getAndremoveAsk();
    syncServerChanges();
	
	LOG(INFO) + "End syncing source ID :" + getID();
}

void CSyncSource::syncClientBlobs(const String& strBaseQuery)
{
    String strQuery;
    for( int i = 0; i < m_arSyncBlobs.size(); i ++)
    {
        CSyncBlob& blob = *m_arSyncBlobs.get(i);

        //CRhoFile oFile;
        //if ( !oFile.open(blob.getFilePath().c_str(),CRhoFile::OpenReadOnly) ) 
        //    continue;

        strQuery = strBaseQuery + "&" + blob.getBody();
        if ( !getNet().pushFile(getUrl()+strQuery, blob.getFilePath() ) ) //oFile.getInputStream()) )
        {
            getSync().setState(CSyncEngine::esStop);
            return;
        }

        getDB().executeSQL("DELETE FROM object_values WHERE source_id=? and attrib_type=blob.file and value=?", getID(), blob.getFilePath() );
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
        if ( strBody.length() == 0 )
            continue;

		LOG(INFO) + "Push client changes to server. Source id: " + getID() + "Size :" + strBody.length();
		LOG(TRACE) + "Push body: " + strBody;		
        if ( getNet().pushData(strUrl+strQuery,strBody) )
        {
            if ( m_arSyncBlobs.size() )
            {
                getDB().executeSQL("DELETE FROM object_values WHERE source_id=? and update_type=? and (attrib_type ISNULL or attrib_type!=?)", getID(), arUpdateTypes[i], "blob.file" );
                syncClientBlobs(strQuery);
            }else
                getDB().executeSQL("DELETE FROM object_values WHERE source_id=? and update_type=?", getID(), arUpdateTypes[i] );
        }else
            getSync().setState(CSyncEngine::esStop);
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
    DBResult( res , getDB().executeSQL("SELECT attrib, object, value, attrib_type "
					 "FROM object_values where source_id=? and update_type =?", getID(), szUpdateType ) );
//TEST ONLY
    boolean bFirst = false;
//TEST ONLY
    for( ; !res.isEnd(); res.next() )
    {
        String strSrcBody = "attrvals[][attrib]=" + res.getStringByIdx(0);

        if ( res.getStringByIdx(1).length() > 0 ) 
            strSrcBody += "&attrvals[][object]=" + res.getStringByIdx(1);

        String value = res.getStringByIdx(2);
        String attribType = res.getStringByIdx(3);
        //TEST ONLY
        if ( bFirst )
        {
            value = "d:/work/BBSign.JPG";
            attribType = "blob.file";
            bFirst = false;
        }
        //TEST ONLY

        if ( value.length() > 0 )
        {
            if ( attribType == "blob.file" )
            {
                common::CFilePath oBlobPath(value);
                strSrcBody += "&attrvals[][value]=";
                strSrcBody += oBlobPath.getBaseName();
                strSrcBody += "&attrvals[][attrib_type]=blob";

                m_arSyncBlobs.add(new CSyncBlob(strSrcBody,value));
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
    boolean bGetAtLeastOnePage = false;
    CTimeInterval startTime = CTimeInterval::getCurrentTime();

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

        bGetAtLeastOnePage = true;

        if ( getAskParams().length() > 0 || getCurPageCount() == 0 )
            break;
    }

    CTimeInterval endTime = CTimeInterval::getCurrentTime();
    getDB().executeSQL("UPDATE sources set last_updated=?,last_inserted_size=?,last_deleted_size=?, \
						 last_sync_duration=?,last_sync_success=? WHERE source_id=?", 
                         endTime.toULong(), getInsertedCount(), getDeletedCount(), (endTime-startTime).toULong(), bGetAtLeastOnePage, getID() );
}

void CSyncSource::processServerData(const char* szData)
{
    CJSONArrayIterator oJsonArr(szData);
    if ( !oJsonArr.isEnd() )
    {
        setCurPageCount(oJsonArr.getCurItem().getInt("count"));
        oJsonArr.next();
    }
    if ( !oJsonArr.isEnd() )
    {
        processToken(oJsonArr.getCurItem().getUInt64("token"));
        oJsonArr.next();
    }
	LOG(INFO) + "Got " + this->getCurPageCount() + " records from server. Source ID: " + getID();
	
    //TODO: support transactions
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
        //TODO: log error
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