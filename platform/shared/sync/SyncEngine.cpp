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

    m_isLoggedIn = isLoggedIn();
    if ( m_isLoggedIn )
        loadClientID();

    syncAllSources();

    setState(esNone);
	
	LOG(INFO) + "End syncing all sources";
}

void CSyncEngine::doSyncSource()
{
    //TODO:doSyncSource
}

void CSyncEngine::loadAllSources()
{
    m_sources.clear();

    DBResult( res, getDB().executeSQL("SELECT source_id,source_url,token from sources order by source_id") );
    for ( ; !res.isEnd(); res.next() )
    { 
        String strUrl = res.getStringByIdx(1);
        if ( strUrl.length() > 0 )
            m_sources.add( new CSyncSource( res.getIntByIdx(0), strUrl, res.getUInt64ByIdx(2), *this) );
    }
}

void CSyncEngine::loadClientID()
{
    if ( m_clientID.length() > 0 )
        return;
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

    CSyncSource& src = *m_sources.get(0);
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
    for( int i = 0; i < m_sources.size(); i++ )
    {
        CSyncSource& src = *m_sources.get(i);
        if ( !src.isEmptyToken() )
            return i;
    }

    return 0;
}

void CSyncEngine::syncAllSources()
{
    for( int i = getStartSource(); i < m_sources.size() && getState() != esExit; i++ )
    {
        CSyncSource& src = *m_sources.get(i);
        if ( m_isLoggedIn && getState() != esStop )
            src.sync();

        fireNotification(src.getID(),src.getServerObjectsCount());
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
    CSyncSource& src0 = *m_sources.get(0);
    String srv0 = getServerFromUrl(src0.getUrl());
    for( int i = 1; i < m_sources.size(); i++ )
    {
        CSyncSource& src = *m_sources.get(i);
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

boolean CSyncEngine::isLoggedIn()
{
    int nCount = 0;
    DBResult( res , getDB().executeSQL("SELECT count(session) FROM sources") );
    if ( !res.isEnd() )
        nCount = res.getIntByIdx(0);

    return nCount > 0;
}

void CSyncEngine::logout()
{
    getDB().executeSQL( "UPDATE sources SET session=NULL" );
    getNet().deleteCookie("");

    loadAllSources();
    for( int i = 0; i < m_sources.size(); i++ )
    {
        CSyncSource& src = *m_sources.get(i);
        getNet().deleteCookie(src.getUrl());
    }

}

void CSyncEngine::resetSyncDB()
{
    getDB().executeSQL( "DELETE from object_values" );
    getDB().executeSQL( "DELETE from client_info" );
    getDB().executeSQL( "UPDATE sources SET token=?", 0 );
    getDB().executeSQL( "VACUUM" );

    m_clientID = "";
}

void CSyncEngine::setNotification(int source_id, String strUrl, String strParams )
{
	LOG(INFO) + "Set notification. Source ID: " + source_id + "; Url :" + strUrl + "; Params: " + strParams;
    clearNotification(source_id);

    String strFullUrl = getNet().resolveUrl(strUrl);

    if ( strFullUrl.length() > 0 )
    {
		LOG(INFO) + " Done Set notification. Source ID: " + source_id + "; Url :" + strFullUrl + "; Params: " + strParams;
		
        CMutexLock lockNotify(m_mxNotifications);
        m_mapNotifications.put(source_id,new CSyncNotification( strFullUrl, strParams ) );
    }
}

void CSyncEngine::fireNotification( int source_id, int nSyncObjectsCount)
{
    String strBody, strUrl;
    {
        CMutexLock lockNotify(m_mxNotifications);

        CSyncNotification* pSN = m_mapNotifications.get(source_id);
        if ( pSN == 0 )
            return;
        CSyncNotification& sn = *pSN;

        strUrl = sn.m_strUrl;
        strBody = "status=";
        strBody += (nSyncObjectsCount > 0 ?"ok":"error");
        if ( sn.m_strParams.length() > 0 )
            strBody += "&" + sn.m_strParams;
    }
	LOG(INFO) + "Fire notification. Source ID: " + source_id + "; Url :" + strUrl + "; Body: " + strBody;
	
    getNet().pushData( strUrl, strBody );

    clearNotification(source_id);
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