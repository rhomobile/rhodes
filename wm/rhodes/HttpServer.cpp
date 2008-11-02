#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include "rhoruby/rhoruby.h"
#include "HttpServer.h"
#include "syncengine/rsyncengine.h"

CHttpServer::CHttpServer(void)
{
  m_bRubyInitialized = false;
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
    ATLTRACE(L"Starting SYNC\n");
    start_sync();
  }
	shttpd_poll(ctx, 1000);
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
  const char *rootpath = GetRootPath();
  sprintf(httproot,"%sapps",rootpath);
  shttpd_set_option(ctx, "root",httproot);

  return true;
}

bool CHttpServer::InitRubyFramework() {
  ATLTRACE(_T("Init ruby framework\n"));
  RhoRubyStart(GetRootPath());
  m_bRubyInitialized = true;
  return true;
}

extern DWORD GetModuleFileNameA(HMODULE hModule,LPSTR lpFileName,DWORD size);
static bool _root_loaded = false;
static char _rootpath[MAX_PATH];
const char* CHttpServer::GetRootPath() {
  if (_root_loaded) {
    return _rootpath;
  }

  int len;
  if( (len = GetModuleFileNameA(NULL,_rootpath,MAX_PATH)) == 0 )
  {
    strcpy(_rootpath,".");
  }
  else
  {
    while( !(_rootpath[len] == _T('\\')  || _rootpath[len] == _T('/')) )
      len--;
    _rootpath[len]=0;
    sprintf(_rootpath,"%s\\rho\\",_rootpath);
  }
  _root_loaded = true;
  return _rootpath;
}

extern "C" wchar_t* wce_mbtowc(const char* a);
LPTSTR CHttpServer::GetLoadingPage(LPTSTR buffer) {
  if (buffer) {
    wchar_t* root  = wce_mbtowc(GetRootPath());
    wsprintf(buffer,L"file://%s%s",root,L"apps\\loading.html");
    free(root);
  }
  return buffer;
}

