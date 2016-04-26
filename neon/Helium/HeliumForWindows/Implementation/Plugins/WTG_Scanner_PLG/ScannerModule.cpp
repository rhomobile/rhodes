#include "ScannerModule.h"
#include "Scanner.h"

CScannerModule::CScannerModule(PPBCORESTRUCT pPBCoreStructure)
{
}

BOOL CScannerModule::onInit(PPBSTRUCT pPBStructure)
{
	wcscpy(m_szModName, L"Scanner");
	RegisterForEvent(PB_BROWSER_BEFORE_NAV_EVENT);
	RegisterForEvent(PB_BROWSER_DOC_COMPLETE_EVENT);
	RegisterForEvent(RHO_APPFOCUS_EVENT);
	return TRUE;
}

void CScannerModule::onDeInit(PPBSTRUCT pPBStructure)
{
	UnRegisterForEvent(PB_BROWSER_BEFORE_NAV_EVENT);
	UnRegisterForEvent(PB_BROWSER_DOC_COMPLETE_EVENT);
	UnRegisterForEvent(RHO_APPFOCUS_EVENT);
}

BOOL CScannerModule::onAttachInstance(PPBSTRUCT pPBStructure, PPBINSTSTRUCT pInstStruct)
{
	//  Create Scanner object and Initialise it.  
	CScanner *pScanner = new CScanner(pInstStruct->instID,  this);
	pInstStruct->pWrappedObj = pScanner;
	BOOL bReturnValue = pScanner->Initialise(pPBStructure->bLaunchingAppHasFocus);
	return TRUE;
}

BOOL CScannerModule::onReleaseInstance(PPBSTRUCT pPBStructure, PPBINSTSTRUCT pInstStruct)
{
	CScanner* pScanner = (CScanner*)(pInstStruct->pWrappedObj);
	if (pScanner)
		delete pScanner;
	return TRUE;
}

BOOL CScannerModule::onBeforeNavigate(int iInstID)
{
	CScanner *pScanner = NULL;
	pScanner = (CScanner*) GetObjFromID(iInstID);

	if (pScanner != NULL)
	{
		return pScanner->BeforeNavigate();
	}
	else
		return FALSE;
}


BOOL CScannerModule::onDocumentComplete(int iInstID)
{
	CScanner *pScanner = NULL;
	pScanner = (CScanner*) GetObjFromID(iInstID);

	if (pScanner != NULL)
	{
		return pScanner->DocumentComplete();
	}
	else
		return FALSE;
}

BOOL CScannerModule::onRhoAppFocus(bool bActivate, int iInstID)
{
	CScanner *pScanner = NULL;
	pScanner = (CScanner*) GetObjFromID(iInstID);
	if (pScanner != NULL)
	{
		return pScanner->ApplicationFocusChange(bActivate);
	}
	else
		return FALSE;
};


BOOL CScannerModule::MetaProc(PBMetaStruct *pbMetaStructure, PPBSTRUCT pPBStructure, void *pParam)
{
	CScanner* pScanner = (CScanner*) (pParam);
	if (!pScanner)
		return FALSE;

	//  Consider my parameter value retrieval hack
//	if (wcslen(pbMetaStructure->lpValue) >= 3 && wcsncmp(pbMetaStructure->lpValue, L"GET", 3) == 0)
//	{
//		int iLen = pScanner->RetrieveEMMLTag(pbMetaStructure->lpParameter, NULL);
//		if (iLen < 0)
//		{
//			DEBUGMSG(TRUE, (L"Failed to Retrieve value for property: %s", pbMetaStructure->lpParameter));
//		}
//		else
//		{
//			WCHAR* szValue = new WCHAR[iLen];
//			pScanner->RetrieveEMMLTag(pbMetaStructure->lpParameter, szValue);
//			DEBUGMSG(TRUE, (L"\nRetrieved value for %s, it was: %s\n", pbMetaStructure->lpParameter, szValue));
//			return TRUE;
//		}
//	}
	Log(PB_LOG_WARNING, L"The Scanner API has been deprecated in 4.0, please transition your applications to use the Barcode API for future releases", _T(__FUNCTION__), __LINE__);
	return pScanner->ParseMETATag(pbMetaStructure->lpParameter, pbMetaStructure->lpValue);
}
