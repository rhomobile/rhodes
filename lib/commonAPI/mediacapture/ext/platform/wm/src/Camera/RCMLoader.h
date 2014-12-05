#pragma once
#include <windows.h>
#include "RcmCAPI.h"

//  Dynamically Loading the RCM DLL
//  Definitions of Methods exported from the EMDK RCM
#define RCM_DLL L"Rcm2Api32.dll"	///<  Name of the RCM Library
typedef DWORD (WINAPI* LPFN_RCM_OPEN_T)	(void);
typedef DWORD (WINAPI* LPFN_RCM_GETSMARTBATTERYSTATUS_T)(PSMART_BATTERY_STATUS pStatus);
typedef DWORD (WINAPI* LPFN_RCM_REGISTERTRIGGEREVENT_T)	(DWORD dwTriggerMask, BOOL bExclusive, LPTSTR lpszEventName, LPHANDLE lphHandle);
typedef DWORD (WINAPI* LPFN_RCM_DEREGISTERTRIGGER_T)	(HANDLE);
typedef DWORD (WINAPI* LPFN_RCM_GETTRIGGERSTATUS_T)		(LPDWORD);
typedef DWORD (WINAPI* LPFN_RCM_GETUNIQUEUNITIDEX_T)	(LPUNITID_EX lpUnitIdEx);

class CRcmLoader
{
public:

	HMODULE	m_hRcmDLL;													///<  HMODULE of the loaded EMDK RCM DLL
	LPFN_RCM_OPEN_T						lpfn_RCM_Open;					///<  Function pointer to EMDK Rcm API
	LPFN_RCM_GETSMARTBATTERYSTATUS_T	lpfn_RCM_GetSmartBatteryStatus;	///<  Function pointer to EMDK Rcm API
	LPFN_RCM_REGISTERTRIGGEREVENT_T		lpfn_RCM_RegisterTriggerEvent;	///<  Function pointer to EMDK Rcm API
	LPFN_RCM_DEREGISTERTRIGGER_T		lpfn_RCM_DeregisterTrigger;		///<  Function pointer to EMDK Rcm API
	LPFN_RCM_GETTRIGGERSTATUS_T			lpfn_RCM_GetTriggerStatus;		///<  Function pointer to EMDK Rcm API
	LPFN_RCM_GETUNIQUEUNITIDEX_T		lpfn_RCM_GetUniqueUnitIdEx;		///<  Function pointer to EMDK Rcm API

	CRcmLoader();
	~CRcmLoader();
	
	BOOL LoadRcmDLL();
	BOOL UnloadRcmDLL();

};