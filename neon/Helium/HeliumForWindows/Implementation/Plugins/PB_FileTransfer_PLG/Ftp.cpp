//  Implementation of the CFtp class.

#include "Http.h"
#include "Ftp.h"

/**
* \author	Paul Henderson (PH, PTJK76)
* \author	Darryn Campbell (DCC, JRQ768)
* \date		December 2009 - Ported from SPB2.22, PH, DC
*/
CFtp::CFtp(const class CInet& rInet)
{
	m_rCInet = &rInet;
	m_hConnection = NULL;
	m_hInet = InternetOpen(	APP_NAME,INTERNET_OPEN_TYPE_DIRECT ,NULL,NULL,0);
}

/**
* \author	Paul Henderson (PH, PTJK76)
* \author	Darryn Campbell (DCC, JRQ768)
* \date		December 2009 - Ported from SPB2.22, PH, DC
*/
CFtp::~CFtp()
{
	if(m_hConnection)
		InternetCloseHandle(m_hConnection);
	if(m_hInet)
		InternetCloseHandle(m_hInet);
}


/**
* \author	Paul Henderson (PH, PTJK76)
* \author	Darryn Campbell (DCC, JRQ768)
* \date		December 2009 - Ported from SPB2.22, PH, DC
*/
DWORD CFtp::GetFile()
{
	DWORD dErr = ERR;
		
	if(m_hConnection == NULL && m_rCInet->m_lpSzSource)
		dErr = Connect(m_rCInet->m_lpSzSource);
	
	if(dErr)
	{
		m_rCInet->m_pModule->Log(PB_LOG_ERROR, L"Couldn't Connect to the FTP Server", _T(__FUNCTION__), __LINE__); 
		return dErr; 
	}
	
	LPTSTR lpRemoteFile;
	for(lpRemoteFile = (LPTSTR)m_rCInet->m_lpSzSource;*lpRemoteFile && *lpRemoteFile != L'/';lpRemoteFile++);
		
	BOOL bOk = FtpGetFile(
			m_hConnection,	
			lpRemoteFile,						
			m_rCInet->m_lpSzDestination,				
			FALSE, 
			FILE_ATTRIBUTE_NORMAL, 
			FTP_TRANSFER_TYPE_BINARY | INTERNET_FLAG_RELOAD,
			INTERNET_FLAG_ASYNC 
			);
	
	dErr = 0;
	if(!bOk)
	{
		m_rCInet->m_pModule->Log(PB_LOG_ERROR, L"Couldn't retreive the file", _T(__FUNCTION__), __LINE__); 
		dErr =  GetLastError();
	}
	

	CloseHandle(m_hConnection);
	m_hConnection = NULL;
	return dErr;
}

/**
* \author	Paul Henderson (PH, PTJK76)
* \author	Darryn Campbell (DCC, JRQ768)
* \date		December 2009 - Ported from SPB2.22, PH, DC
*/
DWORD CFtp::PutFile()
{
	TCHAR errStr[100];
	DWORD dErr = 0;
	
	if(m_hConnection == NULL)
		dErr = Connect(m_rCInet->m_lpSzDestination);
	
	if(dErr)
	{
		m_rCInet->m_pModule->Log(PB_LOG_ERROR, L"Couldn't Connect to the FTP Server", _T(__FUNCTION__), __LINE__); 
		return dErr; 
	}
	
	LPTSTR lpRemoteFile;
	for(lpRemoteFile = (LPTSTR)m_rCInet->m_lpSzDestination;*lpRemoteFile != L'/';lpRemoteFile++);
	lpRemoteFile++;

	BOOL bOk = FtpPutFile(
			m_hConnection, 
			m_rCInet->m_lpSzSource, 
			lpRemoteFile,//m_rCInet->m_lpSzDestination, 
			FTP_TRANSFER_TYPE_BINARY,// | INTERNET_FLAG_RELOAD,
			0
			);
	
	if(!bOk)
	{	
		dErr = GetLastError();
		swprintf(errStr,L"Error Sending: %d ",dErr);
		m_rCInet->m_pModule->Log(PB_LOG_ERROR, errStr, _T(__FUNCTION__), __LINE__);		
	}
	
	return dErr;
}

/**
* \author	Paul Henderson (PH, PTJK76)
* \author	Darryn Campbell (DCC, JRQ768)
* \date		December 2009 - Ported from SPB2.22, PH, DC
*/
DWORD CFtp::Connect(LPCTSTR lpSzAddr)
{
	DWORD err = 0;
	LPTSTR lp;
	LPTSTR pSzFtpPath;
	
	pSzFtpPath = new TCHAR[wcslen(lpSzAddr)+ 1]; 
	if(pSzFtpPath == NULL){
		m_rCInet->m_pModule->Log(PB_LOG_ERROR, L"Failed Memory Allocation", _T(__FUNCTION__), __LINE__); 
		return 1;
	}
	for(lp = (LPTSTR)lpSzAddr;*lp != L'/';lp++);
		wcscpy(pSzFtpPath,lpSzAddr);
	pSzFtpPath [lp - lpSzAddr] = NULL;
	
	m_hConnection =  
		InternetConnect(
		m_hInet, 
		pSzFtpPath, 
		m_rCInet->m_nPort, 
		m_rCInet->m_SzUserName, 
		m_rCInet->m_SzPassWord, 
		INTERNET_SERVICE_FTP, 
		INTERNET_FLAG_PASSIVE, 
		0
		);

	delete [] pSzFtpPath;

	if(m_hConnection == NULL)
		return GetLastError();
	else
		return err;
}
