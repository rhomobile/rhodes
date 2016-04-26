//#include "StdAfx.h"
#include "MediaPlayerModule.h"



/**
* \author	John Heywood (JH, VBC863)
* \date		November 2012 Initial Creation, JH
*/
BOOL CMediaPlayerModule::onInit(PPBSTRUCT pPBStructure)
{
	wcscpy(m_szModName,L"MediaPlayer");
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
* \author	John Heywood (JH, VBC863)
* \date		November 2012 Initial Creation, JH
*/
void CMediaPlayerModule::onDeInit(PPBSTRUCT pPBStructure)
{
	DeleteIMO(m_FileTransferIMO);
	UnRegisterForEvent(PB_BROWSER_BEFORE_NAV_EVENT);
	UnRegisterForEvent(RHO_APPFOCUS_EVENT);
}


/**
* \author	John Heywood (JH, VBC863)
* \date		November 2012 Initial Creation, JH
*/
BOOL CMediaPlayerModule::onAttachInstance(PPBSTRUCT pPBStructure,PPBINSTSTRUCT pInstStruct)
{
	Log(PB_LOG_INFO,L"Attaching Instance",_T(__FUNCTION__),__LINE__);
		
	pInstStruct->pWrappedObj = 
		(CMediaPlayer*) new CMediaPlayer(this, (pPBStructure->bLaunchingAppHasFocus==TRUE));
	
	if(pInstStruct->pWrappedObj)
		return TRUE;
	else
		return FALSE;
}

/**
* \author	John Heywood (JH, VBC863)
* \date		November 2012 Initial Creation, JH
*/
BOOL CMediaPlayerModule::onReleaseInstance(PPBSTRUCT pPBStructure,PPBINSTSTRUCT pInstStruct)
{
	Log(PB_LOG_INFO,L"Deleting Instance",_T(__FUNCTION__),__LINE__);
	delete (CMediaPlayer*)pInstStruct->pWrappedObj;
	pInstStruct->pWrappedObj = NULL;
	return TRUE;
}


/**
* \author	John Heywood (JH, VBC863)
* \date		November 2012 Initial Creation, JH
*/
BOOL CMediaPlayerModule::onBeforeNavigate(int iInstID)
{
	
	//  set the TransferEvent navigation URIs to blank
	CMediaPlayer *pMediaPlayer = (CMediaPlayer *)GetObjFromID(iInstID);
	if(pMediaPlayer)
	{
		pMediaPlayer->Stop();
		pMediaPlayer->SetFilename(NULL);
		pMediaPlayer->SetType(MP_TYPE_INVALID);
		SetIMOProperty(m_FileTransferIMO, L"TransferEvent", L"");
					
	}
	return TRUE;
}

//called from another thread 
int CALLBACK CMediaPlayerModule::IMOCBFunc(PVARSTRUCT pVars,int iTABID,LPARAM lParam)
{
	CMediaPlayerModule *pMediaPlayerModule = (CMediaPlayerModule *)lParam;
	//do something with the Module object
	return IMO_PASS_ON;//allow the navigation to go through to RE
	
}

BOOL CMediaPlayerModule::onRhoAppFocus(bool bActivate, int iInstID)
{
	CMediaPlayer *pAud = NULL;
	pAud = (CMediaPlayer*) GetObjFromID(iInstID);
	if (pAud != NULL)
	{
		return pAud->ApplicationFocusChange(bActivate);
	}
	else
		return FALSE;
};


BOOL CMediaPlayerModule::MetaProc(PBMetaStruct *pbMetaStructure, PPBSTRUCT pPBStructure,VOID* pParam)
{
	Log(PB_LOG_WARNING, L"The Media Player API from the RhoElements Extension has been deprecated in 4.0, please transition your applications to use the common Media Player API for future releases", _T(__FUNCTION__), __LINE__);

	CMediaPlayer *pPlayer = (CMediaPlayer*)pParam;

	// The CMediaPlayer object is made aware of whether we have focus or not and will not
	// acess the hardware unless we are the foreground application.
	// The FileTransfer IMO is local to this application, not shared like the audio device
	// so we don't need to care about focus when setting the IMO properties.
	
	if(pPlayer){

		///////////////////////////////////////////////////////////////////////////////
		// Methods
		///////////////////////////////////////////////////////////////////////////////
		if(cmp(pbMetaStructure->lpParameter, L"Start")){
			pPlayer->Start();
		}
		else if(cmp(pbMetaStructure->lpParameter, L"Stop")){
			pPlayer->Stop();
		}
		
		///////////////////////////////////////////////////////////////////////////////
		// Properties
		///////////////////////////////////////////////////////////////////////////////
		else if(cmp(pbMetaStructure->lpParameter, L"Type")){
			pPlayer->SetType(pbMetaStructure->lpValue);
		}
		else if(cmp(pbMetaStructure->lpParameter, L"Filename")){
			pPlayer->SetFilename(pbMetaStructure->lpValue);
		}

		else{
			//  Unrecognized tag
			Log(PB_LOG_WARNING, L"Unrecognised Meta Tag Provided to Player Capture Module",
			_T(__FUNCTION__), __LINE__);
			return FALSE;
		}
		return true;
	}
	return false;


}


//only ever called by the MediaPlayer Thread
bool CMediaPlayerModule::GetMediaFile(CMediaPlayer *pPlayer, IMOEVENTPROC callback)
{
	LPCWSTR lpzSource = pPlayer->GetFilename();
	LPCWSTR lpzLocalFilename = wcsrchr(lpzSource, L'\\');
	if (!lpzLocalFilename)
	{
		lpzLocalFilename = wcsrchr(lpzSource, L'/');
	}
	if (!lpzLocalFilename)
	{
		return false;
	}

	LPWSTR lpzDestination = new WCHAR[wcslen(lpzLocalFilename)+1];
	wcscpy(lpzDestination, lpzLocalFilename);
	*lpzDestination = L'\\';

	SetIMOCallBack (m_FileTransferIMO, callback, (LPARAM) pPlayer);
	SetIMOProperty (m_FileTransferIMO, L"transferevent", L"Javascript://IMO");
	SetIMOProperty (m_FileTransferIMO, L"source", lpzSource);
	SetIMOProperty (m_FileTransferIMO, L"destination", lpzDestination);
	SetIMOProperty (m_FileTransferIMO, L"overwrite", L"true");

	pPlayer->SetFilename(lpzDestination);
	delete [] lpzDestination;
	
	return (CallIMOMethod (m_FileTransferIMO, L"transfer")==TRUE);
}
