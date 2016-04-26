/******************************************************************************/
#include <windows.h>
#include <stdarg.h>
#include "HttpServer.h"

/******************************************************************************/
// CHttpRequest
/******************************************************************************/
CHttpRequest::CHttpRequest (int count)
{
	ppNames = NULL;
	ppValues = NULL;
	nCount = count;

	if (nCount > 0)
	{
		// Allocate array of pointers to strings and initialise to NULL
		ppNames = new WCHAR* [nCount];
		memset (ppNames, 0, nCount * sizeof (WCHAR*));

		ppValues = new WCHAR* [nCount];
		memset (ppValues, 0, nCount * sizeof (WCHAR*));
	}
}

/******************************************************************************/
CHttpRequest::~CHttpRequest ()
{
	int n;

	if (nCount > 0)
	{
		for (n = 0; n < nCount; n++)
		{
			free (ppNames [n]);
			free (ppValues [n]);
		}

		delete [] ppNames;
		delete [] ppValues;
	}
}

/******************************************************************************/
void CHttpRequest::SetName (int n, WCHAR *pname)
{
	if (n < 0 || n >= nCount)
		return;

	// Deallocate any existing string
	if (ppNames [n])
		free (ppNames [n]);

	// Allocate and copy new one
	ppNames [n] = _wcsdup (pname);
}

/******************************************************************************/
void CHttpRequest::SetValue (int n, WCHAR *pvalue)
{
	if (n < 0 || n >= nCount)
		return;

	// Deallocate any existing string
	if (ppValues [n])
		free (ppValues [n]);

	// Allocate and copy new one
	ppValues [n] = _wcsdup (pvalue);
}

/******************************************************************************/
void CHttpRequest::SetNameValue (int n, WCHAR *pname, WCHAR *pvalue)
{
	if (n < 0 || n >= nCount)
		return;

	// Deallocate any existing string
	if (ppNames [n])
		free (ppNames [n]);

	// Allocate and copy new one
	ppNames [n] = _wcsdup (pname);

	// Deallocate any existing string
	if (ppValues [n])
		free (ppValues [n]);

	// Allocate and copy new one
	ppValues [n] = _wcsdup (pvalue);
}

/******************************************************************************/
void CHttpRequest::SetNameValue (int n, char *pname, char *pvalue)
{
	int length;

	if (n < 0 || n >= nCount)
		return;

	// Deallocate any existing string
	if (ppNames [n])
		free (ppNames [n]);

	// Allocate buffer, convert to Unicode, null terminate
	length = strlen (pname);
	ppNames [n] = (WCHAR*) malloc ((length + 1) * sizeof WCHAR);
	MultiByteToWideChar (CP_ACP, 0, pname, length, ppNames [n], length + 1);

	// Deallocate any existing string
	if (ppValues [n])
		free (ppValues [n]);

	// Allocate buffer, convert to Unicode, null terminate
	length = strlen (pvalue);
	ppValues [n] = (WCHAR*) malloc ((length + 1) * sizeof WCHAR);
	MultiByteToWideChar (CP_ACP, 0, pvalue, length, ppValues [n], length + 1);
}

/******************************************************************************/
LPCWSTR CHttpRequest::GetName (int n)
{
	if (n < 0 || n >= nCount)
		return NULL;

	return ppNames [n];
}

/******************************************************************************/
LPCWSTR CHttpRequest::GetValue (int n)
{
	if (n < 0 || n >= nCount)
		return NULL;

	return ppValues [n];
}

/******************************************************************************/
// CHttpServer
/******************************************************************************/
int CHttpServer::nWSACount = 0;

/******************************************************************************/
CHttpServer::CHttpServer (PREQUESTPROC callback)
{
	WSADATA wsa_data;

	// Initialise Windows sockets one time for all instances
	if (nWSACount++ == 0)
	{
		WSAStartup (WINSOCK_VERSION, &wsa_data);
	}

	pUserData = NULL;
	strcpy (sPath, "");
	wcscpy (wsResponseFile, L"");
	wcscpy (wsPasskey, L"");
	pRequestProc = callback;
	hServerSocket = INVALID_SOCKET;
	hAcceptSocket = INVALID_SOCKET;
	hServerThread = INVALID_HANDLE_VALUE;
	nPort = 80;
}

/******************************************************************************/
CHttpServer::~CHttpServer ()
{
	Stop ();

	if (--nWSACount == 0)
	{
		WSACleanup ();
	}
}

/******************************************************************************/
void CHttpServer::GetLocalAddress (WCHAR *paddress, int length)
{
	char name [64];
	struct hostent *phost;
	struct in_addr addr;

	*paddress = L'\0';

	// Returns first local IP address
	// Following calls should never fail
	if (gethostname (name, sizeof name) == SOCKET_ERROR)
		return;

	if ((phost = gethostbyname (name)) == NULL)
		return;

	memcpy (&addr, phost->h_addr_list [0], sizeof addr);
	MultiByteToWideChar (CP_ACP, 0, inet_ntoa (addr), -1, paddress, length);
}

/******************************************************************************/
void CHttpServer::SetLastError (LPCWSTR pformat, ...)
{
	va_list args;
	va_start (args, pformat);
	StringCchVPrintf (wsError, COUNTOF (wsError), pformat, args);
}

/******************************************************************************/
void CHttpServer::GetLastError (LPWSTR perror, int length)
{
	StringCchCopy (perror, length, wsError);
}

/******************************************************************************/
void CHttpServer::NotifyRequest (CHttpRequest* prequest)
{
	if (pRequestProc)
	{
		(*pRequestProc) (prequest, this);
	}
}

/******************************************************************************/
void CHttpServer::SetPath (LPCWSTR ppath)
{
	WideCharToMultiByte (CP_ACP, 0, ppath, -1, sPath, sizeof sPath - 1, NULL, NULL);
	sPath [sizeof sPath - 1] = '\0';
}

/******************************************************************************/
void CHttpServer::SetResponseFile (LPCWSTR pfilename)
{
	WSAFECOPY (wsResponseFile, pfilename);
}

/******************************************************************************/
void CHttpServer::SetPasskey (LPCWSTR pcode)
{
	WSAFECOPY (wsPasskey, pcode);
}

/******************************************************************************/
int CHttpServer::Start ()
{
	SOCKET new_socket;
	SOCKADDR_IN sin;
	int result, error;

	// Check for already started
	if (hServerSocket != INVALID_SOCKET)
	{
		return HS_SUCCESS;
	}

	result = HS_ERROR;

	if ((new_socket = socket (AF_INET, SOCK_STREAM, IPPROTO_IP)) == INVALID_SOCKET)
	{
		SetLastError (L"socket() returned %d", WSAGetLastError ());
		goto Exit;
	}

	sin.sin_family = AF_INET;
	sin.sin_port = htons (nPort);
	sin.sin_addr.S_un.S_addr = INADDR_ANY;
	if (bind (new_socket, (SOCKADDR*) &sin, sizeof sin) == SOCKET_ERROR)
	{
		error = WSAGetLastError ();
		if (error == WSAEADDRINUSE)
			SetLastError (L"Port %d already in use", nPort);
		else
			SetLastError (L"bind() returned %d", error);
		goto Exit;
	}

	if (listen (new_socket, 0) == SOCKET_ERROR)
	{
		SetLastError (L"listen() returned %d", WSAGetLastError ());
		goto Exit;
	}

	// Record listening socket handle and start thread to wait for connections
	hServerSocket = new_socket;
	hServerThread = CreateThread (NULL, 0, ServerProc, this, 0, NULL);

	result = HS_SUCCESS;

Exit:
	if (result != HS_SUCCESS)
	{
		Stop ();
	}

	return result;
}

/******************************************************************************/
void CHttpServer::Stop ()
{
	if (hAcceptSocket != INVALID_SOCKET)
	{
		closesocket (hAcceptSocket);
		hAcceptSocket = INVALID_SOCKET;
	}

	if (hServerSocket != INVALID_SOCKET)
	{
		closesocket (hServerSocket);
		hServerSocket = INVALID_SOCKET;
	}

	if (hServerThread != INVALID_HANDLE_VALUE)
	{
		WaitForSingleObject (hServerThread, 5000);
		CloseHandle (hServerThread);
		hServerThread = INVALID_HANDLE_VALUE;
	}
}

/******************************************************************************/
DWORD CHttpServer::ServerProc (LPVOID pparam)
{
	CHttpServer *pthis = (CHttpServer*) pparam;
	pthis->ProcessServer ();

	return 0;
}

/******************************************************************************/
void CHttpServer::ProcessServer(void)
{
	SOCKADDR_IN sin;
	int state, n, index, error;
	bool done;
	bool isCors;
	char *pline, *pheadername, *pheadervalue;

	pline = new char [MAX_BUFFER];

	while (1)
	{
		// Wait for connection
		n = sizeof sin;

		// Abhineet - Quit if socket is closed or invalid
		if (hServerSocket == INVALID_SOCKET)
		{
			break;
		}

		hAcceptSocket = accept (hServerSocket, (SOCKADDR*) &sin, &n);

		// Abhineet - Continue if listening socket is closed or invalid
		if (hAcceptSocket == INVALID_SOCKET)
		{
			continue;
		}

		// Set linger option for socket
		struct linger linger;
		linger.l_onoff = 1;
		linger.l_linger = 2;
		int linger_size = sizeof linger;
		setsockopt (hAcceptSocket, SOL_SOCKET, SO_LINGER, (char*) &linger, sizeof linger);

		// Reset index into line being formed
		index = 0;

		// Reset data proessing variables
		pByteBuffer = new BYTE [MAX_BUFFER];
		nBufferLength = 0;
		nBufferIndex = 0;

		// Reset body data
		pBodyData = NULL;
		nBodyDataLength = 0;
		
		//Reset all state
		state = 0;
		done = false;
		isCors = false;

		while (!done)
		{
			switch (state)
			{
			case 0:
				// Wait for GET/POST
				if (WaitForLine(pline, MAX_BUFFER) == -1)
				{
					done = true;
					break;
				}

				ParseMethod (pline);

				state = 1;

			case 1:
				// Wait for header
				if (WaitForLine(pline, MAX_BUFFER) == -1)
				{
					done = true;
					break;
				}

				// Last header received?
				if (pline [0] == 0)
				{
					// Get body data, if any
					if (nBodyDataLength > 0)
					{
						if (pBodyData)
							delete [] pBodyData;

						// We'll null-terminate the data in case we treat it as a string later
						pBodyData = new BYTE [nBodyDataLength + 1];

						if (WaitForData (pBodyData, nBodyDataLength) == -1)
						{
							done = true;
							break;
						}

						pBodyData [nBodyDataLength] = 0;
					}

					// All request data now received, so process it
					error = ProcessRequest ();

					if (pBodyData)
					{
						delete [] pBodyData;
						pBodyData = NULL;
					}

					// Send response
					SendResponse (error, isCors);

					// Close connection
					done = true;
				}
				else
				{
					// Get header name and value
					pheadername = strtok (pline, " :");
					if (pheadername)
					{
						pheadervalue = strtok (NULL, "\0");
						if (pheadervalue)
						{
							if (!_stricmp (pheadername, "Content-Length"))
								nBodyDataLength = atoi (pheadervalue);
							
							// Check for the presence of a CORS Origin header
							if (!_stricmp (pheadername, "Origin"))
							{
								// CORS will now be included when sending the http response
								isCors = true;
							}
						}
					}
				}

				break;
			}
		}

		closesocket (hAcceptSocket);
		hAcceptSocket = INVALID_SOCKET;

		delete [] pByteBuffer;
	}

	delete [] pline;
}

/******************************************************************************/
void CHttpServer::SendResponse (int error, bool isCors)
{
	static char* wkday [] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
	static char* month [] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
	static char html [] = "<html><head><title>RhoElements</title></head><body><h1>REPush received OK</h1></body></html>";
	char header [128];
	FILE *file;
	char *pbuffer;
	int n;
	long length;
	SYSTEMTIME now;

	// Send status
	if (error)
		strcpy (header, "HTTP/1.1 404 Not Found\r\n");
	else
		strcpy (header, "HTTP/1.1 200 OK\r\n");
	send (hAcceptSocket, header, strlen (header), 0);

	// Send headers
	strcpy (header, "Server: RhoElements\r\n");
	send (hAcceptSocket, header, strlen (header), 0);

	if(isCors)
	{
		strcpy (header, "Access-Control-Allow-Origin: *\r\n");
		send (hAcceptSocket, header, strlen (header), 0);
	}

	strcpy (header, "Connection: close\r\n");
	send (hAcceptSocket, header, strlen (header), 0);

	GetSystemTime (&now);
	sprintf (header, "Date: %s, %02d %s %04d %02d:%02d:%02d GMT\r\n", wkday [now.wDayOfWeek], now.wDay,
		month [now.wMonth], now.wYear, now.wHour, now.wMinute, now.wSecond);
	send (hAcceptSocket, header, strlen (header), 0);

	// Nothing more to send if error
	if (error)
	{
		strcpy (header, "\r\n");
		send (hAcceptSocket, header, strlen (header), 0);

		return;
	}

	strcpy (header, "Content-Type: text/html\r\n");
	send (hAcceptSocket, header, strlen (header), 0);

	// Send configured response file, if available, or default page
	if (wsResponseFile [0] != L'\0')
		file = _wfopen (wsResponseFile, L"rb");
	else
		file = NULL;

	if (file)
	{
		fseek (file, 0, SEEK_END);
		length = ftell (file);
		fseek (file, 0, SEEK_SET);
		sprintf (header, "Content-Length: %ld\r\n", length);
		send (hAcceptSocket, header, strlen (header), 0);

		strcpy (header, "\r\n");
		send (hAcceptSocket, header, strlen (header), 0);

		pbuffer = new char [MAX_BUFFER];
		while ((n = fread (pbuffer, 1, MAX_BUFFER, file)) != 0)
			send (hAcceptSocket, pbuffer, n, 0);
		fclose (file);
		//Abhineet - Deallocating memory, otherwise it may lead to 1KB leakage.
		delete [] pbuffer;
	}
	else
	{
		sprintf (header, "Content-Length: %d\r\n", strlen (html));
		send (hAcceptSocket, header, strlen (header), 0);

		strcpy (header, "\r\n");
		send (hAcceptSocket, header, strlen (header), 0);
		
		send (hAcceptSocket, html, strlen (html), 0);
	}
}

/******************************************************************************/
int CHttpServer::ProcessRequest (void)
{
	char *ppath, *pquery;
	int n, count;
	WCHAR **ppnames, **ppvalues;
	CHttpRequest *prequest;
	bool authenticated;

	// Extract path (part before '?')
	ppath = strtok (sURI, "?");

	// Check for correct path (not set means any path)
	if (!ppath)
		return -1;

	if (strlen (sPath) > 0 &&  _stricmp (ppath, sPath))
		return -1;

	if (!_stricmp (sMethod, "POST"))
	{
		// Query is in body data
		pquery = (char*) pBodyData;
	}
	else if (!_stricmp (sMethod, "GET"))
	{
		// Rest of URI is query string
		pquery = strtok (NULL, "\0");
	}
	else
		// Unknown method
		return -1;

	// ALLOW EMPTY QUERY TO BE PROCESSED
	// Anything to do?
	/*if (!pquery || !*pquery)
		return -1;*/

	// Analyse query string to name/value pairs
	URLDecode (pquery, &count, &ppnames, &ppvalues);

	// Authenticate request if passkey set
	authenticated = (wsPasskey [0] == L'\0');

	// Build object with name/value pairs and call back
	if (count > 0)
	{
		prequest = new CHttpRequest (count);

		for (n = 0; n < count; n++)
		{
			if (!_wcsicmp (ppnames [n], L"passkey"))
				if (!wcscmp (ppvalues [n], wsPasskey))
					authenticated = true;

			prequest->SetNameValue (n, ppnames [n], ppvalues [n]);

			delete [] ppnames [n];
			delete [] ppvalues [n];
		}

		delete [] ppnames;
		delete [] ppvalues;

		if (authenticated)
		{
			NotifyRequest (prequest);
			delete prequest;
		}
		else
		{
			delete prequest;
			return -1;
		}
	}

	return 0;
}

/******************************************************************************/
void CHttpServer::URLDecode (char *pquery, int *pcount, WCHAR ***pppnames, WCHAR ***pppvalues)
{
	char *pcopy, *ppair, *ppaircopy, *pseparator, *pvalue;
	int n, count, length;

	// Count number of NV pairs against copy of input
	pcopy = _strdup (pquery);
	count = 0;
	ppair = strtok (pcopy, "&;");
	while (ppair)
	{
		count++;
		ppair = strtok (NULL, "&;");
	}
	free (pcopy);

	// Pass count to caller, quit if nothing to do
	*pcount = count;
	if (count == 0)
		return;

	// Allocate memory
	*pppnames = new WCHAR* [count];
	*pppvalues = new WCHAR* [count];

	// Get NV pairs themselves
	n = 0;
	pcopy = _strdup (pquery);
	ppair = strtok (pcopy, "&;");
	while (ppair)
	{
		// Split pair
		ppaircopy = _strdup (ppair);
		pseparator = strchr (ppaircopy, '=');
		if (pseparator)
		{
			*pseparator = 0;
			pvalue = pseparator + 1;
		}
		else
			// Point pvalue at terminating null, i.e. empty string
			pvalue = ppaircopy + strlen (ppaircopy);

		// Remove URL encoding in place
		RemoveEncoding (ppaircopy, ppaircopy);
		RemoveEncoding (pvalue, pvalue);

		// Allocate memory and copy to caller
		length = strlen (ppaircopy) + 1;
		(*pppnames) [n] = new WCHAR [length];
		MultiByteToWideChar (CP_ACP, 0, ppaircopy, -1, (*pppnames) [n], length);

		length = strlen (pvalue) + 1;
		(*pppvalues) [n] = new WCHAR [length];
		MultiByteToWideChar (CP_ACP, 0, pvalue, -1, (*pppvalues) [n], length);

		n++;

		free (ppaircopy);

		ppair = strtok (NULL, "&;");
	}
	free (pcopy);
}

/******************************************************************************/
void CHttpServer::RemoveEncoding (char *psource, char *pdest)
{
	char hex [3];
	int state;

	state = 0;

	while (*psource)
	{
		switch (state)
		{
		case 0:			// Normal
			if (*psource == '+')
				*pdest++ = ' ';
			else if (*psource == '%')
				state = 1;
			else
				*pdest++ = *psource;
			break;

		case 1:			// First hex digit
			hex [0] = *psource;
			state = 2;
			break;

		case 2:			// Second hex digit
			hex [1] = *psource;
			hex [2] = 0;
			*pdest++ = (char) strtol (hex, NULL, 16);
			state = 0;
			break;
		}

		psource++;
	}

	*pdest = 0;
}

/******************************************************************************/
void CHttpServer::ParseMethod (char *pline)
{
	char *pc;

	strcpy (sMethod, "");
	strcpy (sURI, "");

	// Get METHOD, check for valid
	pc = strtok (pline, " ");
	if (!pc || *pc == 0 || strlen (pc) > sizeof (sMethod) - 1)
		return;
	strcpy (sMethod, pc);

	// Get URI, check for valid
	pc = strtok (NULL, " ");
	if (!pc || *pc == 0 || strlen (pc) > sizeof (sURI) - 1)
		return;
	strcpy (sURI, pc);
}

/******************************************************************************/
int CHttpServer::WaitForByte (void)
{	
	// Returns next byte from connection, blocking if necessary to wait for more.
	// Returns byte value as int, or -1 on error, e.g. connection closed

	// Buffer empty?
	if (nBufferIndex == nBufferLength)
	{
		//Abhineet - Resetting it to zero to avoid infinite loop during invocation of WaitForByte method & Access Violation
		nBufferIndex = 0;
		// Yes, block waiting for more data
		nBufferLength = recv (hAcceptSocket, (char*) pByteBuffer, MAX_BUFFER, 0);
		if (nBufferLength == 0 || nBufferLength == SOCKET_ERROR)
		{
			//Abhineet - Resetting it to zero to avoid infinite loop during invocation of WaitForByte method & Access Violation
			nBufferLength = 0 ;
			return -1;
		}
	}

	// Return next byte from buffer
	return (int) (pByteBuffer [nBufferIndex++]);
}

/******************************************************************************/
int CHttpServer::WaitForData (BYTE *pbuffer, int count)
{
	int n, b;

	for (n = 0; n < count; n++)
	{
		if ((b = WaitForByte ()) == -1)
			return -1;

		*pbuffer++ = (BYTE) b;
	}

	return 0;
}

/******************************************************************************/
int CHttpServer::WaitForLine (char *pline, int max_length)
{
	// Block until line terminated by LF received, CR is ignored.
	// max_length is length of pline buffer, data will be truncated if necessary so line is always null terminated
	int n, index = 0;
	char c;

	while (1)
	{
		// Get next char
		if ((n = WaitForByte ()) == -1)
			return -1;

		c = (char) n;

		// Process char
		switch (c)
		{
		case '\x0D':
			// Ignore CR
			break;

		case '\x0A':
			// LF means end of line
			pline [index] = 0;
			return 0;

		default:
			// Add char to line, allowing space for null
			if (index < max_length - 1)
				pline [index++] = c;
			else{
				//Abhineet - Coming out of the loop if the byte recieved is never end of line and max length reached.
				pline [index] = 0;
				return 0;
			}
			break;
		}
	}
}
