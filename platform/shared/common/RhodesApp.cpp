#include "RhodesApp.h"
#include "common/RhoMutexLock.h"
#include "common/IRhoClassFactory.h"
#include "common/RhoConf.h"
#include "net/INetRequest.h"
#include "ruby/ext/rho/rhoruby.h"
#include <math.h>

#ifdef OS_WINCE
#include <winsock.h>
#endif 
#include "shttpd/src/shttpd.h"
//#include "shttpd/src/std_includes.h"

extern "C" {
void rho_sync_create();
void rho_sync_destroy();
void rho_sync_doSyncAllSources(int show_status_popup);
double geo_latitude();
double geo_longitude();
int geo_known_position();	
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
    if ( m_pInstance ) 
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
	
    m_shttpdCtx = 0;

#ifdef OS_WINCE
    //initializing winsock
    WSADATA WsaData;
    int result = WSAStartup(MAKEWORD(1,1),&WsaData);
#endif

	//rho_logconf_Init(m_strRhoRootPath.c_str());	
    initAppUrls();
    //start(epNormal);
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

    rho_sync_create();
    RhoRubyInitApp();
    callAppActiveCallback();
    rho_ruby_activateApp();

    navigateToUrl(getFirstStartUrl());//canonicalizeRhoUrl("/system/geolocation"));

    while(!m_bExit)
    {
        shttpd_poll( m_shttpdCtx, 100000 );

//#if defined(OS_WINCE)
  //GPS
  //CGPSController::CheckTimeout();
//#endif
    }

    LOG(INFO) + "RhodesApp thread shutdown";

    RhoRubyStop();
    rho_sync_destroy();

    shttpd_fini(m_shttpdCtx);

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
#ifndef OS_MACOSX		
        shutdown_poll(m_shttpdCtx);
#endif //OS_MACOSX
        stop(2000);
    }
}

class CRhoCallbackCall :  public common::CRhoThread
{
    String m_strCallback;
    common::CAutoPtr<common::IRhoClassFactory> m_ptrFactory;
public:
    CRhoCallbackCall(const String& strCallback, common::IRhoClassFactory* factory) : CRhoThread(factory), m_ptrFactory(factory), m_strCallback(strCallback)
    { start(epNormal); }

private:
	virtual void run()
    {
        common::CAutoPtr<net::INetRequest> pNetRequest = m_ptrFactory->createNetRequest();
        NetResponse( resp, pNetRequest->pushData( m_strCallback, "", null ));
        delete this;
    }
};

void CRhodesApp::callAppActiveCallback()
{
    /*String strCallback = RHOCONF().getString("app_did_become_active_callback");
    if ( strCallback.length() == 0 )
        return;

    new CRhoCallbackCall( canonicalizeRhoUrl(strCallback), createClassFactory() );*/
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

    common::CAutoPtr<common::IRhoClassFactory> ptrFactory = createClassFactory();
    common::CAutoPtr<net::INetRequest> pNetRequest = ptrFactory->createNetRequest();
    NetResponse( resp, pNetRequest->pushData( strCallbackUrl, strBody, null ));
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

    common::CAutoPtr<common::IRhoClassFactory> ptrFactory = createClassFactory();
    common::CAutoPtr<net::INetRequest> pNetRequest = ptrFactory->createNetRequest();
    NetResponse( resp, pNetRequest->pushData( strCallbackUrl, strBody, null ));
}

static void callback_geolocation(struct shttpd_arg *arg) 
{
	if (!geo_known_position())
	{
		rho_http_sendresponse(arg, "Reading;Reading;Reading");
		return;
	}
	
	double latitude = geo_latitude();
	double longitude = geo_longitude();

    char location[256];
	sprintf(location,"%.4f\xb0 %s, %.4f\xb0 %s;%f;%f",
		fabs(latitude),latitude < 0 ? "South" : "North",
		fabs(longitude),longitude < 0 ? "West" : "East",
		latitude,longitude);

    LOGC(INFO,CRhodesApp::getLogCategory())+ "Location: " + location;
    rho_http_sendresponse(arg, location);
}

static void callback_syncdb(struct shttpd_arg *arg) 
{
    rho_sync_doSyncAllSources(1);
    rho_http_sendresponse(arg, "");
}

static void callback_redirect_to(struct shttpd_arg *arg) 
{
    String strQuery = shttpd_get_env(arg,"QUERY_STRING");
    int nUrl = strQuery.find_first_of("url=");
    String strUrl;
    if ( nUrl != String::npos )
        strUrl = strQuery.substr(nUrl+4);

    if ( strUrl.length() == 0 )
        strUrl = "/app/";

    rho_http_redirect(arg,strUrl.c_str());
}

static void callback_map(struct shttpd_arg *arg) 
{
    rho_map_location( const_cast<char*>(shttpd_get_env(arg,"QUERY_STRING")) );
    rho_http_sendresponse(arg, "");
}

static void callback_shared(struct shttpd_arg *arg) 
{
    rho_http_senderror(arg, 404, "Not Found");
}

static void callback_AppManager_load(struct shttpd_arg *arg) 
{
    rho_appmanager_load( arg, shttpd_get_env(arg,"QUERY_STRING") );
}

void CRhodesApp::initHttpServer()
{
  LOG(INFO) + "Init http server";
  m_shttpdCtx = shttpd_init(0,NULL);

  String strAppRootPath = getRhoRootPath() + "apps";

  shttpd_set_option(m_shttpdCtx, "root", strAppRootPath.c_str());
  shttpd_set_option(m_shttpdCtx, "ports", getFreeListeningPort());

  //shttpd_register_uri(m_shttpdCtx, "/system/geolocation", &CGPSController::show_geolocation, NULL);
  shttpd_register_uri(m_shttpdCtx, "/system/geolocation", callback_geolocation, this);
  shttpd_register_uri(m_shttpdCtx, "/system/syncdb", callback_syncdb, this);
  shttpd_register_uri(m_shttpdCtx, "/system/redirect_to", callback_redirect_to, this);
  shttpd_register_uri(m_shttpdCtx, "/system/map", callback_map, this);
  shttpd_register_uri(m_shttpdCtx, "/system/shared", callback_shared, this);
  shttpd_register_uri(m_shttpdCtx, "/AppManager/loader/load", callback_AppManager_load, this);
}

const char* CRhodesApp::getFreeListeningPort()
{
#ifdef OS_MACOSX
    return "8080";
#else
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
		    serv_addr.sin_port = htons(0);

		    if ( bind( sockfd, (struct sockaddr *) &serv_addr, sizeof( serv_addr ) ) )
		    {
			    LOG(WARNING) + "Unable to bind";
			    noerrors = 0;
		    }
		    else
		    {
			    char buf[10] = {0};
			    /*socklen_t*/int length = sizeof( serv_addr );

			    getsockname( sockfd, (struct sockaddr *)&serv_addr, &length );
				
			    sprintf(buf,"%d",ntohs(serv_addr.sin_port));
				
			    m_strListeningPorts = buf;
		    }
		    //Clean up
		    closesocket(sockfd);
	    }

    }

    if ( !noerrors )
	    m_strListeningPorts = "8080";
	
    return m_strListeningPorts.c_str();
#endif
}

void CRhodesApp::initAppUrls() 
{
    m_strHomeUrl = "http://localhost:";
    m_strHomeUrl += getFreeListeningPort();

    m_strBlobsDirPath = getRhoRootPath() + "apps/public/db-files";
    m_strLoadingPagePath = "file://" + getRhoRootPath() + "apps/loading.html"; 
}

void CRhodesApp::keepLastVisitedUrl(String strUrl)
{
	LOG(INFO) + "Current URL: " + strUrl;
	
	m_strCurrentUrl = strUrl;

    if ( RHOCONF().getBool("KeepTrackOfLastVisitedPage") )
    {
        if ( strUrl.compare( 0, m_strHomeUrl.length(), m_strHomeUrl ) == 0 )
            strUrl = strUrl.substr(m_strHomeUrl.length());

        int nFragment = strUrl.find('#');
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

const String& CRhodesApp::getCurrentUrl()
{ 
    return m_strCurrentUrl; 
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

	if ( strncmp("http://", strUrl.c_str(), 7 ) == 0 )
		return strUrl;

    String strRes = m_strHomeUrl;
    if ( strUrl.at(0) != '/' && strUrl.at(0) != '\\' )
        strRes += '/';

    strRes += strUrl;
	
	return strRes;
}

}
}

extern "C" {

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
}

void rho_http_senderror(void* httpContext, int nError, const char* szMsg)
{
    struct shttpd_arg *arg = (struct shttpd_arg *)httpContext;

	shttpd_printf(arg, "%s", "HTTP/1.1 %d %s\r\n", nError, szMsg);
	arg->flags |= SHTTPD_END_OF_OUTPUT;
}

void rho_http_sendresponse(void* httpContext, const char* szBody)
{
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

const char* rho_rhodesapp_getoptionsurl()
{
    return RHODESAPP().getOptionsUrl().c_str();
}

void rho_rhodesapp_keeplastvisitedurl(const char* szUrl)
{
    RHODESAPP().keepLastVisitedUrl(szUrl);
}

const char* rho_rhodesapp_getcurrenturl()
{
	return RHODESAPP().getCurrentUrl().c_str();
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
    return RHODESAPP().callCameraCallback(strCallbackUrl, strImagePath, strError, bCancel != 0);
}

void rho_rhodesapp_callDateTimeCallback(const char* strCallbackUrl, long lDateTime, const char* szData, int bCancel )
{
    return RHODESAPP().callDateTimeCallback(strCallbackUrl, lDateTime, szData, bCancel != 0);
}

void rho_rhodesapp_callAppActiveCallback()
{
    return RHODESAPP().callAppActiveCallback();
}

}