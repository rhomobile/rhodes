/**
*  \file Inet.h
*  \brief File to house CInet class and related includes / defines.
*/

#pragma once

#include <windows.h>
#include <Wininet.h>
#include "../../common/Public/PBPlugin.h"
#include "..\..\PBCore/PBCore/CriticalSect.h"

static TCHAR* tcTransferEventNames[] = 
{
	L"transferResult",
	NULL
};


/**
*  Enumeration of the different protocols handled by FileTransfer
*/
enum Protocol_T
{
	PROTOCOL_FTP,	///<  Specified Transfer is via FTP
	PROTOCOL_HTTP,	///<  Specified Transfer is via HTTP
	PROTOCOL_FILE,	///<  Specified Transfer is moving a FILE.
	PROTOCOL_HTTPS,	///<  Specified Transfer is via HTTPS
};

/**
*  CInet class wraps the functionality of the File Transfer module.  
*  Transfers the specified file from the source to the destination using 
*  either FTP, HTTP or a FILE move.  Compatible with FTP / HTTP which 
*  requires a username / password combination.  The success or otherwise of 
*  the transfer is returned to the user via a TransferEvent.
*/
class CInet  
{
	//  Allow FTP and HTTP objects to access CInet private variables
	friend  class CFtp;
	friend  class CHttp;

public:
	
	/**
	*  Initialise variables.
	*  \param iInstanceID	Unique identifier of the Browser instance in which 
	*						this File Transfer module is running.
	*  \param pModule		Pointer to the Browser CFileTransfer class
	*						which created this CInet object.  Used for logging and 
	*						sending navigations back to the core.
	*/
	CInet			(int iInstanceID, PBModule* pModule);

	/**
	*  Free memory and stop threads associated with this object.
	*/
	virtual ~CInet();

	/**
	*  Start a separate thread to transfer the file from the source to the 
	*  destination.
	*  \return Whether or not the thread was successfully created.
	*/
	BOOL			Transfer		();

	/**
	*  Separate the Username, password and port from the given URL and set 
	*  the appropriate member variables as appropriate.  This function should 
	*  only be called with a HTTP or FTP url, not a file URL.
	*  \param tcURL URL to be parsed.
	*  \param tcServerAndPath [out] Concatenation of the server and file path / 
	*                               filename.  This is the same format processed
	*                               by PB2.x
	*  \return Whether or not the URL was successfully parsed.
	*/
	BOOL			ParseURLAttributes(LPCTSTR tcURL, LPTSTR tcServerAndPath);
	
	/**
	*  Accessor method for m_SzUserName, the username used for FTP / HTTP 
	*  transfers where specified.
	*/
	BOOL			SetUserName		(LPCTSTR szUserName);

	/**
	*  Accessor method for m_SzPassWord, the password used for FTP / HTTP 
	*  transfers where specified.
	*/
	BOOL			SetPassword		(LPCTSTR szPassWord);

	/**
	*  Set the destination where the file should be transferred to.  
	*  The destination will be parsed for protocol, username, password and port
	*  number and these attributes will be set as appropriate.
	*  \param szDest Destination URL, can optionally contain protocol and other 
	*  parameters, for FTP: [ftp://][username][:password][@]server[:port]/path/filename.ext 
	*  Leave path blank to place the file in the root directory but you must 
	*  specify a filename.  For HTTP: [http://]server/path/filename.ext, again 
	*  leaving path blank to access the root.  For File: [file://\\]directory\\filename.ext
	*  leaving directory blank to place in the root.
	*  \return Whether or not the Destination was successfully parsed.
	*/
	BOOL			SetDestination	(LPCTSTR szDest);

	/**
	*  Decode the encoded URL.  
	*  If the URL is not encoded then it simply copies the string
	*  \param tcDecodedDestination - Decoded URL
	*  \param szEncodedDestination - Encoded URL
	*/
	void			urlDecode( TCHAR *tcDecodedDestination, LPCTSTR szEncodedDestination);

	/**
	*  Set the source where the file should be transferred from.
	*  The source will be parsed for protocol, username, password and port
	*  number and these attributes will be set as appropriate.  Note this is 
	*  similar to destination but the transfer must occur between the local 
	*  filestore and a remote machine, it can not occur between two remote 
	*  locations (e.g. FTP <--> HTTP).  Logic is used to determine which 
	*  direction the transfer should take place.
	*  \param szSource Source of the file, can optionally contain protocol and 
	*  other parameters, see SetDestination for example format.
	*  \return Whether or not the Source was successfully parsed.
	*/
	BOOL			SetSource		(LPCTSTR szSource);

	/**
	*  Sets the URI to be used when a Transfer Event occurs, this is the 
	*  callback sent to the user and must be set in order for the user to 
	*  receive notification that the transfer has completed.
	*  \param szNavStr The URI to be used when a transfer completes
	*  \return TRUE if the URI was successfully set.
	*/
	BOOL			SetTransferEventString		(LPCTSTR szNavStr);
	
	/**
	*  Gets the URI to be used when a Transfer Event occurs, this is the 
	*  callback sent to the user and must be set in order for the user to 
	*  receive notification that the transfer has completed.
	*  
	*  \return a pointer to the event URI.
	*/
	LPTSTR GetTransferEventString();




	/**
	*  For backwards compatibility Browser 2.x required the user
	*  to explicitly set the protocol using the http, ftp or file meta tag
	*  parameters.  This has been removed for 3.x (Protocol is set by 
	*  qualifying either the source or destination URL) but to maintain 
	*  backwards compabitility this method can explicitly set the Protocol.
	*  \param iProtocol The Protocol to use.
	*  \return TRUE
	*/
	BOOL			SetProtocol			(Protocol_T iProtocol);
	
	/**
	*  Populates the member variable m_szCurrentURL with the current URL of 
	*  the loaded page.
	*  \param pURL Current URL
	*  \return Whether or not m_szCurrentURL was set.
	*/
	BOOL			SetCurrentURL		(LPCTSTR pURL);
	
	/**
	*  For backwards compatibility Browser 2.x required the user to 
	*  explicitly specify whether they were performing a send or receive
	*  operation.  This has been removed for 3.x and replaced with 'Transfer'
	*  (Direction is worked out by which of the source or destination is a 
	*  remote location) but to maintain backwards compabitility this method
	*  needs to explicitly set whether or not the destination is the 
	*  local filesystem (else it is assumed to be the source).
	*  \param bDestinationIsFileSystem Whether or not the destination is a 
	*  remote location.
	*  \return TRUE;
	*/
	BOOL			SetFileDestination	(BOOL bDestinationIsFileSystem);
	
	/**
	*  Accessor method for m_nPort
	*  \param nPort The port to use for the remote Transfer
	*  \return TRUE
	*/
	BOOL			SetPort(WORD nPort);

	/**
	*  Accessor method for m_bCreateDir
	*  \param bCreateDir Whether or not directories should be created on the 
	*  local filesystem to accomplish the transfer.
	*  \return TRUE;
	*/
	BOOL			SetCreateDirectories(BOOL bCreateDir);

	/**
	*  Accessor method for m_bOverWriteDest.
	*  \param bOverwriteDestination Whether or not the destination file can be overwritten (local filesystem only)
	*  \return TRUE
	*/
	BOOL			SetOverwriteDestination(BOOL bOverwriteDestination);

	/**
	*  Created whilst .  Clears Username, Password
	*  Source and Destination and frees the associated memory.  Called during
	*  a page navigate.
	*/
	DWORD ClearNonPersistantVariables();
private:

	/**
	*  Creates a path locally on the device.  Used when transfering TO the 
	*  filesystem and m_bCreateDir is true.
	*  \param szFilePath Path to create.
	*  \return 0 on success, else the error code.
	*/
	DWORD			CreatePath		(LPTSTR szFilePath);

	/**
	*  Thread used to handle the transfer of the file in a separate thread.
	*  Responsible for creating a path to the local file (if appropriate) and 
	*  creating the HTTP or FTP object to handle the transfer.  Sends a 
	*  TransferEvent back to the core once the transfer is complete with the
	*  appropriate response indicating success or failure.
	*  \param lParam Pointer to the CInet object that created the thread.
	*  \return 0 on success, else non zero.
	*/
	static DWORD TransferThread(LPVOID lParam);

	volatile LPTSTR			m_szTransferEvent;	///<  Registered callback to use when Transfer completes.
	
	LPTSTR			m_szCurrentURL;
	LPTSTR			m_lpSzDestination;	///<  Destination URL or File:// location, where the file is being sent to.
	LPTSTR			m_lpSzSource;		///<  Source URL or File:// location, where the file is being sent from.
	LPTSTR          m_SzUserName;		///<  Username to access the specified HTTP or FTP site (optionally set)
	LPTSTR			m_SzPassWord;		///<  Password to access the specified HTTP or FTP site (optionally set)
	///  Whether or not the destination URL is a File:// location.  Crucial for determining the direction of the transfer as the protocol needs to be removed from the destination / source URLs
	BOOL			m_bFileDest;		
	WORD			m_nPort;			///<  The Port to be used for the remote connection.
	int				m_iProtocol;		///<  The Protocol to be used for the transfer.
	BOOL			m_bCreateDir;		///<  Whether or not directories should be created on the local filesystem to accommodate the transfer.
	BOOL			m_bOverWriteDest;	///<  Whether or not the file on the filesystem can be overwritten (if necessary)
	HANDLE			m_hThread;			///<  Handle of the thread transferring the file.
	PBModule*		m_pModule;			///<  Pointer to the CFileTransfer object owning this CInet instance.  Used for logging and navigation.
	int				m_iInstanceID;		///<  Unique identifier of the CFileTransfer object owning this CInet instance.  Used for navigation.
	bool			m_bDestTypeSet;		///<  Boolean of whether or not the destination type has been explicitely set by the interface
	bool			m_bRememberSourceProtocol; ///< Backwards compatibility with Browser where the protocol of the source wasn't being reset on each transfer
	CCriticalSect	m_CritSect;
};
