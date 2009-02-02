#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include "rhoruby/rhoruby.h"
#include "HttpServer.h"
#include "syncengine/rsyncengine.h"
#include "geolocation/LocationController.h"
#include "SyncEngine.h"

static CHttpServer* m_instance = NULL;

CHttpServer* CHttpServer::Create() {
  if (m_instance) 
    return m_instance;
  m_instance = new CHttpServer;
  return m_instance;
}

CHttpServer* CHttpServer::Instance() {
  return m_instance;
}

CHttpServer::CHttpServer(void)
{
#ifdef ENABLE_DYNAMIC_RHOBUNDLE
  m_szRhobundleReloadUrl = NULL;
#endif

  m_bRubyInitialized = false;
  m_pStartPage = NULL;

  InitHttpServer();
	m_thread.Initialize();
  m_hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
  m_thread.AddHandle(m_hEvent, this, NULL);
}

CHttpServer::~CHttpServer(void)
{
  m_thread.RemoveHandle(m_hEvent);
  m_thread.Shutdown();
  shttpd_fini(ctx);
  ATLTRACE(_T("Http server thread shutdown\n"));

  CGPSController* pGPS = CGPSController::Instance();
  pGPS->DeleteInstance();
  if (m_pStartPage) {
	  free(m_pStartPage);
  }

#ifdef ENABLE_DYNAMIC_RHOBUNDLE
  if ( m_szRhobundleReloadUrl )
	  delete m_szRhobundleReloadUrl;
#endif
}

void CHttpServer::ResumeThread()
{
  ::SetEvent(m_hEvent); 
}
void CHttpServer::FreezeThread()
{
  ::ResetEvent(m_hEvent);
}

HRESULT CHttpServer::Execute(DWORD_PTR dwParam, HANDLE hObject)
{
  if (!m_bRubyInitialized) {
    InitRubyFramework();
	  InitStartPage();

#ifdef ENABLE_DYNAMIC_RHOBUNDLE
	m_szRhobundleReloadUrl = callGetRhobundleZipUrl();
#endif
    ATLTRACE(L"Starting SYNC\n");

    CSyncEngine* sync = CSyncEngine::Instance();
    if (sync) sync->ShowHomePage();

//    if (logged_in()){
      
      start_sync();
//    }  else   if (sync) sync->ShowHomePage();
  }
  shttpd_poll(ctx, 1000);

  //GPS
  CGPSController::CheckTimeout();

  return S_OK;
}

HRESULT CHttpServer::CloseHandle(HANDLE hHandle)
{
  if (m_bRubyInitialized) {
    ATLTRACE(_T("\nShutting-down ruby framework\n"));
    RhoRubyStop();
  }
  ATLTRACE(_T("Closing http server handle\n"));
  ::CloseHandle(hHandle);
  return S_OK;
}

bool CHttpServer::InitHttpServer()
{
  ATLTRACE(_T("Init http server\n"));
  ctx = shttpd_init(0,NULL);
  //
  char httproot[MAX_PATH];
  const char *rootpath = RhoGetRootPath();
  sprintf(httproot,"%sapps",rootpath);
  shttpd_set_option(ctx, "root",httproot);
  //
  shttpd_register_uri(ctx, "/system/geolocation", &show_geolocation, NULL);

  return true;
}

bool CHttpServer::InitRubyFramework() {
  ATLTRACE(_T("Init ruby framework\n"));
  RhoRubyStart();
  m_bRubyInitialized = true;
  return true;
}

extern "C" wchar_t* wce_mbtowc(const char* a);
LPTSTR CHttpServer::GetLoadingPage(LPTSTR buffer) {
  if (buffer) {
    wchar_t* root  = wce_mbtowc(RhoGetRootPath());
    wsprintf(buffer,L"file://%s%s",root,L"apps\\loading.html");
    free(root);
  }
  return buffer;
}


#define HOME_PAGE_A "http://localhost:8080"
#define HOME_PAGE_W L"http://localhost:8080/"

bool CHttpServer::InitStartPage() {
	if (m_bRubyInitialized) {
		if (m_pStartPage==NULL) {
			char* slash = "";
			char* start_page = callGetStartPage();
			if (!start_page) {
				start_page = "";
			} else if ( (*start_page!='/')&&(*start_page!='\\') ) {
				slash = "/";
			}
			int len = strlen(HOME_PAGE_A)+strlen(slash)+strlen(start_page);
			char* sp = (char*) malloc(len+1);
			sprintf(sp,"%s%s%s",HOME_PAGE_A,slash,start_page);
			m_pStartPage = wce_mbtowc(sp);
			free(sp);
		}
		return true;
	}
	return false;
}

LPTSTR CHttpServer::GetStartPage() {
	if (m_pStartPage)
		return m_pStartPage;
	else
		return HOME_PAGE_W;
}

#ifdef ENABLE_DYNAMIC_RHOBUNDLE
const char* CHttpServer::GetRhobundleReloadUrl() {
	return m_szRhobundleReloadUrl;
}
#endif