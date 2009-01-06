#include "ServerHost.h"

CServerHost::CServerHost()
{
  m_syncengine = CSyncEngine::Create();
  m_httpserver = CHttpServer::Create();
}

CServerHost::~CServerHost(void)
{
  if (m_httpserver)
    delete m_httpserver;
  if (m_syncengine)
    delete m_syncengine;
}

bool CServerHost::Start(HWND hWnd) {
  // Start HTTP server 
  m_httpserver->ResumeThread();
  //Start sync engine
  m_syncengine->SetMainWindow(hWnd);
  m_syncengine->ResumeThread();
  return true;
}

void CServerHost::Stop() {
//  m_httpserver->FreezeThread();
//  m_syncengine->FreezeThread();
}
