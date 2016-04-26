////////////////////////////////////////////////////////////////////////////////
// Filename: HTTPClient.cpp
// Author:   Darryn Campbell
// Date:     November 2008
//
// Description: Implementation of HTTP Client Class.
//
////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "HTTPClient.h"

////////////////////////////////////////////////////////////////////////
// Function:	CHTTPClient Constructor
// Description:	Initialise the memory and structures required by the 
//				HTTP Client class.
// Author:		Darryn Campbell
// Date:		November 2008
////////////////////////////////////////////////////////////////////////
CHTTPClient::CHTTPClient()
{
	m_hInet			= NULL;
	m_hConnection	= NULL;
	m_hHttpRequest	= NULL;
}

////////////////////////////////////////////////////////////////////////
// Function:	CHTTPClient Constructor
// Description:	Initialise the memory and structures required by the 
//				HTTP Client class.
// Author:		Darryn Campbell
// Date:		November 2008
////////////////////////////////////////////////////////////////////////
CHTTPClient::~CHTTPClient()
{
	delete [] m_lpszHost;
	m_lpszHost = NULL;
	delete [] m_lpszURLPath;
	m_lpszURLPath = NULL;

	delete [] m_szServerResponse;
	m_szServerResponse = NULL;

	if (m_hInet)
		CloseHandle(m_hInet);
	m_hInet = NULL;

	InternetCloseHandle(m_hConnection);
	m_hConnection = NULL;

    InternetCloseHandle(m_hHttpRequest);
	m_hHttpRequest = NULL;
}


////////////////////////////////////////////////////////////////////////
// Function:	StatelessPost
// Description:	Perform an HTTP Post at the Server on the specified URL and 
//				port.  The URL should be of the form: 
//				http://<ip address or friendly name>/<server file>?<logParams>
//				Disregard information returned by the Server and drop the 
//				connection.
// Returns:		TRUE if successful, else the error which caused the 
//				failure.
// Author:		Darryn Campbell
// Date:		November 2008
////////////////////////////////////////////////////////////////////////
DWORD CHTTPClient::StatelessPost(LPCTSTR szURL, INTERNET_PORT port)
{
	BOOL bOk, bRet;			//  Temp variables for return values
	LPTSTR	lpHeadersW;		//  Wide Headers
	char	*lpBufferA;		//  Ansi Headers
	DWORD	dwSize;
	TCHAR szBoundary[50];
	char pcBoundary[50];
	TCHAR szContentTypeHeader[100];
	DWORD dwHTTPRequestFlags = INTERNET_FLAG_IGNORE_REDIRECT_TO_HTTP |
							INTERNET_FLAG_IGNORE_REDIRECT_TO_HTTPS | 
							INTERNET_FLAG_NO_AUTH |
							INTERNET_FLAG_NO_AUTO_REDIRECT |
							INTERNET_FLAG_NO_COOKIES |
							INTERNET_FLAG_NO_UI |
							INTERNET_FLAG_RELOAD;
	
	memset(szBoundary, 0, 50*sizeof(TCHAR));
	GenerateMultipartBoundary(szBoundary);	
	wcstombs(pcBoundary, szBoundary, wcslen(szBoundary)+1);
	wsprintf(szContentTypeHeader, L"Content-Type: multipart/form-data; boundary=%s", szBoundary);
	
	//  Split the passed URL into two parts.  
	//  The hostname is stored in m_lpszHost.
	//  The URL path is stored in m_lpszURLPath.
	if (CrackURL(szURL) == NULL)
	{
		//  Failed to split the URL, its format is invalid
		StatelessPostCleanup();
		return INVALID_URL;
	}
	
	//  Initialise WinINet functions if not already done so
	if(m_hInet == NULL)
		m_hInet = 
			InternetOpen(	
				APP_NAME,
				INTERNET_OPEN_TYPE_DIRECT ,
				NULL, NULL, 0);
	
	// Open an HTTP session to the specified site
	m_hConnection =  
		InternetConnect(
		m_hInet,        //  Handle returned from InternetOpen
		m_lpszHost,     //  Hostname eg: www.symbol.com    or   157.235.111.12 
		port,			//  port to connect on - defaults to 80.
		L"",			//  Username
		L"",			//  Password
		INTERNET_SERVICE_HTTP, 
		0 , 
		0
		);
	
	if(m_hConnection== NULL)
	{
		//  Failed to open HTTP session, this is an internal error
		//  UNABLE_TO_CREATE_HTTP_CONNECTION
		StatelessPostCleanup();
		return UNABLE_TO_CREATE_HTTP_CONNECTION;
	}
	
	// Open an HTTP request handle. 
	m_hHttpRequest = HttpOpenRequest(
		m_hConnection,		//  Handle returned by Internet Connect
		L"POST",			//  HTTP Verb
		m_lpszURLPath,		//  file.asp?key-value pairs
		HTTP_VERSION, 
		NULL, 
		NULL, 
		dwHTTPRequestFlags, //  Internet Options
		0);
	
	if(m_hHttpRequest== NULL)
	{
		//  UNABLE_TO_OPEN_HTTP_REQUEST
		StatelessPostCleanup();
		return UNABLE_TO_OPEN_HTTP_REQUEST;	
	}
  
  	bOk = 
		HttpAddRequestHeaders(
			m_hHttpRequest, 
			szContentTypeHeader, 
			-1, HTTP_ADDREQ_FLAG_ADD);

	if(!bOk)
	{
		//  UNABLE_TO_ADD_HTTP_HEADERS
		StatelessPostCleanup();
		return UNABLE_TO_ADD_HTTP_HEADERS;
	}
	
	//  Establish Connection to Server.  No additional headers
	bOk = HttpSendRequest (m_hHttpRequest, NULL, 0, NULL, 0);
	if(!bOk)
	{
		//  UNABLE_TO_CONNECT_TO_SERVER
		StatelessPostCleanup();
		return UNABLE_TO_CONNECT_TO_SERVER;
	}
	
	//  Call HttpQueryInfo to find out the size of the headers.
	HttpQueryInfo (m_hHttpRequest, HTTP_QUERY_RAW_HEADERS_CRLF, NULL, &dwSize, NULL);
	
	//  Allocate a block of memory for lpHeadersA.
	dwSize = dwSize*4;
	lpHeadersW = new TCHAR [dwSize+1];
	
	//  Call HttpQueryInfo again to get the headers.
	bOk = HttpQueryInfo(
			m_hHttpRequest, 
			HTTP_QUERY_RAW_HEADERS_CRLF, 
			(LPVOID) lpHeadersW, 
			&dwSize, NULL);

	if(!bOk)
	{
		//  UNABLE_TO_RETRIEVE_HTTP_HEADERS_RETURNED
		delete [] lpHeadersW;
		StatelessPostCleanup();
		return UNABLE_TO_RETRIEVE_HTTP_HEADERS_RETURNED;
	}

	//  Check the returned header is 200 OK
	TCHAR* pszOK = NULL;
	pszOK = wcsstr(lpHeadersW, L"200 OK");
	if (pszOK == NULL)
	{
		//  Return value was not 200 OK
		//  HTTP_RESPONSE_NOT_200_OK
		delete[] lpHeadersW;
		StatelessPostCleanup();
		return HTTP_RESPONSE_NOT_200_OK;
	}
	delete[] lpHeadersW;

	////////////////////////
	//  Junk any data the server puts in our receive buffer.
	
	//determine how many bytes are to be read from the server
	InternetQueryDataAvailable(m_hHttpRequest, &dwSize,0,0);
	
	// Allocate a block of memory for lpBufferA.
	lpBufferA = new char [dwSize + 1];
	memset(lpBufferA, 0x00, dwSize + 1);
	
	//  Read the data returned from the server
	bOk = InternetReadFile(
			m_hHttpRequest, 
			(LPVOID)lpBufferA, 
			dwSize, &dwSize);

	if(!bOk)
	{
		//  UNABLE_TO_READ_SERVER_RETURN_DATA
		delete [] lpBufferA;
		StatelessPostCleanup();
		return UNABLE_TO_READ_SERVER_RETURN_DATA;
	}
	if (dwSize != 0)
    lpBufferA [dwSize] = '\0';

	//  Copy the response into m_szServerResponse
	bRet = SetResponseStr(lpBufferA); 
	delete [] lpBufferA;

	StatelessPostCleanup();
	if (bRet)
		return  bRet;
	else
		return MEMORY_ERROR;	//  Unable to copy to m_szServerResponse
}

///////////////////////////////////////////////////////////////////
//
//  Private Functions
//
///////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////
// Function:	StatelessPostCleanup
// Description:	Free the memory associated with a call to the 
//				StatelessPost method.
// Author:		Darryn Campbell
// Date:		November 2008
////////////////////////////////////////////////////////////////////////
void CHTTPClient::StatelessPostCleanup()
{
	InternetCloseHandle(m_hConnection);
    InternetCloseHandle(m_hHttpRequest);
}


////////////////////////////////////////////////////////////////////////
// Function:	GenerateMultipartBoundary
// Description:	To send HTTP we need a unique boundary (normall with 27 -'s at 
//				the start.  We then generate 3 x 4 hexdigits to follow the -'s
// Author:		Dave Sheppard
// Date:		27 April 2005
////////////////////////////////////////////////////////////////////////
void CHTTPClient::GenerateMultipartBoundary(TCHAR* boundaryPrefix)
{
	for (int i= 0; i<27; i++) boundaryPrefix[i] = L'-';

	// We now need 12 hex digits.
	int r0 = rand() & 0xffff;
	int r1 = rand() & 0xffff;
	int r2 = rand() & 0xffff;

	wsprintf(&boundaryPrefix[27], L"%04X%04X%04X", r0, r1, r2);
}


////////////////////////////////////////////////////////////////////////
//  Function:	CrackURL
//  Desc:		Populates m_lpszHost and m_lpszURLPath with the Hostname
//				and URL Path respectively given the passed, full URL
//  Author:		Dave Sheppard (Modified by Darryn Campbell November 2008)
//  Returns:	NULL on error or a pointer to the Hostname on success.
//  Date:		April 2005
////////////////////////////////////////////////////////////////////////
LPCTSTR CHTTPClient::CrackURL(const LPCTSTR lpzURL)
{
	
	LPTSTR lp,lpSzURL = (LPTSTR)lpzURL;
	int iLenURL = wcslen(lpzURL);
	
	//  If the address has an HTTP:// at its head, advance past it.
	if(HasHttp(lpzURL))
		lpSzURL = (LPTSTR)lpzURL + 7;
	
	delete [] m_lpszHost;
	m_lpszHost = NULL;
	delete [] m_lpszURLPath;
	m_lpszURLPath = NULL;
	
	m_lpszHost		= new TCHAR[iLenURL + 1];
	m_lpszURLPath	= new TCHAR[iLenURL + 1];

	if(!m_lpszHost || !m_lpszURLPath){
		//  Memory allocation error
		return NULL;
	}

	//  Check the specified URL actually contains a '/'
	if (wcsstr(lpSzURL, L"/") == NULL)
	{
		//  '/' was not found in the specified URL, 
		return NULL;
	}
	
	
	for(lp = (LPTSTR)lpSzURL; *lp != L'/'; lp++);
		
	wcscpy(m_lpszHost,lpSzURL);
	*(m_lpszHost + (lp-lpSzURL)) = NULL;
	wcscpy(m_lpszURLPath,lp);
	
	return m_lpszHost;
}

////////////////////////////////////////////////////////////////////////
// Function:	HasHTTP
// Description:	Checks for inclusion of the 'http://' protocol specifier
// Author:		Dave Sheppard 
// Date:		April 2005
////////////////////////////////////////////////////////////////////////
BOOL CHTTPClient::HasHttp(const LPCTSTR lpzURL)
{
	//this is used for the toupper string op
	LPTSTR lpSzTemp,lp;
	lpSzTemp		= new TCHAR[wcslen(lpzURL) + 1];
	if(lpSzTemp == NULL){
		//error 
		return NULL;
	}
	wcscpy(lpSzTemp,lpzURL);
	//convert the string to Uppercase
	for(lp = lpSzTemp;*lp;lp++)
		*lp = towupper(*lp);

	
	if(wcsstr(lpSzTemp,L"HTTP://")){
		delete [] lpSzTemp;
		return TRUE;
	}
	delete [] lpSzTemp;
	return FALSE;
}

////////////////////////////////////////////////////////////////////////
// Function:	SetResponseStr
// Description:	Populates the m_szServerResponse variable with the response from
//				the server.
// Author:		Dave Sheppard 
// Date:		April 2005
////////////////////////////////////////////////////////////////////////
BOOL CHTTPClient::SetResponseStr(char *szResponse)
{
	StripCRLF(szResponse);
	
	delete [] m_szServerResponse;
	m_szServerResponse = new TCHAR[strlen(szResponse) + 1];
	if(m_szServerResponse == NULL){
		return FALSE;
	}
	mbstowcs( m_szServerResponse, szResponse,strlen(szResponse));
	DEBUGMSG(1,(L"%s",m_szServerResponse));
	return TRUE;
}

////////////////////////////////////////////////////////////////////////
// Function:	StripCRLF
// Description:	Converts the character sequence CR LF to NULL LF along
//				the length of the entire passed string.
// Author:		Dave Sheppard 
// Date:		April 2005
////////////////////////////////////////////////////////////////////////
BOOL CHTTPClient::StripCRLF(char* pStr)
{
	int index,len = strlen(pStr);
	
	for(index = 0;index < len;index++){
		if(*(pStr + index) == 0x0D)
			if(*(pStr + index + 1) == 0x0A){
				*(pStr + index) = NULL;
				return TRUE;
			}
	}
	return FALSE;
}

