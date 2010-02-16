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

CSyncSource::CSyncSource() : m_syncEngine( *new CSyncEngine(*new db::CDBAdapter()))
{
    m_bTokenFromDB = true;
    m_token = 0;
    m_nCurPageCount = 0;
    m_nInserted = 0;
    m_nDeleted = 0;
    m_nTotalCount = 0;
    m_bGetAtLeastOnePage = false;
    m_eSyncServerDataPass = edpNone;

    m_nErrCode = RhoRuby.ERR_NONE;
    m_bSearchSyncChanges = false;
    m_nProgressStep = -1;
    m_nRefreshTime = 0;
}

CSyncSource::CSyncSource(CSyncEngine& syncEngine ) : m_syncEngine(syncEngine)
{
    m_bTokenFromDB = true;
    m_token = 0;

    m_nCurPageCount = 0;
    m_nInserted = 0;
    m_nDeleted = 0;
    m_nTotalCount = 0;
    m_bGetAtLeastOnePage = false;
    m_eSyncServerDataPass = edpNone;

    m_nErrCode = RhoRuby.ERR_NONE;
    m_bSearchSyncChanges = false;
    m_nProgressStep = -1;
    m_nRefreshTime = 0;
}

CSyncSource::CSyncSource(int id, const String& strUrl, const String& strName, uint64 token, CSyncEngine& syncEngine ) : m_syncEngine(syncEngine)
{
    m_nID = id;
    m_strUrl = strUrl;
    m_strName = strName;
    m_token = token;
    m_bTokenFromDB = true;

    m_nCurPageCount = 0;
    m_nInserted = 0;
    m_nDeleted = 0;
    m_nTotalCount = 0;
    m_bGetAtLeastOnePage = false;
    m_eSyncServerDataPass = edpNone;

    m_nErrCode = RhoRuby.ERR_NONE;
    m_bSearchSyncChanges = false;
    m_nProgressStep = -1;
    m_nRefreshTime = 0;
}

CDBAdapter& CSyncSource::getDB(){ return getSync().getDB(); }
INetRequest& CSyncSource::getNet(){ return getSync().getNet(); }
CSyncNotify& CSyncSource::getNotify(){ return getSync().getNotify(); }

void CSyncSource::sync()
{
    getNotify().fireSyncNotification(null, false, RhoRuby.ERR_NONE, RhoRuby.getMessageText("syncronizing") + getName() + "...");

    CTimeInterval startTime = CTimeInterval::getCurrentTime();

  	PROF_START("Pull");
    if ( isEmptyToken() )
        processToken(1);

    boolean bSyncedServer = false;
    if ( m_strParams.length() == 0 || m_bSearchSyncChanges )
    {
        if ( isPendingClientChanges() )
        {
            syncServerChanges();
            bSyncedServer = true;
        }

        if ( bSyncedServer && isPendingClientChanges() )
            getSync().setState(CSyncEngine::esStop);
        else
        {   
            boolean bSyncClient = false;
            {
                DBResult( res, getDB().executeSQL("SELECT object FROM changed_values WHERE source_id=? LIMIT 1 OFFSET 0", getID()) );
                bSyncClient = !res.isEnd();
            }
            if ( bSyncClient )
            {
                syncClientChanges();
                getAndremoveAsk();
                bSyncedServer = false;
            }
        }
    }

    PROF_STOP("Pull");

    if ( !bSyncedServer )
        syncServerChanges();

    CTimeInterval endTime = CTimeInterval::getCurrentTime();

    getDB().executeSQL("UPDATE sources set last_updated=?,last_inserted_size=?,last_deleted_size=?, \
						 last_sync_duration=?,last_sync_success=?, backend_refresh_time=? WHERE source_id=?", 
                         CLocalTime().toULong(), getInsertedCount(), getDeletedCount(), (
                         endTime-startTime).toULong(), m_bGetAtLeastOnePage, m_nRefreshTime,
                         getID() );
}

boolean CSyncSource::isPendingClientChanges()
{
    DBResult( res, getDB().executeSQL("SELECT object FROM changed_values WHERE source_id=? and update_type='create' and sent>1  LIMIT 1 OFFSET 0", getID()) );
    return !res.isEnd();
}

void CSyncSource::syncClientBlobs(const String& strBaseQuery)
{
    String strQuery;
    for( int i = 0; i < (int)m_arSyncBlobs.size(); i ++)
    {
        CSyncBlob& blob = *m_arSyncBlobs.elementAt(i);

        String strFilePath = RHODESAPP().getRhoRootPath() + "apps" + blob.getFilePath() ;

        strQuery = strBaseQuery + "&" + blob.getBody();
        NetResponse( resp, getNet().pushFile(strQuery, strFilePath, &getSync()) );
        if ( !resp.isOK() )
        {
            getSync().setState(CSyncEngine::esStop);
			if (resp.isResponseRecieved())
				m_nErrCode = RhoRuby.ERR_REMOTESERVER;
			else
				m_nErrCode = RhoRuby.ERR_NETWORK;
            return;
        }

        getDB().startTransaction();
        getDB().executeSQL("DELETE FROM changed_values WHERE source_id=? and attrib_type=? and value=?", getID(), "blob.file", blob.getFilePath() );
        getDB().executeSQL("DELETE FROM object_values WHERE source_id=? and attrib_type=? and value=?", getID(), "blob.file", blob.getFilePath() );
        getDB().endTransaction();
    }

    m_arSyncBlobs.clear();
}

void CSyncSource::syncClientChanges()
{
    const char* arUpdateTypes[] = {"create", "update", "delete"};
    for( int i = 0; i < 3 && getSync().isContinueSync(); i++ )
    {
        String strUrl = getUrl() + "/" + arUpdateTypes[i];
        strUrl += "objects";
        String strQuery = CSyncEngine::SYNC_SOURCE_FORMAT() + "&client_id=" + getSync().getClientID();

        m_arSyncBlobs.removeAllElements();
        String strBody;
        makePushBody(strBody, arUpdateTypes[i]);
        if ( strBody.length() > 0 )
        {
		    LOG(INFO) + "Push client changes to server. Source: " + getName() + "Size :" + strBody.length();
		    LOG(TRACE) + "Push body: " + strBody;		
 
            NetResponse( resp, getNet().pushData(strUrl+strQuery,strBody, &getSync()) );
            if ( !resp.isOK() )
            {
                getSync().setState(CSyncEngine::esStop);
                m_nErrCode = RhoRuby.ERR_REMOTESERVER;
                continue;
            }
        }
 
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

            syncClientBlobs(strUrl+strQuery);
        }else if ( strBody.length() > 0 )
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
    getDB().Lock();
    DBResult( res , getDB().executeSQL("SELECT attrib, object, value, attrib_type, main_id "
					 "FROM changed_values where source_id=? and update_type =? and sent<=1 ORDER BY sent DESC", getID(), szUpdateType ) );

    if ( res.isEnd() )
    {
        getDB().Unlock();
        return;
    }

    for( ; !res.isEnd(); res.next() )
    {
        String strSrcBody = "attrvals[][attrib]=" + res.getStringByIdx(0);

        if ( res.getStringByIdx(1).length() > 0 ) 
            strSrcBody += "&attrvals[][object]=" + res.getStringByIdx(1);

        uint64 main_id = res.getUInt64ByIdx(4);
        if ( main_id != 0 )
            strSrcBody += "&attrvals[][id]=" + convertToStringA(main_id);

        String value = res.getStringByIdx(2);
        String attribType = res.getStringByIdx(3);

        //if ( value.length() > 0 )
        {
            if ( attribType == "blob.file" )
            {
                common::CFilePath oBlobPath(value);
                strSrcBody += "&attrvals[][value]=";
                strSrcBody += oBlobPath.getBaseName();
                strSrcBody += "&attrvals[][attrib_type]=blob";

				if ( value.length() > 0 )
                    m_arSyncBlobs.addElement(new CSyncBlob(strSrcBody,value));
                continue;
            }else
                strSrcBody += "&attrvals[][value]=" + value;
        }

        if ( strBody.length() > 0 )
            strBody += "&";

        strBody += strSrcBody;
    }

    getDB().executeSQL("UPDATE changed_values SET sent=1 WHERE source_id=? and update_type=? and sent=0", getID(), szUpdateType );
    getDB().Unlock();
}

void CSyncSource::getAndremoveAsk()
{
    String askParams = "";
    {
        DBResult( res , getDB().executeSQL("SELECT object, attrib, value "
			     "FROM changed_values WHERE source_id=? and update_type =?", getID(), "ask" ) );
        if ( !res.isEnd() )
        {
            askParams = res.getStringByIdx(2);

            getDB().executeSQL("DELETE FROM object_values WHERE object=? and attrib=? and source_id=?", 
                res.getStringByIdx(0), res.getStringByIdx(1), getID() );
        }
    }

    getDB().executeSQL("DELETE FROM changed_values WHERE source_id=? and update_type=?", getID(), "ask" );

    setAskParams(askParams);
}

void CSyncSource::syncServerChanges()
{
    LOG(INFO) + "Sync server changes source ID :" + getID();

    while( getSync().isContinueSync() )
    {
        setCurPageCount(0);
        String strUrl = getUrl();
        if ( m_strAction.length() > 0 )
            strUrl += '/' + m_strAction;

        String strQuery = getSync().SYNC_SOURCE_FORMAT() + "&client_id=" + getSync().getClientID() + 
                "&p_size=" + getSync().SYNC_PAGE_SIZE() + "&version=" + convertToStringA(getSync().SYNC_VERSION());
        if ( m_strParams.length() > 0 )
            strQuery += m_strParams;
        if( m_strUrlParams.length() > 0 )
	        strQuery += "&" + m_strUrlParams;

        if ( getAskParams().length() > 0 )
        {
            strUrl +=  getSync().SYNC_ASK_ACTION();
            strQuery += "&question=" + getAskParams();
        }

        if ( !m_bTokenFromDB && getToken() > 1 )
            strQuery += "&ack_token=" + convertToStringA(getToken());

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

        processServerData(resp.getCharData());

		//String strData =
        //"[{count:10},{version:1},{total_count: 5425},{token: 123},{s:\"RhoDeleteSource\",ol:[{o:\"rho_del_obj\",av:[{i:55550425},{i:75665819},{i:338165272},{i:402396629},{i:521753981},{i:664143530},{i:678116186},{i:831092394},{i:956041217},{i:970452458}]}]}]";
		/*"[{count: 124},{version: 1},{total_count: 5425},{token: 123},"
        "{s:\"Product\",ol:["
		"{oo:\"123\",o:\"2ed2e0c7-8c4c-99c6-1b37-498d250bb8e7\",av:["
		"{a:\"first_name\",i:47354289,v:\"Lars. \n\n Burgess\", t:\"blob\"},"
        "{a:\"second_name\",i:55555,v:\"Burgess\"}]},"
        "{oo:\"456\", e:\"Something went wrong creating this record on the backend: code 7\"}"
        "]}]"; */
		/*"[{count: 1},{version: 1},{total_count: 1},{token: 123},"
        "{s:\"Product\",ol:["
        "{oo:\"94\", e:\"Something went wrong creating this record on the backend: code 7\"}"
        "]}]";*/
		/*"[{count: 1},{version: 1},{total_count: 1},{token: 123},"
        "{s:\"Product\",ol:["
        "{o:\"94\", av:["
        "{a:\"TEST\",i:55555,v:\"Geny\"}]},"
        "]}]";

		//u:\"query\",  
		processServerData(strData.c_str()); */

        if ( getAskParams().length() > 0 || getCurPageCount() == 0 )
            break;
    }
}

void CSyncSource::processServerData(const char* szData)
{
    PROF_START("Parse");
    CJSONArrayIterator oJsonArr(szData);
    PROF_STOP("Parse");
    PROF_START("Data1");
    if ( !oJsonArr.isEnd() && oJsonArr.getCurItem().hasName("error") )
    {
        m_strError = oJsonArr.getCurItem().getString("error");
        m_nErrCode = RhoRuby.ERR_CUSTOMSYNCSERVER;
        getSync().stopSync();
        return;
    }

    if ( !oJsonArr.isEnd() )
    {
        setCurPageCount(oJsonArr.getCurItem().getInt("count"));
        oJsonArr.next();
    }
    int nVersion = 0;
    if ( !oJsonArr.isEnd() && oJsonArr.getCurItem().hasName("version") )
    {
        nVersion = oJsonArr.getCurItem().getInt("version");
        oJsonArr.next();
    }

    if ( !oJsonArr.isEnd() && oJsonArr.getCurItem().hasName("rt") )
    {
        setRefreshTime(oJsonArr.getCurItem().getInt("rt"));
        oJsonArr.next();
    }

    if ( !oJsonArr.isEnd() && oJsonArr.getCurItem().hasName("total_count") )
    {
        setTotalCount(oJsonArr.getCurItem().getInt("total_count"));
        oJsonArr.next();
    }
    //if ( getServerObjectsCount() == 0 )
    //    getNotify().fireSyncNotification(this, false, RhoRuby.ERR_NONE, "");

    if ( !oJsonArr.isEnd() )
    {
        processToken(oJsonArr.getCurItem().getUInt64("token"));
        oJsonArr.next();
    }else if ( getCurPageCount() == 0 )
    {
        //oo conflicts
        getDB().executeSQL("DELETE FROM changed_values where source_id=? and sent>=3", getID() );
        //
        processToken(0);
    }

	LOG(INFO) + "Got " + getCurPageCount() + "(Processed: " +  getServerObjectsCount() + ") records of " + getTotalCount() + " from server. Source: " + getName()
         + ". Version: " + nVersion;

    PROF_STOP("Data1");
    if ( !oJsonArr.isEnd() && getSync().isContinueSync() )
    {
        PROF_START("Data");
        //TODO: support DBExceptions
        getDB().startTransaction();

        int nSavedPos = oJsonArr.getCurPos();
        setSyncServerDataPass(edpNone);
        processServerData_Ver1(oJsonArr);

        setSyncServerDataPass(edpDeleteObjects);
        oJsonArr.reset(nSavedPos);
        processServerData_Ver1(oJsonArr);

	    PROF_STOP("Data");		    
    	PROF_START("DB");
        getDB().endTransaction();
	    PROF_STOP("DB");

        getNotify().fireObjectsNotification();
    }

	PROF_START("Data1");
    if ( getCurPageCount() > 0 )
        getNotify().fireSyncNotification(this, false, RhoRuby.ERR_NONE, "");
	PROF_STOP("Data1");
}

boolean CSyncSource::processSyncObject_ver1(CJSONEntry oJsonObject, int nSrcID)//throws Exception
{
    const char* strOldObject = oJsonObject.getString("oo");
    if ( isDeleteObjectsPass() != (nSrcID < 0) )
        return true;

    if ( oJsonObject.hasName("e") )
    {
        const char* strError = oJsonObject.getString("e");
        getNotify().addCreateObjectError(nSrcID,strOldObject,strError);
        return true;
    }

	const char* strObject = oJsonObject.getString("o");
	CJSONArrayIterator oJsonArr(oJsonObject, "av");
    //oo conflicts
    boolean bUpdatedOO = false;
    //
    for( ; !oJsonArr.isEnd() && getSync().isContinueSync(); oJsonArr.next() )
	{
		CJSONEntry oJsonEntry = oJsonArr.getCurItem();
        if ( oJsonEntry.isEmpty() )
        	continue;

        if ( nSrcID >= 0 ) //insert
        {
    	    CValue value(oJsonEntry,1);
    	    if ( !downloadBlob(value) )
    		    return false;

            String strAttrib = oJsonEntry.getString("a");
            //oo conflicts
            if ( strOldObject != null && !bUpdatedOO )
            {
                getDB().executeSQL("UPDATE object_values SET object=? where object=? and source_id=?", strObject, strOldObject, nSrcID );
                getDB().executeSQL("UPDATE changed_values SET object=? where object=? and source_id=?", strObject, strOldObject, nSrcID );

                getNotify().onObjectChanged(nSrcID,strOldObject, CSyncNotify::enCreate);

                bUpdatedOO = true;
            }

            DBResult(resInsert, getDB().executeSQLReportNonUnique("INSERT INTO object_values \
                (id, attrib, source_id, object, value, attrib_type) VALUES(?,?,?,?,?,?)", 
                value.m_nID, strAttrib, nSrcID, strObject,
                value.m_strValue, value.m_strAttrType ) );
            if ( resInsert.isNonUnique() )
            {
                getDB().executeSQL("UPDATE object_values \
                    SET id=?, value=?, attrib_type=? WHERE object=? and attrib=? and source_id=?", 
                    value.m_nID, value.m_strValue, value.m_strAttrType,
                    strObject, strAttrib, nSrcID );

                // oo conflicts
                getDB().executeSQL("UPDATE changed_values SET main_id=? where object=? and attrib=? and source_id=? and sent<=1", value.m_nID, strObject, strAttrib, nSrcID );
                getDB().executeSQL("UPDATE changed_values SET sent=4 where object=? and attrib=? and source_id=? and sent>1", strObject, strAttrib, nSrcID );
                //
            }

            getNotify().onObjectChanged(nSrcID,strObject, CSyncNotify::enUpdate);

            m_nInserted++;
        }else
        {
            uint64 id = oJsonEntry.getUInt64("i");
            DBResult( res , getDB().executeSQL("SELECT source_id, object FROM object_values where id=?", id ));
            if ( !res.isEnd() )
            {
                int nDelSrcID = res.getIntByIdx(0);
                String strDelObject = res.getStringByIdx(1);
                getDB().executeSQL("DELETE FROM object_values where id=?", id );
                getNotify().onObjectChanged(nDelSrcID, strDelObject, CSyncNotify::enDelete);
            }
            // oo conflicts
            getDB().executeSQL("UPDATE changed_values SET sent=3 where main_id=?", id );
            //

            m_nDeleted++;
        }
	}
	
	return true;
}

void CSyncSource::processServerData_Ver1(CJSONArrayIterator& oJsonArr)
{
    for( ; !oJsonArr.isEnd() && getSync().isContinueSync(); oJsonArr.next() )
    {
        CJSONEntry oJsonSource = oJsonArr.getCurItem();
        String strSrcName = oJsonSource.getString("s");
        int nSrcID = getID();
        if ( strSrcName.compare("RhoDeleteSource") == 0 )
            nSrcID = -1;
        else if ( strSrcName.compare(getName()) != 0 )
        {
            CSyncSource* pSrc = getSync().findSourceByName(strSrcName);
            if ( pSrc == null )
            {
                LOG(ERROR) + "Sync server send data for unknown source name:" + strSrcName;
                getSync().stopSync();
                m_nErrCode = RhoRuby.ERR_UNEXPECTEDSERVERRESPONSE;
                break;
            }
            nSrcID = pSrc->getID();
        }

        CJSONArrayIterator oJsonObjList(oJsonSource, "ol");
        for( ; !oJsonObjList.isEnd() && getSync().isContinueSync(); oJsonObjList.next() )
        {
            if ( getDB().isUnlockDB() )
            {
		        LOG(INFO) + "Commit transaction because of UI request.";
                getDB().endTransaction();
                getDB().startTransaction();
            }

            CJSONEntry oJsonObject = oJsonObjList.getCurItem();
            if( !processSyncObject_ver1(oJsonObject,nSrcID))
            {
                getSync().stopSync();
                break;
            }

            if ( !isDeleteObjectsPass() && nSrcID >=0 )
            {
                int nSyncObjectCount  = getNotify().incLastSyncObjectCount(nSrcID);

                if ( getProgressStep() > 0 && (nSyncObjectCount%getProgressStep() == 0) )
                    getNotify().fireSyncNotification(this, false, RhoRuby.ERR_NONE, "");
            }

            m_bGetAtLeastOnePage = true;
        }
    }
}

CValue::CValue(json::CJSONEntry& oJsonEntry)//throws JSONException
{
	m_strValue = oJsonEntry.getString("value");
	const char* szAttribType = oJsonEntry.getString("attrib_type");
    m_strAttrType = szAttribType ? szAttribType : "";
	m_nID = oJsonEntry.getUInt64("id");
}

CValue::CValue(json::CJSONEntry& oJsonEntry, int nVer)//throws JSONException
{
    if ( nVer == 1 )
    {
	    m_strValue = oJsonEntry.getString("v");
	    const char* szAttribType = oJsonEntry.getString("t");
        m_strAttrType = szAttribType ? szAttribType : "";
	    m_nID = oJsonEntry.getUInt64("i");
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

    NetResponse(resp, getNet().pullFile(url, fName, &getSync()));
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
