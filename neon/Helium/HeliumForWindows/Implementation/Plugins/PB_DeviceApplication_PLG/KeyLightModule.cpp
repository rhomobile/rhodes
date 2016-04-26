#include "KeylightModule.h"

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		November 2009
* \todo	When I just register for the App Focus event I don't get the 
*  onAppFocus triggered, this is presumably a problem with PBPlugin.cpp and 
*  needs to be fixed.
*/
BOOL CKeylightModule::onInit(PPBSTRUCT pPBStructure)
{
	wcscpy (m_szModName, L"Keylight");
	RegisterForEvent(PB_APPFOCUSEVENT);
	RegisterForEvent(RHO_APPFOCUS_EVENT);
	return TRUE;
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		November 2009
*/
void CKeylightModule::onDeInit(PPBSTRUCT pPBStructure)
{
	UnRegisterForEvent(PB_APPFOCUSEVENT);
	UnRegisterForEvent(RHO_APPFOCUS_EVENT);
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		November 2009
*/
BOOL CKeylightModule::onAttachInstance(PPBSTRUCT pPBStructure, PPBINSTSTRUCT pInstStruct)
{
	//  Create the new PowerOn object and add it to this instance's wrapped object
	CKeylight *pKeylight = new CKeylight(pInstStruct->instID, this);
	BOOL retVal = FALSE;
	if (pKeylight)
	{
		retVal = pKeylight->Initialise(pPBStructure->bLaunchingAppHasFocus == TRUE);
		pInstStruct->pWrappedObj = pKeylight;
	}

	return retVal;
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		November 2009
*/
BOOL CKeylightModule::onReleaseInstance(PPBSTRUCT pPBStructure, PPBINSTSTRUCT pInstStruct)
{
	CKeylight *pKeylight = (CKeylight*) (pInstStruct->pWrappedObj);
	
	if (pKeylight)
	{
		delete pKeylight;
	}

	return TRUE;
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		October 2009
*/
BOOL CKeylightModule::onAppFocus(int iOldID, int iNewID)
{
	//  Consider Initialisation Steps
	if ((iOldID == iNewID) ||
		(iOldID < 0) ||
		(iNewID < 0))
		return TRUE;

	CKeylight *pKeylight = (CKeylight*) GetObjFromID(iNewID);

	//  Test the new application gaining focus is an application 
	//  using the Keylight module
	if (!pKeylight)
		return TRUE;

	//  Turn the keylight on / off or set the intensity as appropriate
	//  (another application may have changed it)
	return pKeylight->RefreshHardware();
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		November 2009
*/
BOOL CKeylightModule::MetaProc(PBMetaStruct *pbMetaStructure, PPBSTRUCT pPBStructure, void *pParam)
{
	CKeylight *pKeylight = (CKeylight*) pParam;

	if (cmp (pbMetaStructure->lpParameter, L"On"))
	{
		//  Turn the Keylight On
		pKeylight->TurnOn();
	}
	else if (cmp(pbMetaStructure->lpParameter, L"Off"))
	{
		//  Turn the Keylight Off
		pKeylight->TurnOff();
	}
	else if (cmp(pbMetaStructure->lpParameter, L"Intensity"))
	{
		//  Set the Keylight Intensity
		DWORD dwIntensity = _wtoi(pbMetaStructure->lpValue);
		pKeylight->SetIntensity(dwIntensity);
	}
	else
	{
		// Unrecognised tag
		Log(PB_LOG_WARNING, L"Unrecognised Meta Tag Provided to Keylight Module",
			_T(__FUNCTION__), __LINE__);
		return FALSE;
	}

	return TRUE;
}

/**
* \author	John Heywood (JH, VBC863)
* \date		September 2012
*/
BOOL CKeylightModule::onRhoAppFocus(bool bActivate, int iInstID)
{
	CKeylight *pKeylight = NULL;
	pKeylight = (CKeylight*) GetObjFromID(iInstID);
	if (pKeylight != NULL)
	{
		return pKeylight->ApplicationFocusChange(bActivate);
	}
	else
		return FALSE;
};
