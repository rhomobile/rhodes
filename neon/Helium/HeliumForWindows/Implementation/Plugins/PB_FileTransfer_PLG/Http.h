/**
*  \file Http.h
*  \brief File to house CHttp class and related includes / defines.
*/

#pragma once

#include "Inet.h"

#define MAX_LOG_MESSAGE			100	///<  Maximum size of a log message.
#define CHUNKSIZE				500	///<  Max Size of each chunk to receive in an HTTP Get
#define INTERNET_MIN_URL_LENGTH 10	///<  Minimum length of a URL, used for URL validation.
#define MAX_FORM_NAME			40	///<  Maximum length of the form ID or Element name 		
//  TODO : This will be configurable in RhoElements 2.0
#define DEFAULT_ELEMENT_NAME	"SpbImagerFile"	///<  Default element name (used in creation of the request)
#define MIN_FILE_LEN			2
/**
*  Class to wrap the functionality of the HTTP interface enabling the user to 
*  download from and post files to an HTTP server.
*/
class CHttp
{
public:
	
	/**
	*  Initialises variables
	*  \param rCInet Pointer to parent CInet object that is using this CFtp
	*  object (1:1 relationship)
	*/
	CHttp				(const class CInet& rCInet);

	/**
	*  Frees memory associated with the HTTP connection
	*/
	virtual ~CHttp();

	/**
	*  Accessor method for m_SzResponse which is set to the HTTP response string
	*  following an HTTP Post.
	*  \return HTTP respose to an HTTP post.
	*/
	LPTSTR	GetResponseStr();

	/**
	*  Post the source file to the HTTP server.  
	*  This method will automatically connect to the HTTP server using the 
	*  address, username, password and port specified as part of the 
	*  associated CInet object.  The local file to transfer and the destination
	*  address of the HTTP server are also both stored as attributes of the 
	*  associated CInet object.  This function will retrieve the response string
	*  from the server and store it in m_SzResponse.
	*  \return 0 on success else the result of Windows GetLastError()
	*/
	BOOL	PutFile				(int protocol);

	/**
	*  Downloads a file from the HTTP server onto the local filesystem.
	*  This method will automatically connect to the HTTP server using the 
	*  address, username and password specified as part of the associated
	*  CInet object.  The remote file to be transferred and where to store the 
	*  file in the local filesotre are also both stored as attributes of the 
	*  associated CInet object.
	*  \return The size of the retrieved file.
	*/
	DWORD	GetFile				(int protocol); 

private:

	/**
	*  Accessor method for m_SzResponse which is set to the HTTP response string
	*  following an HTTP Post.
	*  \param szResponse string to set m_SzResponse to.
	*  \return TRUE if the string was successfully set.
	*/
	BOOL	SetResponseStr		(char* szResponse);

	/**
	*  Removes carriage return and line feed from the specified string.  Used
	*  to format the response received from the server following an HTTP post.
	*  \param pStr String which is to have the CR and LF stripped.
	*  \return TRUE if both characters were present and successfully stripped.
	*/
	BOOL	StripCRLF			(char* pStr);

	/**
	*  Removes the path and filename from the destination address and stores the 
	*  address of the server in m_lpszHost.
	*  \param Address of server optionally followed by a path and filename 
	*  (which is being removed)
	*  \return pointer to m_lpszHost.
	*/
	LPCTSTR CrackURL			(const LPCTSTR lpzURL);
	
	/**
	*  Generates the required text for the http transfer.
	*  \param boundary Data associated with HTTP Request
	*  \param requestBody [out] The populated requested body
	*  \param pFileBits Data associated with HTTP request
	*  \param sizeOfFile Data associated with HTTP request
	*  \param pszFileName Data associated with HTTP request
	*  \return The length of the request body
	*/
	int		GenerateRequestBody	(const char* boundary, char* requestBody, 
									void* pFileBits, int sizeOfFile,
									char* pszFileName);

	/**
	*  To send HTTP we need a unique boundary (normall with 27 -'s at 
	*  the start.  We then generate 3 x 4 hexdigits to follow the -'s
	*  \param boundaryPrefix [out] The populated boundary
	*/
	void	GenerateMultipartBoundary	(TCHAR* boundaryPrefix);

	/**
	*  For HTTP downloads this method opens the destination file on the local 
	*  filesystem so that it can be appended to.  If the file does not exist
	*  then it is created.
	*  \param lpSzFilePathName File name and path of the local file on the device.
	*  \return TRUE if the file was successfully opened, else FALSE.
	*/
	BOOL	OpenFile			(LPCTSTR lpSzFilePathName);

	/**
	*  Write to the file previously opened by OpenFile.  The file is on the 
	*  local filesystem.
	*  \param szBuffer Data to write to the file
	*  \param dwWrite Number of bytes to write
	*  \return Number of bytes actually written.
	*/
	DWORD	AppendToFile		(BYTE* szBuffer,int dwWrite);

	LPTSTR			m_lpszHost;		///<  The HTTP Host to connect to during an HTTP Post
	LPTSTR			m_lpszURLPath;	///<  For HTTP Post this is the file name and file path on the server being posted to
	char			m_SzElementID	[MAX_FORM_NAME + 1];	///<  Used in creation of HTTP Body
	HANDLE			m_hFile;		///<  The local file on the file system being populated during an HTTP download
	HINTERNET		m_hInet;		///<  Handle to the HTTP request (get or post)
	LPTSTR			m_SzResponse;	///<  HTTP response string following an HTTP Post
	const CInet		*m_rCInet;		///<  CInet object which created this HTTP Object and is requesting the transfer

};
