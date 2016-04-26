#include "NotificationLoader.h"

CBarcodeBeeper::CBarcodeBeeper()
{
	m_hNotificationDLL			= NULL;			
	lpfn_Notify_FindClose		= NULL;		
	lpfn_Notify_FindFirst		= NULL;		
	lpfn_Notify_FindNext		= NULL;		
	lpfn_Notify_SetCycleInfo	= NULL;	
	lpfn_Notify_GetCycleInfo	= NULL;	
	lpfn_Notify_SetState		= NULL;		
	lpfn_Notify_GetState		= NULL;		
}

CBarcodeBeeper::~CBarcodeBeeper()
{
	UnloadNotificationDLL();
}

//  Loading Notification DLL dynamically
BOOL CBarcodeBeeper::LoadNotificationDLL()
{
	m_hNotificationDLL = LoadLibrary(NOTIFY_DLL);
	if (!m_hNotificationDLL)
	{
//		Log(SCANNER_LOG_WARNING, L"Unable to load EMDK Notification driver, scanner will not support 'beeping'",
//			_T(__FUNCTION__), __LINE__);
		return FALSE;
	}
	//  Map the function pointers to functions exported from the DLL
	lpfn_Notify_FindClose = (LPFN_NOTIFY_FINDCLOSE_T)GetProcAddress
		(m_hNotificationDLL, L"NOTIFY_FindClose");
	lpfn_Notify_FindNext = (LPFN_NOTIFY_FINDNEXT_T)GetProcAddress
		(m_hNotificationDLL, L"NOTIFY_FindNext");
	lpfn_Notify_FindFirst = (LPFN_NOTIFY_FINDFIRST_T)GetProcAddress
		(m_hNotificationDLL, L"NOTIFY_FindFirst");
	lpfn_Notify_SetCycleInfo = (LPFN_NOTIFY_SETCYCLEINFO_T)GetProcAddress
		(m_hNotificationDLL, L"NOTIFY_SetCycleInfo");
	lpfn_Notify_GetCycleInfo = (LPFN_NOTIFY_GETCYCLEINFO_T)GetProcAddress
		(m_hNotificationDLL, L"NOTIFY_GetCycleInfo");
	lpfn_Notify_SetState = (LPFN_NOTIFY_SETSTATE_T)GetProcAddress
		(m_hNotificationDLL, L"NOTIFY_SetState");
	lpfn_Notify_GetState = (LPFN_NOTIFY_GETSTATE_T)GetProcAddress
		(m_hNotificationDLL, L"NOTIFY_GetState");

	if (lpfn_Notify_FindClose	== NULL ||
		lpfn_Notify_FindNext	== NULL ||
		lpfn_Notify_FindFirst	== NULL ||
		lpfn_Notify_SetCycleInfo== NULL ||
		lpfn_Notify_GetCycleInfo== NULL ||
		lpfn_Notify_SetState	== NULL ||
		lpfn_Notify_GetState	== NULL)
	{
		m_hNotificationDLL = NULL;
		return FALSE;
	}
	else
		return TRUE;
}

BOOL CBarcodeBeeper::UnloadNotificationDLL()
{
	if (m_hNotificationDLL)
	{
		FreeLibrary(m_hNotificationDLL);
		m_hNotificationDLL		= NULL;
		lpfn_Notify_FindClose	= NULL;
		lpfn_Notify_FindNext	= NULL;
		lpfn_Notify_FindFirst	= NULL;
		lpfn_Notify_SetCycleInfo= NULL;
		lpfn_Notify_GetCycleInfo= NULL;
		lpfn_Notify_SetState	= NULL;
		lpfn_Notify_GetState	= NULL;
	}
	return TRUE;
}
