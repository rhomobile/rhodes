#include "Notification.h"
#include "ntfycapi.h"

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		November 2009 (Initial Creation)
*/
CNotification::CNotification(int iInstanceID, PBModule* pModule, bool bHasFocus)
	:m_pModule(pModule)
	,m_iInstanceID(iInstanceID)
	,m_bAppHasFocus(bHasFocus)
{
	//  Initialise the LED, Beeper and Vibrator Attributes
	m_LEDCycleAttributes.dwOnDuration = 1000;
	m_LEDCycleAttributes.dwOffDuration = 1000;
	m_LEDCycleAttributes.dwCount = 1;
	m_BeeperAttributes.dwFrequency = 2000;
	m_BeeperAttributes.dwVolume = 0;
	m_BeeperAttributes.dwDuration = 1000;
	m_PagerAttributes.dwDuration = 1000;
	memset(m_tcNavigationURI, 0, MAX_URL);
	memset(m_tcNotificationArray, 0, MAX_URL);
	m_pNotifications = new CNotificationLoader();
	if (m_pNotifications && m_pNotifications->LoadNotificationDLL())
		m_bNotificationsLoaded = true;
	else 
		m_bNotificationsLoaded = false;

	m_pCycleInfoStore = NULL;
	m_pStateInfoStore = NULL;
	// count up the number of notification objects for use when we lose focus
	m_iNotificationObjCount = 0;
	if (m_bNotificationsLoaded)
	{
		NOTIFY_FINDINFO notifyObject;
		HANDLE	hFindHandle = NULL;
		SI_ALLOC_ALL(&notifyObject);
		notifyObject.StructInfo.dwUsed = 0;

		if(m_pNotifications->lpfn_Notify_FindFirst(&notifyObject, &hFindHandle) == E_NTFY_SUCCESS)
		{
			do
			{
				m_iNotificationObjCount++;
				SI_ALLOC_ALL(&notifyObject);
				notifyObject.StructInfo.dwUsed = 0;
			} while(m_pNotifications->lpfn_Notify_FindNext(&notifyObject, hFindHandle) == E_NTFY_SUCCESS); 
		}
		m_pNotifications->lpfn_Notify_FindClose(hFindHandle);
	}
}

CNotification::~CNotification()
{
	delete [] m_pCycleInfoStore;
	delete [] m_pStateInfoStore;
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		November 2009 (Initial Creation)
*/
BOOL CNotification::EnumerateNotifications()
{
	if (m_bNotificationsLoaded)
	{
		//  Enumerate the Notifications on the device
		NOTIFY_FINDINFO notifyObject;
		HANDLE	hFindHandle = NULL;

		//  Temporary string variables to build the Array of notifications on the 
		//  device
		TCHAR urlTemp[MAX_URL];
		TCHAR urlTempCat[MAX_URL];
		memset(urlTemp, 0, MAX_URL * sizeof(TCHAR));
		memset(urlTempCat, 0, MAX_URL * sizeof(TCHAR));

		//  Format of JavaScript Array is:
		//  new Array(
		//            new Array(notID, notType, notName), 
		//            new Array(notID, notType, notName), 
		//            new Array(notID, notType, notName)
		//           )
		//  notID is the identifier for the notification, numbered 0 upwards.
		//  notType is an integer of type tagNOTIFY_OBJECT_TYPES
		//  notName is a User friendly name for the notification, e.g. Green LED
		wcscpy(urlTempCat, L"new Array(");
		SI_ALLOC_ALL(&notifyObject);
		notifyObject.StructInfo.dwUsed = 0;
		TCHAR tcLogInfo[1024];
		TCHAR tcLogType[32];
		//  Loop through the notifications on the device and build the array
		if(m_pNotifications->lpfn_Notify_FindFirst(&notifyObject, &hFindHandle) == E_NTFY_SUCCESS)
		{
			int iObCount = 0;
			do 
			{
				wsprintf(urlTemp, L"new Array(%d, '%d', '%s'),", iObCount, 
							notifyObject.dwObjectType, notifyObject.szObjectName);
				wcscat(urlTempCat, urlTemp);

				//  Log that the Object has been found
				switch(notifyObject.dwObjectType)
				{
				case NOTIFY_TYPE_LED:
					wsprintf(tcLogType, L"LED");
					break;
				case NOTIFY_TYPE_BEEPER:
					wsprintf(tcLogType, L"Beeper");
					break;
				case NOTIFY_TYPE_VIBRATOR:
					wsprintf(tcLogType, L"Pager");
					break;
				case NOTIFY_TYPE_UNKNOWN: 
				default:
					wsprintf(tcLogType, L"Unknown");
					break;
				}
				wsprintf(tcLogInfo, L"Found Notification Object: (%i) %s [%s]", iObCount, tcLogType, notifyObject.szObjectName);
				m_pModule->Log(PB_LOG_INFO, tcLogInfo, _T(__FUNCTION__), __LINE__);

				iObCount++;
				SI_ALLOC_ALL(&notifyObject);
				notifyObject.StructInfo.dwUsed = 0;
							
			} while(m_pNotifications->lpfn_Notify_FindNext(&notifyObject, hFindHandle) == E_NTFY_SUCCESS);
		}
		urlTempCat[wcslen(urlTempCat)-1] = ')';
		m_pNotifications->lpfn_Notify_FindClose(hFindHandle);

		//  Send the built array to the caller
		if (*m_tcNavigationURI != NULL)
		{
			wcsncpy(m_tcNotificationArray, urlTempCat, MAX_URL);
			CloseHandle(CreateThread (NULL, 0, SendNavigateURI, this, 0, NULL));
		}
	}
	else
	{
		m_pModule->Log(PB_LOG_WARNING, L"Unable to load Notifications API", _T(__FUNCTION__), __LINE__);
	}
	return TRUE;
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		January 2010 (Initial Creation)
*/
DWORD CNotification::SendNavigateURI (LPVOID pparam)
{
	//  Parameter is a pointer to the notification object
	CNotification* pthis = (CNotification*) pparam;
	pthis->m_pModule->SendPBNavigate(tcEnumNotificationsEventNames,
		pthis->m_iInstanceID, 
		pthis->m_tcNavigationURI, 
		pthis->m_tcNotificationArray, NULL);
	return 0;
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		November 2009 (Initial Creation)
*/
BOOL CNotification::SetLEDOnDuration (DWORD dwOnDuration)
{
	m_LEDCycleAttributes.dwOnDuration = dwOnDuration;
	return TRUE;
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		November 2009 (Initial Creation)
*/
BOOL CNotification::SetLEDOffDuration (DWORD dwOffDuration)
{
	m_LEDCycleAttributes.dwOffDuration = dwOffDuration;
	return TRUE;
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		November 2009 (Initial Creation)
*/
BOOL CNotification::SetLEDNumberOfCycles (DWORD dwNumberOfCycles)
{
	m_LEDCycleAttributes.dwCount = dwNumberOfCycles;
	return TRUE;
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		November 2009 (Initial Creation)
*/
BOOL CNotification::SetBeeperFrequency (DWORD dwBeeperFrequency)
{
	m_BeeperAttributes.dwFrequency = dwBeeperFrequency;
	return TRUE;
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		November 2009 (Initial Creation)
*/
BOOL CNotification::SetBeeperVolume (DWORD dwBeeperVolume)
{
	m_BeeperAttributes.dwVolume = dwBeeperVolume;
	return TRUE;
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		November 2009 (Initial Creation)
*/
BOOL CNotification::SetBeeperDuration (DWORD dwBeeperDuration)
{
	m_BeeperAttributes.dwDuration = dwBeeperDuration;
	return TRUE;
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		November 2009 (Initial Creation)
*/
BOOL CNotification::SetVibrateDuration (DWORD dwVibrateDuration)
{
	m_PagerAttributes.dwDuration = dwVibrateDuration;
	return TRUE;
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		November 2009 (Initial Creation)
*/
BOOL CNotification::SetOn (DWORD dwNotificationIndex)
{
	if (m_bNotificationsLoaded)
	{
		//  Update the Cycle Info to update the Beeper Frequency / Volume.  This 
		//  is the only way to set the beeper attributes at the EMDK.
		CYCLE_INFO cycleInfo;
		SI_INIT(&cycleInfo);
		DWORD dwResult = m_pNotifications->lpfn_Notify_GetCycleInfo(dwNotificationIndex, &cycleInfo);
		if(dwResult != E_NTFY_SUCCESS)
			return FALSE;
		//  Test to see if the object is a beeper and update the attributes at the 
		//  EMDK with the last set PB attributes if it is
		if (cycleInfo.dwObjectType == NOTIFY_TYPE_BEEPER)
		{
			SetAttributes(dwNotificationIndex);
		}

		//  Finished Beeper specific setting, this code is common to all 
		//  notification types, LED, beeper and Pager
		if (m_bAppHasFocus)
		{
			dwResult = m_pNotifications->lpfn_Notify_SetState(dwNotificationIndex, NOTIFY_STATE_ON);
			if (dwResult == E_NTFY_SUCCESS)
				return TRUE;
			else
				return FALSE;
		}
		else
		{
			if (m_pStateInfoStore)
			{
				m_pStateInfoStore[dwNotificationIndex] = NOTIFY_STATE_ON;
				return TRUE;
			}
			return FALSE;
		}
	}
	else
	{
		m_pModule->Log(PB_LOG_WARNING, L"Unable to load Notifications API", _T(__FUNCTION__), __LINE__);
		return FALSE;
	}
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		November 2009 (Initial Creation)
*/
BOOL CNotification::SetOff (DWORD dwNotificationIndex)
{
	//  Common to all notification types, LED, beeper and paer.
	DWORD dwResult = E_NTFY_INVALIDHANDLE;
	if (m_bNotificationsLoaded)
	{
		if (m_bAppHasFocus)
		{
			dwResult = m_pNotifications->lpfn_Notify_SetState(dwNotificationIndex, NOTIFY_STATE_OFF);
			if (dwResult == E_NTFY_SUCCESS)
				return TRUE;
		}
		else
		{
			// update the stored value
			if (m_pStateInfoStore)
			{
				m_pStateInfoStore[dwNotificationIndex] = NOTIFY_STATE_OFF;
				return TRUE;
			}
		}
	}
	return FALSE;
}


/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		November 2009 (Initial Creation)
*/
BOOL CNotification::SetCycle (DWORD dwNotificationIndex)
{
	//  Update the Cycle attributes at the EMDK with those last received from 
	//  the application.
	BOOL bSuccess = SetAttributes(dwNotificationIndex);
	if (!bSuccess)
		return FALSE;

	if (m_bAppHasFocus)
	{
		//  Start cycling the specified notification index.
		DWORD dwResult = E_NTFY_INVALIDHANDLE;
		if (m_bNotificationsLoaded)
			dwResult = m_pNotifications->lpfn_Notify_SetState(dwNotificationIndex, NOTIFY_STATE_CYCLE);  
		if(dwResult != E_NTFY_SUCCESS)
			return FALSE;
		else
			return TRUE;
	}
	else
	{
		// update the stored value
		if (m_pStateInfoStore)
		{
			m_pStateInfoStore[dwNotificationIndex] = NOTIFY_STATE_CYCLE;
			return TRUE;
		}
		return FALSE;
	}
}


/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		November 2009 (Initial Creation)
*/
BOOL CNotification::SetNavigationURI(LPCWSTR szNavURI)
{
	//  store the passed navigation URI
	if (wcslen(szNavURI) < MAX_URL)
	{
		memset(m_tcNavigationURI, 0, MAX_URL);
		wcscpy(m_tcNavigationURI, szNavURI);
		return TRUE;
	}
	else
	{
		return FALSE;
	}	
}


/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		November 2009 (Initial Creation)
*/
BOOL CNotification::ParseParams(LPCTSTR tcStringToParse, DWORD* dwNotificationType, 
		DWORD* dwNotificationIndex, DWORD* dwParameter1, DWORD* dwParameter2, DWORD* dwParameter3)
{
	if (!m_bAppHasFocus)
		// ignore all immediate InvokeNotifications if we're not the foreground app
		return FALSE;

	if (m_bNotificationsLoaded)
	{
		//  This function is only used by the InvokeNotification Module to 
		//  convert PB2.x parameters to PB3 parameters.
		DWORD dwResult = 0;
		TCHAR tcChunk[50];
		DWORD dwPos;
		TCHAR Data[MAX_URL];
		wcscpy(Data, tcStringToParse);
		TCHAR* pData = Data;
		
		CYCLE_INFO cycleInfo;
		SI_INIT(&cycleInfo);

		//find the first paramenter, the notification index
		for(dwPos=0; dwPos<wcslen(pData); dwPos++)
		{
			if(pData[dwPos] == ',')
				break;
		}
		memset(tcChunk, 0, 100);
		wcsncpy(tcChunk, pData, dwPos);
		pData+=dwPos+1;
		DWORD dwIndex = _wtoi(tcChunk);


		//if they supplied some paramterers then get and set them
		if(pData[0] != NULL)
		{
			*dwNotificationIndex = dwIndex;
			//get the cycle info for that 
			dwResult = m_pNotifications->lpfn_Notify_GetCycleInfo(dwIndex, &cycleInfo);
			if(dwResult != E_NTFY_SUCCESS)
				return FALSE;

			switch(cycleInfo.dwObjectType)
			{
			case NOTIFY_TYPE_LED:
				*dwNotificationType = NOTIFY_TYPE_LED;
				break;
			case NOTIFY_TYPE_BEEPER:
				*dwNotificationType = NOTIFY_TYPE_BEEPER;
				break;
			case NOTIFY_TYPE_VIBRATOR:
				*dwNotificationType = NOTIFY_TYPE_VIBRATOR;
				break;
			case NOTIFY_TYPE_UNKNOWN: 
				*dwNotificationType = NOTIFY_TYPE_UNKNOWN;
			default:
				return FALSE;
			}

			//find the next parameter
			for(dwPos=0; dwPos<wcslen(pData); dwPos++){
				if(pData[dwPos] == ',')
					break;
			}
			memset(tcChunk, 0, 100);
			wcsncpy(tcChunk, pData, dwPos);
			pData+=dwPos+1;
			*dwParameter1 = _wtoi(tcChunk);
			if(pData[0] == NULL) 
				return TRUE;

			//find the next parameter
			for(dwPos=0; dwPos<wcslen(pData); dwPos++){
				if(pData[dwPos] == ',')
					break;
			}
			memset(tcChunk, 0, 100);
			wcsncpy(tcChunk, pData, dwPos);
			pData+=dwPos+1;
			*dwParameter2 = _wtoi(tcChunk);
			if(pData[0] == NULL) 
				return TRUE;

			//find the next parameter, Count
			for(dwPos=0; dwPos<wcslen(pData); dwPos++){
				if(pData[dwPos] == ',')
					break;
			}
			memset(tcChunk, 0, 100);
			wcsncpy(tcChunk, pData, dwPos);
			*dwParameter3 = _wtoi(tcChunk);
		}
		else
			return FALSE;
	}
	else
		return FALSE;
	
	return TRUE;
}


//////////////////////////////////////
//									//
//       Private Functions			//
//									//
//////////////////////////////////////

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		November 2009 (Initial Creation)
*/
BOOL CNotification::SetAttributes (DWORD dwNotificationIndex)
{
	if (!m_bNotificationsLoaded)
		return FALSE;

	//  Retrieve the cycle information for the specified Notification Index
	CYCLE_INFO cycleInfo;
	SI_INIT(&cycleInfo);
	DWORD dwResult = m_pNotifications->lpfn_Notify_GetCycleInfo(dwNotificationIndex, &cycleInfo);
	if(dwResult != E_NTFY_SUCCESS)
		return FALSE;

	//  Update the cycleInfo object with the last received values from the 
	//  application
	switch(cycleInfo.dwObjectType)
	{
	case NOTIFY_TYPE_LED:
		//  Cycle an LED, copy the LED Parameters last set by the user
		cycleInfo.ObjectTypeSpecific.LedSpecific.dwOnDuration = 
			m_LEDCycleAttributes.dwOnDuration;
		cycleInfo.ObjectTypeSpecific.LedSpecific.dwOffDuration = 
			m_LEDCycleAttributes.dwOffDuration;
		cycleInfo.ObjectTypeSpecific.LedSpecific.dwCount = 
			m_LEDCycleAttributes.dwCount;
		break;
	case NOTIFY_TYPE_BEEPER:
		cycleInfo.ObjectTypeSpecific.BeeperSpecific.dwDuration = 
			m_BeeperAttributes.dwDuration;
		cycleInfo.ObjectTypeSpecific.BeeperSpecific.dwFrequency = 
			m_BeeperAttributes.dwFrequency;
		cycleInfo.ObjectTypeSpecific.BeeperSpecific.dwVolume =
			m_BeeperAttributes.dwVolume;
		break;
	case NOTIFY_TYPE_VIBRATOR:
		cycleInfo.ObjectTypeSpecific.VibratorSpecific.dwDuration = 
			m_PagerAttributes.dwDuration;
		break;
	case NOTIFY_TYPE_UNKNOWN: 
	default:
		//do nothing
		break;
	}

	if (m_bAppHasFocus)
	{
		//now set the new parameters
		dwResult = m_pNotifications->lpfn_Notify_SetCycleInfo(dwNotificationIndex, &cycleInfo);
		if(dwResult != E_NTFY_SUCCESS)
			return FALSE;
		else
			return TRUE;
	}
	else
	{
		// update the stored parameters
		if (m_pCycleInfoStore)
		{
			memcpy(&m_pCycleInfoStore[dwNotificationIndex], &cycleInfo, sizeof(CYCLE_INFO));
			return TRUE;
		}
		return FALSE;
	}
}

/**
* \author	John Heywood (JH, VBC863)
* \date		October 2012 (Initial Creation)
*/
BOOL CNotification::ApplicationFocusChange(bool bActivated)
{
	if (bActivated == m_bAppHasFocus)
		// nothing to do
		return TRUE;

	m_bAppHasFocus = bActivated;

	if (m_iNotificationObjCount > 0)
	{
		if (!bActivated)
		{
			// App has lost focus
			m_pCycleInfoStore = new CYCLE_INFO[m_iNotificationObjCount];
			m_pStateInfoStore = new DWORD[m_iNotificationObjCount];
			if (!m_pCycleInfoStore || !m_pStateInfoStore)
			{
				delete [] m_pCycleInfoStore;
				m_pCycleInfoStore = NULL;
				delete [] m_pStateInfoStore;
				m_pStateInfoStore = NULL;
				return FALSE;
			}
			ZeroMemory(m_pCycleInfoStore, sizeof(CYCLE_INFO) * m_iNotificationObjCount);
			ZeroMemory(m_pStateInfoStore, sizeof(DWORD) * m_iNotificationObjCount);
			for (int i=0; i<m_iNotificationObjCount; i++)
			{
				// store each object's current notification info and state
		        SI_ALLOC_ALL(&m_pCycleInfoStore[i]);
				if (m_pNotifications->lpfn_Notify_GetCycleInfo(i, &m_pCycleInfoStore[i]) != E_NTFY_SUCCESS)
				{
					// we failed to retrieve the cycle info - this Obj may not support it
					// so zero out the entry
					memset(&m_pCycleInfoStore[i], 0, sizeof(CYCLE_INFO));
				}
				if (m_pNotifications->lpfn_Notify_GetState(i, &m_pStateInfoStore[i]) != E_NTFY_SUCCESS)
				{
					// we failed to retrieve the current state so default to OFF
					m_pStateInfoStore[i] = NOTIFY_STATE_OFF;
				}
				// turn each notification object off
				m_pNotifications->lpfn_Notify_SetState(i, NOTIFY_STATE_OFF);
			}
			return TRUE;
		}
		else
		{
			// App has gained focus
			if (!m_pCycleInfoStore || !m_pStateInfoStore)
				return FALSE;

			DWORD dwResult = E_NTFY_SUCCESS;
			for (int i=0; i<m_iNotificationObjCount; i++)
			{
				// restore each notification objects info and state
				m_pNotifications->lpfn_Notify_SetCycleInfo(i, &m_pCycleInfoStore[i]);
				dwResult |= m_pNotifications->lpfn_Notify_SetState(i, m_pStateInfoStore[i]);
			}
			delete [] m_pCycleInfoStore;
			m_pCycleInfoStore = NULL;
			delete [] m_pStateInfoStore;
			m_pStateInfoStore = NULL;
			return (dwResult == E_NTFY_SUCCESS);
		}
	}
	return FALSE;
}
