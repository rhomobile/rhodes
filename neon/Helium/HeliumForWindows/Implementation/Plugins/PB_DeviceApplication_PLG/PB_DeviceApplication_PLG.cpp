// PB_DeviceApplication_PLG.cpp : Defines the entry point for the DLL application.
//

#include "SIPModule.h"
#include "HourglassModule.h"
#include "SystemTimeModule.h"
//#include "ScreenOrientationModule.h"
#include "StylusModule.h"
#include "BacklightModule.h"
#include "VolumeModule.h"
#include "NotificationModule.h"
#include "ntfycapi.h"
#include "AlarmModule.h"
#include "TextSizeModule.h"
#include "ZoomModule.h"
#include "TimerModule.h"
#include "ApplicationModule.h"
#include "PowerOnModule.h"
#include "KeyLightModule.h"
#include "MemoryModule.h"
#include "SystemTime.h"
//  Defines for Device Module
#include <pm.h>
extern "C" BOOL WINAPI TouchCalibrate();
//  Defines for Reboot Module (IOCTL)
#include <winioctl.h>
extern "C" BOOL KernelIoControl(DWORD dwIoControlCode, LPVOID lpInBuf, DWORD nInBufSize, LPVOID lpOutBuf, DWORD nOutBufSize, LPDWORD lpBytesReturned);
#define IOCTL_HAL_REBOOT            CTL_CODE(FILE_DEVICE_HAL, 15, METHOD_BUFFERED, FILE_ANY_ACCESS)
extern "C" void SetCleanRebootFlag(void);
//  Defines for Reboot Module (CAD.dll)
typedef BOOL  (WINAPI* LPFNCAD_COLDBOOT)(void);	
#define _TCADAPI TEXT("cad.dll")
#define CAD_COLDBOOT (LPTSTR)0x86

//  Static Logging Functions
pLogFunc pLogger;
#define WRITELOG(format, ...) WriteLog (_T(__FUNCTION__), __LINE__, format, __VA_ARGS__)
void WriteLog (LPCWSTR pfunction, int line, LPCWSTR pformat, ...)
{
	if (!pLogger)
		return;

	LPWSTR pmessage = new WCHAR [256];
	va_list args;
	va_start (args, pformat);
	StringCchVPrintf (pmessage, 256, pformat, args);
	(*pLogger) (PB_LOG_INFO, pmessage, pfunction, line, L"Device and Application");
	delete [] pmessage;
}

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}

#pragma region SIP

CSIPModule *g_pSIPModule;

BOOL SIP_Preload (PPBSTRUCT pPBStructure, PPBCORESTRUCT pPBCoreStructure)
{
	if (!g_pSIPModule)
	{
		g_pSIPModule = new CSIPModule (pPBCoreStructure);

		if (!g_pSIPModule)
			return false;
	}

	return g_pSIPModule->Preload (pPBStructure, pPBCoreStructure);
}

BOOL SIP_MetaProc (PBMetaStruct *pbMetaStructure, PPBSTRUCT pPBStructure) 
{
	if (g_pSIPModule)
		return g_pSIPModule->Process (pbMetaStructure, pPBStructure);
	else
		return false;
}

BOOL SIP_Dispose (PPBSTRUCT pPBStructure) 
{
	INITRET iniRet;	
	if (g_pSIPModule)
	{
		iniRet = g_pSIPModule->Dispose(pPBStructure);
		if(iniRet == FAILED){
			return FALSE;
		}
		if (iniRet == DEINITIALISED)
		{
			delete g_pSIPModule;
			g_pSIPModule = 0;
		}
	}

	return true;
}

#pragma endregion

#pragma region Hourglass

CHourglassModule *g_pHourglassModule;

BOOL Hourglass_Preload (PPBSTRUCT pPBStructure, PPBCORESTRUCT pPBCoreStructure)
{
	if (!g_pHourglassModule)
	{
		g_pHourglassModule = new CHourglassModule ();

		if (!g_pHourglassModule)
			return false;
	}

	return g_pHourglassModule->Preload (pPBStructure, pPBCoreStructure);
}

BOOL Hourglass_MetaProc (PBMetaStruct *pbMetaStructure, PPBSTRUCT pPBStructure) 
{
	if (g_pHourglassModule)
		return g_pHourglassModule->Process (pbMetaStructure, pPBStructure);
	else
		return false;
}

BOOL Hourglass_Dispose (PPBSTRUCT pPBStructure) 
{
	INITRET iniRet;	
	if (g_pHourglassModule)
	{
		iniRet = g_pHourglassModule->Dispose(pPBStructure);
		if(iniRet == FAILED){
			return FALSE;
		}
		if (iniRet == DEINITIALISED)
		{
			delete g_pHourglassModule;
			g_pHourglassModule = 0;
		}
	}

	return true;
}

#pragma endregion


#pragma region SystemTime

CSystemTimeModule *g_pSystemTimeModule;

BOOL SystemTime_Preload (PPBSTRUCT pPBStructure, PPBCORESTRUCT pPBCoreStructure)
{
	if (!g_pSystemTimeModule)
	{
		g_pSystemTimeModule = new CSystemTimeModule ();

		if (!g_pSystemTimeModule)
			return false;
	}

	return g_pSystemTimeModule->Preload (pPBStructure, pPBCoreStructure);
}

BOOL SystemTime_MetaProc (PBMetaStruct *pbMetaStructure, PPBSTRUCT pPBStructure) 
{
	if (g_pSystemTimeModule)
		return g_pSystemTimeModule->Process (pbMetaStructure, pPBStructure);
	else
		return false;
}

BOOL SystemTime_Dispose (PPBSTRUCT pPBStructure) 
{
	INITRET iniRet;	
	if (g_pSystemTimeModule)
	{
		iniRet = g_pSystemTimeModule->Dispose(pPBStructure);
		if(iniRet == FAILED){
			return FALSE;
		}
		if (iniRet == DEINITIALISED)
		{
			delete g_pSystemTimeModule;
			g_pSystemTimeModule = 0;
		}
	}

	return true;
}

#pragma endregion

#pragma region Stylus

CStylusModule *g_pStylusModule;

BOOL Stylus_Preload (PPBSTRUCT pPBStructure, PPBCORESTRUCT pPBCoreStructure)
{
	if (!g_pStylusModule)
	{
		g_pStylusModule = new CStylusModule ();

		if (!g_pStylusModule)
			return false;
	}

	return g_pStylusModule->Preload (pPBStructure, pPBCoreStructure);
}

BOOL Stylus_MetaProc (PBMetaStruct *pbMetaStructure, PPBSTRUCT pPBStructure) 
{
	if (g_pStylusModule)
		return g_pStylusModule->Process (pbMetaStructure, pPBStructure);
	else
		return false;
}

BOOL Stylus_Dispose (PPBSTRUCT pPBStructure) 
{
	INITRET iniRet;	
	if (g_pStylusModule)
	{
		iniRet = g_pStylusModule->Dispose(pPBStructure);
		if(iniRet == FAILED){
			return FALSE;
		}
		if (iniRet == DEINITIALISED)
		{
			delete g_pStylusModule;
			g_pStylusModule = 0;
		}
	}

	return true;
}

#pragma endregion


#pragma region Backlight

CBacklightModule *g_pBacklightModule;

BOOL Backlight_Preload (PPBSTRUCT pPBStructure, PPBCORESTRUCT pPBCoreStructure)
{
	if (!g_pBacklightModule)
	{
		g_pBacklightModule = new CBacklightModule ();

		if (!g_pBacklightModule)
			return false;
	}

	return g_pBacklightModule->Preload (pPBStructure, pPBCoreStructure);
}

BOOL Backlight_MetaProc (PBMetaStruct *pbMetaStructure, PPBSTRUCT pPBStructure) 
{
	if (g_pBacklightModule)
		return g_pBacklightModule->Process (pbMetaStructure, pPBStructure);
	else
		return false;
}

BOOL Backlight_Dispose (PPBSTRUCT pPBStructure) 
{
	INITRET iniRet;	
	if (g_pBacklightModule)
	{
		iniRet = g_pBacklightModule->Dispose(pPBStructure);
		if(iniRet == FAILED){
			return FALSE;
		}
		if (iniRet == DEINITIALISED)
		{
			delete g_pBacklightModule;
			g_pBacklightModule = 0;
		}
	}

	return true;
}

#pragma endregion


#pragma region Volume

CVolumeModule *g_pVolumeModule;

BOOL Volume_Preload (PPBSTRUCT pPBStructure, PPBCORESTRUCT pPBCoreStructure)
{
	if (!g_pVolumeModule)
	{
		g_pVolumeModule = new CVolumeModule ();

		if (!g_pVolumeModule)
			return false;
	}

	return g_pVolumeModule->Preload (pPBStructure, pPBCoreStructure);
}

BOOL Volume_MetaProc (PBMetaStruct *pbMetaStructure, PPBSTRUCT pPBStructure) 
{
	if (g_pVolumeModule)
		return g_pVolumeModule->Process (pbMetaStructure, pPBStructure);
	else
		return false;
}

BOOL Volume_Dispose (PPBSTRUCT pPBStructure) 
{
	INITRET iniRet;	
	if (g_pVolumeModule)
	{
		iniRet = g_pVolumeModule->Dispose(pPBStructure);
		if(iniRet == FAILED){
			return FALSE;
		}
		if (iniRet == DEINITIALISED)
		{
			delete g_pVolumeModule;
			g_pVolumeModule = 0;
		}
	}

	return true;
}

#pragma endregion


#pragma region Notification

CNotificationModule *g_pNotificationModule;

BOOL Notification_Preload (PPBSTRUCT pPBStructure, PPBCORESTRUCT pPBCoreStructure)
{
	BOOL retVal = TRUE;
	if (!g_pNotificationModule)
	{
		g_pNotificationModule = new CNotificationModule ();

		if (!g_pNotificationModule)
			return false;

		retVal = g_pNotificationModule->Preload (pPBStructure, pPBCoreStructure);
	}

	return retVal;
}

BOOL Notification_MetaProc (PBMetaStruct *pbMetaStructure, PPBSTRUCT pPBStructure) 
{
	if (g_pNotificationModule)
		return g_pNotificationModule->Process (pbMetaStructure, pPBStructure);
	else
		return false;
}

BOOL Notification_Dispose (PPBSTRUCT pPBStructure) 
{
	INITRET iniRet;	
	if (g_pNotificationModule)
	{
		iniRet = g_pNotificationModule->Dispose(pPBStructure);
		if(iniRet == FAILED){
			return FALSE;
		}
		if (iniRet == DEINITIALISED)
		{
			delete g_pNotificationModule;
			g_pNotificationModule = 0;
		}
	}

	return true;
}

BOOL GetNotificationInfo_Preload (PPBSTRUCT pPBStructure, PPBCORESTRUCT pPBCoreStructure)
{
	return Notification_Preload(pPBStructure, pPBCoreStructure);
}

BOOL GetNotificationInfo_MetaProc (PBMetaStruct *pbMetaStructure, PPBSTRUCT pPBStructure) 
{
	if (g_pNotificationModule)
	{
		//  Mimic the old PB 2.x behaviour which is to register to receive notifications
		//  and then call enumerate to receive the notifications immediately
		BOOL bSuccess = TRUE;
		PBMetaStruct pbMetaStructureNotification;
		pbMetaStructureNotification.lpParameter = L"EnumNotificationsEvent";
		pbMetaStructureNotification.lpValue = pbMetaStructure->lpParameter;
		bSuccess = bSuccess && g_pNotificationModule->Process (&pbMetaStructureNotification, pPBStructure);
		//  Invoke the values to be sent back
		pbMetaStructureNotification.lpParameter = L"Enumerate";
		pbMetaStructureNotification.lpValue = L"";
		bSuccess = bSuccess && g_pNotificationModule->Process (&pbMetaStructureNotification, pPBStructure);
		return bSuccess;
	}
	else
		return false;
}

BOOL GetNotificationInfo_Dispose (PPBSTRUCT pPBStructure) 
{
	return Notification_Dispose(pPBStructure);
}

BOOL InvokeNotification_Preload (PPBSTRUCT pPBStructure, PPBCORESTRUCT pPBCoreStructure)
{
	return Notification_Preload(pPBStructure, pPBCoreStructure);
}

BOOL InvokeNotification_MetaProc (PBMetaStruct *pbMetaStructure, PPBSTRUCT pPBStructure) 
{
	if (g_pNotificationModule)
	{
		//  Mimic the PB2.x InvokeNotification
		//  parse the Parameter tag.  This code lifted verbatim from PB2.x
		DWORD dwNotificationType = NOTIFY_TYPE_UNKNOWN;
		DWORD dwNotificationIndex = 0;
		DWORD dwParameter1 = 0;
		DWORD dwParameter2 = 0;
		DWORD dwParameter3 = 0;
		TCHAR tcParameter[MAX_URL];
		BOOL bSuccess = g_pNotificationModule->ParseParams(pPBStructure->iTabID, 
											pbMetaStructure->lpParameter, 
											&dwNotificationType, 
											&dwNotificationIndex, &dwParameter1, 
											&dwParameter2, &dwParameter3);
		if (dwNotificationType == NOTIFY_TYPE_UNKNOWN)
		{
			//  Some problem parsing the parameters
			g_pNotificationModule->Log(PB_LOG_WARNING, 
				L"Meta Tag Content string not in the expected format", 
				_T(__FUNCTION__), __LINE__);
			return FALSE;
		}
		PBMetaStruct pbMetaStructureNotification;

		if (bSuccess)
		{
			switch(dwNotificationType)
			{
			case NOTIFY_TYPE_LED:
				//  Set the LED Parameters, Parameter 1 is the On Duration
				pbMetaStructureNotification.lpParameter = L"SetLEDOnDuration";
				pbMetaStructureNotification.lpValue = _itow(dwParameter1, tcParameter, 10);
				bSuccess = bSuccess && g_pNotificationModule->Process(&pbMetaStructureNotification, pPBStructure);
				//  Set the LED Parameters, Parameter 2 is the Off Duration
				pbMetaStructureNotification.lpParameter = L"SetLEDOffDuration";
				pbMetaStructureNotification.lpValue = _itow(dwParameter2, tcParameter, 10);
				bSuccess = bSuccess && g_pNotificationModule->Process(&pbMetaStructureNotification, pPBStructure);
				//  Set the LED Parameters, Parameter 3 is the Number of Cycles
				pbMetaStructureNotification.lpParameter = L"SetLEDNumberOfCycles";
				pbMetaStructureNotification.lpValue = _itow(dwParameter3, tcParameter, 10);
				bSuccess = bSuccess && g_pNotificationModule->Process(&pbMetaStructureNotification, pPBStructure);
				break;
			case NOTIFY_TYPE_BEEPER:
				//  Set the Beeper Parameters, Parameter 1 is the Frequency
				pbMetaStructureNotification.lpParameter = L"SetBeeperFrequency";
				pbMetaStructureNotification.lpValue = _itow(dwParameter1, tcParameter, 10);
				bSuccess = bSuccess && g_pNotificationModule->Process(&pbMetaStructureNotification, pPBStructure);				
				//  Set the Beeper Parameters, Parameter 2 is the Volume
				pbMetaStructureNotification.lpParameter = L"SetBeeperVolume";
				pbMetaStructureNotification.lpValue = _itow(dwParameter2, tcParameter, 10);
				bSuccess = bSuccess && g_pNotificationModule->Process(&pbMetaStructureNotification, pPBStructure);				
				//  Set the Beeper Parameters, Parameter 3 is the Duration
				pbMetaStructureNotification.lpParameter = L"SetBeeperDuration";
				pbMetaStructureNotification.lpValue = _itow(dwParameter3, tcParameter, 10);
				bSuccess = bSuccess && g_pNotificationModule->Process(&pbMetaStructureNotification, pPBStructure);				
				break;
			case NOTIFY_TYPE_VIBRATOR:
				//  Set the Pager Parameters, Parameter 1 is the Duration
				pbMetaStructureNotification.lpParameter = L"SetVibrateDuration";
				pbMetaStructureNotification.lpValue = _itow(dwParameter1, tcParameter, 10);
				bSuccess = bSuccess && g_pNotificationModule->Process(&pbMetaStructureNotification, pPBStructure);				
				break;
			}
		}
		if (bSuccess)
		{
			//  All the Parameter attributes have been set successfully, 
			//  Cycle the notification
			pbMetaStructureNotification.lpParameter = L"StateCycle";
			pbMetaStructureNotification.lpValue = _itow(dwNotificationIndex, tcParameter, 10);
			return g_pNotificationModule->Process (&pbMetaStructureNotification, pPBStructure);
		}
		else 
			return FALSE;
	}
	else
		return FALSE;
}

BOOL InvokeNotification_Dispose (PPBSTRUCT pPBStructure) 
{
	return Notification_Dispose(pPBStructure);
}

#pragma endregion


#pragma region Device

BOOL Device_Preload (PPBSTRUCT pPBStructure, PPBCORESTRUCT pPBCoreStructure)
{
	//  No concept of a Device module, all Device functionality is carried
	//  out in the DLL exported functions
	if (!pLogger) 
		pLogger = pPBCoreStructure->pLoggingFunc; 
	return TRUE;
}

BOOL Device_MetaProc (PBMetaStruct *pbMetaStructure, PPBSTRUCT pPBStructure) 
{
	//  Expect one of two Meta Tag Parameters, "Suspend" or "Calibrate"
	//  Carry out a case insensitive comparison
	if (_wcsicmp(pbMetaStructure->lpParameter, L"Calibrate") == 0)
	{
		WRITELOG (L"Calibrating the Touch Screen");
		//  Calibrate the Screen
		// Run the coredll function to calibrate the screen.
		TouchCalibrate();
		
		// After calibration...force touch driver to update control area threshold.
		// This is dont by signaling the named event TouchCalcThreshold
		HANDLE hTouchCalcThresholdEvent;
		hTouchCalcThresholdEvent = CreateEvent(NULL, FALSE, FALSE, L"TouchCalcThreshold");
		
		//  Signal Event
		SetEvent(hTouchCalcThresholdEvent);
		//  Free Memory
		CloseHandle(hTouchCalcThresholdEvent);
	}
	else if (_wcsicmp(pbMetaStructure->lpParameter, L"Suspend") == 0)
	{
		WRITELOG (L"Suspending the Device");
		//  Suspend the device (Put it into an idle state)
		DWORD dwRetVal;
		dwRetVal = SetSystemPowerState(0, POWER_STATE_SUSPEND, POWER_FORCE);
		if (dwRetVal != ERROR_SUCCESS)
			return FALSE;
	}
	else if (_wcsicmp(pbMetaStructure->lpParameter, L"Wake") == 0)
	{
		WRITELOG (L"Waking the Device");
		//  Wake the device (Put it into an active state)
		DWORD dwRetVal;
		dwRetVal = SetSystemPowerState(0, POWER_STATE_ON, POWER_FORCE);
		if (dwRetVal != ERROR_SUCCESS)
			return FALSE;
	}
	else if (_wcsicmp(pbMetaStructure->lpParameter, L"PowerOff") == 0)
	{
		WRITELOG (L"PoweringOff the Device");
		//  PowerOff the device (Put it into a state that draws no power)
		DWORD dwRetVal;
		dwRetVal = SetSystemPowerState(NULL, POWER_STATE_OFF, POWER_FORCE);
		if (dwRetVal != ERROR_SUCCESS)
			return FALSE;
	}
	else
	{
		//  Unrecognised meta tag.  Do not log as we do not have a pointer to 
		//  the log function (there is no module).  If a static log function 
		//  ever becomes available perform a log here.
		return FALSE;
	}
	return TRUE;
}

BOOL Device_Dispose (PPBSTRUCT pPBStructure) 
{
	//  No concept of a Device module, all Device functionality is carried
	//  out in the DLL exported functions
	return TRUE;
}

#pragma endregion


#pragma region Alarm

CAlarmModule *g_pAlarmModule;

BOOL Alarm_Preload (PPBSTRUCT pPBStructure, PPBCORESTRUCT pPBCoreStructure)
{
	if (!g_pAlarmModule)
	{
		g_pAlarmModule = new CAlarmModule ();

		if (!g_pAlarmModule)
			return false;
	}

	return g_pAlarmModule->Preload (pPBStructure, pPBCoreStructure);
}

BOOL Alarm_MetaProc (PBMetaStruct *pbMetaStructure, PPBSTRUCT pPBStructure) 
{
	if (g_pAlarmModule)
		return g_pAlarmModule->Process (pbMetaStructure, pPBStructure);
	else
		return false;
}

BOOL Alarm_Dispose (PPBSTRUCT pPBStructure) 
{
	INITRET iniRet;	
	if (g_pAlarmModule)
	{
		iniRet = g_pAlarmModule->Dispose(pPBStructure);
		if(iniRet == FAILED){
			return FALSE;
		}
		if (iniRet == DEINITIALISED)
		{
			delete g_pAlarmModule;
			g_pAlarmModule = 0;
		}
	}

	return true;
}

#pragma endregion


#pragma region TextSize

CTextSizeModule *g_pTextSizeModule;

BOOL TextSize_Preload (PPBSTRUCT pPBStructure, PPBCORESTRUCT pPBCoreStructure)
{
	if (!g_pTextSizeModule)
	{
		g_pTextSizeModule = new CTextSizeModule (pPBCoreStructure);

		if (!g_pTextSizeModule)
			return false;
	}

	return g_pTextSizeModule->Preload (pPBStructure, pPBCoreStructure);
}

BOOL TextSize_MetaProc (PBMetaStruct *pbMetaStructure, PPBSTRUCT pPBStructure) 
{
	if (g_pTextSizeModule)
		return g_pTextSizeModule->Process (pbMetaStructure, pPBStructure);
	else
		return false;
}

BOOL TextSize_Dispose (PPBSTRUCT pPBStructure) 
{
	INITRET iniRet;	
	if (g_pTextSizeModule)
	{
		iniRet = g_pTextSizeModule->Dispose(pPBStructure);
		if(iniRet == FAILED){
			return FALSE;
		}
		if (iniRet == DEINITIALISED)
		{
			delete g_pTextSizeModule;
			g_pTextSizeModule = 0;
		}
	}

	return true;
}

#pragma endregion

#pragma region Zoom

CZoomModule *g_pZoomModule;

BOOL Zoom_Preload (PPBSTRUCT pPBStructure, PPBCORESTRUCT pPBCoreStructure)
{
	if (!g_pZoomModule)
	{
		g_pZoomModule = new CZoomModule (pPBCoreStructure);

		if (!g_pZoomModule)
			return false;
	}

	return g_pZoomModule->Preload (pPBStructure, pPBCoreStructure);
}

BOOL Zoom_MetaProc (PBMetaStruct *pbMetaStructure, PPBSTRUCT pPBStructure) 
{
	if (g_pZoomModule)
		return g_pZoomModule->Process (pbMetaStructure, pPBStructure);
	else
		return false;
}

BOOL Zoom_Dispose (PPBSTRUCT pPBStructure) 
{
	INITRET iniRet;	
	if (g_pZoomModule)
	{
		iniRet = g_pZoomModule->Dispose(pPBStructure);
		if(iniRet == FAILED){
			return FALSE;
		}
		if (iniRet == DEINITIALISED)
		{
			delete g_pZoomModule;
			g_pZoomModule = 0;
		}
	}

	return true;
}

#pragma endregion
#pragma region Timer

CTimerModule *g_pTimerModule;

BOOL Timer_Preload (PPBSTRUCT pPBStructure, PPBCORESTRUCT pPBCoreStructure)
{
	if (!g_pTimerModule)
	{
		g_pTimerModule = new CTimerModule ();

		if (!g_pTimerModule)
			return false;
	}

	return g_pTimerModule->Preload (pPBStructure, pPBCoreStructure);
}

BOOL Timer_MetaProc (PBMetaStruct *pbMetaStructure, PPBSTRUCT pPBStructure) 
{
	if (g_pTimerModule)
		return g_pTimerModule->Process (pbMetaStructure, pPBStructure);
	else
		return false;
}

BOOL Timer_Dispose (PPBSTRUCT pPBStructure) 
{
	INITRET iniRet;	
	if (g_pTimerModule)
	{
		iniRet = g_pTimerModule->Dispose(pPBStructure);
		if(iniRet == FAILED){
			return FALSE;
		}
		if (iniRet == DEINITIALISED)
		{
			delete g_pTimerModule;
			g_pTimerModule = 0;
		}
	}

	return true;
}

#pragma endregion

#pragma region Mmeory

CMemoryModule *g_pMemoryModule;

BOOL Memory_Preload (PPBSTRUCT pPBStructure, PPBCORESTRUCT pPBCoreStructure)
{
	if (!g_pMemoryModule)
	{
		g_pMemoryModule = new CMemoryModule ();

		if (!g_pMemoryModule)
			return false;
	}

	return g_pMemoryModule->Preload (pPBStructure, pPBCoreStructure);
}

BOOL Memory_MetaProc (PBMetaStruct *pbMetaStructure, PPBSTRUCT pPBStructure) 
{
	if (g_pMemoryModule)
		return g_pMemoryModule->Process (pbMetaStructure, pPBStructure);
	else
		return false;
}

BOOL Memory_Dispose (PPBSTRUCT pPBStructure) 
{
	INITRET iniRet;	
	if (g_pMemoryModule)
	{
		iniRet = g_pMemoryModule->Dispose(pPBStructure);
		if(iniRet == FAILED){
			return FALSE;
		}
		if (iniRet == DEINITIALISED)
		{
			delete g_pMemoryModule;
			g_pMemoryModule = 0;
		}
	}

	return true;
}

#pragma endregion

#pragma region Reboot

BOOL Reboot_Preload (PPBSTRUCT pPBStructure, PPBCORESTRUCT pPBCoreStructure)
{
	//  No concept of a Device module, all Device functionality is carried
	//  out in the DLL exported functions
	if (!pLogger) 
		pLogger = pPBCoreStructure->pLoggingFunc; 
	return TRUE;
}

BOOL Reboot_MetaProc (PBMetaStruct *pbMetaStructure, PPBSTRUCT pPBStructure) 
{
	//  Expect one of two Meta Tag Parameters, "Warm" or "Cold"
	//  Carry out a case insensitive comparison
	if (_wcsicmp(pbMetaStructure->lpParameter, L"BootType") == 0 &&
		pbMetaStructure->lpValue != NULL)
	{
		if (_wcsicmp(pbMetaStructure->lpValue, L"Warm") == 0)
		{
			WRITELOG (L"Warm Booting the Device");
			//  Warm Boot the Device
			KernelIoControl(IOCTL_HAL_REBOOT, NULL, 0, NULL, 0, NULL);

		}
		else if (_wcsicmp(pbMetaStructure->lpValue, L"Cold") == 0)
		{
			WRITELOG (L"Cold Booting the Device (Software)");
			//  Cold Boot the Device
			SetCleanRebootFlag();
			KernelIoControl(IOCTL_HAL_REBOOT, NULL, 0, NULL, 0, NULL);

		}
		else if (_wcsicmp(pbMetaStructure->lpValue, L"ColdCAD") == 0)
		{
			WRITELOG (L"Cold Booting the Device (Equivilent to 1+9+Power)");
			//  Cold Boot the device as though the user has pressed 1+9+Power
			//  (or whatever the device Cold Boot HW sequence is)
			LPFNCAD_COLDBOOT lpfnColdboot = (LPFNCAD_COLDBOOT)NULL;
			HINSTANCE hCadLib = (HINSTANCE)NULL;
			// Load an instance of the CAD DLL
			hCadLib = LoadLibrary(_TCADAPI);
			if (NULL == hCadLib)
				return FALSE;
			// Get the address of the reboot function
			lpfnColdboot = (LPFNCAD_COLDBOOT)GetProcAddress(hCadLib,CAD_COLDBOOT);
			if (NULL != lpfnColdboot)
			{
				// set up registry before calling coldboot
				HKEY hKey = NULL;
				DWORD dwDisp;
				LONG lRet;
				//	[HKEY_CURRENT_USER\Software\Symbol\Cad]
				//		"ColdBoot"=dword:00005054
				lRet = RegCreateKeyEx(HKEY_CURRENT_USER,TEXT("Software\\Symbol\\Cad"),0,NULL,
											 REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS,NULL,&hKey,&dwDisp);
				if (lRet == ERROR_SUCCESS)
				{
					DWORD dwSize = sizeof(DWORD);
					DWORD dwData = 0x5054;
					RegSetValueEx(hKey,TEXT("ColdBoot"),0,REG_DWORD, (const BYTE *)&dwData,dwSize);
					RegCloseKey(hKey);		
				}
				lpfnColdboot();
			}
			FreeLibrary(hCadLib);
		}
	}
	else
	{
		//  Unrecognised meta tag.  Do not log as we do not have a pointer to 
		//  the log function (there is no module).  If a static log function 
		//  ever becomes available perform a log here.
		return FALSE;
	}
	return TRUE;
}

BOOL Reboot_Dispose (PPBSTRUCT pPBStructure) 
{
	//  No concept of a Device module, all Device functionality is carried
	//  out in the DLL exported functions
	return TRUE;
}

#pragma endregion


#pragma region Application

CApplicationModule *g_pApplicationModule;

BOOL Application_Preload (PPBSTRUCT pPBStructure, PPBCORESTRUCT pPBCoreStructure)
{
	if (!g_pApplicationModule)
	{
		g_pApplicationModule = new CApplicationModule (pPBCoreStructure);

		if (!g_pApplicationModule)
			return false;
	}

	return g_pApplicationModule->Preload (pPBStructure, pPBCoreStructure);
}

BOOL Application_MetaProc (PBMetaStruct *pbMetaStructure, PPBSTRUCT pPBStructure) 
{
	if (g_pApplicationModule)
		return g_pApplicationModule->Process (pbMetaStructure, pPBStructure);
	else
		return false;
}

BOOL Application_Dispose (PPBSTRUCT pPBStructure) 
{
	INITRET iniRet;	
	if (g_pApplicationModule)
	{
		iniRet = g_pApplicationModule->Dispose(pPBStructure);
		if(iniRet == FAILED){
			return FALSE;
		}
		if (iniRet == DEINITIALISED)
		{
			delete g_pApplicationModule;
			g_pApplicationModule = 0;
		}
	}

	return true;
}

#pragma endregion


#pragma region PowerOn

CPowerOnModule *g_pPowerOnModule;

BOOL PowerOn_Preload (PPBSTRUCT pPBStructure, PPBCORESTRUCT pPBCoreStructure)
{
	if (!g_pPowerOnModule)
	{
		g_pPowerOnModule = new CPowerOnModule ();

		if (!g_pPowerOnModule)
			return false;
	}

	return g_pPowerOnModule->Preload (pPBStructure, pPBCoreStructure);
}

BOOL PowerOn_MetaProc (PBMetaStruct *pbMetaStructure, PPBSTRUCT pPBStructure) 
{
	if (g_pPowerOnModule)
		return g_pPowerOnModule->Process (pbMetaStructure, pPBStructure);
	else
		return false;
}

BOOL PowerOn_Dispose (PPBSTRUCT pPBStructure) 
{
	INITRET iniRet;	
	if (g_pPowerOnModule)
	{
		iniRet = g_pPowerOnModule->Dispose(pPBStructure);
		if(iniRet == FAILED){
			return FALSE;
		}
		if (iniRet == DEINITIALISED)
		{
			delete g_pPowerOnModule;
			g_pPowerOnModule = 0;
		}
	}

	return true;
}

#pragma endregion


#pragma region Keylight

CKeylightModule *g_pKeylightModule;

BOOL Keylight_Preload (PPBSTRUCT pPBStructure, PPBCORESTRUCT pPBCoreStructure)
{
	if (!g_pKeylightModule)
	{
		g_pKeylightModule = new CKeylightModule ();

		if (!g_pKeylightModule)
			return false;
	}

	return g_pKeylightModule->Preload (pPBStructure, pPBCoreStructure);
}

BOOL Keylight_MetaProc (PBMetaStruct *pbMetaStructure, PPBSTRUCT pPBStructure) 
{
	if (g_pKeylightModule)
		return g_pKeylightModule->Process (pbMetaStructure, pPBStructure);
	else
		return false;
}

BOOL Keylight_Dispose (PPBSTRUCT pPBStructure) 
{
	INITRET iniRet;	
	if (g_pKeylightModule)
	{
		iniRet = g_pKeylightModule->Dispose(pPBStructure);
		if(iniRet == FAILED){
			return FALSE;
		}
		if (iniRet == DEINITIALISED)
		{
			delete g_pKeylightModule;
			g_pKeylightModule = 0;
		}
	}

	return true;
}

#pragma endregion



