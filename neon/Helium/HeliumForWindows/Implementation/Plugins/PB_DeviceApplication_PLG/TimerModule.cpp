#include "TimerModule.h"

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		November 2009
*/
BOOL CTimerModule::onInit(PPBSTRUCT pPBStructure)
{
	wcscpy (m_szModName, L"Timer");
	RegisterForEvent(PB_BROWSER_BEFORE_NAV_EVENT);
	return TRUE;
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		November 2009
*/
void CTimerModule::onDeInit(PPBSTRUCT pPBStructure)
{
	UnRegisterForEvent(PB_BROWSER_BEFORE_NAV_EVENT);
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		November 2009
*/
BOOL CTimerModule::onAttachInstance(PPBSTRUCT pPBStructure, PPBINSTSTRUCT pInstStruct)
{
	//  Create this module's instance data
	INSTANCE_DATA *pData = new INSTANCE_DATA;
	pData->instanceID = pInstStruct->instID;
	pData->hTimerCancel = CreateEvent(NULL, FALSE, FALSE, NULL);
	//  Default timeout value is 1 second
	pData->iTimeoutDuration = 1000;
	memset(pData->tcTimerNavigate, 0, sizeof(pData->tcTimerNavigate));
	wcscpy(pData->tcTimerNavigate, L"");
	pData->hTimerThread = NULL;
	pData->bTimerRunning = FALSE;
	pInstStruct->pWrappedObj = pData;
	return TRUE;
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		November 2009
*/
BOOL CTimerModule::onReleaseInstance(PPBSTRUCT pPBStructure, PPBINSTSTRUCT pInstStruct)
{
	INSTANCE_DATA *pData = (INSTANCE_DATA*) (pInstStruct->pWrappedObj);
	if (pData)
	{
		StopTimer(pData);
		CloseHandle(pData->hTimerCancel);
	}
	delete pData;
	
	return TRUE;
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		November 2009
*/
BOOL CTimerModule::onBeforeNavigate(int iInstID)
{
	INSTANCE_DATA *pData = NULL;
	pData = (INSTANCE_DATA*) GetObjFromID(iInstID);

	if (pData != NULL)
	{
		//  We have obtained a pointer to the appropriate INSTANCE_DATA object
		//  Stop the Timer if it is running
		StopTimer(pData);
		//  Set the Nav String to ""
		wcscpy(pData->tcTimerNavigate, L"");
		return TRUE;
	}
	else
		return FALSE;
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		November 2009
*/
BOOL CTimerModule::MetaProc(PBMetaStruct *pbMetaStructure, PPBSTRUCT pPBStructure, void *pParam)
{
	INSTANCE_DATA *pData = (INSTANCE_DATA*) (pParam);
	if (!pData)
		return FALSE;

	if (cmp (pbMetaStructure->lpParameter, L"Interval"))
	{
		//  User has specified the timeout value, in milliseconds, in the 
		//  Meta Proc Value
		int iIntervalDuration = _wtoi(pbMetaStructure->lpValue);
		//  Minimum allowable interval is 500ms
		if (iIntervalDuration < 500)
			iIntervalDuration = 500;
		pData->iTimeoutDuration = iIntervalDuration;
	}
	else if (cmp(pbMetaStructure->lpParameter, L"Timeout"))
	{
		//  User has specified the Navigation URI to use when the timer times out
		wcsncpy(pData->tcTimerNavigate, pbMetaStructure->lpValue, MAX_URL-1);
	}
	else if (cmp(pbMetaStructure->lpParameter, L"Start"))
	{
		//  In order to confiorm with with EMML1.0 we may receive 
		//  Timer with Content=Start and Value=Navigation URL
		//  Therefore if we have a value here set the URL
		if (pbMetaStructure->lpValue && *pbMetaStructure->lpValue)
		{
			//  Value will either be 'KillTimer' or the URI.
			if (_wcsicmp(pbMetaStructure->lpValue, L"KillTimer") == 0)
			{
				//  Stop the Timer
				return StopTimer(pData);
			}
			else
			{
				//  Value is a URI, Set it
				wcsncpy(pData->tcTimerNavigate, pbMetaStructure->lpValue, MAX_URL-1);
			}
		}

		//  Start the Timer Thread if it is not already running
		if (!pData->bTimerRunning)
		{
			CloseHandle(pData->hTimerThread);
			pData->hTimerThread = NULL;
			pData->bTimerRunning = TRUE;
			pData->hTimerThread = CreateThread(NULL, 0, 
									(LPTHREAD_START_ROUTINE)TimerThread, 
									this, 0, NULL);
		}
		else
		{
			Log(PB_LOG_INFO, 
				L"Unable to Start timer, there is already a timer running", 
				_T(__FUNCTION__), __LINE__);
		}
	}
	else if (cmp(pbMetaStructure->lpParameter, L"Stop"))
	{
		//  Stop the thread if it is running
		return StopTimer(pData);
	}
	else
	{
		// Unrecognised tag
		Log(PB_LOG_WARNING, L"Unrecognised Meta Tag Provided to Timer Module",
			_T(__FUNCTION__), __LINE__);
		return FALSE;
	}

	return TRUE;
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		November 2009
*/
BOOL CTimerModule::StopTimer(INSTANCE_DATA* pData)
{
	if (pData->bTimerRunning)
	{
		SetEvent(pData->hTimerCancel);	
		if (WaitForSingleObject(pData->hTimerThread, 250) == WAIT_TIMEOUT)
		{
			//  Thread failed to shut down correctly, terminate it
			TerminateThread(pData->hTimerThread, 0);
			pData->bTimerRunning = FALSE;
		}
		CloseHandle(pData->hTimerThread);
		pData->hTimerThread = NULL;
	}	
	return TRUE;
}

DWORD CTimerModule::TimerThread(LPVOID lParam)
{
	//  lParam holds the Instance data associated with this Timer
	CTimerModule* pModule = (CTimerModule*)lParam;
	if (!pModule)
		return 0;

	INSTANCE_DATA* pData = 
		(INSTANCE_DATA*) pModule->GetObjFromID(pModule->m_pCurrentPBstruct->iTabID);

	DWORD dwWaitReturnValue;
	dwWaitReturnValue = 
		WaitForSingleObject(pData->hTimerCancel, 
							pData->iTimeoutDuration);
	switch (dwWaitReturnValue)
	{
	case WAIT_TIMEOUT:
		{
			//  We have Timed out
			//  Send PBNavigate back to the core if there is a registered URI
			if (*pData->tcTimerNavigate)
			{
				TCHAR tcTime[100];
				SYSTEMTIME SystemTime;
				GetLocalTime( &SystemTime );
				wsprintf(tcTime, L"%02d/%02d/%04d %02d:%02d:%02d"
						,SystemTime.wDay
						,SystemTime.wMonth
						,SystemTime.wYear
						,SystemTime.wHour
						,SystemTime.wMinute
						,SystemTime.wSecond);
				pData->bTimerRunning = FALSE;
				pModule->SendPBNavigate(tcTimeoutEventNames, pData->instanceID, 
								pData->tcTimerNavigate, 
								tcTime, NULL);
			}
			break;
		}
	case WAIT_OBJECT_0:
		{
			//  The thread has been cancelled, quit
			break;
		}
	default:
		{
			//  Some other error occurred during the wait, quit
			break;
		}
	}
	pData->bTimerRunning = FALSE;
	return 0;
}