#include "PowerOnModule.h"

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		November 2009
*/
BOOL CPowerOnModule::onInit(PPBSTRUCT pPBStructure)
{
	wcscpy (m_szModName, L"PowerOn");
	RegisterForEvent(PB_BROWSER_BEFORE_NAV_EVENT);
	return TRUE;
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		November 2009
*/
void CPowerOnModule::onDeInit(PPBSTRUCT pPBStructure)
{
	UnRegisterForEvent(PB_BROWSER_BEFORE_NAV_EVENT);
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		November 2009
*/
BOOL CPowerOnModule::onAttachInstance(PPBSTRUCT pPBStructure, PPBINSTSTRUCT pInstStruct)
{
	//  Create the new PowerOn object and add it to this instance's wrapped object
	CPowerOn *pPowerOn = new CPowerOn(pInstStruct->instID, this);
	if (pPowerOn)
	{
		pPowerOn->Start();
		pInstStruct->pWrappedObj = pPowerOn;
	}

	return TRUE;
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		November 2009
*/
BOOL CPowerOnModule::onReleaseInstance(PPBSTRUCT pPBStructure, PPBINSTSTRUCT pInstStruct)
{
	CPowerOn *pPowerOn = (CPowerOn*) (pInstStruct->pWrappedObj);
	
	if (pPowerOn)
	{
		delete pPowerOn;
	}

	return TRUE;
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		November 2009
*/
BOOL CPowerOnModule::onBeforeNavigate (int iInstID)
{
	CPowerOn *pPowerOn = (CPowerOn*)  GetObjFromID(iInstID);
	if (!pPowerOn)
		return TRUE;
	else
		return pPowerOn->SetNavigationURI(L"");
};

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		November 2009
*/
BOOL CPowerOnModule::MetaProc(PBMetaStruct *pbMetaStructure, PPBSTRUCT pPBStructure, void *pParam)
{
	CPowerOn *pPowerOn = (CPowerOn*) (pParam);
	if (cmp(pbMetaStructure->lpParameter, L"PowerOnEvent") && 
		pbMetaStructure->lpValue != NULL)
	{
		pPowerOn->SetNavigationURI(pbMetaStructure->lpValue);
	}
	else if (cmp(pbMetaStructure->lpParameter, L"PowerOnEventLegacy") && 
		pbMetaStructure->lpValue != NULL)
	{
		if (!cmp(pbMetaStructure->lpValue, L""))
			pPowerOn->SetNavigationURI(pbMetaStructure->lpValue);
		else
			return TRUE;
	}
	else
	{
		// Unrecognised tag
		Log(PB_LOG_WARNING, L"Unrecognised Meta Tag Provided to PowerOn Module, Possibly from Regex'd PB2.x tags",
			_T(__FUNCTION__), __LINE__);
		return FALSE;
	}

	return TRUE;
}