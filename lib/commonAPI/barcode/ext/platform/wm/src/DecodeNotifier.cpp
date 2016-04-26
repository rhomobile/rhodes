#include "DecodeNotifier.h"

CDecodeNotifier::CDecodeNotifier()
{
	m_dwDecodeVolume = 0;
	m_dwDecodeFrequency = 0;
	m_dwInvalidDecodeFrequency = 0;
	m_dwDecodeDuration = 0;
	m_szScanDecodeWav = NULL;
	m_szScanInvalidWav = NULL;
	m_iBeeperIndex = -1;
	m_bGood = false;
	m_pNotifications = NULL;
	m_bNotificationsLoaded = FALSE;
}

CDecodeNotifier::~CDecodeNotifier()
{
	if (m_szScanDecodeWav)
		delete[] m_szScanDecodeWav;
	m_szScanDecodeWav = NULL;
	if (m_szScanInvalidWav)
		delete[] m_szScanInvalidWav;
	m_szScanDecodeWav = NULL;
	delete m_pNotifications;
}

void CDecodeNotifier::Initialise(DWORD dwDecodeVolume,
				DWORD dwDecodeFrequency,
				DWORD dwInvalidDecodeFrequency,
				DWORD dwDecodeDuration,
				WCHAR* szScanDecodeWav,
				WCHAR* szScanInvalidWav)
{
	//  Store the variables in member variables.
	m_dwDecodeVolume = dwDecodeVolume;
	m_dwDecodeFrequency = dwDecodeFrequency;
	m_dwInvalidDecodeFrequency = dwInvalidDecodeFrequency;
	m_dwDecodeDuration = dwDecodeDuration;
	SetSuccessfulDecodeSound(szScanDecodeWav);
	SetInvalidDecodeSound(szScanInvalidWav);
	m_pNotifications = new CBarcodeBeeper();
	if (m_pNotifications)
		m_bNotificationsLoaded = m_pNotifications->LoadNotificationDLL();

	if (m_pNotifications && m_bNotificationsLoaded)
	{
		//  Initialise the Beeper
		NOTIFY_FINDINFO notifyObject;
		HANDLE	hFindHandle = NULL;

		SI_ALLOC_ALL(&notifyObject);
		notifyObject.StructInfo.dwUsed = 0;

		if(m_pNotifications->lpfn_Notify_FindFirst(&notifyObject, &hFindHandle) == E_NTFY_SUCCESS)
		{
			int iObCount = 0;
			do 
			{
				if(notifyObject.dwObjectType == NOTIFY_TYPE_BEEPER)
				{
					m_iBeeperIndex = iObCount;
					break;
				}
				iObCount++;
				SI_ALLOC_ALL(&notifyObject);
				notifyObject.StructInfo.dwUsed = 0;
			} while(m_pNotifications->lpfn_Notify_FindNext(&notifyObject, hFindHandle) == E_NTFY_SUCCESS);
		}
		m_pNotifications->lpfn_Notify_FindClose(hFindHandle);
	}
}

BOOL CDecodeNotifier::GoodDecode()
{
	BOOL bReturnVal = TRUE;
	//  Always play the Wave file if specified in preference
	if (m_szScanDecodeWav && wcslen(m_szScanDecodeWav) > 0)
	{
		bReturnVal = PlaySound(m_szScanDecodeWav, NULL, SND_ASYNC);
	}
	else
	{
		//  Create a thread to play the beeper
		m_bGood = true;
		CloseHandle(CreateThread(NULL, 0, &DecodeBeepThreadNotify, (LPVOID)this, 0, NULL));
	}
	return bReturnVal;
}

BOOL CDecodeNotifier::BadDecode()
{
	BOOL bReturnVal = TRUE;
	//  Always play the Wave file if specified in preference
	if (m_szScanInvalidWav && wcslen(m_szScanInvalidWav) > 0)
	{
		bReturnVal = PlaySound(m_szScanInvalidWav, NULL, SND_ASYNC);
	}
	else
	{
		//  Create a thread to play the beeper
		m_bGood = false;
		CloseHandle(CreateThread(NULL, 0, &DecodeBeepThreadNotify, (LPVOID)this, 0, NULL));
	}
	return bReturnVal;
}

//  Thread to play the beeper using the configured parameters
DWORD CDecodeNotifier::DecodeBeepThreadNotify(LPVOID lParam)
{
	CDecodeNotifier *pDecodeNotifier =(CDecodeNotifier*)lParam;
	DWORD dwResult = 0;
	if (pDecodeNotifier && 
		pDecodeNotifier->m_bNotificationsLoaded)
	{
		if (pDecodeNotifier->m_dwDecodeVolume >= 0)
		{
			dwResult = pDecodeNotifier->m_pNotifications->lpfn_Notify_SetState(pDecodeNotifier->m_iBeeperIndex, NOTIFY_STATE_OFF);
			CYCLE_INFO cycleInfo;
			SI_INIT(&cycleInfo);

			dwResult = pDecodeNotifier->m_pNotifications->lpfn_Notify_GetCycleInfo(pDecodeNotifier->m_iBeeperIndex, &cycleInfo);
			
			cycleInfo.ObjectTypeSpecific.BeeperSpecific.dwFrequency = 
				(pDecodeNotifier->m_bGood ? 
				pDecodeNotifier->m_dwDecodeFrequency : pDecodeNotifier->m_dwInvalidDecodeFrequency);
			cycleInfo.ObjectTypeSpecific.BeeperSpecific.dwVolume = pDecodeNotifier->m_dwDecodeVolume;
			cycleInfo.ObjectTypeSpecific.BeeperSpecific.dwDuration = pDecodeNotifier->m_dwDecodeDuration;
			dwResult = pDecodeNotifier->m_pNotifications->lpfn_Notify_SetCycleInfo(pDecodeNotifier->m_iBeeperIndex, &cycleInfo);

			//now call the notification
			dwResult = pDecodeNotifier->m_pNotifications->lpfn_Notify_SetState(pDecodeNotifier->m_iBeeperIndex, NOTIFY_STATE_CYCLE);  
		}
	}
	return dwResult;

}

void CDecodeNotifier::SetSuccessfulDecodeSound(LPCTSTR szDecodeSound)
{
	if (m_szScanDecodeWav)
	{
		delete[] m_szScanDecodeWav;
		m_szScanDecodeWav = NULL;
	}
	if (szDecodeSound)
	{
		m_szScanDecodeWav = new WCHAR[wcslen(szDecodeSound) + 1];
		int iStart = 0;
		if (wcslen(szDecodeSound) > wcslen(L"file://") &&
			wcsnicmp(szDecodeSound, L"file://", 7) == 0)
			iStart = 7;
		wcscpy(m_szScanDecodeWav, szDecodeSound + iStart);
	}
}

void CDecodeNotifier::SetInvalidDecodeSound(LPCTSTR szInvalidDecodeSound)
{
	if (m_szScanInvalidWav)
	{
		delete[] m_szScanInvalidWav;
		m_szScanInvalidWav = NULL;
	}
	if (szInvalidDecodeSound)
	{
		m_szScanInvalidWav = new WCHAR[wcslen(szInvalidDecodeSound) + 1];
		int iStart = 0;
		if (wcslen(szInvalidDecodeSound) > wcslen(L"file://") &&
			wcsnicmp(szInvalidDecodeSound, L"file://", 7) == 0)
			iStart = 7;
		wcscpy(m_szScanInvalidWav, szInvalidDecodeSound + iStart);
	}
}
