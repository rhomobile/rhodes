
#include "StdAfx.h"
#include "SmartCradleModule.h"
#include "../../../common/public/pbplugin.h"

extern CSmartCradleModule *g_SmartCradleModule;


BOOL CSmartCradleModule::onBeforeNavigate(int iInstID)
{
	CCradle *pCradle;

	PBModule::Log(PB_LOG_DEBUG, L"Change of page for SmartCradle", L"onNaviagtionEvent", __LINE__);
	pCradle = (CCradle *) GetObjFromID(iInstID);
	pCradle->SetDefaultValues();
	return TRUE;
}

CSmartCradleModule::CSmartCradleModule(void)
{
}
//
CSmartCradleModule::~CSmartCradleModule(void)
{
	return;
}
BOOL CSmartCradleModule::onAttachInstance(PBStruct *pPBStructure, PBInstStruct *pPBInstStruct)
{
	Log(PB_LOG_INFO,L"Creating Cradle Instance",L"onAttachInstance",__LINE__);
	CCradle *pSmartCradle=NULL;
	pPBInstStruct->pWrappedObj = pSmartCradle=(CCradle*)new CCradle();
	//if (pPBInstStruct->pWrappedObj)
	//	return TRUE;

	if(pPBInstStruct->pWrappedObj){

	//Save copy with instance
	pSmartCradle->PBStructure.hWnd = pPBStructure->hWnd;
	pSmartCradle->PBStructure.hInstance = pPBStructure->hInstance;
	pSmartCradle->PBStructure.bInvoked = pPBStructure->bInvoked;
	pSmartCradle->PBStructure.iTabID = pPBStructure->iTabID;




		return TRUE;
	}



	return FALSE;
}
BOOL CSmartCradleModule::onReleaseInstance(PBStruct *pPBStructure, PBInstStruct *pPBInstStruct)
{
	Log(PB_LOG_INFO,L"Deleting Cradle Instance",L"onReleaseInstance",__LINE__);
	delete (CCradle*) pPBInstStruct->pWrappedObj;
	pPBInstStruct->pWrappedObj = NULL;
	return TRUE;
}

BOOL CSmartCradleModule::onInit(PPBSTRUCT pPBStructure)
{
	_tcscpy(m_szModName,L"SmartCradle");

	if(RegisterForEvent(PB_BROWSER_BEFORE_NAV_EVENT))
	{
	return true;
	}
	else
	{
	return false;
	}
}

void CSmartCradleModule::onDeInit(PBStruct *pPBStructure)
{
UnRegisterForEvent(PB_BROWSER_BEFORE_NAV_EVENT);
}

BOOL CSmartCradleModule::MetaProc(PBMetaStruct *pPBMetaStructure, PBStruct *pPBStructure, VOID *pParam)
{
	BOOL bRet=TRUE;
	CCradle *pCradle = (CCradle *) pParam;
	Log(PB_LOG_INFO,pPBMetaStructure->lpParameter,L"MetaProc",__LINE__);

	if(!pCradle)
	{
		return FALSE;
	}



	if(cmp(pPBMetaStructure->lpParameter, L"Unlock"))
	{
		pCradle->UnlockCradle();
		return TRUE;
	}
	else if(cmp(pPBMetaStructure->lpParameter, L"ChargeState"))
	{
	
		if(cmp(pPBMetaStructure->lpValue,L"Fast"))
		{
			pCradle->SetCradleFastChargeState(true);
		}
		else if(cmp(pPBMetaStructure->lpValue,L"Slow"))
		{
			pCradle->SetCradleFastChargeState(false);
		}
		else
		{
			Log(PB_LOG_WARNING,L"InValid parameter passed for ChargeSate",L"MetaProc",__LINE__);
		}
		

	}

	else if(cmp(pPBMetaStructure->lpParameter, L"RowId"))
	{
	int temp;
	temp = _wtoi(pPBMetaStructure->lpValue);
	pCradle->SetCradleMatrixRow(temp);
	}
	else if(cmp(pPBMetaStructure->lpParameter, L"ColumnId"))
	{
	int temp;
	temp = _wtoi(pPBMetaStructure->lpValue);
	pCradle->SetCradleMatrixColumn(temp);
	}
	else if(cmp(pPBMetaStructure->lpParameter, L"WallId"))
	{
	int temp;
	temp = _wtoi(pPBMetaStructure->lpValue);
	pCradle->SetCradleMatrixWall(temp);
	}
	else if(cmp(pPBMetaStructure->lpParameter, L"UnlockLEDOnDuration"))
	{
	int temp;
	temp = _wtoi(pPBMetaStructure->lpValue);
	pCradle->SetCradleUnlockOnDuration(temp);
	}
	else if(cmp(pPBMetaStructure->lpParameter, L"UnlockLEDOffDuration"))
	{
	int temp;
	temp = _wtoi(pPBMetaStructure->lpValue);
	pCradle->SetCradleUnlockOffDuration(temp);
	}
		else if(cmp(pPBMetaStructure->lpParameter, L"UnlockLEDOffDuration"))
	{
	int temp;
	temp = _wtoi(pPBMetaStructure->lpValue);
	pCradle->SetCradleUnlockOffDuration(temp);
	}	
	else if(cmp(pPBMetaStructure->lpParameter, L"UnlockTimeOut"))
	{
	int temp;
	temp = _wtoi(pPBMetaStructure->lpValue);
	pCradle->SetCradleUnlockTimeOut(temp);
	}

	else if (cmp(pPBMetaStructure->lpParameter, L"CradleInfoEvent"))
	{
		if (pPBMetaStructure->lpValue != NULL && wcslen(pPBMetaStructure->lpValue) < MAX_URL)
		{
			wcscpy(pCradle->m_CradleInfoEvent, pPBMetaStructure->lpValue);
			return TRUE;
		}
		else
			return FALSE;
	}
	else if (cmp(pPBMetaStructure->lpParameter, L"CradleMatrixEvent"))
	{
		if (pPBMetaStructure->lpValue != NULL && wcslen(pPBMetaStructure->lpValue) < MAX_URL)
		{
			wcscpy(pCradle->m_CradleMatrixEvent, pPBMetaStructure->lpValue);
			return TRUE;
		}
		else
			return FALSE;
	}
	else if (cmp(pPBMetaStructure->lpParameter, L"GetCradleInfo"))
	{
		pCradle->GetCradleInfo();
		return TRUE;
	}
	else if (cmp(pPBMetaStructure->lpParameter, L"GetCradleMatrix"))
	{
		pCradle->GetCradleMatrix();
		return TRUE;
	}
	else
	{
	bRet=false;
	}

	return bRet;
}











