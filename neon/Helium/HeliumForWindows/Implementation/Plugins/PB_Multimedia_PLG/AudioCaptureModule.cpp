//#include "StdAfx.h"
#include "AudioCaptureModule.h"



/**
* \author	Paul Henderson (PH, PTJK76)
* \date		April 2012 Initial Creation, PH
*/
BOOL CAudioCaptureModule::onInit(PPBSTRUCT pPBStructure)
{
	wcscpy(m_szModName,L"AudioCapture");
	m_pDestination = NULL;
	//m_bDestinationSet = false;
	m_FileTransferIMO = CreateIMO(L"filetransfer");
	if(m_FileTransferIMO){
		if(SetIMOCallBack (m_FileTransferIMO,IMOCBFunc,(LPARAM)this)){
			RegisterForEvent(PB_BROWSER_BEFORE_NAV_EVENT);
			RegisterForEvent(RHO_APPFOCUS_EVENT);
			return TRUE;
		}
	}
	return FALSE;
}

/**
* \author	Paul Henderson (PH, PTJK76)
* \date		April 2012 Initial Creation, PH
*/
void CAudioCaptureModule::onDeInit(PPBSTRUCT pPBStructure)
{
	DeleteIMO(m_FileTransferIMO);
	UnRegisterForEvent(PB_BROWSER_BEFORE_NAV_EVENT);
	UnRegisterForEvent(RHO_APPFOCUS_EVENT);
	delete [] m_pDestination;
}


/**
* \author	Paul Henderson (PH, PTJK76)
* \date		April 2012 Initial Creation, PH
*/
BOOL CAudioCaptureModule::onAttachInstance(PPBSTRUCT pPBStructure,PPBINSTSTRUCT pInstStruct)
{
	Log(PB_LOG_INFO,L"Attaching Instance",_T(__FUNCTION__),__LINE__);
		
	pInstStruct->pWrappedObj = 
		(CAudioCapture*) new CAudioCapture(this, (pPBStructure->bLaunchingAppHasFocus == TRUE));
	
	if(pInstStruct->pWrappedObj)
		return TRUE;
	else
		return FALSE;
}

/**
* \author	Paul Henderson (PH, PTJK76)
* \date		April 2012 Initial Creation, PH
*/
BOOL CAudioCaptureModule::onReleaseInstance(PPBSTRUCT pPBStructure,PPBINSTSTRUCT pInstStruct)
{
	Log(PB_LOG_INFO,L"Deleting Instance",_T(__FUNCTION__),__LINE__);
	delete (CAudioCapture*)pInstStruct->pWrappedObj;
	pInstStruct->pWrappedObj = NULL;
	return TRUE;
}


/**
* \author	Paul Henderson (PH, PTJK76)
* \date		April 2012 Initial Creation, PH
*/
BOOL CAudioCaptureModule::onBeforeNavigate(int iInstID)
{
	
	//  set the TransferEvent navigation URIs to blank
	CAudioCapture *pAudioCapture = (CAudioCapture *)GetObjFromID(iInstID);
	if(pAudioCapture)
	{
		pAudioCapture->Cancel();//cancel any ongoing captures
		SetIMOProperty(m_FileTransferIMO, L"TransferEvent", L"");
		pAudioCapture->ResetToDefaults();
		delete [] m_pDestination;
		m_pDestination = NULL;
					
	}
	return TRUE;
}

//called from another thread 
int CALLBACK CAudioCaptureModule::IMOCBFunc(PVARSTRUCT pVars,int iTABID,LPARAM lParam)
{
	CAudioCaptureModule *pAudioCaptureModule = (CAudioCaptureModule *)lParam;
	//do something with the Module object
	return IMO_PASS_ON;//allow the navigation to go through to RE
	
}

BOOL CAudioCaptureModule::onRhoAppFocus(bool bActivate, int iInstID)
{
	CAudioCapture *pAud = NULL;
	pAud = (CAudioCapture*) GetObjFromID(iInstID);
	if (pAud != NULL)
	{
		return pAud->ApplicationFocusChange(bActivate);
	}
	else
		return FALSE;
};


BOOL CAudioCaptureModule::MetaProc(PBMetaStruct *pbMetaStructure, PPBSTRUCT pPBStructure,VOID* pParam)
{
	CAudioCapture *pAudio = (CAudioCapture*)pParam;

	// The CAudioCapture object is made aware of whether we have focus or not and will not
	// acess the hardware unless we are the foreground application.
	// The FileTransfer IMO is local to this application, not shared like the audio device
	// so we don't need to care about focus when setting the IMO properties.
	
	if(pAudio){

		///////////////////////////////////////////////////////////////////////////////
		// Methods
		///////////////////////////////////////////////////////////////////////////////
		if(cmp(pbMetaStructure->lpParameter, L"Start")){
			pAudio->Start();
		}
		else if(cmp(pbMetaStructure->lpParameter, L"Stop")){
			pAudio->Stop();
		}
		else if(cmp(pbMetaStructure->lpParameter, L"Cancel")){
			pAudio->Cancel();
		}
		
		///////////////////////////////////////////////////////////////////////////////
		// Properties
		///////////////////////////////////////////////////////////////////////////////
		else if(cmp(pbMetaStructure->lpParameter, L"Duration")){
			pAudio->SetDuration(_wtoi(pbMetaStructure->lpValue));
		}
		else if(cmp(pbMetaStructure->lpParameter, L"Destination")){
			delete [] m_pDestination;
			m_pDestination = pAudio->setString(pbMetaStructure->lpValue);
		}
		else if(cmp(pbMetaStructure->lpParameter, L"Username")){
			if (wcslen(pbMetaStructure->lpValue))
			return SetIMOProperty(m_FileTransferIMO,L"UserName",pbMetaStructure->lpValue);
		}
		else if(cmp(pbMetaStructure->lpParameter, L"Password")){
			if (wcslen(pbMetaStructure->lpValue))
			return SetIMOProperty(m_FileTransferIMO,L"Password",pbMetaStructure->lpValue);
		}
		else if(cmp(pbMetaStructure->lpParameter, L"Name")){
			pAudio->SetFilename(pbMetaStructure->lpValue);
		}

		else if(cmp(pbMetaStructure->lpParameter, L"AudioSaveEvent")){
			//set the nav string for the filetransfer module
			if (pbMetaStructure->lpValue)
				return SetIMOProperty(m_FileTransferIMO,L"TransferEvent",pbMetaStructure->lpValue);
		}


		else{
			//  Unrecognized tag
			Log(PB_LOG_WARNING, L"Unrecognised Meta Tag Provided to Audio Capture Module",
			_T(__FUNCTION__), __LINE__);
			return FALSE;
		}
		return true;
	}
	return false;


}


//only ever called by the AudioCapture Thread
BOOL CAudioCaptureModule::SendAudioFile(LPWSTR pFileToSend)
{
	//pFileToSend is the source for the file transfer IMO
	
	//need to set the destination each time I send via fileTransfer
	//due the protocol getting reset to file after a transfer 
	if(m_FileTransferIMO && m_pDestination){
		if(wcsstr(m_pDestination,L"FILE")){ 
			SetIMOProperty(m_FileTransferIMO, L"SetFileDestination", L"TRUE");
		}
		else{
			SetIMOProperty(m_FileTransferIMO, L"SetFileDestination", L"FALSE");
		}
		SetIMOProperty(m_FileTransferIMO,L"Overwrite",L"true");
		SetIMOProperty(m_FileTransferIMO,L"Destination",m_pDestination);
	
		
		WCHAR tcFormattedSource[MAXURL];
		wsprintf(tcFormattedSource, L"file://%s", pFileToSend);
		Log(PB_LOG_INFO, tcFormattedSource,	_T(__FUNCTION__), __LINE__);
		if(SetIMOProperty(m_FileTransferIMO,L"Source", tcFormattedSource))
		{
			return CallIMOMethod(m_FileTransferIMO,L"Transfer");
			
		}
		
	}
	return FALSE;
}
