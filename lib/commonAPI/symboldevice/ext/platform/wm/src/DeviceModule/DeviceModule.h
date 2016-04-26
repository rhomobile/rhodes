#ifndef _DEVICEMODULE_H
#define _DEVICEMODULE_H

#include "common/RhodesApp.h"

//Definitions and API for the WCE Power Manager component.
#include <pm.h>

//This module defines device IO control codes.
#include <winioctl.h>

extern "C" HWND getMainWnd();

extern "C" BOOL WINAPI TouchCalibrate();// Used for Touch Calibration

//Reboot Module - IOCTL codes, Used for Warm Booting or Cold Booting the Device
extern "C" BOOL KernelIoControl(DWORD dwIoControlCode, LPVOID lpInBuf, DWORD nInBufSize, LPVOID lpOutBuf, DWORD nOutBufSize, LPDWORD lpBytesReturned);


// Macro definition for defining IOCTL and FSCTL function control codes.  Note
// that function codes 0-2047 are reserved for Microsoft Corporation, and
// 2048-4095 are reserved for customers.
#define IOCTL_HAL_REBOOT CTL_CODE(FILE_DEVICE_HAL, 15, METHOD_BUFFERED, FILE_ANY_ACCESS)

//Reboot Module - Used for Cold Booting the Device
extern "C" void SetCleanRebootFlag(void);

//Reboot Module - CAD.dll, Used for ColdCAD option.
typedef BOOL  (WINAPI* LPFNCAD_COLDBOOT)(void);	

#define _TCADAPI TEXT("cad.dll")

#define CAD_COLDBOOT (LPTSTR)0x86

extern rho::apiGenerator::CMethodResult* m_pStatusEvent; //Status Event: Will update the status.  

/**
* This function is used for logging the user logs.
*/
extern void WriteDeviceModuleLog(LPCTSTR logLevel, LPCTSTR logData);

/**
* Calibrate the device.
*/
extern void CalibrateTheDevice();

/**
* Calibrate the device with callback.
*/
extern void CalibrateTheDeviceWithCallback();

/**
* Suspend the device.
*/
extern void SuspendTheDevice();

/**
* PowerOff the device.
*/
extern void PowerOffTheDevice();

/**
* Idle the device.
*/
extern void IdleTheDevice();

/**
* Wake the device.
*/
extern void WakeTheDevice();

/**
* Reboot the device. Either Warm or Cold or ColdCAD boot is done.
*/
extern void RebootTheDevice(LPCTSTR bootType);

/**
* Sets the callback function for swipe event
*/
extern void SetCallback(rho::apiGenerator::CMethodResult* pCallback);

/**
* Update the callback status
*/
extern void UpdateCallbackStatus(LPCSTR szStatus, LPCSTR szMessage);

/**
* Get the error message
*/
extern void GetErrorMessageDetail(DWORD dwErrorCode, LPSTR szMessage);

#endif
