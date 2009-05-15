#include "stdafx.h"

#include "ServerHost.h"
#include "HttpServer.h"
//#include "rho/sync/SyncThread.h"

//using namespace rho::sync;

CServerHost::CServerHost()
{
    //m_syncThread = CSyncThread::Create();
    m_httpserver = CHttpServer::Create();
}

CServerHost::~CServerHost(void)
{
  if (m_httpserver)
    delete m_httpserver;
  //if (m_syncThread)
  //  delete m_syncThread;
}

bool CServerHost::Start(HWND hWnd) 
{
  // Start HTTP server 
  m_httpserver->SetMainWindow(hWnd);
  m_httpserver->ResumeThread();  
  //Start sync engine
  //m_syncThread->SetMainWindow(hWnd);
  //m_syncThread->ResumeThread();
  return true;
}

void CServerHost::Stop() {
//  m_httpserver->FreezeThread();
//  m_syncengine->FreezeThread();
}
