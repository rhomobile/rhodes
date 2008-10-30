#include "ServerHost.h"

CServerHost::CServerHost(void)
{
}

CServerHost::~CServerHost(void)
{
}

bool CServerHost::Start() {
  //Start sync engine
  m_syncengine.ResumeThread();
  // Start HTTP server 
  m_httpserver.ResumeThread();
  return true;
}

void CServerHost::Stop() {
  m_httpserver.FreezeThread();
  m_syncengine.FreezeThread();
}
