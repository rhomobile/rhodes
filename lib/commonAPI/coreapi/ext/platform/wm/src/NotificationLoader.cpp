#include "NotificationLoader.h"

CNotificationLoader::CNotificationLoader()
{
	m_hNotificationDLL			= NULL;			
	lpfn_Notify_FindClose		= NULL;		
	lpfn_Notify_FindFirst		= NULL;		
	lpfn_Notify_FindNext		= NULL;		
	lpfn_Notify_SetCycleInfo	= NULL;	
	lpfn_Notify_GetCycleInfo	= NULL;	
	lpfn_Notify_SetState		= NULL;		
	lpfn_Notify_GetState		= NULL;		
	int m_iBeeperIndex			= -1;
	int m_iVibrateIndex			= -1;
}

CNotificationLoader::~CNotificationLoader()
{
	UnloadNotificationDLL();
}

//  Loading Notification DLL dynamically
BOOL CNotificationLoader::LoadNotificationDLL()
{
	m_hNotificationDLL = LoadLibrary(NOTIFY_DLL);
	if (!m_hNotificationDLL)
	{
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
	{
		//  Set the indices of the beeper and vibrate index
		NOTIFY_FINDINFO notifyObject;
		HANDLE	hFindHandle = NULL;
		SI_ALLOC_ALL(&notifyObject);
		notifyObject.StructInfo.dwUsed = 0;
		if(lpfn_Notify_FindFirst(&notifyObject, &hFindHandle) == E_NTFY_SUCCESS)
		{
			int iObCount = 0;
			int m_iBeeperIndexForSoftwareOrHardware = 0;
			do 
			{
				if(notifyObject.dwObjectType == NOTIFY_TYPE_BEEPER)
				{
					//  Found the Beeper
					if(m_iBeeperIndexForSoftwareOrHardware == 0)
					{
                         			m_iBeeperIndexForSoftwareOrHardware = iObCount;
					}
                    			m_iBeeperIndex = m_iBeeperIndexForSoftwareOrHardware;
					//continue;
				}
				else if (notifyObject.dwObjectType == NOTIFY_TYPE_VIBRATOR)
				{
					m_iVibrateIndex = iObCount;
				}
				iObCount++;
				SI_ALLOC_ALL(&notifyObject);
				notifyObject.StructInfo.dwUsed = 0;
			} while(lpfn_Notify_FindNext(&notifyObject, hFindHandle) == E_NTFY_SUCCESS);
		}
		lpfn_Notify_FindClose(hFindHandle);
		return TRUE;
	}
}

BOOL CNotificationLoader::UnloadNotificationDLL()
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

BOOL CNotificationLoader::Beep(int iFrequency, int iVolume, int iDuration)
{
	if (m_iBeeperIndex < 0)
		return FALSE;

	CYCLE_INFO cycleInfo;
	SI_INIT(&cycleInfo);
	DWORD dwResult = lpfn_Notify_GetCycleInfo(m_iBeeperIndex, &cycleInfo);
	if(dwResult != E_NTFY_SUCCESS)
		return FALSE;
	cycleInfo.ObjectTypeSpecific.BeeperSpecific.dwFrequency = iFrequency;
	cycleInfo.ObjectTypeSpecific.BeeperSpecific.dwVolume = iVolume;
	cycleInfo.ObjectTypeSpecific.BeeperSpecific.dwDuration = iDuration;
	dwResult = lpfn_Notify_SetCycleInfo(m_iBeeperIndex, &cycleInfo);
	dwResult = lpfn_Notify_SetState(m_iBeeperIndex, NOTIFY_STATE_CYCLE);

	return TRUE;
}

BOOL CNotificationLoader::Vibrate(int iDuration)
{
	if (m_iVibrateIndex < 0)
		return FALSE;

	CYCLE_INFO cycleInfo;
	SI_INIT(&cycleInfo);
	DWORD dwResult = lpfn_Notify_GetCycleInfo(m_iVibrateIndex, &cycleInfo);
	if(dwResult != E_NTFY_SUCCESS)
		return FALSE;
	cycleInfo.ObjectTypeSpecific.VibratorSpecific.dwDuration = iDuration;

	dwResult = lpfn_Notify_SetCycleInfo(m_iVibrateIndex, &cycleInfo);
	dwResult = lpfn_Notify_SetState(m_iVibrateIndex, NOTIFY_STATE_CYCLE);
	return dwResult == E_NTFY_SUCCESS;
}
