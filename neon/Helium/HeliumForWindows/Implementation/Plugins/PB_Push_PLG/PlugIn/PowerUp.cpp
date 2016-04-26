/******************************************************************************/
#include <windows.h>
#include <pm.h>
#include "PowerUp.h"

/******************************************************************************/
// CPowerUp
/******************************************************************************/
CPowerUp::CPowerUp (PPOWERUPPROC pcallback)
{
	pCallback = pcallback;
	pUserData = NULL;	

	bEnabled = FALSE;
	hPowerThread = INVALID_HANDLE_VALUE;
	hPowerQuit = INVALID_HANDLE_VALUE;
	hPowerNotification = INVALID_HANDLE_VALUE;
	hPowerQueue = INVALID_HANDLE_VALUE;
}

/******************************************************************************/
CPowerUp::~CPowerUp ()
{
	Stop ();
}

/******************************************************************************/
void CPowerUp::Start (void)
{
	if (bEnabled)
		return;

	EnablePowerMonitor ();
	hPowerQuit = CreateEvent (NULL, TRUE, FALSE, NULL);
	hPowerThread = CreateThread (NULL, 0, PowerUpProc, this, 0, NULL);

	bEnabled = TRUE;
}

/******************************************************************************/
void CPowerUp::Stop (void)
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

/******************************************************************************/
void CPowerUp::EnablePowerMonitor (void)
{
	MSGQUEUEOPTIONS options;
	memset (&options, 0, sizeof options);
	options.dwSize = sizeof options;
	options.cbMaxMessage = 256;
	options.bReadAccess = TRUE;
	hPowerQueue = CreateMsgQueue (NULL, &options);

	hPowerNotification = RequestPowerNotifications (hPowerQueue, PBT_TRANSITION);
}

/******************************************************************************/
void CPowerUp::DisablePowerMonitor (void)
{
	StopPowerNotifications (hPowerNotification);
	hPowerNotification = INVALID_HANDLE_VALUE;

	CloseMsgQueue (hPowerQueue);
	hPowerQueue = INVALID_HANDLE_VALUE;
}

/******************************************************************************/
void CPowerUp::ProcessPowerThread (void)
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

		// Ignore all but power on
		if (ppower->Flags & POWER_STATE_ON)
		{
			// Get time stamp for message
			time = GetTickCount ();

			// Ignore message within 1 second of the previous one (ignore counter wraparound)
			if (time - previous_time > 1000)
				(*pCallback) (pUserData);

			previous_time = time;
		}
	}
}

/******************************************************************************/
/*static*/ DWORD CPowerUp::PowerUpProc (LPVOID pparam)
{
	CPowerUp *ppower = (CPowerUp*) pparam;
	ppower->ProcessPowerThread ();

	return 0;
}
