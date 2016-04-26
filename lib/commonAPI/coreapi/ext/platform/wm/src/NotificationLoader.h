#pragma once
#include <windows.h>

#include "NtfyDef.h"
#define	E_NTFY_SUCCESS					0

//  Dynamically Loading the Notification DLL (This code is shared by the Scanner and 
//  Device / Application DLLs
//  Definitions of Methods exported from the EMDK Notifications
#define NOTIFY_DLL L"NtfyAPI32.dll"	///<  Name of the EMDK Notification Library
typedef DWORD (WINAPI* LPFN_NOTIFY_FINDCLOSE_T)		(HANDLE);
typedef DWORD (WINAPI* LPFN_NOTIFY_FINDNEXT_T)		(LPNOTIFY_FINDINFO lpNotifyFindInfo, HANDLE hFindHandle);
typedef DWORD (WINAPI* LPFN_NOTIFY_FINDFIRST_T)		(LPNOTIFY_FINDINFO lpNotifyFindInfo, LPHANDLE lphFindHandle);
typedef DWORD (WINAPI* LPFN_NOTIFY_SETCYCLEINFO_T)	(DWORD dwIndex,	LPCYCLE_INFO lpCycleInfo);
typedef DWORD (WINAPI* LPFN_NOTIFY_GETCYCLEINFO_T)	(DWORD dwIndex,	LPCYCLE_INFO lpCycleInfo);
typedef DWORD (WINAPI* LPFN_NOTIFY_SETSTATE_T)		(DWORD dwIndex,	DWORD dwState);
typedef DWORD (WINAPI* LPFN_NOTIFY_GETSTATE_T)		(DWORD dwIndex,	LPDWORD lpdwState);

class CNotificationLoader
{
public:

	HMODULE	m_hNotificationDLL;			///<  HMODULE of the loaded EMDK Notification DLL
	LPFN_NOTIFY_FINDCLOSE_T		lpfn_Notify_FindClose;		///< function pointer to EMDK Notify API
	LPFN_NOTIFY_FINDFIRST_T		lpfn_Notify_FindFirst;		///< function pointer to EMDK Notify API
	LPFN_NOTIFY_FINDNEXT_T		lpfn_Notify_FindNext;		///< function pointer to EMDK Notify API
	LPFN_NOTIFY_SETCYCLEINFO_T	lpfn_Notify_SetCycleInfo;	///< function pointer to EMDK Notify API
	LPFN_NOTIFY_GETCYCLEINFO_T	lpfn_Notify_GetCycleInfo;	///< function pointer to EMDK Notify API
	LPFN_NOTIFY_SETSTATE_T		lpfn_Notify_SetState;		///< function pointer to EMDK Notify API
	LPFN_NOTIFY_GETSTATE_T		lpfn_Notify_GetState;		///< function pointer to EMDK Notify API

	CNotificationLoader();
	~CNotificationLoader();
	
	/**
	*  The Scanner is dependant on the EMDK Notification API for generating
	*  good and bad decode 'beeps'.  Load the Notification DLL dynamically
	*  to avoid catastrophic failure if we try and launch the scanner on
	*  a non-Symbol device (remember this DLL is loaded by the license 
	*  screen and also in the default config's preloads)
	*  \return Whether or not the Notification DLL could be loaded
	*/
	BOOL LoadNotificationDLL();
	BOOL UnloadNotificationDLL();

	BOOL Beep(int iFrequency, int iVolume, int iDuration);
	BOOL Vibrate(int iDuration);

private:
	int m_iBeeperIndex;
	int m_iVibrateIndex;
};