#pragma once

#if (_MSC_VER == 1400)
#include "../atlutil.h" //Using local copy, as 'standard' atlutil has errors
#else
#include "../atlutil-vc08.h" //Using local copy, as 'standard' atlutil has errors
#endif

#include "logging/RhoLog.h"

struct sqlite3;
class CSyncEngine : public IWorkerThreadClient
{
  DEFINE_LOGCLASS;

  HANDLE m_hEvent;
  HANDLE m_hDoSyncEvent;
  CRITICAL_SECTION m_critical_section;

public:
  static CSyncEngine* Create();
  static CSyncEngine* Instance();
  virtual ~CSyncEngine(void);

  CWorkerThread<DefaultThreadTraits> m_thread;

	void ResumeThread();
	void FreezeThread();

  void SetMainWindow(HWND hWnd) { m_hMainWindow = hWnd; }
  void ShowStartPage();

  void Lock();
  void Unlock();
  void TriggerSync();
//  void StartSync();
  void TriggerDbReset();

    static bool SetupInternetConnection(LPCTSTR url);
    static char* remote_data(LPWSTR verb, char* url, char* body, size_t body_size, 
				  bool bGetHeaders, bool bGetRawData = false, bool bCheckSession = false, DWORD* pdwDataSize = NULL,
                  char* contentType = NULL);

private:
  CSyncEngine(void);

private:
  HWND m_hMainWindow;

  HRESULT Execute(DWORD_PTR dwParam, HANDLE hObject);
	HRESULT CloseHandle(HANDLE hHandle);

  sqlite3 *m_database;
  //int	m_delaySync;
  int   m_dbResetDelay;
  bool  m_bSyncInitialized;
  bool  StartSyncEngine();
  bool  PerformSync();
  bool  StopSyncEngine();
};

#ifdef __cplusplus
extern "C" {
#endif

char* fetch_remote_data(char* url);
//unsigned char* fetch_remote_rawdata(char* url, unsigned long* pnSize);
int push_remote_data(char* url, char* data, size_t data_size,char* contentType);
//void start_sync();
void delete_winmo_session(const char *url_string);
int has_network_impl();
void triggerSyncDbReset();
void perform_notification(char* callback, char* params);

#ifdef __cplusplus
} //extern "C" {
#endif
