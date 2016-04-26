#include "SyncMsg.h"

extern BOOL Log			(LogTypeInterface logSeverity,LPCTSTR pLogComment, 
						LPCTSTR pFunctionName, DWORD dwLineNumber,LPCTSTR pCallingModule);

CSyncMsg::CSyncMsg(void)
{
	m_pCB	= NULL;
}

CSyncMsg::~CSyncMsg(void)
{
	m_pCB = deletemem(m_pCB);
}


plgCB *CSyncMsg::deletemem(plgCB *pCallbackRec)
{
	// Return if nothing to do
	if (!pCallbackRec)
		return NULL;

	// Delete the next item in the list (OK if next is NULL)
	deletemem (pCallbackRec->pNext);

	// Delete this item
	delete pCallbackRec;

	return NULL;

	/*if(pCallbackRec){
		if(pCallbackRec->pNext)
			return deletemem(m_pCB->pNext);
		delete pCallbackRec;
	}
	return NULL;*/
}

BOOL CSyncMsg::UnRegisterCallback(PBModule	*pObj)
{
	// Check there's something in the list
	if (!m_pCB)
		return FALSE;

	
	
	// Find matching record in callback list for this module object
	// Remove event from registered bit field
	// If there are no more events registered then delete entry

	plgCB *pprevious, *pentry;

	// Find matching record in list, keeping track of previous list entry if any
	pprevious = NULL;

	for (pentry = m_pCB; pentry; pentry = pentry->pNext)
	{
		if (pentry->pObj == pObj)
			break;

		pprevious = pentry;
	}

	// Check we found it
	if (!pentry)
		return FALSE;

	
	
	// No, remove the record from the list
	if (pprevious)
		pprevious->pNext = pentry->pNext;
	else
	{ 
		if (pentry == m_pCB)
			m_pCB = pentry->pNext;
	}

	// Delete the record itself
	delete pentry;
	

	return TRUE;
}


BOOL CSyncMsg::RegisterCallback(PBModule *pObj,LPCTSTR pCallingModule)
{
	plgCB *pRec = NULL;
	if(pObj == NULL)
		return FALSE;
	
	
	if(m_pCB == NULL){//check for first record
		m_pCB = new plgCB;
		if(m_pCB == NULL){
			//mem error
			return FALSE;
		}
		pRec = m_pCB;
		memset(pRec,0,sizeof(plgCB));
	}
	else{//else check to see if already registered
		for(pRec = m_pCB;pRec;pRec = pRec->pNext)
		{
			if(pObj == pRec->pObj)
				break;//break if we find a match
						
			else if(pRec->pNext == NULL){
				pRec->pNext = new plgCB;
				if(pRec->pNext == NULL){
					Log(PB_LOG_ERROR,L"Could not create a callback record",L"CSync::RegisterCallback",__LINE__,L"Core");//mem error
					return FALSE;
				}
				pRec = pRec->pNext;
				memset(pRec,0,sizeof(plgCB));
				break;
			}
		}
		
	}
	
	pRec->pObj = pObj;
	pRec->pModName = pCallingModule;
	Log(PB_LOG_INFO,L"Registered Primary Callback",L"CSyncMsg::RegisterCallback",__LINE__,L"Core");
	return TRUE;
	
	
	
}

//run synchronously
BOOL CSyncMsg::RunSync(LPARAM lParam1,LPARAM lParam2)
{
	
	plgCB *pCBRec;
	
	if(m_pCB){
				
		//go through each registered callback record 
		for(pCBRec = m_pCB;pCBRec;pCBRec = pCBRec->pNext)
		{
			if(!pCBRec->pObj){
				return FALSE;//Error object pointer is NULL
			}
			if(pCBRec->pObj->onPrimaryMessage((MSG*)lParam1,*(int*)lParam2)){
				*(int*)lParam2 = PB_HANDLED;
					return TRUE;
			}
		}
		
		return TRUE;//success


	}
	return FALSE;//error

}