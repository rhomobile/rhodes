#ifndef __CRADLEAPI32_H
#define __CRADLEAPI32_H


#include <StrucInf.h>
#include "cradle_err.h"

#define		CRADLEAPI			 					WINAPI

// structre definition for cradle version
typedef struct _tagCRADLE_VERSION_INFO
{
    STRUCT_INFO StructInfo;	 // Information about structure, HIWORD = major, LOWORD = minor
    DWORD dwCAPIVersion;
    DWORD dwDriverVersion;
    DWORD dwFirmwareVersion;
} CRADLE_VERSION_INFO;
typedef CRADLE_VERSION_INFO FAR * LPCRADLE_VERSION_INFO;

// structure definition for cradle unlock LED configration
typedef struct _tagCRADLE_UNLOCK_LED_INFO
{
    STRUCT_INFO StructInfo; // struct info
    DWORD dwOnDuration;
    DWORD dwOffDuration;
} CRADLE_UNLOCK_LED_INFO;
typedef CRADLE_UNLOCK_LED_INFO FAR* LPCRADLE_UNLOCK_LED_INFO;

// structure definition for cradle matrix
typedef struct _tagCRADLE_MATRIX_CONFIG
{
    STRUCT_INFO StructInfo;				// struct info
    DWORD   dwRowID;					// row ID number
    DWORD   dwColumnID;					// column ID number
    DWORD   dwWallID;					// wall ID number
} CRADLE_MATRIX_CONFIG;

typedef CRADLE_MATRIX_CONFIG FAR * LPCRADLE_MATRIX_CONFIG;


// structure definition for cradle manufacture info
typedef struct _tagCRADLE_MANUFACTURE_INFO
{
    DWORD  dwDate;					// manufacture date
    DWORD  dwMonth;					// manufacture month
    DWORD  dwYear;					// manufacture year
} CRADLE_MANUFACTURE_INFO;

typedef CRADLE_MANUFACTURE_INFO FAR * LPCRADLE_MANUFACTURE_INFO;

// structure definition for cradle device info
typedef struct _tagCRADLE_DEVICE_INFO
{
    STRUCT_INFO StructInfo; 						// struct info
    WCHAR  szSerialNumber[16];						// cradle serial number (15 bytes + '\0')
    WCHAR  szPartNumber[20];						// cradle part number (18 bytes + '\0' + blank)
    CRADLE_MANUFACTURE_INFO cradleManufactureInfo;	// cradle manufacture date info
    DWORD  dwHardwareID;							// cradle hardware ID

} CRADLE_DEVICE_INFO;

typedef CRADLE_DEVICE_INFO FAR *LPCRADLE_DEVICE_INFO;

#ifdef __cplusplus
extern "C"
{
#endif

DWORD CRADLEAPI Cradle_Open(
    PHANDLE hndCradleInstance
);
DWORD CRADLEAPI Cradle_Close(
    HANDLE hndCradleInstance
);
DWORD CRADLEAPI Cradle_GetVersion(
    HANDLE hndCradleInstance,
    LPCRADLE_VERSION_INFO lpCradleVersionInfo
);

DWORD CRADLEAPI Cradle_Unlock(
    HANDLE hndCradleInstance,
    BYTE   byTimeoutPeriod,
    LPCRADLE_UNLOCK_LED_INFO lpUnlockLedInfo
);

DWORD CRADLEAPI Cradle_GetCradleInfo(
    HANDLE hndCradleInstance,
    LPCRADLE_DEVICE_INFO szGetCradleDeviceInfo
);

DWORD CRADLEAPI Cradle_GetCradleConfig(
    HANDLE hndCradleInstance,
    LPCRADLE_MATRIX_CONFIG szGetCradleMatrixConfig
);


DWORD CRADLEAPI Cradle_SetCradleConfig(
    HANDLE hndCradleInstance,
    LPCRADLE_MATRIX_CONFIG szSetCradleMatrixConfig
);


DWORD CRADLEAPI Cradle_GetFastChargeState(
    HANDLE hndCradleInstance,
    PBOOL pbCradleChrgState
);


DWORD CRADLEAPI Cradle_SetFastChargeState(
    HANDLE hndCradleInstance,
    BOOL  bFastChrgState
);

#ifdef __cplusplus
}
#endif


#endif // __CRADLE_API_32_H
