#ifndef SENSOR_H_34612E1C_97AA_43C1_B1E1_61EB28C378DE
#define SENSOR_H_34612E1C_97AA_43C1_B1E1_61EB28C378DE
#pragma once

#include "SensorApi.h"
////-----------------------------------------------------------------------------
//// Include Files
////-----------------------------------------------------------------------------
//
#include <Service.h>
//// Structure information structure
//typedef struct tagSTRUCT_INFO
//{
//	DWORD	dwAllocated;				// Size of allocated structure in bytes
//	DWORD	dwUsed;						// Amount of structure actually used
//
//} STRUCT_INFO;
//
#define SENSOR_DEVICE L"IST0:"

//-----------------------------------------------------------------------------
// Definitions
//-----------------------------------------------------------------------------

//
//	Save configuration to registry
//
#define IOCTL_SERVICE_SETCONFIG						CTL_CODE(FILE_DEVICE_SERVICE, 0x801 , METHOD_BUFFERED, FILE_ANY_ACCESS)

//
//	Retrieve configuration from registry
//
#define IOCTL_SERVICE_GETCONFIG						CTL_CODE(FILE_DEVICE_SERVICE, 0x802 , METHOD_BUFFERED, FILE_ANY_ACCESS)

//
//	Retrieve the Sensor Event Log
//
#define IOCTL_SERVICE_GET_SENSOR_EVENT_LOG 			CTL_CODE(FILE_DEVICE_SERVICE, 0x803 , METHOD_BUFFERED, FILE_ANY_ACCESS)

enum E_CONFIG_TYPE
{ 	
	UNDEFINED_CONFIG_TYPE=0,//Unknown type of Configuration
	KEEP_ALIVE,		//Keep Alive Configurations
	FACE_DOWN,			//Face down Configurations
	WAKE_UP,			//Wake Up Confuguration including sensitivity and reason and status
	AUTO_ORIENTATION,	//Auto Orientation Configurations
	FREE_FALL_SOUND,	//Free Fall Configuration
	EVENT_LOG			//Sensor Event logging configuration
};

union ISTCONFIGPARAM
{
	WORD	wLevel;					//Sensitivity level 
	BOOL	bLogGPSLocationEnable;	//Enable to log GPS Location
	TCHAR	lpFileName[MAX_PATH];	//Long pointer to null ternimated string
};

//-----------------------------------------------------------------------------
// Structure definitions
//-----------------------------------------------------------------------------
//This structure is used to set or retrieve IST sensor related configuration
typedef struct ISTSystemConfig
{
	STRUCT_INFO			StructInfo;	//Structure information for backward compatibility.
	E_CONFIG_TYPE		eConfigType;//Type of the configuration
	BOOL				blActive;	//Status of the specified configuration.
	ISTCONFIGPARAM		configParam;//Parameter for the specified configuration type
	BOOL				bPersist;	//Enable Persist Configuration setting
	ISTSystemConfig():	eConfigType(UNDEFINED_CONFIG_TYPE),
						blActive(FALSE),
						bPersist(FALSE){StructInfo.dwAllocated=sizeof(ISTSYSTEMCONFIG);
										StructInfo.dwUsed=0;
										configParam.wLevel=0;
										configParam.bLogGPSLocationEnable=FALSE;}
}ISTSYSTEMCONFIG, *pISTSYSTEMCONFIG;

namespace ScreenOrientationExt
{
	class CSensor
	{
	public:
		static bool IsPresent();
		static bool IsSupported();
		static bool EnableAutoRotate(bool enable = true);
		static bool IsAutoRotateEnabled();
	};
}

#endif //SENSOR_H_34612E1C_97AA_43C1_B1E1_61EB28C378DE
