#include <windows.h>
#include <pm.h>
#include "PowerOn.h"

/**
* \author	Geoff Day (GD, FMK Software)
* \date		October 2009 (GD: Initial Creation)
*/
CPowerOn::CPowerOn (int iInstanceID, PBModule* pModule)
{
	bEnabled = FALSE;
	hPowerThread = INVALID_HANDLE_VALUE;
	hPowerQuit = INVALID_HANDLE_VALUE;
	hPowerNotification = INVALID_HANDLE_VALUE;
	hPowerQueue = INVALID_HANDLE_VALUE;
	m_pModule = pModule;
	m_iInstanceID = iInstanceID;
	bDeviceOff = false;
}

/**
* \author	Geoff Day (GD, FMK Software)
* \date		October 2009 (GD: Initial Creation)
*/
CPowerOn::~CPowerOn ()
{
	Stop ();
}

/**
* \author	Geoff Day (GD, FMK Software)
* \date		October 2009 (GD: Initial Creation)
*/
void CPowerOn::Start (void)
{
	if (bEnabled)
		return;

	EnablePowerMonitor ();
	hPowerQuit = CreateEvent (NULL, TRUE, FALSE, NULL);
	hPowerThread = CreateThread (NULL, 0, PowerOnProc, this, 0, NULL);

	bEnabled = TRUE;
}

/**
* \author	Geoff Day (GD, FMK Software)
* \date		October 2009 (GD: Initial Creation)
*/
void CPowerOn::Stop (void)
{
	if (!bEnabled)
		return;

	SetEvent (hPowerQuit);
	WaitForSingleObject (hPowerThread, 5000);

	DisablePowerMonitor ();

	CloseHandle (hPowerQuit);
	hPowerQuit = INVALID_HANDLE_VALUE;
	CloseHandle (hPowerThread);
	hPowerThread = INVALID_HANDLE_VALUE;

	bEnabled = FALSE;
}

/**
* \author	Geoff Day (GD, FMK Software)
* \date		October 2009 (GD: Initial Creation)
*/
void CPowerOn::EnablePowerMonitor (void)
{
	MSGQUEUEOPTIONS options;
	memset (&options, 0, sizeof options);
	options.dwSize = sizeof options;
	options.cbMaxMessage = 256;
	options.bReadAccess = TRUE;
	hPowerQueue = CreateMsgQueue (NULL, &options);

	hPowerNotification = RequestPowerNotifications (hPowerQueue, PBT_TRANSITION);
//	hPowerNotification = RequestPowerNotifications (hPowerQueue, POWER_NOTIFY_ALL);
}

/**
* \author	Geoff Day (GD, FMK Software)
* \date		October 2009 (GD: Initial Creation)
*/
void CPowerOn::DisablePowerMonitor (void)
{
	StopPowerNotifications (hPowerNotification);
	hPowerNotification = INVALID_HANDLE_VALUE;

	CloseMsgQueue (hPowerQueue);
	hPowerQueue = INVALID_HANDLE_VALUE;
}

/**
* \author	Geoff Day (GD, FMK Software)
* \date		October 2009 (GD: Initial Creation)
*/
void CPowerOn::ProcessPowerThread (void)
{
	HANDLE wait_handles [2];
	BYTE power_buffer [64];
	POWER_BROADCAST *ppower;
	DWORD count, flags, time, previous_time;

	wait_handles [0] = hPowerQuit;		// Signalled when thread should quit
	wait_handles [1] = hPowerQueue;		// Signalled on device power on

	previous_time = 0;

	while (1)
	{
		if (WaitForMultipleObjects (2, wait_handles, FALSE, INFINITE) == WAIT_OBJECT_0)
			break;

		// Must be power notification, get notification message
		if (!ReadMsgQueue (hPowerQueue, power_buffer, sizeof power_buffer, &count, 0, &flags))
			continue;

		// Get power state as reported by notification
		ppower = (POWER_BROADCAST*) power_buffer;

		if (ppower->Flags & POWER_STATE_SUSPEND || ppower->Flags & POWER_STATE_OFF)
		{
			bDeviceOff = true;
		}
		// Ignore all but power on
		else if (ppower->Flags & POWER_STATE_ON && bDeviceOff)
		{
			bDeviceOff = false;
			// Get time stamp for message
			time = GetTickCount ();

			// Ignore message within 1 second of the previous one (ignore counter wraparound)
			if (time - previous_time > 1000)
			{
				//  Power On Detected
				m_pModule->Log(PB_LOG_INFO, L"Power On Detected", _T(__FUNCTION__), __LINE__);
				if (*m_tcNavigationURI)
				{
					m_pModule->SendPBNavigate(tcPowerOnEventNames, m_iInstanceID, 
											  m_tcNavigationURI, 
											  NULL);
				}
			}

			previous_time = time;
		}
	}
}

/**
* \author	Geoff Day (GD, FMK Software)
* \date		October 2009 (GD: Initial Creation)
*/
DWORD CPowerOn::PowerOnProc (LPVOID pparam)
{
	CPowerOn *ppower = (CPowerOn*) pparam;
	ppower->ProcessPowerThread ();

	return 0;
}


/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		November 2009 (DCc : Initial Creation)
*/
BOOL CPowerOn::SetNavigationURI(LPCWSTR szNavURI)
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