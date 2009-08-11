#include "SyncEngine.h"
#include "SyncSource.h"

#include "common/AutoPointer.h"
#include "json/JSONIterator.h"
#include "common/RhoConf.h"
#include "sync/ClientRegister.h"

namespace rho {

namespace RhoRuby {
static int ERR_NONE = 0;
static int ERR_NETWORK = 1;
static int ERR_REMOTESERVER = 2;
static int ERR_RUNTIME = 3;
static int ERR_UNEXPECTEDSERVERRESPONSE = 4;
static int ERR_DIFFDOMAINSINSYNCSRC = 5;
}

namespace sync {
IMPLEMENT_LOGCLASS(CSyncEngine,"Sync");

using namespace rho::net;
using namespace rho::common;
using namespace rho::json;

void CSyncEngine::doSyncAllSources()
{
	LOG(INFO) + "Start syncing all sources";
	
    setState(esSyncAllSources);

    loadAllSources();

    m_strSession = loadSession();
    if ( isSessionExist()  )
        m_clientID = loadClientID();
    else
    	LOG(INFO) + "Client is not logged in. No sync will be performed.";

    syncAllSources();

    setState(esNone);
	
	LOG(INFO) + "End syncing all sources";
}

void CSyncEngine::doSyncSource(int nSrcId, String strSrcUrl)
{
    if ( strSrcUrl.length()>0 )
        LOG(ERROR) + "Start syncing source : " + strSrcUrl;
    else
        LOG(ERROR) + "Start syncing source : " + nSrcId;

    setState(esSyncSource);

    loadAllSources();

    m_strSession = loadSession();
    if ( isSessionExist()  )
        m_clientID = loadClientID();
    else
    	LOG(INFO) + "Client is not logged in. No sync will be performed.";
    
    CSyncSource* pSrc = null;
    if ( strSrcUrl.length()>0 )
    	pSrc = findSourceByUrl(strSrcUrl);
    else
        pSrc = findSourceByID(nSrcId);

    if ( pSrc != null )
    {
        CSyncSource& src = *pSrc;
        if ( isSessionExist() && getState() != esStop )
            src.sync();

        fireNotification(src, true);
    }else
    {
        if ( strSrcUrl.length()>0 )
            LOG(ERROR) + "Sync one source : Unknown Source Url: " + strSrcUrl;
        else
            LOG(ERROR) + "Sync one source : Unknown Source ID: " + nSrcId;
    }

    setState(esNone);

    if ( strSrcUrl.length()>0 )
        LOG(ERROR) + "End syncing source : " + strSrcUrl;
    else
        LOG(ERROR) + "End syncing source : " + nSrcId;
}

CSyncSource* CSyncEngine::findSourceByID(int nSrcId)
{
    for( int i = 0; i < (int)m_sources.size(); i++ )
    {
        CSyncSource& src = *m_sources.elementAt(i);
        if ( src.getID() == nSrcId )
            return &src;
    }
    
    return null;
}

CSyncSource* CSyncEngine::findSourceByUrl(const String& strSrcUrl)
{
    for( int i = 0; i < (int)m_sources.size(); i++ )
    {
        CSyncSource& src = *m_sources.elementAt(i);
        if ( src.getUrl() == strSrcUrl )
            return &src;
    }
    
    return null;
}

void CSyncEngine::loadAllSources()
{
    m_sources.clear();

    DBResult( res, getDB().executeSQL("SELECT source_id,source_url,token from sources order by source_id") );
    for ( ; !res.isEnd(); res.next() )
    { 
        String strDbUrl = res.getStringByIdx(1);
		String strUrl = strDbUrl.find("http") == 0 ? strDbUrl : (RHOCONF().getString("syncserver") + strDbUrl);
        if ( strUrl.length() > 0 )
            m_sources.addElement( new CSyncSource( res.getIntByIdx(0), strUrl, res.getUInt64ByIdx(2), *this) );
    }
}

String CSyncEngine::loadClientID()
{
    String clientID = "";
    CMutexLock lockNotify(m_mxLoadClientID);
    boolean bResetClient = false;
    {
        DBResult( res, getDB().executeSQL("SELECT client_id,reset from client_info limit 1") );
        if ( !res.isEnd() )
        {
            clientID = res.getStringByIdx(0);
            bResetClient = res.getIntByIdx(1) > 0;
        }
    }

    if ( clientID.length() == 0 )
    {
        clientID = requestClientIDByNet();

        getDB().executeSQL("DELETE FROM client_info");
        getDB().executeSQL("INSERT INTO client_info (client_id) values (?)", clientID);
    }else if ( bResetClient )
    {
    	if ( !resetClientIDByNet(clientID) )
    		stopSync();
    	else
    		getDB().executeSQL("UPDATE client_info SET reset=? where client_id=?", 0, clientID );	    	
    }

    return clientID;
}

boolean CSyncEngine::resetClientIDByNet(const String& strClientID)//throws Exception
{
    String serverUrl = RHOCONF().getString("syncserver");
    String strUrl = serverUrl + "clientreset";
    String strQuery = "?client_id=" + strClientID;
    
    NetResponse( resp, getNet().pullData(strUrl+strQuery) );
    return resp.isOK();
}

String CSyncEngine::requestClientIDByNet()
{
    String serverUrl = RHOCONF().getString("syncserver");
    String strUrl = serverUrl + "clientcreate";
    String strQuery = SYNC_SOURCE_FORMAT();

    NetResponse(resp,getNet().pullData(strUrl+strQuery));
    if ( resp.isOK() && resp.getCharData() != null )
    {
        const char* szData = resp.getCharData();
        CJSONEntry oJsonEntry(szData);

        CJSONEntry oJsonObject = oJsonEntry.getEntry("client");
        if ( !oJsonObject.isEmpty() )
            return oJsonObject.getString("client_id");
    }

    return "";
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
    for( int i = getStartSource(); i < (int)m_sources.size() && getState() != esExit; i++ )
    {
        CSyncSource& src = *m_sources.elementAt(i);
        if ( isSessionExist() && getState() != esStop )
            src.sync();

        fireNotification(src, true);
    }
}

void CSyncEngine::callLoginCallback(String callback, int nErrCode, String strMessage)
{
	//try{
    String strBody = "error_code=" + convertToStringA(nErrCode);
    strBody += "&error_message=" + strMessage;
    String strUrl = getNet().resolveUrl(callback);
    
	LOG(INFO) + "Login callback: " + callback + ". Body: "+ strBody;

    NetResponse( resp, getNet().pushData( strUrl, strBody ) );
    if ( !resp.isOK() )
        LOG(ERROR) + "Call Login callback failed. Code: " + resp.getRespCode() + "; Error body: " + resp.getCharData();
	//}catch(Exception exc)
	//{
	//	LOG.ERROR("Call Login callback failed.", exc);
	//}
}

static String getServerFromUrl( const String& strUrl );
boolean CSyncEngine::checkAllSourcesFromOneDomain()//throws Exception
{
	loadAllSources();
	
    if ( m_sources.size() == 0 )
        return true;

    //All sources should be from one domain
    CSyncSource& src0 = *m_sources.elementAt(0);
    String srv0 = getServerFromUrl(src0.getUrl());
    for( int i = 1; i < (int)m_sources.size(); i++ )
    {
        CSyncSource& src = *m_sources.elementAt(i);
        String srv = getServerFromUrl(src.getUrl());
        if ( srv.compare( srv0 ) != 0 )
            return false;
    }
	
    return true;
}

void CSyncEngine::login(String name, String password, String callback)
{
	//try {
	if ( !checkAllSourcesFromOneDomain() )
	{
        callLoginCallback(callback, RhoRuby::ERR_DIFFDOMAINSINSYNCSRC, "");
    	return;
	}

    String serverUrl = RHOCONF().getString("syncserver");
    String strBody = "login=" + name + "&password=" + password + "&remember_me=1";

    NetResponse( resp, getNet().pullCookies( serverUrl+"client_login", strBody ) );
    if ( !resp.isOK() )
    {
        callLoginCallback(callback, RhoRuby::ERR_REMOTESERVER, resp.getCharData());
    	return;
    }
    
    getDB().executeSQL( "UPDATE sources SET session=?", "exists" );

    if ( CClientRegister::getInstance() != null )
        CClientRegister::getInstance()->stopWait();
    
    callLoginCallback(callback, RhoRuby::ERR_NONE, "" );
	    
	//}catch(Exception exc)
	//{
	//	LOG.ERROR("Login failed.", exc);
    //	callLoginCallback(callback, RhoRuby.ERR_RUNTIME, exc.getMessage() );
	//}
}

#ifdef OS_MACOSX
extern "C" int rho_sync_logged_in_cookies();
#endif

boolean CSyncEngine::isLoggedIn()
 {
    //TODO: read cookies from DB and set them for each request
 #ifdef OS_MACOSX
    return rho_sync_logged_in_cookies() == 0 ? false : true;
 #else
    int nCount = 0;
    DBResult( res , getDB().executeSQL("SELECT count(session) FROM sources") );
     if ( !res.isEnd() )
        nCount = res.getIntByIdx(0);

    return nCount > 0;
 #endif
}

String CSyncEngine::loadSession()
{
#ifdef OS_MACOSX
    return rho_sync_logged_in_cookies() == 0 ? "" : "exist";
#else
    String strRes = "";
    DBResult( res , getDB().executeSQL("SELECT session FROM sources WHERE session IS NOT NULL") );
    
    if ( !res.isEnd() )
    	strRes = res.getStringByIdx(0);
    
    return strRes;
#endif
}

void CSyncEngine::logout()
{
    getDB().executeSQL( "UPDATE sources SET session=NULL" );
    m_strSession = "";
    getNet().deleteCookie("");

    loadAllSources();
    for( int i = 0; i < (int)m_sources.size(); i++ )
    {
        CSyncSource& src = *m_sources.elementAt(i);
        getNet().deleteCookie(src.getUrl());
    }

}

void CSyncEngine::setNotification(int source_id, String strUrl, String strParams )
{
	LOG(INFO) + "Set notification. Source ID: " + source_id + "; Url :" + strUrl + "; Params: " + strParams;
    String strFullUrl = getNet().resolveUrl(strUrl);

	if ( source_id == -1 )
	{
		CMutexLock lockNotify(m_mxNotifications);
		m_mapNotifications.clear();
		
		if ( strFullUrl.length() > 0 )
		{
			loadAllSources();
			
		    for( int i = 0; i < (int)m_sources.size(); i++ )
		    {
		    	CSyncSource& src = *m_sources.elementAt(i); 
		    	m_mapNotifications.put( src.getID(),new CSyncNotification( strFullUrl, strParams ) );
    	    }
		}
		LOG(INFO) + " Done Set notification for all sources; Url :" + strFullUrl + "; Params: " + strParams;
	}else
	{
        clearNotification(source_id);
        if ( strFullUrl.length() > 0 )
        {
            CMutexLock lockNotify(m_mxNotifications);
            m_mapNotifications.put(source_id,new CSyncNotification( strFullUrl, strParams ) );

		    LOG(INFO) + " Done Set notification. Source ID: " + source_id + "; Url :" + strFullUrl + "; Params: " + strParams;
        }
    }
}

void CSyncEngine::fireNotification( CSyncSource& src, boolean bFinish)
{
    String strBody, strUrl;
    {
        CMutexLock lockNotify(m_mxNotifications);

        CSyncNotification* pSN = m_mapNotifications.get(src.getID());
        if ( pSN == 0 )
            return;
        CSyncNotification& sn = *pSN;

        StringStream ss;

        strUrl = sn.m_strUrl;
		strBody = "";
		ss << src.getTotalCount();
        strBody = "total_count=" + ss.str();
		ss.str("");
		ss << src.getCurPageCount();
        strBody += "&processed_count=" + ss.str();
        
        strBody += "&status=";

        if ( bFinish )
        	strBody += (src.getServerObjectsCount() > 0 ?"ok":"error");
        else
        	strBody += "in_progress";
        
        if ( sn.m_strParams.length() > 0 )
            strBody += "&" + sn.m_strParams;
    }
	LOG(INFO) + "Fire notification. Source ID: " + src.getID() + "; Url :" + strUrl + "; Body: " + strBody;
	
    NetResponse(resp,getNet().pushData( strUrl, strBody ));
    if ( !resp.isOK() )
        LOG(ERROR) + "Fire notification failed. Code: " + resp.getRespCode() + "; Error body: " + resp.getCharData();

    clearNotification(src.getID());
}

void CSyncEngine::clearNotification(int source_id) 
{
	LOG(INFO) + "Clear notification. Source ID: " + source_id;
	
    CMutexLock lockNotify(m_mxNotifications);
    m_mapNotifications.remove(source_id);
}
	
void CSyncEngine::setSyncServer(char* syncserver)
{
	rho_conf_setString("syncserver", syncserver);
	rho_conf_save();
	logout();
}

static String getServerFromUrl( const String& strUrl )
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

    nSrvLen = pEndSrv ? (pEndSrv - pStartSrv) : strlen(pStartSrv);
    return String(pStartSrv, nSrvLen);
}

}
}