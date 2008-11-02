#pragma once
#include "stdafx.h"
#include "atlutil.h" //Using local copy, as 'standard' atlutil has errors

#include "shttpd.h"

class CHttpServer : public IWorkerThreadClient
{
  HANDLE m_hEvent;

public:
  CHttpServer(void);
  virtual ~CHttpServer(void);

  CWorkerThread<DefaultThreadTraits> m_thread;

	void ResumeThread();
	void FreezeThread();
  
  static const char* GetRootPath();
  //returns same buffer filled w/ path to lading page
  static LPTSTR GetLoadingPage(LPTSTR buffer); 

private:
	HRESULT Execute(DWORD_PTR dwParam, HANDLE hObject);
	HRESULT CloseHandle(HANDLE hHandle);

  struct shttpd_ctx	*ctx;
	bool  InitHttpServer();

  bool  m_bRubyInitialized;
  bool  InitRubyFramework();
};
