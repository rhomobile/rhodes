/** 
* \file PowerOn.h
* \brief File to house the CPowerEventListener class, used to alert the user when 
*  the device powers up.
*/

#pragma once

#include <windows.h>
#include <pm.h>
#include "../../common/Public/PBPlugin.h"

static TCHAR* tcPowerOnEventNames[] = {NULL};

/**
*  Class to inform a using application when power is restored to the device 
*  (the device wakes up).  The Windows CE API call RequestPowerNotifications is 
*  used to detect changes of power and events / queues are used to safely and 
*  efficiently send the notifications to the user.  Usage:  To use this class 
*  first create an instance of it and then call Start().  The PBModule passed 
*  into the contructor will contain a SendPBNavigate method which will be called
*  on power on.  The user will need to specify a URI to navigate to using 
*  SetNavigationURI otherwise they will not be informed of PowerOn.
*/
class CPowerEventListener
{
public:
	/** 
	*  Creates a power detection object
	*/
	CPowerEventListener ();

	/**
	*  Stops power detection and in doing so frees associated memory.
	*/
	~CPowerEventListener ();

	/**
	*  Enable power on detection and create associated events
	*/
	void StartListening (void);

	/**
	*  Disable power on detection and free associated memory
	*/
	void StopListening (void);


private:
	///  Whether or not power changes are being detected
	BOOL bEnabled;
	///  Handle associated with the PowerOnProc thread.
	HANDLE hPowerThread;
	///  Handle to event called to stop the PowerOnProc thread.
	HANDLE hPowerQuit;
	///  Handle returned from the Windows API call to RequestPowerNotifications
	HANDLE hPowerNotification;
	///  Handle to queue created to receive power change notifications from the device.
	HANDLE hPowerQueue;


	/**
	*  Create a message queue to receive power transition notifications from 
	*  the Windows API and register for those messages.
	*/
	void EnablePowerMonitor (void);

	/**
	*  Stop receiving power notifications from Windows and free memory 
	*  allocated in EnablePowerMonitor.
	*/
	void DisablePowerMonitor (void);

	/** 
	*  Internal function to detect power changes and notify the user on 
	*  power on.  This function is run in a separate thread.
	*/
	void ProcessPowerThread (void);

	/**
	*  Separate thread whose sole function is to call ProcessPowerThread.
	*/
	static DWORD PowerOnProc (LPVOID pparam);


	virtual void handlePowerEvents(PPOWER_BROADCAST PowerEvent) = 0;

};
