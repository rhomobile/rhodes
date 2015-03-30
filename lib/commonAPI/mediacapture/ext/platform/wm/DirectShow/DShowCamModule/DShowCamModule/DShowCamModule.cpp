// DShowCamModule.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "DShowCamModule.h"
#include "DShowDef.h"
#include "PropertyBag.h"
#include <windows.h>
#include <pm.h>
#include <Notify.h>
#include "DshowCam.h"


#define POWER_KEY_REG_ROOT_MPA			TEXT("Drivers\\BuiltIn\\Power\\Settings")
#define POWER_KEY_REG_ROOT_IAC			TEXT("Drivers\\BuiltIn\\Remkeys")
#define POWER_KEY_REG_MPA				TEXT("DisablePwrSwitch")
#define POWER_KEY_REG_IAC				TEXT("PwrKeySuspendDisabled")
#define POWER_REFRESHEVENT_MPA			TEXT("SymbolPM/PowerDrvrRefershEvent")

#define ENABLE_POWER_KEY_VALUE			0
#define DISABLE_POWER_KEY_VALUE			1
#define MODEL_NAME_MAX_LENGTH			256
//--------------- MPA3 ----------------------
const GUID CLSID_COLOR_CONVERTER = {0xb0a377b8, 0x4eb8, 0x49ce, { 0xb0, 0xf4, 0xf6, 0x21, 0x3f, 0x70, 0xbe, 0x67 } };
//--------------- MPA3 ----------------------

const int LOW		= 352*288;
const int MEDIUM	= 640*480;
const int MIDHIGH	= 800*600;
const int HIGH		= 1280*1024;

//global variables
CDShowCamModule		*g_Prop				= NULL;			// global instance pointer.
HANDLE				g_hWorkerThread 	= NULL;
HANDLE				*g_pEventArray		= NULL;
//HANDLE              g_pEventNoSusRes;
HANDLE				g_hRegPwrNotif		= NULL;
BOOL				g_bStopWorkerThread = FALSE;
//HGLOBAL				hGlobal				= NULL;
HANDLE				m_hEventWakeup		= NULL;
HREGNOTIFY			g_hPowerKeyRegNotify        = NULL;

BOOL APIENTRY DllMain( HANDLE hModule, 
					  DWORD  ul_reason_for_call, 
					  LPVOID lpReserved
					  )
{
	UNREFERENCED_PARAMETER(lpReserved);
	UNREFERENCED_PARAMETER(hModule);

	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}


//------------------------------------------------------------------
//
// Prototype:	int InitDShow(HWND hwnd, RECT rc)
//
// Description:	Exported function to initialize filter graph for camera
//				capture.
//
// Parameters:	hwnd is Handle of the parent window for the directx video
//				window.
//				rc is the client rect for video windo.
//
// Returns:	Boolean "TRUE" if function succeeds otherwise Boolean "FALSE".
// 
// Notes:
//------------------------------------------------------------------
//DSHOWCAM_API int InitDShow(HWND hwnd, RECT rc)
//{
//	int iError = -1;
//	wstring	wsaCamIDs;			// Contains enumurated Camera IDs
//	
//
//	if(hwnd == NULL)
//		return iError;
//
//	/*LONG lRet = GetWindowLong(hwnd, GWL_STYLE);
//	lRet |= WS_CLIPCHILDREN;
//	LONG lRet1 = SetWindowLong(hwnd, GWL_STYLE, lRet);*/
//
//	//Already one instance is running. Exit with an error
//	if(g_Prop != NULL)
//	{
//		g_Prop = NULL;
//		return iError;
//	}
//
//	//Create a new instance CDShowCamModule class
//	g_Prop = new CDShowCamModule();
//
//	if(g_Prop != NULL)
//	{
//		//Find the installed camera devices
//		g_Prop->FindFirstCam(&wsaCamIDs);
//		
//		if(wsaCamIDs.compare(L"") == 0)
//		{
//			/*RECT rc;
//			GetClientRect(hwnd, &rc);*/
//			if(Invoke(hwnd, rc, wsaCamIDs.c_str()) == -1)
//			{
//				iError = -1;
//				goto _exitInvoke;
//			}
//		}
//	}
//	
//	g_pEventArray = (HANDLE *)malloc(2 * sizeof(HANDLE));
//	if(!g_pEventArray)
//	{
//		iError = -1;
//		goto _exitInvoke;
//	}
//
//    //g_pEventNoSusRes = CreateEvent(	NULL,FALSE,FALSE,NULL);
//									 
//   /*if(g_pEventNoSusRes == INVALID_HANDLE_VALUE)
//	{
//		iError = -1;
//		goto _exitInvoke;
//	}*/
//	//This is the event used to unblock the thread for our use
//	g_pEventArray[0] = CreateEvent(	NULL,
//									FALSE, 
//									FALSE,
//									NULL);
//	if(g_pEventArray[0] == NULL)
//	{
//		iError = -1;
//		goto _exitInvoke;
//	}
//
//	//Second member shall be message queue for power management
//	
//	MSGQUEUEOPTIONS Options;
//
//	Options.dwSize = sizeof(MSGQUEUEOPTIONS);
//	Options.dwFlags = MSGQUEUE_NOPRECOMMIT;
//	Options.dwMaxMessages = 0;
//	Options.cbMaxMessage = sizeof(POWER_BROADCAST) + sizeof(POWER_BROADCAST_POWER_INFO);
//	Options.bReadAccess = TRUE;
//
//	g_pEventArray[1] = CreateMsgQueue(NULL, &Options);
//	if(g_pEventArray[1] == NULL)
//	{
//		iError = -1;
//		goto _exitInvoke;
//	}
//
//	g_hRegPwrNotif = RequestPowerNotifications(g_pEventArray[1], PBT_RESUME);
//	if(g_hRegPwrNotif == NULL) 
//	{
//		iError = -1;
//		goto _exitInvoke;
//	}
//
//	DWORD dwThreadId = 0;
//	g_bStopWorkerThread = FALSE;
//	g_hWorkerThread = CreateThread(	NULL, 
//									0,
//									(LPTHREAD_START_ROUTINE)WorkerThread, 
//									NULL, 
//									0, 
//									&dwThreadId);
//	if(!g_hWorkerThread)
//	{
//		iError = -1;
//		goto _exitInvoke;
//	}
//
//	iError = ERROR_SUCCESS;
//
//_exitInvoke:
//	if(iError == -1)
//	{
//		if(g_pEventArray)
//		{
//			CloseHandle(g_pEventArray[0]);
//			CloseHandle(g_pEventArray[1]);
//			StopPowerNotifications(g_hRegPwrNotif);
//			free(g_pEventArray);
//		}
//		//if(g_pEventNoSusRes != NULL)
//		//{
//
//		//	CloseHandle(g_pEventNoSusRes);
//		//	//g_pEventNoSusRes = NULL;
//		//	//free(g_pEventNoSusRes);
//		//}
//		g_Prop = NULL;
//	}
//	return iError;
//}

DSHOWCAM_API int InitDirectShow(HWND hwnd, RECT rc)
{
	int nResult = -1;

	if(NULL != hwnd)
	{
		//Already one instance is running. Exit with an error
		if(NULL == g_Prop)
		{
			//Create a new instance CDShowCamModule class
			g_Prop = new CDShowCamModule();
			if(NULL != g_Prop)
			{
			    //Disable power key as a workaround 
				//for freezing issue after suspend-resume

				wstring	wsaCamIDs;	// Contains enumurated Camera IDs

				//Find the installed camera devices
				g_Prop->FindFirstCam(&wsaCamIDs);

				if(wsaCamIDs.compare(L"") == 0)
				{
					if(Invoke(hwnd, rc, wsaCamIDs.c_str()) != -1)
					{
						return 0;
						//g_pEventArray = (HANDLE *)malloc(2 * sizeof(HANDLE));
						//if(NULL != g_pEventArray)
						//{
						//	//This is the event used to unblock the thread for our use
						//	g_pEventArray[0] = CreateEvent(	NULL, FALSE, FALSE, NULL);

						//	MSGQUEUEOPTIONS Options;
						//	Options.dwSize = sizeof(MSGQUEUEOPTIONS);
						//	Options.dwFlags = MSGQUEUE_NOPRECOMMIT;
						//	Options.dwMaxMessages = 0;
						//	Options.cbMaxMessage = sizeof(POWER_BROADCAST) + sizeof(POWER_BROADCAST_POWER_INFO);
						//	Options.bReadAccess = TRUE;
						//	g_pEventArray[1] = CreateMsgQueue(NULL, &Options);

						//	g_hRegPwrNotif = RequestPowerNotifications(g_pEventArray[1], PBT_RESUME);

						//	if((NULL != g_pEventArray[0]) && (NULL != g_pEventArray[1]) && (NULL != g_hRegPwrNotif))
						//	{
						//		DWORD dwThreadId = 0;
						//		g_bStopWorkerThread = FALSE;
						//		g_hWorkerThread = CreateThread(	NULL, 0, (LPTHREAD_START_ROUTINE)WorkerThread, NULL, 0, &dwThreadId);
						//		if(NULL != g_hWorkerThread)
						//		{
						//			if(FALSE != g_Prop->IsIACDEvice())
						//			{
						//				g_Prop->EnablePowerKeyIAC(FALSE);
						//			}
						//			else // For MPA Devices
						//			{
						//				g_Prop->EnablePowerKeyMPA(FALSE);
						//			}
						//			nResult = 0;
						//		}							
						//	}
						//}
					}
					else
					{
						return -1;
					}
				}
			}
		}
	}

	if(-1 == nResult)
	{
		//free all
		if(g_Prop != NULL)
		{
			delete g_Prop;
			g_Prop = NULL;
		}
		/*if(g_pEventArray)
		{
			if(g_pEventArray[0] != NULL)
			{
				SetEvent(g_pEventArray[0]);
			}
			if(WAIT_TIMEOUT == WaitForSingleObject(g_hWorkerThread, 1000))
			{
				TerminateThread(g_hWorkerThread, 0);
			}
			CloseHandle(g_hWorkerThread);
			g_hWorkerThread = NULL;

			if(g_pEventArray[0] != NULL)
			{
				CloseHandle(g_pEventArray[0]);
				g_pEventArray[0] = NULL;
			}
			if(g_pEventArray[1] != NULL)
			{
				CloseMsgQueue(g_pEventArray[1]);
				g_pEventArray[1] = NULL;
			}

			free(g_pEventArray);
			g_pEventArray = NULL;

			if(g_hRegPwrNotif != NULL)
			{
				StopPowerNotifications(g_hRegPwrNotif);
				CloseHandle(g_hRegPwrNotif);
				g_hRegPwrNotif = NULL;
			}
		}*/

		


	}

	return nResult;
}

BOOL SetWakeupNotificationEllis(void)
{
	//LogEx(LOG_LEVEL_5, LOG_POWERMODULE, TEXT("BOOL CPowerModule::SetWakeupNotification(void)"));
	BOOL bResult = FALSE;
	m_hEventWakeup = CreateEvent(NULL,TRUE,FALSE,TEXT("EVENT_WAKEUP_NOTIFY"));
	HANDLE m_hNotifyWakeup = NULL;
	if (m_hEventWakeup != NULL)
	{
		CE_NOTIFICATION_TRIGGER centNotification;
		memset(&centNotification, 0, sizeof(CE_NOTIFICATION_TRIGGER));
		centNotification.dwSize = sizeof(CE_NOTIFICATION_TRIGGER);
		//Event based notification
		centNotification.dwType = CNT_EVENT;
		centNotification.dwEvent = NOTIFICATION_EVENT_WAKEUP;
		//Named event to be signaled 
		TCHAR szNamedEvent[MAX_PATH];
		memset(szNamedEvent, 0, sizeof(szNamedEvent));
		_tcscpy(szNamedEvent, NAMED_EVENT_PREFIX_TEXT);
		_tcscat(szNamedEvent, TEXT("EVENT_WAKEUP_NOTIFY"));
		centNotification.lpszApplication = szNamedEvent;
		centNotification.lpszArguments = NULL;
		//Prepare event for notification
		ResetEvent(m_hEventWakeup);
		if (NULL != (m_hNotifyWakeup = CeSetUserNotificationEx(0, &centNotification, NULL)))
		{

			//SetEvent(m_hEventWakeup);
			while(!bResult)
			{
				// wait for my event
				if( WaitForSingleObject( m_hEventWakeup, 80*1000 ) != WAIT_OBJECT_0 )
				{
					//MessageBox(NULL,TEXT("test point inside waitforsingle object"),TEXT("TestPoint"),0);

				}
				else
				{
					bResult = TRUE;
				}

			}

		}		

	}

	return bResult;
}

void WorkerThread()
{
	DWORD dwResult = 100; // initialise the dword value to be used. 100 is a random value.
	while(!g_bStopWorkerThread)
	{

		// code added to get the device name - to fix the suspend resume issue
		char oemModelName[50], oemModelTemp[50],str[] = "ES400",*temp;
		char strMC65[] = "MC65";
		char *tempMC65;
		int i,j=0;
		SystemParametersInfo(SPI_GETOEMINFO,50, oemModelName, 0);
		for(i=0; i < 49; i++)
		{
			if( oemModelName[i] != '\0' || oemModelName[i+1] != '\0')
			{
				if (oemModelName[i] != '\0')
				{
					oemModelTemp[j] = oemModelName[i];
					j++;
				}
			}

			else
				break;
		}

		oemModelTemp[j] = '\0';
		temp = strstr(oemModelTemp,str);
		tempMC65 = strstr(oemModelTemp, strMC65);

		if(temp != NULL || tempMC65 != NULL)
		{ 
			if(SetWakeupNotificationEllis())
			{
				dwResult = WAIT_OBJECT_0;
				Sleep(900);
				Pause();
				Run();

			}
		}
		else
		{
			dwResult = WaitForMultipleObjects(2, g_pEventArray,  FALSE,  80*1000);									 
		}


		switch(dwResult)
		{
		case WAIT_OBJECT_0:
			{
				Sleep(500);
				Pause();
				Run();
				break;
			}
		case WAIT_OBJECT_0 + 1:
			{
				POWER_BROADCAST Buffer;
				DWORD Flags;
				DWORD BufferLength;
				if(ReadMsgQueue(g_pEventArray[1],
					(LPVOID)&Buffer,
					sizeof(POWER_BROADCAST),
					&BufferLength,
					INFINITE,
					&Flags))
				{
					switch(Buffer.Message)
					{
					case PBT_RESUME:
						{
							RETAILMSG(1, (TEXT("DShowCamModule: Got Resume!!")));
							Sleep(1000);
							Pause();
							Run();
							break;
						}
					}
				}

				break;
			}
		}
	}
}
//------------------------------------------------------------------
//
// Prototype:	int xxx(XXX xxx)
//
// Description:	Exported function to initialize filter graph for camera
//				capture.
//
// Parameters:	hwnd is Handle of the parent window for the directx video
//				window.
//				rc is the client rect for video windo.
//
// Returns:	Boolean "TRUE" if function succeeds otherwise Boolean "FALSE".
// 
// Notes:
//------------------------------------------------------------------
DSHOWCAM_API int Pause()
{
	int iError = -1;

	if(g_Prop == NULL)
		return iError;
	//Pause the preview.
	if(SUCCEEDED(g_Prop->PauseGrp()))
	{
		//hide the preview window
		if(SUCCEEDED(g_Prop->HidePreview()))
		{
			iError = ERROR_SUCCESS;
		}
	}

	return (iError);
}

//------------------------------------------------------------------
//
// Prototype:	int xxx(XXX xxx)
//
// Description:	Exported function to initialize filter graph for camera
//				capture.
//
// Parameters:	hwnd is Handle of the parent window for the directx video
//				window.
//				rc is the client rect for video windo.
//
// Returns:	Boolean "TRUE" if function succeeds otherwise Boolean "FALSE".
// 
// Notes:
//------------------------------------------------------------------
DSHOWCAM_API int Run()
{
	int iError = -1;

	if(g_Prop == NULL)
	{

		return iError;
	}

	//Show the preview window
	if(SUCCEEDED(g_Prop->ShowPreview()))
	{
		//Resume the preview window
		if(SUCCEEDED(g_Prop->RunGrp()))
		{
			iError = ERROR_SUCCESS;
		}
	}

	return (iError);
}

//------------------------------------------------------------------
//
// Prototype:	int xxx(XXX xxx)
//
// Description:	Exported function to initialize filter graph for camera
//				capture.
//
// Parameters:	hwnd is Handle of the parent window for the directx video
//				window.
//				rc is the client rect for video windo.
//
// Returns:	Boolean "TRUE" if function succeeds otherwise Boolean "FALSE".
// 
// Notes:
//------------------------------------------------------------------
DSHOWCAM_API int Stop()
{
	int iError = -1;

	if(g_Prop == NULL)
		return iError;

	//Show the preview window
	if(SUCCEEDED(g_Prop->StopGrp()))
	{
		iError = ERROR_SUCCESS;
	}

	return (iError);
}

//------------------------------------------------------------------
//
// Prototype:	int xxx(XXX xxx)
//
// Description:	Exported function to initialize filter graph for camera
//				capture.
//
// Parameters:	hwnd is Handle of the parent window for the directx video
//				window.
//				rc is the client rect for video windo.
//
// Returns:	Boolean "TRUE" if function succeeds otherwise Boolean "FALSE".
// 
// Notes:
//------------------------------------------------------------------
DSHOWCAM_API int CaptureStill(const wchar_t* filename, bool isES400)
{
	int iError = ERROR_SUCCESS;
	SYSTEMTIME st;
	if(g_Prop == NULL)
	{
		return -1;
	}

	//GetLocalTime(&st);
//	wsprintf(filename, L"%.4d%.2d%.2d%.2d%.2d%.2d.jpg", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

	//Check for flash setting
	if(g_Prop->bAutoFlash)
	{
		g_Prop->m_tCamTblFlash.plVal = g_Prop->m_tCamTblFlash.plMax;
		if(!SUCCEEDED(g_Prop->Set_PropVal(CAM, &g_Prop->m_tCamTblFlash)))
		{
			iError = -1;
			goto _exitCaptureStill;
		}
	}

	if(!SUCCEEDED(g_Prop->CaptureStill(filename)))
	{
		iError = -2;
		goto _exitCaptureStill;
	}

	if(g_Prop->bAutoFlash)
	{
		g_Prop->m_tCamTblFlash.plVal = g_Prop->m_tCamTblFlash.plMin;
		if(!SUCCEEDED(g_Prop->Set_PropVal(CAM, &g_Prop->m_tCamTblFlash)))
		{
			iError = -1;
			goto _exitCaptureStill;
		}
	}

_exitCaptureStill:
	return iError;

}

//------------------------------------------------------------------
//
// Prototype:	int xxx(XXX xxx)
//
// Description:	Exported function to initialize filter graph for camera
//				capture.
//
// Parameters:	hwnd is Handle of the parent window for the directx video
//				window.
//				rc is the client rect for video windo.
//
// Returns:	Boolean "TRUE" if function succeeds otherwise Boolean "FALSE".
// 
// Notes:
//------------------------------------------------------------------
DSHOWCAM_API int GetMetaDataFromImage(WCHAR *filename, double *latitude, double *longitude, LPTSTR datetime)
{
	int iError = -1;
	JpegMetadata *jmMetaData = new JpegMetadata();
	/*WCHAR fullfilename[TXT_LENGTH] = {0};
	wsprintf(fullfilename, L"\\My Documents\\My Pictures\\MMAXGallery\\%s", filename);*/
	if(jmMetaData != NULL)
	{
		if(ReadImageFileForMetaData(filename, jmMetaData))
		{
			DegreesMinutesSeconds2Degrees(latitude, jmMetaData->ulLatitude);
			DegreesMinutesSeconds2Degrees(longitude, jmMetaData->ulLongitude);
			WCHAR wdatetime[20];
			mbstowcs(wdatetime, jmMetaData->szDateTime, 20);
			wcscpy(datetime, wdatetime);
			iError = ERROR_SUCCESS;
		}

		delete jmMetaData;
	}
	return iError;
}

//------------------------------------------------------------------
//
// Prototype:	int xxx(XXX xxx)
//
// Description:	Exported function to initialize filter graph for camera
//				capture.
//
// Parameters:	hwnd is Handle of the parent window for the directx video
//				window.
//				rc is the client rect for video windo.
//
// Returns:	Boolean "TRUE" if function succeeds otherwise Boolean "FALSE".
// 
// Notes:
//------------------------------------------------------------------
DSHOWCAM_API int SetMetaDataToImage(double latitude, double longitude, LPTSTR filename)
{
	int iError = ERROR_SUCCESS;
	SYSTEMTIME st;
	JpegMetadata *metaData = new JpegMetadata();

	if(g_Prop == NULL)
	{
		iError = -1;
		goto _exitSetMetaData;
	}

	GetLocalTime(&st);
	int dYear, dMonth, dDay, dHour, dMinute, dSecond;
	dYear = (int)st.wYear;
	dMonth = (int)st.wMonth;
	dDay = (int)st.wDay;
	dHour = (int)st.wHour;
	dMinute = (int)st.wMinute;
	dSecond = (int)st.wSecond;

	if(dYear >=0 && dYear <=9999 && dMonth >=0 && dMonth <= 12 && dDay >=0 && dDay <=31 && dHour >=0 && dHour <= 24 && dMinute >=0 && dMinute <=60 && dSecond >=0 && dSecond <=60)
	{
		sprintf(metaData->szDateTime, "%.4d:%.2d:%.2d %.2d:%.2d:%.2d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
	}
	else
	{
		sprintf(metaData->szDateTime,"0000:00:00 00:00:00");
	}


	Degrees2DegreesMinutesSeconds(latitude, metaData->ulLatitude);
	Degrees2DegreesMinutesSeconds(longitude, metaData->ulLongitude);

	//This is an ugly workaround for issue where highest resolution(3mpx) taking up 6MB of current 
	//process virtual memory. For updating metadata we need to uncompress the jpg in memory
	//which again takes up around 6MB for 3mpx. So we reduce the resolution just before the 
	//updating metadata and restore it back after metadata update.
	if(g_Prop->m_tImgRes == High || g_Prop->m_tImgRes == MidHigh)
	{
		g_Prop->StopGrp();

		int index = 0;
		if(g_Prop->m_tStillImgFmtArray != NULL)
		{
			ImageRes imgRes;
			imgRes.nWidth = g_Prop->m_tStillImgFmtArray[index].nMaxWidth;
			imgRes.nHeight = g_Prop->m_tStillImgFmtArray[index].nMaxHeight;
			if(SUCCEEDED(g_Prop->Set_Resolution(&imgRes, S, FALSE)))
			{
				iError = ERROR_SUCCESS;
			}
		}

	}

	if(!UpdateImageFileWithMetaData(filename, metaData))
	{
		iError = -1;
		g_Prop->RunGrp();
		goto _exitSetMetaData;
	}

	if(g_Prop->m_tImgRes == High)
	{
		int index = g_Prop->m_nStillResCnt - 1 - 1;
		if(g_Prop->m_tStillImgFmtArray != NULL)
		{
			ImageRes imgRes;
			imgRes.nWidth = g_Prop->m_tStillImgFmtArray[index].nMaxWidth;
			imgRes.nHeight = g_Prop->m_tStillImgFmtArray[index].nMaxHeight;
			if(SUCCEEDED(g_Prop->Set_Resolution(&imgRes, S, FALSE)))
			{
				iError = ERROR_SUCCESS;
			}
		}

		g_Prop->RunGrp();
	}
	else if(g_Prop->m_tImgRes == MidHigh)
	{
		int index = g_Prop->m_nStillResCnt - 1 - 2;
		if(g_Prop->m_tStillImgFmtArray != NULL)
		{
			ImageRes imgRes;
			imgRes.nWidth = g_Prop->m_tStillImgFmtArray[index].nMaxWidth;
			imgRes.nHeight = g_Prop->m_tStillImgFmtArray[index].nMaxHeight;
			if(SUCCEEDED(g_Prop->Set_Resolution(&imgRes, S, FALSE)))
			{
				iError = ERROR_SUCCESS;
			}
		}

		g_Prop->RunGrp();
	}

_exitSetMetaData:
	delete metaData;
	return iError;
}

//------------------------------------------------------------------
//
// Prototype:	int xxx(XXX xxx)
//
// Description:	Exported function to initialize filter graph for camera
//				capture.
//
// Parameters:	hwnd is Handle of the parent window for the directx video
//				window.
//				rc is the client rect for video windo.
//
// Returns:	Boolean "TRUE" if function succeeds otherwise Boolean "FALSE".
// 
// Notes:
//------------------------------------------------------------------
DSHOWCAM_API int SetFlash(FlashSetting setting)
{
	int iError = -1;
	int value = 0;

	if(g_Prop == NULL)
	{
		return -1;
	}

	if(g_Prop->m_tCamTblFlash.plMin == 0 && g_Prop->m_tCamTblFlash.plMax == 0)
	{
		return -1;
	}

	switch(setting)
	{
	case On:
		value = g_Prop->m_tCamTblFlash.plMax;
		g_Prop->bAutoFlash = false;
		break;
	case Off:
		value = g_Prop->m_tCamTblFlash.plMin;
		g_Prop->bAutoFlash = false;
		break;
	case Auto:
		value = g_Prop->m_tCamTblFlash.plMin;
		g_Prop->bAutoFlash = true;
		break;
	}

	g_Prop->m_tCamTblFlash.plVal = value;

	if(SUCCEEDED(g_Prop->Set_PropVal(CAM, &g_Prop->m_tCamTblFlash)))
	{
		iError = ERROR_SUCCESS;
	}

	return iError;
}
DSHOWCAM_API BOOL ResizePreview(int width, int height)
{
    if(g_Prop == NULL)
	{
		return FALSE;
	}
	return g_Prop->ResizePreview(width,height); 
}
DSHOWCAM_API int SetTestResolution(int width, int height)
{
	int iError = -1;
	int index = 0;

	if(g_Prop == NULL)
	{
		return -1;
	}	

	/*
	* To avoid the out of memory error which comes to highest Megapixel rate (8MPX in MC67)
	* Followng resolutions were decided as base values for each resolution.
	*/
	if(g_Prop->m_tStillImgFmtArray != NULL)
	{

		int i = 0;
		for(i=0; i < g_Prop->m_nStillResCnt; i++)
		{
			if( width == abs(g_Prop->m_tStillImgFmtArray[i].nMaxWidth) && 
				height == abs(g_Prop->m_tStillImgFmtArray[i].nMaxHeight))
			{
				break;		
			}
		}
		if( i == g_Prop->m_nStillResCnt)
		{
			i = 0;
		}
		ImageRes imgRes;
		imgRes.nWidth = g_Prop->m_tStillImgFmtArray[i].nMaxWidth;
		imgRes.nHeight = g_Prop->m_tStillImgFmtArray[i].nMaxHeight;
		if(SUCCEEDED(g_Prop->Set_Resolution(&imgRes, S, TRUE)))
		{
			iError = ERROR_SUCCESS;
		}
	}

	return iError;
}
//------------------------------------------------------------------
//
// Prototype:	int xxx(XXX xxx)
//
// Description:	Exported function to initialize filter graph for camera
//				capture.
//
// Parameters:	hwnd is Handle of the parent window for the directx video
//				window.
//				rc is the client rect for video windo.
//
// Returns:	Boolean "TRUE" if function succeeds otherwise Boolean "FALSE".
// 
// Notes:
//------------------------------------------------------------------
DSHOWCAM_API int SetCameraResolution(CameraSetting setting)
{
	int iError = -1;
	int index = 0;

	if(g_Prop == NULL)
	{
		return -1;
	}

	g_Prop->m_tImgRes = setting;
	//switch(setting)	
	//{
	//case High:
	//	index = g_Prop->m_nStillResCnt - 1 - 1;
	//	break;
	//case Medium:
	//	index = g_Prop->m_nStillResCnt / 2;
	//	break;
	//case Low:
	//	index = 0;
	//	break;
	//case MidHigh:
	//	index = g_Prop->m_nStillResCnt - 1 - 2;
	//	break;
	//}

	/*
	* To avoid the out of memory error which comes to highest Megapixel rate (8MPX in MC67)
	* Followng resolutions were decided as base values for each resolution.
	*/
	if(g_Prop->m_tStillImgFmtArray != NULL)
	{
		DWORD *pArea = new DWORD[g_Prop->m_nStillResCnt];
		if(NULL != pArea)
		{
			int i = 0;
			for(i=0; i < g_Prop->m_nStillResCnt; i++)
			{
				pArea[i] = abs(g_Prop->m_tStillImgFmtArray[i].nMaxWidth * g_Prop->m_tStillImgFmtArray[i].nMaxHeight);
				if((setting == Low) && (pArea[i] >= LOW)){					
					break;
				}else if((setting == Medium) && (pArea[i] >= MEDIUM)){
					break;
				}else if((setting == MidHigh) && (pArea[i] >= MIDHIGH)){
					break;
				}else if((setting == High) && (pArea[i] >= HIGH)){
					break;
				}
			}
			index = i;
			delete [] pArea;
		}

		ImageRes imgRes;
		imgRes.nWidth = g_Prop->m_tStillImgFmtArray[index].nMaxWidth;
		imgRes.nHeight = g_Prop->m_tStillImgFmtArray[index].nMaxHeight;
		if(SUCCEEDED(g_Prop->Set_Resolution(&imgRes, S, TRUE)))
		{
			iError = ERROR_SUCCESS;
		}
	}

	return iError;
}
//------------------------------------------------------------------
//
// Prototype:	int xxx(XXX xxx)
//
// Description:	Exported function to initialize filter graph for camera
//				capture.
//
// Parameters:	hwnd is Handle of the parent window for the directx video
//				window.
//				rc is the client rect for video windo.
//
// Returns:	Boolean "TRUE" if function succeeds otherwise Boolean "FALSE".
// 
// Notes:
//------------------------------------------------------------------
DSHOWCAM_API int SetBrightness(CameraSetting setting)
{
	int iError = -1;
	int value = 0;
	int mean = 0;

	if(g_Prop == NULL)
	{
		return -1;
	}

	if(g_Prop->m_tImgTblBrt.plMin == 0 && g_Prop->m_tImgTblBrt.plMax == 0)
	{
		return -1;
	}

	switch(setting)
	{
	case High:
		value = g_Prop->m_tImgTblBrt.plMax;
		break;
	case Medium:
		mean = (g_Prop->m_tImgTblBrt.plMax - g_Prop->m_tImgTblBrt.plMin)/2;
		value = mean + (mean % g_Prop->m_tImgTblBrt.plDelta);
		break;
	case Low:
		value = g_Prop->m_tImgTblBrt.plMin;
		break;
	}

	g_Prop->m_tImgTblBrt.plVal = value;
	if(SUCCEEDED(g_Prop->Set_PropVal(IMG, &g_Prop->m_tImgTblBrt)))
	{
		iError = ERROR_SUCCESS;
	}

	return iError;
}

//------------------------------------------------------------------
//
// Prototype:	int xxx(XXX xxx)
//
// Description:	Exported function to initialize filter graph for camera
//				capture.
//
// Parameters:	hwnd is Handle of the parent window for the directx video
//				window.
//				rc is the client rect for video windo.
//
// Returns:	Boolean "TRUE" if function succeeds otherwise Boolean "FALSE".
// 
// Notes:
//------------------------------------------------------------------
DSHOWCAM_API void CloseDShow()
{
	if(g_Prop != NULL)
	{
		g_Prop->FreeFilterGrp();
		if(g_Prop->m_tStillImgFmtArray != NULL)
		{
			delete [] g_Prop->m_tStillImgFmtArray;
		}
		//Re-Enable power key (which was applied in InitDShow() to prevent
		//DShow freezing issue after suspend-resumes)
		/*if(FALSE != g_Prop->IsIACDEvice())
		{
			g_Prop->EnablePowerKeyIAC(TRUE);
		}
		else
		{
			g_Prop->EnablePowerKeyMPA(TRUE);
		}*/
		delete g_Prop;
		g_Prop = NULL;
	}
	/*if(hGlobal != NULL)
	{
	GlobalFree(hGlobal);
	hGlobal = NULL;
	}*/

	//if(m_hEventWakeup != NULL)
	//{
	//	CloseHandle(m_hEventWakeup);
	//	m_hEventWakeup = NULL;
	//}

	//g_bStopWorkerThread = TRUE;
	//if(g_pEventArray)
	//{
	//	if(g_pEventArray[0] != NULL)
	//	{
	//		SetEvent(g_pEventArray[0]);
	//	}
	//	if(WAIT_TIMEOUT == WaitForSingleObject(g_hWorkerThread, 1000))
	//	{
	//		TerminateThread(g_hWorkerThread, 0);
	//	}
	//	CloseHandle(g_hWorkerThread);
	//	g_hWorkerThread = NULL;

	//	if(g_pEventArray[0] != NULL)
	//	{
	//		CloseHandle(g_pEventArray[0]);
	//		g_pEventArray[0] = NULL;
	//	}
	//	if(g_pEventArray[1] != NULL)
	//	{
	//		CloseMsgQueue(g_pEventArray[1]);
	//		g_pEventArray[1] = NULL;
	//	}

	//	free(g_pEventArray);
	//	g_pEventArray = NULL;

	//	StopPowerNotifications(g_hRegPwrNotif);
	//	/*if(g_hRegPwrNotif != NULL)
	//	{
	//	CloseHandle(g_hRegPwrNotif);
	//	}*/
	//	g_hRegPwrNotif = NULL;
	//}
}
DSHOWCAM_API int InitDShow(int hWnd, wchar_t* CamId, RECT& rc)
 {
	 int iError = -1;
	 if(NULL == g_Prop)
	 {
		 //Create a new instance CDShowCamModule class
		 g_Prop = new CDShowCamModule();
		 if(NULL != g_Prop)
		 {
			 if(SUCCEEDED(g_Prop->InitFilterGrp()))
			 {	
				 CamConfig cfg;
				 cfg.sCamID = CamId;	// Pass that selected Camera Id to the "BuildFilterGraph(...)" function.
				 cfg.rc = rc;
				 cfg.hwndOwnerWnd = (HWND)hWnd;
				 cfg.bIsAudioEnb = FALSE;	
				 cfg.bIsCapEnb = TRUE;
				 cfg.bIsPrvEnb = TRUE;
				 cfg.bIsStillEnb = TRUE;

				 if(g_Prop->BuildFilterGrp(&cfg))
				 {
					 if(g_Prop->RunGrp())				
					 {
						 iError = ERROR_SUCCESS;
					 }
					 else
					 {
						 iError = -1;
						 goto _exitInvoke;
					 }

					 //Cache the flash capabilities for future control.
					 //g_Prop->m_tCamTblFlash.p = CProp[7];
					 g_Prop->m_tCamTblFlash.p = CameraControl_Flash;
					 if(SUCCEEDED(g_Prop->Get_PropRng(CAM, &g_Prop->m_tCamTblFlash)))
					 {
						 //Make sure flash is off at launch time.
						 if(g_Prop->m_tCamTblFlash.plVal > g_Prop->m_tCamTblFlash.plMin)
						 {
							 g_Prop->m_tCamTblFlash.plVal = g_Prop->m_tCamTblFlash.plMin;
							 if(SUCCEEDED(g_Prop->Set_PropVal(CAM, &g_Prop->m_tCamTblFlash)))
							 {
								 iError = ERROR_SUCCESS;
							 }
							 else
							 {
								 iError = -1;
								 goto _exitInvoke;
							 }
						 }
					 }
					 else
					 {
						 iError = -1;
						 goto _exitInvoke;
					 }

					 //Cache the resolution capabilities for future control.
					 ImgFmt fmt;
					 int resCnt = 0;
					 if(SUCCEEDED(g_Prop->EnumFirstCap(S, &fmt, &resCnt)))
					 {
						 ImgFmt *pfmt = new ImgFmt[resCnt];
						 memcpy(pfmt, &fmt, sizeof(fmt));
						 if(SUCCEEDED(g_Prop->EnumOtherCap(S, &pfmt[1], resCnt)))
						 {
							 g_Prop->m_tStillImgFmtArray = pfmt;
							 g_Prop->m_nStillResCnt = resCnt;
							 iError = ERROR_SUCCESS;
						 }


						 else
						 {

							 delete [] pfmt; // deallocate the memory if their is a failure
							 iError = -1;
							 goto _exitInvoke;
						 }

					 }
					 else
					 {
						 iError = -1;
						 goto _exitInvoke;
					 }

					 //--------------- MPA3 ----------------------
					 //For MPA3 platform with 3 pins does the following behavior needed. Can we isolate it 
					 //only to the two pin environment.
					 //-------------------------------------------
					 if(!IsMPA3()){
						 //Preview pin is rendered via Smart Tee filter which in turn takes 
						 //Video pin of the capture filter. Hence to increase preview res
						 //we need to increase video resolution. Putting it to highest as of now.
						 resCnt = 0;
						 if(SUCCEEDED(g_Prop->EnumFirstCap(V, &fmt, &resCnt)))
						 {
							 ImgFmt *pfmt = new ImgFmt[resCnt];
							 memcpy(pfmt, &fmt, sizeof(fmt));
							 if(SUCCEEDED(g_Prop->EnumOtherCap(V, &pfmt[1], resCnt)))
							 {
								 //Now set the resolution to the middle value
								 //nisha added absolute function to give a positive value to imgRes.nHeight
								 ImageRes imgRes;
								 imgRes.nWidth = abs(pfmt[resCnt - 1].nMaxWidth);
								 imgRes.nHeight = abs(pfmt[resCnt - 1].nMaxHeight);

								 if(SUCCEEDED(g_Prop->Set_Resolution(&imgRes, V, TRUE)))
								 {
									 iError = ERROR_SUCCESS;
								 }
							 }

							 delete [] pfmt;

						 }
						 else
						 {
							 iError = -1;
							 goto _exitInvoke;
						 }
					 }
					 //Cache the brightness capabilities for future control.
					 ImgPropTbl imgTbl;
					 imgTbl.vProp = IProp[0];
					 imgTbl.plDef = 0;
					 imgTbl.plDelta = 0;
					 imgTbl.plFlag = 0;
					 imgTbl.plMax = 0;
					 imgTbl.plMin = 0;
					 imgTbl.plVal = 0;
					 imgTbl.hr = NULL;
					 if(SUCCEEDED(g_Prop->Get_PropRng(IMG, &imgTbl)))
					 {

						 int mean = (imgTbl.plMax - imgTbl.plMin)/2;
						 int value = mean + (mean % imgTbl.plDelta);
						 //Make sure brightness is medium at launch time
						 if(imgTbl.plVal != value)
						 {
							 imgTbl.plVal = value;
							 if(SUCCEEDED(g_Prop->Set_PropVal(IMG, &imgTbl)))
							 {
								 iError = ERROR_SUCCESS;
							 }
							 else
							 {
								 iError = -1;
								 goto _exitInvoke;
							 }
						 }
						 g_Prop->m_tImgTblBrt = imgTbl;
					 }
					 else
					 {
						 iError = -1;
						 goto _exitInvoke;
					 }
				 }
				 else
				 {
					 iError = -1;
					 goto _exitInvoke;
				 }

			 }
		 }
	 }
_exitInvoke:
	 return iError;
 }

//------------------------------------------------------------------
//
// Prototype:	int xxx(XXX xxx)
//
// Description:	Exported function to initialize filter graph for camera
//				capture.
//
// Parameters:	hwnd is Handle of the parent window for the directx video
//				window.
//				rc is the client rect for video windo.
//
// Returns:	Boolean "TRUE" if function succeeds otherwise Boolean "FALSE".
// 
// Notes:
//------------------------------------------------------------------
int Invoke(HWND hWnd, RECT rc, wstring Cam)
{
	CamConfig cfg;
	int iError = -1;

	if(g_Prop == NULL)
	{
		return -1;
	}

	if(SUCCEEDED(g_Prop->InitFilterGrp()))
	{	
		cfg.sCamID = Cam;	// Pass that selected Camera Id to the "BuildFilterGraph(...)" function.
		cfg.rc = rc;
		cfg.hwndOwnerWnd = hWnd;
		cfg.bIsAudioEnb = FALSE;	
		cfg.bIsCapEnb = TRUE;
		cfg.bIsPrvEnb = TRUE;
		cfg.bIsStillEnb = TRUE;

		if(g_Prop->BuildFilterGrp(&cfg))
		{
			if(g_Prop->RunGrp())				
			{
				iError = ERROR_SUCCESS;
			}
			else
			{
				iError = -1;
				goto _exitInvoke;
			}

			//Cache the flash capabilities for future control.
			//g_Prop->m_tCamTblFlash.p = CProp[7];
			g_Prop->m_tCamTblFlash.p = CameraControl_Flash;
			if(SUCCEEDED(g_Prop->Get_PropRng(CAM, &g_Prop->m_tCamTblFlash)))
			{
				//Make sure flash is off at launch time.
				if(g_Prop->m_tCamTblFlash.plVal > g_Prop->m_tCamTblFlash.plMin)
				{
					g_Prop->m_tCamTblFlash.plVal = g_Prop->m_tCamTblFlash.plMin;
					if(SUCCEEDED(g_Prop->Set_PropVal(CAM, &g_Prop->m_tCamTblFlash)))
					{
						iError = ERROR_SUCCESS;
					}
					else
					{
						iError = -1;
						goto _exitInvoke;
					}
				}
			}
			else
			{
				iError = -1;
				goto _exitInvoke;
			}

			//Cache the resolution capabilities for future control.
			ImgFmt fmt;
			int resCnt = 0;
			if(SUCCEEDED(g_Prop->EnumFirstCap(S, &fmt, &resCnt)))
			{
				ImgFmt *pfmt = new ImgFmt[resCnt];
				memcpy(pfmt, &fmt, sizeof(fmt));
				if(SUCCEEDED(g_Prop->EnumOtherCap(S, &pfmt[1], resCnt)))
				{
					g_Prop->m_tStillImgFmtArray = pfmt;
					g_Prop->m_nStillResCnt = resCnt;
					iError = ERROR_SUCCESS;
				}


				else
				{

					delete [] pfmt; // deallocate the memory if their is a failure
					iError = -1;
					goto _exitInvoke;
				}

			}
			else
			{
				iError = -1;
				goto _exitInvoke;
			}
			
			//--------------- MPA3 ----------------------
			//For MPA3 platform with 3 pins does the following behavior needed. Can we isolate it 
			//only to the two pin environment.
			//-------------------------------------------
			if(!IsMPA3()){
				//Preview pin is rendered via Smart Tee filter which in turn takes 
				//Video pin of the capture filter. Hence to increase preview res
				//we need to increase video resolution. Putting it to highest as of now.
				resCnt = 0;
				if(SUCCEEDED(g_Prop->EnumFirstCap(V, &fmt, &resCnt)))
				{
					ImgFmt *pfmt = new ImgFmt[resCnt];
					memcpy(pfmt, &fmt, sizeof(fmt));
					if(SUCCEEDED(g_Prop->EnumOtherCap(V, &pfmt[1], resCnt)))
					{
						//Now set the resolution to the middle value
						//nisha added absolute function to give a positive value to imgRes.nHeight
						ImageRes imgRes;
						imgRes.nWidth = abs(pfmt[resCnt - 1].nMaxWidth);
						imgRes.nHeight = abs(pfmt[resCnt - 1].nMaxHeight);

						if(SUCCEEDED(g_Prop->Set_Resolution(&imgRes, V, TRUE)))
						{
							iError = ERROR_SUCCESS;
						}
					}

					delete [] pfmt;

				}
				else
				{
					iError = -1;
					goto _exitInvoke;
				}
			}
			//Cache the brightness capabilities for future control.
			ImgPropTbl imgTbl;
			imgTbl.vProp = IProp[0];
			imgTbl.plDef = 0;
			imgTbl.plDelta = 0;
			imgTbl.plFlag = 0;
			imgTbl.plMax = 0;
			imgTbl.plMin = 0;
			imgTbl.plVal = 0;
			imgTbl.hr = NULL;
			if(SUCCEEDED(g_Prop->Get_PropRng(IMG, &imgTbl)))
			{

					int mean = (imgTbl.plMax - imgTbl.plMin)/2;
					int value = mean + (mean % imgTbl.plDelta);
					//Make sure brightness is medium at launch time
					if(imgTbl.plVal != value)
					{
						imgTbl.plVal = value;
						if(SUCCEEDED(g_Prop->Set_PropVal(IMG, &imgTbl)))
						{
							iError = ERROR_SUCCESS;
						}
						else
						{
							iError = -1;
							goto _exitInvoke;
						}
					}
					g_Prop->m_tImgTblBrt = imgTbl;
			}
			else
			{
				iError = -1;
				goto _exitInvoke;
			}
		}
		else
		{
			iError = -1;
			goto _exitInvoke;
		}

	}		
	else
	{
		iError = -1;
		goto _exitInvoke;
	}

_exitInvoke:
	//g_Prop->FreeFilterGrp();
	return iError;
}

//------------------------------------------------------------------
//
// Prototype:	int xxx(XXX xxx)
//
// Description:	Exported function to initialize filter graph for camera
//				capture.
//
// Parameters:	hwnd is Handle of the parent window for the directx video
//				window.
//				rc is the client rect for video windo.
//
// Returns:	Boolean "TRUE" if function succeeds otherwise Boolean "FALSE".
// 
// Notes:
//------------------------------------------------------------------
//IStream* CreateStreamByFileName(const CString& strFileName)
//{
//	/*if( NULL != hGlobal)
//	{
//		GlobalFree(hGlobal);
//		hGlobal = NULL;
//
//	}*/
//
//   HANDLE hFile = CreateFile(strFileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
//    if (hFile == INVALID_HANDLE_VALUE) {
//        return 0;
//    }
//   
//   DWORD dwFileSize = GetFileSize(hFile, NULL);
//
//    if (dwFileSize == (DWORD)-1)
//    {
//        CloseHandle(hFile);
//        return 0;
//    }	
//
//	hGlobal = GlobalAlloc(GMEM_MOVEABLE, dwFileSize);
//
//    if (hGlobal == NULL) {
//        CloseHandle(hFile);
//        return 0;
//    }
//
//    LPVOID pvData = GlobalLock(hGlobal);
//
//    if (pvData == NULL) {
//        GlobalUnlock(hGlobal);
//        CloseHandle(hFile);
//        return 0;
//    }
//
//    DWORD dwBytesRead = 0;
//    BOOL bRead = ReadFile(hFile, pvData, dwFileSize, &dwBytesRead, NULL);
//    GlobalUnlock(hGlobal);
//    CloseHandle(hFile);
//
//    if (!bRead)
//    {
//        return 0;
//    }
//
//   IStream* pStream = 0;
//   if (FAILED(CreateStreamOnHGlobal(hGlobal, TRUE, &pStream))) {
//      return 0;
//   }
//
//
//   return pStream;
//}

IStream* CreateStreamByFileName(const CString& strFileName)
{

	HANDLE hFile = CreateFile(strFileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		return 0;
	}

	DWORD dwFileSize = GetFileSize(hFile, NULL);

	if (dwFileSize == (DWORD)-1)
	{
		CloseHandle(hFile);
		return 0;
	}

	IStream* pStream = 0;
	if (FAILED(CreateStreamOnHGlobal(NULL, TRUE, &pStream))) {
		CloseHandle(hFile);
		return 0;
	}

	LPVOID pvData = (LPVOID)LocalAlloc(LPTR, dwFileSize) ;
	if(pvData != NULL)
	{
		DWORD dwBytesRead = 0;
		BOOL bRead = ReadFile(hFile, pvData, dwFileSize, &dwBytesRead, NULL);
		if(bRead)
		{
			DWORD dwWritten = 0;
			pStream->Write(pvData, dwBytesRead, &dwWritten);	
		}

		LocalFree(pvData);			

	}
	CloseHandle(hFile);
	return pStream;

}

//------------------------------------------------------------------
//
// Prototype:	BOOL xxx(XXX xxx)
//
// Description:	check if the device is in MPA3 Platform.
//
// Parameters:	void
//
// Returns:	Boolean "TRUE" if function succeeds otherwise Boolean "FALSE".
// 
// Notes:
//------------------------------------------------------------------
DSHOWCAM_API BOOL IsMPA3()
{
OutputDebugString(L"\nBOOL IsMPA3()\n");
	DWORD       fileAttr;
    fileAttr = GetFileAttributes(_T("\\Windows\\SensorAPI.dll"));
    if (0xFFFFFFFF == fileAttr && GetLastError() == ERROR_FILE_NOT_FOUND)
        return FALSE;
    return TRUE;
}
//------------------------------------------------------------------
//
// Prototype:	int xxx(XXX xxx)
//
// Description:	Exported function to initialize filter graph for camera
//				capture.
//
// Parameters:	hwnd is Handle of the parent window for the directx video
//				window.
//				rc is the client rect for video windo.
//
// Returns:	Boolean "TRUE" if function succeeds otherwise Boolean "FALSE".
// 
// Notes:
//------------------------------------------------------------------
bool Degrees2DegreesMinutesSeconds(double coordinate, LONG *rationalCoordinate)
{
	rationalCoordinate[0] = (int)coordinate;
	rationalCoordinate[1] = 1;
	double fraction = ((coordinate - rationalCoordinate[0]) * 60.0);
	rationalCoordinate[2] = (int)fraction;
	rationalCoordinate[3] = 1;
	fraction = ((fraction - rationalCoordinate[2]) * 60.0);
	rationalCoordinate[4] = (int)(fraction * 100.0);
	rationalCoordinate[5] = 100;

	return true;
}

//------------------------------------------------------------------
//
// Prototype:	int xxx(XXX xxx)
//
// Description:	Exported function to initialize filter graph for camera
//				capture.
//
// Parameters:	hwnd is Handle of the parent window for the directx video
//				window.
//				rc is the client rect for video windo.
//
// Returns:	Boolean "TRUE" if function succeeds otherwise Boolean "FALSE".
// 
// Notes:
//------------------------------------------------------------------
bool DegreesMinutesSeconds2Degrees(double *coordinate, LONG *rationalCoordinate)
{
	*coordinate =  rationalCoordinate[0]/rationalCoordinate[1];
	*coordinate += ((rationalCoordinate[2]/rationalCoordinate[3]) / 60.0);
	*coordinate += ((rationalCoordinate[4]/rationalCoordinate[5]) / 60.0 / 60.0);
	return true;
}

//------------------------------------------------------------------
//
// Prototype:	int xxx(XXX xxx)
//
// Description:	Exported function to initialize filter graph for camera
//				capture.
//
// Parameters:	hwnd is Handle of the parent window for the directx video
//				window.
//				rc is the client rect for video windo.
//
// Returns:	Boolean "TRUE" if function succeeds otherwise Boolean "FALSE".
// 
// Notes:
//------------------------------------------------------------------
bool UpdateImageFileWithMetaData(LPCTSTR lpszPictureFilePath, JpegMetadata *jmMetadata)
{

	CString lpszModPictureFilePath = lpszPictureFilePath;
	lpszModPictureFilePath = lpszModPictureFilePath + _T("_");

	IImagingFactory*    pImgFactory			= NULL;
	//IImage*             pImage				= NULL;
	IStream*			pFileStream			= NULL;
	IImageDecoder*		pImageDecoder		= NULL;
	IImageEncoder*		pImageEncoder		= NULL;
	IImageSink*			pImageSink			= NULL;
	PropertyItem*		pis					= NULL;


	bool                fIsClsFounded		= false;
	GUID                guidImageEncoderCls;
	HRESULT             hr;

	hr = CoCreateInstance ( CLSID_ImagingFactory,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_IImagingFactory,
		(void **)&pImgFactory);
	if (SUCCEEDED(hr))
	{
		pFileStream		= CreateStreamByFileName(lpszPictureFilePath);
		pImageDecoder	= 0;

		hr = pImgFactory->CreateImageDecoder(pFileStream, DecoderInitFlagNone, &pImageDecoder);
		if (SUCCEEDED(hr))
		{
			ImageInfo* pImageInfo = new ImageInfo();
			if(pImageInfo != NULL)
			{
			hr = pImageDecoder->GetImageInfo(pImageInfo);
			if (SUCCEEDED(hr))
			{
				UINT nEncodersCount = 0;
				ImageCodecInfo* pEncoders = NULL;

				hr = pImgFactory->GetInstalledEncoders(&nEncodersCount, &pEncoders);
				if (SUCCEEDED(hr))
				{
					for (UINT i = 0; i < nEncodersCount; i++)
					{
						if (pEncoders[i].FormatID == pImageInfo->RawDataFormat)
						{
							guidImageEncoderCls = pEncoders[i].Clsid;
							fIsClsFounded = true;
							break;
						}//if
					}//for
				}//if

				//delete pImageInfo;

				// encoder founded now goes to properties
				if (fIsClsFounded)
				{
					PropertyItem pi;

					// add latitude
					pi.id = PropertyTagGpsLatitude;
					pi.type = PropertyTagTypeRational;
					pi.length = sizeof(LONG) * 6;
					pi.value = jmMetadata->ulLatitude;

					hr = pImageDecoder->SetPropertyItem(pi);
					//if(FAILED(hr))
					//{
					//	if(pImageInfo != NULL)
					//	{
					//		delete pImageInfo;
					//	}
					//	goto _exitUpdateMetadata;
					//}
					if(SUCCEEDED(hr))
					{
						// add longitude
						pi.id = PropertyTagGpsLongitude;
						pi.type = PropertyTagTypeRational;
						pi.length = sizeof(LONG) * 6;
						pi.value = jmMetadata->ulLongitude;

						hr = pImageDecoder->SetPropertyItem(pi);
						//if(FAILED(hr))
						//{
						//	if(pImageInfo != NULL)
						//	{
						//		delete pImageInfo;
						//	}
						//	goto _exitUpdateMetadata;
						//}
						if(SUCCEEDED(hr))
						{

							// add date/time
							pi.id = PropertyTagDateTime;
							pi.type = PropertyTagTypeASCII;
							pi.length = sizeof(CHAR) * 20;
							pi.value = jmMetadata->szDateTime;

							hr = pImageDecoder->SetPropertyItem(pi);
							//if(FAILED(hr))
							//{
							//	if(pImageInfo != NULL)
							//	{
							//		delete pImageInfo;
							//	}
							//	goto _exitUpdateMetadata;
							//}
							if(SUCCEEDED(hr))
							{
								hr = pImgFactory->CreateImageEncoderToFile(
									&guidImageEncoderCls,
									lpszModPictureFilePath,
									&pImageEncoder);

								if (SUCCEEDED(hr))
								{
									hr = pImageEncoder->GetEncodeSink(&pImageSink);
									if (SUCCEEDED(hr))
									{
										UINT uiPropertiesCount = 0;

										hr = pImageDecoder->GetPropertyCount(&uiPropertiesCount);
										if (SUCCEEDED(hr))
										{
											UINT totalBufferSize = 0;
											UINT numProperties = 0;

											hr = pImageDecoder->GetPropertySize(&totalBufferSize, &numProperties);
											if (SUCCEEDED(hr))
											{
												pis = (PropertyItem*)malloc(totalBufferSize);

												hr = pImageDecoder->GetAllPropertyItems(
													totalBufferSize,
													numProperties,
													pis);

												if (SUCCEEDED(hr))
												{
													hr = pImageSink->PushPropertyItems(
														numProperties,
														totalBufferSize,
														pis);
													if (SUCCEEDED(hr))
													{
														hr = pImageDecoder->BeginDecode(pImageSink, NULL);
														if (SUCCEEDED(hr))
														{
															while (true)
															{
																hr = pImageDecoder->Decode();
																if (hr == E_PENDING)
																{
																	continue;
																}
																else if (FAILED(hr))
																{
																	RETAILMSG(1 , (_T("IImageDecoder::Decode() failed hr = %d"), hr));
																	break;
																}
																else if (SUCCEEDED(hr))
																{
																	break;
																}
																//if
															}//while
															pImageDecoder->EndDecode(hr);

														}//if
													}//if

													//free(pis);
												}//if
											}//if
										}//if

										//pImageSink->Release();
									}//if

									//pImageEncoder->TerminateEncoder();
									//pImageEncoder->Release();
								}//if

							}
						}
					}
				}//if
			}//if

			delete pImageInfo;

			}
		}//if
	}//if
	

	if(pImageSink)
	{
		pImageSink->Release();
	}

	if(pImageEncoder)
	{
		pImageEncoder->TerminateEncoder();
		pImageEncoder->Release();
	}

	if(pis)
	{
		free(pis);
	}

	if(pImageDecoder)
	{
		pImageDecoder->Release();
	}

	if(pImgFactory)
	{
		pImgFactory->Release();
	}

	if(pFileStream)
	{
		pFileStream->Release();
	}

	// try to remove old file with new one
	if (SUCCEEDED(hr))
	{
		hr = S_FALSE;

		if (DeleteFile(lpszPictureFilePath))
		{
			if (MoveFile(lpszModPictureFilePath, lpszPictureFilePath))
			{
				hr = S_OK;
			}//if
		}//if
	}//if
	else
	{
		DeleteFile(lpszModPictureFilePath);
	}

	return SUCCEEDED(hr);
}

//------------------------------------------------------------------
//
// Prototype:	int xxx(XXX xxx)
//
// Description:	Exported function to initialize filter graph for camera
//				capture.
//
// Parameters:	hwnd is Handle of the parent window for the directx video
//				window.
//				rc is the client rect for video windo.
//
// Returns:	Boolean "TRUE" if function succeeds otherwise Boolean "FALSE".
// 
// Notes:
//------------------------------------------------------------------
bool ReadImageFileForMetaData(LPCTSTR lpszPictureFilePath, JpegMetadata *jmMetadata)
{
	CString lpszModPictureFilePath = lpszPictureFilePath;
	lpszModPictureFilePath = lpszModPictureFilePath + _T("_");

	IImagingFactory*    pImgFactory			= NULL;
	//IImage*             pImage				= NULL;
	IStream*			pFileStream			= NULL;
	IImageDecoder*		pImageDecoder		= NULL;

	bool                fIsClsFounded		= false;
	GUID                guidImageEncoderCls;
	HRESULT             hr;

	hr = CoCreateInstance ( CLSID_ImagingFactory,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_IImagingFactory,
		(void **)&pImgFactory);
	if (SUCCEEDED(hr))
	{
		pFileStream		= CreateStreamByFileName(lpszPictureFilePath);
		pImageDecoder	= 0;

		hr = pImgFactory->CreateImageDecoder(pFileStream, DecoderInitFlagNone, &pImageDecoder);
		if (SUCCEEDED(hr))
		{
			ImageInfo* pImageInfo = new ImageInfo();
			if(pImageInfo != NULL)
			{
			hr = pImageDecoder->GetImageInfo(pImageInfo);
			if (SUCCEEDED(hr))
			{
				UINT nEncodersCount = 0;
				ImageCodecInfo* pEncoders = NULL;

				hr = pImgFactory->GetInstalledEncoders(&nEncodersCount, &pEncoders);
				if (SUCCEEDED(hr))
				{
					for (UINT i = 0; i < nEncodersCount; i++)
					{
						if (pEncoders[i].FormatID == pImageInfo->RawDataFormat)
						{
							guidImageEncoderCls = pEncoders[i].Clsid;
							fIsClsFounded = true;
							break;
						}//if
					}//for
				}//if

				//delete pImageInfo;

				// encoder founded now goes to properties
				if (fIsClsFounded)
				{
					UINT size;
					// add latitude
					hr = pImageDecoder->GetPropertyItemSize(PropertyTagGpsLatitude, &size);
					if(SUCCEEDED(hr)/* && (size == (sizeof(LONG) * 6 + sizeof(ULONG) * 2 + sizeof(WORD)))*/)
					{
						PropertyItem* pis = (PropertyItem*)malloc(size);
						if(pis != NULL)
						{
							hr = pImageDecoder->GetPropertyItem(PropertyTagGpsLatitude, size, pis);
							if(SUCCEEDED(hr) && pis->length == sizeof(LONG) * 6)
							{
								memcpy(jmMetadata->ulLatitude, pis->value, pis->length);
							}
							free(pis);
						}
					
					/*else
					{
						if(pImageInfo != NULL)
						{
							delete pImageInfo;
						}    
						goto _exitReadMetadata;
					}*/

					// add longitude
					hr = pImageDecoder->GetPropertyItemSize(PropertyTagGpsLongitude, &size);
					if(SUCCEEDED(hr)/* && (size == (sizeof(LONG) * 6 + sizeof(ULONG) * 2 + sizeof(WORD)))*/)
					{
						PropertyItem* pis = (PropertyItem*)malloc(size);
						if(pis != NULL)
						{
							hr = pImageDecoder->GetPropertyItem(PropertyTagGpsLongitude, size, pis);
							if(SUCCEEDED(hr) && pis->length == sizeof(LONG) * 6)
							{
								memcpy(jmMetadata->ulLongitude, pis->value, pis->length);
							}
							free(pis);
						}
					
					//else
					//{
					//	if(pImageInfo != NULL)
					//	{
					//		delete pImageInfo;
					//	}    
					//	goto _exitReadMetadata;
					//}

					// add date/time
					hr = pImageDecoder->GetPropertyItemSize(PropertyTagDateTime, &size);
					if(SUCCEEDED(hr)/* && (size == (sizeof(CHAR) * 20 + sizeof(ULONG) * 2 + sizeof(WORD)))*/)
					{
						PropertyItem* pis = (PropertyItem*)malloc(size);
						if(pis != NULL)
						{
							hr = pImageDecoder->GetPropertyItem(PropertyTagDateTime, size, pis);
							if(SUCCEEDED(hr) && pis->length == sizeof(CHAR) * 20)
							{
								memcpy(jmMetadata->szDateTime, pis->value, pis->length);
							}
							free(pis);
						}
					}
					//else
					//{
					//	if(pImageInfo != NULL)
					//	{
					//		delete pImageInfo;
					//	}
					//	goto _exitReadMetadata;
					//}
					}
					}
				}//if
			}//if
			
			delete pImageInfo;

			}
		}//if
	}//if

//_exitReadMetadata:
	if(pImageDecoder)
	{
		pImageDecoder->Release();
	}

	if(pImgFactory)
	{
		pImgFactory->Release();
	}

	if(pFileStream)
	{
		pFileStream->Release();
	}

	return SUCCEEDED(hr);
}

// This is the constructor of a class that has been exported.
// see DShowCamModule.h for the class definition
CDShowCamModule::CDShowCamModule(void)
{
	//Initialize the COM library
	HRESULT hr = CoInitialize(NULL);
	if(FAILED(hr))
	{
		m_lErrno = 0x001;
		OutputDebugString(L"\n Error Initializing COM !!!\n");
		exit(1);
	}

	//Initializing class members
	//m_hCamHdl = INVALID_HANDLE_VALUE;
	m_pGraphBuilder = NULL;
	m_pCaptureGraphBuilder = NULL;
	m_pMediaControl = NULL;
	//m_pViewWindow = NULL;
	m_pMediaEventEx = NULL;
	m_pVideoCaptureFilter = NULL;
	m_pAudioCaptureFilter = NULL;
	m_pImgCtrl = NULL;
	m_pCamCtrl = NULL;
	m_pStrConf = NULL;
	m_pMux = NULL;
	m_pSink = NULL;
	m_pVideoEncoder = NULL;
	m_pVideoWrapperFilter = NULL;
	m_pStillSink = NULL;
	m_pVideoCtrl = NULL;
	m_pPrvPin = NULL;
	m_wsVFName = DFT_VDO_FN;
	m_pVideoRenderer = NULL;
	//--------------- MPA3 ----------------------
	m_pColorConverter = NULL;
	//-------------------------------------------
	m_nFileAutoCnt = 0;
	bAutoFlash = false;
	m_tStillImgFmtArray = NULL;
	m_bInited = false;
	m_tImgRes = Low;

	memset(&m_tImgTblBrt, 0, sizeof(m_tImgTblBrt));
	memset(&m_tCamTblFlash, 0, sizeof(m_tCamTblFlash));
}

CDShowCamModule::~CDShowCamModule(void)
{
	//if(g_Prop->m_tStillImgFmtArray != NULL)
	//{
	//	delete [] g_Prop->m_tStillImgFmtArray;
	//}
	//Closes the COM library 
	ReleaseGrp();
	CoUninitialize();
}
BOOL CDShowCamModule::ReleaseGrp()
{
	if (m_pImgCtrl)
	{
		m_pImgCtrl->Release();
		m_pImgCtrl = NULL;
	}

	if (m_pCamCtrl)
	{
		m_pCamCtrl->Release();
		m_pCamCtrl = NULL;
	}

	if (m_pStrConf)
	{
		m_pStrConf->Release();
		m_pStrConf = NULL;
	}

	if (m_pVideoCtrl)
	{
		m_pVideoCtrl->Release();
		m_pVideoCtrl = NULL;
	}

	if (m_pPrvPin)
	{
		m_pPrvPin->Release();
		m_pPrvPin = NULL;
	}

	if (m_pVideoRenderer)
	{
		m_pVideoRenderer->Release();
		m_pVideoRenderer = NULL;
	}

	if (m_pStillSink)
	{
		m_pStillSink->Release();
		m_pStillSink = NULL;
	}

	if (m_pSink)
	{
		m_pSink->Release();
		m_pSink = NULL;
	}

	if (m_pMux)
	{
		m_pMux->Release();
		m_pMux = NULL;
	}

	if (m_pVideoWrapperFilter)
	{
		m_pVideoWrapperFilter->Release();
		m_pVideoWrapperFilter = NULL;
	}

	if (m_pVideoEncoder)
	{
		m_pVideoEncoder->Release();
		m_pVideoEncoder = NULL;
	}

	if (m_pMediaSeeking)
	{
		m_pMediaSeeking->Release();
		m_pMediaSeeking = NULL;
	}

	if (m_pMediaEventEx)
	{
		m_pMediaEventEx->Release();
		m_pMediaEventEx = NULL;
	}

	if (m_pMediaControl)
	{
		m_pMediaControl->Release();
		m_pMediaControl = NULL;
	}

	/*if (m_pViewWindow)
	{
		m_pViewWindow->Release();
		m_pViewWindow = NULL;
	}*/

/*************************************************************/

	if (m_pVideoCaptureFilter)
	{
		//IEnumPins* pPins = NULL;
		//if (FAILED(m_pVideoCaptureFilter->EnumPins(&pPins))) 
		//{
		//	RETAILMSG(1,(TEXT("EnumPins failed!")));
		//}
		//	
		//IPin *pPin[10];
		//ULONG n;
		//LONG count=0;
		//pPins->Reset();
		//pPins->Next(10, &pPin[0], &n) ;

		//count = n;
		//while(count >= 1)
		//{
		//	HRESULT newRC = pPin[count-1]->Release();
		//	while(newRC)
		//		newRC = pPin[count-1]->Release();

		//	count--;
		//}
		//
		//pPins->Release();

		m_pVideoCaptureFilter->Release();
		m_pVideoCaptureFilter = NULL;
	}

/*************************************************************/

	if (m_pGraphBuilder)
	{
		IEnumFilters* pFilters;
		if (FAILED(m_pGraphBuilder->EnumFilters(&pFilters))) 
		{
			RETAILMSG(1,(TEXT("EnumFilters failed!")));
		}
			
		IBaseFilter *pFilter[10];
		ULONG n;
		LONG count=0;
		pFilters->Reset();
		pFilters->Next(10, &pFilter[0], &n) ;

		count = n;
		while(count >= 1)
		{
			if (FAILED(m_pGraphBuilder->RemoveFilter(pFilter[count-1])))
			{
				RETAILMSG(1,(TEXT("RemoveFilter failed!")));
			}
			HRESULT newRC = pFilter[count-1]->Release();
			while(newRC)
				newRC = pFilter[count-1]->Release();

			count--;
		}
		
		pFilters->Release();

		m_pGraphBuilder->Release();
		m_pGraphBuilder = NULL;
	}

/*************************************************************/

	if (m_pCaptureGraphBuilder)
	{
		m_pCaptureGraphBuilder->Release();
		m_pCaptureGraphBuilder = NULL;
	}

/*************************************************************/

	if (m_pAudioCaptureFilter)
	{
		m_pAudioCaptureFilter->Release();
		m_pAudioCaptureFilter = NULL;
	}
	return TRUE;
}

//------------------------------------------------------------------
//
// Prototype:	BOOL FindFirstCam(wstring* pwsCamID)
//
// Description:	This is used to enumerate First DirectShow camera driver.
//
// Parameters:	pwsCamID is unicode string pointer. 
//
// Returns:	Boolean "TRUE" if function succeeds otherwise Boolean "FALSE".
// 
// Notes:
//------------------------------------------------------------------
BOOL CDShowCamModule::FindFirstCam(wstring* pwsCamID)
{
	DEVMGR_DEVICE_INFORMATION di;
	*pwsCamID = L"";
	BOOL bResult = FALSE;
	HANDLE hCamHdl = INVALID_HANDLE_VALUE;

	// The driver material doesn't ship as part of the SDK. This GUID is hardcoded
	// here to be able to enumerate the camera drivers and pass the name of the driver to the video capture filter
	GUID guidCamera = {0xCB998A05, 0x122C, 0x4166, 0x84, 0x6A, 0x93, 0x3E, 0x4D, 0x7E, 0x3C, 0x86};

	di.dwSize = sizeof(di);
	hCamHdl = FindFirstDevice(DeviceSearchByGuid, &guidCamera, &di);
	if((hCamHdl == INVALID_HANDLE_VALUE) || (di.hDevice == NULL))
	{
		OutputDebugString(L"\n!!! No Camera found !!!!\n");
		m_lErrno = 0x003;
	}
	else 
	{
		wcscpy((wchar_t* )pwsCamID->c_str(), di.szLegacyName);
		bResult = TRUE;
	}

	if(INVALID_HANDLE_VALUE != hCamHdl)
	{
		FindClose(hCamHdl);
	}

	return bResult;
} //end FindFirstCam

//------------------------------------------------------------------
//
// Prototype:	BOOL FindNextCam(wstring* pwsCamID)
//
// Description:	This is used to enumerate Next available 
//			 	DirectShow camera driver.
//
// Parameters:	pwsCamID is unicode string pointer. 
//
// Returns:	Boolean "TRUE" if function succeeds otherwise Boolean "FALSE".
// 
// Notes:
//------------------------------------------------------------------
/*BOOL CDShowCamModule::FindNextCam(wstring* pwsCamID)
{
DEVMGR_DEVICE_INFORMATION di;
*pwsCamID = L"";

if(m_hCamHdl == INVALID_HANDLE_VALUE)
{
m_lErrno = 0x003;
}
else 
{
if(FindNextDevice(m_hCamHdl, &di))
{
wcscpy((wchar_t* )pwsCamID->c_str(), di.szLegacyName);
return TRUE;
}

}
return FALSE;

}*/ //end FindNextCam

//------------------------------------------------------------------
//
// Prototype:	HRESULT InitFilterGrp()
//
// Description:	This is used to create the Filter Graph Manager(FGM) and
//				initialize several filter graph related components.
//
// Parameters:	none
//
// Returns:	S_OK if all required components are created successfully.
// 
// Notes:
//------------------------------------------------------------------
HRESULT CDShowCamModule::InitFilterGrp()
{
	HRESULT hr;

	// Create the Filter Graph Manager.
	hr = CoCreateInstance(CLSID_FilterGraph, 0, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void**)&m_pGraphBuilder);
	if(SUCCEEDED(hr))
	{
		// Create the Capture Graph Builder.
		hr = CoCreateInstance(CLSID_CaptureGraphBuilder, NULL, CLSCTX_INPROC_SERVER,\
			IID_ICaptureGraphBuilder2, (void**)&m_pCaptureGraphBuilder);         
		if(SUCCEEDED(hr))
		{
			//Specifies the Filter Graph for the Capture Graph Builder to use.
			hr = m_pCaptureGraphBuilder->SetFiltergraph(m_pGraphBuilder);

			if(SUCCEEDED(hr))
			{
				//Obtain the interfaces for media control, video and media event interfaces
				hr = m_pGraphBuilder->QueryInterface (IID_IMediaControl, (void **)&m_pMediaControl);
				if(FAILED(hr))
					return hr;

				/*hr = m_pGraphBuilder->QueryInterface (IID_IVideoWindow, (void **)&m_pViewWindow);
				if(FAILED(hr))
				return hr;*/

				hr = m_pGraphBuilder->QueryInterface (IID_IMediaEventEx, (void **)&m_pMediaEventEx);
				if(FAILED(hr))
					return hr;

				hr = m_pGraphBuilder->QueryInterface (IID_IMediaSeeking, (void **)&m_pMediaSeeking);
				if(FAILED(hr))
					return hr;
			}

		}
		else
		{
			//free resources
			m_pCaptureGraphBuilder->Release();
			m_pGraphBuilder->Release();
		}

	}
	else
	{
		//free resources
		m_pGraphBuilder->Release();
	}

	return hr; 
} //end InitFilterGrp

//------------------------------------------------------------------
//
// Prototype:	BOOL BuildFilterGrp(CamConfig* ptCamcfg)
//
// Description:	This is used to combine all requested objects in a Filter Graph.
//
// Parameters:	Pointer to CamConfig structure.
//
// Returns:	Boolean "TRUE" if Filter Graph is created successfully  otherwise
//			Boolean "FALSE".
// 
// Notes:
//------------------------------------------------------------------
BOOL CDShowCamModule::BuildFilterGrp(CamConfig* ptCamcfg)
{
	HRESULT hr = S_OK;
	CPropertyBag  PropBag;

	//populate the Camera configeration info to a member stucture
	m_tCamcfg.bIsAudioEnb  = ptCamcfg->bIsAudioEnb;
	m_tCamcfg.bIsCapEnb    = ptCamcfg->bIsCapEnb;
	m_tCamcfg.bIsPrvEnb    = ptCamcfg->bIsPrvEnb;
	m_tCamcfg.bIsStillEnb  = ptCamcfg->bIsStillEnb;
	m_tCamcfg.hwndOwnerWnd = ptCamcfg->hwndOwnerWnd;
	m_tCamcfg.rc.bottom    = ptCamcfg->rc.bottom;
	m_tCamcfg.rc.left      = ptCamcfg->rc.left;
	m_tCamcfg.rc.right     = ptCamcfg->rc.right;
	m_tCamcfg.rc.top       = ptCamcfg->rc.top;
	m_tCamcfg.sCamID       = ptCamcfg->sCamID;


	IPersistPropertyBag* pVideoPropertyBag = NULL;
	IPersistPropertyBag* pAudioPropertyBag = NULL;

	CComVariant varCamName;
	VariantInit(&varCamName);


	if(ptCamcfg->bIsCapEnb)
	{
		hr = CoCreateInstance(CLSID_VideoCapture, NULL, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (void**)&m_pVideoCaptureFilter);
		if(FAILED(hr))
		{
			return FALSE;
		}

		hr = m_pVideoCaptureFilter->QueryInterface(IID_IPersistPropertyBag, (void **)&pVideoPropertyBag);
		if(FAILED(hr))
		{
			return FALSE;
		}

		varCamName = ptCamcfg->sCamID.c_str();
		if(( varCamName.vt == VT_BSTR ) == NULL ) 
		{
			return FALSE;
		}

		PropBag.Write(L"VCapName", &varCamName);   
		pVideoPropertyBag->Load(&PropBag, NULL);
		pVideoPropertyBag->Release();

		// Add Video Capture filter to the graph.
		hr = m_pGraphBuilder->AddFilter(m_pVideoCaptureFilter, L"Video Capture Filter");
		if(FAILED(hr))
		{
			return FALSE;
		}

		////--------------- MPA3 ----------------------
		//Quality of the preview was significantly improved after using this behavior
		//
		if(IsMPA3()){
			ImageRes tRes;
			tRes.nWidth		= 352;
			tRes.nHeight	=-288;	// Minus values returns from the device prop for MPA3/MC67
			
			//Order should be preserved. First for the P pin and then to V pin 
			InitVdoCapFilter(&tRes, P);		
			InitVdoCapFilter(&tRes, V);
			//----------------------
			//m_pVideoCaptureFilter->Release();		
		
		}else{
			RETAILMSG(1, (TEXT("NOT MPA3")));
		}


		//--------------- MPA3 ----------------------
		
		//--------------------------------Adding DMO Encoder Filter to the graph--------------------------------------
		hr = CoCreateInstance(CLSID_DMOWrapperFilter, NULL, CLSCTX_INPROC_SERVER, IID_IBaseFilter, reinterpret_cast<void**>(&m_pVideoEncoder));
		if(FAILED(hr))
		{
			return FALSE;
		}

		hr = m_pVideoEncoder->QueryInterface(IID_IDMOWrapperFilter, reinterpret_cast<void**>(&m_pVideoWrapperFilter));
		if(FAILED(hr))
		{
			return FALSE;
		}

		hr = m_pVideoWrapperFilter->Init(CLSID_CWMV9EncMediaObject, DMOCATEGORY_VIDEO_ENCODER);
		if(FAILED(hr))
		{
			return FALSE;
		}
		m_pVideoWrapperFilter->Release();

		hr = m_pGraphBuilder->AddFilter(m_pVideoEncoder, L"WMV9 DMO Encoder");
		if(FAILED(hr))
		{
			return FALSE;
		}

		//hr = m_pCaptureGraphBuilder->SetOutputFileName(&MEDIASUBTYPE_Asf, DFT_VDO_FN, &m_pMux, &m_pSink);
		hr = m_pCaptureGraphBuilder->SetOutputFileName(&MEDIASUBTYPE_Asf, DFT_VDO_FN, &m_pMux, &m_pSink);
		if((FAILED(hr)))
		{
			return FALSE;
		}

		hr = m_pCaptureGraphBuilder->RenderStream(&PIN_CATEGORY_CAPTURE, &MEDIATYPE_Video, m_pVideoCaptureFilter, m_pVideoEncoder, m_pMux);
		if((FAILED(hr)))
		{
			return FALSE;
		}
		//-----------------------------------------------------------------------------------------------------------
	}

	if(ptCamcfg->bIsAudioEnb)
	{
		hr = CoCreateInstance(CLSID_AudioCapture, NULL, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (void**)&m_pAudioCaptureFilter);
		if(FAILED(hr))
		{
			return FALSE;
		}

		hr = m_pAudioCaptureFilter->QueryInterface(IID_IPersistPropertyBag, (void **) &pAudioPropertyBag);
		if(SUCCEEDED(hr))
		{
			// use the default audio driver. If a particular driver was required
			// it is selected the same as the camera driver.
			hr = pAudioPropertyBag->Load(NULL, NULL);
			if(SUCCEEDED(hr))
			{
				// add the filter to the graph
				hr = m_pGraphBuilder->AddFilter(m_pAudioCaptureFilter, L"Audio Capture Filter");
				if(SUCCEEDED(hr))
				{
					hr = m_pCaptureGraphBuilder->RenderStream(&PIN_CATEGORY_CAPTURE, &MEDIATYPE_Audio, m_pAudioCaptureFilter, NULL, m_pMux);
				}
				else
				{
					return FALSE;
				}

			}
		}
	}

	//Setup still capture Filters
	if(ptCamcfg->bIsCapEnb && ptCamcfg->bIsStillEnb)
	{
		hr = SetupStill();
#ifdef DEBUG_MODE
		if(SUCCEEDED(hr))
		{
			OutputDebugString(L"\n##### Still Setup OK ######\n");
		}
		else
		{
			OutputDebugString(L"\n##### Still Setup Error ######\n");
		}
#endif
	}

	if(ptCamcfg->bIsCapEnb && ptCamcfg->bIsPrvEnb)
	{
		hr = SetupPreview(ptCamcfg->hwndOwnerWnd, ptCamcfg->rc);
		//hr = SetupPreview(m_tCamcfg.hwndOwnerWnd, m_tCamcfg.rc);
#ifdef DEBUG_MODE
		if(SUCCEEDED(hr))
		{
			OutputDebugString(L"\n##### Preview OK ######\n");
		}
		else
		{
			OutputDebugString(L"\n##### Preview Error ######\n");
		}
#endif

		hr = SetupFlip();
#ifdef DEBUG_MODE
		if(SUCCEEDED(hr))
		{
			OutputDebugString(L"\n##### Flip Setup OK ######\n");
			//Set_Flip(true);
		}
		else
		{
			OutputDebugString(L"\n##### Flip Setup Error ######\n");
		}
#endif

	}
	m_bInited = true;
	return TRUE;
}// end BuildFilterGrp

//--------------- MPA3 ----------------------
HRESULT CDShowCamModule::InitVdoCapFilter(ImageRes* ptRes, PinType ePType)
{
OutputDebugString(L"\nHRESULT CDShowCamModule::InitVdoCapFilter(ImageRes* ptRes, PinType ePType)\n");
	HRESULT hr;	
	GUID pType;
	m_pStrConf = NULL;
	
	switch(ePType)
	{
		case V:
			pType = PIN_CATEGORY_CAPTURE;
			break;
		case S:
			pType = PIN_CATEGORY_STILL;
			break;
		case P:
			pType = PIN_CATEGORY_PREVIEW;
			break;
	}
		
	//PIN type, Any media type, Pointer to the capture filter.
	hr = m_pCaptureGraphBuilder->FindInterface(&pType, &MEDIATYPE_Video, m_pVideoCaptureFilter,IID_IAMStreamConfig, (void**)&m_pStrConf);
	//hr = m_pCaptureGraphBuilder->FindInterface(&pType, 0, m_pMux,\
													IID_IAMStreamConfig, (void**)&m_pStrConf);
	
	if(SUCCEEDED(hr))
	{
		VIDEO_STREAM_CONFIG_CAPS scc;
		AM_MEDIA_TYPE *pmtConfig;
		INT iResIdx=0;

	    INT nCount = 0, nSize = 0;

		hr = m_pStrConf->GetNumberOfCapabilities(&nCount, &nSize);
	    if(FAILED(hr))
		{
            return hr;
		}
		
		RETAILMSG(1, (_T("ptRes->nWidth = %d, ptRes->nHeight = %d \n\r"), ptRes->nWidth, ptRes->nHeight));
		
		for(int i=iResIdx; i<nCount;i++)
		{
			hr = m_pStrConf->GetStreamCaps(i, &pmtConfig, (BYTE*)&scc);
	
			if (SUCCEEDED(hr))
			{
				RETAILMSG(1, (_T("scc.MaxOutputSize.cx = %d, scc.MaxOutputSize.cy = %d \n\r"), scc.MaxOutputSize.cx, scc.MaxOutputSize.cy));

				if((abs(scc.MaxOutputSize.cx)==abs(ptRes->nWidth)) && (abs(scc.MaxOutputSize.cy)==abs(ptRes->nHeight)))
				{
					hr = m_pStrConf->SetFormat(pmtConfig);
					if((FAILED(hr)))
					{
						return FALSE;
					}
					break;
				}
			}
			// Delete the media type when you are done.
			DeleteMediaType(pmtConfig);
		}

		//Clean resoures
		m_pStrConf->Release();
	}
	
return hr;
}
//--------------- MPA3 ----------------------


//------------------------------------------------------------------
//
// Prototype:	BOOL FreeFilterGrp(CamConfig* ptCamcfg)
//
// Description:	This is used to combine all requested objects in a Filter Graph.
//
// Parameters:	Pointer to CamConfig structure.
//
// Returns:	Boolean "TRUE" if Filter Graph is created successfully  otherwise
//			Boolean "FALSE".
// 
// Notes:
//------------------------------------------------------------------
BOOL CDShowCamModule::FreeFilterGrp()
{
	HRESULT hr = S_OK;

	if(SUCCEEDED(StopGrp()))
	{
		if(SUCCEEDED(ClosePreview()))
		{
			hr = S_OK;
		}
	}

	if(m_pGraphBuilder != NULL)
	{
		// Enumerate the filters in the graph.
		IEnumFilters *pEnum = NULL;
		hr = m_pGraphBuilder->EnumFilters(&pEnum);
		if (SUCCEEDED(hr))
		{
			IBaseFilter *pFilter = NULL;
			while (S_OK == pEnum->Next(1, &pFilter, NULL))
			{
				// Remove the filter.
				hr = m_pGraphBuilder->RemoveFilter(pFilter);
				// Reset the enumerator.
				pEnum->Reset();
				hr = pFilter->Release();
			}
			pEnum->Release();
		}
	}

	if((m_pMediaSeeking))//x
	{
		hr = (m_pMediaSeeking)->Release();
		(m_pMediaSeeking) = NULL;
	}

	//notifying all the windows the reamining messages before exiting
	if(m_pMediaEventEx != NULL)
	{
		m_pMediaEventEx->SetNotifyWindow(NULL,0,0);
	}
	if((m_pMediaEventEx))//x
	{
		hr = (m_pMediaEventEx)->Release();
		(m_pMediaEventEx) = NULL;
	}

	//if((m_pMediaEvent))
	//{
	//	hr = (m_pMediaEvent)->Release();
	//	(m_pMediaEvent) = NULL;
	//}

	//if((m_pViewWindow))//x
	//{
	//	hr = (m_pViewWindow)->Release();
	//	(m_pViewWindow) = NULL;
	//}

	if((m_pMediaControl))
	{
		hr = (m_pMediaControl)->Release();
		(m_pMediaControl) = NULL;
	}

	if((m_pCaptureGraphBuilder))
	{
		hr = (m_pCaptureGraphBuilder)->Release();
		(m_pCaptureGraphBuilder) = NULL;
	}

	if((m_pVideoWrapperFilter))
	{
		hr = (m_pVideoWrapperFilter)->Release();
		(m_pVideoWrapperFilter) = NULL;
	}

	if((m_pStillSink))
	{
		hr = (m_pStillSink)->Release();
		(m_pStillSink) = NULL;
	}

	if((m_pVideoEncoder))
	{
		hr = (m_pVideoEncoder)->Release();
		(m_pVideoEncoder) = NULL;
	}

	//It is very important to releae m_pCamCtrl, otherwise camera and its related bus may be unavailable
	if((m_pCamCtrl))
	{
		hr = (m_pCamCtrl)->Release();
		(m_pCamCtrl) = NULL;
	}

	if((m_pImgCtrl))
	{
		hr = (m_pImgCtrl)->Release();
		(m_pImgCtrl) = NULL;
	}

	if((m_pVideoRenderer))//x
	{
		hr = (m_pVideoRenderer)->Release();
		(m_pVideoRenderer) = NULL;
	}
	//--------------- MPA3 ----------------------
	// Colour filter
	if((m_pColorConverter))//x
	{
		hr = (m_pColorConverter)->Release();
		(m_pColorConverter) = NULL;
	}
	//--------------- MPA3 ----------------------
	
	if((m_pVideoCaptureFilter))//x
	{
		hr = (m_pVideoCaptureFilter)->Release();
		(m_pVideoCaptureFilter) = NULL;
	}

	if((m_pGraphBuilder))
	{
		hr = (m_pGraphBuilder)->Release();
		(m_pGraphBuilder) = NULL;
	}

	if((m_pPrvPin))
	{
		hr = (m_pPrvPin)->Release();
		(m_pPrvPin) = NULL;
	}

	if((m_pVideoCtrl))
	{
		hr = (m_pVideoCtrl)->Release();
		(m_pVideoCtrl) = NULL;
	}

	if((m_pSink))//x
	{
		hr = (m_pSink)->Release();
		(m_pSink) = NULL;
	}

	if((m_pMux))
	{
		hr = (m_pMux)->Release();
		(m_pMux) = NULL;
	}

	if((m_pStrConf))
	{
		hr = (m_pStrConf)->Release();
		(m_pStrConf) = NULL;
	}

	if((m_pAudioCaptureFilter))
	{
		hr = (m_pAudioCaptureFilter)->Release();
		(m_pAudioCaptureFilter) = NULL;
	}

	m_bInited = false;
	return TRUE;
}// end FreeFilterGrp

//------------------------------------------------------------------
//
// Prototype:	BOOL ReBuildGrp()
//
// Description:	This is used to re connect required components of the Filter Graph.
//
// Parameters:	None
//
// Returns:	Boolean "TRUE" if Re build is successful otherwise
//			Boolean "FALSE".
// 
// Notes:
//------------------------------------------------------------------
BOOL CDShowCamModule::ReBuildGrp()
{
	HRESULT hr;
	//IBaseFilter* pVideoRenderer = NULL;

	//Remove Video capture Filter
	m_pGraphBuilder->RemoveFilter(m_pVideoCaptureFilter);
	//--------------- MPA3 ----------------------
	if(IsMPA3())
	{
		m_pGraphBuilder->RemoveFilter(m_pColorConverter);
	}	
	//--------------- ---- ----------------------
	
	m_pGraphBuilder->RemoveFilter(m_pVideoRenderer);

	//m_pGraphBuilder->RemoveFilter(m_pVideoEncoder);
	//m_pGraphBuilder->RemoveFilter(m_pAudioCaptureFilter);


	// Add Video Capture filter to the graph.
	hr = m_pGraphBuilder->AddFilter(m_pVideoCaptureFilter, L"Video Capture Filter");
	if(FAILED(hr))
	{
		return FALSE;
	}
	//
	// Add color converter filter to the graph. MPA3
	//

	if(IsMPA3())
	{
		hr = m_pGraphBuilder->AddFilter(m_pColorConverter, L"Texas Instruments Color Converter Filter");
		if(FAILED(hr))
		{
			RETAILMSG(1, (TEXT("Adding Texas Instruments Color Converter Filter failed")));
			m_pColorConverter=NULL; //NULL should be passed for RenderStream
			//return FALSE
		}
	}

	// Add Video Renderer filter to the graph.
	hr = m_pGraphBuilder->AddFilter(m_pVideoRenderer, L"My Video Renderer");
	if(FAILED(hr))
	{
		return FALSE;
	}
	//--------------- MPA3 ----------------------
	//Added m_pColorConverter TI filter, Should pass NULL if addFilter fails.
	//
	hr = m_pCaptureGraphBuilder->RenderStream( &PIN_CATEGORY_PREVIEW, &MEDIATYPE_Video, m_pVideoCaptureFilter, m_pColorConverter, m_pVideoRenderer);
	//-------------------- ----------------------
	
	//Remove Filters from the graph
	//hr = SetupPreview(m_tCamcfg.hwndOwnerWnd, m_tCamcfg.rc);
	if((FAILED(hr)))
	{
		return FALSE;
	}

	/*
	hr = m_pGraphBuilder->AddFilter(m_pVideoEncoder, L"WMV9 DMO Encoder");
	if(FAILED(hr))
	{
	return FALSE;
	}
	//hr = m_pGraphBuilder->AddFilter(m_pVideoRenderer, L"My Video Renderer");
	//if(FAILED(hr))
	//{
	//	return FALSE;
	//}
	hr = m_pCaptureGraphBuilder->SetOutputFileName(&MEDIASUBTYPE_Asf, DFT_VDO_FN, &m_pMux, &m_pSink);
	if((FAILED(hr)))
	{
	return FALSE;
	}

	//hr = m_pSink->SetFileName((LPCOLESTR)L"\\My Documents\\My Pictures\\waTemp.wmv", 0);

	#ifdef DEBUG_MODE
	//	if(FAILED(hr))
	//	{
	//		OutputDebugString(L"!!! Error Video File Name Set !!!\n");
	//	}
	#endif

	hr = m_pCaptureGraphBuilder->RenderStream(&PIN_CATEGORY_CAPTURE, &MEDIATYPE_Video, m_pVideoCaptureFilter, m_pVideoEncoder, m_pMux);
	if((FAILED(hr)))
	{
	return FALSE;
	}
	// Add Audio Capture filter to the graph.
	hr = m_pGraphBuilder->AddFilter(m_pAudioCaptureFilter, L"Audio Capture Filter");
	if(FAILED(hr))
	{
	return FALSE;
	}

	hr = m_pCaptureGraphBuilder->RenderStream(&PIN_CATEGORY_CAPTURE, &MEDIATYPE_Audio, m_pAudioCaptureFilter, NULL, m_pMux);
	if((FAILED(hr)))
	{
	return FALSE;
	}

	// Add Still Sink Filter.
	//hr = m_pGraphBuilder->AddFilter(m_pStillSink, L"StillSink");
	//if(FAILED(hr))
	//{
	//	return FALSE;
	//}

	hr = m_pCaptureGraphBuilder->RenderStream( &PIN_CATEGORY_STILL, &MEDIATYPE_Video, m_pVideoCaptureFilter, NULL, m_pStillSink);
	if(SUCCEEDED(hr))
	OutputDebugString(L"\n!!!! Render STILL PIN OK!!!!\n");
	else
	OutputDebugString(L"\n!!!! Render STILL PIN Fail!!!!\n");

	*/
	m_bInited = true;
	return TRUE;
}
//------------------------------------------------------------------
//
// Prototype:	BOOL RunGrp()
//
// Description:	This method switches the entire filter graph into a running state. 
//
// Parameters:	none
//
// Returns:	boolean "TRUE" if Filter Graph has started running successfully otherwise
//			boolean "FALSE".
// 
// Notes:
//------------------------------------------------------------------
BOOL CDShowCamModule::RunGrp()
{	
	if(!m_bInited)
	{
		return false;
	}

	//HRESULT hr = m_pCaptureGraphBuilder->ControlStream(&PIN_CATEGORY_CAPTURE, NULL, NULL, 0, 0, 0, 0);
	HRESULT hr = m_pCaptureGraphBuilder->ControlStream(&PIN_CATEGORY_CAPTURE, &MEDIATYPE_Video, m_pVideoCaptureFilter, 0, 0, 0, 0);
	//hr = m_pCaptureGraphBuilder->ControlStream(&PIN_CATEGORY_CAPTURE, &MEDIATYPE_Audio, m_pAudioCaptureFilter, 0, 0, 0, 0);

	if(!(SUCCEEDED(hr)))
	{
		return false;
	}

	//Run the Filter Graph
	hr = m_pMediaControl->Run();

	if(SUCCEEDED(hr))
		return TRUE;
	else 
		return FALSE;

} //end RunGrp
BOOL CDShowCamModule::ResizePreview(int width, int height)
{
	HRESULT hr = S_FALSE;
    IVideoWindow* pVideoWindow = NULL;

	if (m_pVideoRenderer)
	{
		if(SUCCEEDED(hr = m_pVideoRenderer->QueryInterface( IID_IVideoWindow, (void **)&pVideoWindow)))
		{
			if (SUCCEEDED(hr = pVideoWindow->SetWindowPosition(0, 0, width, height)))
			{
				pVideoWindow->Release();
				return TRUE;
			}
			pVideoWindow->Release();
		}
	}
	return FALSE;
}

//------------------------------------------------------------------
//
// Prototype:	BOOL PauseGrp()
//
// Description:	This method pauses all the filters in the filter graph. 
//
// Parameters:	none
//
// Returns:	boolean "TRUE" if Filter Graph has started running successfully otherwise
//			boolean "FALSE".
// 
// Notes:
//------------------------------------------------------------------
BOOL CDShowCamModule::PauseGrp()
{
	if(!m_bInited)
	{
		return false;
	}

	if(m_pMediaControl == NULL)
	{
		return FALSE;
	}

	//Pause the Filter Graph
	HRESULT hr = m_pMediaControl->Pause();

	if(SUCCEEDED(hr))
		return TRUE;
	else 
		return FALSE;

} //end PauseGrp

//------------------------------------------------------------------
//
// Prototype:	BOOL StopGrp()
//
// Description:	This method switches all filters in the filter graph to a stopped state. 
//
// Parameters:	none
//
// Returns:	boolean "TRUE" if Filter Graph has started running successfully otherwise
//			boolean "FALSE".
// 
// Notes:
//------------------------------------------------------------------
BOOL CDShowCamModule::StopGrp()
{
	if(!m_bInited)
	{
		return false;
	}

	if(m_pMediaControl == NULL)
	{
		return FALSE;
	}

	//Stop the Filter Graph
	HRESULT hr = m_pMediaControl->Stop();

	if(SUCCEEDED(hr))
		return TRUE;
	else 
		return FALSE;

} //end StopGrp

//------------------------------------------------------------------
//
// Prototype:	HRESULT Get_PropRng(PropType ePType, HANDLE hPropTbl)
//
// Description:	This is used to retrieve limit values of either Image, Camera or Custom property.
//
// Parameters:	ePType indicates the enumurated property type to query, 
//			    bPropTbl is a pointer to the corresponding property structure.
//
// Returns:	S_OK if Property values are retrieved successfully.
// 
// Notes:
//------------------------------------------------------------------

HRESULT CDShowCamModule::Get_PropRng(PropType ePType, HANDLE hPropTbl)
{
	HRESULT hr;

	m_pImgCtrl = NULL;
	m_pCamCtrl = NULL;		

	if(!m_bInited)
	{
		return S_FALSE;
	}

	if(ePType==IMG)
	{
		ImgPropTbl* tbl = (ImgPropTbl* )hPropTbl;

		hr = m_pVideoCaptureFilter->QueryInterface(IID_IAMVideoProcAmp, (void **)&m_pImgCtrl);
		if(SUCCEEDED(hr))
		{
			hr = m_pImgCtrl->GetRange(tbl->vProp, &tbl->plMin, &tbl->plMax, &tbl->plDelta, &tbl->plDef, &tbl->plFlag);
			if(SUCCEEDED(hr))
			{
				hr = m_pImgCtrl->Get(tbl->vProp, &tbl->plVal, &tbl->plFlag);
				tbl->hr = hr;
				if(SUCCEEDED(hr))
					OutputDebugString(L"\n----Property Value Get OK-----\n");
				else
					OutputDebugString(L"\n----Property Value Get FAIL-----\n");
			}

		}
		//free resources
		m_pImgCtrl->Release();
	}
	else if(ePType==CAM)
	{
		CamPropTbl* tbl = (CamPropTbl* )hPropTbl;

		hr = m_pVideoCaptureFilter->QueryInterface(IID_IAMCameraControl, (void **)&m_pCamCtrl);
		if(SUCCEEDED(hr))
		{
			hr = m_pCamCtrl->GetRange(tbl->p, &tbl->plMin, &tbl->plMax, &tbl->plDelta, &tbl->plDef, &tbl->plFlag);
			if(SUCCEEDED(hr))
			{
				hr = m_pCamCtrl->Get(tbl->p, &tbl->plVal, &tbl->plFlag);
				tbl->hr = hr;
			}
		}
		//free resources
		m_pCamCtrl->Release();
	}
	else
	{
		CusPropTbl* tbl = (CusPropTbl* )hPropTbl;

		hr = Cus_GetRange(tbl->p, &tbl->plMin, &tbl->plMax, &tbl->plDelta, &tbl->plDef);
		if(SUCCEEDED(hr))
		{
			hr = Cus_Get(tbl->p, &tbl->plVal);
			tbl->hr = hr;
		}
	}

	return hr;
} //end Get_PropRng

//------------------------------------------------------------------
//
// Prototype:	HRESULT Get_PropVal(PropType ePType, HANDLE hPropTbl)
//
// Description:	This is used to retrieve the value of either Image, Camera or Custom property.
//
// Parameters:	ePType indicates the enumurated property type to query, 
//			    bPropTbl is a pointer to the corresponding property structure.
//
// Returns:	S_OK if Property value is retrieved successfully.
// 
// Notes:
//------------------------------------------------------------------
HRESULT CDShowCamModule::Get_PropVal(PropType ePType, HANDLE hPropTbl)
{
	HRESULT hr;

	m_pImgCtrl = NULL;
	m_pCamCtrl = NULL;

	if(!m_bInited)
	{
		return S_FALSE;
	}

	if(ePType==IMG)
	{
		ImgPropTbl* tbl = (ImgPropTbl* )hPropTbl;

		hr = m_pVideoCaptureFilter->QueryInterface(IID_IAMVideoProcAmp, (void **)&m_pImgCtrl);
		if(SUCCEEDED(hr))
		{
			hr = m_pImgCtrl->Get(tbl->vProp, &tbl->plVal, &tbl->plFlag);	
			tbl->hr=hr;
		}
		//free resources
		m_pImgCtrl->Release();
	}
	else if(ePType==CAM)
	{
		CamPropTbl* tbl = (CamPropTbl* )hPropTbl;

		hr = m_pVideoCaptureFilter->QueryInterface(IID_IAMCameraControl, (void **)&m_pCamCtrl);
		if(SUCCEEDED(hr))
		{
			hr = m_pCamCtrl->Get(tbl->p, &tbl->plVal, &tbl->plFlag);	
			tbl->hr=hr;
		}
		//free resources
		m_pCamCtrl->Release();
	}
	else
	{
		CusPropTbl* tbl = (CusPropTbl* )hPropTbl;

		hr = Cus_Get(tbl->p, &tbl->plVal);
		tbl->hr=hr;
	}

	return hr;
} //end Get_PropVal

//------------------------------------------------------------------
//
// Prototype:	HRESULT Set_PropVal(PropType ePType, HANDLE hPropTbl)
//
// Description:	This is used to set the value of either Image, Camera or Custom property.
//
// Parameters:	ePType indicates the enumurated property type to query, 
//			    bPropTbl is a pointer to the corresponding property structure.
//
// Returns:	S_OK if Property value is successfully set.
// 
// Notes:
//------------------------------------------------------------------
HRESULT CDShowCamModule::Set_PropVal(PropType ePType, HANDLE hPropTbl)
{
	HRESULT hr;

	m_pImgCtrl = NULL;
	m_pCamCtrl = NULL;

	if(!m_bInited)
	{
		return S_FALSE;
	}

	if(ePType==IMG)
	{
		ImgPropTbl* tbl = (ImgPropTbl* )hPropTbl;

		hr = m_pVideoCaptureFilter->QueryInterface(IID_IAMVideoProcAmp, (void **)&m_pImgCtrl);
		if(SUCCEEDED(hr))
		{
			hr = m_pImgCtrl->Set(tbl->vProp, tbl->plVal, tbl->plFlag);	
			tbl->hr=hr;
		}
		//free resources
		m_pImgCtrl->Release();
	}
	else if(ePType==CAM)
	{
		CamPropTbl* tbl = (CamPropTbl* )hPropTbl;

		hr = m_pVideoCaptureFilter->QueryInterface(IID_IAMCameraControl, (void **)&m_pCamCtrl);
		if(SUCCEEDED(hr))
		{
			hr = m_pCamCtrl->Set(tbl->p, tbl->plVal, tbl->plFlag);	
			tbl->hr=hr;
		}
		//free resources
		m_pCamCtrl->Release();
	}
	else
	{
		CusPropTbl* tbl = (CusPropTbl* )hPropTbl;

		hr = Cus_Set(tbl->p, tbl->plVal);
		tbl->hr=hr;
	}


	return hr;
} //end Set_PropVal

//------------------------------------------------------------------
//
// Prototype:	HRESULT CaptureStill()
//
// Description:	This is used to trigger capture of a still image.
//				Here the still image name is auto generated and it will be
//				saved in "\My Documnets\My Pictures" folder
//
// Parameters:	none
//
// Returns:	S_OK if Still image is captured successfully.
// 
// Notes:
//------------------------------------------------------------------
HRESULT CDShowCamModule::CaptureStill(WCHAR *filename)
{
	WCHAR day[DAY_LENGTH], time[TIME_LENGTH], FName[TXT_LENGTH], DName[TXT_LENGTH];
	wstring StillFN;
	SYSTEMTIME st;

	IPin* pStillPin = NULL;
	IAMVideoControl* pVideoControl = NULL;
	IFileSinkFilter* pStillFileSink = NULL;
	HRESULT hr = S_OK;
	//DWORD dTickCount;
	//TCHAR waText[1024];

	if(!m_bInited)
	{
		return S_FALSE;
	}

	GetLocalTime(&st);

	wsprintf(day, L"%.2d%.2d%.2d", st.wMonth, st.wDay, st.wYear%100);
	wsprintf(time, L"%.2d%.2d%.2d", st.wHour, st.wMinute, st.wSecond);
	wsprintf(DName, L"\\My Documents\\My Pictures\\MMAXGallery");
	//wsprintf(FName, L"%s\\%s%s%s", DName, day, time, L".jpg");
	wsprintf(FName, L"%s\\%s", DName, filename);

	StillFN = FName;

	if(CreateDirectory(DName, NULL))
	{
#ifdef DEBUG_MODE
		OutputDebugString(L"\n!!!! Error Create Directory !!!!\n");
#endif
	}

	//Set File Name
	hr = m_pStillSink->QueryInterface(IID_IFileSinkFilter, (void **)&pStillFileSink);
	if(SUCCEEDED(hr))
	{
		hr = pStillFileSink->SetFileName((LPCOLESTR)StillFN.c_str(), NULL );
#ifdef DEBUG_MODE
		if(SUCCEEDED(hr))
		{
			OutputDebugString(StillFN.c_str());
		}
#endif
	}

	//Find the still image output pin, retrieve the IAMVideoControl interface, and
#ifdef DEBUG_MODE
	dTickCount=GetTickCount();
	wsprintf(waText, L"\n **** Time Stamp[%d]: Before Calling 'FindPin'for STILL Pin on VideoCaptureFilter\n", dTickCount);
	OutputDebugString(waText);
#endif
	hr = m_pCaptureGraphBuilder->FindPin((IUnknown*)m_pVideoCaptureFilter, PINDIR_OUTPUT, &PIN_CATEGORY_STILL, &MEDIATYPE_Video, FALSE, 0, &pStillPin);
#ifdef DEBUG_MODE
	dTickCount=GetTickCount();
	wsprintf(waText, L"\n **** Time Stamp[%d]: After Calling 'FindPin'for STILL Pin on VideoCaptureFilter\n", dTickCount);
	OutputDebugString(waText);
#endif
	if(SUCCEEDED(hr))
	{
		// Once you have the still image pin, you query it for the
		// IAMVideoControl interface which exposes the SetMode function for 
		// triggering the image capture.
#ifdef DEBUG_MODE
		dTickCount=GetTickCount();
		wsprintf(waText, L"\n **** Time Stamp[%d]: Before Querying IAMVideoControl Interface on VideoCaptureFilter\n", dTickCount);
		OutputDebugString(waText);
#endif
		if(SUCCEEDED(hr = m_pVideoCaptureFilter->QueryInterface( IID_IAMVideoControl, (void **)&pVideoControl )))
		{
#ifdef DEBUG_MODE
			dTickCount=GetTickCount();
			wsprintf(waText, L"\n **** Time Stamp[%d]: After Querying IAMVideoControl Interface on VideoCaptureFilter\n", dTickCount);
			OutputDebugString(waText);
#endif
			// Now that trigger the still image capture.
#ifdef DEBUG_MODE
			dTickCount=GetTickCount();
			wsprintf(waText, L"\n **** Time Stamp[%d]: Before Calling SetMode on IAMVideoControl Interface\n", dTickCount);
			OutputDebugString(waText);
#endif
			hr = pVideoControl->SetMode( pStillPin, VideoControlFlag_Trigger );
#ifdef DEBUG_MODE
			dTickCount=GetTickCount();
			wsprintf(waText, L"\n **** Time Stamp[%d]: After Calling SetMode on IAMVideoControl Interface\n", dTickCount);
			OutputDebugString(waText);
#endif
		}
	}

	//If trigger succeeded, Wait for the EC_CAP_FILE_COMPLETED event.
	if(SUCCEEDED(hr))
	{
		LONG lEventCode = 0, lParam1 = 0, lParam2 = 0;
		do{
			// If no events are received within 60 seconds, then we’ll assume 
			// we have an error. Adjust this value as you see fit.
			hr = m_pMediaEventEx->GetEvent( &lEventCode, &lParam1,&lParam2, 80*1000 );
			if(SUCCEEDED(hr))
			{
				// If we recieve an event, free the event parameters.
				// we can do this immediately because we're not referencing 
				// lparam1 or lparam2.
				if(SUCCEEDED(hr = m_pMediaEventEx->FreeEventParams( lEventCode, lParam1, lParam2 )))
				{
					// If the event was received, then we successfully 
					// captured the still image and can quit looking.
					if(lEventCode == EC_CAP_FILE_COMPLETED)
					{
#ifdef DEBUG_MODE
						dTickCount=GetTickCount();
						wsprintf(waText, L"\n **** Time Stamp[%d]: receiving EC_CAP_FILE_COMPLETED event to notify STILL image capture is completed\n", dTickCount);
						OutputDebugString(waText);
#endif
						break;
					}

				}
			}
			// If the still image isn't captured within 60 seconds
			// of the trigger (or possibly longer if other events come in), 
			// then we’ll assume it’s an error. Continue processing events 
			// until we detect the error.
		}while(SUCCEEDED(hr));
	}

	//clean up
	if(pStillFileSink)
	{
		pStillFileSink->Release();
		pStillFileSink = NULL;
	}

	if(pVideoControl)
	{
		pVideoControl->Release();
		pVideoControl = NULL;
	}

	if(pStillPin)
	{
		pStillPin->Release();
		pStillPin = NULL;
	}

	return hr;
} //end CaptureStill()

//------------------------------------------------------------------
//
// Prototype:	HRESULT CaptureStill(wstring wsSFName)
//
// Description:	This is used to trigger capture of a still image.
//				Here the still image name is passed as a parameter and it will be
//				saved in "\My Documnets\My Pictures" folder
//
// Parameters:	wsSFName is the name given for the still Image
//
// Returns:	S_OK if Still image is captured successfully.
// 
// Notes:
//------------------------------------------------------------------
HRESULT CDShowCamModule::CaptureStill(wstring wsSFName)
{
	//WCHAR day[DAY_LENGTH], FName[TXT_LENGTH], DName[TXT_LENGTH];
	WCHAR DName[TXT_LENGTH];
	wstring StillFN = L"\\tempimg.jpg";
	SYSTEMTIME st;

	IPin* pStillPin = NULL;
	IAMVideoControl* pVideoControl = NULL;
	IFileSinkFilter* pStillFileSink = NULL;
	HRESULT hr = S_OK;

	if(!m_bInited)
	{
		return S_FALSE;
	}

	GetLocalTime(&st);

	//wsprintf(day, L"%.2d%.2d%.2d", st.wMonth, st.wDay, st.wYear%100);
	//wsprintf(FName, L"\\My Documents\\My Pictures\\%s\\%s%s", day, wsSFName.c_str(), L".jpg");
	//wsprintf(DName, L"\\My Documents\\My Pictures\\%s", day);

	//StillFN = FName;

//	if(CreateDirectory(DName, NULL))
//	{
//#ifdef DEBUG_MODE
//		OutputDebugString(L"\n!!!! Error Create Directory !!!!\n");
//#endif
//	}

	//Set File Name
	if(SUCCEEDED(hr = m_pStillSink->QueryInterface( IID_IFileSinkFilter, (void **)&pStillFileSink )))
	{
		hr = pStillFileSink->SetFileName((LPCOLESTR)StillFN.c_str(), NULL );
#ifdef DEBUG_MODE
		if(SUCCEEDED(hr))
		{
			OutputDebugString(StillFN.c_str());
		}
#endif
	}

	//Find the still image output pin, retrieve the IAMVideoControl interface, and
	if(SUCCEEDED(hr = m_pCaptureGraphBuilder->FindPin((IUnknown*)m_pVideoCaptureFilter, PINDIR_OUTPUT, &PIN_CATEGORY_STILL, &MEDIATYPE_Video, FALSE, 0, &pStillPin )))
	{
		// Once you have the still image pin, you query it for the
		// IAMVideoControl interface which exposes the SetMode function for 
		// triggering the image capture.
		if(SUCCEEDED(hr = m_pVideoCaptureFilter->QueryInterface( IID_IAMVideoControl, (void **)&pVideoControl )))
		{
			// Now that trigger the still image capture.
			hr = pVideoControl->SetMode( pStillPin, VideoControlFlag_Trigger );
		}
	}

	//If trigger succeeded, Wait for the EC_CAP_FILE_COMPLETED event.
	if(SUCCEEDED(hr))
	{
		LONG lEventCode = 0, lParam1 = 0, lParam2 = 0;
		do{
			// If no events are received within 60 seconds, then we’ll assume 
			// we have an error. Adjust this value as you see fit.
			hr = m_pMediaEventEx->GetEvent( &lEventCode, &lParam1,&lParam2, 80*1000 );
			if(SUCCEEDED(hr))
			{
				// If we recieve an event, free the event parameters.
				// we can do this immediately because we're not referencing 
				// lparam1 or lparam2.
				if(SUCCEEDED(hr = m_pMediaEventEx->FreeEventParams( lEventCode, lParam1, lParam2 )))
				{
					// If the event was received, then we successfully 
					// captured the still image and can quit looking.
					if(lEventCode == EC_CAP_FILE_COMPLETED)
					{
						DeleteFile(wsSFName.c_str());
						MoveFile(StillFN.c_str(), wsSFName.c_str());
						break;
					}
				}
			}
			// If the still image isn't captured within 60 seconds
			// of the trigger (or possibly longer if other events come in), 
			// then we’ll assume it’s an error. Continue processing events 
			// until we detect the error.
		}while(SUCCEEDED(hr));
	}

	//clean up
	if(pStillFileSink)
	{
		pStillFileSink->Release();
		pStillFileSink = NULL;
	}

	if(pVideoControl)
	{
		pVideoControl->Release();
		pVideoControl = NULL;
	}

	if(pStillPin)
	{
		pStillPin->Release();
		pStillPin = NULL;
	}

	return hr;
} //end CaptureStill()

//------------------------------------------------------------------
//
// Prototype:	HRESULT Set_VdoFileName(wstring wsVFName)
//
// Description:	This is used to set a file name for the current video stream capturing.
//
// Parameters:	wsVFName Unicode string
//
// Returns:	S_OK if Video file name is set successfully.
// 
// Notes:
//------------------------------------------------------------------
HRESULT CDShowCamModule::Set_VdoFileName(wstring wsVFName)
{
	m_wsVFName = wsVFName;
	HRESULT hr = m_pSink->SetFileName((LPCOLESTR)wsVFName.c_str(), 0);

	if(!m_bInited)
	{
		return S_FALSE;
	}

#ifdef DEBUG_MODE
	if(FAILED(hr))
	{
		OutputDebugString(L"!!! Error Video File Name Set !!!\n");
	}
#endif

	return hr;
} //end Set_VdoFileName

//------------------------------------------------------------------
//
// Prototype:	HRESULT StartCapture()
//
// Description:	This is used to trigger video stream capturing.
//
// Parameters:	none
//
// Returns:	S_OK if Stream capturing started successfully.
// 
// Notes:
//------------------------------------------------------------------
HRESULT CDShowCamModule::StartCapture()
{
	HRESULT hr;
	REFERENCE_TIME rtStart = START_TIME, rtStop = STOP_TIME;
	WORD StartCookie = START_COOKIE, StopCookie = STOP_COOKIE;
	TCHAR waTemp[TXT_LENGTH];
	TCHAR waTempFile[TXT_LENGTH];

	if(!m_bInited)
	{
		return S_FALSE;
	}

	//m_iCheck = m_CtrlAudio.GetCheck();
	OutputDebugString(m_wsVFName.c_str());
	wcscpy(waTempFile, m_wsVFName.c_str());
	if(DeleteFile(m_wsVFName.c_str()))
	{
		OutputDebugString(L"\n File is Deleted !!!\n");
	}
	else
	{	
		DWORD dLError=GetLastError();
		if(dLError!=ERROR_FILE_NOT_FOUND)
		{
			wsprintf(waTemp, L"%s%d%s", EXT_VDO_FN, m_nFileAutoCnt, L".wmv");
			m_nFileAutoCnt++;
			m_wsVFName=waTemp;
			OutputDebugString(L"\nExtra File Name:-");
			OutputDebugString(m_wsVFName.c_str());
		}
	}

	if(NULL != m_pSink)
	{
		hr = m_pSink->SetFileName((LPCOLESTR)m_wsVFName.c_str(), 0);
		m_wsVFName=waTempFile;			//To restore the original name
		if(FAILED(hr))
		{
			OutputDebugString(L"\n Error Setting File Name !!\n");
		}
	}

	hr = m_pCaptureGraphBuilder->ControlStream(&PIN_CATEGORY_CAPTURE,
		&MEDIATYPE_Video, m_pVideoCaptureFilter,
		&rtStart, &rtStop, StartCookie, StopCookie);

	hr = m_pCaptureGraphBuilder->ControlStream(&PIN_CATEGORY_CAPTURE,
		&MEDIATYPE_Audio, m_pAudioCaptureFilter,
		&rtStart, &rtStop, StartCookie, StopCookie);

	if(SUCCEEDED(hr))
	{
		LONG lEventCode = 0, lParam1 = 0, lParam2 = 0;

		// wait for the start events for the capture
		do
		{
			// if no events are received within 30 seconds, then we have an error.
			// since we're starting a capture, there should be a minimum delay from the
			// start to the actual start.
			hr = m_pMediaEventEx->GetEvent( &lEventCode, &lParam1, &lParam2, 30*1000 );
			if(SUCCEEDED(hr))
			{
#ifdef DEBUG_MODE			
				if(lEventCode !=0)
				{
					TCHAR waTemp[TXT_LENGTH];
					wsprintf(waTemp, L"\nStart Event Code :- %ld ; Param2 :- %ld", lEventCode, lParam2);
					OutputDebugString(waTemp);
				}
#endif
				// if the event code we recieved was EC_STREAM_CONTROL_STARTED, then
				// we successfully started the video capture
				if(lEventCode==EC_STREAM_CONTROL_STARTED && lParam2==StartCookie)
				{
					OutputDebugString(L"\n---> Stream Capture Started --->");
					break;
				}
				// if we recieve an event, free the event parameters.
				// we can do this because we're not referencing lparam1 or lparam2.
				if(SUCCEEDED(hr = m_pMediaEventEx->FreeEventParams( lEventCode, lParam1, lParam2 )))
				{
#ifdef DEBUG_MODE
					OutputDebugString(L"\n<--- Free Event Params --->");	
#endif
				}
			}
			// if we don't recieve the stream control started event within a reasonable amount of time,
			// then we have an error.
		}while(SUCCEEDED(hr));

	}

	return hr;
} //end StartCapture

//------------------------------------------------------------------
//
// Prototype:	HRESULT StopCapture()
//
// Description:	This is used to stop video stream capturing.
//
// Parameters:	none
//
// Returns:	S_OK if Stream capturing stopped successfully.
// 
// Notes:
//------------------------------------------------------------------
HRESULT CDShowCamModule::StopCapture()
{
	HRESULT hr = S_OK;
	REFERENCE_TIME rtStart = START_TIME;
	REFERENCE_TIME rtStop;

	//REFERENCE_TIME rtStart = MAXLONGLONG;
	WORD VideoStartCookie = VSTART_COOKIE, VideoStopCookie = VSTOP_COOKIE;
	WORD AudioStartCookie = ASTART_COOKIE, AudioStopCookie = ASTOP_COOKIE;
	//BOOL fAudioStopEventFound = FALSE;
	//BOOL fVideoStopEventFound = FALSE;
	BOOL fPreviewPaused = FALSE;

	if(!m_bInited)
	{
		return S_FALSE;
	}

	// Stop the video portion of the capture passing in the video
	// stop cookie we'll wait for.

	m_pMediaSeeking->GetCurrentPosition(&rtStop);
	//hr = m_pCaptureGraphBuilder->ControlStream(&PIN_CATEGORY_CAPTURE, &MEDIATYPE_Video, m_pVideoCaptureFilter, &rtStart, &rtStop, VideoStartCookie, VideoStopCookie);
	hr = m_pCaptureGraphBuilder->ControlStream(&PIN_CATEGORY_CAPTURE, &MEDIATYPE_Video, m_pVideoCaptureFilter, &rtStart, &rtStop, VideoStartCookie, VideoStopCookie);
	if(SUCCEEDED(hr))
	{
#ifdef DEBUG_MODE
		OutputDebugString(L"\n---> Video Stream is stopped <---\n");
#endif
		// Stop the audio portion of the capture passing in the audio
		// stop cookie we'll wait for.
		//hr = m_pCaptureGraphBuilder->ControlStream(&PIN_CATEGORY_CAPTURE, &MEDIATYPE_Audio, m_pAudioCaptureFilter, &rtStart, &rtStop, AudioStartCookie, AudioStopCookie);
		if(m_tCamcfg.bIsAudioEnb)
		{
			//m_pMediaSeeking->GetCurrentPosition(&rtStop);
			hr = m_pCaptureGraphBuilder->ControlStream(&PIN_CATEGORY_CAPTURE, &MEDIATYPE_Audio, m_pAudioCaptureFilter, &rtStart, &rtStop, AudioStartCookie, AudioStopCookie);
			if(SUCCEEDED(hr))
			{
#ifdef DEBUG_MODE
				OutputDebugString(L"\n---> Audio Stream is stopped <---\n");
#endif
			}
			else
			{
#ifdef DEBUG_MODE

				OutputDebugString(L"\n---> Audio Stream Failed to Stop --->\n");
#endif
			}

		}
	}
	else
	{
#ifdef DEBUG_MODE
		OutputDebugString(L"\n---> Video Stream Falied to Stop ---->\n");
#endif
	}

	// For performance purposes now that the video capture has been stopped, freeze the preview
	// window to free up CPU cycles for software video encoding.
	if(SUCCEEDED(hr))
	{
		if(SUCCEEDED(hr = m_pCaptureGraphBuilder->ControlStream( &PIN_CATEGORY_PREVIEW, 
			&MEDIATYPE_Video, m_pVideoCaptureFilter, NULL, 0, 0, 0 )))
		{
			fPreviewPaused = TRUE;
		}

	}
	// Wait for the ControlStream event. 
	OutputDebugString( L"\nWating for the control stream events\n" );	
	LONG lEventCode = 0, lParam1 = 0, lParam2 = 0;
	INT nNoOfEve=1;
	INT count=0;

	if(m_tCamcfg.bIsAudioEnb)
		nNoOfEve=2;

	do
	{
		hr=m_pMediaEventEx->GetEvent(&lEventCode, &lParam1, &lParam2, 20*1000);
		if(SUCCEEDED(hr))
		{
#ifdef DEBUG_MODE
			if(lEventCode!=0)
			{
				TCHAR waTemp[TXT_LENGTH];
				wsprintf(waTemp, L"\nStart Event Code :- %ld ; Param2 :- %ld", lEventCode, lParam2);
				OutputDebugString(waTemp);
			}
#endif
			if(lEventCode==EC_STREAM_CONTROL_STOPPED)
			{
				OutputDebugString( L"Received a control stream stop event\n" );
				count++;
			}
			m_pMediaEventEx->FreeEventParams(lEventCode, lParam1, lParam2);	
		}
	}while(count<nNoOfEve&&SUCCEEDED(hr));

	OutputDebugString( L"The file has been captured\n" );

	if(fPreviewPaused)
	{
		// an end time of MAX_TIME indicates for the stream to never stop 
		LONGLONG llEnd = MAX_TIME;
		m_pCaptureGraphBuilder->ControlStream( &PIN_CATEGORY_PREVIEW, &MEDIATYPE_Video, m_pVideoCaptureFilter, NULL, &llEnd, 0, 0 );
	}	

	//Reset the Graph
	StopGrp();
	Sleep(1000);
	RunGrp();

	return hr;
} //end StopCapture

//------------------------------------------------------------------
//
// Prototype:	HRESULT Set_Flip(BOOL bIsHori)
//
// Description:	This is used to set the flip, Horizontally or Vertically
//				depending on the value bIsHori.
//
// Parameters:	if bIsHori is "TRUE" then Horizontal Flip otherwise Vertical Flip
//
// Returns:	S_OK if Flip is set successfully.
// 
// Notes:
//------------------------------------------------------------------
HRESULT CDShowCamModule::Set_Flip(BOOL bIsHori)
{
	HRESULT	hr;
	//LONG lFlag;
	//TCHAR waTemp[100];

	if(!m_bInited)
	{
		return S_FALSE;
	}

	if(bIsHori)
	{
		hr = m_pVideoCtrl->SetMode(m_pPrvPin, VideoControlFlag_FlipHorizontal);
	}
	else
	{
		hr = m_pVideoCtrl->SetMode(m_pPrvPin, VideoControlFlag_FlipVertical);
	}

#ifdef DEBUG_MODE
	if(SUCCEEDED(hr))
	{
		OutputDebugString(L"\n------------Flip OK------------");
	}
	else
	{
		OutputDebugString(L"\n------------Flip FAIL------------");
	}
#endif

	return hr;
} //end Set_Flip (BOOL bIsHori)

//------------------------------------------------------------------
//
// Prototype:	HRESULT EnumFirstCap(PinType ePType, ImgFmt* ptIFmt, INT* pnNoOfCap)
//
// Description:	This is used to enumerate Video,Still and Preview resolution capabilities.
//
// Parameters:	
//
// Returns:	S_OK if all video resolutions are enumurated.
// 
// Notes:
//------------------------------------------------------------------
HRESULT CDShowCamModule::EnumFirstCap(PinType ePType, ImgFmt* ptIFmt, INT* pnNoOfCap)
{	
	HRESULT hr;
	GUID pType;
	INT nSize = 0;
	m_pStrConf = NULL;
	INT nCounter=0;	
	INT nFlag;
	INT nIdx=0;

	if(!m_bInited)
	{
		return S_FALSE;
	}

	switch(ePType)
	{
	case V:
		pType = PIN_CATEGORY_CAPTURE;
		nFlag = 0;
		break;
	case S:
		pType = PIN_CATEGORY_STILL;
		nFlag = 1;
		break;
	case P:
		pType = PIN_CATEGORY_PREVIEW;
		nFlag = 2;
		break;
	}

	//PIN type, Any media type, Pointer to the Capture filter.
	hr = m_pCaptureGraphBuilder->FindInterface(&pType, 0, m_pVideoCaptureFilter, \
		IID_IAMStreamConfig, (void**)&m_pStrConf);

	//hr = m_pCaptureGraphBuilder->FindInterface(&pType, 0, m_pVideoEncoder, 
	//									IID_IAMStreamConfig, (void**)&m_pStrConf);
	if(SUCCEEDED(hr))
	{
		hr = m_pStrConf->GetNumberOfCapabilities(pnNoOfCap, &nSize);

		/*
		if(m_ptCamCap[nFlag]!=NULL)
		{
		delete [] m_ptCamCap[nFlag];
		OutputDebugString(L"\n---------- Old Object deleted!!!----------");
		}

		//Creating a CamCapability Structure to store enumurated camera capabilities for each PIN type
		m_ptCamCap[nFlag] = new CamCapability[nSize];
		*/
		if(SUCCEEDED(hr))
		{
			// Check the size to make sure we pass in the correct structure.
			if (nSize == sizeof(VIDEO_STREAM_CONFIG_CAPS))
			{
				// Use the video capabilities structure.
				VIDEO_STREAM_CONFIG_CAPS scc;
				AM_MEDIA_TYPE *pmtConfig = NULL;
				hr = m_pStrConf->GetStreamCaps(nIdx, &pmtConfig, (BYTE*)&scc);
				if (SUCCEEDED(hr))
				{
					ptIFmt->nMaxWidth = scc.MaxOutputSize.cx;
					ptIFmt->nMaxHeight = scc.MaxOutputSize.cy;
					ptIFmt->qwMaxFrameRate = scc.MaxFrameInterval;

					if(pmtConfig->majortype == MEDIATYPE_Video)
					{
						while((media_list[nCounter].pMSubType != NULL) && (pmtConfig->subtype != *media_list[nCounter].pMSubType))
						{
							nCounter++;
						}
						ptIFmt->wsColorFmt = media_list[nCounter].wsClrName;

					}

				}
				// Delete the media type when you are done.
				if(pmtConfig != NULL)
				{
					DeleteMediaType(pmtConfig);
				}

			}

		}

	}

	if(m_pStrConf != NULL)
	{
		m_pStrConf->Release();
		m_pStrConf = NULL;
	}

	return hr;

} //end EnumFirstCap(PinType ePType, ImgFmt* ptIFmt, INT* pnNoOfCap)

//------------------------------------------------------------------
//
// Prototype:	HRESULT EnumOtherCap(PinType ePType, ImgFmt* ptIFmt, INT nNoOfCap)
//
// Description:	This is used to enumerate Video,Still and Preview resolution capabilities.
//
// Parameters:	
//
// Returns:	S_OK if all video resolutions are enumurated.
// 
// Notes:
//------------------------------------------------------------------
HRESULT CDShowCamModule::EnumOtherCap(PinType ePType, ImgFmt* ptIFmt, INT nNoOfCap)
{	
	HRESULT hr;
	GUID pType;
	m_pStrConf = NULL;
	INT nCounter=0;	
	INT nFlag;

	if(!m_bInited)
	{
		return S_FALSE;
	}

	switch(ePType)
	{
	case V:
		pType = PIN_CATEGORY_CAPTURE;
		nFlag = 0;
		break;
	case S:
		pType = PIN_CATEGORY_STILL;
		nFlag = 1;
		break;
	case P:
		pType = PIN_CATEGORY_PREVIEW;
		nFlag = 2;
		break;

	}

	//PIN type, Any media type, Pointer to the capture filter.
	hr = m_pCaptureGraphBuilder->FindInterface(&pType, 0, m_pVideoCaptureFilter, \
		IID_IAMStreamConfig, (void**)&m_pStrConf);

	//hr = m_pCaptureGraphBuilder->FindInterface(&pType, 0, m_pVideoEncoder, 
	//									IID_IAMStreamConfig, (void**)&m_pStrConf);
	if(SUCCEEDED(hr))
	{

		// Use the video capabilities structure.
		for (int iFormat = 1; iFormat < nNoOfCap; iFormat++)
		{
			VIDEO_STREAM_CONFIG_CAPS scc;
			AM_MEDIA_TYPE *pmtConfig = NULL;
			hr = m_pStrConf->GetStreamCaps(iFormat, &pmtConfig, (BYTE*)&scc);
			if (SUCCEEDED(hr))
			{
				ptIFmt->nMaxWidth = scc.MaxOutputSize.cx;
				ptIFmt->nMaxHeight = scc.MaxOutputSize.cy;
				ptIFmt->qwMaxFrameRate = scc.MaxFrameInterval;

				if(pmtConfig->majortype == MEDIATYPE_Video)
				{
					while(media_list[nCounter].pMSubType != NULL)
					{
						if(pmtConfig->subtype != *media_list[nCounter].pMSubType)
							nCounter++;

						else
							break;
					}
					ptIFmt->wsColorFmt = media_list[nCounter].wsClrName;
					nCounter=0;

					/*
					//Update this structre for Class internal use
					m_ptCamCap[nFlag]++;
					m_ptCamCap[nFlag]->nIndex = iFormat;
					m_ptCamCap[nFlag]->tImgRes.nWidth = scc.MaxOutputSize.cx;
					m_ptCamCap[nFlag]->tImgRes.nHeight = scc.MaxOutputSize.cy;
					m_ptCamCap[nFlag]->wsColorFmt = media_list[nCounter].wsClrName;
					*/
				}
				ptIFmt++;
			}
			// Delete the media type when you are done.
			if(pmtConfig != NULL)
			{
				DeleteMediaType(pmtConfig);
			}
		}


		/*
		//Insert last record to Identify the end Points
		m_ptCamCap[nFlag]++;
		m_ptCamCap[nFlag]->nIndex = 999;
		m_ptCamCap[nFlag]->tImgRes.nWidth = 999;
		m_ptCamCap[nFlag]->tImgRes.nHeight = 999;
		m_ptCamCap[nFlag]->wsColorFmt = L"";
		m_ptCamCap[nFlag]->qwMaxFrameRate = 999999;
		*/

	}

	if(m_pStrConf != NULL)
	{
		m_pStrConf->Release();
		m_pStrConf = NULL;
	}

	return hr;

} //end EnumOtherCap(PinType ePType, ImgFmt* ptIFmt, INT pnNoOfCap)

//------------------------------------------------------------------
//
// Prototype:	HRESULT Set_Resolution(ImageRes *ptRes, PinType ePType)
//
// Description:	This is used to set the resolution of either Preview, 
//				Still or Capture Pins.
//
// Parameters:	ptRes is a pointer to ImageRes struture, which holds the
//				resolution to set and ePType is Pin category enumuration;
//				V:-Capture Pin, S:-Still Pin, P:-Preview Pin
//
// Returns:	S_OK if Image resolution is set successfully.
// 
// Notes:
//------------------------------------------------------------------
HRESULT CDShowCamModule::Set_Resolution(ImageRes* ptRes, PinType ePType, BOOL bAutoControlGraph)
{	
	HRESULT hr;
	GUID pType;
	m_pStrConf = NULL;
	if(!m_bInited)
	{
		return S_FALSE;
	}

	//capture device can support a range of output formats. For example, 
	//a device might support 16-bit RGB, 32-bit RGB, and YUYV. Within each of these formats, 
	//the device can support a range of frame sizes.
	//The IAMStreamConfig interface is used to report which formats the device supports, 
	//and to set the format. The IAMStreamConfig interface is exposed on the capture filter's capture pin,
	//preview pin, or both. Use the ICaptureGraphBuilder2::FindInterface method to get the interface
	//pointer:

	switch(ePType)
	{
	case V:
		pType = PIN_CATEGORY_CAPTURE;
		OutputDebugString(L"\n----------CAPTURE PIN-----------\n");
		break;
	case S:
		pType = PIN_CATEGORY_STILL;
		OutputDebugString(L"\n----------STILL PIN-----------\n");
		break;
	case P:
		pType = PIN_CATEGORY_PREVIEW;
		OutputDebugString(L"\n----------PREVIEW PIN-----------\n");
		break;

	}

	//PIN type, Any media type, Pointer to the capture filter.
	hr = m_pCaptureGraphBuilder->FindInterface(&pType, 0, m_pVideoCaptureFilter,\
		IID_IAMStreamConfig, (void**)&m_pStrConf);
	//hr = m_pCaptureGraphBuilder->FindInterface(&pType, 0, m_pVideoEncoder,
	//												IID_IAMStreamConfig, (void**)&m_pStrConf);

	if(SUCCEEDED(hr))
	{
		VIDEO_STREAM_CONFIG_CAPS scc;
		AM_MEDIA_TYPE *pmtConfig = NULL;
		INT iResIdx=0;

		INT nCount = 0, nSize = 0;

		hr = m_pStrConf->GetNumberOfCapabilities(&nCount, &nSize);
		if(FAILED(hr))
		{
			goto _exitSetResolution;
		}

		for(int i=iResIdx; i<nCount;i++)
		{
			hr = m_pStrConf->GetStreamCaps(i, &pmtConfig, (BYTE*)&scc);

			if (SUCCEEDED(hr))
			{
				if((abs(scc.MaxOutputSize.cx)==abs(ptRes->nWidth)) && (abs(scc.MaxOutputSize.cy)==abs(ptRes->nHeight)))
				{
					if(bAutoControlGraph)
					{
						CDShowCamModule::StopGrp();
					}

					hr = m_pStrConf->SetFormat(pmtConfig);
					if((FAILED(hr)))
					{
						// Delete the media type when you are done.
						if(pmtConfig != NULL)
						{
							DeleteMediaType(pmtConfig);
						}
						goto _exitSetResolution;
					}
					//if(ePType==V)
					//	CDShowCamModule::ReBuildGrp();
					if(bAutoControlGraph)
					{
						CDShowCamModule::RunGrp();
					}
					// Delete the media type when you are done.
					if(pmtConfig != NULL)
					{
						DeleteMediaType(pmtConfig);
					}					
					break;
				}
			}
			// Delete the media type when you are done.
				if(pmtConfig != NULL)
				{
					DeleteMediaType(pmtConfig);
				}
		}
	}

	/*

	// To configure the device to use a format returned in the 
	// IAMStreamConfig::GetStreamCaps method, 
	// call IAMStreamConfig::SetFormat with the media type:
	AM_MEDIA_TYPE *pmtConfig;
	hr = pConfig->SetFormat(pmtConfig);
	CHK(hr);
	// If the pin is not connected, it will attempt to use this 
	// format when it does connect. If the pin is already connected, 
	// it attempts to reconnect using the new format. In either case, it is possible that the downstream filter will reject the format. 

	// For example, suppose that IAMStreamConfig::GetStreamCaps returns a 24-bit RGB
	// format, with a frame size of 320 x 240 pixels. 
	// You can get this information by examining the major type, subtype, 
	// and format block of the media type:

	if ((pmtConfig.majortype == MEDIATYPE_Video) &&
	(pmtConfig.subtype == MEDIASUBTYPE_RGB24) &&
	(pmtConfig.formattype == FORMAT_VideoInfo) &&
	(pmtConfig.cbFormat >= sizeof (VIDEOINFOHEADER)) &&
	(pmtConfig.pbFormat != NULL))
	{
	VIDEOINFOHEADER *pVih = (VIDEOINFOHEADER*)pmtConfig.pbFormat;
	// pVih contains the detailed format information.
	LONG lWidth = pVih->bmiHeader.biWidth;
	LONG lHeight = pVih->bmiHeader.biHeight;
	}
	// The VIDEO_STREAM_CONFIG_CAPS structure gives the minimum and maximum width 
	// and height that can be used for this media type. It also gives 
	// you the "step" size, which defines the increments by which you can adjust the 
	// width or height. For example, the device might return the following: 
	// MinOutputSize: 160 x 120 
	// MaxOutputSize: 320 x 240 <-- from microsoft doc
	// OutputGranularityX: 8 pixels (horizontal step size) 
	//OutputGranularityY: 8 pixels (vertical step size) 
	// Given these numbers, you could set the width to anything in
	// the range (160, 168, 176, ... 304, 312, 320) and the height to anything
	// in the range (120, 128, 136, ... 104, 112, 120).



	// To set a new frame size, directly modify the AM_MEDIA_TYPE structure returned in
	// IAMStreamConfig::GetStreamCaps:

	pVih->bmiHeader.biWidth = 160;
	pVih->bmiHeader.biHeight = 120;
	pVih->bmiHeader.biSizeImage = DIBSIZE(pVih->bmiHeader);

	// Then pass the media type to the IAMStreamConfig::SetFormat method, 
	// as described previously. 


	Cleanup:
	#ifdef DEBUG_MODE
	LoadString(hInst, STR_CLEANUP, text, MAX_LOADSTRING);
	OutputDebugString(text);
	#endif

	*/
_exitSetResolution:
	if(SUCCEEDED(m_pStrConf->Release()))
	{
		m_pStrConf = NULL;
	}

	return hr;
} //end Set_Resolution(ImageRes *ptRes, PinType ePType)

//------------------------------------------------------------------
//
// Prototype:	HRESULT Set_ColorFmt(ImageRes* ptRes, wstring* pwsClrFmt, PinType ePType)
//
// Description:	This is used to set the color format of either Preview, 
//				Still or Capture Pin.
//
// Parameters:	pwsClrFmt is a unicode string pointer, holding the color
//				format to be set and ePType is Pin category enumuration;
//				V:-Capture Pin, S:-Still Pin, P:-Preview Pin
//
// Returns:	S_OK if Color format is set successfully.
// 
// Notes:
//------------------------------------------------------------------
HRESULT CDShowCamModule::Set_ColorFmt(ImageRes* ptRes, wstring* pwsClrFmt, PinType ePType)
{	
	HRESULT hr;
	GUID pType;
	m_pStrConf = NULL;
	INT nIdx=0;
	INT nCount = 0, nSize = 0;
	INT nCounter = 0;

	if(!m_bInited)
	{
		return S_FALSE;
	}

	OutputDebugString(L"\n---->");
	OutputDebugString(pwsClrFmt->c_str());

	switch(ePType)
	{
	case V:
		pType = PIN_CATEGORY_CAPTURE;
		break;
	case S:
		pType = PIN_CATEGORY_STILL;
		break;
	case P:
		pType = PIN_CATEGORY_PREVIEW;
		break;

	}

	//PIN type, Any media type, Pointer to the capture filter.
	//hr = m_pCaptureGraphBuilder->FindInterface(&pType, 0, m_pVideoCaptureFilter,
	//												IID_IAMStreamConfig, (void**)&m_pStrConf);
	hr = m_pCaptureGraphBuilder->FindInterface(&pType, 0, m_pVideoEncoder,\
		IID_IAMStreamConfig, (void**)&m_pStrConf);

	if(SUCCEEDED(hr))
	{
		VIDEO_STREAM_CONFIG_CAPS scc;
		AM_MEDIA_TYPE *pmtConfig = NULL;

		hr = m_pStrConf->GetNumberOfCapabilities(&nCount, &nSize);
		if(FAILED(hr))
		{
			goto _exitSetColorFmt;
		}

		if (nSize == sizeof(VIDEO_STREAM_CONFIG_CAPS))
		{
			while((*pwsClrFmt != media_list[nCounter].wsClrName) &&\
				(media_list[nCounter].wsClrName != L"INVALIDE"))
			{
				nCounter++;
			}

			if(media_list[nCounter].pMSubType !=NULL)
			{
				for(INT i=nIdx; i<nCount;i++)
				{
					hr = m_pStrConf->GetStreamCaps(i, &pmtConfig, (BYTE*)&scc);
					if (SUCCEEDED(hr))
					{
						if(pmtConfig->majortype == MEDIATYPE_Video)
						{						
							if(pmtConfig->subtype == *media_list[nCounter].pMSubType)
							{
								if((scc.MaxOutputSize.cx==ptRes->nWidth) && (scc.MaxOutputSize.cy==ptRes->nHeight))
								{
									//m_pMediaControl->Stop();
									CDShowCamModule::StopGrp();
									hr = m_pStrConf->SetFormat(pmtConfig);
									//m_pMediaControl->Run();
									CDShowCamModule::RunGrp();
									if(SUCCEEDED(hr))
									{
										goto _exitSetColorFmt;
									}
									break;
								}
							}
						}
					}
				}
				// Delete the media type when you are done.
				if(pmtConfig != NULL)
				{
					DeleteMediaType(pmtConfig);
				}
			}
			else
				OutputDebugString(L"\n-------------NULL---------------");

		}

	}

_exitSetColorFmt:
	if(SUCCEEDED(m_pStrConf->Release()))
	{
		m_pStrConf = NULL;
	}
	return hr;
} //end Set_ColorFmt(wstring* pwsClrFmt, PinType ePType)

//------------------------------------------------------------------
//
// Prototype:	HRESULT Set_FrameRate(LONGLONG qwFrmRate, PinType ePType)
//
// Description:	This is used to set the resolution of either Preview, 
//				Still or Capture Pins.
//
// Parameters:	pwsClrFmt is a unicode string pointer, holding the color format to set
//				and ePType is Pin category enumuration;
//				V:-Capture Pin, S:-Still Pin, P:-Preview Pin
//
// Returns:	S_OK if Color format is set successfully.
// 
// Notes:
//------------------------------------------------------------------
HRESULT CDShowCamModule::Set_FrameRate(LONGLONG qwFrmRate, PinType ePType)
{	
	HRESULT hr;
	GUID pType;
	m_pStrConf = NULL;
	INT nIdx=0;
	INT nCount = 0, nSize = 0;

	if(!m_bInited)
	{
		return S_FALSE;
	}

	switch(ePType)
	{
	case V:
		pType = PIN_CATEGORY_CAPTURE;
		break;
	case S:
		pType = PIN_CATEGORY_STILL;
		break;
	case P:
		pType = PIN_CATEGORY_PREVIEW;
		break;

	}

	//PIN type, Any media type, Pointer to the capture filter.
	hr = m_pCaptureGraphBuilder->FindInterface(&pType, 0, m_pVideoCaptureFilter,\
		IID_IAMStreamConfig, (void**)&m_pStrConf);

	if(SUCCEEDED(hr))
	{
		VIDEO_STREAM_CONFIG_CAPS scc;
		AM_MEDIA_TYPE *pmtConfig = NULL;

		hr = m_pStrConf->GetNumberOfCapabilities(&nCount, &nSize);
		if(FAILED(hr))
		{
			goto _exitFrameRate;
		}

		if (nSize == sizeof(VIDEO_STREAM_CONFIG_CAPS))
		{

			for(INT i=nIdx; i<nCount;i++)
			{
				hr = m_pStrConf->GetStreamCaps(i, &pmtConfig, (BYTE*)&scc);
				if (SUCCEEDED(hr))
				{
					if(pmtConfig->majortype == MEDIATYPE_Video)
					{						
						if(scc.MaxFrameInterval == qwFrmRate)
						{
							//m_pMediaControl->Stop();
							CDShowCamModule::StopGrp();
							hr = m_pStrConf->SetFormat(pmtConfig);
							//m_pMediaControl->Run();
							CDShowCamModule::RunGrp();
							if(SUCCEEDED(hr))
							{
								goto _exitFrameRate;
							}
							break;
						}
					}
				}
				// Delete the media type when you are done.
				if(pmtConfig != NULL)
				{
					DeleteMediaType(pmtConfig);
				}
			}
			OutputDebugString(L"\n-----------Invalide Frame Rate--------------");
		}
	}

_exitFrameRate:
	if(SUCCEEDED(m_pStrConf->Release()))
	{
		m_pStrConf = NULL;
	}
	return hr;
} //end Set_FrameRate(LONGLONG qwFrmRate, PinType ePType)

//------------------------------------------------------------------
//
// Prototype:	HRESULT SetupPreview(HWND OwnerWnd, RECT rc)
//
// Description:	This is a private method which sets up all componets 
//				necessary for a preview window.
//
// Parameters:	Window Handler(OwnerWnd) to which the preview window should display
//				and the location/dimenssion of the window is given as rc
//
// Returns:	S_OK if all preview setup is successful.
// 
// Notes:
//------------------------------------------------------------------
HRESULT CDShowCamModule::SetupPreview(HWND OwnerWnd, RECT rc)
{
	HRESULT hr = S_OK;
	//	IBaseFilter* pVideoRenderer = NULL;
	IVideoWindow* pVideoWindow = NULL;

	hr = CoCreateInstance(CLSID_VideoRenderer, NULL, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (void**)&m_pVideoRenderer);
	if(FAILED(hr))
	{
		return FALSE;
	}
	hr = m_pGraphBuilder->AddFilter(m_pVideoRenderer, L"My Video Renderer");
	if(FAILED(hr))
	{
		return FALSE;
	}
	//--------------- MPA3 ----------------------
	if(IsMPA3())
	{
		hr = CoCreateInstance(CLSID_COLOR_CONVERTER, NULL, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (void**)&m_pColorConverter);
		if(FAILED(hr))
		{
			return FALSE;
		}
	
		hr = m_pGraphBuilder->AddFilter(m_pColorConverter, L"Texas Instruments Color Converter Filter");
		if(FAILED(hr))
		{
			return FALSE;
		}
	}
	//-------------------- ----------------------
	
	// Render the preview pin of the video capture filter. 
	OutputDebugString(L"\n!!!! Render PREVIEW PIN !!!!\n");
	if(SUCCEEDED(hr = m_pCaptureGraphBuilder->RenderStream( &PIN_CATEGORY_PREVIEW, &MEDIATYPE_Video, m_pVideoCaptureFilter, m_pColorConverter, m_pVideoRenderer )))
	{
		//if(SUCCEEDED(hr = m_pCaptureGraphBuilder->RenderStream( &PIN_CATEGORY_PREVIEW, &MEDIATYPE_Audio, m_pAudioCaptureFilter, NULL, NULL )))
		{
			// If rendering the preview succeeded, then we need to find the video
			// renderer filter in order to set up the video window dimensions,
			// location, owner window, etc.
			//LoadString(hInst, STR_VRENDERER, text, MAX_LOADSTRING);
			//if(SUCCEEDED(hr = m_pGraphBuilder->FindFilterByName(L"My Video Renderer", &pVideoRenderer)))
			{
				if(SUCCEEDED(hr = m_pVideoRenderer->QueryInterface( IID_IVideoWindow, (void **)&pVideoWindow)))
				{
					// For now just set the owner window.
					hr = pVideoWindow->put_Owner((OAHWND) OwnerWnd);
					//---------------
					if(SUCCEEDED(hr))
					{
						hr = pVideoWindow->SetWindowForeground(OATRUE);
						if(!(SUCCEEDED(hr)))
						{
							return false;
						}

						hr = pVideoWindow->put_WindowStyle(WS_CHILD | WS_CLIPCHILDREN);
						if(!(SUCCEEDED(hr)))
						{
							return false;
						}
					}
					// Resize the video preview window to match owner window size
					if (pVideoWindow)
					{
						//LoadString(hInst, STR_VV_CAP, text, MAX_LOADSTRING);
						//pVideoWindow->SetWindowPosition(MARGIN_GAP,MARGIN_GAP,((rc.right-rc.left)-2*MARGIN_GAP),((rc.bottom-rc.top)*2/3-2*MARGIN_GAP));
						TCHAR waTemp[100];
						wsprintf(waTemp,L"IN:- %d -- %d -- %d -- %d",rc.bottom,rc.left,rc.right,rc.top);
						OutputDebugString(waTemp);

						pVideoWindow->SetWindowPosition(rc.left, rc.top, (rc.right-rc.left), (rc.bottom-rc.top));
						pVideoWindow->put_Caption(L"CAM1 preview");
					}

					hr = pVideoWindow->put_Visible(OATRUE);
					if(FAILED(hr))
					{
						return false;
					}
					//----------------
				}
			}
		}
	}
	// cleanup
	if(pVideoWindow)
	{
		pVideoWindow->Release();
		pVideoWindow = NULL;
	}
	//if(pVideoRenderer)
	{
		//	pVideoRenderer->Release();
		//	pVideoRenderer = NULL;
	}

	return hr;
}// end SetupPreview

HRESULT CDShowCamModule::ShowPreview()
{
	HRESULT hr = S_OK;
	//	IBaseFilter* pVideoRenderer = NULL;
	IVideoWindow* pVideoWindow = NULL;

	if(!m_bInited)
	{
		return S_FALSE;
	}

	if(SUCCEEDED(hr = m_pVideoRenderer->QueryInterface( IID_IVideoWindow, (void **)&pVideoWindow)))
	{
		hr = pVideoWindow->put_Visible(OATRUE);
		if(FAILED(hr))
		{
			goto _exitShowPreview;
		}
		//----------------
	}

_exitShowPreview:
	if(SUCCEEDED(pVideoWindow->Release()))
	{
		pVideoWindow = NULL;
	}
	return hr;
}

HRESULT CDShowCamModule::HidePreview()
{
	HRESULT hr = S_OK;
	//	IBaseFilter* pVideoRenderer = NULL;
	IVideoWindow* pVideoWindow = NULL;

	if(SUCCEEDED(hr = m_pVideoRenderer->QueryInterface( IID_IVideoWindow, (void **)&pVideoWindow)))
	{
		hr = pVideoWindow->put_Visible(OAFALSE);
		if(FAILED(hr))
		{
			goto _exitHidePreview;
		}
		//----------------
	}

_exitHidePreview:
	if(SUCCEEDED(pVideoWindow->Release()))
	{
		pVideoWindow = NULL;
	}
	return hr;
}

HRESULT CDShowCamModule::ClosePreview()
{
	HRESULT hr = S_OK;
	//	IBaseFilter* pVideoRenderer = NULL;
	IVideoWindow* pVideoWindow = NULL;

	if(SUCCEEDED(hr = m_pVideoRenderer->QueryInterface( IID_IVideoWindow, (void **)&pVideoWindow)))
	{
		hr = pVideoWindow->put_Owner(NULL);
		if(FAILED(hr))
		{
			goto _exitClosePreview;
		}
		//----------------
	}

_exitClosePreview:
	if(SUCCEEDED(pVideoWindow->Release()))
	{
		pVideoWindow = NULL;
	}
	return hr;
}

//------------------------------------------------------------------
//
// Prototype:	HRESULT SetupStill()
//
// Description:	This is a private method which sets up all componets 
//				necessary for a still capture.
//
// Parameters:	none
//
// Returns:	S_OK if still setup is successful.
// 
// Notes:
//------------------------------------------------------------------
HRESULT CDShowCamModule::SetupStill()
{
	HRESULT hr = S_OK;
	WCHAR DName[TXT_LENGTH];
	WCHAR waTemp[TXT_LENGTH];
	SYSTEMTIME st;
	GetLocalTime(&st);

	//LoadString(hInst, STR_STILL_D_F_FMT, text, MAX_LOADSTRING);
	wsprintf(waTemp, L"%.2d%.2d%.2d", st.wMonth, st.wDay, st.wYear%100);
	//LoadString(hInst, STR_STILL_DIR_PTH, text, MAX_LOADSTRING);
	wsprintf(DName, L"\\My Documents\\My Pictures\\%s", waTemp);
	CreateDirectory(DName, NULL);

	//CoCreate and add the still image sink
	OutputDebugString(L"\n!!!! Create CLSID_IMGSinkFilter !!!!\n");
	if(SUCCEEDED(hr = CoCreateInstance(CLSID_IMGSinkFilter, NULL, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (LPVOID *)&m_pStillSink )))
	{
		//LoadString(hInst, STR_STILLSINK, text, MAX_LOADSTRING);
		if(SUCCEEDED(hr = m_pGraphBuilder->AddFilter(m_pStillSink, L"StillSink")))
		{
			OutputDebugString(L"\n!!!! Add CLSID_IMGSinkFilter !!!!\n");
			// Render the still image pin to the still image sink.
			hr = m_pCaptureGraphBuilder->RenderStream( &PIN_CATEGORY_STILL, &MEDIATYPE_Video, m_pVideoCaptureFilter, NULL, m_pStillSink);
			if(SUCCEEDED(hr))
				OutputDebugString(L"\n!!!! Render STILL PIN OK!!!!\n");
			else
				OutputDebugString(L"\n!!!! Render STILL PIN Fail!!!!\n");
		}
	}
	return hr;

}// end SetupStill

//------------------------------------------------------------------
//
// Prototype:	HRESULT SetupFlip()
//
// Description:	This is a private method which sets up all componets 
//				necessary for Window flipping.
//
// Parameters:	none
//
// Returns:	S_OK if Flip setup is successful.
// 
// Notes:
//------------------------------------------------------------------
HRESULT CDShowCamModule::SetupFlip()
{
	HRESULT hr;

	m_pPrvPin = NULL;

	if(SUCCEEDED(hr = m_pCaptureGraphBuilder->FindPin((IUnknown*)m_pVideoCaptureFilter, PINDIR_OUTPUT, &PIN_CATEGORY_CAPTURE, &MEDIATYPE_Video, FALSE, 0, &m_pPrvPin )))
	{
		hr = m_pVideoCaptureFilter->QueryInterface( IID_IAMVideoControl, (void **)&m_pVideoCtrl );
	}


#ifdef DEBUG_MODE
	if(FAILED(hr))
	{
		//	LoadString(hInst, ERR_FLIP_SETUP, text, MAX_LOADSTRING);
		//	OutputDebugString(text);
	}
#endif


	return hr;
}// end SetupFlip

//------------------------------------------------------------------
//
// Prototype:	HRESULT Cus_GetRange(CameraCustomProperty eProp, LONG* plMin,
//											LONG* plMax, LONG* plDelta, LONG* plDef)
//
// Description:	This is a private method which retrives the limits of a given custom property.
//
// Parameters:	eProp is the custom property to query, plMin is pointer to Minimum value of the property,
//				plMax is pointer to Maximum value of the property, plDelta is pointer to Step value of the property,
//				and finally plDef is pointer to Default value of the property)
//
// Returns:	S_OK if limits are retreived successfully.
// 
// Notes:
//------------------------------------------------------------------
HRESULT CDShowCamModule::Cus_GetRange(CameraCustomProperty eProp, LONG* plMin, LONG* plMax, LONG* plDelta, LONG* plDef)
{
	HRESULT hr = S_OK;

	if(!m_bInited)
	{
		return S_FALSE;
	}

	if(eProp == CUS_PROPERTY_AIMING)
	{
		*plMin=0;
		*plMax=2;
		*plDelta=1;
		*plDef=0;
	}
	else
	{
		*plMin=0;
		*plMax=0;
		*plDelta=0;
		*plDef=0;
	}


	return hr;
}//end Cus_GetRange

//------------------------------------------------------------------
//
// Prototype:	HRESULT Cus_Get(CameraCustomProperty eProp, LONG* plVal)
//
// Description:	This is a private method which retrives the current value of the given custom property.
//
// Parameters:	eProp is the custom property to query, plVal returns the current value of the property.
//
// Returns:	S_OK if value is retreived successfully.
// 
// Notes:
//------------------------------------------------------------------
HRESULT CDShowCamModule::Cus_Get(CameraCustomProperty eProp, LONG* plVal)
{
	HRESULT hr = S_FALSE;

	if(m_bInited)
	{
		if(eProp==CUS_PROPERTY_AIMING)
		{
			hr = GunGet((DWORD* )plVal);
		}
	}

	return hr;
}//end Cus_Get

//------------------------------------------------------------------
//
// Prototype:	HRESULT Cus_Set(CameraCustomProperty eProp, LONG plVal)
//
// Description:	This is a private method which sets value of the given custom property.
//
// Parameters:	eProp is the custom property to query, plVal is value to set.
//
// Returns:	S_OK if value is successfully set.
// 
// Notes:
//------------------------------------------------------------------
HRESULT CDShowCamModule::Cus_Set(CameraCustomProperty eProp, LONG plVal)
{
	HRESULT hr = S_FALSE;

	if(m_bInited)
	{
		if(eProp==CUS_PROPERTY_AIMING)
		{
			hr = GunSet((DWORD)plVal);
		}
	}

	return hr;
}//end Cus_Set


//------------------------------------------------------------------
//
// Prototype:	DWORD GunGet(DWORD *value)
//
// Description:	This is a private method which retreives current value of the custom property Aiming.
//
// Parameters:	value is double word pointer, holding current value of the property.
//
// Returns:	S_OK if successful.
// 
// Notes:
//------------------------------------------------------------------
DWORD CDShowCamModule::GunGet(DWORD *value)
{
	UNREFERENCED_PARAMETER(value);
	HRESULT hr=S_OK;

	if(!m_bInited)
	{
		return S_FALSE;
	}

#ifdef CUSTOM_PROP
	IKsPropertySet* pKs;

	m_pCamCtrl = NULL;

	hr = m_pVideoCaptureFilter->QueryInterface(IID_IAMCameraControl, (void **)&m_pCamCtrl);
	if(SUCCEEDED(hr))
	{
		hr = m_pCamCtrl->QueryInterface(IID_IKsPropertySet,(void **)&pKs);

		if(SUCCEEDED(hr))
		{     
			DWORD myLen;
			CSPROPERTY_CAMERACONTROL_S psprop;
			psprop.Property.Set = PROPSETID_SYMBOLCUSTOM_CAMERACONTROL;
			psprop.Property.Id  = CSPROPERTY_SYMBOLCUSTOM_CAMERACONTROL_GUNSITE;
			psprop.Property.Flags = CSPROPERTY_TYPE_GET;

			psprop.Value = 0;
			psprop.Flags = CSPROPERTY_CAMERACONTROL_FLAGS_MANUAL;

			psprop.Capabilities = CSPROPERTY_VIDEOPROCAMP_FLAGS_MANUAL|CSPROPERTY_VIDEOPROCAMP_FLAGS_AUTO;

			myLen = sizeof(CSPROPERTY_CAMERACONTROL_S);

			//hr = pKs->Set(PROPSETID_SYMBOLCUSTOM_CAMERACONTROL, CSPROPERTY_SYMBOLCUSTOM_CAMERACONTROL_GUNSITE,NULL,0, 
			//    &psprop, myLen);
			hr = pKs->Get(PROPSETID_SYMBOLCUSTOM_CAMERACONTROL, CSPROPERTY_SYMBOLCUSTOM_CAMERACONTROL_GUNSITE, NULL,0,
				&psprop, myLen, &myLen);          

			if(!SUCCEEDED(hr))
			{
#ifdef DEBUG_MODE
				// LoadString(hInst, ERR_GUN_GET, text, MAX_LOADSTRING);
				// OutputDebugString(text);
#endif
			}
			else
			{
				if(value!=NULL)
				{
					*value = psprop.Value;
				}
			}
			hr = pKs->Release();          

		}
		//free resources
		m_pCamCtrl->Release();
	}
	else
	{
		OutputDebugString(L"\n!!!! Error Query IID_IAMCameraControl !!!! \n");
	}
#endif
	return hr;
}//end GunGet

//------------------------------------------------------------------
//
// Prototype:	DWORD GunSet(DWORD value)
//
// Description:	This is a private method which sets the Aiming value.
//
// Parameters:	value is double word holding value to set.
//
// Returns:	S_OK if successful.
// 
// Notes:
//------------------------------------------------------------------
DWORD CDShowCamModule::GunSet(DWORD value)
{
	UNREFERENCED_PARAMETER(value);
	HRESULT hr=S_OK;

	if(!m_bInited)
	{
		return S_FALSE;
	}

#ifdef CUSTOM_PROP
	IKsPropertySet* pKs;

	m_pCamCtrl = NULL;

	hr = m_pVideoCaptureFilter->QueryInterface(IID_IAMCameraControl, (void **)&m_pCamCtrl);
	if(SUCCEEDED(hr))
	{
		hr = m_pCamCtrl->QueryInterface(IID_IKsPropertySet,(void **)&pKs);
		if(SUCCEEDED(hr))
		{	
			DWORD myLen;
			CSPROPERTY_CAMERACONTROL_S psprop;
			psprop.Property.Set = PROPSETID_SYMBOLCUSTOM_CAMERACONTROL;
			psprop.Property.Id  = CSPROPERTY_SYMBOLCUSTOM_CAMERACONTROL_GUNSITE;
			psprop.Property.Flags = CSPROPERTY_TYPE_SET;
			psprop.Value = value;
			psprop.Flags = CSPROPERTY_CAMERACONTROL_FLAGS_MANUAL;
			psprop.Capabilities = CSPROPERTY_VIDEOPROCAMP_FLAGS_MANUAL|CSPROPERTY_VIDEOPROCAMP_FLAGS_AUTO;
			myLen = sizeof(CSPROPERTY_CAMERACONTROL_S);

			hr = pKs->Set(PROPSETID_SYMBOLCUSTOM_CAMERACONTROL, CSPROPERTY_SYMBOLCUSTOM_CAMERACONTROL_GUNSITE,NULL,0, 
				&psprop, myLen);

			if(!SUCCEEDED(hr))
			{
#ifdef DEBUG_MODE
				//	LoadString(hInst, ERR_GUN_SET, text, MAX_LOADSTRING);
				//	OutputDebugString(text);
#endif
			}
			hr = pKs->Release();          
		}
		//free resources
		m_pCamCtrl->Release();
	}
	else
	{
		OutputDebugString(L"\n!!!! Error Query IID_IAMCameraControl !!!! \n");
	}
#endif
	return hr;
}// end GunSet

//------------------------------------------------------------------
//
// Prototype:	void DisableMPAPowerKey(BOOL bDisable)
//
// Description:	This will disable the power button
//
// Parameters:	bool bDisable, value to indicate whether to  enable/disable power key
//
// Returns:	void
// 
// Notes:
//------------------------------------------------------------------
void DisableMPAPowerKey(HKEY hKey, BOOL bDisable)
{
    DWORD dwResult;
    HANDLE hEvent = NULL;

    hEvent = CreateEvent(NULL, FALSE, FALSE, POWER_REFRESHEVENT_MPA);

    if (NULL != hEvent)
	{
		dwResult = RegSetValueEx(hKey, POWER_KEY_REG_MPA, 0, REG_DWORD,(LPBYTE)&bDisable, sizeof(DWORD));
		if (ERROR_SUCCESS == dwResult)
		{
			SetEvent(hEvent);
		}
		
		CloseHandle(hEvent);
		hEvent = NULL;
		
	}
}
//------------------------------------------------------------------
//
// Prototype:	void EnablePowerKeyMPA(BOOL bEnablePowerKey)
//
// Description:	On disabling this will register a reg notifcation and disable the power key
//				On enabling this will unregister the previously registerd notification and re-enable the power key
//
// Parameters:	BOOL bEnablePowerKey, value to indicate whether to  enable/disable power key
//
// Returns:	void
// 
// Notes:
//------------------------------------------------------------------
void CDShowCamModule::EnablePowerKeyMPA(BOOL bEnablePowerKey)
{

	HKEY hKey = NULL;
	DWORD dwValue = DISABLE_POWER_KEY_VALUE;

	if (S_OK == RegOpenKeyEx(HKEY_LOCAL_MACHINE, POWER_KEY_REG_ROOT_MPA, 0,  0, &hKey))
	{
		     if(FALSE != bEnablePowerKey)//Enable power key
			 {
				 dwValue = ENABLE_POWER_KEY_VALUE;
				 //On Enabling the power key, remove the notifcation broker
				 if(NULL != g_hPowerKeyRegNotify)
				 {
					 RegistryCloseNotification(g_hPowerKeyRegNotify);
					 g_hPowerKeyRegNotify = NULL;
				 }
			 }

			 DisableMPAPowerKey(hKey, !bEnablePowerKey);

			  //register reg callback incase user or other program re-enable the power key while in camera demo
			 if(FALSE == bEnablePowerKey)
			 {
				 RegistryNotifyCallback(hKey, 
										NULL,
										 POWER_KEY_REG_MPA,
										 &CDShowCamModule::RegistryNotifyCallbackFuncMPA,
										 0,
										 NULL,
										 &g_hPowerKeyRegNotify);
			 }
			 RegCloseKey(hKey);
			
	}
}

//------------------------------------------------------------------
//
// Prototype:	void EnablePowerKey(bool bEnablePowerKey)
//
// Description:	On disabling this will register a reg notifcation and disable the power key
//				On enabling this will unregister the previously registerd notification and re-enable the power key
//
// Parameters:	bool bEnablePowerKey, value to indicate whether to  enable/disable power key
//
// Returns:	void
// 
// Notes:
//------------------------------------------------------------------
void CDShowCamModule::EnablePowerKeyIAC(BOOL bEnablePowerKey)
{
	HKEY hKey = NULL;
	DWORD dwValue = DISABLE_POWER_KEY_VALUE;

	if (S_OK == RegOpenKeyEx(HKEY_LOCAL_MACHINE, POWER_KEY_REG_ROOT_IAC, 0,  0, &hKey))
	{
	     if(FALSE != bEnablePowerKey)//Enable power key
		 {
			 dwValue = ENABLE_POWER_KEY_VALUE;
			 //On Enabling the power key, remove the notifcation broker
			 if(NULL != g_hPowerKeyRegNotify)
			 {
				 RegistryCloseNotification(g_hPowerKeyRegNotify);
				 g_hPowerKeyRegNotify = NULL;
			 }
		 }

		 if(ERROR_SUCCESS == RegSetValueEx(hKey, POWER_KEY_REG_IAC, 0, REG_DWORD, (BYTE*)&dwValue, sizeof(DWORD)))
		 {
		 	 //register reg callback incase user or other program re-enable the power key while in camera demo
			 if(FALSE == bEnablePowerKey)
			 {
				 RegistryNotifyCallback(hKey, 
											 NULL,
											 POWER_KEY_REG_IAC,
											 &CDShowCamModule::RegistryNotifyCallbackFuncIAC,
											 0,
											 NULL,
											 &g_hPowerKeyRegNotify);
			 }
		 }
		 RegCloseKey(hKey);
	}

}
//------------------------------------------------------------------
//
// Prototype:	BOOL CDShowCamModule::IsIACDEvice(void)
//
// Description:	Determines whether the device is a IAC device(ES400 Or MC65)
//				
//
// Parameters:	void
//
// Returns:	BOOL
// 
// Notes:
//------------------------------------------------------------------
BOOL CDShowCamModule::IsIACDEvice(void)
{
	BOOL bIACDevice = FALSE;
	// code added to get the device name - to fix the suspend resume issue
	TCHAR szOemModelName[MODEL_NAME_MAX_LENGTH] = {0};
	const TCHAR szES400[] = TEXT("ES400");
	const TCHAR szMC65[] = TEXT("MC65");
	const TCHAR szMC45[] = TEXT("MC45");
	SystemParametersInfo(SPI_GETOEMINFO, MODEL_NAME_MAX_LENGTH, szOemModelName, 0);

	if(_tcsstr(szOemModelName,szES400) != NULL || _tcsstr(szOemModelName, szMC65) != NULL || _tcsstr(szOemModelName, szMC45) != NULL)
	{
		bIACDevice = TRUE;
	}

	return bIACDevice;
}

//------------------------------------------------------------------
//
// Prototype:	void RegistryNotifyCallbackFunc(HREGNOTIFY hNotify, DWORD dwUserData, const PBYTE pData, const UINT cbData)
//
// Description:	Register notifcation callback for HKLM\Drivers\BuiltIn\Remkeys\PwrKeySuspendDisabled to disable the power key
//				on user or other program  changes this registery directly or indirectly.
//				
//
// Parameters:	HREGNOTIFY hNotify, DWORD dwUserData, const PBYTE pData, const UINT cbData
//
// Returns:	void
// 
// Notes:
//------------------------------------------------------------------
void CDShowCamModule::RegistryNotifyCallbackFuncMPA(HREGNOTIFY hNotify, DWORD dwUserData, const PBYTE pData, const UINT cbData)
{
 //todo - open the registry key here
	DWORD dwValue = (DWORD)*pData;
	HKEY hKey;
	if(ENABLE_POWER_KEY_VALUE == dwValue)//Power button has been re-enabled
	{
		if(S_OK == RegOpenKeyEx(HKEY_LOCAL_MACHINE, POWER_KEY_REG_ROOT_MPA, 0,  0, &hKey))
		{
			DisableMPAPowerKey(hKey, !dwValue);
			RegCloseKey(hKey);
		}
		
	}
}

//------------------------------------------------------------------
//
// Prototype:	void RegistryNotifyCallbackFunc(HREGNOTIFY hNotify, DWORD dwUserData, const PBYTE pData, const UINT cbData)
//
// Description:	Register notifcation callback for HKLM\Drivers\BuiltIn\Remkeys\PwrKeySuspendDisabled to disable the power key
//				on user or other program  changes this registery directly or indirectly.
//				
//
// Parameters:	HREGNOTIFY hNotify, DWORD dwUserData, const PBYTE pData, const UINT cbData
//
// Returns:	void
// 
// Notes:
//------------------------------------------------------------------
void CDShowCamModule::RegistryNotifyCallbackFuncIAC(HREGNOTIFY hNotify, DWORD dwUserData, const PBYTE pData, const UINT cbData)
{
	DWORD dwValue = (DWORD)*pData;
	if(ENABLE_POWER_KEY_VALUE == dwValue)//Power button has been re-enabled
	{	
		HKEY hKey = NULL;
		dwValue = DISABLE_POWER_KEY_VALUE;
		//Have to disable the power button again
		if (S_OK == RegOpenKeyEx(HKEY_LOCAL_MACHINE, POWER_KEY_REG_ROOT_IAC, 0,  0, &hKey))
		{
			RegSetValueEx(hKey, POWER_KEY_REG_IAC, 0, REG_DWORD, (BYTE*)&dwValue, sizeof(DWORD));   
			RegCloseKey(hKey);
		}
	}
	
}

DSHOWCAM_API void GetResolution(std::vector<ImageRes>& supportedRes, wchar_t* camId, PinType ePType)
{


	ImageRes res;
	res.nWidth = 352;
	res.nHeight = 288;
	supportedRes.push_back(res);
	res.nWidth = 640;
	res.nHeight = 480;
	supportedRes.push_back(res);
	res.nWidth = 800;
	res.nHeight = 600;
	supportedRes.push_back(res);
	//res.nWidth = 1280;
	//res.nHeight = 1024;
	//supportedRes.push_back(res);
	//HRESULT hr;
	//CComPtr<ICaptureGraphBuilder2>  pCaptureGraphBuilder;
	//CComPtr<IBaseFilter>            pVideoCap;
	//CComPtr<IDMOWrapperFilter>      pVideoWrapperFilter;
	//CComPtr<IPersistPropertyBag>    pPropertyBag;
	//CComPtr<IGraphBuilder>          pGraph;
	//CComPtr<IMediaControl>          pMediaControl;
	//CComPtr<IMediaEvent>            pMediaEvent;
	//CComPtr<IMediaSeeking>          pMediaSeeking;
	//CComPtr<IVideoWindow>           pVideoWindow;
	//CComPtr<IBaseFilter>            pStillSink;
	//CComPtr<IBaseFilter>            pVideoRender;
	//CComPtr<IBaseFilter>            pVideoEncoder;
	//CComPtr<IBaseFilter>            pMux;
	//CComPtr<IFileSinkFilter>        pFileSink;
	//CComPtr<IAMStreamConfig>        pStrConf;

 //


	//hr = pCaptureGraphBuilder.CoCreateInstance( CLSID_CaptureGraphBuilder );
	//if(SUCCEEDED(hr))
	//{
	//	hr = pGraph.CoCreateInstance( CLSID_FilterGraph );
	//	if(SUCCEEDED(hr))
	//	{
	//		hr = pCaptureGraphBuilder->SetFiltergraph( pGraph );
	//		if(SUCCEEDED(hr))
	//		{
	//			// Query all the interfaces needed later
	//			hr = pGraph.QueryInterface( &pMediaControl );
	//			if(SUCCEEDED(hr))
	//			{
	//				hr = pGraph.QueryInterface( &pMediaEvent );
	//				if(SUCCEEDED(hr))
	//				{
	//					pGraph.QueryInterface( &pMediaSeeking );
	//					if(SUCCEEDED(hr))
	//					{
	//						CComVariant varCamName;
	//						VariantInit(&varCamName);
	//						varCamName = camId;
	//						
	//						// Initialize the video capture filter
	//						hr = pVideoCap.CoCreateInstance( CLSID_VideoCapture );	
	//						if(SUCCEEDED(hr))
	//						{
	//							hr = pVideoCap.QueryInterface( &pPropertyBag );
	//							if(SUCCEEDED(hr))
	//							{
	//								CPropertyBag  PropBag;
	//								hr = PropBag.Write( L"VCapName", &varCamName );   
	//								if(SUCCEEDED(hr))
	//								{				
	//									hr = pPropertyBag->Load( &PropBag, NULL );
	//									if(SUCCEEDED(hr))
	//									{
	//										pPropertyBag.Release();
	//										hr = pGraph->AddFilter( pVideoCap, L"Video capture source" );
	//										if(SUCCEEDED(hr))
	//										{
	//											GUID pType;

	//											switch(ePType)
	//											{
	//											case V:
	//												pType = PIN_CATEGORY_CAPTURE;			
	//												break;
	//											case S:
	//												pType = PIN_CATEGORY_STILL;			
	//												break;
	//											case P:
	//												pType = PIN_CATEGORY_PREVIEW;			
	//												break;

	//											}
	//											hr = pCaptureGraphBuilder->FindInterface(&pType, 0, pVideoCap, IID_IAMStreamConfig, (void**)&pStrConf);


	//											if(SUCCEEDED(hr))
	//											{
	//												VIDEO_STREAM_CONFIG_CAPS scc;
	//												AM_MEDIA_TYPE *pmtConfig;
	//												INT iResIdx=0;

	//												INT nCount = 0, nSize = 0;

	//												hr = pStrConf->GetNumberOfCapabilities(&nCount, &nSize);
	//												if(SUCCEEDED(hr))
	//												{
	//												


	//													for(int i=iResIdx; i<nCount;i++)
	//													{
	//														hr = pStrConf->GetStreamCaps(i, &pmtConfig, (BYTE*)&scc);

	//														if (SUCCEEDED(hr))
	//														{
	//															ImageRes res;
	//															//EMBPD00164210
	//															//some camera hardware provides resolution as -ve values
	//															//correct it before sending it to user
	//															res.nHeight = scc.MaxOutputSize.cy >0 ? scc.MaxOutputSize.cy : -(scc.MaxOutputSize.cy);
	//															res.nWidth = scc.MaxOutputSize.cx >0 ? scc.MaxOutputSize.cx : -(scc.MaxOutputSize.cx) ;
	//															supportedRes.push_back(res);
	//															DeleteMediaType(pmtConfig);
	//															if(res.nHeight == 600 && res.nWidth ==800)
	//															{
	//																break;//support a maximum resolution of 800X600
	//															}
	//																

	//														}
	//														

	//													}
	//												}

	//											}

	//										}
	//									}
	//								}
	//							}
	//						}
	//					}
	//				}
	//			}
	//		}
	//	}
	//}


}

