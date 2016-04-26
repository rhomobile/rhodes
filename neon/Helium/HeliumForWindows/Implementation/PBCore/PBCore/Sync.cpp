#include "StdAfx.h"
#include "Sync.h"

extern BOOL Log			(LogTypeInterface logSeverity,LPCTSTR pLogComment, 
						LPCTSTR pFunctionName, DWORD dwLineNumber,LPCTSTR pCallingModule);

/*********************************************************************************************/
CSync::CSync(void)
{
	m_pHeadEventRec = NULL;
	m_pHeadCallback	= NULL;
	m_lRegLock = 0;
	memset(m_ArrEventRec,0,sizeof(m_ArrEventRec));
	m_hDispatch = CreateThread(NULL,NULL,CSync::DispatchThread,this,NULL,&m_DispatchID);
}
/*********************************************************************************************/
//initialise any callback functions
void CSync::InitCB(PPBCORESTRUCT pbCoreStruct)
{
	m_pbCoreStruct = pbCoreStruct;
}
/*********************************************************************************************/
CSync::~CSync(void)
{
	//Quit the Dispatch Thread
	PostThreadMessage(m_DispatchID,THREAD_QUIT,0,0);

	//Close all Event Handles
	for(int iIndex = 0;iIndex < SYNCARRAYSIZE;iIndex++)
	{
		if(m_ArrEventRec[iIndex].hEvTrigger)
		{
			CloseHandle(m_ArrEventRec[iIndex].hEvTrigger);
			m_ArrEventRec[iIndex].hEvTrigger = NULL;
		}
	}

	//Check if Thread Has Quit
	if (WaitForSingleObject(m_hDispatch,1000) == WAIT_TIMEOUT) TerminateThread(m_hDispatch,0xDEADBEEF);
	CloseHandle(m_hDispatch);

	//Clean up Registration List
	if(m_pHeadCallback) m_pHeadCallback = deletemem(m_pHeadCallback);
}
/*********************************************************************************************/
plgCallback *CSync::deletemem(plgCallback *pCallbackRec)
{
	if(pCallbackRec->pNext)	return deletemem(pCallbackRec->pNext);
	delete pCallbackRec;
	return NULL;
}
/*********************************************************************************************/
BOOL CSync::RegisterCallback(PBModule *pObj,PB_SyncEvent dwEvent,LPCTSTR pCallingModule)
{
	plgCallback *pRec = NULL;
	plgCallback *pCurrent = NULL;
	
	//Sanity Checks
	if(pObj == NULL)  return FALSE;
	if(dwEvent < 0 ) return FALSE;
	if(m_ArrEventRec[dwEvent].hEvTrigger == NULL) return FALSE;

	//Create Node
	pRec = new plgCallback;
	if (!pRec) return FALSE;
	ZeroMemory(pRec,sizeof(plgCallback));

	//Wait for access to list
	while (InterlockedExchange(&m_lRegLock,1) == 1) Sleep(0);

	//Add to List
	if(m_pHeadCallback == NULL)
	{
		m_pHeadCallback = pRec;
	}
	else
	{
		pCurrent = m_pHeadCallback;
		while (pCurrent)
		{
			//This Module ID
			if(pObj == pCurrent->pObj)
			{
				delete pRec;
				pRec=pCurrent;
				break;
			}
			//End of List
			if (pCurrent->pNext == NULL)
			{
				pCurrent->pNext = pRec;
				break;
			}
			pCurrent = pCurrent->pNext;
		}
	}
	//Add Details to Node
	pRec->pObj = pObj;
	pRec->pModName = pCallingModule;
	pRec->dwRegister = pRec->dwRegister | (1<<(dwEvent-1));

	//Free List Lock
	InterlockedExchange(&m_lRegLock,0);
	Log(PB_LOG_INFO,L"Registered Callback",L"CSync::RegisterCallback",__LINE__,L"Core");
	return TRUE;
}
/*********************************************************************************************/
BOOL CSync::UnRegisterCallback (PBModule *pmodule, DWORD event_id)
{
	plgCallback *pprevious, *pentry;
	pprevious = NULL;
	BOOL	bRes = FALSE;

	//Wait for access to list
	while (InterlockedExchange(&m_lRegLock,1) == 1) Sleep(0);

	//Find Entry
	pentry = m_pHeadCallback;

	while (pentry)
	{
		if (pentry->pObj == pmodule) break;
		pprevious = pentry;
		pentry = pentry->pNext;
	}

	// Check we found it
	if (pentry)
	{
		// Remove this event from registered bit field
		DWORD mask = 1 << (event_id - 1);
		pentry->dwRegister &= ~mask;

		// Any callbacks still registered for module?
		if (pentry->dwRegister == 0)
		{
			// No, remove the record from the list
			if (pprevious)
			{
				pprevious->pNext = pentry->pNext;
			}
			else
			{
				if (pentry == m_pHeadCallback) m_pHeadCallback = pentry->pNext;
			}
			// Delete the record itself
			delete pentry;
		}
		bRes = TRUE;
	}

	//Free List Lock
	InterlockedExchange(&m_lRegLock,0);
	return bRes;	
}
/*********************************************************************************************/
//returns the event that will signal when all modules functions have returned
HANDLE CSync::CreateSyncEvent(int iEvent)
{
	EventRecord *pEventRec = &m_ArrEventRec[iEvent];

	if(pEventRec->hEvTrigger == NULL)
	{
		pEventRec->dwEventType = 1<<(iEvent-1);
		pEventRec->hEvTrigger = CreateEvent(NULL,TRUE,FALSE,NULL);
		return pEventRec->hEvTrigger;
	}
	return NULL; 
}
/*********************************************************************************************/
//run synchronously
BOOL CSync::RunSync(int iEvent,LPARAM lParam1,LPARAM lParam2)
{
	EventRecord *pRec = &m_ArrEventRec[iEvent];
	plgCallback *pCBRec;
	
	if(pRec->hEvTrigger == NULL)
	{
		Log(PB_LOG_ERROR,L"Event not set",L"CSync::RunSync",__LINE__,L"Core");
		return FALSE;//no event has been set 
	}		
	
	if(m_pHeadCallback){
		
		ResetEvent(pRec->hEvTrigger);//set the event to non signalled

		//go through each registered callback record 
		for(pCBRec = m_pHeadCallback;pCBRec;pCBRec = pCBRec->pNext)
		{
			
			switch(iEvent)
			{
			case PB_APPFOCUSEVENT:
				//check for a match against each record
				if(pCBRec->dwRegister & pRec->dwEventType){//if this event is to be handled by this callback 
					pCBRec->pObj->onAppFocus((int)lParam1 ,(int)lParam2);
					
				}
				break;
			
			case PB_BROWSER_BEFORE_NAV_EVENT:
				//check for a match against each record
				if(pCBRec->dwRegister & pRec->dwEventType){
					pCBRec->pObj->onBeforeNavigate((int)lParam1);
				}
				break;
			case PB_BROWSER_NAV_COMPLETE_EVENT:
				//check for a match against each record
				if(pCBRec->dwRegister & pRec->dwEventType){
					pCBRec->pObj->onNavigateComplete((int)lParam1);
				}
				break;
			case PB_BROWSER_DOC_COMPLETE_EVENT:
				//check for a match against each record
				if(pCBRec->dwRegister & pRec->dwEventType){
					pCBRec->pObj->onDocumentComplete((int)lParam1);
				}
				break;
			case PB_BROWSER_NAV_ERROR_EVENT:
				//check for a match against each record
				if(pCBRec->dwRegister & pRec->dwEventType){
					pCBRec->pObj->onNavigateError((int)lParam1);
				}
				break;
			case PB_WINDOWCHANGED_EVENT:
				//check for a match against each record
				if(pCBRec->dwRegister & pRec->dwEventType){
					pCBRec->pObj->onWindowChanged((int)lParam1);
				}
				break;
			case RHO_APPFOCUS_EVENT:
				//check for a match against each record
				if(pCBRec->dwRegister & pRec->dwEventType){
					pCBRec->pObj->onRhoAppFocus(lParam1 > 0, (int)lParam2);
				}
				break;
			
			
			}
		}
		SetEvent(pRec->hEvTrigger);
		return TRUE;//success
	}
	return FALSE;//error
}
/*********************************************************************************************/
BOOL CSync::Run(int iEvent,LPARAM lParam1,LPARAM lParam2)
{
	EventRecord *pRec = &m_ArrEventRec[iEvent];	
	
	Log(PB_LOG_INFO,L"Running Event: ",	L"CSync::Run",iEvent,L"Core");
	if(pRec->hEvTrigger == NULL)
	{
		Log(PB_LOG_ERROR,L"Event not set",L"CSync::Run",__LINE__,L"Core");
		return FALSE;
	}

	//Any Registration list
	if(m_pHeadCallback)
	{
		ResetEvent(pRec->hEvTrigger);
		syncThreadParam *pSyncParam = new syncThreadParam;
		if(pSyncParam == NULL)
		{
			Log(PB_LOG_ERROR,L"Mem Alloc error",L"CSync::Run",__LINE__,L"Core");
			SetEvent(pRec->hEvTrigger);//set the event to signalled so's not to hold up any threads
			return FALSE;
		}
		//Fill in Parameters
		pSyncParam->lParam1 = lParam1;
		pSyncParam->lParam2 = lParam2;
		pSyncParam->iType = iEvent;
		Log(PB_LOG_INFO,L"Posting Message for: ",	L"CSync::Run",pRec->dwEventType,L"Core");
		if (!PostThreadMessage(m_DispatchID,THREAD_EVENT,0,(LPARAM)pSyncParam))
		{
			Log(PB_LOG_INFO,L"Failed Posting Message for: ",	L"CSync::Run",pRec->dwEventType,L"Core");
			delete pSyncParam;
			return FALSE;
		}
	}
	else
	{
		Log(PB_LOG_INFO,L"No Registrations yet",	L"CSync::Run",__LINE__,L"Core");
	}
	SetEvent(pRec->hEvTrigger);//set the event to signalled so's not to hold up any threads
	return FALSE;
}
/*********************************************************************************************/
DWORD WINAPI CSync::DispatchThread(LPVOID lParam)
{
	CSync *lpCSync = (CSync *) lParam;
	return lpCSync->DispatchThreadImp();
}
/*********************************************************************************************/
DWORD CSync::DispatchThreadImp()
{
	MSG		Msg = {0};
	BOOL	bQuit = FALSE;

	//Wait For Thread Message
	while (!bQuit && GetMessage(&Msg,NULL,NULL,NULL))
	{
		switch (Msg.message)
		{
			//Quit The Thread
			case THREAD_QUIT:
				bQuit = TRUE;
				break;

			//Process Thread Event
			case THREAD_EVENT:
				while (InterlockedExchange(&m_lRegLock,1) == 1) Sleep(0);			//Wait for Registration List Access
				syncThreadParam *pParam = (syncThreadParam *)Msg.lParam;
				RunSync(pParam->iType,pParam->lParam1,pParam->lParam2);
				delete pParam;
				InterlockedExchange(&m_lRegLock,0);									//Free List Lock
				break;
		}
	}
	return 0;
}

