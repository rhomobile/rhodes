#include "BacklightModule.h"


/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		November 2009
* \date		April 2012   (HP: Modified for TSD)
*/
BOOL CBacklightModule::onInit(PPBSTRUCT pPBStructure)
{
	wcscpy (m_szModName, L"Backlight");
	RegisterForEvent(PB_APPFOCUSEVENT);	
	RegisterForEvent(RHO_APPFOCUS_EVENT);
	m_dwBacklightDLL = 0;
	return TRUE;
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		November 2009
*/
void CBacklightModule::onDeInit(PPBSTRUCT pPBStructure)
{
	UnRegisterForEvent(PB_APPFOCUSEVENT);	
	UnRegisterForEvent(RHO_APPFOCUS_EVENT);
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		November 2009
* \date		April 2012   (HP: Modified for TSD)
*/
BOOL CBacklightModule::onAttachInstance(PPBSTRUCT pPBStructure, PPBINSTSTRUCT pInstStruct)
{
	INSTANCE_DATA *pData = new INSTANCE_DATA;	
	pData->instanceID = pInstStruct->instID;
	//  Set URI to empty string
	memset(pData->tcBacklightSettingsEvent, 0, sizeof(TCHAR));	

	TCHAR LogMessage[MAX_LOG_MESSAGE];
	BOOL retVal = FALSE;
	if (pData)
	{
		//  Attempt to load the Backlight DLL
		
		if (!LoadBacklightDLL())
		{
			//  Backlight DLL has failed to load, log this error
			wsprintf(LogMessage, L"%s %s %s", L"Unable to Load", BACKLIGHT_API_DLL, 
				L"DLL");
			Log(PB_LOG_WARNING, LogMessage, 
				_T(__FUNCTION__), __LINE__);
		}
		else
		{
			//  Backlight DLL has successfully loaded
			//  Find out the number of intensity levels supported by this device, 
			//  this is stored as a member variable to prevent the user requesting
			//  an intensity too great.

			//  Retrieve the current backlight on / off and intensity to store in the 
			//  instance Data
			DWORD dwTempVariable;	
			DWORD dwRetVal;	
			dwRetVal = lpfn_BacklightGetState(&dwTempVariable);
			if (dwRetVal != E_DSP_SUCCESS)
				return FALSE;
			pData->dwBacklightState = dwTempVariable;
			pData->dwDefaultState = dwTempVariable;
			
			dwRetVal = lpfn_BacklightGetIntensity(&dwTempVariable);
			if (dwRetVal != E_DSP_SUCCESS)
				return FALSE;
			pData->dwBacklightIntensity = dwTempVariable;	
			pData->dwDefaultIntensity = dwTempVariable;

			//  Log the maximum intensity value the user can set the backlight to
			dwRetVal = lpfn_BacklightGetIntensityLevels(&dwTempVariable);
			if (dwRetVal != E_DSP_SUCCESS)
				return FALSE;
			pData->dwBacklightRange = dwTempVariable;

			TCHAR tcLogMessage[256];
			wsprintf(tcLogMessage, 
				L"The current device supports levels of backlight intensity between 0 and %i inclusive", 
				dwTempVariable - 1);
			Log(PB_LOG_INFO, tcLogMessage, _T(__FUNCTION__), __LINE__);
			pData->bAppHasFocus = (pPBStructure->bLaunchingAppHasFocus == TRUE);

		}
		pInstStruct->pWrappedObj = pData;
	}
	return retVal;
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		November 2009
* \date		April 2012   (HP: Modified for TSD)
*/
BOOL CBacklightModule::onReleaseInstance(PPBSTRUCT pPBStructure, PPBINSTSTRUCT pInstStruct)
{
	INSTANCE_DATA *pData = (INSTANCE_DATA*) (pInstStruct->pWrappedObj);
	
	if (pData)
	{
		delete pData;
	}

	UnloadBacklightDLL();

	return TRUE;
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		October 2009
* \date		April 2012   (HP: Modified for TSD)
*/
BOOL CBacklightModule::onAppFocus(int iOldID, int iNewID)
{
	//  Consider Initialisation Steps
	if ((iOldID == iNewID) ||
		(iOldID < 0) ||
		(iNewID < 0))
		return TRUE;

	INSTANCE_DATA *pData = (INSTANCE_DATA*) GetObjFromID(iNewID);

	//  Test the new application gaining focus is an application 
	//  using the Backlight module
	if (!pData)
		return TRUE;

	//  Turn the backlight on / off or set the intensity as appropriate
	//  (another application may have changed it)
	lpfn_BacklightSetState(pData->dwBacklightState);
	lpfn_BacklightSetIntensity(pData->dwBacklightIntensity);

	//  Set the Nav String to ""
	wcscpy(pData->tcBacklightSettingsEvent, L"");		
	return TRUE;
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		November 2009
* \date		April 2012   (HP: Modified for TSD)
*/
BOOL CBacklightModule::MetaProc(PBMetaStruct *pbMetaStructure, PPBSTRUCT pPBStructure, void *pParam)
{
	INSTANCE_DATA *pData = (INSTANCE_DATA*) pParam;
	DWORD dwRetVal;

	if (cmp (pbMetaStructure->lpParameter, L"On"))
	{
		pData->dwBacklightState = BACKLIGHT_ON;		
		if (pData->bAppHasFocus)
		{
			dwRetVal = lpfn_BacklightSetState(BACKLIGHT_ON);
			if (dwRetVal != E_DSP_SUCCESS)
			{
				Log(PB_LOG_INFO, 
					L"Unable to turn on the Backlight", 
					_T(__FUNCTION__), __LINE__);
			}
		}
	}
	else if (cmp(pbMetaStructure->lpParameter, L"Off"))
	{
		pData->dwBacklightState = BACKLIGHT_OFF;
		if (pData->bAppHasFocus)
		{
			dwRetVal = lpfn_BacklightSetState(BACKLIGHT_OFF);
			if (dwRetVal != E_DSP_SUCCESS)
			{
				Log(PB_LOG_INFO, 
					L"Unable to turn off the Backlight", 
					_T(__FUNCTION__), __LINE__);
			}
		}
	}
	else if (cmp(pbMetaStructure->lpParameter, L"Intensity"))
	{
		dwRetVal = E_DSP_NULLPTR;		
		if (pbMetaStructure->lpValue)
		{
			DWORD dwBacklightIntensity = _wtoi(pbMetaStructure->lpValue);
			pData->dwBacklightIntensity = dwBacklightIntensity;
			if (pData->bAppHasFocus)
			{
				dwRetVal = lpfn_BacklightSetIntensity(dwBacklightIntensity);
			}
		}
		if (dwRetVal != E_DSP_SUCCESS)
		{
			Log(PB_LOG_INFO, 
				L"Unable to set the Backlight intensity", 
				_T(__FUNCTION__), __LINE__);
		}
	}
	else if (cmp (pbMetaStructure->lpParameter, L"backlightSettingsEvent"))
	{
		//  Set the URI to respond to the GetBacklightSettings method.
		if (wcslen(pbMetaStructure->lpValue) <= MAX_URL)
			wcscpy(pData->tcBacklightSettingsEvent, pbMetaStructure->lpValue);
		else
			return FALSE;
	}
	else if (cmp(pbMetaStructure->lpParameter, L"getBacklightSettings"))
	{
		if (*pData->tcBacklightSettingsEvent)
		{
			DWORD dwBacklightIntensity = pData->dwBacklightIntensity;
			DWORD dwBacklightRange = pData->dwBacklightRange;
			DWORD dwBacklightState = pData->dwBacklightState;

			TCHAR tcBacklightIntensity[10]; //decimal representation of the intensity
			wsprintf(tcBacklightIntensity, L"%d", dwBacklightIntensity);

			TCHAR tcBacklightState[10]; //"on" or "off" or "timeout"
			switch(dwBacklightState)
			{
				case BACKLIGHT_ON:
					wcscpy(tcBacklightState, L"on");
					break;
				case BACKLIGHT_OFF:
					wcscpy(tcBacklightState, L"off");
					break;
				case BACKLIGHT_TIMEOUT:
					wcscpy(tcBacklightState, L"timeout");
					break;
				default:
					wcscpy(tcBacklightState, L"ERROR");
					break;
			}

			TCHAR *tcBacklightArray = new TCHAR[wcslen(L"new Array()") + (dwBacklightRange * 10) + 1];
			wcscpy(tcBacklightArray, L"new Array(");

			for (uint i=0; i<dwBacklightRange; i++)
			{
				wsprintf(tcBacklightArray, L"%s'%i'", tcBacklightArray, i);
				if (i != dwBacklightRange - 1)
					wcscat(tcBacklightArray, L",");
			}

			//  We have finished populating the backlight settings array
			wcscat(tcBacklightArray, L")");
			
			//  tcBacklightArray will be sent to the User

			//  Send to Application
			SendPBNavigate(tcBacklightEventNames, pData->instanceID, pData->tcBacklightSettingsEvent, 
				tcBacklightIntensity, tcBacklightArray, tcBacklightState, NULL);

			//  Delete memory we allocated		
			delete[] tcBacklightArray;
		}
	}
	else
	{
		// Unrecognised tag
		Log(PB_LOG_WARNING, L"Unrecognised Meta Tag Provided to Backlight Module",
			_T(__FUNCTION__), __LINE__);
		return FALSE;
	}

	return TRUE;
}

/**
* \author	Harish Pulimi (HP, PXB743)
* \date		April 2012  (HP: Initial Creation)
*/
BOOL CBacklightModule::LoadBacklightDLL()
{
	NullifyExportedFunctionPointers();

	m_dwBacklightDLL = (DWORD)LoadLibrary(BACKLIGHT_API_DLL);
	if (!m_dwBacklightDLL)
	{
		TCHAR LogMessage[MAX_LOG_MESSAGE];
		wsprintf(LogMessage, L"%s %s %s", L"Could not find", BACKLIGHT_API_DLL, 
			L"DLL");
		Log(PB_LOG_ERROR, LogMessage, 
			_T(__FUNCTION__), __LINE__);
	}
	
	//  Map our function pointers to functions exported from the DLL
	lpfn_BacklightGetIntensityLevels = 
		(LPFN_BACKLIGHT_GET_INTENSITY_LEVELS)GetProcAddress
		((HMODULE)m_dwBacklightDLL, BACKLIGHT_GETINTENSITYLEVELS);
	lpfn_BacklightGetIntensity = 
		(LPFN_BACKLIGHT_GET_INTENSITY)GetProcAddress
		((HMODULE)m_dwBacklightDLL, BACKLIGHT_GETINTENSITY);
	lpfn_BacklightSetIntensity = 
		(LPFN_BACKLIGHT_SET_INTENSITY)GetProcAddress
		((HMODULE)m_dwBacklightDLL, BACKLIGHT_SETINTENSITY);
	lpfn_BacklightGetState = 
		(LPFN_BACKLIGHT_GET_STATE)GetProcAddress
		((HMODULE)m_dwBacklightDLL, BACKLIGHT_GETSTATE);
	lpfn_BacklightSetState = 
		(LPFN_BACKLIGHT_SET_STATE)GetProcAddress
		((HMODULE)m_dwBacklightDLL, BACKLIGHT_SETSTATE);

	//  All functions should now be loaded
	if ((!lpfn_BacklightGetIntensityLevels) ||
		(!lpfn_BacklightGetIntensity) ||
		(!lpfn_BacklightSetIntensity) ||
		(!lpfn_BacklightGetState) ||
		(!lpfn_BacklightSetState))
	{
		//  One of the functions failed to load
		UnloadBacklightDLL();

		TCHAR LogMessage[MAX_LOG_MESSAGE];
		wsprintf(LogMessage, L"%s %s %s", L"Unable to load all functions from", 
			BACKLIGHT_API_DLL, L"DLL");
		Log(PB_LOG_ERROR, LogMessage, 
			_T(__FUNCTION__), __LINE__);

		return FALSE;
	}

	//  Everything Loaded OK
	return TRUE;
}

/**
* \author	Harish Pulimi (HP, PXB743)
* \date		April 2012  (HP: Initial Creation)
*/
void CBacklightModule::UnloadBacklightDLL()
{
	//  If the Backlight DLL had previously been loaded then unload it and
	//  set all function pointers to NULL;
	if (m_dwBacklightDLL)
		FreeLibrary((HMODULE)m_dwBacklightDLL);
	
	//  Nullify member variables
	m_dwBacklightDLL = 0;
	NullifyExportedFunctionPointers();

}

/**
* \author	Harish Pulimi (HP, PXB743)
* \date		April 2012  (HP: Initial Creation)
*/
void CBacklightModule::NullifyExportedFunctionPointers()
{
	lpfn_BacklightGetIntensityLevels= NULL;
	lpfn_BacklightGetIntensity		= NULL;
	lpfn_BacklightSetIntensity		= NULL;
	lpfn_BacklightGetState			= NULL;
	lpfn_BacklightSetState			= NULL;
}

/**
* \author	John Heywood (JH, vbc863)
* \date		September 2012  (JH: Initial Creation)
*/
BOOL CBacklightModule::onRhoAppFocus(bool bActivate, int iInstID)
{
	INSTANCE_DATA *pData = NULL;
	pData = (INSTANCE_DATA*) GetObjFromID(iInstID);
	if (pData != NULL)
	{
		if (!bActivate && pData->bAppHasFocus)
		{
			// App has lost focus - restore defaults
			lpfn_BacklightSetIntensity(pData->dwDefaultIntensity);
			lpfn_BacklightSetState(pData->dwDefaultState);
		}
		else if (bActivate && !pData->bAppHasFocus)
		{
			// App has gained focus
			lpfn_BacklightSetIntensity(pData->dwBacklightIntensity);
			lpfn_BacklightSetState(pData->dwBacklightState);
		}
		pData->bAppHasFocus = bActivate;
		return TRUE;
	}
	else
		return FALSE;
};

