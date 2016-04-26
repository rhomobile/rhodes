//  DCC (28/03/2013): This code is lifted without significant modification from RhoElements 1

/******************************************************************************/
#include <windows.h>

#include <iphlpapi.h>
#pragma comment(lib, "iphlpapi.lib")

#include "WLAN.h"
//#include "..\..\..\Common\Public\PBUtil.h"
#include <ntddndis.h>

/******************************************************************************/
CWLAN::CWLAN()
{
	hFusionModule = 0;
	lpfnOpenFusion = NULL;
	lpfnCloseFusion = NULL;
	lpfnCommandFusion = NULL;
}

/******************************************************************************/
CWLAN::~CWLAN()
{
	Close ();
}

/******************************************************************************/
BOOL CWLAN::Open (void)
{
	DWORD size;
	BYTE *pbuffer;
	FAPI_AdapterIDHeader *pheader;
	FAPI_AdapterLink *plink;
	FAPI_AdapterInfo info;
	BOOL ok;

	// Already opened?
	if (hFusionModule)
		return TRUE;

	ok = FALSE;
	pbuffer = NULL;

	// Load Fusion library
	hFusionModule = LoadLibrary (L"FusionPublicAPI.dll");
	if (!hFusionModule)
		goto Exit;

	lpfnOpenFusion = (LPFN_OPEN_FUSION_API) GetProcAddress(hFusionModule, L"OpenFusionAPI");
	lpfnCloseFusion = (LPFN_CLOSE_FUSION_API) GetProcAddress(hFusionModule, L"CloseFusionAPI");
	lpfnCommandFusion = (LPFN_COMMAND_FUSION_API) GetProcAddress(hFusionModule, L"CommandFusionAPI");

	if (!lpfnOpenFusion || !lpfnCloseFusion || !lpfnCommandFusion)
		goto Exit;

	// Open Fusion API
	if ((*lpfnOpenFusion) (&dwFusionContext, STAT_MODE, L"PB3.0") != FAPI_SUCCESS)
		goto Exit;

	// Get size of buffer needed for adapter enumeration
	if ((*lpfnCommandFusion) (dwFusionContext, ADAPTER_WLAN_GET_BUFFER_SIZE, NULL, 0, &size, sizeof size, NULL) != FAPI_SUCCESS)
		goto Exit;

	// Allocate buffer
	pbuffer = new BYTE [size];

	// Get adapter enumeration data
	if ((*lpfnCommandFusion) (dwFusionContext, ADAPTER_WLAN_GET_ENUM_DATA, NULL, 0, pbuffer, size, NULL) != FAPI_SUCCESS)
		goto Exit;

	// Check for one or more adapters found
	pheader = (FAPI_AdapterIDHeader*) pbuffer;
	if (pheader->numAdapters == 0)
		goto Exit;

	// Get handle of first adapter
	plink = (FAPI_AdapterLink*) (pbuffer + sizeof (FAPI_AdapterIDHeader));
	dwAdapterHandle = plink->Pointer.pWLANAdapterID->dwAdapterHandle;

	// Get adapter information
	if ((*lpfnCommandFusion) (dwFusionContext, ADAPTER_INFO_WLAN, &dwAdapterHandle, sizeof dwAdapterHandle, &info, sizeof info, NULL) != FAPI_SUCCESS)
		goto Exit;

	// Store MAC address of adapter
	memcpy (bMacAddress, &info.macAddr, 6);

	// Find size needed for later calls to GetAdaptersInfo
	if (GetAdaptersInfo (NULL, &ulAdaptersInfoSize) != ERROR_BUFFER_OVERFLOW)
		goto Exit;

	ok = TRUE;

Exit:
	if (pbuffer)
		delete [] pbuffer;

	if (!ok)
	{
		FreeLibrary (hFusionModule);
		hFusionModule = 0;
	}

	return ok;
}

/******************************************************************************/
void CWLAN::Close ()
{
	if (hFusionModule)
	{
		if (lpfnCloseFusion)
			(*lpfnCloseFusion) (dwFusionContext);

		lpfnOpenFusion = NULL;
		lpfnCloseFusion = NULL;
		lpfnCommandFusion = NULL;

		FreeLibrary (hFusionModule);
		hFusionModule = 0;
	}
}

/******************************************************************************/
int CWLAN::GetSignalStrength (void)
{
	int strength;

	if (!hFusionModule)
		return 0;

	if (!lpfnCommandFusion)
		return 0;

	(*lpfnCommandFusion) (dwFusionContext, RF_SIGNAL_STRENGTH_WLAN_GET, &dwAdapterHandle, sizeof dwAdapterHandle,
		&strength, sizeof strength, NULL);

	if (strength < -97)
		return 0;

	if (strength < -90)
		return 1;

	if (strength < -80)
		return 2;

	if (strength < -70)
		return 3;

	if (strength < -60)
		return 4;

	return 5;
}

/******************************************************************************/
BOOL CWLAN::GetAdapterStatus (ADAPTER_STATUS *pstatus)
{
	BYTE *pbuffer;
	IP_ADAPTER_INFO *pinfo, *pmatch;
	ULONG size;
	DWORD error;
	int rssi;
	FAPI_AdapterNDISStat ndis;
	NDIS_802_11_SSID *pssid;
	BOOL ok;

	ok = FALSE;
	pbuffer = NULL;

	// Allocate buffer using previously discovered size
	// Assumes no new adapters have been added since but saves an API call
	// Re-allocates if necessary
	pbuffer = new BYTE [ulAdaptersInfoSize];
	pinfo = (IP_ADAPTER_INFO*) pbuffer;
	size = ulAdaptersInfoSize;
	error = GetAdaptersInfo (pinfo, &size);

	if (error == ERROR_BUFFER_OVERFLOW)
	{
		delete [] pbuffer;
		ulAdaptersInfoSize = size;
		pbuffer = new BYTE [ulAdaptersInfoSize];
		pinfo = (IP_ADAPTER_INFO*) pbuffer;
		size = ulAdaptersInfoSize;
		error = GetAdaptersInfo (pinfo, &size);
	}

	if (error != NO_ERROR)
		goto Exit;

	// Find matching adapter in list by comparing MAC addresses
	pmatch = NULL;
	while (pinfo)
	{
		if (!memcmp (bMacAddress, pinfo->Address, 6))
		{
			pmatch = pinfo;
			break;
		}

		pinfo = pinfo->Next;
	}

	// Found it?
	if (!pmatch)
	{
		LOG(WARNING) + "Unable to find matching WLAN adapter";
		goto Exit;
	}

	// Copy information to caller
	memset (pstatus, 0, sizeof (ADAPTER_STATUS));

	memcpy (pstatus->bMacAddress, bMacAddress, 6);
	mbstowcs (pstatus->szName, pmatch->AdapterName, COUNTOF(pstatus->szName) - 1);

	if (pmatch->DhcpEnabled)
	{
		pstatus->bStatic = FALSE;
		mbstowcs (pstatus->szDHCPAddress, pmatch->DhcpServer.IpAddress.String, 15);//COUNTOF(pstatus->szDHCPAddress) - 1);
	}
	else
		pstatus->bStatic = TRUE;

	mbstowcs (pstatus->szGatewayAddress, pmatch->GatewayList.IpAddress.String, COUNTOF(pstatus->szGatewayAddress) - 1);
	mbstowcs (pstatus->szIpAddress, pmatch->CurrentIpAddress->IpAddress.String, COUNTOF(pstatus->szIpAddress) - 1);
	mbstowcs (pstatus->szSubnetMask, pmatch->CurrentIpAddress->IpMask.String, COUNTOF(pstatus->szSubnetMask) - 1);

	if (pmatch->HaveWins)
		mbstowcs (pstatus->szWinsAddress, pmatch->PrimaryWinsServer.IpAddress.String, COUNTOF(pstatus->szWinsAddress) - 1);

	// Get signal strength via Fusion
	DWORD dwRetVal = (*lpfnCommandFusion) (dwFusionContext, RF_SIGNAL_STRENGTH_WLAN_GET, &dwAdapterHandle, sizeof dwAdapterHandle, &rssi, sizeof rssi, NULL);
	if (dwRetVal != FAPI_SUCCESS)
	{
		LOG(WARNING) + "Call to Fusion API did not return success, actual return: " + dwRetVal;
		goto Exit;
	}

	pstatus->nRSSI = rssi;

	//  Note these values were changed in RhoElements 4.0 to match the percentages reported by Fusion.
	if (rssi < -97) pstatus->nSignalStrength = 0;
	else if (rssi < -90) pstatus->nSignalStrength = 0;
	else if (rssi < -80) pstatus->nSignalStrength = 20;
	else if (rssi < -70) pstatus->nSignalStrength = 40;
	else if (rssi < -60) pstatus->nSignalStrength = 60;
	else if (rssi < -50) pstatus->nSignalStrength = 80;
	else pstatus->nSignalStrength = 100;

	// Get SSID via NDIS
	memset (&ndis, 0, sizeof ndis);
	ndis.dwVersion = FAPI_ADAPTER_NDIS_STAT_VERSION;
	ndis.dwType = FAPI_ADAPTER_NDIS_STAT_TYPE;
	ndis.dwAdapterHandle = dwAdapterHandle;
	ndis.dwOid = OID_802_11_SSID;
	(*lpfnCommandFusion) (dwFusionContext, NDIS_STAT_WLAN_GET_BUFFER_SIZE, &ndis, sizeof ndis, &size, sizeof size, NULL);
	delete [] pbuffer;
	pbuffer = new BYTE [size];
	(*lpfnCommandFusion) (dwFusionContext, NDIS_STAT_WLAN_GET_DATA, &ndis, sizeof ndis, pbuffer, size, NULL);
	pssid = (NDIS_802_11_SSID*) pbuffer;

	if (pssid->SsidLength < COUNTOF(pstatus->szSSID))
	{
		mbstowcs (pstatus->szSSID, (char*) pssid->Ssid, COUNTOF(pstatus->szSSID) - 1);
		pstatus->szSSID [pssid->SsidLength] = L'\0';
	}

	ok = TRUE;

Exit:
	if (pbuffer)
		delete [] pbuffer;

	return ok;
}
