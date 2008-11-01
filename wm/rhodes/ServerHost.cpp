#include "ServerHost.h"

CServerHost::CServerHost(void)
{
  m_syncengine = CSyncEngine::Instance();
}

CServerHost::~CServerHost(void)
{
  if (m_syncengine)
    delete m_syncengine;
}

bool CServerHost::Start() {
  //Start sync engine
  m_syncengine->ResumeThread();
  // Start HTTP server 
  m_httpserver.ResumeThread();
  return true;
}

void CServerHost::Stop() {
  m_httpserver.FreezeThread();
  m_syncengine->FreezeThread();
}
