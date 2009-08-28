#include "SyncSource.h"
#include "SyncEngine.h"

#include "common/RhoFilePath.h"
#include "common/RhoTime.h"
#include "common/StringConverter.h"
#include "json/JSONIterator.h"
#include "ruby/ext/rho/rhoruby.h"

extern "C" const char* RhoGetRootPath();

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

    m_nCurPageCount = 0;
    m_nInserted = 0;
    m_nDeleted = 0;
    m_nTotalCount = 0;
    m_bGetAtLeastOnePage = false;

    m_nErrCode = RhoRuby.ERR_NONE;
}

CSyncSource::CSyncSource(CSyncEngine& syncEngine ) : m_syncEngine(syncEngine)
{
    m_bTokenFromDB = true;

    m_nCurPageCount = 0;
    m_nInserted = 0;
    m_nDeleted = 0;
    m_nTotalCount = 0;
    m_bGetAtLeastOnePage = false;

    m_nErrCode = RhoRuby.ERR_NONE;
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

    m_nErrCode = RhoRuby.ERR_NONE;
}

CDBAdapter& CSyncSource::getDB(){ return getSync().getDB(); }
INetRequest& CSyncSource::getNet(){ return getSync().getNet(); }

void CSyncSource::sync()
{
    m_syncEngine.fireNotification(this, false, RhoRuby.ERR_NONE, "Synchronizing " + getName() + "...");

    CTimeInterval startTime = CTimeInterval::getCurrentTime();
	
    if ( m_strParams.length() == 0 )
    {
        syncClientChanges();
        getAndremoveAsk();
    }
    syncServerChanges();

    CTimeInterval endTime = CTimeInterval::getCurrentTime();
    getDB().executeSQL("UPDATE sources set last_updated=?,last_inserted_size=?,last_deleted_size=?, \
						 last_sync_duration=?,last_sync_success=? WHERE source_id=?", 
                         endTime.toULong(), getInsertedCount(), getDeletedCount(), (endTime-startTime).toULong(), m_bGetAtLeastOnePage, getID() );
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

        String strFilePath = RhoGetRootPath();
        strFilePath += "apps" + blob.getFilePath() ;

        strQuery = strBaseQuery + "&" + blob.getBody();
        NetResponse( resp, getNet().pushFile(strQuery, strFilePath) );
        if ( !resp.isOK() )
        {
            getSync().setState(CSyncEngine::esStop);
            m_nErrCode = RhoRuby.ERR_REMOTESERVER;
            //m_strError = resp.getCharData();
            return;
        }

        getDB().executeSQL("DELETE FROM object_values WHERE source_id=? and attrib_type=? and value=?", getID(), "blob.file", blob.getFilePath() );
    }

    m_arSyncBlobs.clear();
}

void CSyncSource::syncClientChanges()
{
	LOG(INFO) + "Sync client changes source ID :" + getID();

    DBResult( res , getDB().executeSQL("SELECT attrib, object, value, attrib_type, update_type "
					 "FROM object_values where source_id=? and (update_type = 'update' or update_type = 'create' or update_type = 'delete') order by update_type", getID() ) );

    String strUpdateType = "";
	m_arSyncBlobs.removeAllElements();
	m_strPushBody = "";
	
    for( ; !res.isEnd()&& getSync().isContinueSync(); res.next() )
    {
    	String strTemp  = res.getStringByIdx(4);
    	if ( strUpdateType != strTemp )
    	{
    		if ( strUpdateType.length() > 0 )
    		{
    			if ( !sendClientChanges(strUpdateType) )
    			{
                    getSync().setState(CSyncEngine::esStop);
                	continue;
    			}	
    		}
    		
    		m_strPushBody = "";
    		strUpdateType = strTemp;
    	}
    	
    	makePushBody1( res );
    }
    
    if ( getSync().isContinueSync() && strUpdateType.length() > 0 )
    	sendClientChanges(strUpdateType);
}

boolean CSyncSource::sendClientChanges(String strUpdateType)//throws Exception
{
    String strUrl = getUrl() + "/" + strUpdateType;
    strUrl += "objects";
    String strQuery = CSyncEngine::SYNC_SOURCE_FORMAT() + "&client_id=" + getSync().getClientID();

    if ( m_strPushBody.length() > 0 )
    {
	    LOG(INFO)+"Push client changes to server. Source id: " + getID() + "Size :" + m_strPushBody.length();
	    LOG(TRACE) + "Push body: " + m_strPushBody;

        NetResponse( resp, getNet().pushData(strUrl+strQuery,m_strPushBody) );
        if ( !resp.isOK() )
        {
            m_nErrCode = RhoRuby.ERR_REMOTESERVER;
            //m_strError = resp.getCharData();
            return false;
        }
    }

    if ( m_arSyncBlobs.size() > 0  )
    {
	    LOG(INFO) + "Push blobs to server. Source id: " + getID() + "Count :" + m_arSyncBlobs.size();

        getDB().executeSQL("DELETE FROM object_values WHERE source_id=? and update_type=? and (attrib_type IS NULL or attrib_type!=?)", getID(), strUpdateType, "blob.file" );
        syncClientBlobs(strUrl+strQuery);
    }else if ( m_strPushBody.length() > 0 )
        getDB().executeSQL("DELETE FROM object_values WHERE source_id=? and update_type=?", getID(), strUpdateType );
    
    return true;
}

/*
 * Construct the body of the request by filtering 
 * the attr_filter string. The body format should
 * look like the following:
 * create: attrvals[][attrib]=<name|industry>&attrvals[][object]=<locallygeneratedid>&attrvals[][value]=<some value>
 * update: attrvals[][attrib]=<name|industry>&attrvals[][object]=<remoteid>&attrvals[][value]=<some new value>
 * delete: attrvals[][attrib]=<name|industry>&attrvals[][object]=<remoteid>
 */
void CSyncSource::makePushBody1( rho::db::CDBResult& res )//throws DBException
{
    String strSrcBody = "attrvals[][attrib]=" + res.getStringByIdx(0);

    if ( res.getStringByIdx(1).length() > 0 ) 
        strSrcBody += "&attrvals[][object]=" + res.getStringByIdx(1);

    String value = res.getStringByIdx(2);
    String attribType = res.getStringByIdx(3);

    if ( value.length() > 0 )
    {
        if ( attribType == "blob.file" )
        {
            common::CFilePath oBlobPath(value);
            strSrcBody += "&attrvals[][value]=";
            strSrcBody += oBlobPath.getBaseName();
            strSrcBody += "&attrvals[][attrib_type]=blob";

            m_arSyncBlobs.addElement(new CSyncBlob(strSrcBody,value));
            return;
        }else
            strSrcBody += "&attrvals[][value]=" + value;
    }

    if ( m_strPushBody.length() > 0 )
	    m_strPushBody += "&";

    m_strPushBody += strSrcBody;
}

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
//#ifdef __APPLE__
//				value = String(RhoGetRootPath()) + "apps" + value;
//#endif
				
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
    LOG(INFO) + "Sync server changes source ID :" + getID();

    while( getSync().isContinueSync() )
    {
        setCurPageCount(0);
        String strUrl = getUrl();
        if ( m_strAction.length() > 0 )
            strUrl += '/' + m_strAction;

        String strQuery = getSync().SYNC_SOURCE_FORMAT() + "&client_id=" + getSync().getClientID() + 
                "&p_size=" + getSync().SYNC_PAGE_SIZE();
        if ( m_strParams.length() > 0 )
            strQuery += m_strParams;

        if ( getAskParams().length() > 0 )
        {
            strUrl +=  getSync().SYNC_ASK_ACTION();
            strQuery += "&question=" + getAskParams();
        }

        if ( isEmptyToken() )
            processToken(1);
        else if ( !m_bTokenFromDB && getToken() > 1 )
            strQuery += "&ack_token=" + convertToStringA(getToken());

		LOG(INFO) + "Pull changes from server. Url: " + (strUrl+strQuery);
		
        NetResponse(resp,getNet().pullData(strUrl+strQuery));
        if ( !resp.isOK() )
        {
            getSync().stopSync();
            m_nErrCode = RhoRuby.ERR_REMOTESERVER;
            //m_strError = resp.getCharData();

            continue;
        }

        processServerData(resp.getCharData());
/*		String strData =
		"[{count: 124},{total_count: 5425},{token: 123},{version: 1},"
		"{o:\"2ed2e0c7-8c4c-99c6-1b37-498d250bb8e7\",av:["
		"{i:26478681,d:1},"
		"{a:\"first_name\",i:47354289,v:\"Lars\",t:\"blob\",u:\"query\",d:0}]}]";
		//u:\"query\",
		processServerData(strData.c_str());*/

        if ( getAskParams().length() > 0 || getCurPageCount() == 0 )
            break;
    }
}

void CSyncSource::processServerData(const char* szData)
{
    CJSONArrayIterator oJsonArr(szData);
    if ( !oJsonArr.isEnd() && oJsonArr.getCurItem().hasName("error") )
    {
        m_strError = oJsonArr.getCurItem().getString("error");
        m_nErrCode = RhoRuby.ERR_CUSTOMSYNCSERVER;
        processToken(0);
        getSync().stopSync();
        return;
    }

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
        m_syncEngine.fireNotification(this, false, RhoRuby.ERR_NONE, "");

    if ( !oJsonArr.isEnd() )
    {
        processToken(oJsonArr.getCurItem().getUInt64("token"));
        oJsonArr.next();
    }else if ( getCurPageCount() == 0 )
        processToken(0);

    int nVersion = 0;
    if ( !oJsonArr.isEnd() && oJsonArr.getCurItem().hasName("version") )
    {
        nVersion = oJsonArr.getCurItem().getInt("version");
        oJsonArr.next();
    }

	LOG(INFO) + "Got " + getCurPageCount() + " records of " + getTotalCount() + " from server. Source ID: " + getID()
         + ". Version: " + nVersion;
	
    if ( !oJsonArr.isEnd() && getSync().isContinueSync() )
    {
        //TODO: support DBExceptions
        getDB().startTransaction();
        for( ; !oJsonArr.isEnd() && getSync().isContinueSync(); oJsonArr.next() )
        {
            if ( getDB().isUnlockDB() )
            {
			    LOG(INFO) + "Commit transaction because of UI request.";
                RhoRuby_RhomAttribManager_save(getID());
                getDB().endTransaction();
                getDB().startTransaction();
            }

	        CJSONEntry oJsonObject = oJsonArr.getCurItem();
	        boolean bRes = nVersion == 0 ? processSyncObject(oJsonObject) :
	        	processSyncObject_ver1(oJsonObject);
	        
	        if( !bRes)
            {
	            getSync().stopSync();
	            break;
            }

            m_bGetAtLeastOnePage = true;
        }
        RhoRuby_RhomAttribManager_save(getID());
        getDB().endTransaction();
    }

    if ( getServerObjectsCount() < getTotalCount() )
        m_syncEngine.fireNotification(this, false, RhoRuby.ERR_NONE, "");
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
    String fName = CDBAdapter::makeBlobFolderName();
	
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

	fName += "/id_" + convertToStringA(value.m_nID) + strExt;
	
	return fName;
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

    NetResponse(resp, getNet().pullFile(url, fName));
    if ( !resp.isOK() )
    {
        m_nErrCode = RhoRuby.ERR_REMOTESERVER;
        //m_strError = resp.getCharData();

        return false;
    }

    value.m_strAttrType = "blob.file";

    String strAppsPath = RhoGetRootPath();
    strAppsPath += "apps";
    value.m_strValue = fName.substr(strAppsPath.length());
    
    return true;
}

boolean CSyncSource::processSyncObject_ver1(CJSONEntry oJsonObject)//throws Exception
{
	const char* strObject = oJsonObject.getString("o");
	CJSONArrayIterator oJsonArr(oJsonObject, "av");
	
    for( ; !oJsonArr.isEnd() && getSync().isContinueSync(); oJsonArr.next() )
	{
		CJSONEntry oJsonEntry = oJsonArr.getCurItem();
        if ( oJsonEntry.isEmpty() )
        	continue;

        int nDbOp = oJsonEntry.getInt("d");
        if ( nDbOp == 0 ) //insert
        {
    	    CValue value(oJsonEntry,1);
    	    if ( !downloadBlob(value) )
    		    return false;

            String strAttrib = oJsonEntry.getString("a");
	    	String strUpdateType = "query";
	    	if( oJsonEntry.hasName("u") )
	    		strUpdateType = oJsonEntry.getString("u");

            getDB().executeSQL("INSERT INTO object_values \
                (id, attrib, source_id, object, value, update_type,attrib_type) VALUES(?,?,?,?,?,?,?)", 
                value.m_nID, strAttrib, getID(), strObject,
                value.m_strValue,strUpdateType, value.m_strAttrType );

            RhoRuby_RhomAttribManager_add_attrib(getID(),strAttrib.c_str());
            m_nInserted++;
        }else if ( nDbOp == 1 ) //delete
        {
            uint64 id = oJsonEntry.getUInt64("i");
            getDB().executeSQL("DELETE FROM object_values where id=?", id );

            RhoRuby_RhomAttribManager_delete_attribs(getID(),id);
            m_nDeleted++;
        }else{
            LOG(ERROR) + "Unknown DB operation: " + nDbOp;
        }
	}
	
	return true;
}

boolean CSyncSource::processSyncObject(CJSONEntry& oJsonObject)
{
	CJSONEntry oJsonEntry = oJsonObject.getEntry("object_value");
    if ( oJsonEntry.isEmpty() )
    	return true;

    const char* szDbOp = oJsonEntry.getString("db_operation");
    if ( szDbOp && strcmp(szDbOp,"insert")==0 )
    {
    	CValue value(oJsonEntry);
    	//
    	//value.m_strAttrType = "blob.url";
    	//value.m_strValue = "http://img.gazeta.ru/files3/661/3219661/ld.jpg";
    	//
    	if ( !downloadBlob(value) )
    		return false;

        String strAttrib = oJsonEntry.getString("attrib");
        getDB().executeSQL("INSERT INTO object_values \
            (id, attrib, source_id, object, value, update_type,attrib_type) VALUES(?,?,?,?,?,?,?)", 
            value.m_nID, strAttrib, getID(), oJsonEntry.getString("object"),
            value.m_strValue, oJsonEntry.getString("update_type"), value.m_strAttrType );

        RhoRuby_RhomAttribManager_add_attrib(getID(),strAttrib.c_str());
        m_nInserted++;
    }else if ( szDbOp && strcmp(szDbOp,"delete")==0 )
    {
        uint64 id = oJsonEntry.getUInt64("id");
        getDB().executeSQL("DELETE FROM object_values where id=?", id );

        RhoRuby_RhomAttribManager_delete_attribs(getID(),id);
        m_nDeleted++;
    }else{
        LOG(ERROR) + "Unknown DB operation: " + (szDbOp ? szDbOp : "");
    }

    return true;
}

void CSyncSource::processToken(uint64 token)
{
    if ( token > 1 && getToken() == token ){
		//Delete non-confirmed records

        setToken( token ); //For m_bTokenFromDB = false;
        getDB().executeSQL("DELETE FROM object_values where source_id=? and token=?", getID(), token );
	}else
    {
        setToken( token );
        getDB().executeSQL("UPDATE sources SET token=? where source_id=?", token, getID() );
	}

}

}
}