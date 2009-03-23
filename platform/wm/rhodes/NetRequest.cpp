#include "stdafx.h"
#include <string>
#include "SyncUtil.h"
#include "syncengine/rsyncengine.h"
#include "NetRequest.h"

#ifdef _MSC_VER
// warning C4800: 'int' : forcing to bool 'true' or 'false' (performance warning)
#pragma warning ( disable : 4800 )
#endif

extern "C" wchar_t* wce_mbtowc(const char* a);

CNetRequest::CNetRequest(bool local) {
	m_hConnection = NULL;
	m_localhostConn = local;
}

CNetRequest::~CNetRequest(void) {
}

bool CNetRequest::SetupInternetConnection(LPCTSTR url)
{
	int iNetwork;
	HRESULT hResult = E_FAIL;
	DWORD   dwStatus;

	// cleanup the old connection
	if(NULL != m_hConnection)
	{
		hResult=ConnMgrConnectionStatus(m_hConnection,&dwStatus);
		if( SUCCEEDED(hResult) )
		{
			ATLTRACE(L"Internet connection exist, use it\n");
			if( dwStatus & CONNMGR_STATUS_CONNECTED )
				return true;
		}
		ConnMgrReleaseConnection(m_hConnection, FALSE);
		ATLTRACE(L"Internet connection droped, open new one\n");
		m_hConnection = NULL;
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

		hResult = ConnMgrEstablishConnection(&ConnInfo, &m_hConnection);

		// check to see if the attempt failed
		int count = 0;
		while(SUCCEEDED(hResult) && count++ < 60 )
		{
			ATLTRACE(L"Wait for connect (%d).\n",count);
			DWORD dwResult = WaitForSingleObject(m_hConnection, 1000); 
			if (dwResult == (WAIT_OBJECT_0))
			{ 
				hResult=ConnMgrConnectionStatus(m_hConnection,&dwStatus);
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

void CNetRequest::alloc_url_components(URL_COMPONENTS *uri, char *url) {
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

void CNetRequest::free_url_components(URL_COMPONENTS *uri) {
  free(uri->lpszScheme);
  free(uri->lpszHostName);
  free(uri->lpszUserName);
  free(uri->lpszPassword);
  free(uri->lpszUrlPath);
  free(uri->lpszExtraInfo);
}

void CNetRequest::ErrorMessage(LPTSTR pszFunction)
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
	if ( dwRes == 0 ) {
		wprintf(L"%s failed with error %d\n", pszFunction, dwLastError);
	} else {
		wprintf(L"%s failed with error %d: %s\n", pszFunction, dwLastError, pszMessage);
		LocalFree(pszMessage);
    }
}

char* CNetRequest::doRequest(LPWSTR verb, char* url, 
							 char* headers, size_t headers_size,
							 char* body, size_t body_size, 
							 bool bGetHeaders, bool bGetRawData, 
							 bool bCheckSession, DWORD* pdwDataSize) {
	char	*cstr = NULL;
	char	*session = NULL;
	std::string data = "";
	//CAtlStringA data;
	char        sBuf[1024];
	DWORD       dwBytesRead  = 0;
	LPWSTR      urlw;
	HINTERNET   hInet, hConnection, hRequest;
	LPTSTR      pszFunction = NULL;
	LPTSTR		headersw;

	if ( url==NULL || strlen(url)==0 ) {
		return NULL;
	}

	urlw = wce_mbtowc(url);
	headersw = wce_mbtowc(headers);
	hInet = hConnection = hRequest = NULL;

	do {
		if (bCheckSession) {
			// Don't make a connection attempt if there is no session
			session = get_db_session(load_source_url());
			if (!session && !strstr(url, "clientcreate") ) {
				break;
			}
			if (session) 
				free(session);
		}

		if( !m_localhostConn && !SetupInternetConnection(urlw) ) {
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
		if ( HttpSendRequest( hRequest, headersw, headers_size, body, body_size) ) {
			wchar_t res[10];
			DWORD dwLen = 10;
			DWORD nIndex = 0;
			bool bOk = false;
			
			if( HttpQueryInfo(hRequest,HTTP_QUERY_STATUS_CODE,res,&dwLen,&nIndex) ){
				if ( wcscmp(res,L"200") == 0 ) {
					bOk = true;
				} else {
					bOk = false;
					if (bCheckSession) {
						// If we're unauthorized, delete any cookies that might have been
						// stored so we don't reuse them later
						if ( wcscmp(res,L"401") == 0 ) {
							delete_winmo_session(load_source_url());
						}
					}
				}
			}

			if ( bOk ) {
				if ( bGetHeaders ) {
					DWORD dwSize = 0;
					HttpQueryInfo(hRequest, HTTP_QUERY_RAW_HEADERS, NULL, &dwSize, NULL);
					if( dwSize != 0 ) {
						cstr = new char [dwSize+1];
						// Call HttpQueryInfo again to get the headers.
						bOk = (bool) HttpQueryInfoA(hRequest, HTTP_QUERY_RAW_HEADERS, (LPVOID) cstr, &dwSize, NULL);
					}
				} else {
					BOOL bRead = InternetReadFile(hRequest, &sBuf, sizeof(sBuf), &dwBytesRead);
					while (bRead && (dwBytesRead > 0)) {
						data.append(sBuf, dwBytesRead);
						//data.Append(sBuf, dwBytesRead);
						bRead = InternetReadFile(hRequest, &sBuf, sizeof(sBuf), &dwBytesRead);
					}
					if ( bGetRawData && pdwDataSize ) {
						cstr = new char [*pdwDataSize];
						memcpy (cstr, data.c_str(), *pdwDataSize);
					} else {
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
	free(headersw);

	return cstr;
}
