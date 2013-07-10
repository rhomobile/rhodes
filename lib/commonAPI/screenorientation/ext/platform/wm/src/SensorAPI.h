//------------------------------------------------------------------
// FILENAME:		SensorAPI.h
//
// MODULE NAME:		SensorAPI.DLL
//
// Copyright(c) 2012 Motorola Solutions Inc. All rights reserved.
//
// DESCRIPTION:	Public API interface for Integrated Sensor 
//					Technology (IST)
//
// NOTES:
//
// 
//------------------------------------------------------------------

#pragma once
#pragma message ("Using Motorola SensorAPI ...")


//------------------------------------------------------------------
// Nested Include Files
//------------------------------------------------------------------
#include <windows.h>
#include "StrucInf.h"

//File version for SensorAPI.h, (starting from 10)
#define FILE_VERSION_SENSOR_API_H		21


//--------------------------------------------------------------------
// Sensor System specific Definitions
//--------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Sensor System specific Enumerations
//-----------------------------------------------------------------------------
//Sensor Open Modes
typedef enum _OPEN_MODE
{
	SENSOR_OPEN_MODE_DEFAULT = 0,
	SENSOR_OPEN_MODE_MASTER  = 1
}OPEN_MODE;

// Sensor sub system components
typedef enum  _SENSOR_SYSTEM_COMPONENT 
{
	SENSOR_SYSTEM_MICRO		=1,     //Microprocessor specific to Sensor system
	SENSOR_SYSTEM_FIRMWARE	=2,		//Sensor system specific firmware
	SENSOR_SYSTEM_VP		=10,	//Sensor Virtual Platform 
	SENSOR_SYSTEM_HAL		=11,	//Sensor HAL
	SENSOR_SYSTEM_DRIVER	=12,	//Sensor Driver
	SENSOR_SYSTEM_API		=13,	//Sensor API
	SENSOR_SYSTEM_SERVICE	=14		//Sersor system service
}SENSOR_SYSTEM_COMPONENT;

// Types of sensors supported by the SENSOR API
typedef enum _SENSOR_TYPE
{
	SENSOR_TYPE_UNDEFINED				=0, //Unknown type of sensor 
	SENSOR_TYPE_ACCELEROMETER			=1,
	SENSOR_TYPE_ORIENTATION				=2,
	SENSOR_TYPE_TILT_ANGLE				=3,
	SENSOR_TYPE_MOTION					=4,
	SENSOR_TYPE_ECOMPASS				=5,
	SENSOR_TYPE_MAGNETOMETER			=6,
	SENSOR_TYPE_GYROSCOPE				=7,
	SENSOR_TYPE_AMBIENT_LIGHT			=8,
	SENSOR_TYPE_PROXIMITY				=9,
	SENSOR_TYPE_PROXIMITY_LONG_RANGE	=10,
	SENSOR_TYPE_PRESSURE				=11,
	SENSOR_TYPE_TEMPERATURE				=12,
	SENSOR_TYPE_HUMIDITY				=13,
	SENSOR_TYPE_USR_DEFINED				=1000,
	SENSOR_TYPE_ALL						=9999 //to be used with SENSOR_findfirst()
}SENSOR_TYPE;

// Units of data or property values
typedef enum _DATA_UNIT
{
	DATA_UNIT_NONE				=0,
	DATA_UNIT_G					=1,
	DATA_UNIT_LUX				=2,
	DATA_UNIT_DEGREES			=3,
	DATA_UNIT_DEGREES_PER_SEC	=4,
	DATA_UNIT_PASCAL			=5,
	DATA_UNIT_CELSIUS			=6,
	DATA_UNIT_GAUSS				=7,
	DATA_UNIT_PERCENT			=8
}DATA_UNIT;

// Defines the context of a measured quantity (the location of the sensor)
typedef enum _SENSOR_CONTEXT
{
	SENSOR_CONTEXT_UNDEFINED	=0,
	SENSOR_CONTEXT_DEVICE		=1,
	SENSOR_CONTEXT_ENVIRONMENT	=2,
	SENSOR_CONTEXT_USER			=3,
	SENSOR_CONTEXT_REMOTE		=4
}SENSOR_CONTEXT;

// Methods used to connect sensors to the device
typedef enum _SENSOR_CONNECTION_TYPE
{
	SENSOR_CONNECTION_UNDEFINED	=0,
	SENSOR_CONNECTION_INTERNAL	=1,
	SENSOR_CONNECTION_ATTACHED	=2,
	SENSOR_CONNECTION_WIRELESS	=3,
	SENSOR_CONNECTION_WIRED		=4,
	SENSOR_CONNECTION_NETWORK	=5,
	SENSOR_CONNECTION_DERIVED	=6
}SENSOR_CONNECTION_TYPE;

//Types of data from a sensor
typedef enum _SENSOR_DATA_TYPE 
{
	DATA_TYPE_NONE		=0,
	DATA_TYPE_VECTOR	=1,
	DATA_TYPE_SCALAR	=2
	
}SENSOR_DATA_TYPE;

//Types of registration for streaming data and property changes
typedef enum _NOTIFIER_TYPE
{
	NOTIFIER_TYPE_NONE = 0,
	NOTIFIER_TYPE_EVENT = 1,
	NOTIFIER_TYPE_CALLBACK = 2
}NOTIFIER_TYPE;

//Types of sampling types
typedef enum _SAMPLING_TYPE
{
	SAMPLING_TYPE_NONE			=0,
	SAMPLING_TYPE_ON_CHANGE		=1,
	SAMPLING_TYPE_ON_RATE		=2
}SAMPLING_TYPE;

//Sample rates for continuous streaming mode
typedef enum _SAMPLE_RATE
{
	SAMPLE_RATE_1_HZ		= 1,
	SAMPLE_RATE_2_HZ		= 2,
	SAMPLE_RATE_5_HZ		= 5,
	SAMPLE_RATE_10_HZ		= 10,
	SAMPLE_RATE_20_HZ		= 20,
	SAMPLE_RATE_50_HZ		= 50,
	SAMPLE_RATE_100_HZ		= 100,
	SAMPLE_RATE_200_HZ		= 200,
	SAMPLE_RATE_400_HZ		= 400
}SAMPLE_RATE;

// Properties of sensors. Each sensor would only set the properties relevant to it
typedef enum _SENSOR_PROPERTY
{
	//Common properties
	SENSOR_PROP_UNDEFINED		=0,
	SENSOR_PROP_MIN_VALUE		=1,
	SENSOR_PROP_MAX_VALUE		=2,
	SENSOR_PROP_RANGE			=3,
	SENSOR_PROP_LOW_THRESHOLD	=4,
	SENSOR_PROP_HIGH_THRESHOLD	=5,		
	SENSOR_PROP_SAMPLE_RATE		=6,	
	SENSOR_PROP_ACCURACY 		=7,
	SENSOR_PROP_SCALE			=8,
	SENSOR_PROP_USER_DEFINED	=1000
	
}SENSOR_PROPERTY;

//Device orientation types
typedef enum _DEVICE_ORIENTATION
{
	DEV_ORIENTATION_PORTRAIT_DOWN	= 0,
	DEV_ORIENTATION_PORTRAIT_UP		= 1,
	DEV_ORIENTATION_LANDSCAPE_LEFT	= 2,
	DEV_ORIENTATION_LANDSCAPE_RIGHT	= 3,
	DEV_ORIENTATION_FACE_UP			= 4,
	DEV_ORIENTATION_FACE_DOWN		= 5,
	DEV_ORIENTATION_UNKNOWN			= 255
}DEVICE_ORIENTATION;

//Masks to extract motion type from motion sensor data
#define MOTION_DATA_HI_MOTION_MASK		(0x40)
#define MOTION_DATA_FREEFALL_MASK		(0x80)


//-----------------------------------------------------------------------------
// Structure Definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//
// Struct:			SENSOR_VERSION_T
//
// Description:		This structure is used for representing version information 
//					of ensor sub system related components		
//-----------------------------------------------------------------------------
typedef struct _SENSOR_VERSION_T
{ 
	STRUCT_INFO	StructInfo; //Structure information for backward compatibility
	
	DWORD dwMajor;			// Major version number 
	DWORD dwMinor;			// Minor version Number 
	DWORD dwBuild;			// Build number
	TCHAR szVersion[128];	// Null terminated string containing version number
} SENSOR_VERSION_T, *PSENSOR_VERSION_T;


//-----------------------------------------------------------------------------
//
// Struct:			SENSOR_INFO
//
// Description:		This structure is used for the sensor enumeration and to 
//					get access to a specific sensor on the device. 		
//-----------------------------------------------------------------------------
typedef struct _SENSOR_INFO_T
{
	STRUCT_INFO			StructInfo; //Structure information for backward compatibility
	
	DWORD 				dwUID;			// unique identifier for a particular sensor 
	SENSOR_TYPE		eType;			// type of sensor
	SENSOR_CONTEXT 	eContext;		// Context of measurements being taken by sensor
	SENSOR_CONNECTION_TYPE 	eConnection;	// Connection type
	SAMPLING_TYPE 		eSamplingType;	// Sampling type
	DATA_UNIT			eUnit;			// Unit 
	TCHAR 				szName[128];	// Null terminated string of the friendly name
	TCHAR 				szVendor[128];	// Null terminated string of the vendor name
} SENSOR_INFO_T, *PSENSOR_INFO_T;


//-----------------------------------------------------------------------------
//
// Struct:			SENSOR_DATA_T
//
// Description:		This structure is used to retrieve data from a sensor
//-----------------------------------------------------------------------------
typedef struct _SENSOR_DATA_T
{
	STRUCT_INFO			StructInfo; //Structure information for backward compatibility
	
	SENSOR_DATA_TYPE eType;	// Type of the data 
	DWORD	dwTimeStamp;		// Time stamp relative to sample time of Data
	union _data					// Union definition to hold different types of Data
	{
		struct _vector			// Structure for Vector type Data
		{
			LONG		nX;		// X component of the vector data
			LONG		nY;		// Y component of the vector data
			LONG		nZ;		// Z component of the vector data
		}vector;
		struct _scalar			// Structure for Scalar type data
		{
			LONG nValue;		// Value of the scalar data
		}scalar;
	}data;
} SENSOR_DATA_T, * PSENSOR_DATA_T;

//-----------------------------------------------------------------------------
// Call back function pointer type definitions
//-----------------------------------------------------------------------------

//------------------------------------------------------------------------------
//
// Name			: PFN_SENSOR_PROPERTY_CALLBACK 
//
// Description	: Function pointer type for property changed event listener.
//
// Parameters	:	[IN]HANDLE - Handle of the sensor instance who's property 
//						has changed.
//					[IN]E_SENSOR_PROPERTY - Name of the changed property
//					[IN]LONG - New value of the changed property
//
// Returns		: DWORD - Must return 0; All other values reserved for future use.
//
//------------------------------------------------------------------------------ 
typedef DWORD (*PFN_SENSOR_PROPERTY_CALLBACK ) (HANDLE, SENSOR_PROPERTY, LONG);
//------------------------------------------------------------------------------
//
// Name			: PFN_SENSOR_DATA_CALLBACK
//
// Description	: Function pointer type for streaming data listener.
//
// Parameters	:	[IN]HANDLE - Handle of the sensor instance who's data 
//						is available.
//					[IN]SENSOR_DATA_T* - Data Sample buffer
//					[IN]DWORD - Size of the Sample buffer
//					[IN] DWORD - Number of data samples dropped from buffer and 
//						overwritten since last delivery of data.
//					
// Returns		: DWORD - Must return 0; All other values reserved for future use.
//
//------------------------------------------------------------------------------ 
typedef DWORD (*PFN_SENSOR_DATA_CALLBACK) (HANDLE,SENSOR_DATA_T*, DWORD, DWORD); 

//------------------------------------------------------------------
// Error code Definitions
//------------------------------------------------------------------

//Success
#define E_SENSOR_SUCCESS					0x0
//Specified function or parameter is unsupported 
#define E_SENSOR_UNSUPPORTED				0xE0000001
//Failed to access sensor Driver
#define E_SENSOR_FAILED_TO_ACCESS_DRIVER	0xE0000002
//Failed to access sensor service
#define E_SENSOR_FAILED_TO_ACCESS_SERVICE	0xE0000003
//A parameter specified is invalid
#define E_SENSOR_INVALID_PARAMETER			0xE0000004
//Failed to allocated memory
#define E_SENSOR_FAILED_TO_ALLOCATE_MEMORY	0xE0000005
//Failed to allocate/create a resource (an event, thread)
#define E_SENSOR_ALLOCATE_RESOURCE			0xE0000006
//Invalid handle is specified
#define E_SENSOR_INVALID_HANDLE				0xE0000007
//Sampling from sensor is active
#define E_SENSOR_SAMPLING_IN_PROGRESS		0xE0000008
//There is a already registered notifier for the sensor handle
#define E_SENSOR_NOTIFIER_EXISTS			0xE0000009
//No notifier registered for the specified sensor handle
#define E_SENSOR_NOTIFIER_DOES_NOT_EXIST	0xE000000A
//Failed to read data from registry
#define E_SENSOR_FAILED_TO_READ_REGISTRY	0xE000000B
//Specified sensor is not found
#define E_SENSOR_NOT_FOUND					0xE000000C
//No more items to return
#define E_SENSOR_NO_MORE_ITEMS				0xE000000D
//Another open instance of sensor is already in master mode (system wide)
#define E_SENSOR_MASTER_ALREADY_OPENED		0xE000000E
//Exception has occurred in driver
#define E_SENSOR_DRIVER_EXCEPTION			0xE000000F
//Error setting the parameter value
#define E_SENSOR_SETTING_PARAMS				0xE0000010
//Error setting value due to read only
#define E_SENSOR_READ_ONLY					0xE0000011
//No data to return
#define E_SENSOR_NO_DATA					0xE0000012
//Buffer size specified is less than existing notifier threshold
#define E_SENSOR_BUFFER_SMALLER_THAN_NOTIFIER_THRESHOLD	0xE0000013


#ifdef __cplusplus
extern "C" {
#endif

//------------------------------------------------------------------
// Exported function prototypes
//------------------------------------------------------------------

//------------------------------------------------------------------------------
//
// Name			: SENSOR_GetVersion
//
// Description	: Gets version numbers of sensor sub system components
//
// Parameters	:	[IN]SENSOR_SYSTEM_COMPONENT - Identifier of the SENSOR sub-system 
//								component
//					[OUT]SENSOR_VERSION_T - Pointer to pre allocated SENSOR_VERSION_T 
//								structure
//
// Returns		: DWORD - Indicates success/failure status
//
//------------------------------------------------------------------------------ 
DWORD SENSOR_GetVersion(SENSOR_SYSTEM_COMPONENT  eComponent, 
					 SENSOR_VERSION_T * ptVersion);
					 
//------------------------------------------------------------------------------
//
// Name			: SENSOR_FindFirst
//
// Description	: Find the first sensor of the type defined in SENSOR_INFO_T
//
// Parameters	:	[IN/OUT]SENSOR_INFO_T* - Pointer to pre allocated 
//								SENSOR_INFO_T structure.
//					[OUT]HANDLE * - Pointer to a HANDLE type variable. 
//								On function return this will hold handle which should 
//								be used in subsequent calls to SENSOR_FindNext() and 
//								SENSOR_FindClose() functions
//
// Returns		: DWORD - Indicates success/failure status
//
//------------------------------------------------------------------------------
DWORD SENSOR_FindFirst(SENSOR_INFO_T * ptSensorInfo, 
					HANDLE * phFindHandle);

//------------------------------------------------------------------------------
//
// Name			: SENSOR_FindNext
//
// Description	: Gets next available sensor
//
// Parameters	:	[IN]HANDLE - Handle to the find instance returned from 
//								successful call to SENSOR_FindFirst() function
//					[OUT]SENSOR_INFO_T * - Pointer to pre allocated SENSOR_INFO_T 
//								structure. This would be filled with details of the
//								next matching device upon successful return
//
// Returns		: DWORD - Indicates success/failure status
//
//------------------------------------------------------------------------------ 
DWORD SENSOR_FindNext(HANDLE hFindHandle, 
				   SENSOR_INFO_T * ptSensorInfo);

//------------------------------------------------------------------------------
//
// Name			: SENSOR_FindClose
//
// Description	: Closes the find file handle and free resources.
//
// Parameters	:	[IN]HANDLE - Handle to the find instance returned from 
//								successful call to SENSOR_FindFirst() or SENSOR_FindNext()
//								function
//		
// Returns		: DWORD - Indicates success/failure status
//
//------------------------------------------------------------------------------ 
DWORD SENSOR_FindClose(HANDLE hFindHandle);


//------------------------------------------------------------------------------
//
// Name			: SENSOR_Open
//
// Description	: Gets access to a particular sensor specified by "
//								ptSensorInfo.eSensorType" or "ptSensorInfo.dwUID". 
//
// Parameters	:	
//						[IN] OPEN_MODES - Sensor open mode
//						[IN/OUT]SENSOR_INFO_T * - Pointer to pre allocated
//								SENSOR_INFO_T structure returned from SENSOR_FindFirst() 
//								or SENSOR_FindNext(). Only "dwSize", "dwUID" and	or "eSensorType" 
//								fields are relevant as inputs. On successful return remaining 
//								fields of ptSensorInfo will be filled with relevant information.
//					[OUT]HANDLE* - On successful return, this will contain a 
//								handle to the opened sensor instance.
//
// Returns		: DWORD - Indicates success/failure status
//
// Notes			: "dwUID" will have precedence, if both "dwUID" and "eSensorType" 
//							are specified when calling this	function.
//
//------------------------------------------------------------------------------ 
DWORD SENSOR_Open(OPEN_MODE eMode, SENSOR_INFO_T * ptSensorInfo, 
		HANDLE * phSensor);

//------------------------------------------------------------------------------
//
// Name			: SENSOR_Close
//
// Description	: Closes specified sensor instance handle and frees resources
//
// Parameters	:	[IN]HANDLE - Sensor instance HANDLE to be closed 
//
// Returns		: DWORD - Indicates success/failure status
//
//------------------------------------------------------------------------------
DWORD SENSOR_Close(HANDLE hSensor);

//------------------------------------------------------------------------------
//
// Name			: SENSOR_GetProperty
//
// Description	: Gets current value of a property specified in eProperty
//
// Parameters	:	[IN]HANDLE - Sensor instance opened with SENSOR_Open()
//					[IN]SENSOR_PROPERTY - Name of the property for which 
//									information should be retrieved
//					[OUT]LONG* - Pointer to pre allocated LONG type variable.
//								On successful return this will contain the current 
//								value of the property
//					
// Returns		: DWORD - Indicates success/failure status
//
//------------------------------------------------------------------------------
DWORD SENSOR_GetProperty(HANDLE hSensor, 
		SENSOR_PROPERTY eProperty, LONG * pnValue);

//------------------------------------------------------------------------------
//
// Name			: SENSOR_SetProperty
//
// Description	: Sets value to a specified sensor property
//
// Parameters	:	[IN]HANDLE - Sensor instance opened with SENSOR_Open()
//					[IN]SENSOR_PROPERTY - Name of the property for which 
//									value to be set
//					[IN]LONG - LONG variable containing value to be set
//
// Returns		: DWORD - Indicates success/failure status
//
//------------------------------------------------------------------------------
DWORD SENSOR_SetProperty(HANDLE hSensor, 
		SENSOR_PROPERTY eProperty, LONG  nValue);		

//------------------------------------------------------------------------------
//
// Name			: SENSOR_GetSensorData
//
// Description	: Gets an synchronous data sample from sensor
//
// Parameters	:	[IN]HANDLE - Sensor instance opened with SENSOR_Open()
//					[OUT]SENSOR_DATA_T * - address of SENSOR_DATA_T structure
//					
// Returns		: DWORD - Indicates success/failure status
//
//------------------------------------------------------------------------------
DWORD SENSOR_GetSensorData(HANDLE hSensor, SENSOR_DATA_T * ptData);

//------------------------------------------------------------------------------
//
// Name			: SENSOR_SetBufferSize
//
// Description	: Sets the maximum number of samples that can be stored in the
//				  internal queue for bufferred data reads. Samples returned by
//				  calling GetSampledData will be removed from the queue. If the
//				  queue fills up, old data samples will be overwritten with new
//				  ones.
//
// Parameters	: [IN]HANDLE - Sensor instance opened with SENSOR_Open()
//				  [IN]DWORD - Maximum number of samples retained in the queue 
//						(length of the queue)
//					
// Returns		: DWORD - Indicates success/failure status
//
//------------------------------------------------------------------------------
DWORD SENSOR_SetBufferSize(HANDLE hSensor, DWORD dwMaxSampleCount);


//------------------------------------------------------------------------------
//
// Name			: SENSOR_ClearBuffer
//
// Description	: Empties the internal data queue.
//
// Parameters	: [IN]HANDLE - Sensor instance opened with SENSOR_Open()
//					
// Returns		: DWORD - Indicates success/failure status
//
//------------------------------------------------------------------------------
DWORD SENSOR_ClearBuffer(HANDLE hSensor);

//------------------------------------------------------------------------------
//
// Name			: SENSOR_StartSampling
//
// Description	: Starts sampling data and queue them in internal queue
//
// Parameters	: [IN]HANDLE - Sensor instance opened with SENSOR_Open()
//					
// Returns		: DWORD - Indicates success/failure status
//
//------------------------------------------------------------------------------
DWORD SENSOR_StartSampling(HANDLE hSensor);

//------------------------------------------------------------------------------
//
// Name			: SENSOR_StopSampling
//
// Description	: Stops sampling from specified sensor
//
// Parameters	: [IN]HANDLE - Sensor instance opened with SENSOR_Open()
//					
// Returns		: DWORD - Indicates success/failure status
//
//------------------------------------------------------------------------------
DWORD SENSOR_StopSampling(HANDLE hSensor);

//------------------------------------------------------------------------------
//
// Name			: SENSOR_GetBufferData
//
// Description	: Gets the given number of samples from the internal data queue
//				  and fills the given buffer with the samples. 
//
// Parameters	: [IN]HANDLE - Sensor instance opened with SENSOR_Open()
//				  [IN/OUT]SENSOR_DATA_T* - SENSOR_DATA_T type array to fill with data
//				  [IN/OUT]DWORD * - Pointer to DWORD containg length of the array in 
//							number of data samples. On successful return it will 
//							contain the number of samples filled.
//				  [OUT] DWORD * - A pointer to preallocated DWORD type variable. 
//						Which will contain number of data samples dropped from buffer and 
//						overwritten since last delivery of data.
//					
// Returns		: DWORD - Indicates success/failure status
//
//------------------------------------------------------------------------------
DWORD SENSOR_GetBufferData(HANDLE hSensor, SENSOR_DATA_T* ptDataBuffer, DWORD* pdwSampleCount, DWORD* pdwDropCount);


//------------------------------------------------------------------------------
//
// Name			: SENSOR_RegisterDataNotification
//
// Description	: Register for a notification (Callback, Event etc) to get
//				  notified when the given number of samples has been recieved
//				  from the sensor
//
// Parameters	:[IN]HANDLE - Sensor instance opened with SENSOR_Open()
//				 [IN]NOTIFIER_TYPE - Enum value defining the notification type
//				 [IN]LPVOID - Parameter containing the structure relevant to the
//					notification type. For callbacks this is a function pointer.
//					For events it's a HANDLE.
//				 [IN]DWORD - Number of samples which should be in the sample
//					queue to trigger a notification
//					
// Returns		: DWORD - Indicates success/failure status
//
//------------------------------------------------------------------------------
DWORD SENSOR_RegisterDataNotification(HANDLE hSensor, NOTIFIER_TYPE eNotifierType, LPVOID lpNotifier, DWORD dwNotifyThreshold);


//------------------------------------------------------------------------------
//
// Name			: SENSOR_DeregisterDataNotification
//
// Description	: Remove the notifier.
//
// Parameters	: [IN]HANDLE - Sensor instance opened with SENSOR_Open()
//					
// Returns		: DWORD - Indicates success/failure status
//
//------------------------------------------------------------------------------
DWORD SENSOR_DeregisterDataNotification(HANDLE hSensor);


//------------------------------------------------------------------------------
//
// Name			: SENSOR_RegisterPropertyNotification
//
// Description	: 
//
// Parameters	: [IN]HANDLE - Sensor instance opened with SENSOR_Open()
//				  [IN]NOTIFIER_TYPE - Enum value defining the notification type
//				  [IN]LPVOID - Parameter containing the structure relevant to the
//					 notification type. For callbacks this is a function pointer.
//					 For events it's a HANDLE.
// Returns		: DWORD - Indicates success/failure status
//
//------------------------------------------------------------------------------
DWORD SENSOR_RegisterPropertyNotification(HANDLE hSensor, NOTIFIER_TYPE eNotifierType, LPVOID lpNotifier);


//------------------------------------------------------------------------------
//
// Name			: SENSOR_DeregisterPropertyNotification
//
// Description	: 
//
// Parameters	: [IN]HANDLE - Sensor instance opened with SENSOR_Open()
//					
// Returns		:	DWORD - Indicates success/failure status
//
//------------------------------------------------------------------------------
DWORD SENSOR_DeregisterPropertyNotification(HANDLE hSensor);


//------------------------------------------------------------------------------
//
// Name			: SENSOR_GetChangedProperty
//
// Description	: UnRegisters a previously regstered data signalling event
//
// Parameters	:	[IN]HANDLE - Sensor instance opened with SENSOR_Open()
//					[IN]HANDL - handle of the signaled event
//					[OUT]E_SENSOR_PROPERTY * - pointer to pre allocated E_SENSOR_PROPERTY
//								type variable. It will contain type of the property which 
//								was signaled.
//					[OUT] LONG * - pointer to pre allocated LONG type variable. It will 
//								contain the new value of the changed property.
//					
// Returns		: DWORD - Indicates success/failure status
//
//------------------------------------------------------------------------------
DWORD SENSOR_GetChangedProperty(HANDLE hSensor, SENSOR_PROPERTY *pProp, LONG * plNewValue); 

#ifdef __cplusplus
}
#endif


