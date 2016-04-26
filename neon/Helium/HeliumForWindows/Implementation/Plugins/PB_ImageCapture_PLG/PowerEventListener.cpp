#include "stdafx.h"
#include "PowerEventListener.h"

/**
* \author	Geoff Day (GD, FMK Software)
* \date		October 2009 (GD: Initial Creation)
*/
CPowerEventListener::CPowerEventListener ()
{
	bEnabled = FALSE;
	hPowerThread = INVALID_HANDLE_VALUE;
	hPowerQuit = INVALID_HANDLE_VALUE;
	hPowerNotification = INVALID_HANDLE_VALUE;
	hPowerQueue = INVALID_HANDLE_VALUE;
}

/**
* \author	Geoff Day (GD, FMK Software)
* \date		October 2009 (GD: Initial Creation)
*/
CPowerEventListener::~CPowerEventListener ()
{
	StopListening ();
}

/**
* \author	Geoff Day (GD, FMK Software)
* \date		October 2009 (GD: Initial Creation)
*/
void CPowerEventListener::StartListening (void)
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
void CPowerEventListener::StopListening (void)
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
void CPowerEventListener::EnablePowerMonitor (void)
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
void CPowerEventListener::DisablePowerMonitor (void)
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
void CPowerEventListener::ProcessPowerThread (void)
{
	HANDLE wait_handles [2];
	BYTE power_buffer [64];
	POWER_BROADCAST *ppower;
	DWORD count, flags, previous_time;

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

		handlePowerEvents(ppower);
		
	}
}

/**
* \author	Geoff Day (GD, FMK Software)
* \date		October 2009 (GD: Initial Creation)
*/
DWORD CPowerEventListener::PowerOnProc (LPVOID pparam)
{
	CPowerEventListener *ppower = (CPowerEventListener*) pparam;
	ppower->ProcessPowerThread ();

	return 0;
}


