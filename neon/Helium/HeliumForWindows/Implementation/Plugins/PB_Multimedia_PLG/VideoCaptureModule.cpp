//#include "StdAfx.h"
#include "VideoCaptureModule.h"



/**
* \author	Paul Henderson (PH, PTJK76)
* \date		April 2012 Initial Creation, PH
*/
BOOL CVideoCaptureModule::onInit(PPBSTRUCT pPBStructure)
{
	wcscpy(m_szModName,L"VideoCapture");
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
void CVideoCaptureModule::onDeInit(PPBSTRUCT pPBStructure)
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
BOOL CVideoCaptureModule::onAttachInstance(PPBSTRUCT pPBStructure,PPBINSTSTRUCT pInstStruct)
{
	Log(PB_LOG_INFO,L"Attaching Instance",_T(__FUNCTION__),__LINE__);
		
	pInstStruct->pWrappedObj = 
		(CVideoCapture*) new CVideoCapture(pPBStructure->hInstance, pPBStructure->hWnd, this, (pPBStructure->bLaunchingAppHasFocus==TRUE));
	
	if(pInstStruct->pWrappedObj)
		return TRUE;
	else
		return FALSE;
}

/**
* \author	Paul Henderson (PH, PTJK76)
* \date		April 2012 Initial Creation, PH
*/
BOOL CVideoCaptureModule::onReleaseInstance(PPBSTRUCT pPBStructure,PPBINSTSTRUCT pInstStruct)
{
	Log(PB_LOG_INFO,L"Deleting Instance",_T(__FUNCTION__),__LINE__);
	delete (CVideoCapture*)pInstStruct->pWrappedObj;
	pInstStruct->pWrappedObj = NULL;
	return TRUE;
}


/**
* \author	Paul Henderson (PH, PTJK76)
* \date		April 2012 Initial Creation, PH
*/
BOOL CVideoCaptureModule::onBeforeNavigate(int iInstID)
{
	
	//  set the TransferEvent navigation URIs to blank
	CVideoCapture *pVideoCapture = (CVideoCapture *)GetObjFromID(iInstID);
	if(pVideoCapture)
	{
		pVideoCapture->Cancel();//cancel any ongoing captures
		SetIMOProperty(m_FileTransferIMO, L"TransferEvent", L"");
					
	}
	return TRUE;
}

//called from another thread 
int CALLBACK CVideoCaptureModule::IMOCBFunc(PVARSTRUCT pVars,int iTABID,LPARAM lParam)
{
	CVideoCaptureModule *pVideoCaptureModule = (CVideoCaptureModule *)lParam;
	//do something with the Module object
	return IMO_PASS_ON;//allow the navigation to go through to RE
	
}

BOOL CVideoCaptureModule::onRhoAppFocus(bool bActivate, int iInstID)
{
	CVideoCapture *pAud = NULL;
	pAud = (CVideoCapture*) GetObjFromID(iInstID);
	if (pAud != NULL)
	{
		return pAud->ApplicationFocusChange(bActivate);
	}
	else
		return FALSE;
};


BOOL CVideoCaptureModule::MetaProc(PBMetaStruct *pbMetaStructure, PPBSTRUCT pPBStructure,VOID* pParam)
{
	Log(PB_LOG_WARNING, L"The Video Capture API from the RhoElements Extension has been deprecated in 4.0, please transition your applications to use the common Video Capture API for future releases", _T(__FUNCTION__), __LINE__);

	CVideoCapture *pVideo = (CVideoCapture*)pParam;

	// The CVideoCapture object is made aware of whether we have focus or not and will not
	// acess the hardware unless we are the foreground application.
	// The FileTransfer IMO is local to this application, not shared like the audio device
	// so we don't need to care about focus when setting the IMO properties.
	
	if(pVideo){

			if(pVideo->IsCameraPresent())
			{

            ///////////////////////////////////////////////////////////////////////////////
            // Methods
            ///////////////////////////////////////////////////////////////////////////////
            if(cmp(pbMetaStructure->lpParameter, L"Start")){
                pVideo->Start();
            }
            else if(cmp(pbMetaStructure->lpParameter, L"Stop")){
                pVideo->Stop();
            }
            else if(cmp(pbMetaStructure->lpParameter, L"Cancel")){
                pVideo->Cancel();
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Properties
            ///////////////////////////////////////////////////////////////////////////////
            else if(cmp(pbMetaStructure->lpParameter, L"Duration")){
                pVideo->SetDuration(_wtoi(pbMetaStructure->lpValue));
            }
            else if(cmp(pbMetaStructure->lpParameter, L"Destination")){
                m_pDestination = pVideo->setString(pbMetaStructure->lpValue);
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
                pVideo->SetFilename(pbMetaStructure->lpValue);
            }

		else if(cmp(pbMetaStructure->lpParameter, L"VideoSaveEvent")){
			//set the nav string for the filetransfer module
			if (pbMetaStructure->lpValue)
				return SetIMOProperty(m_FileTransferIMO,L"TransferEvent",pbMetaStructure->lpValue);
		}


            else{
                //  Unrecognized tag
                Log(PB_LOG_WARNING, L"Unrecognised Meta Tag Provided to Video Capture Module",
                    _T(__FUNCTION__), __LINE__);
                return FALSE;
            }
            return true;
			}
			else
			{
				 Log(PB_LOG_WARNING, L"No camera found on this device, hence your request cannot be processed!!",
                    _T(__FUNCTION__), __LINE__);
			}
        }
    
    return false;


}


//only ever called by the VideoCapture Thread
BOOL CVideoCaptureModule::SendVideoFile(LPWSTR pFileToSend)
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
