#include "stdafx.h"

#include "NetRequest.h"
#include "common/AutoPointer.h"

#if defined(_WIN32_WCE)
#include <connmgr.h>
#endif
#include <wininet.h>

extern "C" char* HTTPResolveUrl(char* url);

namespace rho {
namespace net {
IMPLEMENT_LOGCLASS(CNetRequest,"Net");

class CNetRequestImpl
{
    DEFINE_LOGCLASS;
    class CNetData : public INetData
    {
        bool   m_bValid;
        String m_data;

    public:
        CNetData() : m_bValid(false){}

        bool isValid()const{ return m_bValid; }
        void setValid(bool b){ m_bValid = b; }

        virtual const char* getCharData()
        {
            return m_bValid ? m_data.c_str() : null;
        }

        virtual unsigned int getDataSize()
        {
            return m_bValid ? m_data.size() : 0;
        }

        String& getRawData(){ return m_data; }
    };

    LPCTSTR  pszErrFunction;
    HINTERNET  hInet, hConnection, hRequest;
    URL_COMPONENTS uri;
    CAtlStringW strReqUrlW;
    CNetRequest& m_oNetReq;
    static CNetRequestImpl* m_pInstance;
public :
    CNetRequestImpl(CNetRequest& oNetReq, const char* method, const String& strUrl);
    ~CNetRequestImpl();

    void close();
    bool isError(){ return pszErrFunction!= null; }
    INetData* sendString(const String& strBody);
    INetData* sendStream(common::InputStream* body);

    void ErrorMessage(LPCTSTR pszFunction);
    void readInetFile( void* hRequest, CNetData* pNetData );
    void alloc_url_components(URL_COMPONENTS *uri, const wchar_t *url);
    void free_url_components(URL_COMPONENTS *uri);
    void readResponse(CNetData* pNetData);

    static void cancel()
    {
        if ( m_pInstance )
        {
            int i = 0;
            if ( m_pInstance->hRequest ) 
                i = 3;

            m_pInstance->close();
        }
    }

};
CNetRequestImpl* CNetRequestImpl::m_pInstance;
IMPLEMENT_LOGCLASS(CNetRequestImpl,"Net");

INetData* CNetRequest::pullData(const String& strUrl )
{
    return doRequest("GET",strUrl,String());
}

boolean CNetRequest::pushData(const String& strUrl, const String& strBody)
{
    common::CAutoPtr<INetData> ptrData = doRequest("POST",strUrl,strBody);
    return ptrData->getCharData() != null;
}

boolean CNetRequest::pushFile(const String& strUrl, const String& strFilePath)
{
    //TODO: pushFile
    return false;
}

boolean CNetRequest::pullCookies(const String& strUrl, const String& strBody)
{
    common::CAutoPtr<INetData> ptrData = doRequest("POST",strUrl,strBody);
    return ptrData->getCharData() != null;
}

//if strUrl.length() == 0 delete all cookies if possible
void CNetRequest::deleteCookie(const String& strUrl)
{
    if ( strUrl.length() > 0 )
        ::InternetSetCookieA(strUrl.c_str(), NULL, "");
}

String CNetRequest::resolveUrl(const String& strUrl)
{
    char* url = HTTPResolveUrl( strdup(strUrl.c_str()) );
    String res = url;
    free(url);
    return res;
}

void CNetRequest::cancelAll()
{
    CNetRequestImpl::cancel();
}

INetData* CNetRequest::doRequest( const char* method, const String& strUrl, const String& strBody )
{
    CNetRequestImpl oImpl(*this,method,strUrl);
    return oImpl.sendString(strBody);
}

CNetRequestImpl::CNetRequestImpl(CNetRequest& oNetReq, const char* method, const String& strUrl) : m_oNetReq(oNetReq)
{
    pszErrFunction = NULL;
    hInet = NULL, hConnection = NULL, hRequest = NULL;
    memset(&uri, 0, sizeof(uri) );
    m_pInstance = this;

    CAtlStringW strUrlW(strUrl.c_str());

    do 
    {
        if ( !m_oNetReq.SetupInternetConnection(strUrlW) )
        {
            pszErrFunction = L"SetupInternetConnection";
            break;
        }

        hInet = InternetOpen(_T("rhodes-wm"), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, NULL );
        if ( !hInet ) 
        {
            pszErrFunction = L"InternetOpen";
            break;
        }

        DWORD dwUrlLength = 1024;
        CAtlStringW strCanonicalUrlW;
        if ( !InternetCanonicalizeUrl( strUrlW, strCanonicalUrlW.GetBuffer(dwUrlLength), &dwUrlLength, 0) )
        {
            pszErrFunction = _T("InternetCanonicalizeUrl");
            break;
        }
        strCanonicalUrlW.ReleaseBuffer();

		alloc_url_components( &uri, strCanonicalUrlW );
        if( !InternetCrackUrl( strCanonicalUrlW, strCanonicalUrlW.GetLength(), 0, &uri ) ) 
        {
			pszErrFunction = L"InternetCrackUrl";
			break;
		}

        hConnection = InternetConnect( hInet, uri.lpszHostName, uri.nPort, _T("anonymous"), NULL, 
          INTERNET_SERVICE_HTTP, 0, 0 );
        if ( !hConnection ) 
        {
            pszErrFunction = L"InternetConnect";
            break;
        }

        strReqUrlW = uri.lpszUrlPath;
        strReqUrlW += uri.lpszExtraInfo;
        hRequest = HttpOpenRequest( hConnection, CAtlStringW(method), strReqUrlW, NULL, NULL, NULL, 
          INTERNET_FLAG_KEEP_CONNECTION|INTERNET_FLAG_NO_CACHE_WRITE, NULL );
        if ( !hRequest ) 
        {
            pszErrFunction = L"HttpOpenRequest";
            break;
        }

    }while(0);
}

INetData* CNetRequestImpl::sendString(const String& strBody)
{
    CNetData* pNetData = new CNetData;

    do
    {
        if ( isError() )
            break;

        CAtlStringW strHeaders = L"Content-Type: application/x-www-form-urlencoded\r\n";
        if ( !HttpAddRequestHeaders( hRequest, strHeaders, -1, HTTP_ADDREQ_FLAG_ADD|HTTP_ADDREQ_FLAG_REPLACE ) )
        {
            pszErrFunction = L"HttpAddRequestHeaders";
            break;
        }

        if ( !HttpSendRequest( hRequest, NULL, 0, const_cast<char*>(strBody.c_str()), strBody.length() ) )
        {
            pszErrFunction = L"HttpSendRequest";
            break;
        }
        //Sleep(5000);
        readResponse(pNetData);
    }while(0);

    return pNetData;
}

void CNetRequestImpl::readResponse(CNetData* pNetData)
{
    DWORD dwLen = 10;
    wchar_t szHttpRes[10];
    DWORD nIndex = 0;

    if( !HttpQueryInfo( hRequest, HTTP_QUERY_STATUS_CODE, szHttpRes, &dwLen, &nIndex) )
    {
        pszErrFunction = L"HttpSendRequest";
        return;
    }

    if ( wcscmp( szHttpRes, L"200") != 0 )
    {
        LOG(ERROR) + "An error occured connecting to the sync source: " + szHttpRes + " returned.";

        // If we're unauthorized, delete any cookies that might have been
        // stored so we don't reuse them later
        if ( wcscmp( szHttpRes, L"401") == 0 ) 
        {
            CAtlStringA strUrlA;
            int nQuest = strReqUrlW.Find('?'); 
            if ( nQuest > 0 )
                strUrlA = strReqUrlW.Mid(0,nQuest-1);
            else
                strUrlA = strReqUrlW;

            m_oNetReq.deleteCookie(strUrlA.GetString());
        }

        return;
	}

    readInetFile(hRequest,pNetData);
}

INetData* CNetRequestImpl::sendStream(common::InputStream* body)
{
    CNetData* pNetData = new CNetData;

    do
    {
        if ( isError() )
            break;

        //TODO: send multipart data

        readResponse(pNetData);
    }while(0);

    return pNetData;
}

void CNetRequestImpl::close()
{
	if (pszErrFunction)
		ErrorMessage(pszErrFunction);

    free_url_components(&uri);

	if ( hRequest ) 
        InternetCloseHandle(hRequest);
	if ( hConnection ) 
        InternetCloseHandle(hConnection);
	if ( hInet ) 
        InternetCloseHandle(hInet);

    memset(&uri, 0, sizeof(uri));

    hRequest = 0;
    hConnection = 0;
    hInet = 0;
}

CNetRequestImpl::~CNetRequestImpl()
{
    close();

    m_pInstance = 0;
}

void CNetRequestImpl::readInetFile( HINTERNET hRequest, CNetData* pNetData )
{
    DWORD dwBufSize = 4096;
    char* pBuf = (char*)malloc(dwBufSize);
    DWORD dwBytesRead = 0;
    BOOL bRead = FALSE;
    do
    {
        bRead = InternetReadFile(hRequest, pBuf, dwBufSize, &dwBytesRead);
        if ( bRead )
        {
            if (dwBytesRead > 0)
                pNetData->getRawData().append(pBuf,dwBytesRead);

            pNetData->setValid(true);
        }
    }while(bRead && dwBytesRead > 0);
}

void CNetRequestImpl::ErrorMessage(LPCTSTR pszFunction)
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
    {
        //wprintf(L"%s failed with error %d\n", pszFunction, dwLastError);
        //TODO: Log error
    }
    else
    {
        //wprintf(L"%s failed with error %d: %s\n", pszFunction, dwLastError, pszMessage);
        //TODO: Log error
        LocalFree(pszMessage);
    }
}

void CNetRequestImpl::alloc_url_components(URL_COMPONENTS *uri, const wchar_t *url) 
{
  int dwLength = wcslen(url)*sizeof(wchar_t);
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

void CNetRequestImpl::free_url_components(URL_COMPONENTS *uri) 
{
  free(uri->lpszScheme);
  free(uri->lpszHostName);
  free(uri->lpszUserName);
  free(uri->lpszPassword);
  free(uri->lpszUrlPath);
  free(uri->lpszExtraInfo);
}

bool CNetRequest::SetupInternetConnection(LPCTSTR url)
{
#if defined (_WIN32_WCE)
	int iNetwork;
	HRESULT hResult = E_FAIL;
	DWORD   dwStatus;
    static HANDLE hConnection = NULL;

	// cleanup the old connection
	if(NULL != hConnection)
	{
		hResult = ConnMgrConnectionStatus( hConnection, &dwStatus );
		if( SUCCEEDED(hResult) )
		{
			LOG(INFO) + "Internet connection exist, use it";
			if( dwStatus & CONNMGR_STATUS_CONNECTED )
				return true;
		}
		ConnMgrReleaseConnection(hConnection, FALSE);
		LOG(INFO) + "Internet connection droped, open new one";
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
		LOG(INFO) + "Try establish Internet connection";
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
			LOG(INFO) + "Wait for connect (" + count + ")";
			DWORD dwResult = WaitForSingleObject(hConnection, 1000); 
			if (dwResult == (WAIT_OBJECT_0))
			{ 
				hResult=ConnMgrConnectionStatus(hConnection,&dwStatus);
				if( SUCCEEDED(hResult) )
				{
					if( dwStatus & CONNMGR_STATUS_CONNECTED )
					{
						LOG(INFO) + "Connected";
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
	LOG(ERROR) + "Failed to connect";
	return false;
#else
	return true;
#endif //_WIN32_WCE
}

}
}

extern "C" int rho_net_has_network()
{
    //TODO: rho_net_has_network
    return 1;
}
