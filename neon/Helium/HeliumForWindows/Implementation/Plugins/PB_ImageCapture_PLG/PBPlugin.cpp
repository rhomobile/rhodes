#include "stdafx.h"

#include "../../common/public/pbplugin.h"
#include <windows.h>

#pragma once

#pragma region ClassMembers

////////////////////////////////////////////////////////////////////////////////
/// Function:		PBModule
/// Description:	Sets the Module count variable and copies the default name "Abstract" to the object identifier
/// Returns:		N/A
/// Author:			Paul Henderson
/// Date:			May 2009
////////////////////////////////////////////////////////////////////////////////
PBModule::PBModule()
{
	
	m_iLoadCount		= 0;

	m_pCurrentPBstruct	= NULL;///>used to hold the current structure for the global callback functions
	

	_tcscpy(m_szModName,L"Abstract");
	
	m_pInstHead			= NULL;
	m_hQuitEvent		= CreateEvent(NULL,TRUE,FALSE,NULL);
}

////////////////////////////////////////////////////////////////////////////////
/// Function:		~PBModule
/// Description:	Destructor not currently used
/// Returns:		N/A
/// Author:			Paul Henderson
/// Date:			May 2009
////////////////////////////////////////////////////////////////////////////////
PBModule::~PBModule()
{
	//delete the instance counting memory
	deleteInst(m_pInstHead);
	CloseHandle(m_hQuitEvent);
	
}


////////////////////////////////////////////////////////////////////////
/// Function:		cmp
/// Description:	compares to TCHARs - case insensitive
/// Returns:		TRUE if successful
/// Author:			Paul Henderson
/// Date:			September 2009
////////////////////////////////////////////////////////////////////////
BOOL PBModule::cmp(LPCTSTR tc1, LPCTSTR tc2)
{
	if (!tc1 || !tc2)
		return false;
	return !_wcsicmp(tc1, tc2);

}


BOOL PBModule::Init(PPBCORESTRUCT pPBCoreStructure)
{
	m_pCoreStructure = pPBCoreStructure;
	return m_pCoreStructure->pCallPlgMethod ? TRUE :FALSE;
	
}

////////////////////////////////////////////////////////////////////////
/// Function:		Preload
/// Description:	Sets up the function pointers from the core,
///					maintains a module count and calls onInit() when called for the first time
/// Returns:		An enumeration type defined in PBPlugin.h
/// Author:			Paul Henderson
/// Date:			May 2009
////////////////////////////////////////////////////////////////////////
BOOL PBModule::Preload(PPBSTRUCT pPBStructure, PPBCORESTRUCT pPBCoreStructure)
{
	m_pCoreStructure = pPBCoreStructure;
	m_pCurrentPBstruct = pPBStructure;

	++m_iLoadCount;
		
	PPBINSTSTRUCT pInstStruct = new PBInstStruct;
	if(pInstStruct == NULL){
		Log(PB_LOG_ERROR,L"Mem allocation",L"Preload",__LINE__);
		return FALSE;
	}
	pInstStruct->instID			= pPBStructure->iTabID;
	pInstStruct->pWrappedObj	= NULL;
	pInstStruct->pNext			= NULL;

	if(m_pInstHead == NULL){
		pPBCoreStructure->pLoggingFunc(PB_LOG_INFO,L"First call",L"Preload",__LINE__,m_szModName);
		m_pInstHead = pInstStruct;
		if(!onInit(pPBStructure)){	///>let the derived class know about it by calling it's onInit()
			pPBCoreStructure->pLoggingFunc(PB_LOG_ERROR,L"Failed to initialise in the derived object",L"Preload",__LINE__,m_szModName);///>log error
			return FALSE;
		}
	
	}
	
	else{
		/**
		*		
		*	go through the linked list and look for a match
		*/
		PPBINSTSTRUCT pInst;
		for(pInst = m_pInstHead;pInst->pNext;pInst = pInstStruct->pNext)
		{
			if(pInst->instID == pPBStructure->iTabID){
				pPBCoreStructure->pLoggingFunc(PB_LOG_ERROR,L"Preload called twice same instance",L"Preload",__LINE__,m_szModName);
				delete pInstStruct;
				return FALSE;
			}
		}
		pInst->pNext = pInstStruct;
	}
	
	
	if(!onAttachInstance(pPBStructure,pInstStruct)){///>let the derived object know about a new PB app instance
		Log(PB_LOG_ERROR,L"Failed onAttachInstance in the derived object",L"Preload",__LINE__);///>log error	
		return FALSE;				///>error the derived function has failed
	}
	
	return TRUE;
}

//returns the head of the list
PPBINSTSTRUCT PBModule::dispose(PPBSTRUCT pPBStructure,PPBINSTSTRUCT pInstStruct)
{
	PPBINSTSTRUCT pTempInst;

	if(pInstStruct == NULL)
		return NULL;
	
	if(pInstStruct->instID == pPBStructure->iTabID){
		//pInstStruct = dispose(pPBStructure,pInstStruct->pNext);
		pTempInst = pInstStruct->pNext;
		delete pInstStruct;
		return pTempInst;
	}
	
	pInstStruct->pNext =  dispose(pPBStructure,pInstStruct->pNext);
	return pInstStruct;
}


////////////////////////////////////////////////////////////////////////
/// Function:		Dispose
/// Description:	Calls onDeInit() when the last Dispose has been issued
/// Returns:		An enumeration type INITRET defined in PBPlugin.h
/// Author:			Paul Henderson
/// Date:			May 2009
////////////////////////////////////////////////////////////////////////
INITRET PBModule::Dispose(PPBSTRUCT pPBStructure)
{
	PPBINSTSTRUCT pInstStruct,pTempStruct;
	--m_iLoadCount;
	
	if(m_pInstHead->instID == pPBStructure->iTabID){
		
		if(!onReleaseInstance(pPBStructure,m_pInstHead)){///>let the derived object know about it
			Log(PB_LOG_ERROR,L"Failed onReleaseInstance in the derived object",L"Dispose",__LINE__);///>log error	
			return FAILED;				///>error the derived function has failed
		}
		if(m_pInstHead->pNext){
			pTempStruct = m_pInstHead->pNext;
			delete m_pInstHead;
			m_pInstHead = pTempStruct;
		}
		else{
			onDeInit(pPBStructure);
			PulseEvent(m_hQuitEvent);
			return DEINITIALISED;///>return DEINITIALISED when the last instance has been deleted
		}
		return OK;
	}
	

	for(pInstStruct = m_pInstHead;pInstStruct->pNext;pInstStruct = pInstStruct->pNext)
	{
		if(pInstStruct->pNext->instID == pPBStructure->iTabID){
			///>found a match
			if(!onReleaseInstance(pPBStructure,pInstStruct->pNext)){///>let the derived object know about it
				Log(PB_LOG_ERROR,L"Failed onReleaseInstance in the derived object",L"Dispose",__LINE__);///>log error	
				return FAILED;				///>error the derived function has failed
			}
			pTempStruct = pInstStruct->pNext->pNext;		
			delete pInstStruct->pNext;
			pInstStruct->pNext = pTempStruct;
			return OK;
			

		}
	}
	
	Log(PB_LOG_ERROR,L"iTabID not found",L"Dispose",__LINE__);///> the tab ID was not found, so log an error
	return FAILED;///>
}


////////////////////////////////////////////////////////////////////////
/// Function:		Process
/// Description:	Calls Metaproc with the correct object pointer.
///					
/// Returns:		TRUE if successful; 
/// Author:			Paul Henderson
/// Date:			Aug 2009
////////////////////////////////////////////////////////////////////////
BOOL PBModule::Process(PBMetaStruct *pbMetaStructure, PPBSTRUCT pPBStructure)
{
	BOOL bOk= FALSE;
	if (pPBStructure->hWnd == NULL || pPBStructure->hInstance == NULL)
		return FALSE;
	m_pCurrentPBstruct = pPBStructure;///>used to hold the current structure for the global callback functions
	
	void *pWrappedpObj = GetObjFromID(pPBStructure->iTabID);
	if(pWrappedpObj == NULL)
		return FALSE;

	return MetaProc(pbMetaStructure,pPBStructure,pWrappedpObj);
	
}

////////////////////////////////////////////////////////////////////////
BOOL PBModule::ProcessMessage (int appid, HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	PPBINSTSTRUCT pInstStruct;

	// Notify specified instance
	for (pInstStruct = m_pInstHead; pInstStruct; pInstStruct = pInstStruct->pNext)
	{
		if (pInstStruct->instID == appid)
			 return onMessage (hwnd, msg, wparam, lparam, pInstStruct->pWrappedObj);
	}

	return FALSE;
}

////////////////////////////////////////////////////////////////////////
BOOL PBModule::StaticProcessMessage (PBModule *pmodule, int appid, HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	return pmodule->ProcessMessage (appid, hwnd, msg, wparam, lparam);
}




////////////////////////////////////////////////////////////////////////////////
// Description:	Log the specified message to the logging module via function pointer to the core.
//				
// Authors:		Paul Henderson
// Change History:		
//	May 2009	- Initial Revision (PH)
////////////////////////////////////////////////////////////////////////////////
BOOL PBModule::Log(LogTypeInterface logSeverity, LPCTSTR pLogComment, 
		   LPCTSTR pFunctionName, DWORD dwLineNumber)
{
	if(m_pCoreStructure->pLoggingFunc){
		return m_pCoreStructure->pLoggingFunc(logSeverity,pLogComment,pFunctionName ,dwLineNumber,m_szModName);
	}
	return FALSE;
	
}

////////////////////////////////////////////////////////////////////////
/// Function:		SendMetaFunc
/// Description:	Method that is used when a module sends a meta tag to another module usually contained in another DLL.
///					This version of the overloaded function sets properties in another module, which is why w have a parameter and a value.  				
/// Returns:		TRUE if successful;
/// Author:			Paul Henderson
/// Date:			May 2009
////////////////////////////////////////////////////////////////////////
BOOL PBModule::SetModProperty(LPCTSTR pTargetMod,LPCTSTR pParam,LPCTSTR pValue)
{
	if(m_pCoreStructure->pSetPlugProp)
		return m_pCoreStructure->pSetPlugProp(m_pCurrentPBstruct,pTargetMod,pParam,pValue,m_szModName);
	return FALSE;
}

////////////////////////////////////////////////////////////////////////
/// Function:		SendMetaFunc
/// Description:	Method that is used when a module sends a meta tag to another module usually contained in another DLL.
///					
/// Returns:		TRUE if successful;
/// Author:			Paul Henderson
/// Date:			May 2009
////////////////////////////////////////////////////////////////////////
BOOL PBModule::CallModMethod(LPCTSTR pTargetMod,LPCTSTR pMethod)
{
	
	if(m_pCoreStructure->pCallPlgMethod)
		return m_pCoreStructure->pCallPlgMethod(m_pCurrentPBstruct,pTargetMod,pMethod,m_szModName);
	return FALSE;
}





////////////////////////////////////////////////////////////////////////
/// Function:		RegisterForMessage
/// Description:	Method that is used when a module requests notification of a message from the core.
///					If wparam or lparam is NULL a notification will be received for the message only.
///					This is an asynchronous method that places the message on the core register and returns immediately.
///					
/// Returns:		TRUE if successful;
/// Author:			Paul Henderson
/// Date:			May 2009
////////////////////////////////////////////////////////////////////////
BOOL PBModule::RegisterForMessage(HWND hwnd, UINT msg)
{
	return RegisterForMessage (this, StaticProcessMessage, m_pCurrentPBstruct->iTabID, hwnd, msg);
}

BOOL PBModule::RegisterForMessage(UINT msg)
{
	return RegisterForMessage (m_pCurrentPBstruct->hWnd, msg);
}


////////////////////////////////////////////////////////////////////////
/// Function:		PBRegisterForMessage
/// Description:	Used when a module requests notification of a message from the core.
///					Can be called from a thread when no this pointer is available to to a specific module.
///					If wparam or lparam is NULL a notification will be received for the message only.
///					This is an asynchronous method that places the message on the core register and returns immediately.
///					
/// Returns:		TRUE if successful;
/// Author:			Paul Henderson
/// Date:			May 2009
////////////////////////////////////////////////////////////////////////
BOOL PBModule::RegisterForMessage(PBModule *pmodule, MESSAGECALLBACK *pcallback, int appid, HWND hwnd, UINT msg)
{
	if(m_pCoreStructure->pRegMessFunc){
			
		REGISTERMESSAGE reg;

		reg.hWnd = hwnd;
		reg.nAppID = appid;
		reg.pCallback = pcallback;
		reg.pModule = pmodule;
		reg.uMessage = msg;

		return m_pCoreStructure->pRegMessFunc(&reg);
		
	}
	return FALSE;
}

/******************************************************************************/
/******************************************************************************/
// Functions for handling paint notifications from the core

/******************************************************************************/
void PBModule::ProcessPaint (int appid, PAINTSTRUCT *pps)
{
	PPBINSTSTRUCT pInstStruct;

	// Notify specified instance
	for (pInstStruct = m_pInstHead; pInstStruct; pInstStruct = pInstStruct->pNext)
	{
		if (pInstStruct->instID == appid)
			 onPaint (pps, pInstStruct->pWrappedObj);
	}
}

/******************************************************************************/
void PBModule::StaticProcessPaint (PBModule *pmodule, int appid, PAINTSTRUCT *pps)
{
	pmodule->ProcessPaint (appid, pps);
}

/******************************************************************************/
BOOL PBModule::RegisterForPaint (void)
{
	if (m_pCoreStructure->pRegPaintFunc)
	{
		REGISTERPAINT reg;

		reg.hWnd = m_pCurrentPBstruct->hWnd;
		reg.nAppID = m_pCurrentPBstruct->iTabID;
		reg.pCallback = StaticProcessPaint;
		reg.pModule = this;

		return m_pCoreStructure->pRegPaintFunc (&reg);
	}

	return FALSE;
}

/******************************************************************************/


////////////////////////////////////////////////////////////////////////
/// Function:		RegisterForAppFocus
/// Description:	Method that is used when a module requests notification of PB application focus.
///
/// Author:			Paul Henderson
/// Date:			September 2009
////////////////////////////////////////////////////////////////////////
void PBModule::RegisterForAppFocus()
{
	//create the thread that waits for a change in application instance event
	HANDLE hAppFocus  = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) AppFocusThread,(LPVOID) this, 0, 0);
}




////////////////////////////////////////////////////////////////////////
/// Function:		AppFocusThread
/// Description:	Thread that is started when a module calls 'RegisterForAppFocus()'
///					It calls 'onAppFocus()' when a PB application gains focus  
/// Author:			Paul Henderson
/// Date:			September 2009
////////////////////////////////////////////////////////////////////////
DWORD WINAPI PBModule::AppFocusThread(LPVOID lparam)
{
	PBModule *pObj = (PBModule *)lparam;
	HANDLE	hEvents[2];
	//int iOldID,iNewID;
	//iOldID = iNewID = PBINVALID_APPID;

	hEvents[0] = pObj->m_pCoreStructure->pEventStructure->PBAppGotFocusEvent;
	hEvents[1] = pObj->m_hQuitEvent;

	DWORD dwEvent;
	while(1)
	{
		dwEvent =  WaitForMultipleObjects( 2, hEvents,FALSE,INFINITE);
		switch(dwEvent - WAIT_OBJECT_0)
				{
					case 0://app got focus 
						//check for a application focus change
						if(*pObj->m_pCoreStructure->pPreviousInstID != *pObj->m_pCoreStructure->pCurrentInstID)
							pObj->onAppFocus(*pObj->m_pCoreStructure->pPreviousInstID,*pObj->m_pCoreStructure->pCurrentInstID);
						/* needed if we use SetEvent instead of PulseEvent in the core
						while(1)
						{
							if(WaitForSingleObject(hEvents[0],0) == WAIT_TIMEOUT)
								break;
							Sleep(100);
						}
						*/
						break;
											
					case 1://close thread
						return 0;
				}
			
	
	}
}

void *PBModule::GetObjFromID(int iID)
{
	if(iID != PB_INVALID_APPID){
		PPBINSTSTRUCT pInstStruct;
		for(pInstStruct = m_pInstHead;;pInstStruct = pInstStruct->pNext)
		{
			if(pInstStruct->instID == iID)
				return pInstStruct->pWrappedObj;
			if(pInstStruct->pNext == NULL)
				break;///> instance ID not found
		}
	}
	return NULL;
}




////////////////////////////////////////////////////////////////////////
/// Function:		SendPBNavigate
/// Description:	Method that is used when a module executes a navigation.
///					This method requires one navigation string with as many arguments as necessary.
///					This is an asynchronous method that places the navigation URI into a queue and returns immediately.
///
/// Returns:		TRUE if successful;
/// Author:			Paul Henderson
/// Date:			May 2009
////////////////////////////////////////////////////////////////////////
BOOL PBModule::SendPBNavigate(int iInstID,LPCTSTR lpNavStr,...)
{
	BOOL bRet = FALSE;
	VARSTRUCT  structVar,*pVars,*pStartVars;
	pStartVars = pVars = &structVar;
	memset(pStartVars,0,sizeof(VARSTRUCT));

	TCHAR *l_ParamVal;
	
	va_list l_Arg;
	va_start(l_Arg, lpNavStr);

	l_ParamVal = va_arg(l_Arg,TCHAR*);	// Move past the Format String
	while(l_ParamVal)
	{
		//copy the pointer to our pointer within the structure
		pVars->pStr = l_ParamVal;
		l_ParamVal = va_arg(l_Arg,TCHAR*);
		if(l_ParamVal){
			pVars->pNextVar = new VARSTRUCT;
			if(pVars->pNextVar == NULL){
				bRet = FALSE;
				goto _cleanup;
			}
			pVars->pNextVar->pNextVar = NULL;	// GD
			pVars = pVars->pNextVar;
		}

		
	}
	
	bRet = m_pCoreStructure->pNavigateFunc(iInstID,lpNavStr,pStartVars,m_szModName);
_cleanup:
	deleteMem(pStartVars);
	
	return bRet;
}

////////////////////////////////////////////////////////////////////////
/// Function:		deleteMem
/// Description:	Method that is used internally.
///					Recursively deletes the linked list created in SendPBNavigate
///
/// Returns:		void;
/// Author:			Paul Henderson
/// Date:			May 2009
////////////////////////////////////////////////////////////////////////
void PBModule::deleteMem(PVARSTRUCT pVarStruct)
{
	if(pVarStruct->pNextVar){
		deleteMem(pVarStruct->pNextVar);
		delete pVarStruct->pNextVar;
		pVarStruct->pNextVar = NULL;
	}
	
}

////////////////////////////////////////////////////////////////////////
/// Function:		deleteInst
/// Description:	Method that is used internally.
///					Recursively deletes the linked list used for tracking instance state
///
/// Returns:		void;
/// Author:			Paul Henderson
/// Date:			July 2009
////////////////////////////////////////////////////////////////////////
void PBModule::deleteInst(PPBINSTSTRUCT pInstStruct)
{
	if(pInstStruct->pNext){
		deleteInst(pInstStruct->pNext);
		delete pInstStruct->pNext;
		pInstStruct->pNext = NULL;
	}
	
}







#pragma endregion



