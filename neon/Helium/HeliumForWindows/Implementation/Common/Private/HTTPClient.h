////////////////////////////////////////////////////////////////////////////////
// Filename: HTTPClient.h
// Author:   Darryn Campbell
// Date:     Nov 2008
//
// Description: HTTP Client class implementas an HTTP Client
//
////////////////////////////////////////////////////////////////////////////////
#include <Wininet.h>
#include "common_defines.h"

/////////////////////
//  Erroneous return values
//  If you change these values it will impact the Logger module class 
//  CHTTPLogger.
#define MEMORY_ERROR 1
#define INVALID_URL 2
#define UNABLE_TO_CREATE_HTTP_CONNECTION 3
#define UNABLE_TO_OPEN_HTTP_REQUEST 4
#define UNABLE_TO_ADD_HTTP_HEADERS 5
#define UNABLE_TO_CONNECT_TO_SERVER 6
#define UNABLE_TO_RETRIEVE_HTTP_HEADERS_RETURNED 7
#define HTTP_RESPONSE_NOT_200_OK 8
#define UNABLE_TO_READ_SERVER_RETURN_DATA 9

//  Class to encapsulate the functionality of an HTTP Client
class CHTTPClient
{
	///////////////
	//  Public   //
	///////////////
public:
	//  Constructor / Destructor
	CHTTPClient();
	~CHTTPClient();

	////////////////////////////////////////////////////////////////////////
	// Function:	StatelessPost
	// Description:	Perform an HTTP Post at the Server on the specified URL and 
	//				port.  The URL should be of the form: 
	//				http://<ip address or friendly name>/<server file>?<logParams>
	//				Disregard information returned by the Server and drop the 
	//				connection.
	//				This method is used by the HTTP logger, the reason for making
	//				it stateless is it greatly simplifies the connection error 
	//				handling.
	// Returns:		TRUE if successful, else the error which caused the 
	//				failure.
	// Author:		Darryn Campbell
	// Date:		November 2008
	////////////////////////////////////////////////////////////////////////
	DWORD StatelessPost(LPCTSTR szURL, INTERNET_PORT port = 80);

	///////////////
	//  Private  //
	///////////////
private:
	//  Private Methods:

	////////////////////////////////////////////////////////////////////////
	// Function:	StatelessPostCleanup
	// Description:	Free the memory associated with a call to the 
	//				StatelessPost method.
	// Author:		Darryn Campbell
	// Date:		November 2008
	////////////////////////////////////////////////////////////////////////
	void StatelessPostCleanup();

	////////////////////////////////////////////////////////////////////////
	// Function:	GenerateMultipartBoundary
	// Description:	To send HTTP we need a unique boundary (normall with 27 -'s at 
	//				the start.  We then generate 3 x 4 hexdigits to follow the -'s
	// Author:		Dave Sheppard
	// Date:		27 April 2005
	////////////////////////////////////////////////////////////////////////
	void GenerateMultipartBoundary(TCHAR* boundaryPrefix);

	////////////////////////////////////////////////////////////////////////
	//  Function:	CrackURL
	//  Desc:		Populates m_lpszHost and m_lpszURLPath with the Hostname
	//				and URL Path respectively given the passed, full URL
	//  Author:		Dave Sheppard (Modified by Darryn Campbell November 2008)
	//  Returns:	NULL on error or a pointer to the Hostname on success.
	//  Date:		April 2005
	////////////////////////////////////////////////////////////////////////
	LPCTSTR CrackURL(const LPCTSTR lpzURL);

	////////////////////////////////////////////////////////////////////////
	// Function:	HasHTTP
	// Description:	Checks for inclusion of the 'http://' protocol specifier
	// Author:		Dave Sheppard 
	// Date:		April 2005
	////////////////////////////////////////////////////////////////////////
	BOOL HasHttp(const LPCTSTR lpzURL);

	////////////////////////////////////////////////////////////////////////
	// Function:	SetResponseStr
	// Description:	Populates the m_szServerResponse variable with the response from
	//				the server.
	// Author:		Dave Sheppard 
	// Date:		April 2005
	////////////////////////////////////////////////////////////////////////
	BOOL SetResponseStr(char *szResponse);

	////////////////////////////////////////////////////////////////////////
	// Function:	StripCRLF
	// Description:	Converts the character sequence CR LF to NULL LF along
	//				the length of the entire passed string.
	// Author:		Dave Sheppard 
	// Date:		April 2005
	////////////////////////////////////////////////////////////////////////
	BOOL StripCRLF(char* pStr);

	/////////////////
	//  Private Attributes

	//  Attributes associated with current stateless post's Internet session 
	HINTERNET		m_hInet;
	HINTERNET		m_hConnection;
	HINTERNET		m_hHttpRequest;
	LPTSTR			m_lpszHost;		//  Hostname
	LPTSTR			m_lpszURLPath;	//  URL Path
	LPTSTR			m_szServerResponse;	//  Server Response

};