#include "SyncEngine.h"
#include "SyncSource.h"

#include "common/AutoPointer.h"
#include "json/JSONIterator.h"

namespace rho {
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
        loadClientID();
    else
    	LOG(INFO) + "Client is not logged in. No sync will be performed.";

    syncAllSources();

    setState(esNone);
	
	LOG(INFO) + "End syncing all sources";
}

void CSyncEngine::doSyncSource(int nSrcId)
{
	LOG(INFO) + "Start syncing source : " + nSrcId;
	
    setState(esSyncSource);

    loadAllSources();

    m_strSession = loadSession();
    if ( isSessionExist()  )
        loadClientID();
    else
    	LOG(INFO) + "Client is not logged in. No sync will be performed.";
    
    CSyncSource* pSrc = findSourceByID(nSrcId);
    if ( pSrc != null )
    {
        CSyncSource& src = *pSrc;
        if ( isSessionExist() && getState() != esStop )
            src.sync();

        fireNotification(src, true);
    }else
    	LOG(ERROR) + "Sync one source : Unknown Source ID: " + nSrcId;
    
    setState(esNone);
	
	LOG(INFO) + "End  syncing source : " + nSrcId;
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

void CSyncEngine::loadAllSources()
{
    m_sources.clear();

    DBResult( res, getDB().executeSQL("SELECT source_id,source_url,token from sources order by source_id") );
    for ( ; !res.isEnd(); res.next() )
    { 
        String strUrl = res.getStringByIdx(1);
        if ( strUrl.length() > 0 )
            m_sources.addElement( new CSyncSource( res.getIntByIdx(0), strUrl, res.getUInt64ByIdx(2), *this) );
    }
}

void CSyncEngine::loadClientID()
{
    m_clientID = "";
    {
        DBResult( res, getDB().executeSQL("SELECT client_id from client_info limit 1") );
        if ( !res.isEnd() )
            m_clientID = res.getStringByIdx(0);
    }

    if ( m_clientID.length() == 0 )
    {
        m_clientID = requestClientIDByNet();

        getDB().executeSQL("DELETE FROM client_info");
        getDB().executeSQL("INSERT INTO client_info (client_id) values (?)", m_clientID);
    }
}

String CSyncEngine::requestClientIDByNet()
{
    if ( m_sources.size() == 0 )
        return "";

    CSyncSource& src = *m_sources.elementAt(0);
    String strUrl = src.getUrl() + "/clientcreate";
    String strQuery = SYNC_SOURCE_FORMAT();

    NetRequestStr(szData,getNet().pullData(strUrl+strQuery));
    if ( szData != null )
    {
        CJSONEntry oJsonEntry(szData);

        CJSONEntry oJsonObject = oJsonEntry.getEntry("client");
        if ( !oJsonObject.isEmpty() )
            return oJsonObject.getString("client_id");
    }

    return String();
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

static String getServerFromUrl( const String& strUrl );
boolean CSyncEngine::login(String name, String password)
{
    loadAllSources();
    return doLogin(name,password);
}

boolean CSyncEngine::doLogin(String name, String password)
{
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

    String strBody = "login=" + name + "&password=" + password + "&remember_me=1";
    if ( !getNet().pullCookies( src0.getUrl()+"/client_login", strBody) )
        return false;

    getDB().executeSQL( "UPDATE sources SET session=?", "exists" );

    return true;
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
    getNet().deleteCookie("");

    loadAllSources();
    for( int i = 0; i < (int)m_sources.size(); i++ )
    {
        CSyncSource& src = *m_sources.elementAt(i);
        getNet().deleteCookie(src.getUrl());
    }

}

void CSyncEngine::resetSyncDB()
{
    getDB().executeSQL( "DELETE from object_values" );
    getDB().executeSQL( "UPDATE client_info SET reset=?", 1 );
    getDB().executeSQL( "UPDATE sources SET token=?", 0 );
    getDB().executeSQL( "VACUUM" );

    m_clientID = "";
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

        strUrl = sn.m_strUrl;
        strBody += "total_count=" + src.getTotalCount();
        strBody += "&processed_count=" + src.getCurPageCount();
        
        strBody = "&status=";
        if ( bFinish )
        	strBody += (src.getServerObjectsCount() > 0 ?"ok":"error");
        else
        	strBody += "in_progress";
        
        if ( sn.m_strParams.length() > 0 )
            strBody += "&" + sn.m_strParams;
    }
	LOG(INFO) + "Fire notification. Source ID: " + src.getID() + "; Url :" + strUrl + "; Body: " + strBody;
	
    getNet().pushData( strUrl, strBody );

    clearNotification(src.getID());
}

void CSyncEngine::clearNotification(int source_id) 
{
	LOG(INFO) + "Clear notification. Source ID: " + source_id;
	
    CMutexLock lockNotify(m_mxNotifications);
    m_mapNotifications.remove(source_id);
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