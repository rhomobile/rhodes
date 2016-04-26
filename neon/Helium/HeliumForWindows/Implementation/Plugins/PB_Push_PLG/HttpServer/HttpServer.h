/** 
* \file HttpServer.h
*/

/******************************************************************************/
#pragma once
#pragma comment (lib, "ws2.lib")

/******************************************************************************/
#include <windows.h>
#include <winsock2.h>

/******************************************************************************/
/** Macro giving count of elements in an array */
#define COUNTOF(x) (sizeof(x)/sizeof(x[0]))

/** Macro to copy a wide string, guaranteed not to overrun the destination and always to be null terminated */
#define WSAFECOPY(d,s)	StringCchCopy(d,COUNTOF(d),s)

/******************************************************************************/
/** \name Return values from server functions */
//@{
#define		HS_SUCCESS			0
#define		HS_ERROR			-1
//@}

/** Size to use when allocating working buffers */
#define		MAX_BUFFER		1024

class CHttpRequest;
class CHttpServer;

/******************************************************************************/
/**
* Class holding details of successful HTTP request
*/
/******************************************************************************/
class CHttpRequest
{
public:
	/** Constructor
	* \param count Number of name/value pairs
	*/
	CHttpRequest (int count);

	~CHttpRequest ();

	/** Gets count of name/value pairs
	* \return Count */
	int GetCount (void) { return nCount; }

	/** Gets read-only pointer to specified name string
	* \param n Zero-based index of string
	* \return Pointer to string
	*/
	LPCWSTR GetName (int n);

	/** Gets read-only pointer to specified value string
	* \param n Zero-based index of string
	* \return Pointer to string
	*/
	LPCWSTR GetValue (int n);

	/** Sets specified name string
	* \param n Zero-based index of string
	* \param pname Name string to set
	*/
	void SetName (int n, WCHAR *pname);

	/** Sets specified value string
	* \param n Zero-based index of string
	* \param pvalue Value string to set
	*/
	void SetValue (int n, WCHAR *pvalue);

	/** Sets specified name/value pair in one call
	* \param n Zero-based index of pair
	* \param pname Name string to set
	* \param pvalue Value string to set
	*/
	void SetNameValue (int n, WCHAR *pname, WCHAR *pvalue);

	/** Sets specified name/value pair in one call, converting to wide strings
	* \param n Zero-based index of pair
	* \param pname Name string to set
	* \param pvalue Value string to set
	*/
	void SetNameValue (int n, char *pname, char *pvalue);

private:
	WCHAR **ppNames;	///< Array of pointers to name strings
	WCHAR **ppValues;	///< Array of pointers to value strings
	int nCount;			///< Count of name/value pairs
};

/******************************************************************************/
/** Definition of callback function called when a request is received. */
typedef void (*PREQUESTPROC) (CHttpRequest*, CHttpServer*);	

/******************************************************************************/
/**
* Class to run an HTTP server
*/
/******************************************************************************/
class CHttpServer
{
public:
	/** Sets values to defaults and initializes Windows sockets stack if this is the first instance to use it
	* \param callback Pointer to function to be called when a successful request is received
	*/
	CHttpServer (PREQUESTPROC callback);

	/** Stops any running server and closes Windosw socket stack if this is the last instance using it
	*/
	~CHttpServer ();

	/** Starts server and begins listening for connections.
	* \return Error status - HS_SUCCESS if OK, HS_ERROR otherwise in which case GetLastError() can be called to get the error description
	*/
	int Start ();

	/** Stops server, disconnecting any connected clients.
	*/
	void Stop ();

	/** Sets virtual path which client must specifiy in HTTP request (default is blank).
	* \param ppath Virtual path
	*/
	void SetPath (LPCWSTR ppath);

	/** Sets name of HTML file which is sent to client in response to a successful request
	* (default is blank causing a simple built-in response to be sent).
	* \param pfilename Filename of response file
	*/
	void SetResponseFile (LPCWSTR pfilename);

	/** Sets passkey which the client must include in the request (default is blank meaning no passkey required).
	* \param pkey Passkey value
	*/
	void SetPasskey (LPCWSTR pkey);

	/** Sets TCP port on which to listen (default 80, the standard HTTP port).
	* CE devices often have a web server already running on port 80 so use this function to set a different value.
	* Set the port before calling Start().
	* \param nport TCP port
	*/
	void SetPort (int nport) {nPort = nport;}

	/** Gets the device's own IP address (the first address if the device has more than one).
	* \param paddress String buffer to copy the address into
	* \param length Maximum number of characters in buffer
	*/
	void GetLocalAddress (WCHAR *paddress, int length);

	/** Indicates if the server is started.
	* \return TRUE if server is started
	*/
	bool IsStarted () { return hServerSocket != INVALID_SOCKET; }

	/** Gets the most recent error description (generally set by Start() function).
	* \param perror String buffer to copy the error description into
	* \param length Maximum number of characters in buffer
	*/
	void GetLastError (LPWSTR perror, int length);

	void *pUserData;	///< Can be used to hold any generic data (not processed by the class itself)

private:
	static int nWSACount;	///< Static count to ensure Windows sockets stack is only started once

	PREQUESTPROC pRequestProc;	///< Pointer to callback for when a successful request is received
	SOCKET hServerSocket;		///< Socket to listen for incoming connections
	SOCKET hAcceptSocket;		///< Socket connected to incoming client
	HANDLE hServerThread;		///< Thread which waits for connection then processes HTTP request
	char sMethod [32];			///< Buffer to hold HTTP method (e.g. GET, POST)
	char sURI [MAX_BUFFER];		///< Buffer to hold virtual path plus parameters in request
	BYTE *pBodyData;			///< Buffer to hold request body data if any (for POST requests)
	int nBodyDataLength;		///< Length of body data, if any

	WCHAR wsError [64];			///< Most recent error description

	/**
	* Sets most recent error description.
	* \param pformat Format string as per printf
	* \param variable Variable number of parameters, corresponding to format paramter
	*/
	void SetLastError (LPCWSTR pformat, ...);

	char sPath [64];	///< Virtual path which client must specifiy in HTTP request
	WCHAR wsResponseFile [64];	///< Name of HTML file which is sent to client in response to a successful request
	WCHAR wsPasskey [64];	///< Passkey which the client must include in the request 
	int nPort;	///< TCP port on which to listen

	/** Thread procedure function. Calls non-static ProcessServer() to do all the work.
	* \param pparam Value passed by CreateThread(), used to pass 'this' pointer to static function
	* \return Always zero
	*/
	static DWORD ServerProc (LPVOID pparam);

	/** The main function which waits for an incoming connection and processes the HTTP request */
	void ProcessServer (void);

	/** Analyses the HTTP request once ProcessServer() has extracted the relevant data */
	int ProcessRequest (void);

	/** Sends the HTTP response. If there was an error processing the request then a 404 status is sent.
	* Otherwise a 200 status is sent followed by the contents of the wsResponseFile if set, or a simple built-in response if not.
	* \param error Error code from request processing, zero if no error (actual error code is not currently used)
	* \param isCors Whether the request had an Origin set
	*/
	void SendResponse (int error, bool isCors);

	/** Decodes HTML character escaping. Replaces + with space and \%xx with the character whose ASCII code is xx in hex.
	* \param psource HTML string to be decoded
	* \param pdest String buffer to receive decoded text, must be long enough but decoded string will always be shorter
	* or the same length as the source string, so it's allowable to pass the same buffer for source and destination
	*/
	void RemoveEncoding (char *psource, char *pdest);

	/** Extracts the HTTP method and URI.
	* The method should be GET or POST, and the URI is the part of the request after the server address.
	* \param pline Line as received in HTTP request
	*/
	void ParseMethod (char *pline);

	/** Extracts name/value pairs from the HTTP request, converting them to wide strings. String buffers are allocated
	* for each pair which must be freed by the caller.
	* \param pquery Query string from HTTP request
	* \param pcount Pointer to integer to receive the count of pairs found
	* \param pppnames The address of an array of wide character string pointers, will be allocated and filled with names extracted from the query
	* \param pppvalues The address of an array of wide character string pointers, will be allocated and filled with values extracted from the query
	*/
	void URLDecode (char *pquery, int *pcount, WCHAR ***pppnames, WCHAR ***pppvalues);

	/** Makes the callback when a successful HTTP request if received.
	* \param prequest CHttpRequest object holding request details, including name/value pairs
	*/
	void NotifyRequest (CHttpRequest* prequest);

	/** Fetches the next byte from the TCP connection. Data is buffered internally, and if the buffer is empty then this function
	* blocks waiting for more data to be available. All available data (up to the buffer size) is then fetched from the connection
	* into the buffer.
	* \return The next byte cast as an integer, or -1 if there was an error (e.g. the connection was closed)
	*/
	int WaitForByte (void);

	/** Fills a buffer with data from the TCP connection. Blocks if necessary until enough data is available.
	* \param pbuffer Buffer to receive data
	* \param count Number of bytes to wait for
	* \return -1 if there was an error (e.g. the connection was closed)
	*/
	int WaitForData (BYTE *pbuffer, int count);

	/** Gets a line of text from the TCP connection. Lines are terminated by LF, while CR characters are ignored.
	* \param pline Character buffer to receive line
	* \param max_length Length of buffer in characters - string will always be null terminated, truncating the data if necessary
	* \return -1 if there was an error (e.g. the connection was closed)
	*/
	int WaitForLine (char *pline, int max_length);

	/** \name Variables to handle internal buffering of TCP port data */
	//@{
	BYTE *pByteBuffer;	///< Pointer to internal buffer
	int nBufferIndex;	///< Read position in buffer
	int nBufferLength;	///< Size of data in buffer, set when reading TCP port
	//@}
};