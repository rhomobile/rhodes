#pragma once
#include "../stdafx.h"
#include "../atlutil.h" //Using local copy, as 'standard' atlutil has errors

#include "shttpd.h"
#include "sqlite3.h"

class CSyncEngine : public IWorkerThreadClient
{
  HANDLE m_hEvent;
  HANDLE m_hDoSyncEvent;

public:
  CSyncEngine(void);
  virtual ~CSyncEngine(void);

  CWorkerThread<DefaultThreadTraits> m_thread;

	void ResumeThread();
	void FreezeThread();

private:
	HRESULT Execute(DWORD_PTR dwParam, HANDLE hObject);
	HRESULT CloseHandle(HANDLE hHandle);

  sqlite3 *m_database;
  bool  m_bSyncInitialized;
  bool  StartSyncEngine();
  bool  PerformSync();
  bool  StopSyncEngine();
};

#ifdef __cplusplus
extern "C" {
#endif

char* fetch_remote_data(char* url);
int push_remote_data(char* url, char* data, size_t data_size);

#ifdef __cplusplus
} //extern "C" {
#endif
