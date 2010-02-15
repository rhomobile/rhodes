#include "SyncEngine.h"
#include "SyncSource.h"

#include "json/JSONIterator.h"
#include "common/RhoConf.h"
#include "common/StringConverter.h"
#include "sync/ClientRegister.h"
#include "net/URI.h"
#include "statistic/RhoProfiler.h"
#include "ruby/ext/rho/rhoruby.h"
#include "net/URI.h"

namespace rho {
const _CRhoRuby& RhoRuby = _CRhoRuby();

/*static*/ String _CRhoRuby::getMessageText(const char* szName)
{
    return rho_ruby_getMessageText(szName);
}

/*static*/ String _CRhoRuby::getErrorText(int nError)
{
    return rho_ruby_getErrorText(nError);
}

namespace sync {
IMPLEMENT_LOGCLASS(CSyncEngine,"Sync");

using namespace rho::net;
using namespace rho::common;
using namespace rho::json;

CSyncEngine::CSyncEngine(db::CDBAdapter& db): m_dbAdapter(db), m_NetRequest(0), m_syncState(esNone), m_oSyncNotify(*this)
{
    m_bStopByUser = false;
    m_nSyncPageSize = 2000;
    m_bNoThreaded = false;
}

String CSyncEngine::SYNC_PAGE_SIZE() { return convertToStringA(m_nSyncPageSize); }

void CSyncEngine::doSyncAllSources()
{
    setState(esSyncAllSources);
    m_bStopByUser = false;
    loadAllSources();

    m_strSession = loadSession();
    if ( isSessionExist()  )
    {
        m_clientID = loadClientID();
        getNotify().cleanLastSyncObjectCount();

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
    else
    {
        if ( m_sources.size() > 0 )
        {
            CSyncSource& src = *m_sources.elementAt(getStartSource());
    	    //src.m_strError = "Client is not logged in. No sync will be performed.";
            src.m_nErrCode = RhoRuby.ERR_CLIENTISNOTLOGGEDIN;

            getNotify().fireSyncNotification(&src, true, src.m_nErrCode, "");
        }else
            getNotify().fireSyncNotification(null, true, RhoRuby.ERR_CLIENTISNOTLOGGEDIN, "");
    }

    if ( getState() != esExit )
        setState(esNone);
}

void CSyncEngine::doSyncSource(const CSourceID& oSrcID, String strParams, String strAction, boolean bSearchSyncChanges, int nProgressStep)
{
    setState(esSyncSource);
    m_bStopByUser = false;
    loadAllSources();

    CSyncSource* pSrc = findSource(oSrcID);
    if ( pSrc != null )
    {
        CSyncSource& src = *pSrc;

        LOG(INFO) +"Started synchronization of the data source: " + src.getName();

    	src.m_strParams = strParams;
    	src.m_strAction = strAction;
    	src.m_bSearchSyncChanges = bSearchSyncChanges;
        src.m_nProgressStep = nProgressStep;
      	if ( oSrcID.m_strUrl.length() != 0 )
       	{
            net::URI uri(oSrcID.m_strUrl);
       		src.setUrlParams(uri.getQueryString());

            if (uri.getScheme().length()>0)
       			src.setUrl(uri.getPathSpecificPart());
       	}

	    m_strSession = loadSession();
	    if ( isSessionExist()  ) {
	    	m_clientID = loadClientID();
	        if ( getState() != esStop )
            {
                getNotify().cleanLastSyncObjectCount();
	            src.sync();
            }
	    } else {
	    	//src.m_strError = "Client is not logged in. No sync will be performed.";
            src.m_nErrCode = RhoRuby.ERR_CLIENTISNOTLOGGEDIN;
	    }

        getNotify().fireSyncNotification(&src, true, src.m_nErrCode, src.m_nErrCode == RhoRuby.ERR_NONE ? RhoRuby.getMessageText("sync_completed") : "");
    }else
    {
        LOG(ERROR) + "Sync one source : Unknown Source " + oSrcID.toString();

        CSyncSource src(*this);
    	//src.m_strError = "Unknown sync source.";
        src.m_nErrCode = RhoRuby.ERR_RUNTIME;

        getNotify().fireSyncNotification(&src, true, src.m_nErrCode, "");
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
    CSourceID oSrcID;
    oSrcID.m_strName = strSrcName;
    return findSource(oSrcID);
}

void CSyncEngine::loadAllSources()
{
    m_sources.clear();

    DBResult( res, getDB().executeSQL("SELECT source_id,source_url,token,name from sources ORDER BY priority") );
    for ( ; !res.isEnd(); res.next() )
    { 
        String strDbUrl = res.getStringByIdx(1);
        if ( strDbUrl.length() == 0 )
            continue;

        if ( strDbUrl.at(0) == '/' || strDbUrl.at(0) == '\\' )
            strDbUrl.erase(strDbUrl.begin());

		String strUrl = strDbUrl.find("http") == 0 ? strDbUrl : (RHOCONF().getPath("syncserver") + strDbUrl);
        if ( strUrl.at(strUrl.length()-1) == '/' || strUrl.at(strUrl.length()-1) == '\\' )
            strUrl.erase(strUrl.end()-1);

        String strName = res.getStringByIdx(3);
        if ( strUrl.length() > 0 )
            m_sources.addElement( new CSyncSource( res.getIntByIdx(0), strUrl, strName, res.getUInt64ByIdx(2), *this) );
    }
}

String CSyncEngine::loadClientID()
{
    String clientID = "";
    synchronized(m_mxLoadClientID)
    {
        boolean bResetClient = false;
        int nInitialSyncState = 0;
        {
            DBResult( res, getDB().executeSQL("SELECT client_id,reset,initialsync_state from client_info limit 1") );
            if ( !res.isEnd() )
            {
                clientID = res.getStringByIdx(0);
                bResetClient = res.getIntByIdx(1) > 0;
                nInitialSyncState = res.getIntByIdx(2);
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
            {
                if ( m_sources.size() > 0 )
                {
                    CSyncSource& src = *m_sources.elementAt(getStartSource());
                    src.m_nErrCode = RhoRuby.ERR_REMOTESERVER;

                    getNotify().fireSyncNotification(&src, true, src.m_nErrCode, "");
                }else
                    getNotify().fireSyncNotification(null, true, RhoRuby.ERR_REMOTESERVER, "");

    		    stopSync();
            }
    	    else
    		    getDB().executeSQL("UPDATE client_info SET reset=? where client_id=?", 0, clientID );	    	
        }
//TODO: doInitialSync
//        if ( nInitialSyncState == 0 && isContinueSync() )
//        	doInitialSync(clientID);
    }
    return clientID;
}

boolean CSyncEngine::resetClientIDByNet(const String& strClientID)//throws Exception
{
    String serverUrl = RHOCONF().getPath("syncserver");
    String strUrl = serverUrl + "clientreset";
    String strQuery = "?client_id=" + strClientID;
    if ( CClientRegister::getInstance() != null )
        strQuery += "&" + CClientRegister::getInstance()->getRegisterBody();
    
    NetResponse( resp, getNet().pullData(strUrl+strQuery, this) );
    if ( resp.isOK() )
    {
        if ( CClientRegister::getInstance() != null )
            return CClientRegister::getInstance()->doRegister(*this);

        return true;
    }

    return false;
}

String CSyncEngine::requestClientIDByNet()
{
    String serverUrl = RHOCONF().getPath("syncserver");
    String strUrl = serverUrl + "clientcreate";
    String strQuery = SYNC_SOURCE_FORMAT();
    if ( CClientRegister::getInstance() != null )
        strQuery += "&" + CClientRegister::getInstance()->getRegisterBody();

    NetResponse(resp,getNet().pullData(strUrl+strQuery, this));
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

void CSyncEngine::doInitialSync(String strClientID)//throws Exception
{
	LOG(INFO) + "Initial sync: start";
	getNotify().fireInitialSyncNotification(false, RhoRuby.ERR_NONE);

    String serverUrl = RHOCONF().getPath("syncserver");
    String strUrl = serverUrl + "initialsync";
    String strQuery = "?client_id=" + strClientID;
    String strDataUrl = "";
/*
    {	    
        NetResponse( resp, getNet().pullData(strUrl+strQuery, this) );
        if ( !resp.isOK() )
        {
    	    LOG(ERROR) + "Initial sync failed: server return an error.";
    	    stopSync();
    	    getNotify().fireInitialSyncNotification(true, RhoRuby.ERR_REMOTESERVER);
    	    return;
        }
        //TODO: check is server return no initial sync
        if ( resp.getCharData() != null )
        {
		    LOG(INFO) + "Initial sync: got response from server: " + resp.getCharData();
        	
            const char* szData = resp.getCharData();
            CJSONEntry oJsonEntry(szData);

            CJSONEntry oJsonObject = oJsonEntry.getEntry("initialsync");
            if ( !oJsonObject.isEmpty() )
            {
        	    strDataUrl = oJsonObject.getString("data");
            }
        }
        if ( strDataUrl.length() == 0 )
        {
    	    LOG(ERROR) + "Initial sync failed: server return incorrect response.";
    	    stopSync();
    	    getNotify().fireInitialSyncNotification(true, RhoRuby.ERR_REMOTESERVER);
    	    return;
        }
    }*/

    String fDataName =  getDB().getDBPath() + "_initial";
    /*{
	    LOG(INFO) + "Initial sync: download data from server: " + strDataUrl;
        NetResponse( resp1, getNet().pullFile(strDataUrl+strQuery, fDataName, this) );
        if ( !resp1.isOK() )
        {
    	    LOG(ERROR) + "Initial sync failed: cannot download database file.";
    	    stopSync();
    	    getNotify().fireInitialSyncNotification(true, RhoRuby.ERR_REMOTESERVER);
    	    return;
        }
    } */

	LOG(INFO) + "Initial sync: change db";
    
    //getDB().setInitialSyncDB(fDataName);
    
	getDB().executeSQL("UPDATE client_info SET initialsync_state=? where client_id=?", 1, strClientID );	    	
    
    getNotify().fireInitialSyncNotification(true, RhoRuby.ERR_NONE);        
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
    boolean bError = false;
    for( int i = getStartSource(); i < (int)m_sources.size() && isContinueSync(); i++ )
    {
        CSyncSource& src = *m_sources.elementAt(i);
        if ( isSessionExist() && getState() != esStop )
            src.sync();

        getNotify().onSyncSourceEnd(i, m_sources);
        bError = src.m_nErrCode != RhoRuby.ERR_NONE;
    }

    if ( !bError)
    	getNotify().fireSyncNotification(null, true, RhoRuby.ERR_NONE, RhoRuby.getMessageText("sync_completed"));
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
    PROF_START("Login");
	//try {
	if ( !checkAllSourcesFromOneDomain() )
	{
        getNotify().callLoginCallback(callback, RhoRuby.ERR_DIFFDOMAINSINSYNCSRC, "");
    	return;
	}

    String serverUrl = RHOCONF().getPath("syncserver");
    String strBody = "login=" + name + "&password=" + password + "&remember_me=1";

    NetResponse( resp, getNet().pullCookies( serverUrl+"client_login", strBody, this ) );
    
    if ( !resp.isResponseRecieved())
    {
        getNotify().callLoginCallback(callback, RhoRuby.ERR_NETWORK, resp.getCharData());
        return;
    }

    if ( resp.isUnathorized() )
    {
        getNotify().callLoginCallback(callback, RhoRuby.ERR_UNATHORIZED, resp.getCharData());
    	return;
    }

    if ( !resp.isOK() )
    {
        getNotify().callLoginCallback(callback, RhoRuby.ERR_REMOTESERVER, resp.getCharData());
    	return;
    }

    String strSession = resp.getCharData();
    if ( strSession.length() == 0 )
    {
    	LOG(ERROR) + "Return empty session.";
    	getNotify().callLoginCallback(callback, RhoRuby.ERR_UNEXPECTEDSERVERRESPONSE, "" );
        return;
    }

    //TODO: move session to client_info table
    getDB().executeSQL( "UPDATE sources SET session=?", strSession );

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
    int nCount = 0;
    DBResult( res , getDB().executeSQL("SELECT count(session) FROM sources") );
     if ( !res.isEnd() )
        nCount = res.getIntByIdx(0);

    return nCount > 0;
}

String CSyncEngine::loadSession()
{
    String strRes = "";
    DBResult( res , getDB().executeSQL("SELECT session FROM sources WHERE session IS NOT NULL") );
    
    if ( !res.isEnd() )
    	strRes = res.getStringByIdx(0);
    
    return strRes;
}

void CSyncEngine::logout()
{
    getDB().executeSQL( "UPDATE sources SET session=NULL" );
    m_strSession = "";

    loadAllSources();
}
	
void CSyncEngine::setSyncServer(char* syncserver)
{
	rho_conf_setString("syncserver", syncserver);
	rho_conf_save();

    getDB().executeSQL("DELETE FROM client_info");

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

String CSyncEngine::CSourceID::toString()const
{
    if ( m_strName.length() > 0 )
        return "name : " + m_strName;
    else if ( m_strUrl.length() > 0 )
        return "url : " + m_strUrl;

    return "# : " + convertToStringA(m_nID);
}

boolean CSyncEngine::CSourceID::isEqual(CSyncSource& src)const
{
    if ( m_strName.length() > 0 )
        return src.getName().compare(m_strName)==0;
    else if ( m_strUrl.length() > 0 )
    {
        net::URI uri1(m_strUrl);
        net::URI uri2(src.getUrl());
    	
    	return uri1.getPath().compare(uri2.getPath()) == 0;
        //return src.getUrl().compare(m_strUrl)==0;
    }

    return m_nID == src.getID();
}
}
}
