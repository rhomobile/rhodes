// Inet.cpp: implementation of the CInet class.
//
//////////////////////////////////////////////////////////////////////

#include "Inet.h"
#include "Http.h"
#include "Ftp.h"
#include "URLDecomposition.h"
#include "../../Common/Private/RelativeURLs.h"

/**
* \author	Paul Henderson (PH, PTJK76)
* \author	Darryn Campbell (DCC, JRQ768)
* \date		December 2009 - Ported from SPB2.22, PH, DC
*/
CInet::CInet(int iInstanceID, PBModule* pModule)
{
	m_pModule			= pModule;
	m_iInstanceID		= iInstanceID;
	m_szTransferEvent	= NULL;
	m_lpSzDestination	= NULL;
	m_lpSzSource		= NULL;
	m_SzUserName		= NULL;
	m_SzPassWord		= NULL;
	m_hThread			= NULL;
	m_bCreateDir		= FALSE;
	m_bOverWriteDest	= FALSE;
	m_bFileDest			= FALSE;
	m_iProtocol			= PROTOCOL_FILE;
	m_szCurrentURL		= NULL;
	m_bDestTypeSet		= FALSE;

	//  Backwards compatibility with legacy bug in Browser where the
	//  software was remembering the last transfer source (FTP / HTTP / FILE)
	//  and wasn't being reset properly.  Users may have taken advantage
	//  of this to not set up each transfer properly
	m_bRememberSourceProtocol = false;
	LPCTSTR pSettingValue = 
		m_pModule->GetAppSettingPtr(m_iInstanceID, L"FileTransfer\\RememberSourceProtocol", L"Value");
	if (pSettingValue)
	{
		if (pSettingValue && pSettingValue[0] != L'0')
			m_bRememberSourceProtocol = true;
		else
			m_bRememberSourceProtocol = false;
	}
	

}

/**
* \author	Paul Henderson (PH, PTJK76)
* \author	Darryn Campbell (DCC, JRQ768)
* \date		December 2009 - Ported from SPB2.22, PH, DC
*/
CInet::~CInet()
{
	delete [] m_szTransferEvent;
	delete [] m_lpSzDestination;
	delete [] m_lpSzSource;
	delete [] m_SzPassWord;
	if(m_hThread != NULL){
		if(WaitForSingleObject(m_hThread,3000) == WAIT_TIMEOUT){
			TerminateThread (m_hThread, 0);
		}
		CloseHandle(m_hThread);
		m_hThread = NULL;
	}
	
 }

/**
* \author	Paul Henderson (PH, PTJK76)
* \author	Darryn Campbell (DCC, JRQ768)
* \date		December 2009 - Ported from SPB2.22, PH, DC
*/
BOOL CInet::SetUserName(LPCTSTR szUserName)
{
	delete [] m_SzUserName;
	m_SzUserName = new TCHAR[wcslen(szUserName) + 1];
	if(m_SzUserName == NULL){
		m_pModule->Log(PB_LOG_ERROR, L"MEMORY ERROR", _T(__FUNCTION__), __LINE__);
		return FALSE;
	}
	wcscpy(m_SzUserName,szUserName);
	return TRUE;
}


/**
* \author	Paul Henderson (PH, PTJK76)
* \author	Darryn Campbell (DCC, JRQ768)
* \date		December 2009 - Ported from SPB2.22, PH, DC
*/
BOOL CInet::SetPassword(LPCTSTR szPassWord)
{
	delete [] m_SzPassWord;
	m_SzPassWord = new TCHAR[wcslen(szPassWord) + 1];
	if(m_SzPassWord == NULL){
		m_pModule->Log(PB_LOG_ERROR, L"MEMORY ERROR", _T(__FUNCTION__), __LINE__);
		return FALSE;
	}
	wcscpy(m_SzPassWord,szPassWord);
	return TRUE;
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		December 2009 - Initial Creation, DCC
*/
BOOL CInet::SetProtocol(Protocol_T iProtocol)
{
	m_iProtocol = iProtocol;

	//  Set default port (SetPort is always called after SetProtocol in 
	//  Browser)
	if (iProtocol == PROTOCOL_HTTP)
		SetPort(INTERNET_DEFAULT_HTTP_PORT);
	else if (iProtocol == PROTOCOL_HTTPS)	// GD Added support for HTTPS
		SetPort(INTERNET_DEFAULT_HTTPS_PORT);
	else if(iProtocol == PROTOCOL_FTP)
		SetPort(INTERNET_DEFAULT_FTP_PORT);

	return TRUE;
}

/**
* \author	Paul Henderson (PH, PTJK76)
* \author	Darryn Campbell (DCC, JRQ768)
* \date		Feb 2010 - Initial Creation, PH
* \date		February 2010 - Adding file protocol if address starts '\'
*/
BOOL CInet::SetCurrentURL(LPCTSTR pURL)
{
	if(pURL)
	{
		delete [] m_szCurrentURL;
		bool bNeedsFileProtocol = false;
		if (pURL[0] == '\\')
			bNeedsFileProtocol = true;
		int iLengthOfCurrentURL = _tcslen(pURL) + 1;
		if (bNeedsFileProtocol)
			iLengthOfCurrentURL += wcslen(L"file://");
		m_szCurrentURL = new TCHAR[iLengthOfCurrentURL];

		if(m_szCurrentURL)
		{
			if (bNeedsFileProtocol)
			{
				wsprintf(m_szCurrentURL, L"file://%s", pURL);
			}
			else
				_tcscpy(m_szCurrentURL,pURL);
			return TRUE;
		}
	}
	delete []m_szCurrentURL;
	m_szCurrentURL = NULL;
	return FALSE;
}




/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		December 2009 - Initial Creation, DCC
*/
BOOL CInet::SetPort(WORD nPort)
{
	m_nPort = nPort;
	return TRUE;
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		December 2009 - Initial Creation, DCC
*/
BOOL CInet::SetCreateDirectories(BOOL bCreateDir)
{
	m_bCreateDir = bCreateDir;
	return TRUE;
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		December 2009 - Initial Creation, DCC
*/
BOOL CInet::SetOverwriteDestination(BOOL bOverwriteDestination)
{
	m_bOverWriteDest = bOverwriteDestination;
	return TRUE;
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		December 2009 - Initial Creation, DCC
*/
BOOL CInet::SetFileDestination(BOOL bDestinationIsFileSystem)
{
	//  This method will always be called last (translated from Send / Receive)
	//  Therefore if the user has set the File:// protocol we know the 
	//  destination is the local filesystem regardless.
	m_bDestTypeSet = true;
	if (m_iProtocol == PROTOCOL_FILE)
		m_bFileDest = TRUE;
	else
		m_bFileDest = bDestinationIsFileSystem;
	return TRUE;
}

/**
* \author	Harish Pulimi (PXB743)
* \date		November 2012 - Initial Creation, HP
*/
void CInet::urlDecode( TCHAR *tcDecodedDestination, LPCTSTR tcEncodedDestination) 
{	
	char code[3] = {0}; //Encoded character
	int ascii = 0; //Decoded ASCII value
	char *end = NULL; //Escape character

	unsigned int decodeIndex = 0;
	unsigned int strLength = wcslen(tcEncodedDestination);

	for (unsigned int encodeIndex=0; encodeIndex<strLength; encodeIndex++)
	{    
        if (tcEncodedDestination[encodeIndex] == '%') 
		{
			code[0] = tcEncodedDestination[encodeIndex+1];
			code[1] = tcEncodedDestination[encodeIndex+2];											
            tcDecodedDestination[decodeIndex++] = strtoul(code, &end, 16);
            encodeIndex += 2;
        } 
		else 
		{			
			tcDecodedDestination[decodeIndex++] = tcEncodedDestination[encodeIndex];			
        }
    }    
}

/**
* \author	Paul Henderson (PH, PTJK76)
* \author	Darryn Campbell (DCC, JRQ768)
* \author	Geoff Day (GD, XFH386)
* \date		December 2009 - Initial Creation, PH
* \date		December 2009 - Updated to extract the username / password etc 
*                           from the URL.
* \date		February 2014 - Added support for HTTPS
*/
BOOL CInet::SetDestination(LPCTSTR szDest)
{
	if(*szDest == 0){
		return FALSE;
	}
	
	//  szDest may be specified as a relative or an absolute URL, convert 
	//  relative URLs to absolute URLs.
	//  Dereferenced destination is the URL to use as the destination (accounting for 
	//  if a relative path was specified)
	TCHAR tcDereferencedDestination[MAX_URL];
	memset(tcDereferencedDestination, 0, MAX_URL * sizeof(TCHAR));
	//Decode the URL
	urlDecode(tcDereferencedDestination, szDest);

	if (IsRelativeURL(szDest))
	{
		if (!DereferenceURL(szDest, tcDereferencedDestination, m_szCurrentURL, m_pModule))
			return FALSE;
	}

	delete [] m_lpSzDestination;
	m_lpSzDestination = NULL;
	//  Stripped destination is a concatenation of the server address and the 
	//  filename / file path.  It is stripped of the username / password /
	//  protocol & port.
	TCHAR tcStrippedDestination[MAX_URL];

	//  Check for File Protocol
	TCHAR retStr[MAX_URL];
	memset(retStr, 0, MAX_URL * sizeof(TCHAR));
	if (GetProtocolFromURL(tcDereferencedDestination, retStr) > 0 &&
		_wcsicmp(retStr, L"file") == 0)
	{
		//  The protocol is explicitly stated as File
		if (m_iInstanceID >= PB_APP_ARRSIZE)
		{
			SetProtocol(PROTOCOL_FILE);
		}
		m_bFileDest = TRUE;
		//  Advance past File://(backslash)
		wcscpy(tcStrippedDestination, tcDereferencedDestination + 8);
	}
	else if (GetProtocolFromURL(tcDereferencedDestination, retStr) > 0 &&
		_wcsicmp(retStr, L"http") == 0)
	{
		//  The protocol is explicitly stated as HTTP
		SetProtocol(PROTOCOL_HTTP);
		m_bFileDest = FALSE;
		if (!ParseURLAttributes(tcDereferencedDestination, tcStrippedDestination))
			return FALSE;
	}
	// GD
	else if (GetProtocolFromURL(tcDereferencedDestination, retStr) > 0 &&
		_wcsicmp(retStr, L"https") == 0)
	{
		//  The protocol is explicitly stated as HTTPS
		SetProtocol(PROTOCOL_HTTPS);
		m_bFileDest = FALSE;
		if (!ParseURLAttributes(tcDereferencedDestination, tcStrippedDestination))
			return FALSE;
	}
	else if (GetProtocolFromURL(tcDereferencedDestination, retStr) > 0 &&
		_wcsicmp(retStr, L"ftp") == 0)
	{
		//  The protocol is explicitly stated as FTP
		SetProtocol(PROTOCOL_FTP);
		m_bFileDest = FALSE;
		if (!ParseURLAttributes(tcDereferencedDestination, tcStrippedDestination))
			return FALSE;
	}
	else if (GetProtocolFromURL(tcDereferencedDestination, retStr) <= 0)
	{
		//  The URL does not specify a protocol
		//  tcStrippedDestination should be set to the destination string
		wcscpy(tcStrippedDestination, tcDereferencedDestination);
		if (!m_bDestTypeSet)
			m_bFileDest = TRUE;
	}

	//  Populate the Destination member variable.	
	m_lpSzDestination = new TCHAR[wcslen(tcStrippedDestination) + 1];
	if(m_lpSzDestination)
	{
		wcscpy(m_lpSzDestination, tcStrippedDestination);
		return TRUE;
	}
	else
	{
		//  Memory Error	
		delete[]m_lpSzDestination;
		m_lpSzDestination = NULL;
		return FALSE;
	}
}


/**
* \author	Paul Henderson (PH, PTJK76)
* \author	Darryn Campbell (DCC, JRQ768)
* \date		December 2009 - Initial Creation, PH
* \date		December 2009 - Updated to extract the username / password etc 
*                           from the URL.
*/
BOOL CInet::SetSource(LPCTSTR szSource)
{
	if(!szSource || *szSource == 0){
		return FALSE;
	}

	//  szSource may be specified as a relative or an absolute URL, convert 
	//  relative URLs to absolute URLs.
	//  Dereferenced source is the URL to use as the source (accounting for 
	//  if a relative path was specified)
	TCHAR tcDereferencedSource[MAX_URL];
	memset(tcDereferencedSource, 0, MAX_URL * sizeof(TCHAR));
	if (IsRelativeURL(szSource))
	{
		if (!DereferenceURL(szSource, tcDereferencedSource, m_szCurrentURL, m_pModule))
			return FALSE;
	}
	else
		wcscpy(tcDereferencedSource, szSource);


	delete [] m_lpSzSource;
	m_lpSzSource = NULL;
	//  Stipped source is a concatenation of the server address and the 
	//  file name / file path.  It is stripped of username / password / 
	//  protocol & port
	TCHAR tcStrippedSource[MAX_URL];

	//  Check for File Protocol
	TCHAR retStr[MAX_URL];
	memset(retStr, 0, MAX_URL * sizeof(TCHAR));
	if (GetProtocolFromURL(tcDereferencedSource, retStr) > 0 &&
		_wcsicmp(retStr, L"file") == 0)
	{
		//  The protocol is explicitly stated as File
		//  Advance past File://(backslash)
		wcscpy(tcStrippedSource, tcDereferencedSource + 8);
	}
	else if (GetProtocolFromURL(tcDereferencedSource, retStr) > 0 &&
		_wcsicmp(retStr, L"http") == 0)
	{
		//  The protocol is explicitly stated as HTTP
		SetProtocol(PROTOCOL_HTTP);
		m_bFileDest = TRUE;
		if (!ParseURLAttributes(tcDereferencedSource, tcStrippedSource))
			return FALSE;
	}
	// GD - Support HTTPS
	else if (GetProtocolFromURL(tcDereferencedSource, retStr) > 0 &&
		_wcsicmp(retStr, L"https") == 0)
	{
		//  The protocol is explicitly stated as HTTPS
		SetProtocol(PROTOCOL_HTTPS);
		m_bFileDest = TRUE;
		if (!ParseURLAttributes(tcDereferencedSource, tcStrippedSource))
			return FALSE;
	}
	else if (GetProtocolFromURL(tcDereferencedSource, retStr) > 0 &&
		_wcsicmp(retStr, L"ftp") == 0)
	{
		//  The protocol is explicitly stated as FTP
		SetProtocol(PROTOCOL_FTP);
		m_bFileDest = TRUE;
		if (!ParseURLAttributes(tcDereferencedSource, tcStrippedSource))
			return FALSE;
	}
	else if (GetProtocolFromURL(tcDereferencedSource, retStr) <= 0)
	{
		//  The URL does not specify a protocol
		//  tcStrippedSource should be set to the source string
		wcscpy(tcStrippedSource, tcDereferencedSource);
	}

	//  Populate the source member variable
	m_lpSzSource = new TCHAR[wcslen(tcStrippedSource) + 1];
	if(m_lpSzSource)
	{
		wcscpy(m_lpSzSource, tcStrippedSource);
		return TRUE;
	}
	else
	{
		//  Memory Error
		delete[]m_lpSzSource;
		m_lpSzSource = NULL;
		return FALSE;
	}
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		December 2009 - Initial Creation, DC
*/
BOOL CInet::ParseURLAttributes(LPCTSTR tcURL, LPTSTR tcServerAndPath)
{
	TCHAR retStr[MAX_URL];
	memset(retStr, 0, MAX_URL * sizeof(TCHAR));
	//  Obtain the Username from the URL if specified
	if (GetUsernameFromURL(tcURL, retStr) > 0)
	{
		SetUserName(retStr);
	}
	//  Obtain the Password from the URL if specified
	if (GetPasswordFromURL(tcURL, retStr) > 0)
	{
		SetPassword(retStr);
	}
	//  Obtain the Port from the URL if specified
	if (GetPortFromURL(tcURL, retStr) > 0)
	{
		SetPort((WORD) _wtoi(retStr));
	}
	if (GetServerFromURL(tcURL, retStr) <= 0)
	{
		//  Error parsing URL, quit
		m_pModule->Log(PB_LOG_WARNING, L"Unable to Parse URL",
						_T(__FUNCTION__), __LINE__);
		return FALSE;
	}
	wcscpy(tcServerAndPath, retStr);
	if (GetFileAndPathFromURL(tcURL, retStr) <= 0)
	{
		//  Error parsing URL
		m_pModule->Log(PB_LOG_WARNING, L"Unable to Parse URL",
						_T(__FUNCTION__), __LINE__);
		return FALSE;
	}
	wcscat(tcServerAndPath, L"/");
	wcscat(tcServerAndPath, retStr);
	return TRUE;
}


/**
* \author	Paul Henderson (PH, PTJK76)
* \author	Darryn Campbell (DCC, JRQ768)
* \date		December 2009 - Ported from SPB2.22, PH, DC
*/
BOOL CInet::SetTransferEventString(LPCTSTR szNavStr)
{
	Lock lock(&m_CritSect);
	
	int len = wcslen(szNavStr);
	
	delete [] m_szTransferEvent;
	m_szTransferEvent = new TCHAR[len + 1];
	if(m_szTransferEvent)
	{
		if(wcscpy(m_szTransferEvent,szNavStr))
			return TRUE;
	}
	delete [] m_szTransferEvent;
	return FALSE;
	
}

LPTSTR CInet::GetTransferEventString()
{
	Lock lock(&m_CritSect);
	return m_szTransferEvent;
}

/**
* \author	Paul Henderson (PH, PTJK76)
* \author	Darryn Campbell (DCC, JRQ768)
* \date		December 2009 - Ported from SPB2.22, PH, DC
*/
BOOL CInet::Transfer()
{
	
	WCHAR szErr[20];
	
	
	if(m_lpSzDestination == NULL){
		wsprintf(szErr,L"ERROR: %d",ERROR_INVALID_NAME);
		m_pModule->SendPBNavigate(tcTransferEventNames, 
		m_iInstanceID, 
		m_szTransferEvent, 
		szErr, NULL);
		return FALSE;
	}
	if(m_lpSzSource == NULL){
		wsprintf(szErr,L"ERROR: %d",ERROR_INVALID_NAME);
		m_pModule->SendPBNavigate(tcTransferEventNames, 
		m_iInstanceID, 
		m_szTransferEvent, 
		szErr, NULL);
		return FALSE;
	}
	
	
	if(m_hThread){
		if(WaitForSingleObject(m_hThread,0) == WAIT_TIMEOUT)	//ensure no other threads are running
			return FALSE;
		CloseHandle(m_hThread);//precaution in case the thread was terminated earlier and did not close the handle
	}
	
	m_hThread = CreateThread (NULL, 0, 
		(unsigned long (__stdcall *)(void *))TransferThread,(void *)this, NULL, NULL);
		
	if(m_hThread == NULL)
		return FALSE;

	
	return TRUE;
}

/**
* \author	Paul Henderson (PH, PTJK76)
* \author	Darryn Campbell (DCC, JRQ768)
* \date		December 2009 - Ported from SPB2.22, PH, DC
*/
DWORD CInet::TransferThread(LPVOID lParam)
{
	CInet*	pInet = (CInet*)lParam;
	CFtp	*pFtp = NULL;
	CHttp	*pHttp = NULL;
	TCHAR	szResponse[MAX_URL + 1];
	VARSTRUCT  structVar;
	DWORD dRet;
	
	BOOL bFailIfExists = TRUE;
	HANDLE hFind;

	//  Resolving SRs during T&V of 1.0.0.3.  A number of T&V tests were
	//  one type of file transfer followed by another, this highlighted that
	//  the default protocol was not being reset to File after each transfer.
	//  Note: the logic for backwards compatibility (with SPB2 syntax) relies 
	//  on the protocol defaulting to file.
	//  Note: FileTransfers created via IMO (PB_APP_ARRSIZE) will never
	//        default back to File.
	int iCurrentTransferProtocol = pInet->m_iProtocol;
	if (!pInet->m_bRememberSourceProtocol && 
		pInet->m_iInstanceID < PB_APP_ARRSIZE)
	{
		pInet->m_iProtocol = PROTOCOL_FILE;
	}

	if(pInet->m_bCreateDir)//if the user has set createFolders to true
	{
		if(pInet->m_bFileDest)//if the destination is the file system
		{
			dRet = pInet->CreatePath(pInet->m_lpSzDestination);
			if(dRet)
			{
				wsprintf(szResponse,L"Error Code: %d",dRet);
				return 0;
			}
		}
	}
	
	switch(iCurrentTransferProtocol)
	{
		case PROTOCOL_HTTP:
		case PROTOCOL_HTTPS:	// GD - Added HTTPS; WinInet flags will be set during CHttp::PutFile()
			pHttp = new CHttp(*pInet);
			if(pHttp == NULL)
			{
				//error
				return 1;
			}	
			if(!pInet->m_bFileDest)
			{
				if(pHttp->PutFile(iCurrentTransferProtocol))
					wcscpy(szResponse,pHttp->GetResponseStr());
				else
					wcscpy(szResponse,L"ERROR:");
			}
			else
			{
				//must be receive
				if(pInet->m_bCreateDir)
				{
					dRet = pInet->CreatePath(pInet->m_lpSzDestination);
					if(dRet)
					{
						wsprintf(szResponse,L"Error Code: %d",dRet);
						break;
					}
				}
				WIN32_FIND_DATA lpFileData;
				hFind = FindFirstFile(pInet->m_lpSzDestination, &lpFileData);
				if (hFind!= (HANDLE)INVALID_HANDLE_VALUE && 
					!pInet->m_bOverWriteDest)
				{
					wcscpy(szResponse, L"Error: File Exists");
					break;
				}
				else
				{
					dRet = pHttp->GetFile(iCurrentTransferProtocol);
					if(dRet)
					{
						wcscpy(szResponse,L"OK: File Received");
						break;
					}
				}
				wsprintf(szResponse,L"ERROR: %d",GetLastError());

			}
		break;

		case PROTOCOL_FTP:
			pFtp = new CFtp(*pInet);
			if(pFtp == NULL)
			{
				//error
				return 1;
			}
			if(!pInet->m_bFileDest)
			{
				dRet = pFtp->PutFile();
				if(dRet == 0)
				{
					wcscpy(szResponse,L"OK: File Sent");
					break;
				}
				wsprintf(szResponse,L"Error Code: %d",dRet);
			}
			else{
				//must be receive
				if(pInet->m_bCreateDir)
				{
					dRet = pInet->CreatePath(pInet->m_lpSzDestination);
					if(dRet)
					{
						wsprintf(szResponse,L"Error Code: %d",dRet);
						break;
					}
				}
				WIN32_FIND_DATA lpFileData;
				hFind = FindFirstFile(pInet->m_lpSzDestination, &lpFileData);
				
				if (hFind!= (HANDLE)INVALID_HANDLE_VALUE && 
					!pInet->m_bOverWriteDest)
				{
					wcscpy(szResponse, L"Error: File Exists");
				}
				else
				{
					dRet = pFtp->GetFile();
					if(dRet == 0)
					{
						wcscpy(szResponse,L"OK: File Received");
						break;
					}
					else
						wsprintf(szResponse,L"Error Code: %d",GetLastError());
				}
			}
		break;
		
		case PROTOCOL_FILE:
			//Since MoveFile does not overwrite an existing file,
			//delete the destination file id overwrite is specicified
			if(pInet->m_bOverWriteDest)
			{
				DeleteFile(pInet->m_lpSzDestination);
			}
			if(MoveFile(pInet->m_lpSzSource,pInet->m_lpSzDestination) == TRUE)
			{
				//if there are no errors
				wcscpy(szResponse,L"OK: File Moved");
				break;
			}
						
			wsprintf(szResponse,L"Error Code: %d",GetLastError());
		break;

		default:
		//error log no protocol
		CloseHandle(pInet->m_hThread);
		pInet->m_hThread = NULL;
		return 2;

	}
		
	//send a response
	if(pInet->m_szTransferEvent && *pInet->m_szTransferEvent){
		memset(&structVar,0,sizeof(VARSTRUCT));
		structVar.pStr = szResponse;
		pInet->m_pModule->SendPBNavigate(tcTransferEventNames, 
										pInet->m_iInstanceID, 
										pInet->GetTransferEventString(),//pInet->m_szTransferEvent, 
										szResponse, NULL);
	}
	
	FindClose(hFind);
	delete pFtp;
	delete pHttp;
	CloseHandle(pInet->m_hThread);
	pInet->m_hThread = NULL;
	return 0;
}	

/**
* \author	Paul Henderson (PH, PTJK76)
* \date		December 2009 - Ported from SPB2.22, PH
*/
DWORD CInet::CreatePath(LPTSTR szFilePath)
{
		DWORD	dwErr = 1;
		
		//  Check '\' exists in the file path
		if (wcsstr(szFilePath, L"\\") == NULL)
			return 0;

		LPWSTR	lpSzDirStr = NULL;	
		LPWSTR  lpSzNewDir = NULL;
		LPTSTR	pStr,lpDir;
		int iLen = wcslen(szFilePath);
		
		short iCount,iMarker;
		
		
		lpSzDirStr = new WCHAR[iLen + 1];
		lpSzNewDir = new WCHAR[iLen + 1];
		
		if(lpSzDirStr && lpSzNewDir){
			pStr = szFilePath;
			//find the last '\' to seperate the filename from the directories
			for(iCount = 0;*pStr != NULL;pStr++)
			{
				if(*pStr == L'\\'){
					iMarker = iCount;
				}
				iCount++;
			}
			
			if(*szFilePath == L'\\'){						//EG: "\new\test\another"
				wcsncpy(lpSzDirStr,szFilePath,iMarker);
				lpSzDirStr[iMarker] = NULL;
			}
			else{											//EG: "new\test\another"
				
				*lpSzDirStr = L'\\';
				wcsncpy(lpSzDirStr + 1,szFilePath,iMarker);
				lpSzDirStr[iMarker + 1] = NULL;
			}
			
			pStr = lpSzDirStr;
			
			//Now we have the directories only in tempDir EG: "\new\test\another"
			
			
			lpDir = lpSzNewDir;
			//create each directory in turn
			while(*pStr == L'\\'){
				*lpDir++ = *pStr++;
				for(;*pStr != L'\\' && *pStr;pStr++){
					*lpDir++ = *pStr;
					//iCount++;//loop until end of dir name
				}
				*lpDir = NULL;
				
				if(!::CreateDirectory(lpSzNewDir,NULL)){
					dwErr = GetLastError();	
					if(dwErr != 183){//if the dir exists (183) then just ignore the error
						goto _exit;
						
					}
					dwErr = 0;
				}
				if(*pStr == NULL){
					dwErr = 0;
					break;//reached the last dir
				}
			}

		}

_exit:
		delete [] lpSzDirStr;
		delete [] lpSzNewDir;
		return dwErr;

}


/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		July 2010 -  Username, password etc. 
*						should not persist across pages.
*/
DWORD CInet::ClearNonPersistantVariables()
{
	delete [] m_SzUserName;
	m_SzUserName		= NULL;
	delete [] m_SzPassWord;
	m_SzPassWord		= NULL;
	delete [] m_lpSzDestination;
	m_lpSzDestination	= NULL;
	delete [] m_lpSzSource;
	m_lpSzSource		= NULL;
	m_bCreateDir		= FALSE;
	m_bOverWriteDest	= FALSE;

	return 0;
}
