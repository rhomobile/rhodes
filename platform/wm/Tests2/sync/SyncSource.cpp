#include "SyncSource.h"
#include "SyncEngine.h"

#include "common/RhoFilePath.h"
#include "common/RhoTime.h"
#include "json/JSONIterator.h"

namespace rho {
namespace sync {

using namespace rho::net;
using namespace rho::db;
using namespace rho::common;
using namespace rho::json;

CSyncSource::CSyncSource() : m_syncEngine(CSyncEngine())
{
}

CDBAdapter& CSyncSource::getDB(){ return getSync().getDB(); }
INetRequest& CSyncSource::getNet(){ return getSync().getNet(); }

void CSyncSource::sync()
{
    syncClientChanges();
    syncServerChanges();
    getSync().fireNotification(getID(),getServerObjectsCount());
}

void CSyncSource::syncClientChanges()
{
    const char* arUpdateTypes[] = {"update", "create", "delete"};
    for( int i = 0; i < 3 && getSync().getState() != CSyncEngine::esStop; i++ )
    {
        String strQuery = arUpdateTypes[i];
        strQuery += CSyncEngine::SYNC_SOURCE_FORMAT() + getSync().getClientID();

        String strBody;
        makePushBody(strBody, arUpdateTypes[i]);

        //TODO: SyncBlob
        CNetResponse oResp;
        getNet().pushData(getUrl(),strQuery,strBody,oResp);

        if ( oResp.isOK() )
        {
            getDB().executeSQL("DELETE FROM object_values WHERE source_id=? and update_type=?", getID(), arUpdateTypes[i] );
        }else{
            //getSync().setState(CSyncEngine::esStop);
        }
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
    DBResult( res , getDB().executeSQL("SELECT attrib, object, value, attrib_type \
					 FROM object_values where source_id=? and update_type =?", getID(), szUpdateType ) );

    for( ; !res.isEnd(); res.next() )
    {
        strBody += "attrvals[][attrib]=" + res.getStringByIdx(0);

        if ( res.getStringByIdx(1).length() > 0 ) 
            strBody += "&attrvals[][object]=" + res.getStringByIdx(1);

        String value = res.getStringByIdx(2);
        if ( value.length() > 0 )
        {
            if ( res.getStringByIdx(3) == "blob.file" )
            {
                common::CFilePath oBlobPath(value);
                strBody += "&attrvals[][value]=";
                strBody += oBlobPath.getBaseName();
                strBody += "&attrvals[][attrib_type]=blob";
            }else
                strBody += "&attrvals[][value]=" + value;
        }
    }
}

String CSyncSource::getAndremoveAsk()
{
    String askParams = "";
    {
        DBResult( res , getDB().executeSQL("SELECT value from object_values where source_id=? and update_type=?", 
            getID(), "ask" ) );
        askParams = res.isEnd() ? "" : res.getStringByIdx(0);
    }

    getDB().executeSQL("DELETE FROM object_values WHERE source_id=? and update_type=?", getID(), "ask" );

    return askParams;
}

void CSyncSource::syncServerChanges()
{
    String askParams = getAndremoveAsk();
    boolean bGetAtLeastOnePage = false;
    int nTry = 0;

    CTimeInterval startTime = CTimeInterval::getCurrentTime();

    while( getSync().isContinueSync() )
    {
        String strUrl = getUrl();
        String strQuery = getSync().SYNC_SOURCE_FORMAT() + "&client_id=" + getSync().getClientID() + 
                "&p_size=" + getSync().SYNC_PAGE_SIZE();

        if ( askParams.length() > 0 )
        {
            strUrl +=  getSync().SYNC_ASK_ACTION();
            strQuery += "&question=" + askParams;
        }
        //TODO: header

        if ( isEmptyToken() )
            processToken(1);

        CNetResponse oResp;
        getNet().pullData(strUrl, strQuery, oResp);

        if ( !oResp.isResponseRecieved() )
        {
            if ( nTry < getSync().MAX_SYNC_TRY_COUNT() )
                nTry++;
            else
                getSync().stopSync();
            continue;
        }else if ( !oResp.isOK() )
        {
            getSync().stopSync();
            continue;
        }

        processServerData(oResp.m_strResponse);

        if ( askParams.length() > 0 )
            break;
         nTry = 0;
    }

    CTimeInterval endTime = CTimeInterval::getCurrentTime();
    getDB().executeSQL("UPDATE sources set last_updated=?,last_inserted_size=?,last_deleted_size=?, \
						 last_sync_duration=?,last_sync_success=? WHERE source_id=?", 
                         endTime.toULong(), getInsertedCount(), getDeletedCount(), (endTime-startTime).toULong(), bGetAtLeastOnePage, getID() );
}

void CSyncSource::processServerData(const String& strData)
{
    CJSONArrayIterator oJsonArr(strData);
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

    for( ; !oJsonArr.isEnd(); oJsonArr.next() )
    {
        CJSONEntry oJsonEntry = oJsonArr.getCurItem();

        CJSONEntry oJsonObject = oJsonEntry.getEntry("object_value");
        if ( !oJsonObject.isEmpty() )
            processSyncObject(oJsonObject);
    }
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