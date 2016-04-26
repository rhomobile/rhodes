//--------------------------------------------------------------------
// FILENAME:			DiagnosticAPI32.h
//
// Copyright(c) 2014 Symbol Technologies, Inc.  All rights reserved.
//
// DESCRIPTION:			Diagnostic API DLL header.
//
// NOTES:				Public	
//
// AUTHOR:				Kavyashree D
// CREATION DATE:		7/11/2013
// DERIVED FROM:		New File
//
// EDIT HISTORY:

//********************************************************************
#ifndef  DIAGNOSTICAPI32_H_

#define  DIAGNOSTICAPI32_H_

#include <StrucInf.h>


#define 	DIAGAPI WINAPI
#define 	MANF_STRING_LEN 			36 // manfucture date will be of type 10/10/2000
#define 	DIAG_RET_SUCCESS			0
#define 	DIAG_RET_FAILURE 			0xFFFF


typedef enum{
		DIAG_BATT_HEALTH_PERCENT = 0x1, 
		DIAG_BATT_CAPACITY_MINS = 0x1<<1, 
		DIAG_BATT_CAPACITY_PERCENT = 0x1<<2,
		DIAG_BATT_MANUFACTURE_DATE = 0x1<<3, 
		DIAG_BATT_APPROX_REMAINING_LIFE_MONTHS = 0x1<<4,
		DIAG_BATT_REQ_TIME_IN_CRADLE_MINS = 0x1<<5,
		DIAG_BATT_TIME_SINCE_LAST_BATT_REPLACE_DAYS = 0x1<<6,
		DIAG_BATT_TIME_ELAPSED_CHARGING_MINS = 0x1<<7,
		DIAG_GEN_TIME_SINCE_LAST_REBOOT_MINS = 0x1<<8,
		DIAG_BATT_ALL = DIAG_BATT_HEALTH_PERCENT|DIAG_BATT_CAPACITY_MINS|DIAG_BATT_CAPACITY_PERCENT|
								DIAG_BATT_MANUFACTURE_DATE|DIAG_BATT_APPROX_REMAINING_LIFE_MONTHS|
								DIAG_BATT_REQ_TIME_IN_CRADLE_MINS|DIAG_BATT_TIME_SINCE_LAST_BATT_REPLACE_DAYS|DIAG_BATT_TIME_ELAPSED_CHARGING_MINS,
		DIAG_ALL = DIAG_BATT_ALL |DIAG_GEN_TIME_SINCE_LAST_REBOOT_MINS
} EDIAGPARAMID;

//Input structure to Diagnostic driver
typedef struct tagDIAG_IN_DATA
{		
		STRUCT_INFO StructInfo;
		DWORD dwTripMin ;  //shopping trip in minutes
		DWORD  dwAvgCurrent ; //Average current consumption.
}DIAG_IN_DATA_t;

typedef DIAG_IN_DATA_t  * PDIAG_IN_DATA;

//structure to which data ill be filled from diagnostic driver
typedef struct tagDIAG_OUT_DATA
{		
		STRUCT_INFO StructInfo;
		DWORD dwBattHlthPer;
		DWORD dwBattCapInMins;
		DWORD dwBattCapInPercent;
		DWORD dwBattRemainingLifeMnths;
		DWORD dwBattReqTimeinCradleMins;
		DWORD dwTimSincLstBattReplDays;
		DWORD dwTimSincLstRebootMins;
		DWORD dwTimElapsedChargingMins;
		WCHAR szManufacturingDate[MANF_STRING_LEN];
} DIAG_OUT_DATA_t;

typedef DIAG_OUT_DATA_t  * PDIAG_OUT_DATA;

//Diagnostic API for Application to get diagnostic data
DWORD DIAGAPI DIAG_GetDiagnosticData (
        EDIAGPARAMID eParamId, 
        PDIAG_IN_DATA pInData, 
        PDIAG_OUT_DATA pOutData);



#endif	// #ifndef DIAGNOSTICAPI32_H_

