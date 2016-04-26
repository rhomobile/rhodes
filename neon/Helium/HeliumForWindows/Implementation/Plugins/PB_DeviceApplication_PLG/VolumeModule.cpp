#include "VolumeModule.h"

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		November 2009
*/
BOOL CVolumeModule::onInit(PPBSTRUCT pPBStructure)
{
	wcscpy (m_szModName, L"Volume");
	RegisterForEvent(PB_APPFOCUSEVENT);
	RegisterForEvent(RHO_APPFOCUS_EVENT);
	return TRUE;
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		November 2009
*/
void CVolumeModule::onDeInit(PPBSTRUCT pPBStructure)
{
	UnRegisterForEvent(PB_APPFOCUSEVENT);
	UnRegisterForEvent(RHO_APPFOCUS_EVENT);
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		November 2009
*/
BOOL CVolumeModule::onAttachInstance(PPBSTRUCT pPBStructure, PPBINSTSTRUCT pInstStruct)
{
	INSTANCE_DATA *pData = new INSTANCE_DATA;
	pData->instanceID = pInstStruct->instID;
	
	//  Retrieve the current volume level to store in the 
	//  instance Data
	DWORD dwTempVariable;
	DWORD dwRetVal;
	dwRetVal = waveOutGetVolume((HWAVEOUT)-1, &dwTempVariable);
	if (dwRetVal != MMSYSERR_NOERROR)
		return FALSE;

	pData->dwCurrentVolume = dwTempVariable;
	pData->dwDefaultVolume = dwTempVariable;
	pData->bAppHasFocus = pPBStructure->bLaunchingAppHasFocus;
	pInstStruct->pWrappedObj = pData;

	return TRUE;
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		November 2009
*/
BOOL CVolumeModule::onReleaseInstance(PPBSTRUCT pPBStructure, PPBINSTSTRUCT pInstStruct)
{
	INSTANCE_DATA *pData = (INSTANCE_DATA*) (pInstStruct->pWrappedObj);
	
	if (pData)
	{
		delete pData;
	}

	return TRUE;
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		October 2009
*/
BOOL CVolumeModule::onAppFocus(int iOldID, int iNewID)
{
	//  Consider Initialisation Steps
	if ((iOldID == iNewID) ||
		(iOldID < 0) ||
		(iNewID < 0))
		return TRUE;

	INSTANCE_DATA *pData = (INSTANCE_DATA*) GetObjFromID(iNewID);

	//  Test the new application gaining focus is an application 
	//  using the Keylight module
	if (!pData)
		return TRUE;

	//  Turn the backlight on / off or set the intensity as appropriate
	//  (another application may have changed it)
	waveOutSetVolume((HWAVEOUT)-1, pData->dwCurrentVolume);
	return TRUE;
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		November 2009
*/
BOOL CVolumeModule::MetaProc(PBMetaStruct *pbMetaStructure, PPBSTRUCT pPBStructure, void *pParam)
{
	INSTANCE_DATA *pData = (INSTANCE_DATA*) pParam;

	if (cmp(pbMetaStructure->lpParameter, L"SetVolume") &&
		pbMetaStructure->lpValue != NULL && !(cmp(pbMetaStructure->lpValue, L"")))
	{
		DWORD dwVolume = 0;
		//  Assume receiving the volume as a hex value if prepended with 0x, else
		//  assume the value is a decimal
		if ((wcslen(pbMetaStructure->lpValue) > 2) && 
			(_wcsnicmp(pbMetaStructure->lpValue, L"0x", 2) == 0))
		{
			//  Passed value is a Hex Number
			dwVolume = wcstol(pbMetaStructure->lpValue + 2, NULL, 16);
		}
		else
		{
			dwVolume = wcstol(pbMetaStructure->lpValue, NULL, 10);
		}
		pData->dwCurrentVolume = dwVolume;
		if (pData->bAppHasFocus)
			waveOutSetVolume((HWAVEOUT)-1, dwVolume);
	}
	else
	{
		// Unrecognised tag
		Log(PB_LOG_INFO, L"Unrecognised Meta Tag Provided to Volume Module, possibly from Regex'd PB2.x tags",
			_T(__FUNCTION__), __LINE__);
		return FALSE;
	}

	return TRUE;
}

BOOL CVolumeModule::onRhoAppFocus(bool bActivate, int iInstID)
{
	INSTANCE_DATA *pData = NULL;
	pData = (INSTANCE_DATA*) GetObjFromID(iInstID);
	if (pData != NULL)
	{
		if (!bActivate && pData->bAppHasFocus)
		{
			// We've just lost focus
			pData->bAppHasFocus = false;

			// reset the volume to default
			waveOutSetVolume((HWAVEOUT)-1, pData->dwDefaultVolume);
		}
		else if (bActivate && !pData->bAppHasFocus)
		{
			// we've just regained focus
			// store the last system volume
			DWORD dwTempVariable;
			if (waveOutGetVolume((HWAVEOUT)-1, &dwTempVariable) == MMSYSERR_NOERROR)
				pData->dwDefaultVolume = dwTempVariable;

			// set our required volume
			waveOutSetVolume((HWAVEOUT)-1, pData->dwCurrentVolume);
			pData->bAppHasFocus = true;
		}
		return TRUE;
	}
	else
		return FALSE;
};
