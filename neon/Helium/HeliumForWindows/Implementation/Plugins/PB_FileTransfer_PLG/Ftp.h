/**
*  \file Ftp.h
*  \brief File to house CFtp class and related includes / defines.
*/

#pragma once

#include "Inet.h"

/**
*  CFtp class wraps the functionality of the FTP interface enabling the user 
*  to transfer files to and from an FTP site.
*/
class CFtp
{
public:
	/**
	*  Initialise variables and WinInet
	*  \param rInet Pointer to parent CInet object that is using this CFtp
	*  object (1:1 relationship)
	*/
	CFtp								(const class CInet& rInet);

	/**
	*  Free memory associated with this FTP session
	*/
	virtual ~CFtp						();

	/**
	*  Retrieve a file from the FTP server.  This method will automatically 
	*  connect to the FTP server using the address, username, password and
	*  port specified as part of the associated CInet object.   
	*  The remote file to receive and where to store
	*  it locally on the device are also both stored as attributes of the 
	*  associated CInet object.
	*  \return 0 on success else the result of Windows GetLastError()
	*/
	DWORD			GetFile				();

	/**
	*  Send a file to the FTP server.  This method will automatically 
	*  connect to the FTP server using the address, username, password and
	*  port specified as part of the associated CInet object.   
	*  The local file to transfer and the destination
	*  address / filename on the FTP server are also both stored as attributes
	*  of the associated CInet object.
	*  \return 0 on success else the result of Windows GetLastError()
	*/
	DWORD			PutFile				();

private:
	
	/**
	*  Connect to the FTP server.
	*  \param lpSzAddr Address of the FTP server to connect to.  Anything after 
	*                  the first '/' is ignored so you can pass 
	*                  e.g. 157.235.138.214/temp/file.txt
	*  \return 0 on success else the result of Windows GetLastError()
	*/
	DWORD			Connect				(LPCTSTR lpSzAddr);

	HINTERNET		m_hConnection;	///<  The connection to the FTP Server
	HINTERNET		m_hInet;		///<  Handle to be used for all WinINet functions
	const CInet		*m_rCInet;		///<  CInet object which created this FTP Object and is requesting the transfer
};

