#include "Alarm.h"
#include "Notify.h"

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		November 2009 (DCC: Initial Creation, Modified from PB2.x Alarm functionality)
*/
CAlarm::CAlarm(int iInstanceID, PBModule* pModule)
{
	//  Initialise Variables and Create Events
	m_pModule		= pModule;
	m_iInstanceID	= iInstanceID;
	m_hAlarmTriggeredEvent = NULL;
	m_hAlarmCancelledEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	wsprintf(m_tcNamedEvent, L"%s_%i\0", L"PB_ALARM_EVENT", m_iInstanceID);
	m_hAlarmTriggeredEvent = CreateEvent(NULL, FALSE, FALSE, m_tcNamedEvent);
	m_bListenerThreadRunning = FALSE;
	m_AlarmInfo.bRepeat = FALSE;
	m_AlarmInfo.dwHHInterval = 0;
	m_AlarmInfo.dwMMInterval = 0;
	m_AlarmInfo.dwSSInterval = 0;
	m_AlarmInfo.stTime.wDay = 0;
	m_AlarmInfo.stTime.wDayOfWeek = 0;
	m_AlarmInfo.stTime.wHour = 0;
	m_AlarmInfo.stTime.wMilliseconds = 0;
	m_AlarmInfo.stTime.wMinute = 0;
	m_AlarmInfo.stTime.wMonth = 0;
	m_AlarmInfo.stTime.wSecond = 0;
	m_AlarmInfo.stTime.wYear = 0;
	m_hNotify = NULL;
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		November 2009 (DCC: Initial Creation, Modified from PB2.x Alarm functionality)
*/
CAlarm::~CAlarm()
{
	ClearAlarmNotifications();
	CloseHandle(m_hAlarmCancelledEvent);
	m_hAlarmCancelledEvent = NULL;
	CloseHandle(m_hAlarmTriggeredEvent);
	m_hAlarmTriggeredEvent = NULL;
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		November 2009 (DCC: Initial Creation, Modified from PB2.x Alarm functionality)
*/
void CAlarm::SetRepeat (BOOL bRepeat)
{
	m_AlarmInfo.bRepeat = bRepeat;
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		November 2009 (DCC: Initial Creation, Modified from PB2.x Alarm functionality)
*/
BOOL CAlarm::SetNavigationURI(LPCWSTR tcNavURI)
{
	//  store the passed navigation URI
	if (wcslen(tcNavURI) < MAX_URL)
	{
		memset(m_tcNavigationURI, 0, MAX_URL);
		wcsncpy(m_tcNavigationURI, tcNavURI, MAX_URL);
		return TRUE;
	}
	else
	{
		return FALSE;
	}	
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		November 2009 (DCC: Initial Creation, Modified from PB2.x Alarm functionality)
*/
BOOL CAlarm::ParseInterval (LPCTSTR tcIntervalValue)
{
	DWORD dwHours = 0;
	DWORD dwMinutes = 0;
	DWORD dwSeconds = 0;
	BOOL parseSuccess = FALSE;
	LPCTSTR tcValue = tcIntervalValue;
	if (*tcValue)
	{
		dwHours = _wtoi(tcValue);
		tcValue = wcsstr(tcValue, L"-");
		if (dwHours < 999 && tcValue != NULL)
		{
			tcValue++;
			dwMinutes = _wtoi(tcValue);
			tcValue = wcsstr(tcValue, L"-");
			if (dwMinutes < 60 &&tcValue != NULL)
			{
				tcValue++;
				dwSeconds = _wtoi(tcValue);
				if (dwSeconds < 60)
					parseSuccess = TRUE;
			}
		}
	}
	if (parseSuccess)
	{
		m_AlarmInfo.dwHHInterval = dwHours;
		m_AlarmInfo.dwMMInterval = dwMinutes;
		m_AlarmInfo.dwSSInterval = dwSeconds;
	}

	return parseSuccess;
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		November 2009 (DCC: Initial Creation, Modified from PB2.x Alarm functionality)
*/
BOOL CAlarm::ParseTime (LPCTSTR tcTimeValue)
{
	m_AlarmInfo.dwHHInterval = 0;
	m_AlarmInfo.dwMMInterval = 0;
	m_AlarmInfo.dwSSInterval = 0;

	TCHAR tcChunk[10];
	LPCTSTR pData = tcTimeValue;

	//format is UTC		yyyy-mm-ddThh-mm-ssTZD
	//example:			2005-03-22T11-56-00+01-00
	if(wcslen(pData) != 25) 
		return FALSE;

	//get the year
	memset(tcChunk, 0, 20);
	wcsncpy(tcChunk, pData, 4);
	WORD wYear = (WORD)_wtol(tcChunk);
	if (wYear < 1970 || wYear > 3000)
		return FALSE;
	m_AlarmInfo.stTime.wYear = wYear;
	pData+=5;
		
	//get the month
	memset(tcChunk, 0, 20);
	wcsncpy(tcChunk, pData, 2);
	WORD wMonth = (WORD)_wtoi(tcChunk);
	if (wMonth < 1 || wMonth > 12)
		return FALSE;
	m_AlarmInfo.stTime.wMonth = wMonth;
	pData+=3;

	//get the day
	memset(tcChunk, 0, 20);
	wcsncpy(tcChunk, pData, 2);
	WORD wDay = (WORD)_wtoi(tcChunk);
	if (wDay < 1 || wDay > 31)
		return FALSE;
	m_AlarmInfo.stTime.wDay = wDay;
	pData+=3;

	//get the hour
	memset(tcChunk, 0, 20);
	wcsncpy(tcChunk, pData, 2);
	WORD wHour = (WORD)_wtoi(tcChunk);
	if (wHour > 24)
		return FALSE;
	m_AlarmInfo.stTime.wHour = wHour;
	pData+=3;

	//get the minute
	memset(tcChunk, 0, 20);
	wcsncpy(tcChunk, pData, 2);
	WORD wMinute = (WORD)_wtoi(tcChunk);
	if (wMinute < 0 || wMinute > 59)
		return FALSE;
	m_AlarmInfo.stTime.wMinute = wMinute;
	pData+=3;

	//get the second
	memset(tcChunk, 0, 20);
	wcsncpy(tcChunk, pData, 2);
	WORD wSecond = (WORD)_wtoi(tcChunk);
	if (wSecond < 0 || wSecond > 59)
		return FALSE;
	m_AlarmInfo.stTime.wSecond = wSecond;
	pData+=2;

	//get the TZD
	if(pData[0] == '+')
	{
		pData++;
		//offset the hours
		memset(tcChunk, 0, 20);
		wcsncpy(tcChunk, pData, 2);
		WORD wHourOffset = (WORD)_wtoi(tcChunk);
		if (wHourOffset < 0 || wHourOffset > 24)
			return FALSE;
		m_AlarmInfo.stTime.wHour += wHourOffset;
		pData+=3;

		//offset the minutes
		memset(tcChunk, 0, 20);
		wcsncpy(tcChunk, pData, 2);
		WORD wMinuteOffset = (WORD)_wtoi(tcChunk);
		if (wMinuteOffset < 0 || wMinuteOffset > 59)
			return FALSE;
		m_AlarmInfo.stTime.wMinute += wMinuteOffset;
	}
	else if (pData[0] == '-')
	{
		pData++;
		//offset the hours
		memset(tcChunk, 0, 20);
		wcsncpy(tcChunk, pData, 2);
		WORD wHourOffset = (WORD)_wtoi(tcChunk);
		if (wHourOffset < 0 || wHourOffset > 24)
			return FALSE;
		m_AlarmInfo.stTime.wHour -= wHourOffset;
		pData+=3;

		//offset the minutes
		memset(tcChunk, 0, 20);
		wcsncpy(tcChunk, pData, 2);
		WORD wMinuteOffset = (WORD)_wtoi(tcChunk);
		if (wMinuteOffset < 0 || wMinuteOffset > 59)
			return FALSE;
		m_AlarmInfo.stTime.wMinute -= wMinuteOffset;	
	}
	else
		return FALSE;

	//  Time has successfully parsed :)
	return TRUE;
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		November 2009 (DCC: Initial Creation, Modified from PB2.x Alarm functionality)
*/
BOOL CAlarm::Set()
{
	if (m_bListenerThreadRunning)
	{
		//  Unable to Set Alarm, Alarm already running
		m_pModule->Log(PB_LOG_INFO, 
			L"Unable to Set Alarm, there is already an alarm running", 
			_T(__FUNCTION__), __LINE__);
		return FALSE;
	}

	//  Create the thread to listen for the Alarm Notification
	CloseHandle(m_hAlarmListenerThread);
	m_bListenerThreadRunning = TRUE;
	m_hAlarmListenerThread = 
		CreateThread(NULL, 0, 
		(LPTHREAD_START_ROUTINE)AlarmNotificationListener, this, 0, NULL);


	SYSTEMTIME st;

	DWORD delay = ((m_AlarmInfo.dwHHInterval * 60) * 60) + 
					(m_AlarmInfo.dwMMInterval * 60) + m_AlarmInfo.dwSSInterval;
	if(delay < 30)
		delay = 30;

	if(delay && !(m_AlarmInfo.dwHHInterval == 0 && 
					m_AlarmInfo.dwMMInterval == 0 && 
					m_AlarmInfo.dwSSInterval == 0))
	{
		FILETIME ft;
		LONGLONG time64 = 0;

		// get current time
		GetLocalTime(&st);

		// increment by [delay] seconds
		SystemTimeToFileTime(&st, &ft);

		DEBUGMSG(TRUE, (_T("Current time\r\n%02i:%02i:%02i\r\n"), 
							st.wHour, st.wMinute, st.wSecond));

		// *** store file time to time64
		time64 = ft.dwHighDateTime;
		time64 *= 0x100000000;
		time64 |= ((LONGLONG) ft.dwLowDateTime);
		// ***

		time64 += (delay * 10000000);

		// *** store time64 back to file time
		ft.dwHighDateTime = (DWORD) (time64/0x100000000);
		ft.dwLowDateTime = (DWORD) (time64%0x100000000);
		// ***

		FileTimeToSystemTime(&ft, &st);

		DEBUGMSG(TRUE, (_T("Run at time\r\n%02i:%02i:%02i\r\n"), 
							st.wHour, st.wMinute, st.wSecond));
	}
	else
		st = m_AlarmInfo.stTime;

	TCHAR tcLogInfo[1024];
	wsprintf(tcLogInfo, L"Alarm Will Fire at: %02i:%02i:%02i", 
							st.wHour, st.wMinute, st.wSecond);
	m_pModule->Log(PB_LOG_INFO, tcLogInfo, _T(__FUNCTION__), __LINE__);


	// register app to run
	UserNotificationTrigger trigger;
	UserNotificationType notification;

	ZeroMemory (&trigger, sizeof (UserNotificationTrigger)) ;
	ZeroMemory (&notification, sizeof (UserNotificationType)) ;

	trigger.dwSize = sizeof(CE_NOTIFICATION_TRIGGER);
	trigger.dwType = CNT_TIME;

	//  Named event to be triggered when the Alarm Fires
	TCHAR tcNamedEventFormatted[50];
	wsprintf(tcNamedEventFormatted, L"\\\\.\\Notifications\\NamedEvents\\%s", m_tcNamedEvent);

	trigger.lpszApplication = tcNamedEventFormatted;
	trigger.lpszArguments = NULL;
	trigger.stStartTime = st;
	trigger.stEndTime = st;

	if (m_hNotify != NULL)
	{
		//  Need to cancel the notification here for repeat alarms, we won't reach
		//  here if the user attempts to set two alarms as that condition would
		//  be caught earlier.
		CeClearUserNotification(m_hNotify);
		m_hNotify = NULL;
	}
	//  Set the Notification with the Windows API
	m_hNotify = CeSetUserNotificationEx(m_hNotify, &trigger, &notification);
	if(m_hNotify == NULL)
	{
		//  Failed to Set Notification
		StopAlarmListenerThread();
		return FALSE;
	}
	
	//  Notification was successfully set
	return TRUE;
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		November 2009 (DCC: Initial Creation, Modified from PB2.x Alarm functionality)
*/
BOOL CAlarm::ClearAlarmNotifications()
{
	if (m_hNotify != NULL)
	{
		CeClearUserNotification(m_hNotify);
		m_hNotify = NULL;
	}
	StopAlarmListenerThread();
	return TRUE;
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		November 2009 (DCC: Initial Creation, Modified from PB2.x Alarm functionality)
*/
BOOL CAlarm::StopAlarmListenerThread()
{
	//  Stop the Alarm Notification Listener Thread
	SetEvent(m_hAlarmCancelledEvent);
	if (WaitForSingleObject(m_hAlarmListenerThread, 1000) == WAIT_TIMEOUT)
	{
		//  Terminate the Alarm listener if it did not shut down gracefully
		TerminateThread(m_hAlarmListenerThread, 0);
	}
	CloseHandle(m_hAlarmListenerThread);
	m_hAlarmListenerThread = NULL;
	return TRUE;
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		November 2009 (DCC: Initial Creation, Modified from PB2.x Alarm functionality)
*/
DWORD CAlarm::AlarmNotificationListener(LPVOID lpParam)
{
	//  Passed Paramter is the CAlarm Object in question
	CAlarm* pAlarm = (CAlarm*)lpParam;
	BOOL bRepeat = pAlarm->m_AlarmInfo.bRepeat;
	
	HANDLE hWaitHandles[2];
	hWaitHandles[0] = pAlarm->m_hAlarmTriggeredEvent;
	hWaitHandles[1] = pAlarm->m_hAlarmCancelledEvent;

	//  Wait for the Named Event to be triggered
	DWORD dwEvent = WaitForMultipleObjects (
		2,
		hWaitHandles,
		FALSE,
		INFINITE);
	switch (dwEvent)
	{
	case WAIT_OBJECT_0:
		{
			//  The Named event has been triggered, fire a PB Navigate
			if (*pAlarm->m_tcNavigationURI)
			{
				pAlarm->m_pModule->SendPBNavigate(
									tcAlarmTriggeredEventNames,
									pAlarm->m_iInstanceID, 
									pAlarm->m_tcNavigationURI, 
									NULL);				
			}
			break;
		}
	case WAIT_OBJECT_0 + 1:
		{
			//  The Alarm has been cancelled, exit and do not repeat the alarm
			bRepeat = FALSE;
			break;
		}
	}
	ResetEvent(pAlarm->m_hAlarmCancelledEvent);
	ResetEvent(pAlarm->m_hAlarmTriggeredEvent);
	pAlarm->m_bListenerThreadRunning = FALSE;
	if (bRepeat && 
		!(pAlarm->m_AlarmInfo.dwHHInterval == 0 && 
		pAlarm->m_AlarmInfo.dwMMInterval == 0 && 
		pAlarm->m_AlarmInfo.dwSSInterval == 0))
	{
		//  Repeat the Alarm if it was set on an interval
		pAlarm->Set();
	}
	return 0;
}