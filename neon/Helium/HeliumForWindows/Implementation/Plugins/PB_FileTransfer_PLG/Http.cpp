// Http.cpp: implementation of the CHttp class.

#include <wchar.h>
#include "Http.h"

/**
* \author	Paul Henderson (PH, PTJK76)
* \date		December 2009 - Ported from SPB2.22, PH
*/
CHttp::CHttp(const class CInet& rCInet)
{
	m_rCInet = &rCInet;
	m_hInet				= NULL;
	m_lpszHost			= NULL;
	m_lpszURLPath		= NULL;
	m_SzResponse		= NULL;
	strcpy				(m_SzElementID,DEFAULT_ELEMENT_NAME);
}

/**
* \author	Paul Henderson (PH, PTJK76)
* \date		December 2009 - Ported from SPB2.22, PH
*/
CHttp::~CHttp()
{
	delete [] m_lpszHost;
	delete [] m_lpszURLPath;
	if(NULL != m_hInet) 
	{
		if(InternetCloseHandle(m_hInet))
			m_hInet = NULL;
	}

}



/**
* \author	Paul Henderson (PH, PTJK76)
* \date		December 2009 - Ported from SPB2.22, PH
* \return	On successs the transferred file size otherwise 0
*/
DWORD CHttp::GetFile(int protocol)
{
	BYTE	byteBuffer	[CHUNKSIZE + 1];
	DWORD	dFileSize = 0;
	HINTERNET hHttpRequest;
	DWORD dwRead;
	BOOL bFileOpen = FALSE;
	
	DWORD dwRet = 0;
	
	int count = 0;

	//validate the length of the source
	int len = wcslen(m_rCInet->m_lpSzSource);
	if(len < INTERNET_MIN_URL_LENGTH  || len > INTERNET_MAX_URL_LENGTH){
		goto _Cleanup;
	}
	
	
	//validate the length of the destination
	len = wcslen(m_rCInet->m_lpSzDestination);
	if(len < MIN_FILE_LEN  || len > MAX_PATH){
		goto _Cleanup;
	}


	//check for and if necessary open the internet connection
	if(m_hInet == NULL)
    {
        LPCTSTR pProxySetting =NULL;
        pProxySetting = m_rCInet->m_pModule->GetAppSettingPtr(m_rCInet->m_iInstanceID, L"HTTP_Proxy", L"Value");  
     
        if((pProxySetting==NULL)||(pProxySetting==L"")||(*pProxySetting==_T('\0')))
        {
            m_hInet = InternetOpen(APP_NAME,INTERNET_OPEN_TYPE_DIRECT ,NULL,NULL,0); 
        }
        else
        {
            m_hInet=InternetOpen(APP_NAME,INTERNET_OPEN_TYPE_PROXY,pProxySetting,TEXT("local"), 0);
        }
     //m_hInet = InternetOpen(APP_NAME,INTERNET_OPEN_TYPE_PRECONFIG ,NULL,NULL,0);
   }

	if(m_hInet == NULL){
		m_rCInet->m_pModule->Log(PB_LOG_ERROR, L"Failed to open a connection", _T(__FUNCTION__), __LINE__);
		goto _Cleanup;
	}

	//allocate memory for the fully qualified source string
	LPTSTR szURL = new TCHAR[wcslen(m_rCInet->m_lpSzSource)+8];
	if(szURL == NULL){
		m_rCInet->m_pModule->Log(PB_LOG_ERROR, L"MEM_ALLOC_ERR", _T(__FUNCTION__), __LINE__);
		goto _Cleanup;
	}
	
	//check for a HTTP protocol included in the string, if not add it
	// GD - Check for HTTPS also, depending on protocol
	if (protocol == PROTOCOL_HTTPS)
	{
		if(!wcsstr(m_rCInet->m_lpSzSource,L"https://"))
			wsprintf(szURL,L"%s%s",L"https://",m_rCInet->m_lpSzSource);
		else
			wcscpy(szURL,m_rCInet->m_lpSzSource);
	}
	else
	{
		if(!wcsstr(m_rCInet->m_lpSzSource,L"http://"))
			wsprintf(szURL,L"%s%s",L"http://",m_rCInet->m_lpSzSource);
		else
			wcscpy(szURL,m_rCInet->m_lpSzSource);
	}
	
	//open the source URL
	/**
	* Need to add the port number if this is different than the default 80
	*/
	TCHAR* szUrlWithPort = NULL;
	if (m_rCInet->m_nPort != 80)
	{
		szUrlWithPort = new TCHAR[wcslen(szURL) + 6];
		wmemset(szUrlWithPort, '\0', wcslen(szURL) + 6);
		TCHAR* ptcUrlWithoutProtocol = wcsstr(szURL, L"//") + 2;
		TCHAR* ptcPrefix = new TCHAR[wcslen(ptcUrlWithoutProtocol)];
		wmemset(ptcPrefix, '\0', wcslen(ptcUrlWithoutProtocol));
		ptcPrefix = wmemcpy(ptcPrefix, ptcUrlWithoutProtocol, wcsstr(ptcUrlWithoutProtocol, L"/") - ptcUrlWithoutProtocol);

		// GD - Add correct prefix for protocol
		if (protocol == PROTOCOL_HTTPS)
			wsprintf(szUrlWithPort, L"https://%s:%d/%s", ptcPrefix, m_rCInet->m_nPort, wcsstr(ptcUrlWithoutProtocol, L"/") + 1);
		else
			wsprintf(szUrlWithPort, L"http://%s:%d/%s", ptcPrefix, m_rCInet->m_nPort, wcsstr(ptcUrlWithoutProtocol, L"/") + 1);

		delete [] szURL;
		szURL = szUrlWithPort;
	}

	// GD - Add the secure flag is we're using HTTPS
	DWORD flags = 0;
	if (protocol == PROTOCOL_HTTPS)
		flags |= INTERNET_FLAG_SECURE;
	hHttpRequest = InternetOpenUrl(m_hInet,szURL,NULL,0,flags,0);
	
	if(hHttpRequest == NULL){
		if(NULL != m_hInet){
			if(InternetCloseHandle(m_hInet)){
				m_hInet = NULL;				
				m_hInet = InternetOpen(	APP_NAME,INTERNET_OPEN_TYPE_DIRECT ,NULL,NULL,0);
				if(m_hInet == NULL){
					m_rCInet->m_pModule->Log(PB_LOG_ERROR, L"Failed to open a connection", _T(__FUNCTION__), __LINE__);
					goto _Cleanup;
				}	
			}
		}
		hHttpRequest = InternetOpenUrl(m_hInet,szURL,NULL,0,0,0);
		if(hHttpRequest == NULL){		  
		   m_rCInet->m_pModule->Log(PB_LOG_ERROR, L"Failed to open the URL", _T(__FUNCTION__), __LINE__);
		   goto _Cleanup;
		}
	}
	

	//  Delete the file if it exists already 
	if (m_rCInet->m_bOverWriteDest)
		DeleteFile(m_rCInet->m_lpSzDestination);

	//read the URL file and save locally
	while(1)	
	{	
		//read from the HTTP server
		if(!InternetReadFile(hHttpRequest,byteBuffer,CHUNKSIZE,&dwRead)){
			if(dFileSize == 0){
				//error as no data has been read
				DEBUGMSG(1,(L"Internet ReadFile Error!!!!\n"));
				goto _Cleanup;
			}
			
		}
		
		if(dwRead == 0){
			break;
		}

		//if the file is not open then open it
		if(!bFileOpen){
			bFileOpen = OpenFile(m_rCInet->m_lpSzDestination);
			if(bFileOpen == FALSE){
				m_rCInet->m_pModule->Log(PB_LOG_ERROR, L"Failed to save the file", _T(__FUNCTION__), __LINE__);
				goto _Cleanup;
			}
			bFileOpen = TRUE;
			
		}
		
		DEBUGMSG(1,(L"****%d   %d\n",dwRead, count++));
		if((dFileSize += AppendToFile(byteBuffer, dwRead))==0){
			m_rCInet->m_pModule->Log(PB_LOG_ERROR, L"Failed to append to the file", _T(__FUNCTION__), __LINE__);
			goto _Cleanup;	
		}
	};    		 
  
	//if we have made it here all should be good
	m_rCInet->m_pModule->Log(PB_LOG_INFO, L"File successfully received", _T(__FUNCTION__), __LINE__);
	dwRet = dFileSize;
	
	
_Cleanup:
	delete []  szURL;
	InternetCloseHandle(hHttpRequest);
	CloseHandle(m_hFile);
	m_hFile = NULL;
		
  
	return dwRet;
  
 }
	

	
/**
* \author	Paul Henderson (PH, PTJK76)
* \date		December 2009 - Ported from SPB2.22, PH
*/
LPCTSTR CHttp::CrackURL(const LPCTSTR lpzURL)
{
	
	LPTSTR lp,lpSzURL = (LPTSTR)lpzURL;
	int iLenURL = wcslen(lpzURL);
	
	delete [] m_lpszHost;
	delete [] m_lpszURLPath;
	
	m_lpszHost		= new TCHAR[iLenURL + 1];
	m_lpszURLPath	= new TCHAR[iLenURL + 1];

	if(!m_lpszHost || !m_lpszURLPath){
		//error
		return NULL;
	}
	
	
	for(lp = (LPTSTR)lpSzURL; (*lp != L'/') && (*lp != '\0'); lp++);
	if (*lp == L'/')
		wcscpy(m_lpszURLPath, lp);
	else
		wcscpy(m_lpszURLPath, L"");
	
		
	wcscpy(m_lpszHost,lpSzURL);
	*(m_lpszHost + (lp-lpSzURL)) = NULL;
	
	return m_lpszHost;
}


/**
* \author	Paul Henderson (PH, PTJK76)
* \author	Dave Sheppard (DS)
* \date		April 2005	  - Initial Creation, DS
* \date		December 2009 - Ported from SPB2.22, PH
*/
int CHttp::GenerateRequestBody(const char* boundary, char* requestBody, 
							   void* pFileBits,int sizeOfFile,char* pszFileName)
{
	int iLen;
	char filename[50];
	char* contentType = {"Content-Type: tiger/*"};
	char contentDisposition[100];
	
	sprintf(filename,pszFileName);
	sprintf(contentDisposition, "Content-Disposition: form-data; name=\"%s\"; filename=\"%s\"",m_SzElementID, filename);
	
	strcpy(requestBody, "--");
	strcat(requestBody, boundary);
	strcat(requestBody, "\r\n");
	strcat(requestBody, contentDisposition);
	strcat(requestBody, "\r\n");
	strcat(requestBody, contentType);
	strcat(requestBody, "\r\n\r\n");
	iLen = strlen(requestBody);
	memcpy(&requestBody[iLen], pFileBits, sizeOfFile);	
	iLen+= sizeOfFile;
	sprintf(&requestBody[iLen], "\r\n--%s--\r\n", boundary);
	iLen+= 8;
	iLen+= strlen(boundary);
	return iLen;
}

/**
* \author	Paul Henderson (PH, PTJK76)
* \author	Geoff Day (GD, XFH386)
* \date		December 2009 - Ported from SPB2.22, PH
* \date		February 2014 - Added HTTPS support
*/
BOOL CHttp::PutFile(int protocol)
{
	TCHAR szMessage[MAX_LOG_MESSAGE + 1];//100
	
	DWORD	dwNumberOfBytesWritten= 0;
	
	BOOL	bOk,bRet;
	DWORD	dwSize;
	char	*lpBufferA = NULL;	  //Ansi
	LPTSTR	lpHeadersW;	//Wide
	BYTE*	byteBuffer;
	HANDLE	hFile;

	HINTERNET		hConnection = NULL;
	HINTERNET		hHttpRequest = NULL;

	TCHAR szContentTypeHeader[100];
	TCHAR szBoundary[50];
	char pcBoundary[50];
	char *szRequestBody = NULL;
	int iRequestSize;
	char *szFilename = NULL;

	DWORD dwFlags = INTERNET_FLAG_IGNORE_REDIRECT_TO_HTTP |
								INTERNET_FLAG_IGNORE_REDIRECT_TO_HTTPS | 
								INTERNET_FLAG_KEEP_CONNECTION |
								//INTERNET_FLAG_NO_AUTH |
								//INTERNET_FLAG_NO_AUTO_REDIRECT |
								INTERNET_FLAG_NO_COOKIES |
								INTERNET_FLAG_NO_UI |
								INTERNET_FLAG_RELOAD;

	// GD - Add the secure flag if we're using HTTPS
	if (protocol == PROTOCOL_HTTPS)
			dwFlags |= INTERNET_FLAG_SECURE;

	//get the filename from the destination string
	int len = wcslen(m_rCInet->m_lpSzSource);
		LPTSTR pTemp;
		for(pTemp = (LPTSTR)&m_rCInet->m_lpSzSource[len]
			;*(pTemp-1) != L'\\';pTemp--){
			if(!len){
				break;	
			}
			len--;
		}
		len = wcslen(pTemp);
		szFilename = new char[len + 1];
		if(szFilename == NULL)
			return NULL;
		wcstombs( szFilename, pTemp,len);
		szFilename[len] = '\0';
	
	//open the file that we wish to send
	hFile = CreateFile(m_rCInet->m_lpSzSource, 
                   GENERIC_READ, 
                   FILE_SHARE_READ, 
                   NULL, 
                   OPEN_EXISTING, 
                   FILE_ATTRIBUTE_NORMAL, 
                   NULL);	
	if(hFile == INVALID_HANDLE_VALUE ){
		bRet = FALSE;
		goto _cleanup;	
	}
	
	DWORD dwNumBytes,dwFileSize;
	dwFileSize = GetFileSize (hFile, NULL) ;
	byteBuffer = new BYTE[dwFileSize];
	if(byteBuffer == NULL)
	{
		m_rCInet->m_pModule->Log(PB_LOG_WARNING, L"Insufficnet device memory to transfer file", _T(__FUNCTION__), __LINE__);
		bRet = FALSE;
		goto _cleanup;	
	}
	
	dwNumBytes = 0;
	bOk = ReadFile(hFile,byteBuffer, dwFileSize, &dwNumBytes, NULL);
	if(!bOk || dwFileSize!=dwNumBytes)
	{
		bRet = FALSE;
		goto _cleanup;	
	}
	
	CloseHandle(hFile);	
	hFile = NULL;
	
	//send an Info message
	wsprintf(szMessage,L"File size: %d",dwFileSize);
	m_rCInet->m_pModule->Log(PB_LOG_INFO, szMessage, _T(__FUNCTION__), __LINE__);
  	
	memset(szBoundary, 0, 50*sizeof(TCHAR));
	GenerateMultipartBoundary(szBoundary);	
	wcstombs(pcBoundary, szBoundary, wcslen(szBoundary)+1);
	wsprintf(szContentTypeHeader, L"Content-Type: multipart/form-data; boundary=%s", szBoundary);
	
	// Now call the other function
	szRequestBody = new char[dwFileSize + 500];
	if (!szRequestBody)
	{
		//  Insufficient memory to allocate Request body
		m_rCInet->m_pModule->Log(PB_LOG_WARNING, L"Insufficnet device memory to transfer file", _T(__FUNCTION__), __LINE__);
		bRet = FALSE;
		goto _cleanup;	
	}
	iRequestSize = GenerateRequestBody(pcBoundary, szRequestBody,byteBuffer,dwFileSize,szFilename);
	wsprintf(szContentTypeHeader, L"Content-Type: multipart/form-data; boundary=%s", szBoundary);
	  
	CrackURL(m_rCInet->m_lpSzDestination);
	
	if(m_hInet == NULL)
		m_hInet = InternetOpen(	APP_NAME,INTERNET_OPEN_TYPE_DIRECT ,NULL,NULL,0);
	
	// Open an internet connection handle 
	hConnection =  
		InternetConnect(
		m_hInet,        //class scope handle
		m_lpszHost,     //class scope host eg: www.symbol.com    or   157.235.111.12 
		m_rCInet->m_nPort,
		m_rCInet->m_SzUserName, 
		m_rCInet->m_SzPassWord, 
		INTERNET_SERVICE_HTTP, 
		0 , 
		0
		);
	
	if(hConnection== NULL){
		m_rCInet->m_pModule->Log(PB_LOG_ERROR, L"Could not connect", _T(__FUNCTION__), __LINE__);
		bRet = FALSE;
		goto _cleanup;	
	}
	
	m_rCInet->m_pModule->Log(PB_LOG_INFO,L"Connected", _T(__FUNCTION__), __LINE__);
  
  // Open an HTTP request handle. 
	hHttpRequest = HttpOpenRequest(
		hConnection, 
		L"POST", 
		m_lpszURLPath,//m_lpSzURL, //tcobject
		HTTP_VERSION, 
		NULL, 
		NULL, 
		dwFlags,
		0);
	
	
	if(hHttpRequest== NULL){
		bRet = FALSE;
		goto _cleanup;	
	}
  
  
	bOk = HttpAddRequestHeaders(hHttpRequest, szContentTypeHeader, -1, HTTP_ADDREQ_FLAG_ADD);
	if(!bOk){
		bRet = FALSE;
		goto _cleanup;	
	}
	
	m_rCInet->m_pModule->Log(PB_LOG_INFO,L"Added request Headers", _T(__FUNCTION__), __LINE__);

	// Send a request to the HTTP server. 
	bOk = HttpSendRequest (hHttpRequest, NULL, 0, szRequestBody, iRequestSize);
	if(!bOk){
		WCHAR *pmessage;
		DWORD error = GetLastError ();
		switch (error)
		{
		case 12055:
			pmessage = L"Certificate common name doesn't match request";
			break;

		case 12037:
			pmessage = L"Certifcate has invalid date";
			break;

		case 12045:
			pmessage = L"Certificate is not trusted";
			break;

		default:
			pmessage = L"Failed: sending the request";
			break;
		}
		m_rCInet->m_pModule->Log(PB_LOG_ERROR,pmessage, _T(__FUNCTION__), __LINE__);
		bRet = FALSE;
		goto _cleanup;
	}
	
	
	// Call HttpQueryInfo to find out the size of the headers.
	HttpQueryInfo (hHttpRequest, HTTP_QUERY_RAW_HEADERS_CRLF, NULL, &dwSize, NULL);
	m_rCInet->m_pModule->Log(PB_LOG_INFO,L"Got the size of the request headers", _T(__FUNCTION__), __LINE__);

	
	// Allocate a block of memory for lpHeadersA.
	dwSize = dwSize*4;
	lpHeadersW = new TCHAR [dwSize+1];
	
	// Call HttpQueryInfo again to get the headers.
	bOk = HttpQueryInfo (hHttpRequest,	HTTP_QUERY_RAW_HEADERS_CRLF, (LPVOID) lpHeadersW, &dwSize, NULL);
	if(!bOk){
		m_rCInet->m_pModule->Log(PB_LOG_ERROR,L"Failed: receiving the request", _T(__FUNCTION__), __LINE__);
		delete [] lpHeadersW;
		bRet = FALSE;
		goto _cleanup;
	}	
	// Check the case with invalid credentails
	if(wcsstr(lpHeadersW,L"401 Authorization Required"))
	{
		m_rCInet->m_pModule->Log(PB_LOG_ERROR,L"Failed: HTTP Error 401 Authorization Required", _T(__FUNCTION__), __LINE__);
		delete [] lpHeadersW;
		bRet = FALSE;
		goto _cleanup;
	}
		
	delete[] lpHeadersW;
	
	m_rCInet->m_pModule->Log(PB_LOG_INFO,L"Got the request headers", _T(__FUNCTION__), __LINE__);
	
	//determine how many bytes are to be read from the server
	InternetQueryDataAvailable(hHttpRequest, &dwSize,0,0);
	// Allocate a block of memory for lpBufferA.
	lpBufferA = new char [dwSize + 1];
	memset(lpBufferA, 0x00, dwSize + 1);
	
	bOk = InternetReadFile (hHttpRequest, (LPVOID)lpBufferA, dwSize, &dwSize);
	if(!bOk){
		bRet = FALSE;
		delete [] lpBufferA;
		goto _cleanup;
	}
	if (dwSize != 0)
    lpBufferA [dwSize] = '\0';
	m_rCInet->m_pModule->Log(PB_LOG_INFO, L"File Sent", _T(__FUNCTION__), __LINE__);
	bRet = SetResponseStr(lpBufferA); //copy the response into the Http variable
	delete [] lpBufferA;

    if(!bRet)
	{
      DWORD err = GetLastError();
      swprintf(szMessage,L"Error Code: %d",err);
      m_rCInet->m_pModule->Log(PB_LOG_ERROR, szMessage, _T(__FUNCTION__), __LINE__);
    }

 _cleanup:    
	if(hFile != INVALID_HANDLE_VALUE)
		  CloseHandle(hFile);
	delete [] szRequestBody;
	delete [] szFilename;
	delete [] byteBuffer;
    
	delete [] m_lpszHost;
	delete [] m_lpszURLPath;
	m_lpszHost		= NULL;
	m_lpszURLPath	= NULL;
	InternetCloseHandle(hConnection);
    InternetCloseHandle(hHttpRequest);
        
    return bRet;

}

/**
* \author	Paul Henderson (PH, PTJK76)
* \author	Dave Sheppard (DS)
* \date		April 2005 - Initial Creation
* \date		December 2009 - Ported from SPB2.22, PH
*/
void CHttp::GenerateMultipartBoundary(TCHAR* boundaryPrefix)
{
	for (int i= 0; i<27; i++) boundaryPrefix[i] = L'-';

	// We now need 12 hex digits.
	int r0 = rand() & 0xffff;
	int r1 = rand() & 0xffff;
	int r2 = rand() & 0xffff;

	wsprintf(&boundaryPrefix[27], L"%04X%04X%04X", r0, r1, r2);
}

/**
* \author	Paul Henderson (PH, PTJK76)
* \date		December 2009 - Ported from SPB2.22, PH
*/
BOOL CHttp::OpenFile(LPCTSTR lpSzFilePathName)
{
	m_hFile = CreateFile(lpSzFilePathName, 
                   GENERIC_WRITE, 
                   FILE_SHARE_READ, 
                   NULL, 
                   CREATE_ALWAYS, 
                   FILE_ATTRIBUTE_NORMAL, 
                   NULL); 
	if(m_hFile == INVALID_HANDLE_VALUE ){
		return FALSE;
		m_hFile = NULL;
	}
	return TRUE;
}

/**
* \author	Paul Henderson (PH, PTJK76)
* \date		December 2009 - Ported from SPB2.22, PH
*/
DWORD CHttp::AppendToFile(BYTE* szBuffer,int dwWrite)
{
	if (m_hFile != INVALID_HANDLE_VALUE) 
    { 
		DWORD dwNumBytes; 
        WriteFile(m_hFile, szBuffer, dwWrite, &dwNumBytes, NULL); // Write the file header 
        DEBUGMSG(1,(L"%d %d\n",dwWrite,dwNumBytes));
		return dwNumBytes;
    } 
	return 0;  
}

/**
* \author	Paul Henderson (PH, PTJK76)
* \date		December 2009 - Ported from SPB2.22, PH
*/
BOOL CHttp::SetResponseStr(char *szResponse)
{
	StripCRLF(szResponse);
	
	delete [] m_SzResponse;
	m_SzResponse = new TCHAR[strlen(szResponse) + 1];
	memset(m_SzResponse, 0, sizeof(TCHAR) * (strlen(szResponse) + 1));
	if(m_SzResponse == NULL)
	{
		m_rCInet->m_pModule->Log(PB_LOG_ERROR, L"MEMORY ERROR", _T(__FUNCTION__), __LINE__);
		return FALSE;
	}
	mbstowcs( m_SzResponse, szResponse,strlen(szResponse));
	DEBUGMSG(1,(L"%s",m_SzResponse));
	return TRUE;
}

/**
* \author	Paul Henderson (PH, PTJK76)
* \date		December 2009 - Ported from SPB2.22, PH
*/
BOOL CHttp::StripCRLF(char* pStr)
{
	int index,len = strlen(pStr);
	
	for(index = 0;index < len;index++)
	{
		if(*(pStr + index) == 0x0D)
			if(*(pStr + index + 1) == 0x0A)
			{
				*(pStr + index) = NULL;
				return TRUE;
			}
	}
	return FALSE;
}

/**
* \author	Paul Henderson (PH, PTJK76)
* \date		December 2009 - Ported from SPB2.22, PH
*/
LPTSTR CHttp::GetResponseStr()
{
	return m_SzResponse;
}
