#include "SignatureModule.h"

/**
* \author	Paul Henderson (PH, PTJK76)
* \date		November 2009 Initial Creation, PH
*/
BOOL CSignatureModule::onInit(PPBSTRUCT pPBStructure)
{
	wcscpy(m_szModName,L"SignatureCapture");
	m_bDestinationSet = false;
	m_FileTransferIMO = CreateIMO(L"filetransfer");
	if(m_FileTransferIMO){
		if(SetIMOCallBack (m_FileTransferIMO,IMOCBFunc,(LPARAM)this)){
			RegisterForEvent(PB_BROWSER_BEFORE_NAV_EVENT);
			RegisterForEvent(PB_APPFOCUSEVENT);
			return TRUE;
		}
	}
	return FALSE;
}

/**
* \author	Paul Henderson (PH, PTJK76)
* \date		November 2009 Initial Creation, PH
*/
void CSignatureModule::onDeInit(PPBSTRUCT pPBStructure)
{
	DeleteIMO(m_FileTransferIMO);
	UnRegisterForEvent(PB_BROWSER_BEFORE_NAV_EVENT);
	UnRegisterForEvent(PB_APPFOCUSEVENT);
}

/**
* \author	Paul Henderson (PH, PTJK76)
* \date		November 2009 Initial Creation, PH
*/
BOOL CSignatureModule::onAttachInstance(PPBSTRUCT pPBStructure,PPBINSTSTRUCT pInstStruct)
{
	Log(PB_LOG_INFO,L"Attaching Instance",_T(__FUNCTION__),__LINE__);
		
	pInstStruct->pWrappedObj = 
		(CSignature*) new CSignature(	pPBStructure->hInstance,
										pPBStructure->hWnd, 
										pInstStruct->instID, 
										this,
										m_pCoreStructure->bBuiltAsRhodesExtension);
	
	if(pInstStruct->pWrappedObj)
		return TRUE;
	else
		return FALSE;
}

/**
* \author	Paul Henderson (PH, PTJK76)
* \date		November 2009 Initial Creation, PH
*/
BOOL CSignatureModule::onReleaseInstance(PPBSTRUCT pPBStructure,PPBINSTSTRUCT pInstStruct)
{
	Log(PB_LOG_INFO,L"Deleting Instance",_T(__FUNCTION__),__LINE__);
	delete (CSignature*)pInstStruct->pWrappedObj;
	pInstStruct->pWrappedObj = NULL;
	return TRUE;
}


/**
* \author	Paul Henderson (PH, PTJK76)
* \date		November 2009 Initial Creation, PH
*/
BOOL CSignatureModule::onMessage (HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam, void *pobject)
{
	CSignature *pSig = (CSignature *)pobject; 

	// GD - forced to return FALSE to enable other modules to get the message as well
	PostMessage(pSig->m_hSigBox,WM_MOUSEOUT,0,0);
	return FALSE;
}

/**
* \author	Paul Henderson (PH, PTJK76)
* \date		November 2009 Initial Creation, PH
*/
BOOL CSignatureModule::onBeforeNavigate(int iInstID)
{
	//  If we are navigating to a new page hide the signature capture box and 
	//  set the TransferEvent navigation URIs to blank
	CSignature *pSignature = (CSignature *)GetObjFromID(iInstID);
	if(pSignature)
	{
		pSignature->SetSignatureCaptureURI(L"");
		pSignature->SetVectorNavigationURI(L"");
		pSignature->DoVectors(FALSE);
		SetIMOProperty(m_FileTransferIMO, L"TransferEvent", L"");
		pSignature->Hide();
		pSignature->ClearScreen();
			
	}
	return TRUE;
}
/**
* \author	Paul Henderson (PH, PTJK76)
* \date		November 2009 Initial Creation, PH
*/
BOOL CSignatureModule::onAppFocus(int iOldID, int iNewID)
{
	//  To Be Implemented
	return TRUE;
}

/**
* \author	Paul Henderson (PH, PTJK76)
* \author	Darryn Campbell (DCC, JRQ768)
* \date		November 2009 Initial Creation, PH
* \date		December 2009 Added extra parameters to bring into line with PB3 
*			documentation, DCC.
*/
BOOL CSignatureModule::MetaProc(PBMetaStruct *pbMetaStructure, PPBSTRUCT pPBStructure, VOID* pParam)
{
	Log(PB_LOG_WARNING, L"The Siguature Capture API has been deprecated in 4.0, please transition your applications to use the common Signature API for future releases", _T(__FUNCTION__), __LINE__);

	CSignature *pSig = (CSignature*)pParam;
	Log(PB_LOG_INFO, pbMetaStructure->lpParameter, _T(__FUNCTION__), __LINE__);

	//  Process the received Meta Tag
	if(cmp(pbMetaStructure->lpParameter, L"Visibility"))
	{
		if(cmp(pbMetaStructure->lpValue, L"Visible")) 
		{
			pSig->Initialise();
			return pSig->Show();
		}
		else if(cmp(pbMetaStructure->lpValue, L"Hidden")) 
		{
			return pSig->Hide();
		}
	}
	else if (cmp(pbMetaStructure->lpParameter, L"Enabled")||cmp(pbMetaStructure->lpParameter,L"Enable"))
	{
		//  In order to maintain compatibility with PB2.x we also need to 
		//  condider enabled / disabled as well as visibility.
		pSig->Initialise();
		return pSig->Show();
	}
	else if (cmp(pbMetaStructure->lpParameter, L"Disabled"))
	{
		return pSig->Hide();
	}
	else if(cmp(pbMetaStructure->lpParameter, L"Border"))
	{
		if(cmp(pbMetaStructure->lpValue, L"Visible")) 
		{
			return pSig->SetBorder(TRUE);
		}
		else if(cmp(pbMetaStructure->lpValue, L"Hidden")) 
		{
			return pSig->SetBorder(FALSE);
		}
	}
	else if(cmp(pbMetaStructure->lpParameter, L"Clear")) 
	{
		return pSig->ClearScreen();
	}
	else if(cmp(pbMetaStructure->lpParameter, L"Width")) 
	{
		if (pbMetaStructure->lpValue)
			return pSig->SetWidth(_wtoi(pbMetaStructure->lpValue));
	}
	else if(cmp(pbMetaStructure->lpParameter, L"Height")) 
	{
		if (pbMetaStructure->lpValue)
			return pSig->SetHeight(_wtoi(pbMetaStructure->lpValue));
	}
	else if(cmp(pbMetaStructure->lpParameter, L"Left")) 
	{
		if (pbMetaStructure->lpValue)
			return pSig->SetXpos(_wtoi(pbMetaStructure->lpValue));
	}
	else if(cmp(pbMetaStructure->lpParameter, L"Top")) 
	{
		if (pbMetaStructure->lpValue)
			return pSig->SetYpos(_wtoi(pbMetaStructure->lpValue));
	}
	else if(cmp(pbMetaStructure->lpParameter, L"Penwidth")) 
	{
		if (pbMetaStructure->lpValue)
			return pSig->SetLineWidth(_wtoi(pbMetaStructure->lpValue));
	}
	else if(cmp(pbMetaStructure->lpParameter, L"Pencolor")) 
	{
		if (pbMetaStructure->lpValue)
			return pSig->SetLineColor(pbMetaStructure->lpValue);
	}
	else if(cmp(pbMetaStructure->lpParameter, L"BGColor")) 
	{
		if (pbMetaStructure->lpValue)
			return pSig->SetBgColor(pbMetaStructure->lpValue);
	}
	else if(cmp(pbMetaStructure->lpParameter, L"Destination")) 
	{
		m_bDestinationSet = true;
		if (pbMetaStructure->lpValue)
			return SetIMOProperty(m_FileTransferIMO,L"Destination",pbMetaStructure->lpValue); 
	}
	else if(cmp(pbMetaStructure->lpParameter, L"Name")) 
	{
		if (pbMetaStructure->lpValue)
			return pSig->SetName(pbMetaStructure->lpValue);	
	}
	else if(cmp(pbMetaStructure->lpParameter, L"Username"))
	{
		if (pbMetaStructure->lpValue)
			return SetIMOProperty(m_FileTransferIMO,L"UserName",pbMetaStructure->lpValue);
	}
	else if(cmp(pbMetaStructure->lpParameter, L"Password"))
	{
		if (pbMetaStructure->lpValue)
			return SetIMOProperty(m_FileTransferIMO,L"Password",pbMetaStructure->lpValue);
	}
	else if(cmp(pbMetaStructure->lpParameter, L"Protocol"))
	{
		if (pbMetaStructure->lpValue)
			return SetIMOProperty(m_FileTransferIMO,L"Protocol",pbMetaStructure->lpValue);
	}
	else if(cmp(pbMetaStructure->lpParameter, L"SignatureSaveEvent")) 
	{
		//set the nav string for the filetransfer module
		if (pbMetaStructure->lpValue)
			return SetIMOProperty(m_FileTransferIMO,L"TransferEvent",pbMetaStructure->lpValue);
	}
	else if(cmp(pbMetaStructure->lpParameter, L"SignatureCaptureEvent"))
	{
		if (pbMetaStructure->lpValue)
			return pSig->SetSignatureCaptureURI(pbMetaStructure->lpValue);
	}
	else if(cmp(pbMetaStructure->lpParameter, L"VectorEvent"))
	{
		//  Set the signature capture module to vector mode
		if (pbMetaStructure->lpValue)
		{
			pSig->SetVectorNavigationURI(pbMetaStructure->lpValue);
			pSig->DoVectors(TRUE);
			return TRUE;
		}
	}
	else if(cmp(pbMetaStructure->lpParameter, L"capture")) 
	{
		BOOL bRetVal = TRUE;
		//  Capture will save the signature as a bitmap and transfer this 
		//  to a remote destination if a destination parameter has been set.
		//  Create a Bitmap based on the Captured Signature
		if(pSig->CreateDIBitmap() == FALSE)
		{
			return FALSE;
		}

		//  Call FileTransfer Plug-in to transfer the captured bitmap to the 
		//  specified destination.
		if(m_FileTransferIMO && m_bDestinationSet)
		{
			bRetVal = FALSE;
			TCHAR tcFormattedSource[MAXURL];
			wsprintf(tcFormattedSource, L"file://\\%s", pSig->m_lpSzFilename);
			if(SetIMOProperty(m_FileTransferIMO,L"Source", tcFormattedSource))
			{
				if(SetIMOProperty(m_FileTransferIMO,L"Overwrite",L"true"))
				{
					if (SetIMOProperty(m_FileTransferIMO, L"SetFileDestination", L"FALSE"))
					{
						bRetVal = CallIMOMethod(m_FileTransferIMO,L"Transfer");
					}
				}
			}
		}

		pSig->CaptureSignature();

		//  The transfer to FileTransfer has failed
		return bRetVal;
	}
	else
	{
		//  Unrecognized tag
		Log(PB_LOG_WARNING, L"Unrecognised Meta Tag Provided to Signature Capture Module",
		_T(__FUNCTION__), __LINE__);
		return FALSE;
	}

	return FALSE;
}

//called from another thread 
int CALLBACK CSignatureModule::IMOCBFunc(PVARSTRUCT pVars,int iTABID,LPARAM lParam)
{
	CSignatureModule *pSigModule = (CSignatureModule *)lParam;
	//do something with the Module object
	return IMO_PASS_ON;//allow the navigation to go through to Browser
	//MessageBox(m_pCurrentPBstruct->hWnd,(LPTSTR)pVars->pStr,L"Transferred",MB_OK);
}