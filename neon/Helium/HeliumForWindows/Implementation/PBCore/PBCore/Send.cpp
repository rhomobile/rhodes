#include "StdAfx.h"
#include "Send.h"

CSend::CSend(void)
{
	
	m_hInet			= NULL;
	m_SzResponse	= NULL;
	m_szHost		= NULL;
	m_SzUser		= NULL;
	m_SzPass		= NULL;
	m_szPath		= NULL;
	m_nPort			= INTERNET_DEFAULT_HTTP_PORT;
	m_hConnection	= NULL;
	memset(m_szPostData,0,MAX_URL*2);
}

CSend::CSend(HINTERNET	hInet)
{
	CSend();
	m_hInet = hInet;
}

CSend::~CSend(void)
{
	delete [] m_szHost;
	delete [] m_SzResponse;
	delete [] m_szPath;
	if(m_hInet){	
		CloseHandle(m_hInet);
	}
}


// GD - rewrote to handle path in URL, August 2010
bool CSend::CrackURL(LPCWSTR lpzURL)
{
	int state, path;
	uint source;
	WCHAR c;
	bool done;

	// Skip protocol if present
	if (!wcsnicmp (lpzURL, L"http://", 7))
		source = 7;
	else
		source = 0;

	// Free any existing values
	delete [] m_szHost;
	m_szHost = 0;

	delete [] m_szPath;
	m_szPath = 0;

	m_nPort = INTERNET_DEFAULT_HTTP_PORT;

	state = 0;
	done = false;

	while (source < wcslen (lpzURL))
	{
		c = lpzURL [source];

		switch (state)
		{
		case 0:		// Collecting host, wait for ':' or '/'
			if (c == L':' || c == L'/')
			{
				// source = length of host string
				m_szHost = new WCHAR [source + 1];
				wcsncpy (m_szHost, lpzURL, source);
				m_szHost [source] = L'\0';

				if (c == L':')
				{
					// Convert port number - will ignore non-numeric chars
					m_nPort = (WORD) _wtoi (lpzURL + source + 1);
					state = 1;
				}
				else
				{
					path = source;
					state = 2;
				}
			}
			break;

		case 1:		// Skipping port, wait for '/'
			if (c == L'/')
			{
				path = source;
				state = 2;
			}
			break;

		case 2:		// Waiting for end of input, do nothing
			break;
		}

		source++;
	}

	// End of input reached, process according to current state
	switch (state)
	{
	case 0:		// Collecting host, source = length of host string
		m_szHost = new WCHAR [source + 1];
		wcsncpy (m_szHost, lpzURL, source);
		m_szHost [source] = L'\0';
		break;

	case 1:		// Skipping port chars, do nothing
		break;

	case 2:		// Collecting path, copy from start of path to end of string
		m_szPath = new WCHAR [source - path + 1];
		wcscpy (m_szPath, lpzURL + path);
		break;
	}

	// Must at least have host
	if (!m_szHost || wcslen (m_szHost) == 0)
		return false;

	return true;
}

//if the URL has a port it will overwite any previous port settings
/*bool CSend::CrackURL(const LPCTSTR lpzURL)
{
	LPTSTR pTemp,pURL = (LPTSTR)lpzURL;
		
	int iLenURL = _tcsclen(lpzURL);

	if(!_memicmp(pURL,_T("http://"),14)){
		//move past the protocol
		pURL+=7;
	}
   
	delete [] m_szHost;
	m_szHost = new TCHAR[iLenURL + 1];
	if(!m_szHost){
		return false;
	}

	int iIndex,iAddIndex;
	iIndex = iAddIndex = 0;
	for(pTemp = pURL;pTemp[iIndex];iIndex++)
	{
		if(pTemp[iIndex]==_T(':')){
			m_nPort = (WORD)_ttoi(&pTemp[iIndex+1]);
			break;
		}
		else{
			m_szHost[iAddIndex++]= pTemp[iIndex];
		}
	}
	m_szHost[iAddIndex]=NULL;
		
	
	return true;

}*/

bool CSend::SetUserPass(LPCTSTR pUser,LPCTSTR pPass)
{
	int iPassLen,iUserLen;

	iUserLen = _tcslen(pPass);
	iPassLen = _tcslen(pUser);
	if(!iUserLen || !iPassLen){
		return false;//user or pass is zero length
	}
	delete [] m_SzUser;
	delete [] m_SzPass;
	m_SzUser = new TCHAR[iUserLen + 1];
	m_SzPass = new TCHAR[iPassLen + 1];
	if(!m_SzUser || !m_SzPass){
		delete [] m_SzUser;
		delete [] m_SzPass;
		m_SzUser = m_SzPass = NULL;
		return false;
	}
	_tcscpy(m_SzUser,pUser);
	_tcscpy(m_SzPass,pPass);
	return true;
}

bool CSend::Open(LPCTSTR pHostAdd,LPCTSTR pUser,LPCTSTR pPass)
{
	if(!pHostAdd || *pHostAdd == 0){
		return FALSE;
	}
	bool bRet = false;
	
	if( !CrackURL(pHostAdd)){//decompose the url ie: protocol, port and address
		return false;
	}

	if(pUser && pPass){
		return SetUserPass(pUser,pPass);
	}
	
	return true;

}

//add a name value pair to be send in the next send
void CSend::AddNameValue(LPCTSTR pName,LPCTSTR pValue)
{
	if(*m_szPostData==NULL){
		// GD - add the server path, if any, in front of any name/value pairs
		if (m_szPath)
			_tcscat(m_szPostData ,m_szPath);

		_tcscat(m_szPostData ,_T("?"));

		//_tcscpy(m_szPostData ,_T("?"));
		_tcscat(m_szPostData,pName);
		_tcscat(m_szPostData+1,_T("="));
		_tcscat(m_szPostData+1,pValue);
	}
	else{
		_tcscat(m_szPostData+1,_T("&"));
		_tcscat(m_szPostData+1,pName);
		_tcscat(m_szPostData+1,_T("="));
		_tcscat(m_szPostData+1,pValue);

	}


}


int CSend::Send()//return error code
{
	HINTERNET  hHttpRequest;
	int iRet = 1;//set to general error
	if(!m_szHost){
		return SEND_GENERAL_ERR;
	}
	
	if(m_hInet == NULL){//if we don't already have a connection
		if((iRet = open())!=0){//if we cannot open return the error code
			return iRet;
		}
	}
	
	DWORD dwFlags = INTERNET_FLAG_IGNORE_REDIRECT_TO_HTTP |
							INTERNET_FLAG_IGNORE_REDIRECT_TO_HTTPS | 
							INTERNET_FLAG_NO_AUTH |
							INTERNET_FLAG_NO_AUTO_REDIRECT |
							INTERNET_FLAG_NO_COOKIES |
							INTERNET_FLAG_NO_UI |
							INTERNET_FLAG_RELOAD;
	
	TCHAR szBoundary[50];
	char pcBoundary[50];
	TCHAR szContentTypeHeader[100];
	char *pResponseStr = NULL;
	
	memset(szBoundary, 0, 50*sizeof(TCHAR));
	GenerateMultipartBoundary(szBoundary);	
	wcstombs(pcBoundary, szBoundary, wcslen(szBoundary)+1);
	wsprintf(szContentTypeHeader, L"Content-Type: multipart/form-data; boundary=%s", szBoundary);
	

	// Open an HTTP request handle. 
	hHttpRequest= HttpOpenRequest(
	m_hConnection, 
	L"POST", 
	m_szPostData,//the data string to send EG: ?severity=3&timeHour=8
	HTTP_VERSION, 
	NULL, 
	NULL, 
	dwFlags, 
	0);
	
	
	if(hHttpRequest== NULL){
		return SEND_REQUEST_ERR;
		goto _cleanup;	
	}
  
  
	if(!HttpAddRequestHeaders(hHttpRequest, szContentTypeHeader, -1, HTTP_ADDREQ_FLAG_ADD)){
		iRet = SEND_REQUEST_ERR;
		goto _cleanup;	
	}
			
	// Send a request to the HTTP server. 
	if(!HttpSendRequest (hHttpRequest, NULL, 0, NULL,0)){
		OutputDebugString(L"Failed to send HTTP log entry\n");
		iRet = SEND_REQUEST_ERR;
		goto _cleanup;
	}
	

	LPTSTR	pResponseHeader = NULL;
	DWORD	dwSize;

	
	// we have sent our data now get the response
	// Call HttpQueryInfo to find out the size of the headers.
	HttpQueryInfo (hHttpRequest, HTTP_QUERY_RAW_HEADERS_CRLF, NULL, &dwSize, NULL);
	
	if(!dwSize){
		goto _cleanup;
	}
	// Allocate a block of memory for lpHeadersA.
	dwSize = dwSize*4;
	
	pResponseHeader = new TCHAR [dwSize+1];
	if(!pResponseHeader){
		goto _cleanup;
	}
	
	//
	HttpQueryInfo (hHttpRequest,HTTP_QUERY_RAW_HEADERS_CRLF, (LPVOID) pResponseHeader, &dwSize, NULL);
	delete [] pResponseHeader;

	InternetQueryDataAvailable(hHttpRequest, &dwSize,0,0);		
	if(!dwSize){
		goto _cleanup;
	}
	
	pResponseStr = new char [dwSize + 1];
	if(!pResponseStr){
		goto _cleanup;
	}
	memset(pResponseStr, 0x00, dwSize + 1);
	
	
	if(!InternetReadFile (hHttpRequest, (LPVOID)pResponseStr, dwSize, &dwSize)){
		goto _cleanup;
	}
	if (dwSize){
		pResponseStr [dwSize] = '\0';

		// GD - moved to cleanup in case http request fails
		// 3/6/10
		//memset(m_szPostData,0,MAX_URL);//reset the request strings now that we have successfully sent

		size_t iRespLen = strlen(pResponseStr);
		m_SzResponse = new TCHAR[iRespLen + 1];
		if(m_SzResponse){
			if(mbstowcs(m_SzResponse,pResponseStr,iRespLen)==iRespLen){
				iRet =  0;
			}
		}
	}
	
	
_cleanup:
	InternetCloseHandle(hHttpRequest);
	delete [] pResponseStr;

	memset(m_szPostData,0,MAX_URL);//reset the request strings

	return iRet;

		
}

//opens a connection to the url 
int CSend::open()
{
	
	if(m_hInet == NULL){
		m_hInet = InternetOpen(	APP_NAME_TEST,INTERNET_OPEN_TYPE_DIRECT ,NULL,NULL,0);
		if(m_hInet == NULL){
			return SEND_INTERNET_ERR;
		}
	}
	
	// Open an internet connection handle 
	m_hConnection =  
		InternetConnect(
		m_hInet,        //class scope handle
		m_szHost,     //class scope host eg: www.symbol.com    or   157.235.111.12 
		m_nPort, 
		m_SzUser, 
		m_SzPass, 
		INTERNET_SERVICE_HTTP, 
		0 , 
		0
		);
	
	if(m_hConnection== NULL){
		return SEND_CONNECT_ERR;
			
	}
	return 0;
}	
  






void CSend::close()
{
	if(m_hInet){	
		if(m_hConnection){
			InternetCloseHandle(m_hConnection);
			m_hConnection = NULL;
		}
		CloseHandle(m_hInet);
		m_hInet = NULL;

	}
	
}

/**
* \author	Dave Sheppard (DS)
* \date		April 2005 - Initial Creation
* \date		December 2009 - Ported from SPB2.22, PH
*/
void CSend::GenerateMultipartBoundary(LPTSTR boundaryPrefix)
{
	for (int i= 0; i<27; i++) boundaryPrefix[i] = L'-';

	// We now need 12 hex digits.
	int r0 = rand() & 0xffff;
	int r1 = rand() & 0xffff;
	int r2 = rand() & 0xffff;

	wsprintf(&boundaryPrefix[27], L"%04X%04X%04X", r0, r1, r2);
}


