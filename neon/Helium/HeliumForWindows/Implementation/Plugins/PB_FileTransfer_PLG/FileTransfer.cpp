#include "FileTransfer.h"

/**
* \author	Paul Henderson (PH, PTJK76)
* \date		November 2009 Initial Creation, PH
*/
BOOL CFileTransfer::onInit(PPBSTRUCT pPBStructure)
{
	wcscpy(m_szModName,L"FileTransfer");	
	if(RegisterForEvent(PB_BROWSER_BEFORE_NAV_EVENT)){
		return TRUE;
	}
	Log(PB_LOG_ERROR,L"Failed to register", _T(__FUNCTION__), __LINE__);
	return FALSE;
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		December 2009 Initial Creation, DCC
*/
void CFileTransfer::onDeInit(PPBSTRUCT pPBStructure)
{
	UnRegisterForEvent(PB_BROWSER_BEFORE_NAV_EVENT);
}

/**
* \author	Paul Henderson (PH, PTJK76)
* \date		November 2009 Initial Creation, PH
*/
BOOL CFileTransfer::onAttachInstance(PPBSTRUCT pPBStructure,PPBINSTSTRUCT pInstStruct)
{
	Log(PB_LOG_INFO,L"Creating Instance", _T(__FUNCTION__), __LINE__);
	pInstStruct->pWrappedObj = (CInet*) new CInet(pInstStruct->instID, this);

	if(pInstStruct->pWrappedObj)
		return TRUE;
	else
		return FALSE;
}

/**
* \author	Paul Henderson (PH, PTJK76)
* \date		November 2009 Initial Creation, PH
*/
BOOL CFileTransfer::onReleaseInstance(PPBSTRUCT pPBStructure,PPBINSTSTRUCT pInstStruct)
{
	Log(PB_LOG_INFO,L"Deleting Instance", _T(__FUNCTION__), __LINE__);
	delete (CInet*)pInstStruct->pWrappedObj;
	pInstStruct->pWrappedObj = NULL;
	return TRUE;
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		December 2009 Initial Creation, DCC
*/
BOOL CFileTransfer::onBeforeNavigate(int iInstID)
{
	//  If we are navigating to a new page reset the TransferEvent URI
	CInet *pInet = (CInet*)GetObjFromID(iInstID);
	
	if(pInet)
	{
		pInet->SetTransferEventString(L"");
		//    Set Username, Password, Source, Destination, 
		//  CreateFolders and OverWrite back to their default values
		pInet->ClearNonPersistantVariables();
	}
	return TRUE;
}


/**
* \author	Paul Henderson (PH, PTJK76)
* \date		November 2009 Initial Creation, PH
*/
BOOL CFileTransfer::MetaProc(PBMetaStruct *pbMetaStructure, PPBSTRUCT pPBStructure,VOID* pParam)
{
	CInet *pInet = (CInet*)pParam;
	LPTSTR pErr;
	if(cmp(pbMetaStructure->lpParameter,L"Protocol"))
	{
		if(cmp(pbMetaStructure->lpValue,L"File")) 
			pInet->SetProtocol(PROTOCOL_FILE);
		else if(cmp(pbMetaStructure->lpValue,L"Http")) 
			pInet->SetProtocol(PROTOCOL_HTTP);
		else if(cmp(pbMetaStructure->lpValue,L"Ftp"))
			pInet->SetProtocol(PROTOCOL_FTP);
	}
	else if (cmp(pbMetaStructure->lpParameter, L"SetFileDestination"))
	{
		
		if (cmp(pbMetaStructure->lpValue, L"True"))
			pInet->SetFileDestination(TRUE);
		else if (cmp(pbMetaStructure->lpValue, L"False"))
			pInet->SetFileDestination(FALSE);
	}
	else if (cmp(pbMetaStructure->lpParameter, L"Port"))
	{
		pInet->SetPort((WORD) _wtoi(pbMetaStructure->lpValue));
	}
	else if(cmp(pbMetaStructure->lpParameter, L"CreateFolders")) 
	{
		if(cmp(pbMetaStructure->lpValue,L"True"))
			pInet->SetCreateDirectories(TRUE);
		else if(cmp(pbMetaStructure->lpValue,L"False"))
			pInet->SetCreateDirectories(FALSE);
	}
	else if(cmp(pbMetaStructure->lpParameter, L"Source")) 
	{
		pInet->SetCurrentURL(GetCurrURL(pPBStructure->iTabID));
		return pInet->SetSource(pbMetaStructure->lpValue);
	}
	else if(cmp(pbMetaStructure->lpParameter, L"Destination")) 
	{
		pInet->SetCurrentURL(GetCurrURL(pPBStructure->iTabID));
		return pInet->SetDestination(pbMetaStructure->lpValue);	
	}
	else if (cmp(pbMetaStructure->lpParameter, L"Overwrite"))
	{
		if(cmp(pbMetaStructure->lpValue,L"True"))
			pInet->SetOverwriteDestination(TRUE);
		else if(cmp(pbMetaStructure->lpValue,L"False"))
			pInet->SetOverwriteDestination(FALSE);
	}
	else if (cmp(pbMetaStructure->lpParameter, L"Username"))
	{
		pInet->SetUserName(pbMetaStructure->lpValue);
	}
	else if (cmp(pbMetaStructure->lpParameter, L"Password"))
	{
		pInet->SetPassword(pbMetaStructure->lpValue);
	}
	else if(cmp(pbMetaStructure->lpParameter, L"TransferEvent")) 
	{
		return pInet->SetTransferEventString(pbMetaStructure->lpValue);
	}
	else if(cmp(pbMetaStructure->lpParameter, L"Transfer")) 
	{
		return pInet->Transfer();
	}
	else
	{
		//  Unrecognized tag
		pErr = new TCHAR[100];
		if(pErr){
			_tcscpy(pErr,L"Unrecognised Meta Tag sent to FileTransfer: \"");
			_tcscat(pErr,pbMetaStructure->lpParameter);
			_tcscat(pErr,L"\"");
			Log(PB_LOG_WARNING, pErr,_T(__FUNCTION__), __LINE__);
			delete [] pErr;
		}
		return FALSE;
	}
	return TRUE;
}




	