/******************************************************************************/
#include <windows.h>
#include <pmpolicy.h>
#include <pm.h>

/******************************************************************************/
HANDLE hPowerThread = INVALID_HANDLE_VALUE;
HANDLE hPowerTimer = INVALID_HANDLE_VALUE;
HANDLE hPowerNotification = INVALID_HANDLE_VALUE;
HANDLE hPowerQueue = INVALID_HANDLE_VALUE;
long lPowerCountdown = 0, lPowerTimeout = 0;
CRITICAL_SECTION csPower;
int nEnabledCount = 0;

/******************************************************************************/
void EnablePowerMonitor (void)
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
void DisablePowerMonitor (void)
{
	StopPowerNotifications (hPowerNotification);
	hPowerNotification = INVALID_HANDLE_VALUE;

	CloseMsgQueue (hPowerQueue);
	hPowerQueue = INVALID_HANDLE_VALUE;
}

/******************************************************************************/
void GetPowerTimeout (void)
{
	HKEY hkey;
	DWORD type, size;

	lPowerTimeout = 0L;

	if (RegOpenKeyEx (HKEY_LOCAL_MACHINE, L"System\\CurrentControlSet\\Control\\Power\\Timeouts", 0, 0, &hkey) != ERROR_SUCCESS)
		return;

	size = sizeof lPowerTimeout;

	// Ignore failure, we'll close the key and return anyway
	RegQueryValueEx (hkey, L"BattSuspendTimeout", NULL, &type, (LPBYTE) &lPowerTimeout, &size);

	RegCloseKey (hkey);
}

/******************************************************************************/
void ResetPowerCountdown (void)
{
	EnterCriticalSection (&csPower);
	lPowerCountdown = lPowerTimeout;
	LeaveCriticalSection (&csPower);

#if _DEBUG
	/*MessageBeep (-1);
	Sleep (200);*/
#endif
}

/******************************************************************************/
DWORD PowerThread (LPVOID pparam)
{
	BOOL suspend;
	HANDLE wait_handles [2];
	DWORD wait_result;
	BYTE power_buffer [64];
	POWER_BROADCAST *ppower;
	DWORD count, flags, time, previous_time;

	wait_handles [0] = hPowerTimer;		// Signalled when thread should quit
	wait_handles [1] = hPowerQueue;		// Signalled on device power on

	previous_time = 0;

	while (1)
	{
		wait_result = WaitForMultipleObjects (2, wait_handles, FALSE, 1000);

		// Quitting?
		if (wait_result == WAIT_OBJECT_0)
			break;

		// Power notification?
		if (wait_result == WAIT_OBJECT_0 + 1)
		{
			// Get next power notification message
			if (!ReadMsgQueue (hPowerQueue, power_buffer, sizeof power_buffer, &count, 0, &flags))
				continue;

			// Get time stamp for message
			time = GetTickCount ();

			// Ignore message within 1 second of the previous one (ignore counter wraparound)
			if (time - previous_time > 1000)
			{
				// Get power state as reported by notification
				ppower = (POWER_BROADCAST*) power_buffer;

				if (ppower->Flags & POWER_STATE_ON)
					ResetPowerCountdown ();
			}

			previous_time = time;

			continue;
		}

		// Must be one-second timeout
		SystemIdleTimerReset ();

		suspend = FALSE;

		// Safely decrement counter if above zero, suspend if now zero
		EnterCriticalSection (&csPower);
		if (lPowerCountdown > 0)
			suspend = (--lPowerCountdown == 0);
		LeaveCriticalSection (&csPower);

		if (suspend)
			SetSystemPowerState (NULL, POWER_STATE_IDLE, POWER_FORCE);
	}

	return 0;
}

/******************************************************************************/
void EnableUnattended (void)
{
	// Inc ref count and enable if count was zero
	if (nEnabledCount++ > 0)
		return;

	PowerPolicyNotify (PPN_UNATTENDEDMODE, TRUE);

	InitializeCriticalSection (&csPower);
	EnablePowerMonitor ();
	hPowerTimer = CreateEvent (NULL, TRUE, FALSE, NULL);
	hPowerThread = CreateThread (NULL, 0, PowerThread, NULL, 0, NULL);
	GetPowerTimeout ();
	ResetPowerCountdown ();
}

/******************************************************************************/
void DisableUnattended (void)
{
	// Dec ref count and disable if count now zero
	if (--nEnabledCount > 0)
		return;

	SetEvent (hPowerTimer);
	WaitForSingleObject (hPowerThread, 5000);

	DisablePowerMonitor ();

	CloseHandle (hPowerTimer);
	hPowerTimer = INVALID_HANDLE_VALUE;
	CloseHandle (hPowerThread);
	hPowerThread = INVALID_HANDLE_VALUE;
	DeleteCriticalSection (&csPower);

	PowerPolicyNotify (PPN_UNATTENDEDMODE, FALSE);
}
