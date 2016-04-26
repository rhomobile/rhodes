#include "RcmLoader.h"

CRcmLoader::CRcmLoader()
{
	m_hRcmDLL						= NULL;			
	lpfn_RCM_Open					= NULL;		
	lpfn_RCM_GetSmartBatteryStatus	= NULL;
	lpfn_RCM_RegisterTriggerEvent	= NULL;
	lpfn_RCM_DeregisterTrigger		= NULL;
	lpfn_RCM_GetTriggerStatus		= NULL;
	lpfn_RCM_GetUniqueUnitIdEx		= NULL;
}

CRcmLoader::~CRcmLoader()
{
	UnloadRcmDLL();
}

//  Loading RCM DLL dynamically
BOOL CRcmLoader::LoadRcmDLL()
{
	m_hRcmDLL = LoadLibrary(RCM_DLL);
	if (!m_hRcmDLL)
	{
		return FALSE;
	}
	//  Map the function pointers to functions exported from the DLL
	lpfn_RCM_Open = (LPFN_RCM_OPEN_T)GetProcAddress
		(m_hRcmDLL, L"RCM_Open");
	lpfn_RCM_GetSmartBatteryStatus = (LPFN_RCM_GETSMARTBATTERYSTATUS_T)GetProcAddress
		(m_hRcmDLL, L"RCM_GetSmartBatteryStatus");
	lpfn_RCM_RegisterTriggerEvent = (LPFN_RCM_REGISTERTRIGGEREVENT_T)GetProcAddress
		(m_hRcmDLL, L"RCM_RegisterTriggerEvent");
	lpfn_RCM_DeregisterTrigger = (LPFN_RCM_DEREGISTERTRIGGER_T)GetProcAddress
		(m_hRcmDLL, L"RCM_DeregisterTrigger");
	lpfn_RCM_GetTriggerStatus = (LPFN_RCM_GETTRIGGERSTATUS_T)GetProcAddress
		(m_hRcmDLL, L"RCM_GetTriggerStatus");
	lpfn_RCM_GetUniqueUnitIdEx = (LPFN_RCM_GETUNIQUEUNITIDEX_T)GetProcAddress
		(m_hRcmDLL, L"RCM_GetUniqueUnitIdEx");

	if (lpfn_RCM_Open					== NULL ||
		lpfn_RCM_GetSmartBatteryStatus	== NULL ||
		lpfn_RCM_RegisterTriggerEvent	== NULL ||
		lpfn_RCM_DeregisterTrigger		== NULL ||
		lpfn_RCM_GetTriggerStatus		== NULL ||
		lpfn_RCM_GetUniqueUnitIdEx		== NULL)
	{
		m_hRcmDLL = NULL;
		return FALSE;
	}
	else
		return TRUE;
}

BOOL CRcmLoader::UnloadRcmDLL()
{
	if (m_hRcmDLL)
	{
		FreeLibrary(m_hRcmDLL);
		m_hRcmDLL						= NULL;
		lpfn_RCM_Open					= NULL;
		lpfn_RCM_GetSmartBatteryStatus	= NULL;
		lpfn_RCM_RegisterTriggerEvent	= NULL;
		lpfn_RCM_DeregisterTrigger		= NULL;
		lpfn_RCM_GetTriggerStatus		= NULL;
	}
	return TRUE;
}
