#pragma once

class CHttpServer;
/*namespace rho{
namespace sync{
    class CSyncThread;
};
};*/

class CServerHost
{
public:
  CServerHost(void);
  virtual ~CServerHost(void);

  bool Start(HWND hWnd);
  void Stop ();

private:
    CHttpServer* m_httpserver;
    //rho::sync::CSyncThread* m_syncThread;
};
