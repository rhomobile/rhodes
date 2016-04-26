/** 
* \file PowerOn.h
* \brief File to house the CPowerOn class, used to alert the user when 
*  the device powers up.
*/

#pragma once

#include <windows.h>
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
class CPowerOn
{
public:
	/** 
	*  Creates a power detection object
	*  \param iInstanceID Unique Browser application Identifier
	*  \param pModule Pointer to associated PBModule which represents the PB
	*  instance to which this object is related.
	*/
	CPowerOn (int iInstanceID, PBModule* pModule);

	/**
	*  Stops power detection and in doing so frees associated memory.
	*/
	~CPowerOn ();

	/**
	*  Enable power on detection and create associated events
	*/
	void Start (void);

	/**
	*  Disable power on detection and free associated memory
	*/
	void Stop (void);

	/**
	*  Specify the Navigation string.
	*  \param szNavURI The Navigation String to be sent when invoking 
	*  SendPBNavigate
	*  \return Whether or not the Navigation URI was successfully set.
	*/
	BOOL SetNavigationURI(LPCWSTR szNavURI);

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
	///  Pointer to PBModule associated with this PowerOn object, used to send navigates and logs to the core.
	PBModule*	m_pModule;
	///  Unique Identifier for the Browser Application which uses this object
	int			m_iInstanceID;
	/// The URI To Navigate the PB Browser to
	TCHAR m_tcNavigationURI[MAXURL];
	///  Whether or not the previous state was off.  Added to avoid confusion with multiple power on's for accelerometer based screen rotation.
	bool	bDeviceOff;

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
};
