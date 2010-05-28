#include "SyncEngine.h"
#include "SyncSource.h"
#include "SyncThread.h"

#include "json/JSONIterator.h"
#include "common/RhoConf.h"
#include "common/StringConverter.h"
#include "sync/ClientRegister.h"
#include "net/URI.h"
#include "statistic/RhoProfiler.h"
#include "rubyext/RhoRuby.h"
#include "common/RhoTime.h"
#include "common/RhoFilePath.h"
#include "common/RhoFile.h"
#include "SyncProtocol_3.h"
#include "net/URI.h"

namespace rho {
namespace sync {
IMPLEMENT_LOGCLASS(CSyncEngine,"Sync");

using namespace rho::net;
using namespace rho::common;
using namespace rho::json;

CSyncEngine::CSyncEngine():m_NetRequest(0), m_syncState(esNone), m_oSyncNotify(*this)
{
    m_bNoThreaded = false;
    m_bStopByUser = false;
    m_nSyncPageSize = 2000;

    initProtocol();
}

void CSyncEngine::initProtocol()
{
    m_SyncProtocol = new CSyncProtocol_3();
}

void CSyncEngine::prepareSync(ESyncState eState)
{
    setState(eState);
    m_bStopByUser = false;
    m_nErrCode = RhoRuby.ERR_NONE;

    loadAllSources();

    m_strSession = loadSession();
    if ( isSessionExist()  )
    {
        m_clientID = loadClientID();
        if ( m_nErrCode == RhoRuby.ERR_NONE )
        {
            getNotify().cleanLastSyncObjectCount();
   	        doBulkSync();

            return;
        }
    }else
        m_nErrCode = RhoRuby.ERR_CLIENTISNOTLOGGEDIN;

    if ( m_sources.size() > 0 )
    {
        CSyncSource& src = *m_sources.elementAt(getStartSource());
        src.m_nErrCode = m_nErrCode;
        getNotify().fireSyncNotification(&src, true, src.m_nErrCode, "");
    }else
        getNotify().fireSyncNotification(null, true, m_nErrCode, "");

    stopSync();
}

void CSyncEngine::doSyncAllSources()
{
    prepareSync(esSyncAllSources);

    if ( isContinueSync() )
    {
	    PROF_CREATE_COUNTER("Net");	    
	    PROF_CREATE_COUNTER("Parse");
	    PROF_CREATE_COUNTER("DB");
	    PROF_CREATE_COUNTER("Data");
	    PROF_CREATE_COUNTER("Data1");
	    PROF_CREATE_COUNTER("Pull");
	    PROF_START("Sync");

        syncAllSources();

	    PROF_DESTROY_COUNTER("Net");	    
	    PROF_DESTROY_COUNTER("Parse");
	    PROF_DESTROY_COUNTER("DB");
	    PROF_DESTROY_COUNTER("Data");
	    PROF_DESTROY_COUNTER("Data1");
	    PROF_DESTROY_COUNTER("Pull");
	    PROF_STOP("Sync");

    }

    getNotify().cleanCreateObjectErrors();

    if ( getState() != esExit )
        setState(esNone);
}

void CSyncEngine::doSearch(rho::Vector<rho::String>& arSources, String strParams, String strAction, boolean bSearchSyncChanges, int nProgressStep)
{
    prepareSync(esSearch);
    if ( !isContinueSync() )
    {
        if ( getState() != esExit )
            setState(esNone);

        return;
    }

    CTimeInterval startTime = CTimeInterval::getCurrentTime();

    if ( bSearchSyncChanges )
    {
        for ( int i = 0; i < (int)arSources.size(); i++ )
        {
            CSyncSource* pSrc = findSourceByName(arSources.elementAt(i));
            if ( pSrc != null )
                pSrc->syncClientChanges();
        }
    }

    int nErrCode = 0;
    while( isContinueSync() )
    {
        int nSearchCount = 0;
        String strUrl = getProtocol().getServerQueryUrl(strAction);
        String strQuery = getProtocol().getServerQueryBody("", getClientID(), getSyncPageSize());

        if ( strParams.length() > 0 )
            strQuery += strParams;

        for ( int i = 0; i < (int)arSources.size(); i++ )
        {
            CSyncSource* pSrc = findSourceByName(arSources.elementAt(i));
            if ( pSrc != null )
            {
                strQuery += "&source[][name]=" + pSrc->getName();

                if ( !pSrc->isTokenFromDB() && pSrc->getToken() > 1 )
                    strQuery += "&source[][token]=" + convertToStringA(pSrc->getToken());
            }
        }

		LOG(INFO) + "Call search on server. Url: " + (strUrl+strQuery);
        NetResponse(resp,getNet().pullData(strUrl+strQuery, this));

        if ( !resp.isOK() )
        {
            stopSync();
            nErrCode = RhoRuby.getErrorFromResponse(resp);
            continue;
        }

        const char* szData = resp.getCharData();

        CJSONArrayIterator oJsonArr(szData);

        for( ; !oJsonArr.isEnd() && isContinueSync(); oJsonArr.next() )
        {
            CJSONArrayIterator oSrcArr(oJsonArr.getCurItem());

            int nVersion = 0;
            if ( !oSrcArr.isEnd() && oSrcArr.getCurItem().hasName("version") )
            {
                nVersion = oSrcArr.getCurItem().getInt("version");
                oJsonArr.next();
            }

            if ( nVersion != getProtocol().getVersion() )
            {
                LOG(ERROR) + "Sync server send search data with incompatible version. Client version: " + convertToStringA(getProtocol().getVersion()) +
                    "; Server response version: " + convertToStringA(nVersion);
                stopSync();
                nErrCode = RhoRuby.ERR_UNEXPECTEDSERVERRESPONSE;
                continue;
            }

            if ( !oSrcArr.getCurItem().hasName("source") )
            {
                LOG(ERROR) + "Sync server send search data without source name.";
                stopSync();
                nErrCode = RhoRuby.ERR_UNEXPECTEDSERVERRESPONSE;
                continue;
            }

            String strSrcName = oSrcArr.getCurItem().getString("source");
            CSyncSource* pSrc = findSourceByName(strSrcName);
            if ( pSrc == null )
            {
                LOG(ERROR) + "Sync server send search data for unknown source name:" + strSrcName;
                stopSync();
                nErrCode = RhoRuby.ERR_UNEXPECTEDSERVERRESPONSE;
                continue;
            }

            oSrcArr.reset(0);
            pSrc->m_bIsSearch = true;
            pSrc->setProgressStep(nProgressStep);
            pSrc->processServerResponse_ver3(oSrcArr);

            nSearchCount += pSrc->getCurPageCount();
        }

        if ( nSearchCount == 0 )
            break;
    }  

    if ( isContinueSync() )
    {
        CSyncSource& src = *m_sources.elementAt(getStartSource());
        src.m_bIsSearch = true;

    	getNotify().fireSyncNotification(&src, true, RhoRuby.ERR_NONE, RhoRuby.getMessageText("sync_completed"));
    }
    else if ( nErrCode != 0 )
    {
        CSyncSource& src = *m_sources.elementAt(getStartSource());
        src.m_nErrCode = nErrCode;
        src.m_bIsSearch = true;
        getNotify().fireSyncNotification(&src, true, src.m_nErrCode, "");
    }

    //update db info
    CTimeInterval endTime = CTimeInterval::getCurrentTime();
    unsigned long timeUpdated = CLocalTime().toULong();
    for ( int i = 0; i < (int)arSources.size(); i++ )
    {
        CSyncSource* pSrc = findSourceByName(arSources.elementAt(i));
        if ( pSrc == null )
            continue;
        CSyncSource& oSrc = *pSrc;
        oSrc.getDB().executeSQL("UPDATE sources set last_updated=?,last_inserted_size=?,last_deleted_size=?, \
						 last_sync_duration=?,last_sync_success=?, backend_refresh_time=? WHERE source_id=?", 
                         timeUpdated, oSrc.getInsertedCount(), oSrc.getDeletedCount(), 
                         (endTime-startTime).toULong(), oSrc.getGetAtLeastOnePage(), oSrc.getRefreshTime(),
                         oSrc.getID() );
    }
    //

    getNotify().cleanCreateObjectErrors();
    if ( getState() != esExit )
        setState(esNone);
}

void CSyncEngine::doSyncSource(const CSourceID& oSrcID)
{
    prepareSync(esSyncSource);

    if ( isContinueSync() )
    {
        CSyncSource* pSrc = findSource(oSrcID);
        if ( pSrc != null )
        {
            CSyncSource& src = *pSrc;
            LOG(INFO) +"Started synchronization of the data source: " + src.getName();

            src.sync();

            getNotify().fireSyncNotification(&src, true, src.m_nErrCode, src.m_nErrCode == RhoRuby.ERR_NONE ? RhoRuby.getMessageText("sync_completed") : "");
        }else
        {
            LOG(ERROR) + "Sync one source : Unknown Source " + oSrcID.toString();

            CSyncSource src(*this, getUserDB() );
    	    //src.m_strError = "Unknown sync source.";
            src.m_nErrCode = RhoRuby.ERR_RUNTIME;

            getNotify().fireSyncNotification(&src, true, src.m_nErrCode, "");
        }
    }

    getNotify().cleanCreateObjectErrors();

    if ( getState() != esExit )
        setState(esNone);
}

CSyncSource* CSyncEngine::findSource(const CSourceID& oSrcID)
{
    for( int i = 0; i < (int)m_sources.size(); i++ )
    {
        CSyncSource& src = *m_sources.elementAt(i);
        if ( oSrcID.isEqual(src) )
            return &src;
    }
    
    return null;
}

CSyncSource* CSyncEngine::findSourceByName(const String& strSrcName)
{
    return findSource(CSourceID(strSrcName));
}

void CSyncEngine::loadAllSources()
{
    m_sources.clear();
    m_arPartitions.clear();

    DBResult( res, getUserDB().executeSQL("SELECT source_id,sync_type,name, partition from sources ORDER BY sync_priority") );
    for ( ; !res.isEnd(); res.next() )
    { 
        String strShouldSync = res.getStringByIdx(1);
        if ( strShouldSync.compare("none") == 0 )
            continue;

        String strName = res.getStringByIdx(2);
        String strPartition = res.getStringByIdx(3);

        if ( m_arPartitions.indexOf(strPartition) < 0 )
            m_arPartitions.addElement(strPartition);

        m_sources.addElement( new CSyncSource( res.getIntByIdx(0), strName, strShouldSync, getDB(strPartition), *this) );
    }
}

String CSyncEngine::loadClientID()
{
    String clientID = "";
    synchronized(m_mxLoadClientID)
    {
        boolean bResetClient = false;
        {
            DBResult( res, getUserDB().executeSQL("SELECT client_id,reset from client_info limit 1") );
            if ( !res.isEnd() )
            {
                clientID = res.getStringByIdx(0);
                bResetClient = res.getIntByIdx(1) > 0;
            }
        }

        if ( clientID.length() == 0 )
        {
            clientID = requestClientIDByNet();

            DBResult( res , getUserDB().executeSQL("SELECT * FROM client_info") );
            if ( !res.isEnd() )
                getUserDB().executeSQL("UPDATE client_info SET client_id=?", clientID);
            else
                getUserDB().executeSQL("INSERT INTO client_info (client_id) values (?)", clientID);

        }else if ( bResetClient )
        {
    	    if ( !resetClientIDByNet(clientID) )
    		    stopSync();
    	    else
    		    getUserDB().executeSQL("UPDATE client_info SET reset=? where client_id=?", 0, clientID );	    	
        }
    }

    return clientID;
}

void CSyncEngine::processServerSources(String strSources)
{
    if ( strSources.length() > 0 )
    {
        //TODO:processServerSources 
        //NetResponse(resp,getNet().pushData( getNet().resolveUrl("/system/loadserversources"), strSources, null ));
        //loadAllSources();

        //rho_db_init_attr_manager();
    }
}

boolean CSyncEngine::resetClientIDByNet(const String& strClientID)//throws Exception
{
    //TODO: send client register info in client reset 
    //String strBody = "";
    //if ( CClientRegister::getInstance() != null )
    //    strBody += CClientRegister::getInstance()->getRegisterBody();

    NetResponse( resp, getNet().pullData(getProtocol().getClientResetUrl(strClientID), this) );

/*    processServerSources("{\"server_sources\":[{\"name\":\"Product\",\"partition\":\"application\",\"source_id\":\"2\",\"sync_priority\":\"0\","
        "\"schema_version\":\"7.0\",\"schema\":{"
        "\"columns\":[\'brand\',\'created_at\',\'name\',\'price\',\'quantity\',\'sku\',\'updated_at\']"*/
/*        "\"sql\":\"CREATE TABLE Product ( "
        "brand varchar default NULL,"
        "created_at varchar default NULL,"
        "name varchar default NULL,"
        "price varchar default NULL,"
        "quantity int default NULL,"
        "sku varchar default NULL,"
        "updated_at varchar default NULL,"
        "test varchar default NULL,"
        "object varchar(255) PRIMARY KEY )\"*/
        //"}}]}"); 

    if ( !resp.isOK() )
        m_nErrCode = RhoRuby.getErrorFromResponse(resp);
    else
    {
        processServerSources(resp.getCharData());
    }

    return resp.isOK();
}

String CSyncEngine::requestClientIDByNet()
{
    //TODO: send client register info in client create 
    //String strBody = "";
    //if ( CClientRegister::getInstance() != null )
    //    strBody += CClientRegister::getInstance()->getRegisterBody();

    NetResponse(resp,getNet().pullData(getProtocol().getClientCreateUrl(), this));
    if ( resp.isOK() && resp.getCharData() != null )
    {
        const char* szData = resp.getCharData();
        /*
        "{\"client\":{\"client_id\":\"vasy\"},\"server_sources\":[{\"name\":\"Product\",\"partition\":\"application\",\"source_id\":\"2\",\"sync_priority\":\"0\","
        "\"schema\":{\"version\":\"1.0\","
        "\"sql\":\"CREATE TABLE Product ( "
        "brand varchar default NULL,"
        "created_at varchar default NULL,"
        "name varchar default NULL,"
        "price varchar default NULL,"
        "quantity int default NULL,"
        "sku varchar default NULL,"
        "updated_at varchar default NULL,"
        "object varchar(255) PRIMARY KEY )\"}}]}";*/

        CJSONEntry oJsonEntry(szData);

        if (oJsonEntry.hasName("sources") )
            processServerSources(szData);

        CJSONEntry oJsonObject = oJsonEntry.getEntry("client");
        if ( !oJsonObject.isEmpty() )
            return oJsonObject.getString("client_id");
    }else
    {
        m_nErrCode = RhoRuby.getErrorFromResponse(resp);
        if ( m_nErrCode == RhoRuby.ERR_NONE )
            m_nErrCode = RhoRuby.ERR_UNEXPECTEDSERVERRESPONSE;
    }

    return "";
}

void CSyncEngine::doBulkSync()//throws Exception
{
//    processServerSources(String("{\"partition\":\"") + "application" + "\"}");

    if ( !RHOCONF().isExist("bulksync_state") )
        return;

    int nBulkSyncState = RHOCONF().getInt("bulksync_state");
    if ( nBulkSyncState >= 1 || !isContinueSync() )
        return;

	LOG(INFO) + "Bulk sync: start";
    getNotify().fireBulkSyncNotification(false, "start", "", RhoRuby.ERR_NONE);        

    for (int i = 0; i < (int)m_arPartitions.size() && isContinueSync(); i++)
        loadBulkPartition(m_arPartitions.elementAt(i));

    if (isContinueSync())
    {
        RHOCONF().setInt("bulksync_state", 1, true);
        getNotify().fireBulkSyncNotification(true, "", "", RhoRuby.ERR_NONE);
    }
}

extern "C" int rho_unzip_file(const char* szZipPath);

static String getHostFromUrl( const String& strUrl );
void CSyncEngine::loadBulkPartition(const String& strPartition )
{
    db::CDBAdapter& dbPartition = getDB(strPartition); 
    String serverUrl = RHOCONF().getPath("syncserver");
    String strUrl = serverUrl + "bulk_data";
    String strQuery = "?client_id=" + m_clientID + "&partition=" + strPartition;
    String strDataUrl = "", strCmd = "";

  	getNotify().fireBulkSyncNotification(false, "start", strPartition, RhoRuby.ERR_NONE);

    while(strCmd.length() == 0&&isContinueSync())
    {	    
        NetResponse( resp, getNet().pullData(strUrl+strQuery, this) );
        const char* szData = resp.getCharData();
        if ( !resp.isOK() || szData == null || *szData == 0)
        {
    	    LOG(ERROR) + "Bulk sync failed: server return an error.";
    	    stopSync();
    	    getNotify().fireBulkSyncNotification(true, "", strPartition, RhoRuby.ERR_REMOTESERVER);
    	    return;
        }

	    LOG(INFO) + "Bulk sync: got response from server: " + szData;
    	
        CJSONEntry oJsonEntry(szData);
        strCmd = oJsonEntry.getString("result");
        if ( oJsonEntry.hasName("url") )
   	        strDataUrl = oJsonEntry.getString("url");
        
        if ( strCmd.compare("wait") == 0)
        {
            int nTimeout = RHOCONF().getInt("bulksync_timeout_sec");
            if ( nTimeout == 0 )
                nTimeout = 5;

            CSyncThread::getInstance()->wait(nTimeout);
            strCmd = "";
        }
    }

    if ( strCmd.compare("nop") == 0)
    {
	    LOG(INFO) + "Bulk sync return no data.";
      	getNotify().fireBulkSyncNotification(true, "", strPartition, RhoRuby.ERR_NONE);

	    return;
    }

    if ( !isContinueSync() )
        return;

   	getNotify().fireBulkSyncNotification(false, "download", strPartition, RhoRuby.ERR_NONE);

    String fDataName = makeBulkDataFileName(strDataUrl, dbPartition.getDBPath(), "");
    String strZip = ".rzip";
	String hostName = getHostFromUrl(serverUrl);
	if (hostName.c_str()[hostName.length()-1] == '/') {
		hostName = hostName.substr(0,hostName.length()-1);
	}
    String strSqlDataUrl = hostName + strDataUrl+strZip;
    LOG(INFO) + "Bulk sync: download data from server: " + strSqlDataUrl;
    {
        NetResponse( resp1, getNet().pullFile(strSqlDataUrl, fDataName+strZip, this, null) );
        if ( !resp1.isOK() )
        {
	        LOG(ERROR) + "Bulk sync failed: cannot download database file.";
	        stopSync();
	        getNotify().fireBulkSyncNotification(true, "", strPartition, RhoRuby.ERR_REMOTESERVER);
	        return;
        }
    }

    if ( !isContinueSync() )
        return;

    LOG(INFO) + "Bulk sync: unzip db";

    if ( !rho_unzip_file((fDataName+strZip).c_str()) )
    {
        CRhoFile::deleteFile((fDataName+strZip).c_str());
        LOG(ERROR) + "Bulk sync failed: cannot unzip database file.";
        stopSync();
        getNotify().fireBulkSyncNotification(true, "", strPartition, RhoRuby.ERR_UNEXPECTEDSERVERRESPONSE);
        return;
    }
    CRhoFile::deleteFile((fDataName+strZip).c_str());

	LOG(INFO) + "Bulk sync: start change db";
   	getNotify().fireBulkSyncNotification(false, "change_db", strPartition, RhoRuby.ERR_NONE);
    
    dbPartition.setBulkSyncDB(fDataName);
    processServerSources(String("{\"partition\":\"") + strPartition + "\"}");

	LOG(INFO) + "Bulk sync: end change db";
   	getNotify().fireBulkSyncNotification(false, "", strPartition, RhoRuby.ERR_NONE);
}

String CSyncEngine::makeBulkDataFileName(String strDataUrl, String strDbPath, String strExt)
{
    CFilePath oUrlPath(strDataUrl);
    String strNewName = oUrlPath.getBaseName();
    String strOldName = RHOCONF().getString("bulksync_filename");
    if ( strOldName.length() > 0 && strNewName.compare(strOldName) != 0 )
    {
        CFilePath oFilePath(strDbPath);
        String strFToDelete = oFilePath.changeBaseName(strOldName+strExt);
        LOG(INFO) + "Bulk sync: remove old bulk file '" + strFToDelete + "'";

        CRhoFile::deleteFile( strFToDelete.c_str() );
    }

    RHOCONF().setString("bulksync_filename", strNewName, true);

    CFilePath oFilePath(strDbPath);
    return oFilePath.changeBaseName(strNewName+strExt);
}

int CSyncEngine::getStartSource()
{
    for( int i = 0; i < (int)m_sources.size(); i++ )
    {
        CSyncSource& src = *m_sources.elementAt(i);
        if ( !src.isEmptyToken() )
            return i;
    }

    return 0;
}

void CSyncEngine::syncAllSources()
{
    //TODO: do not stop on error source
    boolean bError = false;
    for( int i = getStartSource(); i < (int)m_sources.size() && isContinueSync(); i++ )
    {
        CSyncSource& src = *m_sources.elementAt(i);
        if ( src.getSyncType().compare("bulk_sync_only")==0 )
            continue;

        if ( isSessionExist() && getState() != esStop )
            src.sync();

        getNotify().onSyncSourceEnd(i, m_sources);
        bError = src.m_nErrCode != RhoRuby.ERR_NONE;
    }

    if ( !bError)
    	getNotify().fireSyncNotification(null, true, RhoRuby.ERR_NONE, RhoRuby.getMessageText("sync_completed"));
}

void CSyncEngine::login(String name, String password, String callback)
{
    PROF_START("Login");
	//try {

    NetResponse( resp, getNet().pullCookies( getProtocol().getLoginUrl(), getProtocol().getLoginBody(name, password), this ) );
    int nErrCode = RhoRuby.getErrorFromResponse(resp);
    if ( nErrCode != RhoRuby.ERR_NONE )
    {
        getNotify().callLoginCallback(callback, nErrCode, resp.getCharData());
        return;
    }

    String strSession = resp.getCharData();
    if ( strSession.length() == 0 )
    {
    	LOG(ERROR) + "Return empty session.";
    	getNotify().callLoginCallback(callback, RhoRuby.ERR_UNEXPECTEDSERVERRESPONSE, "" );
        return;
    }

    DBResult( res , getUserDB().executeSQL("SELECT * FROM client_info") );
    if ( !res.isEnd() )
        getUserDB().executeSQL( "UPDATE client_info SET session=?", strSession );
    else
        getUserDB().executeSQL("INSERT INTO client_info (session) values (?)", strSession);


    if ( CClientRegister::getInstance() != null )
        CClientRegister::getInstance()->stopWait();

    getNotify().callLoginCallback(callback, RhoRuby.ERR_NONE, "" );
	
    PROF_STOP("Login");
	//}catch(Exception exc)
	//{
	//	LOG.ERROR("Login failed.", exc);
    //	callLoginCallback(callback, RhoRuby.ERR_RUNTIME, exc.getMessage() );
	//}
}

boolean CSyncEngine::isLoggedIn()
 {
    String strRes = "";
    DBResult( res , getUserDB().executeSQL("SELECT session FROM client_info") );
    if ( !res.isEnd() )
    	strRes = res.getStringByIdx(0);
    
    return strRes.length() > 0;
}

String CSyncEngine::loadSession()
{
    m_strSession = "";
    DBResult( res , getUserDB().executeSQL("SELECT session FROM client_info") );
    
    if ( !res.isEnd() )
    	m_strSession = res.getStringByIdx(0);
    
    return m_strSession;
}

void CSyncEngine::logout()
{
    getUserDB().executeSQL( "UPDATE client_info SET session=NULL" );
    m_strSession = "";

    loadAllSources();
}
	
void CSyncEngine::setSyncServer(char* syncserver)
{
	rho_conf_setString("syncserver", syncserver);
	rho_conf_save();

    getUserDB().executeSQL("DELETE FROM client_info");

	logout();
}

static String getHostFromUrl( const String& strUrl )
{
    const char* url = strUrl.c_str();
    const char* pStartSrv, *pEndSrv;
    int nSrvLen;
    const char* pHttp = strstr(url,"://");
    if ( !pHttp )
        pHttp = strstr(url,":\\\\");

    if ( pHttp )
        pStartSrv = pHttp+3;
    else
        pStartSrv = url;

    pEndSrv = strchr( pStartSrv, '/');
    if ( !pEndSrv )
        pEndSrv = strchr( pStartSrv, '\\');

    nSrvLen = pEndSrv ? (pEndSrv+1 - url) : strlen(url);
    return String(url, nSrvLen);
}

String CSyncEngine::CSourceID::toString()const
{
    if ( m_strName.length() > 0 )
        return "name : " + m_strName;

    return "# : " + convertToStringA(m_nID);
}

boolean CSyncEngine::CSourceID::isEqual(CSyncSource& src)const
{
    if ( m_strName.length() > 0 )
        return src.getName().compare(m_strName)==0;

    return m_nID == src.getID();
}
}
}
