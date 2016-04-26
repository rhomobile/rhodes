#include "stdafx.h"


#include "..\..\Common\Public\PBplugin.h"
#include <windows.h>

#pragma once



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
	m_pCurrentPBstruct	= NULL;///<used to hold the current structure for the global callback functions
	_tcscpy(m_szModName,L"Abstract");
	m_pInstHead			= NULL;
	
	
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
	return;	
}


////////////////////////////////////////////////////////////////////////////////
/// Function:		CreateIMO
/// Description:	Method that is called to create an instance of module for internal use.
/// Returns:		IMO reference for use in IMO calls
/// Author:			Paul Henderson
/// Date:			November 2009
////////////////////////////////////////////////////////////////////////////////
IMOREF	PBModule::CreateIMO	(LPCTSTR pTargetModuleName)
{
	return m_pCoreStructure->pCreateImo(m_pCurrentPBstruct,pTargetModuleName,m_szModName);
}





////////////////////////////////////////////////////////////////////////////////
/// Function:		DeleteIMO
/// Description:	Deletes the instance of module referenced by IMORef
/// Returns:		TRUE if successful
/// Author:			Paul Henderson
/// Date:			November  2009
////////////////////////////////////////////////////////////////////////////////
BOOL	PBModule::DeleteIMO	(IMOREF IMORef)
{
	return m_pCoreStructure->pDeleteImo(IMORef);
}	
	



////////////////////////////////////////////////////////////////////////////////
/// Function:		SetIMOProperty
/// Description:	Sets a property on a IMO reference
/// Returns:		TRUE if successful
/// Author:			Paul Henderson
/// Date:			November  2009
////////////////////////////////////////////////////////////////////////////////
BOOL	PBModule::SetIMOProperty(IMOREF IMORef,LPCTSTR pParam,LPCTSTR pValue)
{
	return m_pCoreStructure->pSetImoProperty(IMORef,pParam,pValue);
}
	



////////////////////////////////////////////////////////////////////////////////
/// Function:		CallIMOMethod
/// Description:	Calls a method on a IMO reference
/// Returns:		TRUE if successful
/// Author:			Paul Henderson
/// Date:			November  2009
////////////////////////////////////////////////////////////////////////////////
BOOL	PBModule::CallIMOMethod	(IMOREF IMORef,LPCTSTR pMethod)
{
	return m_pCoreStructure->pCallImoMethod(IMORef,pMethod);
}


////////////////////////////////////////////////////////////////////////////////
/// Function:		SetIMOCallBack
/// Description:	Sets a callback function to receive navigation (return data) from the plugin.
/// Returns:		TRUE if successful
/// Author:			Paul Henderson
/// Date:			December  2009
////////////////////////////////////////////////////////////////////////////////
BOOL	PBModule::SetIMOCallBack(IMOREF IMORef,IMOEVENTPROC IMOEventProc,LPARAM lParam)
{
	return m_pCoreStructure->pSetImoCallBack(IMORef,IMOEventProc,lParam);
}


////////////////////////////////////////////////////////////////////////////////
/// Function:		GetAppIDFromIMO
/// Description:	Every IMO is created by an application, this will convert from IMO ID to app ID.
/// Returns:		Returns the application ID of the IMO.
/// Author:			Paul Henderson
/// Date:			February  2009
////////////////////////////////////////////////////////////////////////////////
int	PBModule::GetAppIDFromIMO(int iImoID)
{
	
	int iID;
	
	if(iImoID >= PB_APP_ARRSIZE ){
		for(iID=0;iID < PB_APP_ARRSIZE;iID++)
		{
			if(iImoID > ((iID*IMOS_PER_APP)+PB_APP_ARRSIZE)&& iImoID < (((iID+1)*IMOS_PER_APP)+PB_APP_ARRSIZE)){
				return iID;
			}
		} 
	}
	//not an IMO ID or a negative IMO which has no alliance with an application, so just return the int;
	return iImoID;

}




////////////////////////////////////////////////////////////////////////////////
/// Function:		GetEMMLObj
/// Description:	Gets the EMML object for iTabID.
/// Returns:		A pointer to the EMML object.
/// Author:			Paul Henderson
/// Date:			December  2009
////////////////////////////////////////////////////////////////////////////////
CEMML* PBModule::GetEMMLObj(int iTabID)
{
	return m_pCoreStructure->pGetEmmlObj(iTabID);
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
		if(!onInit(pPBStructure)){	///<let the derived class know about it by calling it's onInit()
			pPBCoreStructure->pLoggingFunc(PB_LOG_ERROR,L"Failed to initialise in the derived object",L"Preload",__LINE__,m_szModName);///<log error
			return FALSE;
		}
	
	}
	
	else{
		/**
		*		
		*	go through the linked list and look for a match
		*/
		/*PPBINSTSTRUCT pInst;
		for(pInst = m_pInstHead;pInst->pNext;pInst = pInst->pNext)
		{
			if(pInst->instID == pPBStructure->iTabID){
				pPBCoreStructure->pLoggingFunc(PB_LOG_ERROR,L"Preload called twice same instance",L"Preload",__LINE__,m_szModName);
				delete pInstStruct;
				return FALSE;
			}
		}
		pInst->pNext = pInstStruct;*/

		PPBINSTSTRUCT pInst = m_pInstHead;	// NULL already tested for above
		while (1)
		{
			if (pInst->instID == pPBStructure->iTabID)
			{
				pPBCoreStructure->pLoggingFunc(PB_LOG_ERROR,L"Preload called twice same instance",L"Preload",__LINE__,m_szModName);
				delete pInstStruct;
				return FALSE;
			}

			// End of the list?
			if (!pInst->pNext)
			{
				// Yes, add new record to the end
				pInst->pNext = pInstStruct;
				break;
			}

			pInst = pInst->pNext;
		}



	}
	
	
	if(!onAttachInstance(pPBStructure,pInstStruct)){///<let the derived object know about a new PB app instance
		Log(PB_LOG_ERROR,L"Failed onAttachInstance in the derived object",L"Preload",__LINE__);///<log error	
		return FALSE;				///<error the derived function has failed
	}
	
	return TRUE;
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
	// Find data for this instance in linked list
	// Call onReleaseInstance
	// Delete instance data, re-linking list as necessary
	// Will leave m_pInstHead == NULL if all instances deleted => call onDeInit and notify core

	PPBINSTSTRUCT pinst;
	//  GD: Decrementing count even if function fails?  Code left as is.
	--m_iLoadCount;

	// Find data for this instance in linked list
	for (pinst = m_pInstHead; pinst; pinst = pinst->pNext)
	{
		if (pinst->instID == pPBStructure->iTabID)
			break;
	}

	// Found it?
	if (!pinst)
	{
		WCHAR *perror = new WCHAR [64];
		wsprintf (perror, L"Tab ID %d not found", pPBStructure->iTabID);
		Log (PB_LOG_ERROR, perror, _T(__FUNCTION__), __LINE__);
		delete [] perror;
		return FAILED;
	}

	// Call onReleaseInstance, quit if plugin returns FALSE
	if (!onReleaseInstance (pPBStructure, pinst))
	{
		Log (PB_LOG_ERROR, L"onReleaseInstance failed in derived object", _T(__FUNCTION__), __LINE__);
		return FAILED;
	}

	// Delete instance data, re-linking list as necessary
	if (pinst == m_pInstHead)
	{
		// Remove head of list
		m_pInstHead = pinst->pNext;
		delete pinst;
	}
	else
	{
		// Get parent of this instance - quit if not found, which should never happen
		PPBINSTSTRUCT pparent;
		for (pparent = m_pInstHead; pparent->pNext; pparent = pparent->pNext)
			if (pparent->pNext == pinst)
				break;

		if (!pparent->pNext)
		{
			Log (PB_LOG_ERROR, L"Parent instance not found", _T(__FUNCTION__), __LINE__);
			return FAILED;
		}

		pparent->pNext = pinst->pNext;
		delete pinst;
	}

	// m_pInstHead == NULL if all instances deleted => call onDeInit and notify core
	if (!m_pInstHead)
	{
		onDeInit (pPBStructure);
//		PulseEvent (m_hQuitEvent);
		return DEINITIALISED;
	}


	return OK;
}



/*
	PPBINSTSTRUCT pInstStruct,pTempStruct;
	--m_iLoadCount;
	
	if(m_pInstHead->instID == pPBStructure->iTabID){
		if(!onReleaseInstance(pPBStructure,m_pInstHead)){///<let the derived object know about it
			Log(PB_LOG_ERROR,L"Failed onReleaseInstance in the derived object",L"Dispose",__LINE__);///<log error	
			return FAILED;				///<error the derived function has failed
		}
		if(m_pInstHead->pNext){
			pTempStruct = m_pInstHead->pNext;
			delete m_pInstHead;
			m_pInstHead = pTempStruct;
		}
		else{
			onDeInit(pPBStructure);
			PulseEvent(m_hQuitEvent);
			return DEINITIALISED;///<return DEINITIALISED when the last instance has been deleted
		}
		return OK;
	}
	

	for(pInstStruct = m_pInstHead;pInstStruct->pNext;pInstStruct = pInstStruct->pNext)
	{
		if(pInstStruct->pNext->instID == pPBStructure->iTabID){
			///<found a match
			if(!onReleaseInstance(pPBStructure,pInstStruct->pNext)){///<let the derived object know about it
				Log(PB_LOG_ERROR,L"Failed onReleaseInstance in the derived object",L"Dispose",__LINE__);///<log error	
				return FAILED;				///<error the derived function has failed
			}
			pTempStruct = pInstStruct->pNext->pNext;		
			delete pInstStruct->pNext;
			pInstStruct->pNext = pTempStruct;
			return OK;
			

		}
	}
	
	Log(PB_LOG_ERROR,L"iTabID not found",L"Dispose",__LINE__);///< the tab ID was not found, so log an error
	return FAILED;///<
}
*/

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
	m_pCurrentPBstruct = pPBStructure;///<used to hold the current structure for the global callback functions
	
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
/// Function:		UnRegisterForMessage
/// Description:	Method that is used when a module no longer requires notification of a message from the core.
///					This is an asynchronous method that places the message on the core register and returns immediately.
///					
/// Returns:		TRUE if successful;
/// Author:			John Heywood
/// Date:			May 2013
////////////////////////////////////////////////////////////////////////
BOOL PBModule::UnRegisterForMessage(HWND hwnd, UINT msg)
{
	return UnRegisterForMessage (this, StaticProcessMessage, m_pCurrentPBstruct->iTabID, hwnd, msg);
}

BOOL PBModule::UnRegisterForMessage(UINT msg)
{
	return UnRegisterForMessage (m_pCurrentPBstruct->hWnd, msg);
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

////////////////////////////////////////////////////////////////////////
/// Function:		UnRegisterForMessage
/// Description:	Used when a module no longer requires notification of a message from the core.
///					Can be called from a thread when no this pointer is available to to a specific module.
///					This is an asynchronous method that places the message on the core register and returns immediately.
///					
/// Returns:		TRUE if successful;
/// Author:			John Heywood
/// Date:			May 2013
////////////////////////////////////////////////////////////////////////
BOOL PBModule::UnRegisterForMessage(PBModule *pmodule, MESSAGECALLBACK *pcallback, int appid, HWND hwnd, UINT msg)
{
	if(m_pCoreStructure->pDeRegMessFunc){
			
		REGISTERMESSAGE reg;

		reg.hWnd = hwnd;
		reg.nAppID = appid;
		reg.pCallback = pcallback;
		reg.pModule = pmodule;
		reg.uMessage = msg;

		return m_pCoreStructure->pDeRegMessFunc(&reg);
		
	}
	return FALSE;
}

////////////////////////////////////////////////////////////////////////
/// Function:		RegisterForPrimaryMessage
/// Description:	Used when a module requests notification of a primary message from the core.
///					Primary messages are trapped before dispatch to the browser WndProc.  Plugins can
///					prevent messages from being handled by the browser WndProc by registering for them
///					via this API, and then returning TRUE on receiving the message in onPrimaryMessage.
///					Can be called from a thread when no this pointer is available to to a specific module.
///					
/// Returns:		TRUE if successful;
/// Author:			John Heywood
/// Date:			April 2013
////////////////////////////////////////////////////////////////////////
BOOL PBModule::RegisterForPrimaryMessage(UINT iWMMsgID)
{
	if(m_pCoreStructure->pRegPrimaryMessFunc){
			
		return m_pCoreStructure->pRegPrimaryMessFunc(iWMMsgID);
		
	}
	return FALSE;
}

////////////////////////////////////////////////////////////////////////
/// Function:		UnRegisterForPrimaryMessage
/// Description:	Used when a module no longer requires notification of a primary message from the core.
///					Can be called from a thread when no this pointer is available to to a specific module.
///					
/// Returns:		TRUE if successful;
/// Author:			John Heywood
/// Date:			April 2013
////////////////////////////////////////////////////////////////////////
BOOL PBModule::UnRegisterForPrimaryMessage(UINT iWMMsgID)
{
	if(m_pCoreStructure->pDeRegPrimaryMessFunc){
			
		return m_pCoreStructure->pDeRegPrimaryMessFunc(iWMMsgID);
		
	}
	return FALSE;
}

/******************************************************************************/
/******************************************************************************/
// Functions for handling paint notifications from the core

/******************************************************************************/
void PBModule::ProcessPaint (int appid, HDC hdc)
{
	PPBINSTSTRUCT pInstStruct;

	// Notify specified instance
	for (pInstStruct = m_pInstHead; pInstStruct; pInstStruct = pInstStruct->pNext)
	{
		if (pInstStruct->instID == appid)
			 onPaint (hdc, pInstStruct->pWrappedObj);
	}
}

/******************************************************************************/
void PBModule::StaticProcessPaint (PBModule *pmodule, int appid, HDC hdc)
{
	pmodule->ProcessPaint (appid, hdc);
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
/// Function:		RegisterForEvent
/// Description:	Method that is used when a module requests notification of a core event.
///
/// Author:			Paul Henderson
/// Date:			September 2009
////////////////////////////////////////////////////////////////////////
BOOL PBModule::RegisterForEvent(PB_SyncEvent iEventID)
{
	return m_pCoreStructure->pRegisterForEvent(this,iEventID,m_szModName);
}

////////////////////////////////////////////////////////////////////////
/// Function:		UnRegisterForEvent
/// Description:	Method that is used when a module requests notification of a core event.
///
/// Author:			Paul Henderson
/// Date:			September 2009
////////////////////////////////////////////////////////////////////////
BOOL PBModule::UnRegisterForEvent(PB_SyncEvent iEventID)
{
	return m_pCoreStructure->pUnRegisterForEvent(this,iEventID,m_szModName);
}


////////////////////////////////////////////////////////////////////////
/// Function:		GetObjFromID
/// Description:	Returns a pointer to the target object.
///
/// The returned pointer should be cast to the target object type before use.
///
/// Author:			Paul Henderson
/// Date:			September 2009
////////////////////////////////////////////////////////////////////////
void *PBModule::GetObjFromID(int iID)
{
	if(iID != PB_INVALID_APPID){
		PPBINSTSTRUCT pInstStruct;
		for(pInstStruct = m_pInstHead;;pInstStruct = pInstStruct->pNext)
		{
			if(pInstStruct->instID == iID)
				return pInstStruct->pWrappedObj;
			if(pInstStruct->pNext == NULL)
				break;///< instance ID not found
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
/// Update:			Geoff Day, Nov 2009
////////////////////////////////////////////////////////////////////////
BOOL PBModule::SendPBNavigate (TCHAR* tcJSONNames[], int inst_id, LPCTSTR pnavstr, ...)
{
	// Initialise variable parameter list
	va_list arg_list;
	va_start (arg_list, pnavstr);

	// Point to first optional parameter
	LPCTSTR pparam = va_arg (arg_list, LPCTSTR);

	// Process each parameter
	VARSTRUCT *pvar, *pfirst_var = NULL, *pnext;

	while (pparam)
	{
		// Allocate VARSTRUCT for parameter on end of list, if any
		if (pfirst_var)
		{
			pvar->pNextVar = new VARSTRUCT;
			pvar = pvar->pNextVar;
		}
		else
		{
			pfirst_var = new VARSTRUCT;
			pvar = pfirst_var;
		}

		pvar->pNextVar = NULL;

		// Fill in data
		pvar->pStr = pparam;

		// Point to next parameter
		pparam = va_arg (arg_list, LPCTSTR);
	}

	// Call core function
	BOOL result = m_pCoreStructure->pNavigateFunc (tcJSONNames, inst_id, pnavstr, 
		pfirst_var, m_szModName);

	// Free memory
	for (pvar = pfirst_var; pvar; pvar = pnext)
	{
		pnext = pvar->pNextVar;
		delete pvar;
	}

	return result;
}

////////////////////////////////////////////////////////////////////////
/// Function:		SendPBNavigate
/// Description:	Method that is used when a module executes a navigation.
///					This method takes an array of pointers to strings plus a count.
///					This is an asynchronous method that places the navigation URI into a queue and returns immediately.
///
/// Returns:		TRUE if successful;
/// Author:			Geoff Day
/// Date:			Nov 2009
////////////////////////////////////////////////////////////////////////
BOOL PBModule::SendPBNavigate (TCHAR* tcJSONNames[], int inst_id, LPCTSTR pnavstr, int count, LPCTSTR *pstrings)
{
	VARSTRUCT *pvar, *pfirst_var = NULL, *pnext;
	int n;

	for (n = 0; n < count; n++)
	{
		// Allocate VARSTRUCT for parameter on end of list, if any
		if (pfirst_var)
		{
			pvar->pNextVar = new VARSTRUCT;
			pvar = pvar->pNextVar;
		}
		else
		{
			pfirst_var = new VARSTRUCT;
			pvar = pfirst_var;
		}

		pvar->pNextVar = NULL;

		// Fill in data
		pvar->pStr = pstrings [n];
	}

	// Call core function
	BOOL result = m_pCoreStructure->pNavigateFunc (tcJSONNames, 
		inst_id, pnavstr, pfirst_var, m_szModName);

	// Free memory
	for (pvar = pfirst_var; pvar; pvar = pnext)
	{
		pnext = pvar->pNextVar;
		delete pvar;
	}

	return result;
}


BOOL PBModule::AddPreload(int iAppID,LPTSTR pPreloadStr)
{
	return m_pCoreStructure->pAddPreLoad(iAppID,pPreloadStr);
}
BOOL PBModule::AddDefaultMeta(int iAppID,LPTSTR metaTag)
{
	return m_pCoreStructure->pAddDefaultMetaTag(iAppID,metaTag);
}
LPCTSTR PBModule::GetSettingPtr(LPCTSTR pXmlPath,LPTSTR pAttName)
{
	return m_pCoreStructure->pGetGlobalSettingPtr(pXmlPath,pAttName);
}		
LPCTSTR PBModule::GetAppSettingPtr(int iAppID,LPCTSTR pXmlPath,LPTSTR pAttName)
{
	return m_pCoreStructure->pGetAppSettingPtr(iAppID,pXmlPath,pAttName);
}
LPCTSTR PBModule::GetDefaultMetaTagsPtr(int iAppID,int iMetaIndex)
{
	return m_pCoreStructure->pGetDefaultMetaTagsPtr(iAppID,iMetaIndex);
}

LPCTSTR PBModule::GetPreLoadPtr(int iAppID,int iPreLoadIndex)
{
	return m_pCoreStructure->pGetAppPreloadsPtr(iAppID,iPreLoadIndex);
}
	
BOOL PBModule::SetAppValue(int iAppID,LPCTSTR pXmlPath,LPCTSTR pValue)
{
	return m_pCoreStructure->pSetAppSetting(iAppID,pXmlPath,pValue);
}
	
BOOL PBModule::SetGlobalValue(LPCTSTR pXmlPath, LPCTSTR pValue)
{
	return m_pCoreStructure->pSetGlobalSetting(pXmlPath,pValue);
}

////////////////////////////////////////////////////////////////////////
/// Function:		isInt
/// Description:	Method that checks if a string contains any non digit characters. 
///
/// Returns:		TRUE if there are no non-digit characters in the string. 
/// Author:			Dinka Ranns
/// Date:			July 2011
////////////////////////////////////////////////////////////////////////
BOOL PBModule::isInt (const wchar_t * str)
{
	const wchar_t *temp = str;
	int num_digit=0;
	if (temp == NULL) return false;
	
	if (((*temp)=='+')|| ((*temp)=='-')) temp++;

	while (*temp)
	{
		if (!isdigit(*temp)) 
		{
			return false;
		}
		++temp;
		++num_digit;
	}
	if (num_digit >0 ) return true;
	else return false;
}
////////////////////////////////////////////////////////////////////////
/// Function:		isInt
/// Description:	Method that checks if a string contains any non digit characters. 
///
/// Returns:		TRUE if there are no non-digit characters in the string. 
/// Author:			Dinka Ranns
/// Date:			July 2011
////////////////////////////////////////////////////////////////////////
BOOL PBModule::isFloat (const wchar_t * str)
{
	const wchar_t *temp = str;
	bool hasPoint = false;
	int num_digit=0;

	if (temp == NULL) return false;

		
	if (((*temp)=='+')|| ((*temp)=='-')) temp++;

	while (*temp)
	{
		if (!isdigit(*temp)) 
		{
			if ((*temp =='.') && (!hasPoint))
			{
				hasPoint = true;
			}
			else 
			{
				return false;
			}
		}
		else 
		{
			++num_digit;
		}

		++temp;

	}
	if (num_digit >0 ) return true;
	else return false;
}
/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		March 2011, Initial Creation, DCC
*/
CVersion::CVersion(void)
{
	//  Defined in PB_Defines.h
	iMajorVer = 0;
	iFeatureVer = 0;
	iMaintenanceVer = 0;
	iReleaseCandidateVer = 0;
	iHotFixVer = 0;
	tcComponentName = NULL;
	pNext = NULL;
};

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		March 2011, Initial Creation, DCC
*/
CVersion::~CVersion(void)
{
	delete[] tcComponentName;
}
