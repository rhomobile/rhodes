#include "DeviceModule.h"

rho::apiGenerator::CMethodResult* m_pStatusEvent = NULL;

void CalibrateTheDeviceWithCallback(){
	BOOL isCalibrationDone = FALSE;
	//  Calibrate the Screen
	// Run the coredll function to calibrate the screen.
	isCalibrationDone = TouchCalibrate();
	
	// After calibration...force touch driver to update control area threshold.
	// This is done by signaling the named event TouchCalcThreshold
	HANDLE hTouchCalcThresholdEvent;
	hTouchCalcThresholdEvent = CreateEvent(NULL, FALSE, FALSE, L"TouchCalcThreshold");
	
	//  Signal Event
	SetEvent(hTouchCalcThresholdEvent);
	//  Free Memory
	CloseHandle(hTouchCalcThresholdEvent);      
	
	//Forcefully activating the main window.
	SendMessage( getMainWnd(), WM_ACTIVATE, 1, TRUE);
	
	if(isCalibrationDone == TRUE)
	{
		if(m_pStatusEvent != NULL)
		{
			UpdateCallbackStatus("success","Calibration was done succesfully.");
		}
		WriteDeviceModuleLog(L"INFO", L"Calibration was done succesfully.");
		isCalibrationDone = FALSE;
	}
	else
	{
		if(m_pStatusEvent != NULL)
		{
			UpdateCallbackStatus("failed","Calibration was failed.");
		}
		WriteDeviceModuleLog(L"INFO", L"Calibration was failed.");
	}
}
void CalibrateTheDevice(){
	BOOL isCalibrationDone = FALSE;
	//  Calibrate the Screen
	// Run the coredll function to calibrate the screen.
	isCalibrationDone = TouchCalibrate();
	
	// After calibration...force touch driver to update control area threshold.
	// This is done by signaling the named event TouchCalcThreshold
	HANDLE hTouchCalcThresholdEvent;
	hTouchCalcThresholdEvent = CreateEvent(NULL, FALSE, FALSE, L"TouchCalcThreshold");
	
	//  Signal Event
	SetEvent(hTouchCalcThresholdEvent);
	//  Free Memory
	CloseHandle(hTouchCalcThresholdEvent);      
	
	if(isCalibrationDone == TRUE)
	{
		WriteDeviceModuleLog(L"INFO", L"Calibration was done succesfully.");
		isCalibrationDone = FALSE;
	}
	else
	{
		WriteDeviceModuleLog(L"INFO", L"Calibration was failed.");
	}
}
void SuspendTheDevice(){
	//  Suspend the device (Put it into an idle state)
	DWORD returnValue;
	returnValue = SetSystemPowerState(0, POWER_STATE_SUSPEND, POWER_FORCE);
	if (returnValue != ERROR_SUCCESS)
	{ 
		WriteDeviceModuleLog(L"INFO", L"Unable to suspend the device.");
		if(m_pStatusEvent != NULL)
		{	
			char szMessage[256] = "";
			memset(szMessage,0,sizeof(szMessage));
			GetErrorMessageDetail(returnValue,szMessage);
			UpdateCallbackStatus("failed",szMessage);
		}			
	}
}
void PowerOffTheDevice(){
	//  PowerOff the device (Put it into a state that draws no power)
	DWORD returnValue;
	returnValue = SetSystemPowerState(0, POWER_STATE_OFF, POWER_FORCE);
	if (returnValue != ERROR_SUCCESS)
	{
		WriteDeviceModuleLog(L"INFO", L"Unable to power off the device.");
		if(m_pStatusEvent != NULL)
		{
			char szMessage[256] = "";
			memset(szMessage,0,sizeof(szMessage));
			GetErrorMessageDetail(returnValue,szMessage);
			UpdateCallbackStatus("failed",szMessage);
		}	
	}
}
void IdleTheDevice(){
	//  Wake the device (Put it into an active state)
	DWORD returnValue;
	returnValue = SetSystemPowerState(0, POWER_STATE_IDLE, POWER_FORCE);
	if (returnValue != ERROR_SUCCESS)
	{
		WriteDeviceModuleLog(L"INFO", L"Unable to change the device mode to idle state.");
		if(m_pStatusEvent != NULL)
		{
			char szMessage[256] = "";
			memset(szMessage,0,sizeof(szMessage));
			GetErrorMessageDetail(returnValue,szMessage);
			UpdateCallbackStatus("failed",szMessage);
		}	
	}
}
void WakeTheDevice(){
	//  Wake the device (Put it into an active state)
	DWORD returnValue;
	returnValue = SetSystemPowerState(0, POWER_STATE_ON, POWER_FORCE);
	if (returnValue != ERROR_SUCCESS)
	{
		WriteDeviceModuleLog(L"INFO", L"Unable to wake the device.");
		if(m_pStatusEvent != NULL)
		{
			char szMessage[256] = "";
			memset(szMessage,0,sizeof(szMessage));
			GetErrorMessageDetail(returnValue,szMessage);
			UpdateCallbackStatus("failed",szMessage);
		}	
	}
}
void RebootTheDevice(LPCTSTR bootType){
	BOOL resVal = FALSE;
	if (_wcsicmp(bootType, L"Warm") == 0)
	{
		WriteDeviceModuleLog(L"INFO", L"Warm Booting the device");
		//Warm Boot the Device
		resVal = KernelIoControl(IOCTL_HAL_REBOOT, NULL, 0, NULL, 0, NULL);
		if(resVal == FALSE)
		{
			WriteDeviceModuleLog(L"INFO", L"Failed to do Warm booting.");
			if(m_pStatusEvent != NULL)
			{
				UpdateCallbackStatus("failed","Failed to do Warm booting.");
			}	
		}
	}
	else if (_wcsicmp(bootType, L"Cold") == 0)
	{
		WriteDeviceModuleLog(L"INFO", L"Booting the device.");
			
		SetCleanRebootFlag();
		resVal = KernelIoControl(IOCTL_HAL_REBOOT, NULL, 0, NULL, 0, NULL);
		if(resVal == FALSE)
		{
			WriteDeviceModuleLog(L"INFO", L"Failed to do Cold booting.");
			if(m_pStatusEvent != NULL)
			{
				UpdateCallbackStatus("failed","Failed to do Cold booting.");
			}	
		}
	}
	else if (_wcsicmp(bootType, L"ColdCAD") == 0)
	{
		WriteDeviceModuleLog(L"INFO", L"Booting the device.");
		//  Cold Boot the device as though the user has pressed 1+9+Power
		//  (or whatever the device Cold Boot HW sequence is)
		LPFNCAD_COLDBOOT lpfnColdboot = (LPFNCAD_COLDBOOT)NULL;
		HINSTANCE hCadLib = (HINSTANCE)NULL;
		// Load an instance of the CAD DLL
		hCadLib = LoadLibrary(_TCADAPI);
		if (NULL == hCadLib){
			WriteDeviceModuleLog(L"INFO", L"Unable to load CAD DLL.");
			return;
		}
		// Get the address of the reboot function
		lpfnColdboot = (LPFNCAD_COLDBOOT)GetProcAddress(hCadLib,CAD_COLDBOOT);
		if (NULL != lpfnColdboot)
		{
			// set up registry before calling coldboot
			HKEY hKey = NULL;
			DWORD dwDisp;
			LONG lRet;
			lRet = RegCreateKeyEx(HKEY_CURRENT_USER,TEXT("Software\\Symbol\\Cad"),0,NULL,
										 REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS,NULL,&hKey,&dwDisp);
			if (lRet == ERROR_SUCCESS)
			{
				DWORD dwSize = sizeof(DWORD);
				DWORD dwData = 0x5054;
				RegSetValueEx(hKey,TEXT("ColdBoot"),0,REG_DWORD, (const BYTE *)&dwData,dwSize);
				RegCloseKey(hKey);		
			}
			resVal = lpfnColdboot();
			if(resVal == FALSE)
			{
				WriteDeviceModuleLog(L"INFO", L"Failed to do ColdCAD booting.");
				if(m_pStatusEvent != NULL)
				{
					UpdateCallbackStatus("failed","Failed to do ColdCAD booting.");
				}	
			}
		}
		else
		{
			WriteDeviceModuleLog(L"INFO", L"Unable to retrieve the function address.");
		}
		FreeLibrary(hCadLib);
	}
	else
	{
		WriteDeviceModuleLog(L"INFO", L"Warm Booting the device because the bootType value is either empty i.e. '' or other than 'Warm' or 'Cold' or 'ColdCAD' or nothing has been passed as an argument.");
		//Warm Boot the Device
		resVal = KernelIoControl(IOCTL_HAL_REBOOT, NULL, 0, NULL, 0, NULL);
		if(resVal == FALSE)
		{
			WriteDeviceModuleLog(L"INFO", L"Failed to do Warm booting.");
			if(m_pStatusEvent != NULL)
			{
				UpdateCallbackStatus("failed","Failed to do Warm booting.");
			}	
		}
	}
}

void SetCallback(rho::apiGenerator::CMethodResult* pCallback)
{
	m_pStatusEvent = pCallback;
}

void UpdateCallbackStatus(LPCSTR szStatus, LPCSTR szMessage)
{
	if(m_pStatusEvent != NULL){
		//Used for storing the appropriate status
		rho::Hashtable<rho::String, rho::String> statusData;
		statusData.put( "status",szStatus);
		statusData.put( "message",szMessage);
		m_pStatusEvent->set(statusData);		
	}	
}
void WriteDeviceModuleLog(LPCTSTR logLevel, LPCTSTR logData){
	if (_wcsicmp(logLevel, L"INFO") == 0)
	{
		LOG(INFO) + logData;
	}
	else if (_wcsicmp(logLevel, L"ERROR") == 0)
	{
		LOG(ERROR) + logData;
	}
	else if (_wcsicmp(logLevel, L"WARNING") == 0)
	{
		LOG(WARNING) + logData;
	}
	else if (_wcsicmp(logLevel, L"FATAL") == 0)
	{
		LOG(FATAL) + logData;
	}
}

void GetErrorMessageDetail(DWORD dwErrorCode, LPSTR szMessage)
{
	char szErrorMessage[256] = "";
	memset(szErrorMessage,0,sizeof(szErrorMessage));

	switch(dwErrorCode)
	{
		case ERROR_INVALID_FUNCTION:
			strcpy(szErrorMessage,"Incorrect function.");
			break;
		case ERROR_FILE_NOT_FOUND:
			strcpy(szErrorMessage,"System power state was not set.");
			break;
		case ERROR_PATH_NOT_FOUND:
			strcpy(szErrorMessage,"The system cannot find the path specified.");
			break;
		case ERROR_TOO_MANY_OPEN_FILES:
			strcpy(szErrorMessage,"The system cannot open the file.");
			break;
		case ERROR_ACCESS_DENIED:
			strcpy(szErrorMessage,"Access is denied.");
			break;
		case ERROR_INVALID_HANDLE:
			strcpy(szErrorMessage,"The handle is invalid.");
			break;
		case ERROR_BAD_ENVIRONMENT:
			strcpy(szErrorMessage,"The environment is incorrect.");
			break;
		case ERROR_INVALID_BLOCK:
			strcpy(szErrorMessage,"The storage control block address is invalid.");
			break;
		case ERROR_BAD_FORMAT:
			strcpy(szErrorMessage,"An attempt was made to load a program with an incorrect format.");
			break;
		case ERROR_NOT_ENOUGH_MEMORY:
			strcpy(szErrorMessage,"Not enough storage is available to process this command.");
			break;
		case ERROR_INVALID_ACCESS:
			strcpy(szErrorMessage,"The access code is invalid.");
			break;
		case ERROR_INVALID_DATA:
			strcpy(szErrorMessage,"The data is invalid.");
			break;
		case ERROR_OUTOFMEMORY:
			strcpy(szErrorMessage,"Not enough storage is available to complete this operation.");
			break;
		case ERROR_NOT_READY:
			strcpy(szErrorMessage,"The device is not ready.");
			break;
		case ERROR_BAD_COMMAND:
			strcpy(szErrorMessage,"The device does not recognize the command.");
			break;
		case ERROR_BAD_LENGTH:
			strcpy(szErrorMessage,"The program issued a command but the command length is incorrect.");
			break;
		case ERROR_BAD_UNIT:
			strcpy(szErrorMessage,"The system cannot find the device specified.");
			break;
		default:
			strcpy(szErrorMessage,"System power state was not set.");
			break;
	}
	strcpy(szMessage,szErrorMessage);
}
