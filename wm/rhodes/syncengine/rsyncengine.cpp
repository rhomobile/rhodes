#include "../stdafx.h"
#include <connmgr.h>
#include <wininet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "../resource.h"
#include "../HttpServer.h"
#include "SyncEngine.h"
#include "SyncUtil.h"
#include "rsyncengine.h"
#include "rhoruby/rhoruby.h"

// Sync engine ruby extension hooks
#ifdef __cplusplus
extern "C" {
#endif
#include "SyncManagerI.h"

void lock_sync_mutex() {
  CSyncEngine* sync = CSyncEngine::Instance();
  if (sync) sync->Lock();
}

void unlock_sync_mutex() {
  CSyncEngine* sync = CSyncEngine::Instance();
  if (sync) sync->Unlock();
}

void dosync() {
  CSyncEngine* sync = CSyncEngine::Instance();
  if (sync) sync->TriggerSync();
}

void start_sync() {
  CSyncEngine* sync = CSyncEngine::Instance();
  if (sync) sync->StartSync();
}

/*int login(const char*arg1, const char* arg2){
  return db_login(arg1, arg2);
}*/

#ifdef __cplusplus
} //extern "C" {
#endif

// Instance of the sync engine
static CSyncEngine* m_instance = NULL;

//Sync engine thread
CSyncEngine* CSyncEngine::Create() {
  if (m_instance) 
    return m_instance;
  m_instance = new CSyncEngine;
  return m_instance;
}

CSyncEngine* CSyncEngine::Instance() {
  return m_instance;
}

CSyncEngine::CSyncEngine(void)
{
  InitializeCriticalSection(&m_critical_section);
  m_hMainWindow = NULL;
  m_database = NULL;
  stop_running = 1;
  m_delaySync = 0;
  m_dbResetDelay = 0;
  m_bSyncInitialized = false;
  m_hDoSyncEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_thread.Initialize();
  m_hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
  m_thread.AddHandle(m_hEvent, this, NULL);
}

CSyncEngine::~CSyncEngine(void)
{
  //Lock();
  stop_running = 1;
  ::SetEvent(m_hDoSyncEvent);
  //Unlock();

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
//  ATLTRACE(_T("Sync engine thread locked\n"));
}

void CSyncEngine::Unlock() {
  LeaveCriticalSection(&m_critical_section);
//  ATLTRACE(_T("Sync engine thread unlocked\n"));
}

void CSyncEngine::TriggerSync() {
  //Lock();
  ::SetEvent(m_hDoSyncEvent);
  //Unlock();
}

void CSyncEngine::StartSync() {
  //Lock();
  stop_running = 0;
  ::SetEvent(m_hDoSyncEvent);
  //Unlock();
}

void CSyncEngine::ShowHomePage()
{
  if (m_hMainWindow) {
    ::PostMessage(m_hMainWindow,WM_COMMAND,IDM_HOME,0);
    m_hMainWindow = NULL;
  }
}

HRESULT CSyncEngine::Execute(DWORD_PTR dwParam, HANDLE hObject)
{
  if (!m_bSyncInitialized) {
    StartSyncEngine();
    //WaitForSingleObject(m_hDoSyncEvent,INFINITE);
  }//else

  ATLTRACE(_T("Wait Sync timeout\n"));
  WaitForSingleObject(m_hDoSyncEvent,WAIT_TIME_SECONDS*1000);
  
  PerformSync();
  //ShowHomePage();

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
  ATLTRACE(_T("Starting sync engine\n"));
  char dbpath[MAX_PATH];
  sprintf(dbpath,"%sdb\\syncdb.sqlite",RhoGetRootPath());

  Lock();
  sqlite3_open(dbpath,&m_database);
  Unlock();

  // Set the delay based on records from the database
  m_delaySync = get_object_count_from_database(m_database);
  start_sync_engine(m_database);
  m_bSyncInitialized = true;
  stop_running = 0;

  return true;
}

bool CSyncEngine::PerformSync()
{
  //Lock();

  ATLTRACE(_T("Performing sync\n"));
  ::ResetEvent(m_hDoSyncEvent);
  if (!m_delaySync && !m_dbResetDelay) {
	  process_local_changes();
  } else if (m_dbResetDelay) {
	/* reset db for next iteration */
	reset_sync_db();
	clear_client_id();
	m_dbResetDelay = 0;
	m_delaySync = 0;
  } else {
    m_delaySync = 0;
  }

  //Unlock();
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

void CSyncEngine::TriggerDbReset()
{
  m_dbResetDelay = 1;
}

void ErrorMessage(LPTSTR pszFunction)
{ 
    // Retrieve the system error message for the last-error code

    LPTSTR pszMessage = NULL;
    DWORD dwLastError = GetLastError(); 

    DWORD dwRes = FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | 
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dwLastError,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&pszMessage,
        0, NULL );

    // Display the error message and exit the process
    if ( dwRes == 0 )
      wprintf(L"%s failed with error %d\n", pszFunction, dwLastError);
    else{
      wprintf(L"%s failed with error %d: %s\n", pszFunction, dwLastError, pszMessage);
      LocalFree(pszMessage);
    }
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

char* remote_data(LPWSTR verb, char* url, char* body, size_t body_size, 
				  bool bGetHeaders, bool bGetRawData = false, bool bCheckSession = false, DWORD* pdwDataSize = NULL) {
  char       *cstr = NULL;
  char		 *session = NULL;
  std::string data = "";
  //CAtlStringA data;
  char        sBuf[1024];
  DWORD       dwBytesRead  = 0;
  LPWSTR      urlw;
  HINTERNET   hInet, hConnection, hRequest;
  LPTSTR      pszFunction = NULL;

  if ( url==NULL || strlen(url)==0 ) return NULL;

  urlw = wce_mbtowc(url);  
  hInet = hConnection = hRequest = NULL;

  do {
	  // Don't make a connection attempt if there is no session
    session = get_db_session(load_source_url());
	  if ( bCheckSession && !session && !strstr(url, "clientcreate") ) {
	    break;
	  }
	  if (session) free(session);

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
      INTERNET_FLAG_KEEP_CONNECTION|INTERNET_FLAG_NO_CACHE_WRITE, NULL );
    if ( !hRequest ) {
      pszFunction = L"HttpOpenRequest";
      free_url_components(&uri);
      break;
    }

    free_url_components(&uri);

    //Send data
    if ( HttpSendRequest( hRequest, NULL, 0, body, body_size) ) {
      wchar_t res[10];
      DWORD dwLen = 10;
      DWORD nIndex = 0;
      bool bOk = false;
      if( HttpQueryInfo(hRequest,HTTP_QUERY_STATUS_CODE,res,&dwLen,&nIndex) ){
        if ( wcscmp(res,L"200") == 0 )
          bOk = true;
		else {
          bOk = false;
		  // If we're unauthorized, delete any cookies that might have been
		  // stored so we don't reuse them later
		  if ( wcscmp(res,L"401") == 0 ) delete_winmo_session(load_source_url());
		}
      }

      if ( bOk ){
        if ( bGetHeaders ){
          DWORD dwSize = 0;
	        HttpQueryInfo(hRequest, HTTP_QUERY_RAW_HEADERS, NULL, &dwSize, NULL);
	        if( dwSize != 0 )
	        {
		        cstr = new char [dwSize+1];
		        // Call HttpQueryInfo again to get the headers.
		        bOk = (bool) HttpQueryInfoA(hRequest, HTTP_QUERY_RAW_HEADERS, (LPVOID) cstr, &dwSize, NULL);
          }
        }else{
          BOOL bRead = InternetReadFile(hRequest, &sBuf, sizeof(sBuf), &dwBytesRead);
          while (bRead && (dwBytesRead > 0)) {
            data.append(sBuf, dwBytesRead);
            //data.Append(sBuf, dwBytesRead);
            bRead = InternetReadFile(hRequest, &sBuf, sizeof(sBuf), &dwBytesRead);
          }
		      if ( bGetRawData && pdwDataSize ){
			      cstr = new char [*pdwDataSize];
            memcpy (cstr, data.c_str(), *pdwDataSize);
		      }
		      else {
			      //make a copy of recieved data
			      cstr = new char [data.size()+1];
			      strcpy (cstr, data.c_str());
            //cstr = new char [data.GetLength()+1];
            //strcpy (cstr, data.GetString());
		      }
        }
      }
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
  return remote_data(L"GET", url, NULL, 0, false, false, true);
}

int push_remote_data(char* url, char* data, size_t data_size) {
  char* res = remote_data(L"POST", url, data, data_size, false, false, true);
  int  nRes = res==NULL ? 1 : 0;
  if ( res )
      free(res);
  return nRes;
  //return remote_data(L"POST", url, data, data_size, false, false, true)==NULL ? 1 : 0;
}

int makeLoginRequest(char* url, char* data ){
  char* res = remote_data(L"POST", url, data, strlen(data), false, false, false);
  int  nRes = res==NULL ? 1 : 0;
  if ( res )
      free(res);
  return nRes;

  //return remote_data(L"POST", url, data, strlen(data), false, false, false)==NULL ? 0 : 1;
}

void delete_winmo_session(const char *url_string) {
  BOOL bReturn;
  // Delete the session cookie.
  LPTSTR url = wce_mbtowc(url_string);
  bReturn = InternetSetCookie(url, NULL, L"");
  free(url);
}

void triggerSyncDbReset() {
  CSyncEngine* sync = CSyncEngine::Instance();
  if (sync) {
	sync->TriggerDbReset();
	sync->TriggerSync();
  }
}