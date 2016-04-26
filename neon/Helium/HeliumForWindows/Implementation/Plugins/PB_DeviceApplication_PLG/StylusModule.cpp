#include "StylusModule.h"
#define STYLUS_DEVICE_ID L"TCH1:"
#define WIN_CE_7_MAJOR_VERSION 7

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		November 2009
*/
BOOL CStylusModule::onInit(PPBSTRUCT pPBStructure)
{
	wcscpy (m_szModName, L"Stylus");
	RegisterForEvent(PB_APPFOCUSEVENT);
	return TRUE;
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		November 2009
*/
void CStylusModule::onDeInit(PPBSTRUCT pPBStructure)
{
	UnRegisterForEvent(PB_APPFOCUSEVENT);
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		October 2009
*/
BOOL CStylusModule::onAttachInstance(PPBSTRUCT pPBStructure, PPBINSTSTRUCT pInstStruct)
{
	INSTANCE_DATA *pData = new INSTANCE_DATA;

	//  Retrive the current stylus enabled / disabled and store in the 
	//  instance Data
	CEDEVICE_POWER_STATE stylusState;
	DWORD dwResult = GetDevicePower(STYLUS_DEVICE_ID, POWER_NAME, &stylusState);
	
	//  Check the Return values are sensible
	if (dwResult != ERROR_SUCCESS || (stylusState != D0 && stylusState != D4))
	{
		//  We did not expect this return value from GetDevicePower
		Log(PB_LOG_INFO, 
			L"Unable to retrieve current stylus state, assuming stylus is enabled", 
			_T(__FUNCTION__), __LINE__);
		stylusState = (CEDEVICE_POWER_STATE)STYLUS_ENABLED;
	}
	//  Store the Stylus State in the Instance Data
	pData->currentStylusState = (STYLUS_STATE)stylusState;
	pInstStruct->pWrappedObj = pData;

	return TRUE;

}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		October 2009
*/
BOOL CStylusModule::onReleaseInstance(PPBSTRUCT pPBStructure, PPBINSTSTRUCT pInstStruct)
{
	INSTANCE_DATA *pData = (INSTANCE_DATA*) (pInstStruct->pWrappedObj);
	if (pData)
		delete pData;

	return TRUE;
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		October 2009
*/
BOOL CStylusModule::onAppFocus(int iOldID, int iNewID)
{
	//  Consider Initialisation Steps
	if ((iOldID == iNewID) ||
		(iOldID < 0) ||
		(iNewID < 0))
		return TRUE;

	INSTANCE_DATA *pDataNew = (INSTANCE_DATA*) GetObjFromID(iNewID);

	//  Test the new application gaining focus is an application 
	//  using the Stylus module and exit if it is not.
	if (!pDataNew)
		return TRUE;

	//  Retrieve the current state the stylus should be set to according to the 
	//  focussed application.
	STYLUS_STATE newStylusState = pDataNew->currentStylusState;

	//  Configure the new stylus state
	DWORD dwResult = 
		DevicePowerNotify(STYLUS_DEVICE_ID, 
							(CEDEVICE_POWER_STATE)newStylusState, 
							POWER_NAME);

	return (dwResult == ERROR_SUCCESS);
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		November 2009
*/
BOOL CStylusModule::MetaProc(PBMetaStruct *pbMetaStructure, PPBSTRUCT pPBStructure, void *pParam)
{
	INSTANCE_DATA *pDataNew = (INSTANCE_DATA*) pParam;
	DWORD dwResult;
	if (cmp (pbMetaStructure->lpParameter, L"Enabled") ||
		cmp (pbMetaStructure->lpParameter, L"Enable"))
	{
		//  Enable the Stylus
		pDataNew->currentStylusState = STYLUS_ENABLED;
		dwResult = DevicePowerNotify(STYLUS_DEVICE_ID, 
							(CEDEVICE_POWER_STATE)pDataNew->currentStylusState, 
							POWER_NAME);
		Log(PB_LOG_INFO, L"Enabling the Stylus", _T(__FUNCTION__), __LINE__);
	}
	else if (cmp(pbMetaStructure->lpParameter, L"Disabled") || cmp(pbMetaStructure->lpParameter, L"Disable"))
	{
		//  Disable the Stylus
		
		// disable() method of stylus module in some of the  CE device is not working.
		//  Fixed Disable Stylus Issue for WM device
		OSVERSIONINFO osv;
		::ZeroMemory(&osv,sizeof(OSVERSIONINFO));
		osv.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
		BOOL retVal = GetVersionEx(&osv);
		if(TRUE == retVal)
		{
			//Try first with D4
			pDataNew->currentStylusState = STYLUS_DISABLED;
			dwResult = DevicePowerNotify(STYLUS_DEVICE_ID, 
						(CEDEVICE_POWER_STATE)pDataNew->currentStylusState, 
						POWER_NAME);
			
			//Try next time with D3
			pDataNew->currentStylusState = STYLUS_DISABLED_D3;
			dwResult = DevicePowerNotify(STYLUS_DEVICE_ID, 
						(CEDEVICE_POWER_STATE)pDataNew->currentStylusState, 
						POWER_NAME);
			Log(PB_LOG_INFO, L"Disabling the Stylus", _T(__FUNCTION__), __LINE__);			
		}
		else
		{
			// Unable to get CE version using system call, log it
			Log(PB_LOG_ERROR, L"Unable to get the CE version using system call",
			_T(__FUNCTION__), __LINE__);
		
		}
		
		
	}
	else
	{
		// Unrecognised tag
		Log(PB_LOG_WARNING, L"Unrecognised Meta Tag Provided to Stylus Module",
			_T(__FUNCTION__), __LINE__);
		return FALSE;
	}

	return (dwResult == ERROR_SUCCESS);
}
