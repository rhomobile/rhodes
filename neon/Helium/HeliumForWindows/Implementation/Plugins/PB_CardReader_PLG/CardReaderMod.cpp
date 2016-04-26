#include "StdAfx.h"
#include "CardReaderMod.h"
#include "CardReader.h"

CCardReaderMod::CCardReaderMod():
	m_hStopEvent(INVALID_HANDLE_VALUE),
	m_hKeypadMutex(INVALID_HANDLE_VALUE)
{
}

////////////////////////////////////////////////////////////////////////////////
//	Description:	Method that is called when a module receives the first preload.
//
//	Scope:			Public
//				
//	Authors:		Mike Schuette
//	Change History:		
//					Oct 2009 - Created
////////////////////////////////////////////////////////////////////////////////
BOOL CCardReaderMod::onInit(PBStruct *pPBStructure)
{
	_tcscpy(m_szModName, L"CardReader");

	if (m_hStopEvent == INVALID_HANDLE_VALUE)
		// Create an event so that main routine can signal CardReaderReadThread to exit
		m_hStopEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

	if (m_hKeypadMutex == INVALID_HANDLE_VALUE)
		// Create a mutex to suspend a card reader close until the user's PIN has been entered
		m_hKeypadMutex = CreateMutex(NULL, FALSE, NULL);

	//Register for change in app focus events
	RegisterForEvent(PB_BROWSER_BEFORE_NAV_EVENT);
	RegisterForEvent(RHO_APPFOCUS_EVENT);
	
	//by default close the reader during a page navigate
	m_bReaderStayOpen = FALSE;

	m_eResumeAction = NONE;

	return RegisterForEvent(PB_APPFOCUSEVENT); 
}


////////////////////////////////////////////////////////////////////////////////
//	Description:	Method that is called when a module receives the last dispose.
//
//	Scope:			Public
//				
//	Authors:		Mike Schuette
//	Change History:		
//					Oct 2009 - Created
////////////////////////////////////////////////////////////////////////////////
void CCardReaderMod::onDeInit(PBStruct *pPBStructure)
{
	// No more stop event
	if (m_hStopEvent != INVALID_HANDLE_VALUE)
	{
		CloseHandle (m_hStopEvent);
		m_hStopEvent = INVALID_HANDLE_VALUE;
	}

	// No more keypad mutex
	if (m_hKeypadMutex != INVALID_HANDLE_VALUE)
	{
		CloseHandle (m_hKeypadMutex);
		m_hKeypadMutex = INVALID_HANDLE_VALUE;
	}

	Log(PB_LOG_INFO,L"calling: UnRegisterForAppFocus",L"onDeInit",__LINE__);
	UnRegisterForEvent(PB_BROWSER_BEFORE_NAV_EVENT);
	UnRegisterForEvent(PB_APPFOCUSEVENT);
	UnRegisterForEvent(RHO_APPFOCUS_EVENT);

	delete m_pInstHead;
	m_pInstHead = NULL;
}


////////////////////////////////////////////////////////////////////////////////
//	Description:	Method that is called when a module receives the first preload for a new instance.
//
//	Scope:			Public
//				
//	Authors:		Mike Schuette
//	Change History:		
//					Oct 2009 - Created
////////////////////////////////////////////////////////////////////////////////
BOOL CCardReaderMod::onAttachInstance(PBStruct *pPBStructure, PBInstStruct *pPBInstStruct)
{
	Log(PB_LOG_INFO,L"Creating Instance",L"onAttachInstance",__LINE__);
	m_bAppHasFocus = (pPBStructure->bLaunchingAppHasFocus == TRUE);
	pPBInstStruct->pWrappedObj = new CCardReader(this, pPBStructure->iTabID, pPBStructure->hWnd);
	if (pPBInstStruct->pWrappedObj)
		return TRUE;
	return FALSE;
}

////////////////////////////////////////////////////////////////////////////////
//	Description:	Method that is called when a module receives a final Dispose on an instance.
//
//	Scope:			Public
//				
//	Authors:		Mike Schuette
//	Change History:		
//					Oct 2009 - Created
////////////////////////////////////////////////////////////////////////////////
BOOL CCardReaderMod::onReleaseInstance(PBStruct *pPBStructure, PBInstStruct *pPBInstStruct)
{
	Log(PB_LOG_INFO,L"Deleting Instance",L"onReleaseInstance",__LINE__);
	delete (CCardReader *) pPBInstStruct->pWrappedObj;
	pPBInstStruct->pWrappedObj = NULL;
	return TRUE;
}

BOOL CCardReaderMod::onRhoAppFocus(bool bActivate, int iInstID)
{
	CCardReader *pCardReader = NULL;
	pCardReader = (CCardReader*) GetObjFromID(iInstID);
	if (pCardReader == NULL)
	{
		m_bAppHasFocus = bActivate;
		m_eResumeAction = NONE;
		return FALSE;
	}

	if (m_bAppHasFocus && !bActivate)
	{
		// we've just lost focus
		m_eResumeAction = NONE;
		if (pCardReader->GetCardReaderHasBeenOpened())
			m_eResumeAction = RESUME;
		onAppFocus(iInstID, PB_INVALID_APPID);
	}
	else if (!m_bAppHasFocus && bActivate)
	{
		// we've just regained focus
		if (m_eResumeAction == RESUME)
			onAppFocus(PB_INVALID_APPID, iInstID);
		else if (m_eResumeAction == ACTIVATE)
		{
			pCardReader->OpenTheCardReader(FALSE);
			pCardReader->SetThreadReadHandle(CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)pCardReader->CardReaderReadThread, pCardReader, 0, NULL));
		}
	}
	m_bAppHasFocus = bActivate;
	return TRUE;
};

////////////////////////////////////////////////////////////////////////////////
//	Description:	Provides Browser with a way of invoking functionality in the module.
//					pbMetaStructure is parsed to determine which action to perform.
//
//	Scope:			Public
//				
//	Authors:		Mike Schuette
//	Change History:		
//					Oct 2009 - Created
////////////////////////////////////////////////////////////////////////////////
BOOL CCardReaderMod::MetaProc(PBMetaStruct *pPBMetaStructure, PBStruct *pPBStructure, VOID *pParam)
{
	Log(PB_LOG_WARNING, L"The Card Reader API from the RhoElements Extension has been deprecated in 4.0, please transition your applications to use the common Card Reader API for future releases", _T(__FUNCTION__), __LINE__);

	DWORD Status;

	CCardReader *pCardReader = (CCardReader *) pParam;

	Log(PB_LOG_INFO,pPBMetaStructure->lpParameter,L"MetaProc",__LINE__);

	// start the parsing of the Meta tag info
	if(cmp(pPBMetaStructure->lpParameter, L"close"))
	{
		//shut down the card reader, terminate all signals/threads associated with it
		pCardReader->StopTheCardReader(NO_FOCUS_SWITCH);
		pCardReader->SetCardReaderNavString(L"");
		m_eResumeAction = NONE;
		return TRUE;
	}
	else if(cmp(pPBMetaStructure->lpParameter, L"readevent") || cmp(pPBMetaStructure->lpParameter, L"open"))
	{
		if (cmp(pPBMetaStructure->lpParameter, L"readevent") && cmp(pPBMetaStructure->lpValue, L"close"))
		{
		    //shut down the card reader, terminate all signals/threads associated with it
		    pCardReader->StopTheCardReader(NO_FOCUS_SWITCH);
		    pCardReader->SetCardReaderNavString(L"");
			m_eResumeAction = NONE;
		    return TRUE;
		}
		//if lpParameter is "open" or "readevent" with an empty or NULL string, then set the
		//nav string to "", else copy the lpParameter string to it
		else if (cmp(pPBMetaStructure->lpParameter, L"open"))
		{
			pCardReader->SetCardReaderNavString(L"");
		}
		else
		{
			if (pPBMetaStructure->lpValue)
				pCardReader->SetCardReaderNavString(pPBMetaStructure->lpValue);
			else
				pCardReader->SetCardReaderNavString(L"");
		}

		if (!m_bAppHasFocus)
		{
			if (m_eResumeAction == NONE)
				m_eResumeAction = ACTIVATE;
			return TRUE;
		}

		//open the card reader, create signals/threads to handle processing of card reader data
		Status = pCardReader->OpenTheCardReader(FALSE);
		
		if (Status != MSR_SUCCESS)
			return FALSE;

		if (pCardReader->GetThreadReadHandle() == INVALID_HANDLE_VALUE)
			// Create a thread to read the card swipe data.
			pCardReader->SetThreadReadHandle(CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)pCardReader->CardReaderReadThread, pCardReader, 0, NULL));
	}
	else if(cmp(pPBMetaStructure->lpParameter, L"pintimeout"))
	{
		if (isFloat(pPBMetaStructure->lpValue))
			return(pCardReader->SetPinTimeout(pPBMetaStructure->lpValue));
		else
			return FALSE;
	}
	else if(cmp(pPBMetaStructure->lpParameter, L"pinentry"))
	{
		return(pCardReader->SetPinEntry(pPBMetaStructure->lpValue));
	}
	else if(cmp(pPBMetaStructure->lpParameter, L"pandata"))
	{
		return(pCardReader->ManualPANEntry(pPBMetaStructure->lpValue));
	}
	else if(cmp(pPBMetaStructure->lpParameter, L"autotab"))
	{
		return(pCardReader->SetAutoTab(pPBMetaStructure->lpValue));
	}
	else if(cmp(pPBMetaStructure->lpParameter, L"autoenter"))
	{
		return(pCardReader->SetAutoEnter(pPBMetaStructure->lpValue));
	}
	else if(cmp(pPBMetaStructure->lpParameter, L"modulename"))
	{
		return(pCardReader->SetDriverName(pPBMetaStructure->lpValue));
	}
	else if(cmp(pPBMetaStructure->lpParameter, L"PersistReaderSettings"))
	{
		if(cmp(pPBMetaStructure->lpValue,L"True")){
			PersistReaderSettings(TRUE);
		}
		else{ 
			PersistReaderSettings(FALSE);
		}
		return TRUE;

	}
	else
	{
		Log(PB_LOG_WARNING,L"Unrecognized Meta Tag",L"MetaProc",__LINE__);
		return FALSE;
	}
	

	return TRUE;
}


////////////////////////////////////////////////////////////////////////////////
//	Description:	Method that is called when an application gains focus.
//					It shuts down processing of data from the previously
//					focused app and begins processing of data on the new app.
//
//	Scope:			Public
//				
//	Authors:		Mike Schuette
//	Change History:		
//					Oct 2009 - Created
////////////////////////////////////////////////////////////////////////////////
BOOL CCardReaderMod::onAppFocus(int iOldID, int iNewID)
{
	//  Ignore focus switch to / from the license screen
	if (iOldID == PB_LICENCE_SCREEN || iNewID == PB_LICENCE_SCREEN)
		return TRUE;

	CCardReader *pNewFocusedCardReader;
	CCardReader *pOldFocusedCardReader;
	TCHAR lpszBuffer[100];

	Log(PB_LOG_INFO, L"Change of app focus for card reader", L"onAppFocus", __LINE__);

	// Look for app instance pointer of newly and previously focused apps. 
	pOldFocusedCardReader = (CCardReader *) GetObjFromID(iOldID);
	pNewFocusedCardReader = (CCardReader *) GetObjFromID(iNewID);

	if (pOldFocusedCardReader!=NULL)
	{
		wsprintf(lpszBuffer, L"Old cardreader obj id #%.16d, ptr %.16p", iOldID, pOldFocusedCardReader);
		Log(PB_LOG_INFO,lpszBuffer , L"onAppFocus", __LINE__);
	}

	if (pNewFocusedCardReader!=NULL)
	{
		wsprintf(lpszBuffer, L"New cardreader obj id #%.16d, ptr %.16p", iNewID, pNewFocusedCardReader);
		Log(PB_LOG_INFO,lpszBuffer , L"onAppFocus", __LINE__);
	}

	// If previously focused app had opened the device, stop it and close it 
	if (pOldFocusedCardReader!=NULL && pOldFocusedCardReader->GetCardReaderHasBeenOpened()) {
		pOldFocusedCardReader->StopTheCardReader(FOCUS_SWITCH);
	}

	// If newly focused app has already opened the device, repoen it, 
	// recreate the read thread
	if (pNewFocusedCardReader!=NULL && pNewFocusedCardReader->GetCardReaderHasBeenOpened())
	{
		DWORD Status = pNewFocusedCardReader->OpenTheCardReader(FOCUS_SWITCH);
		
		if (Status != MSR_SUCCESS)
				return FALSE;

		if (pNewFocusedCardReader->GetThreadReadHandle() == INVALID_HANDLE_VALUE)
			// Create a thread to read the card swipe data.
			pNewFocusedCardReader->SetThreadReadHandle(CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)pNewFocusedCardReader->CardReaderReadThread, pNewFocusedCardReader, 0, NULL));
	}

	return TRUE;
}


////////////////////////////////////////////////////////////////////////////////
//	Description:	Method that is called when an browser navigates to a new page.
//
//	Scope:			Public
//				
//	Authors:		Mike Schuette
//	Change History:		
//					Oct 2009 - Created
////////////////////////////////////////////////////////////////////////////////
BOOL CCardReaderMod::onBeforeNavigate(int iInstID)
{
	CCardReader *pCardReader;

	Log(PB_LOG_INFO, L"Change of page for card reader", L"onNavEvent", __LINE__);

	// Look for app instance pointer of the currently focused app. 
	pCardReader = (CCardReader *) GetObjFromID(iInstID);

	if(!m_bReaderStayOpen){
		// If focused app had opened the device, stop it and close it 
		if (pCardReader!=NULL && pCardReader->GetCardReaderHasBeenOpened())
			pCardReader->StopTheCardReader(NO_FOCUS_SWITCH);

		
		// Set all parameters to their default values
		pCardReader->SetAutoTab(L"disabled");
		pCardReader->SetAutoEnter(L"disabled");
		pCardReader->SetPinTimeout(DEFAULT_PIN_ENTRY_TIMEOUT_S);
		pCardReader->SetPinEntry(PIN_ENTRY_OFF);
		pCardReader->SetDCR(FALSE);
	}
	// Set nav string to empty string
	pCardReader->SetCardReaderNavString(L"");

	return TRUE;
}

////////////////////////////////////////////////////////////////////////
// Function:	GetStopEventHandle
// Description:	Get the value of the attribute m_hStopEvent
//
// Scope:		Public
//
// Return:		value of the attribute m_hStopEvent
//
//
// Author:		Mike Schuette
// Change History:
//				Oct 2009 - Created
////////////////////////////////////////////////////////////////////////
HANDLE CCardReaderMod::GetStopEventHandle()
{
	return m_hStopEvent;
}

////////////////////////////////////////////////////////////////////////
// Function:	GetKeypadMutexHandle
// Description:	Get the value of the attribute m_hKeypadMutex
//
// Scope:		Public
//
// Return:		value of the attribute m_hKeypadMutex
//
//
// Author:		Mike Schuette
// Change History:
//				Oct 2009 - Created
////////////////////////////////////////////////////////////////////////
HANDLE CCardReaderMod::GetKeypadMutexHandle()
{
	return m_hKeypadMutex;
}

////////////////////////////////////////////////////////////////////////////////
//	Description:	Append the app instance ID to the log message and call the
//					module's Log method
//
//	Scope:			Public
//
//	Return:			TRUE, if Log() is successful, FALSE otherwise.
//				
//	Authors:		Mike Schuette
//	Change History:		
//					Oct 2009 - Created
////////////////////////////////////////////////////////////////////////////////
BOOL CCardReaderMod::LogInst(LogTypeInterface logSeverity, LPCTSTR lpszLogComment, 
		   LPCTSTR lpszFunctionName, DWORD dwLineNumber)
{
	TCHAR buffer[1024];

	wsprintf(buffer, L"%s - instance id = #%d", lpszLogComment, m_pCurrentPBstruct->iTabID);

	return(Log(logSeverity,buffer,lpszFunctionName ,dwLineNumber));
}


////////////////////////////////////////////////////////////////////////
// Function:	PersistReaderSettings
// Description:	Sets the persist state of the reader settings during a page navigate
//
// Scope:		Public
//
// Return:		void
//
//
// Author:		Paul Henderson
// Change History:
//				Oct 2010 - Created
////////////////////////////////////////////////////////////////////////
void CCardReaderMod::PersistReaderSettings(BOOL bState)
{

	m_bReaderStayOpen = bState;

}