#include "../stdafx.h"
#include <connmgr.h>
#include <wininet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "../HttpServer.h"
#include "SyncEngine.h"
#include "rsyncengine.h"

// Sync engine ruby extension hooks
#ifdef __cplusplus
extern "C" {
#endif

void lock_sync_mutex() {
  CSyncEngine::Instance()->Lock();
}

void unlock_sync_mutex() {
  CSyncEngine::Instance()->Unlock();
}

void dosync() {
  CSyncEngine::Instance()->TriggerSync();
}

#ifdef __cplusplus
} //extern "C" {
#endif

// Instance of the sync engine
static CSyncEngine* m_instance = NULL;

//Sync engine thread

CSyncEngine* CSyncEngine::Instance() {
  if (m_instance) 
    return m_instance;
  m_instance = new CSyncEngine;
  return m_instance;
}

CSyncEngine::CSyncEngine(void)
{
  InitializeCriticalSection(&m_critical_section);
  m_database = NULL;
  m_bSyncInitialized = false;
  m_hDoSyncEvent = CreateEvent(NULL, TRUE, TRUE, NULL);
	m_thread.Initialize();
  m_hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
  m_thread.AddHandle(m_hEvent, this, NULL);
}

CSyncEngine::~CSyncEngine(void)
{
  //TBD InterlockedDecrement()
  Lock();
  stop_running = 1;
  ::SetEvent(m_hDoSyncEvent);
  Unlock();

	m_thread.RemoveHandle(m_hEvent);
	m_thread.Shutdown();
  ATLTRACE(_T("Sync engine thread shutdown\n"));
  DeleteCriticalSection(&m_critical_section);
}

void CSyncEngine::ResumeThread()
{
  ::SetEvent(m_hEvent); 
}

void CSyncEngine::FreezeThread()
{
  ::ResetEvent(m_hEvent);
}

void CSyncEngine::Lock() {
  EnterCriticalSection(&m_critical_section);
  ATLTRACE(_T("Sync engine thread locked\n"));
}

void CSyncEngine::Unlock() {
  LeaveCriticalSection(&m_critical_section);
  ATLTRACE(_T("Sync engine thread unlocked\n"));
}

void CSyncEngine::TriggerSync() {
  Lock();
  ::SetEvent(m_hDoSyncEvent);
  Unlock();
}

HRESULT CSyncEngine::Execute(DWORD_PTR dwParam, HANDLE hObject)
{
  if (!m_bSyncInitialized) {
    StartSyncEngine();
  }
  
  WaitForSingleObject(m_hDoSyncEvent,WAIT_TIME_SECONDS*1000);
  PerformSync();

  return S_OK;
}

HRESULT CSyncEngine::CloseHandle(HANDLE hHandle)
{
  if (m_bSyncInitialized) {
    ATLTRACE(_T("\nShutting-down sync engine\n"));
    StopSyncEngine();
  }
  ATLTRACE(_T("Closing sync engine handles\n"));
  ::CloseHandle(hHandle);
  ::CloseHandle(m_hDoSyncEvent);
  return S_OK;
}

bool CSyncEngine::StartSyncEngine()
{
  Lock();

  ATLTRACE(_T("Starting sync engine\n"));
  char dbpath[MAX_PATH];
  sprintf(dbpath,"%sdb\\syncdb.sqlite",CHttpServer::GetRootPath());
  sqlite3_open(dbpath,&m_database);
  start_sync_engine(m_database);
  m_bSyncInitialized = true;

  Unlock();
  return true;
}

bool CSyncEngine::PerformSync()
{
  Lock();

  ATLTRACE(_T("Performing sync\n"));
  ::ResetEvent(m_hDoSyncEvent);
  process_local_changes();

  Unlock();
  return true;
}

bool CSyncEngine::StopSyncEngine()
{
  Lock();

  ATLTRACE(_T("Stopping sync engine\n"));
  if (m_database)
    sqlite3_close(m_database);

  Unlock();
  return true;
}

void ErrorMessage(LPTSTR pszFunction)
{ 
    // Retrieve the system error message for the last-error code

    LPTSTR pszMessage;
    DWORD dwLastError = GetLastError(); 

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | 
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dwLastError,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&pszMessage,
        0, NULL );

    // Display the error message and exit the process

    wprintf(L"%s failed with error %d: %s\n", pszFunction, dwLastError, pszMessage);

    LocalFree(pszMessage);
}

HANDLE hConnection = NULL;

bool SetupInternetConnection(LPCTSTR url)
{
	int iNetwork;
	HRESULT hResult = E_FAIL;
	DWORD   dwStatus;

	// cleanup the old connection
	if(NULL != hConnection)
	{
		hResult=ConnMgrConnectionStatus(hConnection,&dwStatus);
		if( SUCCEEDED(hResult) )
		{
			ATLTRACE(L"Internet connection exist, use it\n");
			if( dwStatus & CONNMGR_STATUS_CONNECTED )
				return true;
		}
		ConnMgrReleaseConnection(hConnection, FALSE);
		ATLTRACE(L"Internet connection droped, open new one\n");
		hConnection = NULL;
	}

	// get the right network to connect to
	iNetwork = 0;
	//CONNMGR_DESTINATION_INFO DestInfo;

	GUID pguid;
	if( FAILED( ConnMgrMapURL(url, &pguid, NULL) ) )
		return false;

	//while( SUCCEEDED(ConnMgrEnumDestinations(iNetwork++, &DestInfo)))
	{	
		ATLTRACE(L"Try establish Internet connection \n");
		// actually try to establish the connection
		CONNMGR_CONNECTIONINFO ConnInfo;

		ZeroMemory(&ConnInfo, sizeof(ConnInfo));
		ConnInfo.cbSize = sizeof(ConnInfo);
		ConnInfo.dwParams = CONNMGR_PARAM_GUIDDESTNET;
		ConnInfo.dwPriority = CONNMGR_PRIORITY_HIPRIBKGND;//CONNMGR_PRIORITY_USERBACKGROUND;
#if ( _WIN32_WCE >= 0x500 )
		ConnInfo.dwFlags = CONNMGR_FLAG_NO_ERROR_MSGS;
#endif
		ConnInfo.guidDestNet = pguid;

		hResult = ConnMgrEstablishConnection(&ConnInfo, &hConnection);

		// check to see if the attempt failed
		int count = 0;
		while(SUCCEEDED(hResult) && count++ < 60 )
		{
			ATLTRACE(L"Wait for connect (%d).\n",count);
			DWORD dwResult = WaitForSingleObject( hConnection, 1000); 
			if (dwResult == (WAIT_OBJECT_0))
			{ 
				hResult=ConnMgrConnectionStatus(hConnection,&dwStatus);
				if( SUCCEEDED(hResult) )
				{
					if( dwStatus & CONNMGR_STATUS_CONNECTED )
					{
						ATLTRACE(L"Connected\n");
						return true;
					}
					if( dwStatus & CONNMGR_STATUS_WAITINGCONNECTION )
					{
						continue;
					}
					break;
				}
			}
		}
	}
	ATLTRACE(L"Failed to connect\n");
	return false;
}

void alloc_url_components(URL_COMPONENTS *uri, char *url) {
  int dwLength = strlen(url)*sizeof(wchar_t);
  memset(uri, 0, sizeof(URL_COMPONENTS));

  uri->dwStructSize = sizeof(URL_COMPONENTS);
  uri->lpszScheme = (LPWSTR)malloc(dwLength);
  uri->dwSchemeLength = dwLength;
  uri->lpszHostName = (LPWSTR)malloc(dwLength);
  uri->dwHostNameLength = dwLength;
  uri->lpszUserName = (LPWSTR)malloc(dwLength);
  uri->dwUserNameLength = dwLength;
  uri->lpszPassword = (LPWSTR)malloc(dwLength);
  uri->dwPasswordLength = dwLength;
  uri->lpszUrlPath = (LPWSTR)malloc(dwLength);
  uri->dwUrlPathLength = dwLength;
  uri->lpszExtraInfo = (LPWSTR)malloc(dwLength);
  uri->dwExtraInfoLength = dwLength;
}

void free_url_components(URL_COMPONENTS *uri) {
  free(uri->lpszScheme);
  free(uri->lpszHostName);
  free(uri->lpszUserName);
  free(uri->lpszPassword);
  free(uri->lpszUrlPath);
  free(uri->lpszExtraInfo);
}

extern "C" wchar_t* wce_mbtowc(const char* a);

char* remote_data(LPWSTR verb, char* url, char* body, size_t body_size) {
  char       *cstr = NULL;
  std::string data = "";
  char        sBuf[1024];
  DWORD       dwBytesRead  = 0;
  LPWSTR      urlw;
  HINTERNET   hInet, hConnection, hRequest;
  LPTSTR      pszFunction = NULL;

  if ( url==NULL || strlen(url)==0 ) return NULL;

  urlw = wce_mbtowc(url);  
  hInet = hConnection = hRequest = NULL;

  do {

    if( !SetupInternetConnection(urlw) ) {
      break;
    }

    hInet = InternetOpen(_T("rhodes-wm"), 
      INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, NULL );
    if ( !hInet ) {
      pszFunction = L"InternetOpen";
      break;
    }

    DWORD lpdwBufferLength = sizeof(sBuf)/sizeof(wchar_t);
    if ( !InternetCanonicalizeUrl(urlw, (LPWSTR)sBuf, &lpdwBufferLength, 0) ) {
      pszFunction = L"InternetCanonicalizeUrl";
      break;
    }

    ATLTRACE(L"Connecting to url: %s\n",(LPWSTR)sBuf);

    URL_COMPONENTS uri;
    alloc_url_components(&uri,url);
    if( !InternetCrackUrl((LPWSTR)sBuf,lpdwBufferLength,0,&uri) ) {
      pszFunction = L"InternetCrackUrl";
      free_url_components(&uri);
      break;
    }

    hConnection = InternetConnect( hInet, 
      uri.lpszHostName, uri.nPort, _T("anonymous"), NULL, 
      INTERNET_SERVICE_HTTP, 0, 0 );
    if ( !hConnection ) {
      pszFunction = L"InternetConnect";
      free_url_components(&uri);
      break;
    }

    wsprintf((LPWSTR)sBuf,L"%s%s",uri.lpszUrlPath,uri.lpszExtraInfo);
    hRequest = HttpOpenRequest( hConnection, verb, 
      (LPWSTR)sBuf, NULL, NULL, NULL, 
      INTERNET_FLAG_KEEP_CONNECTION, NULL );
    if ( !hRequest ) {
      pszFunction = L"HttpOpenRequest";
      free_url_components(&uri);
      break;
    }

    free_url_components(&uri);

    //Send data
    if ( HttpSendRequest( hRequest, NULL, 0, body, body_size) ) {
      BOOL bRead = InternetReadFile(hRequest, &sBuf, sizeof(sBuf), &dwBytesRead);
      while (bRead && (dwBytesRead > 0)) {
        data.append(sBuf, dwBytesRead);
        bRead = InternetReadFile(hRequest, &sBuf, sizeof(sBuf), &dwBytesRead);
      }
      //make a copy of recieved data
      cstr = new char [data.size()+1];
      strcpy (cstr, data.c_str());
    } else {
      pszFunction = L"HttpOpenRequest";
    }

  } while(0);

  if (pszFunction) {
    ErrorMessage(pszFunction);
  }

  if(hRequest) InternetCloseHandle(hRequest);
  if(hConnection) InternetCloseHandle(hConnection);
  if(hInet) InternetCloseHandle(hInet);

  free(urlw);
  return cstr;
}

char* fetch_remote_data(char* url) {
  return remote_data(L"GET", url, NULL, 0);
}

int push_remote_data(char* url, char* data, size_t data_size) {
  return remote_data(L"GET", url, NULL, 0)==NULL ? 0 : 1;
}
