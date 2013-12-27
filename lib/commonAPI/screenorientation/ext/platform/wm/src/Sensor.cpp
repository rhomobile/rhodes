#include <windows.h>
#include "common/RhoStd.h"
#include "common/StringConverter.h"
#include "logging/RhoLog.h"
#include "Sensor.h"

const wchar_t* const SENSOR_MODULE_NAME = L"\\Windows\\SensorApi.dll";

/**
 * Check if the sensor api is supported
 * 
 * @author GXV738 (6/12/2013)
 * 
 * @return bool 
 */
bool ScreenOrientationExt::CSensor::IsPresent()
{
	DWORD dwAttribs = ::GetFileAttributes(SENSOR_MODULE_NAME);
	if (0xFFFFFFFF == dwAttribs)
	{
		LOG(TRACE) + " Cannot find sensor module : " + rho::common::convertToStringA<unsigned long>(::GetLastError());
	}
	bool canOpenSensor = false;
	HANDLE hSensor = CreateFile(SENSOR_DEVICE, 0, 0, NULL, OPEN_EXISTING, 0, NULL);
	if(hSensor != INVALID_HANDLE_VALUE)
	{
		canOpenSensor  = true;
		::CloseHandle(hSensor);
		hSensor = NULL;
	}
	else
	{
		LOG(TRACE) + " Cannot open sensor device : " + rho::common::convertToStringA<unsigned long>(::GetLastError());
	}

	return ((0xFFFFFFFF != dwAttribs) && ((dwAttribs & FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY) && canOpenSensor);
}

/**
 * On devices like wt41no and vc70 even if the sensor api is 
 * available, its still unsupported. Includes a present check as 
 * well. 
 * 
 * @author GXV738 (6/12/2013)
 * 
 * @return bool 
 */
bool ScreenOrientationExt::CSensor::IsSupported()
{
	bool supported = false;
#if 0	
	if (IsPresent())
	{
		// Disable Auto rotation for VC70 and WT41N0
		//get the system OEM string
		TCHAR szPlatform[MAX_PATH+1] = {0};		
		SystemParametersInfo(SPI_GETOEMINFO, MAX_PATH, szPlatform, 0);
		
		for(UINT i=0; i<wcslen(szPlatform); i++)
			szPlatform[i] |= 0x20;
		
		if(wcsstr(szPlatform, L"wt41n0") || wcsstr(szPlatform, L"vc70"))
		{	
			LOG(INFO)+ "Device does not support IST/Sensor, autorotation will not be available";	
			supported = false;
		}
		else
		{
			supported = true;
		}
	}
#else
	supported = IsPresent();
#endif
	return supported;
}

/**
 * 
 * 
 * @author GXV738 (6/12/2013)
 * 
 * @param enable if true turns on the AutoRotate feature, false 
 *  			 turns it off
 * 
 * @return bool true if the operation succeeds
 */
bool ScreenOrientationExt::CSensor::EnableAutoRotate(bool enable)
{
	bool done = false;
			
	HANDLE hSensor = CreateFile(SENSOR_DEVICE, 0, 0, NULL, OPEN_EXISTING, 0, NULL);
	if(hSensor != INVALID_HANDLE_VALUE)
	{
		ISTSystemConfig systemConfig;
		::ZeroMemory(&systemConfig, sizeof(ISTSYSTEMCONFIG));
        systemConfig.StructInfo.dwAllocated = sizeof(ISTSYSTEMCONFIG);

		//  Get the current auto rotate setting	
		BOOL bCurrentAutoRotation = TRUE;
		E_CONFIG_TYPE eCurrentConfig = AUTO_ORIENTATION;  

		if (DeviceIoControl(hSensor, IOCTL_SERVICE_GETCONFIG, &eCurrentConfig, sizeof(E_CONFIG_TYPE), &systemConfig, sizeof(ISTSystemConfig), NULL, NULL))
		{
			bCurrentAutoRotation = systemConfig.blActive;
			done = true;

			systemConfig.eConfigType = AUTO_ORIENTATION;
			if ((FALSE != bCurrentAutoRotation) && !enable)
			{				
				//  Autorotation is currently on, switch it off
				systemConfig.blActive = FALSE;
				if (!DeviceIoControl(hSensor, IOCTL_SERVICE_SETCONFIG, &systemConfig, sizeof(ISTSystemConfig), NULL, NULL, NULL, NULL))
				{
					done = false;
					LOG(TRACE) + "Sensor: Unable to turn off automatic screen rotation : " + rho::common::convertToStringA<unsigned long>(::GetLastError());	
				}				
			}
			else if ((FALSE == bCurrentAutoRotation) && enable)
			{
				//  Autorotation is currently off, switch it on
				systemConfig.blActive = TRUE;
				if (!DeviceIoControl(hSensor, IOCTL_SERVICE_SETCONFIG, &systemConfig, sizeof(ISTSystemConfig), NULL, NULL, NULL, NULL))
				{
					done = false;
					LOG(TRACE) + "Sensor: Unable to turn on  automatic screen rotation : " + rho::common::convertToStringA<unsigned long>(::GetLastError());	
				}
			}								
		}
		else
		{
			LOG(TRACE) + "Sensor:  Unable to get value for automatic screen rotation : " + rho::common::convertToStringA<unsigned long>(::GetLastError());	
		}
	}
	else
	{
		LOG(TRACE) + "Cannot open sensor device : " + rho::common::convertToStringA<unsigned long>(::GetLastError());	
	}
	return done;		
}

/**
 * 
 * 
 * @author GXV738 (6/12/2013)
 * 
 * @return bool bool true if autorotate is enabled and false if 
 *              autorotate is disabled. A false is returned when   
 *              there is an error.                                 
 *  
 */
bool ScreenOrientationExt::CSensor::IsAutoRotateEnabled()
{
	bool enabled  = false;

	HANDLE hSensor = CreateFile(SENSOR_DEVICE, 0, 0, NULL, OPEN_EXISTING, 0, NULL);
	if(hSensor != INVALID_HANDLE_VALUE)
	{
		ISTSystemConfig systemConfig;
		::ZeroMemory(&systemConfig, sizeof(ISTSYSTEMCONFIG));
        systemConfig.StructInfo.dwAllocated = sizeof(ISTSYSTEMCONFIG);

		//  Get the current auto rotate setting	
		BOOL bCurrentAutoRotation = TRUE;
		E_CONFIG_TYPE eCurrentConfig = AUTO_ORIENTATION;  

		if (DeviceIoControl(hSensor, IOCTL_SERVICE_GETCONFIG, &eCurrentConfig, sizeof(E_CONFIG_TYPE),
			&systemConfig, sizeof(ISTSystemConfig), NULL, NULL))
		{
			enabled = (FALSE != systemConfig.blActive);							
		}
		else
		{
			LOG(TRACE) + "Sensor:  Unable to get value for automatic screen rotation : " + rho::common::convertToStringA<unsigned long>(::GetLastError());	
		}

		::CloseHandle(hSensor);
		hSensor = NULL;
	}
	else
	{
		
			LOG(TRACE) + "Cannot open sensor device : " + rho::common::convertToStringA<unsigned long>(::GetLastError());
	}
	return enabled;			
}
