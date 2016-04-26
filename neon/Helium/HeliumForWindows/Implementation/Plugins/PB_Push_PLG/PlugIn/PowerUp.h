/** 
* \file PowerUp.h
*/

/******************************************************************************/
#pragma once

/******************************************************************************/
#include <windows.h>

/******************************************************************************/
typedef void (*PPOWERUPPROC) (void*);

/******************************************************************************/
class CPowerUp
{
public:
	/** Creates a power detection object
	* \param pcallback Address of function which will be called on power up detection
	*/
	CPowerUp (PPOWERUPPROC pcallback);
	~CPowerUp ();

	void *pUserData;	///< Pointer to any data the user wants, not processed by the object other than to send it with the detection callback.

	void Start (void);	///< Enables power on detection
	void Stop (void);	///< Disables power on detection

	/** Internal function to handle detection thread.
	* Public so it can be called from a static function but should not be called from externally.
	*/
	void ProcessPowerThread (void);

private:
	PPOWERUPPROC pCallback;
	BOOL bEnabled;
	HANDLE hPowerThread;
	HANDLE hPowerQuit;
	HANDLE hPowerNotification;
	HANDLE hPowerQueue;

	void EnablePowerMonitor (void);
	void DisablePowerMonitor (void);
	static DWORD PowerUpProc (LPVOID pparam);
};
