#include "RhodesApp.h"
#include "common/RhoMutexLock.h"
#include "common/IRhoClassFactory.h"
#include "common/RhoConf.h"
#include "common/RhoFilePath.h"
#include "net/INetRequest.h"
#include "net/HttpServer.h"
#include "ruby/ext/rho/rhoruby.h"
//#include <math.h>
#include "sync/ClientRegister.h"
#include "sync/SyncThread.h"
#include "net/AsyncHttp.h"

#ifdef OS_WINCE
#include <winsock.h>
#endif 
//#include "shttpd/src/shttpd.h"
//#include "shttpd/src/std_includes.h"

using rho::net::HttpHeader;
using rho::net::HttpHeaderList;
using rho::net::CHttpServer;

extern "C" {
void rho_sync_create();
void rho_sync_destroy();
void rho_sync_doSyncAllSources(int show_status_popup);
void rho_map_location(char* query);
void rho_appmanager_load( void* httpContext, const char* szQuery);
void webview_navigate(char* url, int index);
}

namespace rho {
namespace common{

IMPLEMENT_LOGCLASS(CRhodesApp,"RhodesApp");
CRhodesApp* CRhodesApp::m_pInstance = 0;

/*static*/ CRhodesApp* CRhodesApp::Create(const String& strRootPath)
{
    if ( m_pInstance != null) 
        return m_pInstance;

    m_pInstance = new CRhodesApp(strRootPath);
    return m_pInstance;
}

/*static*/void CRhodesApp::Destroy()
{
    if ( m_pInstance )
        delete m_pInstance;

    m_pInstance = 0;
}

CRhodesApp::CRhodesApp(const String& strRootPath) : CRhoThread(createClassFactory())
{
    m_strRhoRootPath = strRootPath;
    m_bExit = false;

#if !defined(RHO_HTTPD_COMMON_IMPL)
    m_shttpdCtx = 0;
#endif

    m_ptrFactory = createClassFactory();
    m_NetRequest = m_ptrFactory->createNetRequest();
    m_oGeoLocation.init(m_ptrFactory);

#if defined( OS_WINCE ) || defined (OS_WINDOWS)
    //initializing winsock
    WSADATA WsaData;
    int result = WSAStartup(MAKEWORD(2,2),&WsaData);
#endif

    //rho_logconf_Init(m_strRhoRootPath.c_str());
    initAppUrls();
    //start(epNormal);

    getSplashScreen().init();
}

void CRhodesApp::startApp()
{
    start(epNormal);
}

void CRhodesApp::run()
{
    LOG(INFO) + "Starting RhodesApp main routine...";
    initHttpServer();
    RhoRubyStart();

    LOG(INFO) + "Starting sync engine...";
    rho_sync_create();
    LOG(INFO) + "RhoRubyInitApp...";
    RhoRubyInitApp();

    LOG(INFO) + "activate app";
    rho_ruby_activateApp();

    getSplashScreen().hide();

    LOG(INFO) + "navigate to first start url";
    navigateToUrl(getFirstStartUrl());

#if !defined(RHO_HTTPD_COMMON_IMPL)
    while(!m_bExit)
    {
        shttpd_poll( m_shttpdCtx, 100000 );

//#if defined(OS_WINCE)
  //GPS
  //CGPSController::CheckTimeout();
//#endif
    }
#else
    m_httpServer->run();
#endif

    LOG(INFO) + "RhodesApp thread shutdown";

    RhoRubyStop();
    rho_sync_destroy();

#if !defined(RHO_HTTPD_COMMON_IMPL)
    shttpd_fini(m_shttpdCtx);
#endif

//#if defined(OS_WINCE)
//    CGPSController* pGPS = CGPSController::Instance();
//    pGPS->DeleteInstance();
//#endif

}

CRhodesApp::~CRhodesApp(void)
{
    stopApp();

#ifdef OS_WINCE
    WSACleanup();
#endif

}

void CRhodesApp::stopApp()
{
    if (!m_bExit)
    {
        m_bExit = true;
#if !defined(RHO_HTTPD_COMMON_IMPL)
        shutdown_poll(m_shttpdCtx);
#else
		m_httpServer->stop();
#endif //RHO_HTTPD_COMMON_IMPL
        stop(2000);
    }

    rho_asynchttp_destroy();
}

class CRhoCallbackCall :  public common::CRhoThread
{
    common::CAutoPtr<common::IRhoClassFactory> m_ptrFactory;
	String m_strCallback, m_strBody;
public:
    CRhoCallbackCall(const String& strCallback, const String& strBody, common::IRhoClassFactory* factory) : CRhoThread(factory), 
        m_ptrFactory(factory), m_strCallback(strCallback), m_strBody(strBody)
    { start(epNormal); }

private:
    virtual void run()
    {
        common::CAutoPtr<net::INetRequest> pNetRequest = m_ptrFactory->createNetRequest();
		common::CAutoPtr<net::INetResponse> presp = pNetRequest->pushData( m_strCallback, m_strBody, null );
        delete this;
    }
};

void CRhodesApp::runCallbackInThread(const String& strCallback, const String& strBody)
{
    new CRhoCallbackCall(strCallback, strBody, createClassFactory() );
}

static void callback_activateapp(void *arg, String const &strQuery)
{
    rho_ruby_activateApp();
    String strMsg;
    rho_http_sendresponse(arg, strMsg.c_str());
}

void CRhodesApp::callAppActiveCallback(boolean bActive)
{
    m_httpServer->pause(!bActive);
    if (bActive)
    {
        String strUrl = m_strHomeUrl + "/system/activateapp";
        NetResponse(resp,getNet().pullData( strUrl, null ));
        if ( !resp.isOK() )
            LOG(ERROR) + "activate app failed. Code: " + resp.getRespCode() + "; Error body: " + resp.getCharData();

        LOG(INFO) + "navigate to first start url";
        navigateToUrl(getFirstStartUrl());
    }
}

void CRhodesApp::callCameraCallback(String strCallbackUrl, const String& strImagePath, 
    const String& strError, boolean bCancel ) 
{
    strCallbackUrl = canonicalizeRhoUrl(strCallbackUrl);
    String strBody;
    if ( bCancel || strError.length() > 0 )
    {
        if ( bCancel )
            strBody = "status=cancel&message=User canceled operation.";
        else
            strBody = "status=error&message=" + strError;
    }else
        strBody = "status=ok&image_uri=%2Fpublic%2Fdb-files%2F" + strImagePath;

    strBody += "&rho_callback=1";
    NetRequest( getNet().pushData( strCallbackUrl, strBody, null ) );
}

void CRhodesApp::callDateTimeCallback(String strCallbackUrl, long lDateTime, const char* szData, int bCancel )
{
    strCallbackUrl = canonicalizeRhoUrl(strCallbackUrl);
    String strBody;
    if ( bCancel )
        strBody = "status=cancel&message=User canceled operation.";
    else
        strBody = "status=ok&result=" + convertToStringA(lDateTime);

    if ( szData && *szData )
    {
        strBody += "&opaque=";
        strBody += szData;
    }

    strBody += "&rho_callback=1";
    NetRequest( getNet().pushData( strCallbackUrl, strBody, null ) );
}

static void callback_syncdb(void *arg
#if defined(RHO_HTTPD_COMMON_IMPL)
                            , String const &/*query*/
#endif
                           )
{
    rho_sync_doSyncAllSources(1);
    rho_http_sendresponse(arg, "");
}

static void callback_redirect_to(void *arg
#if defined(RHO_HTTPD_COMMON_IMPL)
                                 , String const &strQuery
#endif
                                )
{
#if !defined(RHO_HTTPD_COMMON_IMPL)
    String strQuery = shttpd_get_env((shttpd_arg *)arg,"QUERY_STRING");
#endif
    size_t nUrl = strQuery.find("url=");
    String strUrl;
    if ( nUrl != String::npos )
        strUrl = strQuery.substr(nUrl+4);

    if ( strUrl.length() == 0 )
        strUrl = "/app/";

    rho_http_redirect(arg, strUrl.c_str());
}

static void callback_map(void *arg
#if defined(RHO_HTTPD_COMMON_IMPL)
                         , String const &query
#endif
                        )
{
#if !defined(RHO_HTTPD_COMMON_IMPL)
    String query = shttpd_get_env((shttpd_arg *)arg,"QUERY_STRING");
#endif
    rho_map_location( const_cast<char*>(query.c_str()) );
    rho_http_sendresponse(arg, "");
}

static void callback_shared(void *arg
#if defined(RHO_HTTPD_COMMON_IMPL)
                            , String const &/*query*/
#endif
                           )
{
    rho_http_senderror(arg, 404, "Not Found");
}

static void callback_AppManager_load(void *arg
#if defined(RHO_HTTPD_COMMON_IMPL)
                                     , String const &query
#endif
                                    )
{
#if !defined(RHO_HTTPD_COMMON_IMPL)
    String query = shttpd_get_env((shttpd_arg *)arg,"QUERY_STRING");
#endif
    rho_appmanager_load( arg, query.c_str() );
}

static void callback_getrhomessage(void *arg, String const &strQuery)
{
    String strMsg;
    size_t nErrorPos = strQuery.find("error=");
    if ( nErrorPos != String::npos )
    {
        String strError = strQuery.substr(nErrorPos+6);
        int nError = atoi(strError.c_str());

        strMsg = rho_ruby_internal_getErrorText(nError);
    }

    rho_http_sendresponse(arg, strMsg.c_str());
}

const String& CRhodesApp::getRhoMessage(int nError, const char* szName)
{
    String strUrl = m_strHomeUrl + "/system/getrhomessage?";
    if ( nError )
        strUrl += "error=" + convertToStringA(nError);
    else if ( szName && *szName )
    {
        strUrl = "msgid=";
        strUrl += szName;
    }

    NetResponse(resp,getNet().pullData( strUrl, null ));
    if ( !resp.isOK() )
    {
        LOG(ERROR) + "getRhoMessage failed. Code: " + resp.getRespCode() + "; Error body: " + resp.getCharData();
        m_strRhoMessage = "";
    }
    else
        m_strRhoMessage = resp.getCharData();

    return m_strRhoMessage;
}

void CRhodesApp::initHttpServer()
{
    String strAppRootPath = getRhoRootPath() + "apps";
    
#if !defined(RHO_HTTPD_COMMON_IMPL)
    LOG(INFO) + "Init http server";
    m_shttpdCtx = shttpd_init(0,NULL);

    shttpd_set_option(m_shttpdCtx, "root", strAppRootPath.c_str());
    shttpd_set_option(m_shttpdCtx, "ports", getFreeListeningPort());

    //shttpd_register_uri(m_shttpdCtx, "/system/geolocation", &CGPSController::show_geolocation, NULL);
    shttpd_register_uri(m_shttpdCtx, "/system/geolocation", callback_geolocation, this);
    shttpd_register_uri(m_shttpdCtx, "/system/syncdb", callback_syncdb, this);
    shttpd_register_uri(m_shttpdCtx, "/system/redirect_to", callback_redirect_to, this);
    shttpd_register_uri(m_shttpdCtx, "/system/map", callback_map, this);
    shttpd_register_uri(m_shttpdCtx, "/system/shared", callback_shared, this);
    shttpd_register_uri(m_shttpdCtx, "/AppManager/loader/load", callback_AppManager_load, this);
#else

    m_httpServer = new net::CHttpServer(atoi(getFreeListeningPort()), strAppRootPath);
    m_httpServer->register_uri("/system/geolocation", rubyext::CGeoLocation::callback_geolocation);
    m_httpServer->register_uri("/system/syncdb", callback_syncdb);
    m_httpServer->register_uri("/system/redirect_to", callback_redirect_to);
    m_httpServer->register_uri("/system/map", callback_map);
    m_httpServer->register_uri("/system/shared", callback_shared);
    m_httpServer->register_uri("/AppManager/loader/load", callback_AppManager_load);
    m_httpServer->register_uri("/system/getrhomessage", callback_getrhomessage);
    m_httpServer->register_uri("/system/activateapp", callback_activateapp);
#endif
}

const char* CRhodesApp::getFreeListeningPort()
{
	if ( m_strListeningPorts.length() > 0 )
		return m_strListeningPorts.c_str();
	
	int noerrors = 1;
	LOG(INFO) + "Trying to get free listening port.";
	
	//get free port
	int sockfd = -1;
	struct sockaddr_in serv_addr = {0};
	//struct hostent *server = {0};
	//int result = -1;
	
	if ( noerrors )
	{
		sockfd = socket(AF_INET, SOCK_STREAM, 0);
		if ( sockfd < 0 )
		{
			LOG(WARNING) + ("Unable to open socket");
			noerrors = 0;
		}
		
		if ( noerrors )
		{
			//server = gethostbyname( "localhost" );
			
			memset((void *) &serv_addr, 0, sizeof(serv_addr));
			serv_addr.sin_family = AF_INET;
			serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
			serv_addr.sin_port = htons(8080);

			if ( bind( sockfd, (struct sockaddr *) &serv_addr, sizeof( serv_addr ) ) )
            {
    			serv_addr.sin_port = htons(0);
			    if ( bind( sockfd, (struct sockaddr *) &serv_addr, sizeof( serv_addr ) ) )
			    {
				    LOG(WARNING) + "Unable to bind";
				    noerrors = 0;
			    }
            }

			if ( noerrors )
			{
				char buf[10] = {0};
#ifdef OS_MACOSX
				socklen_t
#else				
				int
#endif				
				length = sizeof( serv_addr );
				
				getsockname( sockfd, (struct sockaddr *)&serv_addr, &length );
				
				sprintf(buf,"%d",ntohs(serv_addr.sin_port));
				
				m_strListeningPorts = buf;
			}
			//Clean up
#if defined(OS_ANDROID)
			close(sockfd);
#else
			closesocket(sockfd);
#endif
		}
		
	}
	
	if ( !noerrors )
		m_strListeningPorts = "8080";
	
	LOG(INFO) + "Free listening port: " + m_strListeningPorts;
	
	return m_strListeningPorts.c_str();
}
	
void CRhodesApp::initAppUrls() 
{
    m_currentTabIndex = 0;
    
    m_strHomeUrl = "http://localhost:";
    m_strHomeUrl += getFreeListeningPort();

    m_strBlobsDirPath = getRhoRootPath() + "apps/public/db-files";
    m_strLoadingPagePath = "file://" + getRhoRootPath() + "apps/app/loading.html";
	m_strLoadingPngPath = getRhoRootPath() + "apps/app/loading.png";
}

void CRhodesApp::keepLastVisitedUrl(String strUrl)
{
    LOG(INFO) + "Current URL: " + strUrl;

    m_currentUrls[m_currentTabIndex] = canonicalizeRhoUrl(strUrl);

    if ( RHOCONF().getBool("KeepTrackOfLastVisitedPage") )
    {
        if ( strUrl.compare( 0, m_strHomeUrl.length(), m_strHomeUrl ) == 0 )
            strUrl = strUrl.substr(m_strHomeUrl.length());

        size_t nFragment = strUrl.find('#');
        if ( nFragment != String::npos )
            strUrl = strUrl.substr(0, nFragment);

        RHOCONF().setString("LastVisitedPage",strUrl);		
        RHOCONF().saveToFile();
    }
}

const String& CRhodesApp::getStartUrl()
{
    m_strStartUrl = canonicalizeRhoUrl( RHOCONF().getString("start_path") );
    return m_strStartUrl;
}

const String& CRhodesApp::getOptionsUrl()
{
    m_strOptionsUrl = canonicalizeRhoUrl( RHOCONF().getString("options_path") );
    return m_strOptionsUrl;
}

const String& CRhodesApp::getCurrentUrl(int index)
{ 
    return m_currentUrls[m_currentTabIndex]; 
}

const String& CRhodesApp::getFirstStartUrl()
{
    m_strFirstStartUrl = getStartUrl();
    if ( RHOCONF().getBool("KeepTrackOfLastVisitedPage") ) 
    {
        rho::String strLastPage = RHOCONF().getString("LastVisitedPage");
        if (strLastPage.length() > 0)
            m_strFirstStartUrl = canonicalizeRhoUrl(strLastPage);
    }

    return m_strFirstStartUrl;
}

const String& CRhodesApp::getRhobundleReloadUrl() 
{
    m_strRhobundleReloadUrl = RHOCONF().getString("rhobundle_zip_url");
    return m_strRhobundleReloadUrl;
}

void CRhodesApp::navigateToUrl( const String& strUrl)
{
    webview_navigate(const_cast<char*>(strUrl.c_str()), 0);
}

String CRhodesApp::canonicalizeRhoUrl(const String& strUrl) 
{
    if (strUrl.length() == 0 )
        return m_strHomeUrl;

    if ( strncmp("http://", strUrl.c_str(), 7 ) == 0 ||
        strncmp("mailto:", strUrl.c_str(), 7) == 0 ||
        strncmp("tel:", strUrl.c_str(), 4) == 0)
        return strUrl;

    return CFilePath::join(m_strHomeUrl,strUrl);
}

void CRhodesApp::addViewMenuItem( const String& strLabel, const String& strLink )
{
    if ( strLabel.length() == 0 )
        return;

    synchronized(m_mxViewMenuItems)
    {
        m_hashViewMenuItems.put(strLabel, strLink);

        if ( strcasecmp( strLabel.c_str(), "back" )==0 && strcasecmp( strLink.c_str(), "back" )!=0 )
            m_strAppBackUrl = canonicalizeRhoUrl(strLink);
    }
}

extern "C" void
menu_iter(const char* szLabel, const char* szLink, void* pThis)
{
    ((CRhodesApp*)pThis)->addViewMenuItem(szLabel, szLink );
}

void CRhodesApp::setViewMenu(unsigned long valMenu)
{
    {
        synchronized(m_mxViewMenuItems)

        m_hashViewMenuItems.clear();
        m_strAppBackUrl="";
    }

    rho_ruby_enum_strhash(valMenu, menu_iter, this);
}

boolean CRhodesApp::sendLog() 
{
    String strDevicePin = rho::sync::CClientRegister::getInstance() ? rho::sync::CClientRegister::getInstance()->getDevicePin() : "";
	String strClientID = rho::sync::CSyncThread::getSyncEngine().loadClientID();

    String strLogUrl = RHOCONF().getPath("logserver");
    if ( strLogUrl.length() == 0 )
        strLogUrl = RHOCONF().getPath("syncserver");

	String strQuery = strLogUrl + "client_log?" +
	    "client_id=" + strClientID + "&device_pin=" + strDevicePin + "&log_name=" + RHOCONF().getString("logname");

    NetResponse( resp, getNet().pushFile( strQuery, LOGCONF().getLogFilePath(), &(rho::sync::CSyncThread::getSyncEngine()), null ) );
    if ( !resp.isOK() )
    {
        LOG(ERROR) + "send_log failed : network error";
        return false;
    }

    return true;
}

String CRhodesApp::addCallbackObject(ICallbackObject* pCallbackObject, String strName)
{
    int nIndex = -1;
    for (int i = 0; i < (int)m_arCallbackObjects.size(); i++)
    {
        if ( m_arCallbackObjects.elementAt(i) == 0 )
            nIndex = i;
    }
//    rho_ruby_holdValue(valObject);
    if ( nIndex  == -1 )
    {
        m_arCallbackObjects.addElement(pCallbackObject);
        nIndex = m_arCallbackObjects.size()-1;
    }else
        m_arCallbackObjects.setElementAt(pCallbackObject,nIndex);

    String strRes = "__rho_object[" + strName + "]=" + convertToStringA(nIndex);

    return strRes;
}

void CRhodesApp::delCallbackObject(ICallbackObject* pCallbackObject)
{
    for (int i = 0; i < (int)m_arCallbackObjects.size(); i++)
    {
        if ( m_arCallbackObjects.elementAt(i) == pCallbackObject )
        {
            m_arCallbackObjects.setElementAt(0,i);
//            rho_ruby_releaseValue(valObject);
        }
    }
}

unsigned long CRhodesApp::getCallbackObject(int nIndex)
{
    if ( nIndex < 0 || nIndex > m_arCallbackObjects.size() )
        return rho_ruby_get_NIL();

    ICallbackObject* pCallbackObject = m_arCallbackObjects.elementAt(nIndex);
    if ( !pCallbackObject )
        return rho_ruby_get_NIL();

    return pCallbackObject->getObjectValue();
}

}
}

extern "C" {

unsigned long rho_rhodesapp_GetCallbackObject(int nIndex)
{
    return RHODESAPP().getCallbackObject(nIndex);
}

char* rho_http_normalizeurl(const char* szUrl) 
{
    rho::String strRes = RHODESAPP().canonicalizeRhoUrl(szUrl);
    return strdup(strRes.c_str());
}

void rho_http_free(void* data)
{
    free(data);
}
    
void rho_http_redirect( void* httpContext, const char* szUrl)
{
#if !defined(RHO_HTTPD_COMMON_IMPL)
    struct shttpd_arg *arg = (struct shttpd_arg *)httpContext;
    
    shttpd_printf(arg, "%s", "HTTP/1.1 301 Moved Permanently\r\n");
    shttpd_printf(arg, "Location: %s\r\n", szUrl );
    shttpd_printf(arg, "%s", "Content-Length: 0\r\n");
    shttpd_printf(arg, "%s", "Connection: close\r\n");
//#ifndef OS_MACOSX
    shttpd_printf(arg, "%s", "Pragma: no-cache\r\n" );
    shttpd_printf(arg, "%s", "Cache-Control: must-revalidate\r\n" );
    shttpd_printf(arg, "%s", "Cache-Control: no-cache\r\n" );
    shttpd_printf(arg, "%s", "Cache-Control: no-store\r\n" );	
    shttpd_printf(arg, "%s", "Expires: 0\r\n" );
//#endif

    shttpd_printf(arg, "%s", "Content-Type: text/plain\r\n\r\n");

    arg->flags |= SHTTPD_END_OF_OUTPUT;
#else
    HttpHeaderList headers;
    headers.push_back(HttpHeader("Location", szUrl));
    headers.push_back(HttpHeader("Content-Length", 0));
    headers.push_back(HttpHeader("Pragma", "no-cache"));
    headers.push_back(HttpHeader("Cache-Control", "must-revalidate"));
    headers.push_back(HttpHeader("Cache-Control", "no-cache"));
    headers.push_back(HttpHeader("Cache-Control", "no-store"));
    headers.push_back(HttpHeader("Expires", 0));
    headers.push_back(HttpHeader("Content-Type", "text/plain"));
    
    CHttpServer *serv = (CHttpServer *)httpContext;
    serv->send_response(serv->create_response("301 Moved Permanently", headers));
#endif
}

void rho_http_senderror(void* httpContext, int nError, const char* szMsg)
{
#if !defined(RHO_HTTPD_COMMON_IMPL)
    struct shttpd_arg *arg = (struct shttpd_arg *)httpContext;

    shttpd_printf(arg, "%s", "HTTP/1.1 %d %s\r\n", nError, szMsg);
    arg->flags |= SHTTPD_END_OF_OUTPUT;
#else
    char buf[30];
    snprintf(buf, sizeof(buf), "%d", nError);
    
    rho::String reason = buf;
    reason += " ";
    reason += szMsg;
    
    CHttpServer *serv = (CHttpServer *)httpContext;
    serv->send_response(serv->create_response(reason));
#endif
}

void rho_http_sendresponse(void* httpContext, const char* szBody)
{
#if !defined(RHO_HTTPD_COMMON_IMPL)
    struct shttpd_arg *arg = (struct shttpd_arg *)httpContext;
    unsigned long nBodySize = strlen(szBody);
    
    shttpd_printf(arg, "%s", "HTTP/1.1 200 OK\r\n");
    shttpd_printf(arg, "Content-Length: %lu\r\n", nBodySize );
    shttpd_printf(arg, "%s", "Connection: close\r\n");
#ifndef OS_MACOSX
    shttpd_printf(arg, "%s", "Pragma: no-cache\r\n" );
    shttpd_printf(arg, "%s", "Cache-Control: no-cache\r\n" );
    shttpd_printf(arg, "%s", "Expires: 0\r\n" );
#else
    const char *fmt = "%a, %d %b %Y %H:%M:%S GMT";
    char date[64], lm[64];
    time_t	_current_time = time(0);
    strftime(date, sizeof(date), fmt, localtime(&_current_time));
    strftime(lm, sizeof(lm), fmt, localtime(&_current_time));
    
    shttpd_printf(arg, "Date: %s\r\n", date);
    shttpd_printf(arg, "Last-Modified: %s\r\n", lm);
    shttpd_printf(arg, "Etag: \"%lx.%lx\"\r\n", (unsigned long) _current_time, nBodySize);
#endif

    shttpd_printf(arg, "%s", "Content-Type: text/html; charset=ISO-8859-4\r\n\r\n");
    shttpd_printf(arg, "%s", szBody );

    arg->flags |= SHTTPD_END_OF_OUTPUT;
#else
    size_t nBodySize = strlen(szBody);
    
    HttpHeaderList headers;
    headers.push_back(HttpHeader("Content-Length", nBodySize));
    headers.push_back(HttpHeader("Pragma", "no-cache"));
    headers.push_back(HttpHeader("Cache-Control", "no-cache"));
    headers.push_back(HttpHeader("Expires", 0));
    
    const char *fmt = "%a, %d %b %Y %H:%M:%S GMT";
    char date[64], lm[64], etag[64];
    time_t	_current_time = time(0);
    strftime(date, sizeof(date), fmt, localtime(&_current_time));
    strftime(lm, sizeof(lm), fmt, localtime(&_current_time));
    snprintf(etag, sizeof(etag), "\"%lx.%lx\"", (unsigned long)_current_time, (unsigned long)nBodySize);
    
    headers.push_back(HttpHeader("Date", date));
    headers.push_back(HttpHeader("Last-Modified", lm));
    headers.push_back(HttpHeader("Etag", etag));
    
    CHttpServer *serv = (CHttpServer *)httpContext;
    serv->send_response(serv->create_response("200 OK", headers, szBody));
#endif
}

int	rho_http_snprintf(char *buf, size_t buflen, const char *fmt, ...)
{
	va_list		ap;
	int		n;
		
	if (buflen == 0)
		return (0);
		
	va_start(ap, fmt);
	n = vsnprintf(buf, buflen, fmt, ap);
	va_end(ap);
		
	if (n < 0 || (size_t) n >= buflen)
		n = buflen - 1;
	buf[n] = '\0';
		
	return (n);
}
	
void rho_rhodesapp_create(const char* szRootPath)
{
    rho::common::CRhodesApp::Create(szRootPath);
}

void rho_rhodesapp_start()
{
    RHODESAPP().startApp();
}
    
void rho_rhodesapp_destroy()
{
    rho::common::CRhodesApp::Destroy();
}

const char* rho_rhodesapp_getfirststarturl()
{
    return RHODESAPP().getFirstStartUrl().c_str();
}

const char* rho_rhodesapp_getstarturl()
{
    return RHODESAPP().getStartUrl().c_str();
}

const char* rho_rhodesapp_gethomeurl()
{
    return RHODESAPP().getHomeUrl().c_str();
}

const char* rho_rhodesapp_getoptionsurl()
{
    return RHODESAPP().getOptionsUrl().c_str();
}

void rho_rhodesapp_keeplastvisitedurl(const char* szUrl)
{
    RHODESAPP().keepLastVisitedUrl(szUrl);
}

const char* rho_rhodesapp_getcurrenturl(int index)
{
    return RHODESAPP().getCurrentUrl(index).c_str();
}

const char* rho_rhodesapp_getloadingpagepath()
{
    return RHODESAPP().getLoadingPagePath().c_str();
}

const char* rho_rhodesapp_getblobsdirpath()
{
    return RHODESAPP().getBlobsDirPath().c_str();
}

void rho_rhodesapp_callCameraCallback(const char* strCallbackUrl, const char* strImagePath, 
    const char* strError, int bCancel )
{
    RHODESAPP().callCameraCallback(strCallbackUrl, strImagePath, strError, bCancel != 0);
}

void rho_rhodesapp_callDateTimeCallback(const char* strCallbackUrl, long lDateTime, const char* szData, int bCancel )
{
    RHODESAPP().callDateTimeCallback(strCallbackUrl, lDateTime, szData, bCancel != 0);
}

void rho_rhodesapp_callAppActiveCallback(int nActive)
{
    RHODESAPP().callAppActiveCallback(nActive!=0);
}

void rho_rhodesapp_setViewMenu(unsigned long valMenu)
{
    RHODESAPP().setViewMenu(valMenu);
}

const char* rho_rhodesapp_getappbackurl()
{
    return RHODESAPP().getAppBackUrl().c_str();
}

const char* rho_ruby_getErrorText(int nError)
{
    return RHODESAPP().getRhoMessage( nError, "").c_str();
}

const char* rho_ruby_getMessageText(const char* szName)
{
    return "";//RHODESAPP().getRhoMessage( 0, szName).c_str();
}

int rho_rhodesapp_isrubycompiler()
{
    return 0;
}

int rho_conf_send_log()
{
    return RHODESAPP().sendLog();
}

static const char table64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

int rho_base64_encode(const char *src, int srclen, char *dst)
{
    if (srclen < 0)
        srclen = strlen(src);
    if (!dst)
        return (srclen/3)*4 + (srclen%3 ? 4 : 0) + 1;
    
    int out = 0;
    for(int in = 0; in < srclen; in += 3, out += 4) {
        
        unsigned x = 0;
        int actual = 0;
        for (int i = 0; i < 3; ++i) {
            char c;
            if (in + i >= srclen)
                c = 0;
            else {
                c = src[in + i];
                actual += 8;
            }
            x = (x << 8) + (unsigned char)c;
        }
        
        for (int i = 0; i < 4; ++i) {
            if (actual <= 0) {
                dst[out + i] = '=';
            }
            else {
                dst[out + i] = table64[(x >> 18) & 0x3F];
                x <<= 6;
                actual -= 6;
            }
        }
    }
    
    dst[out++] = '\0';
    return out;
}

int rho_base64_decode(const char *src, int srclen, char *dst)
{
    if (srclen < 0)
        srclen = strlen(src);
    // Do not decode in case if srclen can not be divided by 4
    if (srclen%4)
        return 0;
    if (!dst)
        return srclen*3/4 + 1;
    
    char *found;
    int out = 0;
    for (int in = 0; in < srclen; in += 4, out += 3) {
        unsigned x = 0;
        for (int i = 0; i < 4; ++i) {
            if ((found = strchr(const_cast<char*>(table64), src[in + i])) != NULL)
                x = (x << 6) + (unsigned int)(found - table64);
            else if (src[in + i] == '=')
                x <<= 6;
        }
        
        for (int i = 0; i < 3; ++i) {
            dst[out + i] = (unsigned char)((x >> 16) & 0xFF);
            x <<= 8;
        }
    }
    dst[out++] = '\0';
    return out;
}

}

