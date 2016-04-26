#include "CradleModule.h"


/**
* \author	John Heywood (JHMH, VBC863)
* \date		June 2012 Initial Creation, JHMH
*/
BOOL CCradleModule::onInit(PPBSTRUCT pPBStructure)
{
	wcscpy(m_szModName,L"Cradle");
	RegisterForEvent(PB_BROWSER_BEFORE_NAV_EVENT);
	return TRUE;
}

/**
* \author	John Heywood (JHMH, VBC863)
* \date		June 2012 Initial Creation, JHMH
*/
void CCradleModule::onDeInit(PPBSTRUCT pPBStructure)
{
	UnRegisterForEvent(PB_BROWSER_BEFORE_NAV_EVENT);
}


/**
* \author	John Heywood (JHMH, VBC863)
* \date		June 2012 Initial Creation, JHMH
*/
BOOL CCradleModule::onAttachInstance(PPBSTRUCT pPBStructure,PPBINSTSTRUCT pInstStruct)
{
	Log(PB_LOG_INFO,L"Attaching Instance",_T(__FUNCTION__),__LINE__);
	//RegisterForMessage (pPBStructure->hWnd, WM_MOUSEMOVE);
		
	pInstStruct->pWrappedObj = 
		(CCradle*) new CCradle(pInstStruct->instID, this);
	
	if(pInstStruct->pWrappedObj)
		return TRUE;
	else
		return FALSE;
}

/**
* \author	John Heywood (JHMH, VBC863)
* \date		June 2012 Initial Creation, JHMH
*/
BOOL CCradleModule::onReleaseInstance(PPBSTRUCT pPBStructure,PPBINSTSTRUCT pInstStruct)
{
	Log(PB_LOG_INFO,L"Deleting Instance",_T(__FUNCTION__),__LINE__);
	delete (CCradle*)pInstStruct->pWrappedObj;
	pInstStruct->pWrappedObj = NULL;
	return TRUE;
}


/**
* \author	John Heywood (JHMH, VBC863)
* \date		June 2012 Initial Creation, JHMH
*/
BOOL CCradleModule::onBeforeNavigate(int iInstID)
{
	// Get the instance data - problem if it's not available
	CCradle *pCradle = (CCradle*) GetObjFromID (iInstID);
	if (!pCradle)
	{
		Log(PB_LOG_WARNING, L"Instance data not available",_T(__FUNCTION__), __LINE__);
		return FALSE;
	}

	pCradle->ClearCradleURI(); // ignore the return value as we don't care whether it was set or not

	return TRUE;
}

BOOL CCradleModule::MetaProc(PBMetaStruct *pbMetaStructure, PPBSTRUCT pPBStructure,VOID* pParam)
{
	CCradle *pCradle = (CCradle*)pParam;
	
	if(pCradle){

		///////////////////////////////////////////////////////////////////////////////
		// Methods
		///////////////////////////////////////////////////////////////////////////////
		if(cmp(pbMetaStructure->lpParameter, L"getCradleId")){
			return pCradle->GetCradleID();
		}
		
		///////////////////////////////////////////////////////////////////////////////
		// Properties
		///////////////////////////////////////////////////////////////////////////////
		else if(cmp(pbMetaStructure->lpParameter, L"cradleEvent")){
			return pCradle->SetCradleURI(pbMetaStructure->lpValue);
		}

		//  Unrecognized tag
		Log(PB_LOG_WARNING, L"Unrecognised Meta Tag Provided to Cradle Module",
			_T(__FUNCTION__), __LINE__);
	}
	return false;
}

