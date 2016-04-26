#include "StdAfx.h"
#include "EMMPMod.h"

CEMMPMod::CEMMPMod(void)
	:m_FileTransferIMO(NULL)
{

}

CEMMPMod::~CEMMPMod(void)
{
}

BOOL CEMMPMod::onInit(PPBSTRUCT pPBStructure)
{
	RegisterForEvent(PB_BROWSER_BEFORE_NAV_EVENT);
	return (BOOL)_tcscpy(m_szModName,L"EMMLProfile");
}

BOOL CEMMPMod::onDeInit(PBSTRUCT pPBStructure)
{
	if(m_FileTransferIMO)
		DeleteIMO(m_FileTransferIMO);

	UnRegisterForEvent(PB_BROWSER_BEFORE_NAV_EVENT);
	return TRUE;
}

BOOL CEMMPMod::onAttachInstance(PPBSTRUCT pPBStructure , PPBINSTSTRUCT pInstStruct)
{
	pInstStruct->instID;
	pInstStruct->pWrappedObj = (CEMMP*) new CEMMP(this, pPBStructure->iTabID);

	if(pInstStruct->pWrappedObj)
	{
		return TRUE;
	}
	return FALSE;

}

BOOL CEMMPMod::onReleaseInstance(PPBSTRUCT pPBStructure , PPBINSTSTRUCT pInstStruct)
{
	delete pInstStruct->pWrappedObj;
	pInstStruct->pWrappedObj = NULL;
	return TRUE;

}

BOOL CEMMPMod::MetaProc(PBMetaStruct *pbMetaStructure, PPBSTRUCT pPBStructure,VOID* pParam)
{
	CEMMP *pEMMP = (CEMMP*)pParam;
	
	Log(PB_LOG_INFO,pbMetaStructure->lpParameter,L"MetaProc",__LINE__);
	
	//start the parsing of the Meta tag info
	if(cmp(pbMetaStructure->lpParameter, L"importProfile") ||
		cmp(pbMetaStructure->lpParameter, L"import")) {	
		return pEMMP->Import((LPCTSTR)pbMetaStructure->lpValue);
	}
	else if(cmp(pbMetaStructure->lpParameter, L"name")){
		pEMMP->SetName((LPCTSTR)pbMetaStructure->lpValue);
		return TRUE;
	}
	else if(cmp(pbMetaStructure->lpParameter, L"clear")) {
		return pEMMP->Delete((LPCTSTR)pbMetaStructure->lpValue);
	}
	else if(cmp(pbMetaStructure->lpParameter, L"apply")) {
		return pEMMP->Apply((LPCTSTR)pbMetaStructure->lpValue);
	}
	else{
		return FALSE;//unrecognized tag
	}

}

/**
* \author	Paul Henderson (PH, PTJK76)
* \date		November 2009 Initial Creation, PH
*/
BOOL CEMMPMod::onBeforeNavigate(int iInstID)
{
	//  If we are navigating to a new page hide the signature capture box and 
	//  set the TransferEvent navigation URIs to blank
	CEMMP *pEMMP = (CEMMP *)GetObjFromID(iInstID);
	if(pEMMP)
	{
		//delete the temporary profile pages
		pEMMP->m_pEMMLObj->DeleteEMMLProfilePage();
		Log(PB_LOG_INFO, L"Deleted temporary profile page due to page navigation.", _T(__FUNCTION__), __LINE__);
	}
	return TRUE;
}