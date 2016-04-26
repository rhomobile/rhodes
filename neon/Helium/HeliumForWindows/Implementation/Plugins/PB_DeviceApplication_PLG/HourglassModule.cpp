#include "HourglassModule.h"

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		November 2009
*/
BOOL CHourglassModule::onInit(PPBSTRUCT pPBStructure)
{
	wcscpy (m_szModName, L"Hourglass");
	m_hWindowInitialisedEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	RegisterForEvent(PB_APPFOCUSEVENT);
	RegisterForEvent(PB_BROWSER_BEFORE_NAV_EVENT);
	RegisterForEvent(PB_BROWSER_DOC_COMPLETE_EVENT);
	RegisterForEvent(RHO_APPFOCUS_EVENT);
	return TRUE;
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		November 2009
*/
void CHourglassModule::onDeInit(PPBSTRUCT pPBStructure)
{
	UnRegisterForEvent(PB_APPFOCUSEVENT);
	UnRegisterForEvent(PB_BROWSER_BEFORE_NAV_EVENT);
	UnRegisterForEvent(PB_BROWSER_DOC_COMPLETE_EVENT);
	UnRegisterForEvent(RHO_APPFOCUS_EVENT);
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		November 2009
*/
BOOL CHourglassModule::onAttachInstance(PPBSTRUCT pPBStructure, PPBINSTSTRUCT pInstStruct)
{
	// Create new server, add instance data, store in plugin framework
	//  Read some values from the configuration

	//  Read the value of 'WaitCursorEnabled' from the configuration
	BOOL bWaitCursorConfigSetting;
	if (!(RetrieveNoWaitConfigValue(pInstStruct->instID, &bWaitCursorConfigSetting)))
		bWaitCursorConfigSetting = TRUE;

	//  Read the value of 'WaitCursorLeft' from the configuration
	DWORD dwHourglassDefaultLeftPosition;
	if (!(RetrieveHourglassDefaultLeft(pInstStruct->instID, &dwHourglassDefaultLeftPosition)))
		dwHourglassDefaultLeftPosition = (unsigned long) (GetSystemMetrics(SM_CXSCREEN)/2)-(ANIMATIONWIDTH/2);

	//  Read the value of 'WaitCursorTop' from the configuration
	DWORD dwHourglassDefaultTopPosition;
	if (!(RetrieveHourglassDefaultTop(pInstStruct->instID, &dwHourglassDefaultTopPosition)))
		dwHourglassDefaultTopPosition = (unsigned long) (GetSystemMetrics(SM_CYSCREEN)/2)-(ANIMATIONHEIGHT/2);

	if (bWaitCursorConfigSetting)
	{
		CHourglassAnimation *pHourglass = 
			new CHourglassAnimation(pPBStructure->hInstance, 
									pPBStructure->hWnd,
									m_hWindowInitialisedEvent,
									20,
									dwHourglassDefaultLeftPosition, 
									dwHourglassDefaultTopPosition);

		pInstStruct->pWrappedObj = pHourglass;

		//  Wait for the Animation window to be successfully created
		WaitForSingleObject(m_hWindowInitialisedEvent, 5000);
	}
	return TRUE;
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		November 2009
*/
BOOL CHourglassModule::onReleaseInstance(PPBSTRUCT pPBStructure, PPBINSTSTRUCT pInstStruct)
{
	CHourglassAnimation *pHourglass = (CHourglassAnimation*) (pInstStruct->pWrappedObj);
	
	if (pHourglass)
	{
		delete pHourglass;
	}

	return TRUE;
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		October 2009
*/
BOOL CHourglassModule::onAppFocus(int iOldID, int iNewID)
{
	//  Consider Initialisation Steps
	if ((iOldID == iNewID) ||
		(iOldID < 0) ||
		(iNewID < 0))
		return TRUE;

	
	CHourglassAnimation *pHourglassOld = (CHourglassAnimation*) GetObjFromID(iOldID);
	CHourglassAnimation *pHourglassNew = (CHourglassAnimation*) GetObjFromID(iNewID);

	//  If either the old or the new applications make use of the Hourglass 
	//  set their focus as appropriate
	if (pHourglassOld)
		pHourglassOld->SetFocus(FALSE);

	if (pHourglassNew)
		pHourglassNew->SetFocus(TRUE);

	return TRUE;
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		November 2009
*/
BOOL CHourglassModule::onBeforeNavigate (int iInstID)
{
	CHourglassAnimation *pHourglass = (CHourglassAnimation*) GetObjFromID(iInstID);
	if (!pHourglass)
		return TRUE;
	else
		return pHourglass->RequestToShow(TRUE);
};
	
/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		November 2009
*/
BOOL CHourglassModule::onDocumentComplete (int iInstID)
{
	CHourglassAnimation *pHourglass = (CHourglassAnimation*) GetObjFromID(iInstID);
	if (!pHourglass)
		return TRUE;
	else
		return pHourglass->RequestToShow(FALSE);
};


/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		November 2009
*/
BOOL CHourglassModule::MetaProc(PBMetaStruct *pbMetaStructure, PPBSTRUCT pPBStructure, void *pParam)
{
	CHourglassAnimation *pHourglass = (CHourglassAnimation*) (pParam);
	if (!pHourglass)
		return FALSE;

	if (cmp (pbMetaStructure->lpParameter, L"Visibility"))
	{
		if (cmp (pbMetaStructure->lpValue, L"Visible"))
		{
			//  Show the Hourglass
			return pHourglass->RequestToShow(TRUE);
		}
		else if (cmp(pbMetaStructure->lpValue, L"Hidden"))
		{
			//  Hide the Hourglass
			return pHourglass->RequestToShow(FALSE);
		}
	}
	else if (cmp (pbMetaStructure->lpParameter, L"Left"))
	{
		//  Move the Hourglass horizontally
		int iMetaValueAsInt = _wtoi(pbMetaStructure->lpValue);
		return pHourglass->MoveHourglassLeft(iMetaValueAsInt);
	}
	else if (cmp (pbMetaStructure->lpParameter, L"Top"))
	{
		int iMetaValueAsInt = _wtoi(pbMetaStructure->lpValue);
		return pHourglass->MoveHourglassTop(iMetaValueAsInt);
	}
	else
	{
		// Unrecognised tag
		Log(PB_LOG_WARNING, L"Unrecognised Meta Tag Provided to Hourglass Module",
			_T(__FUNCTION__), __LINE__);
		return FALSE;
	}

	return TRUE;
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		February 2010
*/
bool CHourglassModule::RetrieveNoWaitConfigValue(int iInstance, BOOL* value) 
{
	//  Read the value of WaitCursorEnabled and return in the value parameter
	//  Previously: 'No Wait Cursor' registry entry in PB2.x
	bool retVal = false;
	LPCTSTR pVal;
	pVal = GetAppSettingPtr(iInstance,L"GUI\\HourglassEnabled",L"Value");
	if(pVal != NULL)
	{
		if (*pVal==L'0')
			*value = FALSE;
		else
			*value = TRUE;
		retVal = true;
	}
	else
	{
		Log(PB_LOG_WARNING,
			L"Unable to retrieve WaitCursorEnabled from Configuration", 
			_T(__FUNCTION__), __LINE__);	
		
	}
	return retVal;
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		February 2010
*/
bool CHourglassModule::RetrieveHourglassDefaultLeft(int iInstance, DWORD* value) 
{
	//  Read the value of WaitCursorLeft and return in the value parameter
	//  Previously: 'Software\\Symbol\\SymbolPB\\CursorWindowPos\\X' registry
	//  entry in PB2.X
	bool retVal = false;
	LPCTSTR pVal;
	pVal = GetAppSettingPtr(iInstance,L"GUI\\HourglassLeft",L"Value");
	//we actually have three situations to handle
	//1:GetAppSettingPtr fail; 2.returned value success; 3.returned value not set in config file
	if(pVal != NULL)
	{
		if(*pVal != L'\0'){
			//returned value success
			*value = _wtoi(pVal);
			retVal = true;
		}
		//else no value has been set in the config file
	}
	else
	{
		Log(PB_LOG_ERROR, 
			L"Unable to retrieve HourglassLeft from Configuration", 
			_T(__FUNCTION__), __LINE__);	
		
	}
	return retVal;
};

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		February 2010
*/
bool CHourglassModule::RetrieveHourglassDefaultTop(int iInstance, DWORD* value) 
{
	//  Read the value of WaitCursorTop and return in the value parameter
	//  Previously: 'Software\\Symbol\\SymbolPB\\CursorWindowPos\\Y' registry
	//  entry in PB2.x
	bool retVal = false;
	LPCTSTR pVal;
	pVal = GetAppSettingPtr(iInstance,L"GUI\\HourglassTop",L"Value");
	if(pVal != NULL)
	{
		if(*pVal != L'\0'){
			//returned value success
			*value = _wtoi(pVal);
			retVal = true;
		}
		//else no value has been set in the config file
	}
	else
	{
		Log(PB_LOG_ERROR, 
			L"Unable to retrieve HourglassTop from Configuration", 
			_T(__FUNCTION__), __LINE__);	
		
	}
	return retVal;
};

/**
* \author	John Heywood (JH, VBC863)
* \date		September 2012
*/
BOOL CHourglassModule::onRhoAppFocus(bool bActivate, int iInstID)
{
	CHourglassAnimation *pHourglass = NULL;
	pHourglass = (CHourglassAnimation*) GetObjFromID(iInstID);
	if (pHourglass != NULL)
	{
		return pHourglass->SetFocus((BOOL)bActivate);
	}
	else
		return FALSE;
};
