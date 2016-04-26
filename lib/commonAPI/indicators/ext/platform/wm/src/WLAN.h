//  DCC (28/03/2013): This code is lifted without significant modification from RhoElements 1

/******************************************************************************/
#include <windows.h>
#include "FusionPublicAPI.h"
#include "logging/RhoLog.h"

/******************************************************************************/
#pragma once

/** Macro giving count of elements in an array */
#define COUNTOF(x) (sizeof(x)/sizeof(x[0]))

/******************************************************************************/
typedef DWORD (WINAPI* LPFN_OPEN_FUSION_API)   (PDWORD,FAPI_ACCESS_TYPE,PTCHAR);
typedef DWORD (WINAPI* LPFN_CLOSE_FUSION_API)  (DWORD);
typedef	DWORD (WINAPI* LPFN_COMMAND_FUSION_API)(DWORD,DWORD,PVOID,DWORD,PVOID,DWORD, PDWORD);

/******************************************************************************/
struct ADAPTER_STATUS
{
	int nSignalStrength;		// Percentage
	WCHAR szSSID [32];
	BYTE bMacAddress [6];
	WCHAR szName [64];
	WCHAR szDHCPAddress [16];
	BOOL bStatic;
	WCHAR szGatewayAddress [16];
	WCHAR szIpAddress [16];
	int nRSSI;
	WCHAR szSubnetMask [16];
	WCHAR szWinsAddress [16];
};

/******************************************************************************/
class CWLAN
{
public:
	CWLAN();
	virtual ~CWLAN();

	BOOL Open (void);
	void Close (void);
	int GetSignalStrength (void);		// 0 (weakest) to 5 (strongest)
	BOOL GetAdapterStatus (ADAPTER_STATUS *pstatus);

private:
	HMODULE hFusionModule;
	LPFN_OPEN_FUSION_API lpfnOpenFusion;
	LPFN_CLOSE_FUSION_API lpfnCloseFusion;
	LPFN_COMMAND_FUSION_API lpfnCommandFusion;
	DWORD dwFusionContext;
	DWORD dwAdapterHandle;
	BYTE bMacAddress [6];
	ULONG ulAdaptersInfoSize;
};
