#include "SyncSource.h"
#include "SyncEngine.h"

#include "common/RhoFilePath.h"
#include "common/RhoTime.h"
#include "common/StringConverter.h"
#include "common/RhodesApp.h"
#include "json/JSONIterator.h"
#include "ruby/ext/rho/rhoruby.h"
#include "statistic/RhoProfiler.h"

namespace rho {
namespace sync {
IMPLEMENT_LOGCLASS(CSyncSource,"Sync");

using namespace rho::net;
using namespace rho::db;
using namespace rho::common;
using namespace rho::json;
/*
CSyncSource::CSyncSource() : m_syncEngine( *new CSyncEngine(*new db::CDBAdapter()))
{
    m_bTokenFromDB = true;

    m_nCurPageCount = 0;
    m_nInserted = 0;
    m_nDeleted = 0;
    m_nTotalCount = 0;
    m_bGetAtLeastOnePage = false;

    m_nErrCode = RhoRuby.ERR_NONE;

    m_bIsSearch = false;
}
*/
CSyncSource::CSyncSource(CSyncEngine& syncEngine, db::CDBAdapter& db  ) : m_syncEngine(syncEngine), m_dbAdapter(db)
{
    m_bTokenFromDB = true;

    m_nCurPageCount = 0;
    m_nInserted = 0;
    m_nDeleted = 0;
    m_nTotalCount = 0;
    m_bGetAtLeastOnePage = false;

    m_nErrCode = RhoRuby.ERR_NONE;
    m_bIsSearch = false;
}

CSyncSource::CSyncSource(int id, const String& strName, uint64 token, const String& strSyncType, db::CDBAdapter& db, CSyncEngine& syncEngine ) : m_syncEngine(syncEngine), m_dbAdapter(db)
{
    m_nID = id;
    m_strName = strName;
    m_token = token;
    m_strSyncType = strSyncType;
    m_bTokenFromDB = true;

    m_nCurPageCount = 0;
    m_nInserted = 0;
    m_nDeleted = 0;
    m_nTotalCount = 0;
    m_bGetAtLeastOnePage = false;

    m_nErrCode = RhoRuby.ERR_NONE;
    m_bIsSearch = false;
}

INetRequest& CSyncSource::getNet(){ return getSync().getNet(); }
CSyncNotify& CSyncSource::getNotify(){ return getSync().getNotify(); }
ISyncProtocol& CSyncSource::getProtocol(){ return getSync().getProtocol(); }

void CSyncSource::sync()
{
    getNotify().fireSyncNotification(null, false, RhoRuby.ERR_NONE, RhoRuby.getMessageText("syncronizing") + getName() + "...");

    CTimeInterval startTime = CTimeInterval::getCurrentTime();
    m_bIsSearch = false;

    if ( isEmptyToken() )
        processToken(1);

    boolean bSyncedServer = syncClientChanges();
    if ( !bSyncedServer )
        syncServerChanges();

    CTimeInterval endTime = CTimeInterval::getCurrentTime();

    getDB().executeSQL("UPDATE sources set last_updated=?,last_inserted_size=?,last_deleted_size=?, \
						 last_sync_duration=?,last_sync_success=?, backend_refresh_time=? WHERE source_id=?", 
                         CLocalTime().toULong(), getInsertedCount(), getDeletedCount(), (
                         endTime-startTime).toULong(), m_bGetAtLeastOnePage, m_nRefreshTime,
                         getID() );
}

boolean CSyncSource::syncClientChanges()
{
    boolean bSyncedServer = false;
    if ( isPendingClientChanges() )
    {
        syncServerChanges();
        bSyncedServer = true;
    }

    if ( bSyncedServer && isPendingClientChanges() )
        getSync().setState(CSyncEngine::esStop);
    else
    {   
      	PROF_START("Pull");

        boolean bSyncClient = false;
        {
            DBResult( res, getDB().executeSQL("SELECT object FROM changed_values WHERE source_id=? LIMIT 1 OFFSET 0", getID()) );
            bSyncClient = !res.isEnd();
        }
        if ( bSyncClient )
        {
            doSyncClientChanges();
            bSyncedServer = false;
        }

        PROF_STOP("Pull");
    }

    return bSyncedServer;
}

boolean CSyncSource::isPendingClientChanges()
{
    DBResult( res, getDB().executeSQL("SELECT object FROM changed_values WHERE source_id=? and update_type='create' and sent>1  LIMIT 1 OFFSET 0", getID()) );
    return !res.isEnd();
}

void CSyncSource::syncClientBlobs(const String& strBaseQuery)
{
    for( int i = 0; i < (int)m_arSyncBlobs.size(); i ++)
    {
        CSyncBlob& blob = *m_arSyncBlobs.elementAt(i);

        String strFilePath = blob.getFilePath().length() > 0 ? RHODESAPP().getRhoRootPath() + "apps" + blob.getFilePath() : "";

        String strQuery = strBaseQuery + "&" + blob.getBody();
        NetResponse( resp, getNet().pushFile( strQuery, strFilePath, &getSync(), null) );
        if ( !resp.isOK() )
        {
            getSync().setState(CSyncEngine::esStop);
			if (resp.isResponseRecieved())
				m_nErrCode = RhoRuby.ERR_REMOTESERVER;
			else
				m_nErrCode = RhoRuby.ERR_NETWORK;
            return;
        }

        getDB().executeSQL("DELETE FROM object_values WHERE source_id=? and attrib_type=? and value=?", getID(), "blob.file", blob.getFilePath() );
    }

    m_arSyncBlobs.clear();
}

void CSyncSource::doSyncClientChanges()
{
    String arUpdateTypes[] = {"create", "update", "delete"};
    boolean arUpdateSent[] = {false, false, false};

    m_arSyncBlobs.removeAllElements();
    String strBody = "{\"source_name\":\"" + getName() + "\",\"client_id\":\"" + getSync().getClientID() + "\"";
    boolean bSend = false;
    for( int i = 0; i < 3 && getSync().isContinueSync(); i++ )
    {
        String strBody1;
        makePushBody_Ver3(strBody1, arUpdateTypes[i]);
        if (strBody1.length() > 0)
        {
            strBody += "," + strBody1;
            arUpdateSent[i] = true;
            bSend = true;
        }
    }
    strBody += "}";

    if ( bSend )
    {
        LOG(INFO) + "Push client changes to server. Source: " + getName() + "Size :" + strBody.length();
        LOG(TRACE) + "Push body: " + strBody;		

        NetResponse( resp, getNet().pushData(
            getProtocol().getClientChangesUrl(getName(), "", getSync().getClientID()),strBody, &getSync()) );
        if ( !resp.isOK() )
        {
            getSync().setState(CSyncEngine::esStop);
            m_nErrCode = RhoRuby.ERR_REMOTESERVER;
        }
    }

    afterSyncClientChanges(arUpdateSent);
}

void CSyncSource::afterSyncClientChanges(boolean arUpdateSent[])
{
    const char* arUpdateTypes[] = {"create", "update", "delete"};
    for( int i = 0; i < 3 && getSync().isContinueSync(); i++ )
    {
        if ( m_arSyncBlobs.size() > 0 )
        {
		    LOG(INFO) + "Push blobs to server. Source: " + getName() + "Count :" + m_arSyncBlobs.size();
            //oo conflicts
            if ( i < 1 ) //create
                getDB().executeSQL("UPDATE changed_values SET sent=2 WHERE source_id=? and update_type=? and (attrib_type IS NULL or attrib_type!=?) and sent=1", 
                    getID(), arUpdateTypes[i], "blob.file" );
            else
            //
                getDB().executeSQL("DELETE FROM changed_values WHERE source_id=? and update_type=? and (attrib_type IS NULL or attrib_type!=? and sent=1)", 
                    getID(), arUpdateTypes[i], "blob.file" );

            //TODO: sync blobs ver3
            syncClientBlobs(getProtocol().getClientChangesUrl(getName(), arUpdateTypes[i], getSync().getClientID()));
        }else if ( arUpdateSent[i] )
        {
            //oo conflicts
            if ( i < 1 ) //create
                getDB().executeSQL("UPDATE changed_values SET sent=2 WHERE source_id=? and update_type=? and sent=1", getID(), arUpdateTypes[i] );
            else
            //
                getDB().executeSQL("DELETE FROM changed_values WHERE source_id=? and update_type=? and sent=1", getID(), arUpdateTypes[i] );
        }
    }
}

//{"source_name":"SampleAdapter","client_id":1,"create":{"1":{"brand":"Apple","name":"iPhone","price":"199.99"}}}
//{"source_name":"SampleAdapter","client_id":1,"update":{"1":{"brand":"Apple","name":"iPhone","price":"199.99"}}}
//{"source_name":"SampleAdapter","client_id":1,"delete":{"1":{"brand":"Apple","name":"iPhone","price":"199.99"}}}
//{"source_name":"SampleAdapter","client_id":1,"delete":{"3":{"brand":"HTC","name":"Fuze","price":"299.99"}},"create":{"1":{"brand":"Apple","name":"iPhone","price":"199.99"}},"update":{"2":{"brand":"Android","name":"G2","price":"99.99"}}}
void CSyncSource::makePushBody_Ver3(String& strBody, const String& strUpdateType)
{
    getDB().Lock();
    DBResult( res , getDB().executeSQL("SELECT attrib, object, value, attrib_type "
        "FROM changed_values where source_id=? and update_type =? and sent<=1 ORDER BY object", getID(), strUpdateType.c_str() ) );

    if ( res.isEnd() )
    {
        getDB().Unlock();
        return;
    }

    String strCurObject = "";
    boolean bFirst = true;
    for( ; !res.isEnd(); res.next() )
    {
        String strAttrib = res.getStringByIdx(0);
        String strObject = res.getStringByIdx(1);
        String value = res.getStringByIdx(2);
        String attribType = res.getStringByIdx(3);

        if ( attribType.compare("blob.file") == 0 )
        {
            common::CFilePath oBlobPath(value);

            String strBlobBody = "{\"source_name\":\"" + getName() + "\",\"client_id\":\"" + getSync().getClientID() + "\",";
            strBlobBody += "\"" + strUpdateType + "\":{";
            strBlobBody += "\"" + strObject + "\":{";
            strBlobBody += "\"" + strAttrib + "\":\"" + oBlobPath.getBaseName() + "\"";
            //TODO: attrib_type set to blob?
            //strSrcBody += "&attrvals[][attrib_type]=blob";

            strBlobBody += "}}}";

            m_arSyncBlobs.addElement(new CSyncBlob(strBlobBody,value));

            continue;
        }

        if ( strBody.length() == 0 )
            strBody += "\"" + strUpdateType + "\":{";

        if ( strObject.compare(strCurObject) != 0 )
        {
            if ( strCurObject.length() > 0 )
            {
                if ( !bFirst )
                    strBody += "}";
                strBody += ",";
            }

            bFirst = true;
            strBody += "\"" + strObject + "\"";
            strCurObject = strObject;
        }
            
        if (!bFirst)
            strBody += ",";

        if ( strAttrib.length() > 0  )
        {
            if ( bFirst )
                strBody += ":{";

            strBody += "\"" + strAttrib + "\":\"" + value + "\"";
            bFirst = false;
        }
    }

    if ( strBody.length() > 0 )
    {
        if ( !bFirst )
            strBody += "}";

        strBody += "}";
    }

    getDB().executeSQL("UPDATE changed_values SET sent=1 WHERE source_id=? and update_type=? and sent=0", getID(), strUpdateType.c_str() );
    getDB().Unlock();
}

void CSyncSource::syncServerChanges()
{
    LOG(INFO) + "Sync server changes source ID :" + getID();

    while( getSync().isContinueSync() )
    {
        setCurPageCount(0);
        String strUrl = getProtocol().getServerQueryUrl("");
        String strQuery = getProtocol().getServerQueryBody(getName(), getSync().getClientID(), getSync().getSyncPageSize());

        if ( !m_bTokenFromDB && getToken() > 1 )
            strQuery += "&token=" + convertToStringA(getToken());

		LOG(INFO) + "Pull changes from server. Url: " + (strUrl+strQuery);
        PROF_START("Net");	    
        NetResponse(resp,getNet().pullData(strUrl+strQuery, &getSync()));
		PROF_STOP("Net");

        if ( !resp.isOK() )
        {
            getSync().stopSync();
			if (resp.isResponseRecieved())
				m_nErrCode = RhoRuby.ERR_REMOTESERVER;
			else
				m_nErrCode = RhoRuby.ERR_NETWORK;
            continue;
        }

        const char* szData = resp.getCharData();
        
        //const char* szData = "[{\"version\":3},{\"token\":\"35639160294387\"},{\"count\":3},{\"progress_count\":0},{\"total_count\":3},{\"metadata\":\"{\\\"foo\\\":\\\"bar\\\"}\",\"insert\":{\"1\":{\"price\":\"199.99\",\"brand\":\"Apple\",\"name\":\"iPhone\"}}}]";

        PROF_START("Parse");
        CJSONArrayIterator oJsonArr(szData);
        PROF_STOP("Parse");

        processServerResponse_ver3(oJsonArr);

        if ( getToken() == 0 )
            break;
    }
}

void CSyncSource::processServerResponse_ver3(CJSONArrayIterator& oJsonArr)
{
    PROF_START("Data1");

    int nVersion = 0;
    if ( !oJsonArr.isEnd() && oJsonArr.getCurItem().hasName("version") )
    {
        nVersion = oJsonArr.getCurItem().getInt("version");
        oJsonArr.next();
    }

    if ( nVersion != getProtocol().getVersion() )
    {
        LOG(ERROR) + "Sync server send data with incompatible version. Client version: " + convertToStringA(getProtocol().getVersion()) +
            "; Server response version: " + convertToStringA(nVersion) + ". Source name: " + getName();
        getSync().stopSync();
        m_nErrCode = RhoRuby.ERR_SYNCVERSION;
        return;
    }

    if ( !oJsonArr.isEnd() && oJsonArr.getCurItem().hasName("source") )
    {
        //skip it. it uses in search only
        oJsonArr.next();
    }

    if ( !oJsonArr.isEnd() && oJsonArr.getCurItem().hasName("token"))
    {
        processToken(oJsonArr.getCurItem().getUInt64("token"));
        oJsonArr.next();
    }

    if ( !oJsonArr.isEnd() && oJsonArr.getCurItem().hasName("count") )
    {
        setCurPageCount(oJsonArr.getCurItem().getInt("count"));
        oJsonArr.next();
    }

    if ( !oJsonArr.isEnd() && oJsonArr.getCurItem().hasName("refresh_time") )
    {
        setRefreshTime(oJsonArr.getCurItem().getInt("refresh_time"));
        oJsonArr.next();
    }

    if ( !oJsonArr.isEnd() && oJsonArr.getCurItem().hasName("progress_count") )
    {
        //TODO: progress_count
        //setTotalCount(oJsonArr.getCurItem().getInt("progress_count"));
        oJsonArr.next();
    }

    if ( !oJsonArr.isEnd() && oJsonArr.getCurItem().hasName("total_count") )
    {
        setTotalCount(oJsonArr.getCurItem().getInt("total_count"));
        oJsonArr.next();
    }

   /* if ( !oJsonArr.isEnd() && oJsonArr.getCurItem().hasName("source-error") )
    {
        CJSONEntry oJsonErr = oJsonArr.getCurItem().getEntry("source-error");
        m_strError = oJsonErr.getString("message");
        m_nErrCode = RhoRuby.ERR_CUSTOMSYNCSERVER;
        getSync().stopSync();
        return;
    }*/

    //if ( getServerObjectsCount() == 0 )
    //    getNotify().fireSyncNotification(this, false, RhoRuby.ERR_NONE, "");

    if ( getToken() == 0 )
    {
        //oo conflicts
        getDB().executeSQL("DELETE FROM changed_values where source_id=? and sent>=3", getID() );
        //

    }

	LOG(INFO) + "Got " + getCurPageCount() + "(Processed: " +  getServerObjectsCount() + ") records of " + getTotalCount() + " from server. Source: " + getName()
         + ". Version: " + nVersion;

    PROF_STOP("Data1");
    if ( !oJsonArr.isEnd() && getSync().isContinueSync() )
    {
        CJSONStructIterator iterCmds(oJsonArr.getCurItem());
        if ( !iterCmds.isEnd() )
        {
            PROF_START("Data");
            //TODO: support DBExceptions
            getDB().startTransaction();

            for( ; !iterCmds.isEnd() && getSync().isContinueSync(); iterCmds.next() )
            {
                String strCmd = iterCmds.getCurKey();

                if ( strCmd.compare("metadata") == 0 )
                {
                    String strMetadata = iterCmds.getCurValue().getString();
                    getDB().executeSQL("UPDATE sources SET metadata=? WHERE source_id=?", strMetadata, getID() );
                }else if ( strCmd.compare("links") == 0 || strCmd.compare("delete") == 0 || strCmd.compare("insert") == 0)
                {
                    CJSONStructIterator objIter(iterCmds.getCurValue());

                    for( ; !objIter.isEnd() && getSync().isContinueSync(); objIter.next() )
                    {
                        String strObject = objIter.getCurKey();
                        CJSONStructIterator attrIter( objIter.getCurValue() );
                        for( ; !attrIter.isEnd() && getSync().isContinueSync(); attrIter.next() )
                        {
                            String strAttrib = attrIter.getCurKey();
                            String strValue = attrIter.getCurValue().getString();

                            processServerCmd_Ver3(strCmd,strObject,strAttrib,strValue);
                        }

                        int nSyncObjectCount  = getNotify().incLastSyncObjectCount(getID());
                        if ( getProgressStep() > 0 && (nSyncObjectCount%getProgressStep() == 0) )
                            getNotify().fireSyncNotification(this, false, RhoRuby.ERR_NONE, "");
                    }
                }
            }

	        PROF_STOP("Data");		    
    	    PROF_START("DB");
            getDB().endTransaction();
	        PROF_STOP("DB");

            getNotify().fireObjectsNotification();
        }
    }

	PROF_START("Data1");
    if ( getCurPageCount() > 0 )
        getNotify().fireSyncNotification(this, false, RhoRuby.ERR_NONE, "");
	PROF_STOP("Data1");
}

void CSyncSource::processServerCmd_Ver3(const String& strCmd, const String& strObject, const String& strAttrib, const String& strValue)//throws Exception
{
    if ( strCmd.compare("insert") == 0 )
    {
        //TODO: blobs
        //CValue value(oJsonEntry,1);
        //if ( !downloadBlob(value) )
	    //    return false;
        
        DBResult(resInsert, getDB().executeSQLReportNonUnique("INSERT INTO object_values \
            (attrib, source_id, object, value, attrib_type) VALUES(?,?,?,?,?)", 
             strAttrib, getID(), strObject, strValue, "" ) );
        if ( resInsert.isNonUnique() )
        {
            getDB().executeSQL("UPDATE object_values \
                SET value=?, attrib_type=? WHERE object=? and attrib=? and source_id=?", 
                strValue, "", strObject, strAttrib, getID() );

            // oo conflicts
            getDB().executeSQL("UPDATE changed_values SET sent=4 where object=? and attrib=? and source_id=? and sent>1", strObject, strAttrib, getID() );
            //
        }

        getNotify().onObjectChanged(getID(),strObject, CSyncNotify::enUpdate);
        m_nInserted++;
    }else if (strCmd.compare("delete") == 0)
    {
        getDB().executeSQL("DELETE FROM object_values where object=? and attrib=? and source_id=?", strObject, strAttrib, getID() );
        getNotify().onObjectChanged(getID(), strObject, CSyncNotify::enDelete);
        // oo conflicts
        getDB().executeSQL("UPDATE changed_values SET sent=3 where object=? and attrib=? and source_id=?", strObject, strAttrib, getID() );
        //

        m_nDeleted++;
    }else if ( strCmd.compare("links") == 0 )
    {
        getDB().executeSQL("UPDATE object_values SET object=? where object=? and source_id=?", strValue, strObject, getID() );
        getDB().executeSQL("UPDATE changed_values SET object=?,sent=3 where object=? and source_id=?", strValue, strObject, getID() );

        getNotify().onObjectChanged(getID(), strObject, CSyncNotify::enCreate);
    }

}

String CSyncSource::makeFileName(const CValue& value)//throws Exception
{
	String strExt = ".bin";

    const char* url = value.m_strValue.c_str();
    const char* quest = strchr(url,'?');
    char szExt[20];
    szExt[0] = 0;
    if (quest){
        const char* extStart = strstr(quest,"extension=");
        if ( extStart ){
            const char* extEnd = strstr(extStart,"&");
            if (extEnd){
                int nExtLen = extEnd-(extStart+10);
                strncpy(szExt,extStart+10,nExtLen);
                szExt[nExtLen] = 0;
            }
            else
                strcpy(szExt,extStart+10);
        }
    }

    if ( !szExt[0] ){
        const char* dot = strrchr(url,'.');
        //TODO: process :http://img.lenta.ru/news/2009/03/11/acid/picture.jpg?test=.img
        if (dot){
            if (quest){
                if(quest>dot){
                    strncpy(szExt,dot,quest-dot);
                    szExt[quest-dot] = 0;
                }
            }
            else
                strcpy(szExt,dot);
        }
    }

    if ( szExt[0] )
        strExt = szExt;

	String fName = RHODESAPP().getBlobsDirPath() + "/id_" + convertToStringA(value.m_nID) + strExt;
	
	return  fName;
}

boolean CSyncSource::downloadBlob(CValue& value)//throws Exception
{
	if ( value.m_strAttrType != "blob.url"  )
		return true;
	
	String fName = makeFileName( value );
	String url = value.m_strValue;
	const char* nQuest = strchr(url.c_str(),'?');
	if ( nQuest > 0 )
		url += "&";
	else
		url += "?";
	url += "client_id=" + getSync().getClientID();

    NetResponse(resp, getNet().pullFile(url, fName, &getSync(), null));
    if ( !resp.isOK() )
    {
		if (resp.isResponseRecieved())
			m_nErrCode = RhoRuby.ERR_REMOTESERVER;
		else
			m_nErrCode = RhoRuby.ERR_NETWORK;
        //m_strError = resp.getCharData();

        return false;
    }

    value.m_strAttrType = "blob.file";

    String strAppsPath = RHODESAPP().getRhoRootPath() + "apps";
    value.m_strValue = fName.substr(strAppsPath.length());
    
    return true;
}

void CSyncSource::processToken(uint64 token)
{
    if ( token > 1 && getToken() == token ){
		//Delete non-confirmed records

        setToken( token ); //For m_bTokenFromDB = false;
        //getDB().executeSQL("DELETE FROM object_values where source_id=? and token=?", getID(), token );
        //TODO: add special table for id,token
	}else
    {
        setToken( token );
        getDB().executeSQL("UPDATE sources SET token=? where source_id=?", token, getID() );
	}

}

}
}
