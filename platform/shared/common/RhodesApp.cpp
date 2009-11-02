#include "RhodesApp.h"
#include "common/RhoMutexLock.h"
#include "common/IRhoClassFactory.h"
#include "common/RhoConf.h"
#include "ext/rho/rhoruby.h"

#ifdef OS_WINCE
#include <winsock.h>
#endif 
#include "shttpd/src/shttpd.h"

extern "C" {
void rho_sync_create();
void rho_sync_destroy();
}

namespace rho {
namespace common{

IMPLEMENT_LOGCLASS(CRhodesApp,"RhodesApp");
CRhodesApp* CRhodesApp::m_pInstance = 0;

/*static*/ CRhodesApp* CRhodesApp::Create(const String& strRootPath, IRhoBrowser* pRhoBrowser)
{
    if ( m_pInstance ) 
        return m_pInstance;

    m_pInstance = new CRhodesApp(strRootPath,pRhoBrowser);
    return m_pInstance;
}

/*static*/void CRhodesApp::Destroy()
{
    if ( m_pInstance )
        delete m_pInstance;

    m_pInstance = 0;
}

CRhodesApp::CRhodesApp(const String& strRootPath, IRhoBrowser* pRhoBrowser) : CRhoThread(createClassFactory())
{
    m_strRhoRootPath = strRootPath;
    m_ptrRhoBrowser = pRhoBrowser;
    m_bExit = false;

    rho_logconf_Init(getRhoRootPath().c_str());

    m_shttpdCtx = 0;

#ifdef OS_WINCE
    //initializing winsock
    WSADATA WsaData;
    int result = WSAStartup(MAKEWORD(1,1),&WsaData);
#endif

    initAppUrls();

    start(epLow);
}

void CRhodesApp::run()
{
	LOG(INFO) + "Starting RhodesApp main routine...";

    initHttpServer();
    RhoRubyStart();

    m_ptrRhoBrowser->navigateUrl(getFirstStartUrl());

    rho_sync_create();
    RhoRubyInitApp();

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
    exitApp();

#ifdef OS_WINCE
    WSACleanup();
#endif

}

void CRhodesApp::exitApp()
{
    if (!m_bExit)
    {
        m_bExit = true;
        shutdown_poll(m_shttpdCtx);

        stop(2000);
    }
}

void CRhodesApp::initHttpServer()
{
  LOG(INFO) + "Init http server";
  m_shttpdCtx = shttpd_init(0,NULL);

  String strAppRootPath = getRhoRootPath() + "apps";

  shttpd_set_option(m_shttpdCtx, "root", strAppRootPath.c_str());
  shttpd_set_option(m_shttpdCtx, "ports", getFreeListeningPort());

//#if defined(OS_WINCE)
//  shttpd_register_uri(m_shttpdCtx, "/system/geolocation", &CGPSController::show_geolocation, NULL);
//#endif
}

const char* CRhodesApp::getFreeListeningPort()
{
    if ( m_strListeningPorts.length() > 0 )
        return m_strListeningPorts.c_str();

    int noerrors = 1;
    LOG(INFO) + "Trying to get free listening port.";
	
    //get free port
    SOCKET sockfd = -1;
    struct sockaddr_in serv_addr = {0};
    struct hostent *server = {0};
    int result = -1;

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
		    server = gethostbyname( "localhost" );

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
			    int length = sizeof( serv_addr );

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
}

const String& CRhodesApp::getRhoRootPath()
{
    if ( m_strRhoRootPath.length() == 0 )
        m_strRhoRootPath = getSystemRootPath() + "rho/";

    return m_strRhoRootPath;
}

String CRhodesApp::getSystemRootPath()
{
#if defined (OS_WINDOWS) || defined(OS_WINCE)
  char rootpath[MAX_PATH];
  int len;
  if ( (len = GetModuleFileNameA(NULL,rootpath,MAX_PATH)) == 0 )
    strcpy(rootpath,".");
  else
  {
    while( !(rootpath[len] == '\\'  || rootpath[len] == '/') )
      len--;
    rootpath[len+1]=0;
  }

  return rootpath;
#endif

}

void CRhodesApp::initAppUrls() 
{
    m_strHomeUrl = "http://localhost:";
    m_strHomeUrl += getFreeListeningPort();

    m_strStartUrl = canonicalizeRhoUrl( RHOCONF().getString("start_path") );
    convertToStringW(m_strStartUrl.c_str(), m_strStartUrlW);

    m_strOptionsUrl = canonicalizeRhoUrl( RHOCONF().getString("options_path") );
    convertToStringW( m_strOptionsUrl.c_str(), m_strOptionsUrlW );

    m_strRhobundleReloadUrl = RHOCONF().getString("rhobundle_zip_url");
}

String CRhodesApp::getFirstStartUrl()
{
    rho::String strLastPage;
	if ( RHOCONF().getBool("KeepTrackOfLastVisitedPage") ) 
    {
	    strLastPage = RHOCONF().getString("LastVisitedPage");
	    if (strLastPage.length() > 0)
            strLastPage = canonicalizeRhoUrl(strLastPage);
    }

    return strLastPage.length() > 0 ? strLastPage : m_strStartUrl;
}

void CRhodesApp::keepLastVisitedUrl(String strUrl)
{
    if ( RHOCONF().getBool("KeepTrackOfLastVisitedPage") )
    {
        if ( strUrl.compare( 0, m_strHomeUrl.length(), m_strHomeUrl ) == 0 )
            strUrl = strUrl.substr(m_strHomeUrl.length());

	    RHOCONF().setString("LastVisitedPage",strUrl);		
	    RHOCONF().saveToFile();
    }
}

const String& CRhodesApp::getRhobundleReloadUrl() 
{
	return m_strRhobundleReloadUrl;
}

const String& CRhodesApp::getStartUrl()
{
    return m_strStartUrl;
}

const StringW& CRhodesApp::getStartUrlW()
{
    return m_strStartUrlW;
}

const StringW& CRhodesApp::getOptionsUrlW()
{
    return m_strOptionsUrlW;
}

void CRhodesApp::navigateToUrl( const String& strUrl)
{
    m_ptrRhoBrowser->navigateUrl(canonicalizeRhoUrl(strUrl));
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

String CRhodesApp::getLoadingPagePath() 
{
    String strRes = "file://" + getRhoRootPath() + "apps/loading.html"; 
    return strRes;
}

const char* CRhodesApp::getRelativeBlobsPath() 
{
    return "apps/public/db-files";
}

const wchar_t* CRhodesApp::getRelativeBlobsPathW() 
{
    return L"apps/public/db-files";
}

}
}

extern "C" {

char* HTTPResolveUrl(char* szUrl) 
{
    rho::String strRes = RHODESAPP().canonicalizeRhoUrl(szUrl);
    free(szUrl);
    return strdup(strRes.c_str());
}
}