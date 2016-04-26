#include "MemoryModule.h"

/**
* \author	Harish Pulimi (HP, PXB743)
* \date		March 2012
*/
BOOL CMemoryModule::onInit(PPBSTRUCT pPBStructure)
{
	wcscpy (m_szModName, L"memory");
	RegisterForEvent(PB_BROWSER_BEFORE_NAV_EVENT);
	return TRUE;
}

/**
* \author	Harish Pulimi (HP, PXB743)
* \date		March 2012
*/
void CMemoryModule::onDeInit(PPBSTRUCT pPBStructure)
{
	UnRegisterForEvent(PB_BROWSER_BEFORE_NAV_EVENT);
}

/**
* \author	Harish Pulimi (HP, PXB743)
* \date		March 2012
*/
BOOL CMemoryModule::onAttachInstance(PPBSTRUCT pPBStructure, PPBINSTSTRUCT pInstStruct)
{
	//  Create this module's instance data
	INSTANCE_DATA *pData = new INSTANCE_DATA;
	pData->instanceID = pInstStruct->instID;
	pData->hMemoryCancel = CreateEvent(NULL, FALSE, FALSE, NULL);
	//  Default timeout value is 5 seconds
	pData->iMemoryRefreshInterval = 5000;
	memset(pData->tcMemoryNavigate, 0, sizeof(pData->tcMemoryNavigate));
	wcscpy(pData->tcMemoryNavigate, L"");
	pData->hMemoryThread = NULL;
	pData->bThreadRunning = FALSE;
	pData->sMemStat.dwLength = sizeof(MEMORYSTATUS);
	GlobalMemoryStatus(&pData->sMemStat); 	
	pData->sMemStat.dwTotalPhys /= DIV;
	pData->llLowMemThreshold = pData->sMemStat.dwTotalPhys / 10; //Default: 10% of the total memory
	pInstStruct->pWrappedObj = pData;
	return TRUE;
}

/**
* \author	Harish Pulimi (HP, PXB743)
* \date		March 2012
*/
BOOL CMemoryModule::onReleaseInstance(PPBSTRUCT pPBStructure, PPBINSTSTRUCT pInstStruct)
{
	INSTANCE_DATA *pData = (INSTANCE_DATA*) (pInstStruct->pWrappedObj);
	if (pData)
	{
		StopMemory(pData);
		CloseHandle(pData->hMemoryCancel);
	}
	delete pData;
	
	return TRUE;
}

/**
* \author	Harish Pulimi (HP, PXB743)
* \date		March 2012
*/
BOOL CMemoryModule::onBeforeNavigate(int iInstID)
{
	INSTANCE_DATA *pData = NULL;
	pData = (INSTANCE_DATA*) GetObjFromID(iInstID);

	if (pData != NULL)
	{
		//  We have obtained a pointer to the appropriate INSTANCE_DATA object
		//  Stop the Memory if it is running
		StopMemory(pData);
		//  Set the Nav String to ""
		wcscpy(pData->tcMemoryNavigate, L"");
		return TRUE;
	}
	else
		return FALSE;
}

/**
* \author	Harish Pulimi (HP, PXB743)
* \date		March 2012
*/
BOOL CMemoryModule::MetaProc(PBMetaStruct *pbMetaStructure, PPBSTRUCT pPBStructure, void *pParam)
{
	INSTANCE_DATA *pData = (INSTANCE_DATA*) (pParam);
	if (!pData)
		return FALSE;	

	if (cmp (pbMetaStructure->lpParameter, L"lowMemThreshold"))
	{
		//  User has specified the memory Threshold value in KB, in the 
		//  Meta Proc Value
		long long llThreshold = _wtoll(pbMetaStructure->lpValue);
		//  Minimum allowable interval is 1500(KB)		
		if (llThreshold < 1500)
			llThreshold = 1500;
		//TODO: read the system registry value instead of this hard coded value
		pData->llLowMemThreshold = llThreshold;	

		//  Start the Memory Thread if it is not already running
		if (!pData->bThreadRunning)
		{
			CloseHandle(pData->hMemoryThread);
			pData->hMemoryThread = NULL;
			pData->bThreadRunning = TRUE;
			pData->hMemoryThread = CreateThread(NULL, 0, 
									(LPTHREAD_START_ROUTINE)MemoryThread, 
									this, 0, NULL);
		}
		else
		{
			Log(PB_LOG_INFO, 
				L"Unable to Start Memory thread, there is already a thread running", 
				_T(__FUNCTION__), __LINE__);
		}
	}	
	// Currently not exposed to the user
	else if (cmp(pbMetaStructure->lpParameter, L"memoryRefreshInterval"))
	{
		int iInterval = _wtoi(pbMetaStructure->lpValue);
		if (iInterval < 1000)
		{
			iInterval = 1000;
		}
		pData->iMemoryRefreshInterval = iInterval;	
	}
	else if (cmp(pbMetaStructure->lpParameter, L"memoryEvent"))
	{
		//  User has specified the Navigation URI to use when the memory event is called
		wcsncpy(pData->tcMemoryNavigate, pbMetaStructure->lpValue, MAX_URL-1);
	}
	else if (cmp(pbMetaStructure->lpParameter, L"getMemoryStats"))
	{
		if (pData->tcMemoryNavigate && *pData->tcMemoryNavigate)
		{
			//SendPBNavigate
			//Get the memory stats
			GlobalMemoryStatus(&pData->sMemStat);
			pData->sMemStat.dwTotalPhys /= DIV;
			pData->sMemStat.dwAvailPhys /= DIV;
			WCHAR wcTotal[100];
			WCHAR wcAvail[100];
			wsprintf(wcTotal, L"%u", pData->sMemStat.dwTotalPhys);
			wsprintf(wcAvail, L"%u", pData->sMemStat.dwAvailPhys);	
								
			SendPBNavigate(tcMemoryEventNames, pData->instanceID, 
				pData->tcMemoryNavigate, 
				wcTotal, wcAvail,															
				NULL);
		}
		else
		{
			//TODO: raise an error
		}
	}	
	else
	{
		// Unrecognised tag
		Log(PB_LOG_WARNING, L"Unrecognised Meta Tag Provided to Memory notification module",
			_T(__FUNCTION__), __LINE__);
		return FALSE;
	}

	return TRUE;
}

/**
* \author	Harish Pulimi (HP, PXB743)
* \date		March 2012
*/
BOOL CMemoryModule::StopMemory(INSTANCE_DATA* pData)
{
	if (pData->bThreadRunning)
	{
		SetEvent(pData->hMemoryCancel);	
		if (WaitForSingleObject(pData->hMemoryThread, 250) == WAIT_TIMEOUT)
		{
			//  Thread failed to shut down correctly, terminate it
			TerminateThread(pData->hMemoryThread, 0);
			pData->bThreadRunning = FALSE;
		}
		CloseHandle(pData->hMemoryThread);
		pData->hMemoryThread = NULL;
	}	
	return TRUE;
}

DWORD CMemoryModule::MemoryThread(LPVOID lParam)
{
	//  lParam holds the Instance data associated with this Memory
	CMemoryModule* pModule = (CMemoryModule*)lParam;
	if (!pModule)
		return 0;

	INSTANCE_DATA* pData = 
		(INSTANCE_DATA*) pModule->GetObjFromID(pModule->m_pCurrentPBstruct->iTabID);	

	DWORD dwWaitReturnValue;
	dwWaitReturnValue = 
		WaitForSingleObject(pData->hMemoryCancel, 
							pData->iMemoryRefreshInterval);
	while (dwWaitReturnValue != WAIT_OBJECT_0)
	{
		switch (dwWaitReturnValue)
		{
			case WAIT_TIMEOUT:
				{
					//  We have Timed out
					//  Send PBNavigate back to the core if there is a registered URI
					if (*pData->tcMemoryNavigate)
					{
						//Get the memory stats
						GlobalMemoryStatus(&pData->sMemStat);
						pData->sMemStat.dwTotalPhys /= DIV;
						pData->sMemStat.dwAvailPhys /= DIV;				

						if (pData->sMemStat.dwAvailPhys < pData->llLowMemThreshold)
						{
							WCHAR wcTotal[100];
							WCHAR wcAvail[100];
							wsprintf(wcTotal, L"%u", pData->sMemStat.dwTotalPhys);
							wsprintf(wcAvail, L"%u", pData->sMemStat.dwAvailPhys);	

							pModule->SendPBNavigate(tcMemoryEventNames, pData->instanceID, 
								pData->tcMemoryNavigate, 
								wcTotal, wcAvail,															
								NULL);
						}
						else
						{
							//ignore
						}
					}
					else
					{
						//do nothing??
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
		dwWaitReturnValue = WaitForSingleObject(pData->hMemoryCancel, 
							pData->iMemoryRefreshInterval);
	}

	pData->bThreadRunning = FALSE;
	return 0;
}