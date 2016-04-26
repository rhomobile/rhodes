#include "NotificationModule.h"

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		November 2009
*/
BOOL CNotificationModule::onInit(PPBSTRUCT pPBStructure)
{
	wcscpy (m_szModName, L"Notification");
	RegisterForEvent(PB_BROWSER_BEFORE_NAV_EVENT);
	RegisterForEvent(RHO_APPFOCUS_EVENT);
	return TRUE;
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		November 2009
*/
void CNotificationModule::onDeInit(PPBSTRUCT pPBStructure)
{
	UnRegisterForEvent(PB_APPFOCUSEVENT);
	UnRegisterForEvent(PB_BROWSER_BEFORE_NAV_EVENT);
	UnRegisterForEvent(RHO_APPFOCUS_EVENT);
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		November 2009
*/
BOOL CNotificationModule::onAttachInstance(PPBSTRUCT pPBStructure, PPBINSTSTRUCT pInstStruct)
{
	// Create new server, add instance data, store in plugin framework
	//  Read some values from the configuration

	CNotification *pNotification = 
		new CNotification(pPBStructure->iTabID, this, pPBStructure->bLaunchingAppHasFocus);

	pInstStruct->pWrappedObj = pNotification;

	return TRUE;
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		November 2009
*/
BOOL CNotificationModule::onReleaseInstance(PPBSTRUCT pPBStructure, PPBINSTSTRUCT pInstStruct)
{
	CNotification *pNotification = (CNotification*) (pInstStruct->pWrappedObj);
	
	if (pNotification)
	{
		delete pNotification;
	}

	return TRUE;
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		November 2009
*/
BOOL CNotificationModule::onBeforeNavigate (int iInstID)
{
	CNotification *pNotification = (CNotification*) GetObjFromID(iInstID);
	if (!pNotification)
		return TRUE;
	else
		return pNotification->SetNavigationURI(L"");
};
	
/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		November 2009
*/
BOOL CNotificationModule::MetaProc(PBMetaStruct *pbMetaStructure, PPBSTRUCT pPBStructure, void *pParam)
{
	Log(PB_LOG_WARNING, L"The Notification API from the RhoElements Extension has been deprecated in 4.0, please transition your applications to use the common Notification API for future releases", _T(__FUNCTION__), __LINE__);

	CNotification* pNotification = (CNotification*)pParam;

	if (cmp (pbMetaStructure->lpParameter, L"EnumNotificationsEvent"))
	{
		//  EnumDeviceNotifications is how the user specifies the URI which 
		//  will be used to enumerate the notifications available on the device.
		if (*pbMetaStructure->lpValue)
			return pNotification->SetNavigationURI(pbMetaStructure->lpValue);
		else
			return pNotification->SetNavigationURI(L"");
	}
	else if (cmp(pbMetaStructure->lpParameter, L"Enumerate"))
	{
		//  Enumerate the Notifications available on the device and return 
		//  them via the URI registered at EnumDeviceNotifications.
		return pNotification->EnumerateNotifications();
	}
	else if (cmp(pbMetaStructure->lpParameter, L"SetLEDOnDuration"))
	{
		DWORD dwValueAsDWORD = _wtoi(pbMetaStructure->lpValue);
		return pNotification->SetLEDOnDuration(dwValueAsDWORD);
	}
	else if (cmp(pbMetaStructure->lpParameter, L"SetLEDOffDuration"))
	{
		DWORD dwValueAsDWORD = _wtoi(pbMetaStructure->lpValue);
		return pNotification->SetLEDOffDuration(dwValueAsDWORD);
	}
	else if (cmp(pbMetaStructure->lpParameter, L"SetLEDNumberOfCycles"))
	{
		DWORD dwValueAsDWORD = _wtoi(pbMetaStructure->lpValue);
		return pNotification->SetLEDNumberOfCycles(dwValueAsDWORD);
	}
	else if (cmp(pbMetaStructure->lpParameter, L"SetBeeperFrequency"))
	{
		DWORD dwValueAsDWORD = _wtoi(pbMetaStructure->lpValue);
		return pNotification->SetBeeperFrequency(dwValueAsDWORD);
	}
	else if (cmp(pbMetaStructure->lpParameter, L"SetBeeperVolume"))
	{
		DWORD dwValueAsDWORD = _wtoi(pbMetaStructure->lpValue);
		return pNotification->SetBeeperVolume(dwValueAsDWORD);
	}
	else if (cmp(pbMetaStructure->lpParameter, L"SetBeeperDuration"))
	{
		DWORD dwValueAsDWORD = _wtoi(pbMetaStructure->lpValue);
		return pNotification->SetBeeperDuration(dwValueAsDWORD);
	}
	else if (cmp(pbMetaStructure->lpParameter, L"SetVibrateDuration"))
	{
		DWORD dwValueAsDWORD = _wtoi(pbMetaStructure->lpValue);
		return pNotification->SetVibrateDuration(dwValueAsDWORD);
	}
	else if (cmp(pbMetaStructure->lpParameter, L"StateOn"))
	{
		DWORD dwValueAsDWORD = _wtoi(pbMetaStructure->lpValue);
		return pNotification->SetOn(dwValueAsDWORD);
	}
	else if (cmp(pbMetaStructure->lpParameter, L"StateOff"))
	{
		DWORD dwValueAsDWORD = _wtoi(pbMetaStructure->lpValue);
		return pNotification->SetOff(dwValueAsDWORD);
	}
	else if (cmp(pbMetaStructure->lpParameter, L"StateCycle"))
	{
		DWORD dwValueAsDWORD = _wtoi(pbMetaStructure->lpValue);
		return pNotification->SetCycle(dwValueAsDWORD);
	}
	else
	{
		// Unrecognised tag
		Log(PB_LOG_WARNING, L"Unrecognised Meta Tag Provided to Notification Module",
			_T(__FUNCTION__), __LINE__);
		return FALSE;
	}

	return TRUE;
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		November 2009
*/
BOOL CNotificationModule::ParseParams(int iInstanceID, LPCTSTR tcStringToParse, DWORD* dwNotificationType, 
	DWORD* dwNotificationIndex, DWORD* dwParameter1, DWORD* dwParameter2, DWORD* dwParameter3)
{
	CNotification *pNotification = (CNotification*) GetObjFromID(iInstanceID);
	if (pNotification)
		return pNotification->ParseParams(tcStringToParse, dwNotificationType, 
											dwNotificationIndex, dwParameter1, 
											dwParameter2, dwParameter3);
	else
		return FALSE;
}

/**
* \author	John Heywood (JH, VBC863)
* \date		September 2012
*/
BOOL CNotificationModule::onRhoAppFocus(bool bActivate, int iInstID)
{
	CNotification *pNot = NULL;
	pNot = (CNotification*) GetObjFromID(iInstID);
	if (pNot != NULL)
	{
		return pNot->ApplicationFocusChange(bActivate);
	}
	else
		return FALSE;
};
