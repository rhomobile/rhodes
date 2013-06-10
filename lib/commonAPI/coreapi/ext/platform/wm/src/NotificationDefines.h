
//--------------------------------------------------------------------
// FILENAME:			NtfyDef.h
//
// Copyright(c) 1999,2000 Symbol Technologies Inc. All rights reserved.
//
// DESCRIPTION:			Constants and types used by notification functions
//
// NOTES:				Public
//
// 
//--------------------------------------------------------------------


#ifndef NTFYDEF_H_
#define NTFYDEF_H_

#ifdef __cplusplus
extern "C"
{
#endif


//--------------------------------------------------------------------
// Nested Includes
//--------------------------------------------------------------------

#include <StrucInf.h>


//--------------------------------------------------------------------
// Constants
//--------------------------------------------------------------------


#define NOTIFYAPI WINAPI

#define MAX_OBJECT_NAME MAX_PATH


//--------------------------------------------------------------------
// Types
//--------------------------------------------------------------------


enum tagNOTIFY_OBJECT_TYPES
{
	NOTIFY_TYPE_LED			= 0,
	NOTIFY_TYPE_BEEPER		= 1,
	NOTIFY_TYPE_VIBRATOR	= 2,
	
	NOTIFY_TYPE_UNKNOWN		= -1
};


enum tag_NOTIFY_OBJECT_STATES
{
	NOTIFY_STATE_OFF		= 0,
	NOTIFY_STATE_ON			= 1,
	NOTIFY_STATE_CYCLE		= 2,
	
	NOTIFY_STATE_UNKNOWN	= -1
};


// Find information structure
//   Find information structure is filled in by C API by
//     enumerating and reading the registry keys under
//     ..\Drivers\Active\..  and ..\Drivers\BuiltIn\..

typedef struct tagNOTIFY_FINDINFO_W
{
	STRUCT_INFO 	StructInfo;

	WCHAR szObjectName[MAX_OBJECT_NAME];
	DWORD dwObjectType;
	DWORD dwObjectIndex;

} NOTIFY_FINDINFO_W;

typedef NOTIFY_FINDINFO_W FAR * LPNOTIFY_FINDINFO_W;

typedef struct tagNOTIFY_FINDINFO_A
{
	STRUCT_INFO 	StructInfo;

	CHAR szObjectName[MAX_OBJECT_NAME];
	DWORD dwObjectType;
	DWORD dwObjectIndex;

} NOTIFY_FINDINFO_A;

typedef NOTIFY_FINDINFO_A FAR * LPNOTIFY_FINDINFO_A;

#ifdef UNICODE
#define NOTIFY_FINDINFO NOTIFY_FINDINFO_W
#define LPNOTIFY_FINDINFO LPNOTIFY_FINDINFO_W
#else
#define NOTIFY_FINDINFO NOTIFY_FINDINFO_A
#define LPNOTIFY_FINDINFO LPNOTIFY_FINDINFO_A
#endif


// Version information structure
typedef struct tagNOTIFY_VERSION_INFO
{
	STRUCT_INFO 	StructInfo;

	DWORD			dwCAPIVersion;	// HIWORD = major ver LOWORD = minor ver.

} NOTIFY_VERSION_INFO;

typedef NOTIFY_VERSION_INFO FAR * LPNOTIFY_VERSION_INFO;


typedef struct tagLED_SPECIFIC
{
	DWORD dwOnDuration;		// Duration LED is on per cycle in milliseconds

	DWORD dwOffDuration;	// Duration LED is off per cycle in milliseconds

	DWORD dwCount;			// Number of cycles (on+off) to generate

} LED_SPECIFIC;


typedef struct tagBEEPER_SPECIFIC
{
	DWORD dwFrequency;		// Frequency of beep in Hertz

	DWORD dwVolume;			// Volume of beep

	DWORD dwDuration;		// Duration of beep in milliseconds

} BEEPER_SPECIFIC;


typedef struct tagVIBRATOR_SPECIFIC
{
	DWORD dwDuration;		// Duration of vibration in milliseconds

} VIBRATOR_SPECIFIC;


typedef union tagOBJECT_TYPE_SPECIFIC
{
	DWORD dwUntyped[20];					// For init and anonymous access

	LED_SPECIFIC LedSpecific;

	BEEPER_SPECIFIC BeeperSpecific;

	VIBRATOR_SPECIFIC VibratorSpecific;

} OBJECT_TYPE_SPECIFIC;


typedef struct tagCYCLE_INFO
{
	STRUCT_INFO 	StructInfo;

	DWORD dwObjectType;

	OBJECT_TYPE_SPECIFIC ObjectTypeSpecific;

} CYCLE_INFO;

typedef CYCLE_INFO FAR * LPCYCLE_INFO;


#ifdef __cplusplus
}
#endif

#endif	// #ifndef NTFYDEF_H_	

