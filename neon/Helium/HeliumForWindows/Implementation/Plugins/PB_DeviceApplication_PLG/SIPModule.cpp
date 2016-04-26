#include "SIPModule.h"

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		March 2010
*/
CSIPModule::CSIPModule(PPBCORESTRUCT pPBCoreStructure)
{
	m_pPBCoreStructure = pPBCoreStructure;
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		November 2009
*/
BOOL CSIPModule::onInit(PPBSTRUCT pPBStructure)
{
	RegisterForEvent(PB_APPFOCUSEVENT);
	wcscpy (m_szModName, L"SIP");
	//  Read the Value of the 'FullScreen' Setting
	m_bFullScreen = PB_NO_TASKBAR;
	LPCTSTR pTempFullScreen = GetSettingPtr(L"Configuration\\Screen\\FullScreen", L"Value");
	if(pTempFullScreen)
	{
		if (*pTempFullScreen==L'1')
			m_bFullScreen = PB_NO_TASKBAR;
		else
			m_bFullScreen = PB_WITH_TASKBAR;
	}

	return TRUE;
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		November 2009
*/
void CSIPModule::onDeInit(PPBSTRUCT pPBStructure)
{
	UnRegisterForEvent(PB_APPFOCUSEVENT);
	UnRegisterForEvent (PB_PRIMARY_MESSAGE);
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		October 2009
*/
BOOL CSIPModule::onAttachInstance(PPBSTRUCT pPBStructure, PPBINSTSTRUCT pInstStruct)
{
	//  Read the Value of the 'ResizeOnSIP' Setting
	LPCTSTR pTempResizeOnSIP = GetAppSettingPtr(pInstStruct->instID,L"SIP\\ResizeOnSIP",L"Value");
	//  Read the Value of the 'ScrollTechnique' setting
	LPCTSTR pFingerScrolling = GetAppSettingPtr(pInstStruct->instID,L"Scrolling\\ScrollTechnique", L"Value");

	if (wcsicmp(pFingerScrolling, L"FingerScroll") == 0)
		m_bFingerScrollingEnabled = true;
	else
		m_bFingerScrollingEnabled = false;

	if(pTempResizeOnSIP)
	{
		//  Resize on SIP is not compatible with Finger Scrolling
		if (m_bFingerScrollingEnabled)
		{
			//  Log that Finger scrolling and resize on Sip are not compatible
			Log(PB_LOG_WARNING, 
				L"ResizeOnSIP configuration option is not compatible with Finger Scrolling, brower window will not be resized when the SIP appears", 
				_T(__FUNCTION__), __LINE__);
		}
		else
		{
			if (*pTempResizeOnSIP==L'1')
				RegisterForEvent (PB_PRIMARY_MESSAGE);
		}
	}

	INSTANCE_DATA *pData = new INSTANCE_DATA;
	pData->pSIPController = new CSIP();
	
	//  CSIP() Initialises the Original Position attribute to the current
	//  position of the SIP, use that to set the default values for left and 
	//  top.
	pData->left = pData->pSIPController->GetOriginalPosition().left;
	pData->top = pData->pSIPController->GetOriginalPosition().top;
	pData->sipControl = pData->pSIPController->GetSipControl();
	pData->hwndHTML = pPBStructure->hWnd;
	pData->dwLastSettingChangeTime = 0;

	pInstStruct->pWrappedObj = pData;

	return TRUE;
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		October 2009
*/
BOOL CSIPModule::onReleaseInstance(PPBSTRUCT pPBStructure, PPBINSTSTRUCT pInstStruct)
{
	INSTANCE_DATA *pData = (INSTANCE_DATA*) (pInstStruct->pWrappedObj);
	if (pData)
	{
		if (pData->pSIPController)
		{
			pData->pSIPController->CloseSIP();
			delete pData->pSIPController;
		}
		delete pData;
	}

	return TRUE;
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		October 2009
*/
BOOL CSIPModule::onAppFocus(int iOldID, int iNewID)
{
	//  Consider Initialisation Steps
	if ((iOldID == iNewID) ||
		(iOldID < 0) ||
		(iNewID < 0))
		return TRUE;

	INSTANCE_DATA *pDataNew = (INSTANCE_DATA*) GetObjFromID(iNewID);

	//  Test the new application gaining focus is an application 
	//  using the ScreenOrientation module
	if (!pDataNew)
		return TRUE;

	//  Move the SIP to the coordinates stored in the object instance
	if (pDataNew->pSIPController)
	{
		pDataNew->pSIPController->MoveSip(pDataNew->left, pDataNew->top);
		pDataNew->pSIPController->SetSipControl(pDataNew->hwndHTML, pDataNew->sipControl);
		return TRUE;
	}
	else
		return FALSE;
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		November 2009
*/
BOOL CSIPModule::MetaProc(PBMetaStruct *pbMetaStructure, PPBSTRUCT pPBStructure, void *pParam)
{
	INSTANCE_DATA *pData = (INSTANCE_DATA*) (pParam);
	if (!pData->pSIPController)
		return FALSE;

	//  MoveSIP Meta Tags
	int iMetaValueAsInt = 0;
	if (cmp (pbMetaStructure->lpParameter, L"left"))
	{
		if (m_bFingerScrollingEnabled)
		{	
			Log(PB_LOG_WARNING, L"Moving the SIP is not compatible with Finger scrolling", _T(__FUNCTION__), __LINE__);
			return TRUE;
		}
		iMetaValueAsInt = _wtoi(pbMetaStructure->lpValue);
		pData->left = iMetaValueAsInt;
		return pData->pSIPController->MoveSip(iMetaValueAsInt, -9999);
	}
	else if (cmp(pbMetaStructure->lpParameter, L"top"))
	{
		if (m_bFingerScrollingEnabled)
		{	
			Log(PB_LOG_WARNING, L"Moving the SIP is not compatible with Finger scrolling", _T(__FUNCTION__), __LINE__);
			return TRUE;
		}
		iMetaValueAsInt = _wtoi(pbMetaStructure->lpValue);
		pData->top = iMetaValueAsInt;
		return pData->pSIPController->MoveSip(-9999, iMetaValueAsInt);
	}
	//  SIP Control Meta Tags
	else if (cmp(pbMetaStructure->lpParameter, L"Manual"))
	{
		pData->sipControl = SIP_CONTROL_MANUAL;
		//  WebKit engine sends an indication to the core of editable field focus change
		if (!m_pPBCoreStructure->pSipControlChange(SIP_CONTROL_MANUAL))
		{
			//  IE Engine is in use
			pData->pSIPController->SetSipControl(pData->hwndHTML, SIP_CONTROL_MANUAL);
		}

	}
	else if (cmp(pbMetaStructure->lpParameter, L"Automatic"))
	{
		pData->sipControl = SIP_CONTROL_AUTOMATIC;
		//  WebKit engine sends an indication to the core of editable field focus change
		if (!m_pPBCoreStructure->pSipControlChange(SIP_CONTROL_AUTOMATIC))
		{
			//  IE Engine is in use
			pData->pSIPController->SetSipControl(pData->hwndHTML, SIP_CONTROL_AUTOMATIC);
		}
	}
	else
	{
		// Unrecognised tag
		Log(PB_LOG_WARNING, L"Unrecognised Meta Tag Provided to MoveSIP Module",
			_T(__FUNCTION__), __LINE__);
		return FALSE;
	}

	return TRUE;
}

BOOL CSIPModule::onPrimaryMessage (MSG *pmsg, int ncurrid)
{
	int iDebounceDuration = 0;

	//  In multiple instances we only want to do this when we are the focused
	//  application
	INSTANCE_DATA *pdata = (INSTANCE_DATA*) GetObjFromID (ncurrid);
	if (!pdata)
	{
		return FALSE;
	}

	switch (pmsg->message)
	{
	case WM_SETTINGCHANGE:
		if (pmsg->wParam == SPI_SETSIPINFO)
		{
			//  The SIP has appeared
			if (pdata->dwLastSettingChangeTime + iDebounceDuration < pmsg->time)
			{
				pdata->dwLastSettingChangeTime = pmsg->time;
				m_pPBCoreStructure->pResizePB(TRUE, TRUE);
			}
		}
		break;
	}
	//  Do not block this message from proceeding further in the pump
	return FALSE;
}