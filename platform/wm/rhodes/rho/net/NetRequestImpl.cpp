#include "stdafx.h"

#include "NetRequestImpl.h"
#include "common/RhoFile.h"
#include "common/RhoFilePath.h"
#include "NetRequest.h"
#include "common/StringConverter.h"
#include "net/URI.h"
#include "common/RhoConf.h"

#if defined(_WIN32_WCE)
#include <connmgr.h>
#endif

#ifdef OS_WINCE
extern "C" int strnicmp( const char *s1, const char *s2, size_t count );
#endif

namespace rho {
namespace net {
IMPLEMENT_LOGCLASS(CNetRequestImpl,"Net");
common::CMutex CNetRequestImpl::m_mxInternet;
HINTERNET CNetRequestImpl::m_hInternet;
HANDLE    CNetRequestImpl::m_hWceConnMgrConnection;

CNetRequestImpl::CNetRequestImpl(CNetRequest* pParent, const char* method, const String& strUrl, 
    IRhoSession* oSession, Hashtable<String,String>* pHeaders, boolean sslVerifyPeer)
{
    m_pParent = pParent;
    m_pParent->m_pCurNetRequestImpl = this;
    m_pHeaders = pHeaders;
    m_bCancel = false;
    m_pSession = oSession;
    m_sslVerifyPeer = sslVerifyPeer;

    m_pszErrFunction = NULL;
    m_hConnection = NULL;
    m_hRequest = NULL;
    memset(&m_uri, 0, sizeof(m_uri) );

    m_strUrl = strUrl;
    CAtlStringW strUrlW(strUrl.c_str());

    LOG(INFO) + "Method: " + method + ";Url: " + strUrl;
    do 
    {
        if ( !initConnection(URI::isLocalHost(strUrl.c_str()), strUrlW) )
            break;

        DWORD dwUrlLength = 1024;
        CAtlStringW strCanonicalUrlW;
        if ( !InternetCanonicalizeUrl( strUrlW, strCanonicalUrlW.GetBuffer(dwUrlLength), &dwUrlLength, 0) )
        {
            m_pszErrFunction = _T("InternetCanonicalizeUrl");
            break;
        }
        strCanonicalUrlW.ReleaseBuffer();

		alloc_url_components( &m_uri, strCanonicalUrlW );
        if( !InternetCrackUrl( strCanonicalUrlW, strCanonicalUrlW.GetLength(), 0, &m_uri ) ) 
        {
			m_pszErrFunction = L"InternetCrackUrl";
			break;
		}

#ifdef OS_WINDOWS
		if (RHOCONF().isExist("http_proxy_host")) {
			rho::String login, password;
			
			if (RHOCONF().isExist("http_proxy_login"))
				login = RHOCONF().getString ("http_proxy_login");
			else
				login = "anonymous";

			if (RHOCONF().isExist("http_proxy_password"))
				password = RHOCONF().getString("http_proxy_password");

			m_hConnection = InternetConnect(m_hInternet, m_uri.lpszHostName, m_uri.nPort, 
											//rho::common::convertToStringW(login).c_str(),
											//password.length() ? rho::common::convertToStringW(password).c_str() : NULL,
											_T("baran"), _T("baran"),
											INTERNET_SERVICE_HTTP, 0, 0);
		} else {
			m_hConnection = InternetConnect(m_hInternet, m_uri.lpszHostName, m_uri.nPort, _T("anonymous"), NULL, 
											INTERNET_SERVICE_HTTP, 0, 0);
		}
#else
        m_hConnection = InternetConnect( m_hInternet, m_uri.lpszHostName, m_uri.nPort, _T("anonymous"), 
										 NULL, INTERNET_SERVICE_HTTP, 0, 0 );
#endif
        if ( !m_hConnection ) 
        {
            m_pszErrFunction = L"InternetConnect";
            break;
        }

        m_strReqUrlW = m_uri.lpszUrlPath;
        m_strReqUrlW += m_uri.lpszExtraInfo;
        DWORD dwFlags = INTERNET_FLAG_KEEP_CONNECTION|INTERNET_FLAG_NO_CACHE_WRITE|INTERNET_FLAG_NO_COOKIES;
        if ( m_uri.lpszScheme && wcsicmp(m_uri.lpszScheme,L"https")==0)
            dwFlags |= INTERNET_FLAG_SECURE;

        if ( !m_sslVerifyPeer )
            dwFlags |= INTERNET_FLAG_IGNORE_CERT_CN_INVALID|INTERNET_FLAG_IGNORE_CERT_DATE_INVALID;

        m_hRequest = HttpOpenRequest( m_hConnection, CAtlStringW(method), m_strReqUrlW, NULL, NULL, NULL, dwFlags, NULL );
        if ( !m_hRequest ) 
        {
            m_pszErrFunction = L"HttpOpenRequest";
            break;
        }

        if (oSession!=null)
        {
			String strSession = oSession->getSession();
			LOG(INFO) + "Cookie : " + strSession;
			if ( strSession.length() > 0 )
            {
                String strHeader = "Cookie: " + strSession + "\r\n";

                if ( !HttpAddRequestHeaders( m_hRequest, common::convertToStringW(strHeader).c_str(), -1, HTTP_ADDREQ_FLAG_ADD|HTTP_ADDREQ_FLAG_REPLACE ) )
                    m_pszErrFunction = L"HttpAddRequestHeaders";
            }
        }

    }while(0);
}

boolean CNetRequestImpl::checkSslCertError()
{
    DWORD dwError = GetLastError ();
    if (!m_sslVerifyPeer &&(
        (dwError == ERROR_INTERNET_INVALID_CA) ||
        (dwError == ERROR_INTERNET_CLIENT_AUTH_CERT_NEEDED) ||
        (dwError == ERROR_INTERNET_SEC_CERT_DATE_INVALID) ||
        (dwError == ERROR_INTERNET_SEC_CERT_CN_INVALID)))
    {
        DWORD dwFlag;
        DWORD dwBuffLen = sizeof(dwFlag);
        InternetQueryOption (m_hRequest, INTERNET_OPTION_SECURITY_FLAGS,(LPVOID)&dwFlag, &dwBuffLen);
        dwFlag |= (SECURITY_FLAG_IGNORE_UNKNOWN_CA
            | SECURITY_FLAG_IGNORE_CERT_CN_INVALID
            | SECURITY_FLAG_IGNORE_CERT_DATE_INVALID );
        InternetSetOption (m_hRequest, INTERNET_OPTION_SECURITY_FLAGS, &dwFlag, sizeof (dwFlag) );
        /*
        INTERNET_CERTIFICATE_INFO sInfo;
        DWORD dwSize = sizeof(sInfo);
        if(!InternetQueryOption(m_hSess,INTERNET_OPTION_SECURITY_CERTIFICATE_STRUCT,
            &sInfo, &dwSize))
        {
            dwError = GetLastError();
        }
        */

        return true;
    }

    return false;
}

String CNetRequestImpl::getBodyContentType()
{
    if ( m_pSession )
        return m_pSession->getContentType();
    else
        return "application/x-www-form-urlencoded";
}

CNetResponseImpl* CNetRequestImpl::sendString(const String& strBody)
{
    CNetResponseImpl* pNetResp = new CNetResponseImpl;

    do
    {
        if ( isError() )
            break;

        if ( strBody.length() > 0  )
        {
            CAtlStringW strHeaders = L"Content-Type: ";
            strHeaders += getBodyContentType().c_str();
            strHeaders += L"\r\n";

            if ( !HttpAddRequestHeaders( m_hRequest, strHeaders, -1, HTTP_ADDREQ_FLAG_ADD|HTTP_ADDREQ_FLAG_REPLACE ) )
            {
                m_pszErrFunction = L"HttpAddRequestHeaders";
                break;
            }
        }
        writeHeaders(m_pHeaders);

        if ( !HttpSendRequest( m_hRequest, NULL, 0, const_cast<char*>(strBody.c_str()), strBody.length() ) )
        {
            if (checkSslCertError())
            {
                if ( !HttpSendRequest( m_hRequest, NULL, 0, const_cast<char*>(strBody.c_str()), strBody.length() ) )
                {
                    m_pszErrFunction = L"HttpSendRequest";
                    break;
                }
            }else
            {
                m_pszErrFunction = L"HttpSendRequest";
                break;
            }
        }

        readResponse(pNetResp);
        if ( isError() )
            break;

        readInetFile(m_hRequest,pNetResp);
    }while(0);

    return pNetResp;
}

void CNetRequestImpl::writeHeaders(Hashtable<String,String>* pHeaders)
{
    if ( pHeaders && pHeaders->size() > 0 )
    {
        String strHeaders;

        for ( Hashtable<String,String>::iterator it = pHeaders->begin();  it != pHeaders->end(); ++it )
            strHeaders += it->first + ":" + it->second + "\r\n";

        if ( !HttpAddRequestHeaders( m_hRequest, common::convertToStringW(strHeaders).c_str(), -1, HTTP_ADDREQ_FLAG_ADD|HTTP_ADDREQ_FLAG_REPLACE ) )
        {
            m_pszErrFunction = L"HttpAddRequestHeaders";
            return;
        }
    }
}

boolean CNetRequestImpl::readHeaders(Hashtable<String,String>& oHeaders)
{
    oHeaders.clear();

    CAtlStringW strHeaders;
    DWORD dwLen = 0;
    DWORD nIndex = 0;
    if( !HttpQueryInfo( m_hRequest, HTTP_QUERY_RAW_HEADERS_CRLF, null, &dwLen, &nIndex) )
    {   
        DWORD dwErr = ::GetLastError();
        if ( dwErr != ERROR_INSUFFICIENT_BUFFER )
        {
            m_pszErrFunction = L"HttpQueryInfo";
            return false;
        }
    }
    if( !HttpQueryInfo( m_hRequest, HTTP_QUERY_RAW_HEADERS_CRLF, strHeaders.GetBuffer(dwLen), &dwLen, &nIndex) )
    {
        m_pszErrFunction = L"HttpQueryInfo";
        return false;
    }
    strHeaders.ReleaseBuffer();

    int nStart = 0;
    for(int nEnd = strHeaders.Find(L"\r\n", nStart); nEnd > 0; nStart = nEnd+2, nEnd = strHeaders.Find(L"\r\n", nStart) )
    {
        CAtlStringW strHeader = strHeaders.Mid(nStart, nEnd-nStart);
        int nSep = strHeader.Find(':');
        if (nSep < 0 )
            continue;

        CAtlStringW strName = strHeader.Mid(0, nSep);
        strName.Trim();
        strName.MakeLower();
        CAtlStringW strValue = strHeader.Mid(nSep+1);
        strValue.Trim();

        oHeaders.put(common::convertToStringA(strName.GetString()),common::convertToStringA(strValue.GetString()));
    }

    return true;
}

String CNetRequestImpl::makeClientCookie()
{
    DWORD nIndex = 0;
    String cookie;
    while(true)
    {
        CAtlStringW strCookie;
        DWORD dwLen = 0;
        if( !HttpQueryInfo( m_hRequest, HTTP_QUERY_SET_COOKIE, null, &dwLen, &nIndex) )
        {   
            DWORD dwErr = ::GetLastError();
            if ( dwErr == ERROR_HTTP_HEADER_NOT_FOUND  )
                break;

            if ( dwErr != ERROR_INSUFFICIENT_BUFFER )
            {
                m_pszErrFunction = L"HttpQueryInfo";
                break;
            }
        }
        if( !HttpQueryInfo( m_hRequest, HTTP_QUERY_SET_COOKIE, strCookie.GetBuffer(dwLen), &dwLen, &nIndex) )
        {
            m_pszErrFunction = L"HttpQueryInfo";
            break;
        }
        strCookie.ReleaseBuffer();

        URI::parseCookie(common::convertToStringA(strCookie.GetString()).c_str(), cookie);
    }
    if (m_pszErrFunction)
        return "";

//    if ( cookie.strAuth.length() > 0 || cookie.strSession.length() >0 )
//        return cookie.strAuth + ";" + cookie.strSession + ";";

    return cookie;
}

void CNetRequestImpl::readResponse(CNetResponseImpl* pNetResp)
{
    DWORD dwLen = 10;
    wchar_t szHttpRes[10];
    DWORD nIndex = 0;

    if( !HttpQueryInfo( m_hRequest, HTTP_QUERY_STATUS_CODE, szHttpRes, &dwLen, &nIndex) )
    {
        m_pszErrFunction = L"HttpQueryInfo";
        return;
    }
    int nCode = _wtoi(szHttpRes);
    pNetResp->setResponseCode(nCode);

    if ( m_pHeaders )
    {
        if ( !readHeaders(*m_pHeaders) )
            return;
    }

    if ( nCode != 200 && nCode != 206 && nCode != 416 )
    {
        LOG(ERROR) + "An error occured connecting to the sync source: " + szHttpRes + " returned.";

        // If we're unauthorized, delete any cookies that might have been
        // stored so we don't reuse them later
        if ( nCode == 401 && m_pSession ) 
        {
            m_pSession->logout();
        }
	}

    if (pNetResp->isOK())
        pNetResp->setCookies(makeClientCookie());
}

CNetResponseImpl* CNetRequestImpl::downloadFile(common::CRhoFile& oFile)
{
    CNetResponseImpl* pNetResp = new CNetResponseImpl;
    const int nDownloadBufferSize = 1024*100;
    char* pDownloadBuffer = 0;

    do
    {
        writeHeaders(m_pHeaders);

        if ( isError() )
            break;

        //if ( oFile.size() > 0 )
        {
            CAtlStringW strHeaders = L"Range: bytes=";
            strHeaders += common::convertToStringW(oFile.size()).c_str();
            strHeaders += L"-";
            //strHeaders += common::convertToStringW(oFile.size()+30068032).c_str();
            strHeaders += "\r\n";

            if ( !HttpAddRequestHeaders( m_hRequest, strHeaders, -1, HTTP_ADDREQ_FLAG_ADD|HTTP_ADDREQ_FLAG_REPLACE ) )
            {
                m_pszErrFunction = L"HttpAddRequestHeaders";
                break;
            }
        }

        if ( !HttpSendRequest( m_hRequest, NULL, 0, NULL, 0 ) )
        {
            if (checkSslCertError())
            {
                if ( !HttpSendRequest( m_hRequest, NULL, 0, NULL, 0 ) )
                {
                    m_pszErrFunction = L"HttpSendRequest";
                    break;
                }
            }else
            {
                m_pszErrFunction = L"HttpSendRequest";
                break;
            }
        }

        readResponse(pNetResp);
        if ( isError() )
            break;

        if ( pNetResp->getRespCode() == 416 )
        {
            pNetResp->setResponseCode(206);
            break;
        }

        if (!pDownloadBuffer)
            pDownloadBuffer = new char[nDownloadBufferSize];

        readInetFile(m_hRequest,pNetResp, &oFile, pDownloadBuffer, nDownloadBufferSize);

    }while(0);

    if (pDownloadBuffer)
        delete pDownloadBuffer;

    return pNetResp;
}

static const wchar_t* szMultipartContType = 
    L"Content-Type: multipart/form-data; boundary=----------A6174410D6AD474183FDE48F5662FCC5\r\n";
static const char* szMultipartPostfix = 
    "\r\n------------A6174410D6AD474183FDE48F5662FCC5--";

int CNetRequestImpl::processMultipartItems( VectorPtr<CMultipartItem*>& arItems )
{
    int nSize = 0;
    for( int i = 0; i < (int)arItems.size(); i++ )
    {
        CMultipartItem& oItem = *arItems.elementAt(i); 

        if ( oItem.m_strName.length() == 0 )
            oItem.m_strName = "blob";

        if ( oItem.m_strFileName.length() == 0 )
        {
            if ( oItem.m_strFilePath.length() > 0 )
            {
                common::CFilePath oPath(oItem.m_strFilePath);
                oItem.m_strFileName = oPath.getBaseName();
            }
            //else
            //    oItem.m_strFileName = "doesnotmatter.txt";
        }

        oItem.m_strDataPrefix = i > 0 ? "\r\n" : "";
        oItem.m_strDataPrefix += 
            "------------A6174410D6AD474183FDE48F5662FCC5\r\n"
            "Content-Disposition: form-data; name=\"";
        oItem.m_strDataPrefix += oItem.m_strName + "\"";
        if (oItem.m_strFileName.length()>0)
            oItem.m_strDataPrefix += "; filename=\"" + oItem.m_strFileName + "\"";
        oItem.m_strDataPrefix += "\r\n";
        if ( oItem.m_strContentType.length() > 0 )
            oItem.m_strDataPrefix += "Content-Type: " + oItem.m_strContentType + "\r\n";

        int nContentSize = 0;
        if ( oItem.m_strFilePath.length() > 0 )
        {
            common::CRhoFile oFile;
            if ( oFile.open(oItem.m_strFilePath.c_str(),common::CRhoFile::OpenReadOnly) ) 
                nContentSize = oFile.size();
        }
        else
            nContentSize = oItem.m_strBody.length();

        if ( oItem.m_strContentType.length() > 0 )
            oItem.m_strDataPrefix += "Content-Length: " + common::convertToStringA(nContentSize) + "\r\n";

        oItem.m_strDataPrefix += "\r\n";

        nSize += oItem.m_strDataPrefix.length() + nContentSize;

    }

    nSize += strlen(szMultipartPostfix);

    return nSize;
}

CNetResponseImpl* CNetRequestImpl::sendMultipartData(VectorPtr<CMultipartItem*>& arItems)
{
    CNetResponseImpl* pNetResp = new CNetResponseImpl;

    do
    {
        writeHeaders(m_pHeaders);

        if ( isError() )
            break;

        if ( !HttpAddRequestHeaders( m_hRequest, szMultipartContType, -1, HTTP_ADDREQ_FLAG_ADD|HTTP_ADDREQ_FLAG_REPLACE ) )
        {
            m_pszErrFunction = L"HttpAddRequestHeaders";
            break;
        }

	    INTERNET_BUFFERS BufferIn;
        memset(&BufferIn, 0, sizeof(INTERNET_BUFFERS));
	    BufferIn.dwStructSize = sizeof( INTERNET_BUFFERS ); // Must be set or error will occur
        BufferIn.dwBufferTotal = processMultipartItems( arItems );

        if(!HttpSendRequestEx( m_hRequest, &BufferIn, NULL, 0, 0))
        {
            if (checkSslCertError())
            {
                if(!HttpSendRequestEx( m_hRequest, &BufferIn, NULL, 0, 0))
                {
                    m_pszErrFunction = L"HttpSendRequestEx";
                    break;
                }
            }else
            {
                m_pszErrFunction = L"HttpSendRequestEx";
                break;
            }
        }

        //write all items
        for( int i = 0; i < (int)arItems.size(); i++ )
        {
            CMultipartItem& oItem = *arItems.elementAt(i); 

            if ( oItem.m_strFilePath.length() > 0 )
            {
                common::CRhoFile oFile;
                if ( !oFile.open(oItem.m_strFilePath.c_str(),common::CRhoFile::OpenReadOnly) ) 
                {
                    m_pszErrFunction = L"InternetWriteFile";
                    return pNetResp;
                }
                common::InputStream* bodyStream = oFile.getInputStream();

                if ( !internetWriteHeader( oItem.m_strDataPrefix.c_str(), "", "") )
                {
                    m_pszErrFunction = L"InternetWriteFile";
                    return pNetResp;
                }

                DWORD dwBytesWritten = 0;
                if ( bodyStream->available() > 0 )
                {
                    DWORD dwBufSize = 4096;
                    char* pBuf = (char*)malloc(dwBufSize);
                    int nReaded = 0;

	                do
	                {
                        nReaded = bodyStream->read(pBuf,0,dwBufSize);
                        if ( nReaded > 0 )
                        {
		                    if ( !InternetWriteFile( m_hRequest, pBuf, nReaded, &dwBytesWritten) )
                            {
                                m_pszErrFunction = L"InternetWriteFile";
                                return pNetResp;
                            }
                        }
	                }while(nReaded > 0);

                    free(pBuf);
                }

            }else
            {
                if ( !internetWriteHeader( oItem.m_strDataPrefix.c_str(), oItem.m_strBody.c_str(), "") )
                {
                    m_pszErrFunction = L"InternetWriteFile";
                    return pNetResp;
                }
            }

        }

        if ( !internetWriteHeader( "", "", szMultipartPostfix) )
        {
            m_pszErrFunction = L"InternetWriteFile";
            return pNetResp;
        }

        if ( !HttpEndRequest(m_hRequest, NULL, 0, 0) )
        {
            m_pszErrFunction = L"HttpEndRequest";
            break;
        }

        if ( isError() )
            break;

        readResponse(pNetResp);
        if ( isError() )
            break;

        readInetFile(m_hRequest,pNetResp);

    }while(0);

    return pNetResp;
}

bool CNetRequestImpl::internetWriteHeader( const char* szPrefix, const char* szBody, const char* szPrefixEnd)
{
    DWORD dwBytesWritten = 0;
    if ( szPrefix && *szPrefix && !InternetWriteFile( m_hRequest, szPrefix, strlen(szPrefix), &dwBytesWritten) )
        return false;

    if ( szBody && *szBody && !InternetWriteFile( m_hRequest, szBody, strlen(szBody), &dwBytesWritten) )
        return false;

    if ( szPrefixEnd && *szPrefixEnd && !InternetWriteFile( m_hRequest, szPrefixEnd, strlen(szPrefixEnd), &dwBytesWritten) )
        return false;

    return true;
}

void CNetRequestImpl::cancel()
{
    m_bCancel = true;

	if ( m_hRequest ) 
        InternetCloseHandle(m_hRequest);
/*	if ( hConnection ) 
        InternetCloseHandle(hConnection);
	if ( hInet ) 
        InternetCloseHandle(hInet); */
/*
    hRequest = 0;
    hConnection = 0;
    hInet = 0;*/
}

void CNetRequestImpl::close()
{
	if (!m_bCancel && m_pszErrFunction)
		ErrorMessage(m_pszErrFunction);

    free_url_components(&m_uri);

	if ( m_hRequest ) 
        InternetCloseHandle(m_hRequest);
	if ( m_hConnection ) 
        InternetCloseHandle(m_hConnection);
//	if ( hInet ) 
//        InternetCloseHandle(hInet);

    memset(&m_uri, 0, sizeof(m_uri));

    m_hRequest = 0;
    m_hConnection = 0;
//    hInet = 0;
}

CNetRequestImpl::~CNetRequestImpl()
{
    close();
    m_pParent->m_pCurNetRequestImpl = null;
}

void CNetRequestImpl::readInetFile( HINTERNET hRequest, CNetResponseImpl* pNetResp, common::CRhoFile* pFile /*=NULL*/,
    char* pBuf, DWORD dwBufSize )
{
    //if ( pNetResp->getRespCode() == 500 || pNetResp->getRespCode() == 422 )
    //    return;
    char* pBufToFree = 0;
    if (!pBuf)
    {
        if ( dwBufSize==0)
            dwBufSize=1024*50;

        pBuf = (char*)malloc(dwBufSize);
        pBufToFree = pBuf;
    }

    //DWORD dwBufSize = 1024*100;
    //char* pBuf = (char*)malloc(dwBufSize);
    //char* pBufToFree = pBuf;
    DWORD dwBytesRead = 0;
    BOOL bRead = FALSE;
    do
    {
        bRead = InternetReadFile(hRequest, pBuf, dwBufSize, &dwBytesRead);
        if ( !bRead )
        {
            m_pszErrFunction = L"InternetReadFile";
            pNetResp->setResponseCode(408);
            break;
        }

        if (dwBytesRead > 0)
        {
            if ( pFile )
                pFile->write(pBuf,dwBytesRead);
            else
                pNetResp->getRawData().append(pBuf,dwBytesRead);
        }

        pNetResp->setValid(true);

    }while(bRead && dwBytesRead > 0);

    if ( !pNetResp->isOK() )
        LOG(TRACE) + "Server response: " + pNetResp->getCharData();

    if ( pBufToFree )
        free(pBufToFree);
}

void CNetRequestImpl::ErrorMessage(LPCTSTR pszFunction)
{ 
    // Retrieve the system error message for the last-error code
    LPTSTR pszMessage = NULL;
    DWORD dwLastError = GetLastError(); 

    DWORD dwLen = FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | 
        //FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_FROM_HMODULE|
        FORMAT_MESSAGE_IGNORE_INSERTS,
        GetModuleHandle( _T("wininet.dll") ),
        dwLastError,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&pszMessage,
        0, NULL );

    wchar_t* szExtError = 0;
    if ( dwLastError == ERROR_INTERNET_EXTENDED_ERROR )
    {
        DWORD  dwInetError =0, dwExtLength = 0;
        InternetGetLastResponseInfo( &dwInetError, NULL, &dwExtLength );

        if ( dwExtLength > 0 )
        {
            szExtError = (wchar_t*)malloc(sizeof(wchar_t)*(dwExtLength+1));
            InternetGetLastResponseInfo( &dwInetError, szExtError, &dwExtLength );
        }
    }

    rho::LogMessage oLogMsg(__FILE__, __LINE__, L_ERROR, LOGCONF(), getLogCategory() );
    oLogMsg + "Call " + pszFunction + " failed. Url:" + m_strUrl.c_str() + ". With code : " + dwLastError;

    if ( pszMessage ) 
        oLogMsg + ".Message: " + pszMessage;
    if ( szExtError && *szExtError )
        oLogMsg + ".Extended info: " + szExtError;

    if ( szExtError )
        free(szExtError);
    if ( pszMessage )
        LocalFree(pszMessage);
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
  if ( uri->lpszScheme )
    free(uri->lpszScheme);
  if ( uri->lpszHostName )
    free(uri->lpszHostName);
  if (uri->lpszUserName)
    free(uri->lpszUserName);
  if (uri->lpszPassword)
    free(uri->lpszPassword);
  if (uri->lpszUrlPath)
    free(uri->lpszUrlPath);
  if (uri->lpszExtraInfo)
    free(uri->lpszExtraInfo);
}

bool CNetRequestImpl::initConnection(boolean bLocalHost, LPCTSTR url)
{
    common::CMutexLock lock(m_mxInternet);

    if (!bLocalHost)
    {
        if ( !SetupInternetConnection(url) )
            return false;
    }

    if (m_hInternet)
        return true;

#ifdef OS_WINDOWS
	if (RHOCONF().isExist("http_proxy_host")) {
		rho::String proxyName = RHOCONF().getString("http_proxy_host");

		if (RHOCONF().isExist("http_proxy_port")) {
			proxyName += ":" +  RHOCONF().getString("http_proxy_port");
		}

		LOG(ERROR) + "PROXY: " + proxyName;

		m_hInternet = InternetOpen(_T("rhodes-wm"), INTERNET_OPEN_TYPE_PROXY, 
									rho::common::convertToStringW(proxyName).c_str(), 
									NULL, NULL);
	} else {
		m_hInternet = InternetOpen(_T("rhodes-wm"), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, NULL );
	}
#else
    m_hInternet = InternetOpen(_T("rhodes-wm"), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, NULL );
#endif

	if ( !m_hInternet ) 
    {
        m_pszErrFunction = L"InternetOpen";
        return false;
    }

    return true;
}

/*static*/void CNetRequestImpl::deinitConnection()
{
    common::CMutexLock lock(m_mxInternet);

    if (m_hInternet)
        InternetCloseHandle(m_hInternet);
    m_hInternet = NULL;

#if defined (_WIN32_WCE)
    if ( m_hWceConnMgrConnection )
        ConnMgrReleaseConnection(m_hWceConnMgrConnection, FALSE);

    m_hWceConnMgrConnection = NULL;
#endif //_WIN32_WCE

}

bool CNetRequestImpl::SetupInternetConnection(LPCTSTR url)
{
#if defined (_WIN32_WCE)
	int iNetwork;
	HRESULT hResult = E_FAIL;
	DWORD   dwStatus;

	// cleanup the old connection
	if(NULL != m_hWceConnMgrConnection)
	{
		hResult = ConnMgrConnectionStatus( m_hWceConnMgrConnection, &dwStatus );
		if( SUCCEEDED(hResult) )
		{
			LOG(INFO) + "Internet connection exist, use it";
			if( dwStatus & CONNMGR_STATUS_CONNECTED )
				return true;
		}
		ConnMgrReleaseConnection(m_hWceConnMgrConnection, FALSE);
		LOG(INFO) + "Internet connection droped, open new one";
		m_hWceConnMgrConnection = NULL;
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

		hResult = ConnMgrEstablishConnection(&ConnInfo, &m_hWceConnMgrConnection);

		// check to see if the attempt failed
		int count = 0;
		while(SUCCEEDED(hResult) && count++ < 60 )
		{
			LOG(INFO) + "Wait for connect (" + count + ")";
			DWORD dwResult = WaitForSingleObject(m_hWceConnMgrConnection, 1000); 
			if (dwResult == (WAIT_OBJECT_0))
			{ 
				hResult=ConnMgrConnectionStatus(m_hWceConnMgrConnection,&dwStatus);
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
