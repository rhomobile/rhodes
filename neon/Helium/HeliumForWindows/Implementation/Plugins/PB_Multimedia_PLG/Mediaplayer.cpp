#include "MediaPlayer.h"
#include "MediaPlayerModule.h"

#define ID_VIEW_FULLSCREEN		40016

HANDLE	CMediaPlayer::m_hPlayThread = NULL;
HANDLE	CMediaPlayer::m_hPlayProcess = NULL;

/**
* \author	John Heywood (JH, VBC863)
* \date		November 2012 Initial Creation, JH
*/
CMediaPlayer::CMediaPlayer(CMediaPlayerModule *pModule, bool bHasFocus)
{
	m_pModule			= pModule;
	m_lpzFilename		= NULL;
	m_eType				= MP_TYPE_INVALID;
	m_bAppHasFocus		= bHasFocus;	
	m_bDeleteMediaFile	= FALSE;
}


/**
* \author	John Heywood (JH, VBC863)
* \date		November 2012 Initial Creation, JH
*/
CMediaPlayer::~CMediaPlayer()
{
	if (m_hPlayProcess) // video playing
	{
		TerminateProcess(m_hPlayProcess,0);
		m_hPlayProcess = NULL;
	}
	else if (m_hPlayThread) // audio playing
	{
		PlaySound(NULL, NULL, 0);
		if (WaitForSingleObject(m_hPlayThread, 500) == WAIT_TIMEOUT)
		{
			TerminateThread(m_hPlayThread, -1);
		}
		m_hPlayThread = NULL;
	}
}


/**
* \author	John Heywood (JH, VBC863)
* \date		November 2012 Initial Creation, JH
*/
LPWSTR CMediaPlayer::SetFilename(LPCWSTR lpFilename)
{
	if (m_hPlayThread)
	{
		m_pModule->Log(PB_LOG_WARNING, L"Unable to set filename while playing",	_T(__FUNCTION__), __LINE__);
	}
	else
	{
		if (m_lpzFilename)
		{
			delete [] m_lpzFilename;
			m_lpzFilename = NULL;
		}

		if (lpFilename)
			m_lpzFilename = setString(lpFilename);
	}
	return m_lpzFilename;
}


/**
* \author	John Heywood (JH, VBC863)
* \date		November 2012 Initial Creation, JH
*/
void CMediaPlayer::Start(void)
{
	if (!m_bAppHasFocus) // if we aren't focussed we can't use the hardware so ignore
		return;

	if ((m_lpzFilename) && (!m_hPlayThread))
	{
		if (m_eType == MP_TYPE_VIDEO)
		{
			// WM/CEPlayer handles streamed video so we can just pass it the URI
			launchPlayer();
		}
		else if (m_eType == MP_TYPE_AUDIO)
		{
			// PlaySound can only play local files
			if (wcsnicmp(L"file://", m_lpzFilename, 7) == 0)
			{
				// local file - just need to remove the file://
				LPWSTR lpzTemp = m_lpzFilename;
				m_lpzFilename = setString(m_lpzFilename + 7);
				if (lpzTemp)
				{
					delete [] lpzTemp;
					lpzTemp = NULL;
				}
				m_bDeleteMediaFile = FALSE;

				// No file transfer to do so we can play it straight away
				playLocalAudio();
			}
			else if ((!wcsnicmp (m_lpzFilename, L"http://", 7)) || (!wcsnicmp (m_lpzFilename, L"ftp://", 6)))
			{
				// remote file (http:// or ftp://)
				m_bDeleteMediaFile = TRUE;
				m_pModule->GetMediaFile(this, &CMediaPlayer::getFileIMOCallback);
			}
		}
	}
}


/**
* \author	John Heywood (JH, VBC863)
* \date		November 2012 Initial Creation, JH
*/
void CMediaPlayer::Stop(void)
{
	if (m_hPlayThread)
	{
		switch (m_eType)
		{
		case MP_TYPE_AUDIO:
			PlaySound(NULL, NULL, 0);
			if (WaitForSingleObject(m_hPlayThread, 500) == WAIT_TIMEOUT)
			{
				// if the thread closes normally it will close and reset the handle, we
				// only need to do it if we have to terminate the thread.
				TerminateThread(m_hPlayThread, -1);
				CloseHandle(m_hPlayThread);
				m_hPlayThread = NULL;
			}
			break;

		case MP_TYPE_VIDEO:
			// Even if the video has finished playing and the player window has closed
			// the player will still be running, and the file will be locked
			TerminateProcess(m_hPlayProcess, -1);
			WaitForSingleObject(m_hPlayProcess, 500);
			CloseHandle(m_hPlayThread);
			m_hPlayThread = NULL;
			CloseHandle(m_hPlayProcess);
			m_hPlayProcess = NULL;
			break;

		default:
			return;
		}
		if (m_bDeleteMediaFile)
			DeleteFile(m_lpzFilename);
	}
}


/**
* static function to kill the player thread so that VideoCapture can unlock
* any in-use media files
*/
void CMediaPlayer::KillPlayer()
{
	if (m_hPlayProcess)
	{
		TerminateProcess(m_hPlayProcess, -1);
		WaitForSingleObject(m_hPlayProcess, 500);
		CloseHandle(m_hPlayProcess);
		m_hPlayProcess = NULL;
	}
	if (m_hPlayThread)
	{
		TerminateThread(m_hPlayThread, -1);
		CloseHandle(m_hPlayThread);
		m_hPlayThread = NULL;
	}
}


/**
* \author	John Heywood (JH, VBC863)
* \date		November 2012 Initial Creation, JH
*/
DWORD WINAPI CMediaPlayer::playThreadProc(LPVOID lpParam)
{
	DWORD dwRet = S_OK;
	CMediaPlayer *pMediaPlayer = (CMediaPlayer*)lpParam;
	if (pMediaPlayer)
	{
		if (!PlaySound(pMediaPlayer->m_lpzFilename, NULL, SND_FILENAME|SND_SYNC|SND_NODEFAULT))
			dwRet = S_FALSE;

		CloseHandle(pMediaPlayer->m_hPlayThread);
		pMediaPlayer->m_hPlayThread = NULL;
	}
	return dwRet;
}


/**
* \author	John Heywood (JH, VBC863)
* \date		November 2012 Initial Creation, JH
*/
bool CMediaPlayer::playLocalAudio()
{
	m_hPlayThread = CreateThread(NULL, 0, playThreadProc, this, 0, NULL);
	return (m_hPlayThread != NULL);
}


/**
* \author	John Heywood (JH, VBC863)
* \date		November 2012 Initial Creation, JH
*/
int CMediaPlayer::getFileIMOCallback(PVARSTRUCT pvars, int id, LPARAM lparam)
{
	CMediaPlayer *pthis = (CMediaPlayer*) lparam;

	if (pvars && !wcsnicmp (pvars->pStr, L"OK", 2))
		pthis->playLocalAudio();

	return IMO_HANDLED;
}


/**
* \author	John Heywood (JH, VBC863)
* \date		November 2012 Initial Creation, JH
*/
bool CMediaPlayer::launchPlayer()
{
	PROCESS_INFORMATION pi;
	if (!CreateProcess(L"\\windows\\WMPlayer.exe", m_lpzFilename, NULL, NULL, FALSE, 0, NULL, NULL, NULL, &pi))
	{
		// for WinCE CEPlayer we need to set a registry key to make sure it launches full screen
		HKEY hKey = 0;
		LPCWSTR subkey = L"SOFTWARE\\Microsoft\\CEPlayer";

		if( RegOpenKeyEx(HKEY_LOCAL_MACHINE,subkey,0,0,&hKey) == ERROR_SUCCESS)
		{
			DWORD dwType = REG_DWORD;
			DWORD dwData = 1; // Set AlwaysFullSize to 1
			RegSetValueEx(hKey, L"AlwaysFullSize", 0, dwType, (BYTE*)&dwData, sizeof(dwData));
			RegCloseKey(hKey);
		}

		if (!CreateProcess(L"\\windows\\CEPlayer.exe", m_lpzFilename, NULL, NULL, FALSE, 0, NULL, NULL, NULL, &pi))
		{
			// if CEPlayer doesn't exist either, try VPlayer
			if (!CreateProcess(L"\\windows\\VPlayer.exe", m_lpzFilename, NULL, NULL, FALSE, 0, NULL, NULL, NULL, &pi))
			{
				m_pModule->Log(PB_LOG_WARNING, L"Error launching MediaPlayer",	_T(__FUNCTION__), __LINE__);
				return false;
			}
		}
	}
	m_hPlayProcess = pi.hProcess;
	m_hPlayThread = pi.hThread;
	return true;
}


/**
* \author	John Heywood (JH, VBC863)
* \date		November 2012 Initial Creation, JH
*/
//use try/catch for mem fail
LPWSTR CMediaPlayer::setString(LPCWSTR pStr)
{
	LPWSTR pTemp;
	try{
		pTemp = new WCHAR[wcslen(pStr)+1];
		wcscpy(pTemp,pStr);
	}
	catch(...){
		
		return NULL;
	}
	return pTemp;
}


/**
* \author	John Heywood (JH, VBC863)
* \date		November 2012 Initial Creation, JH
*/
void CMediaPlayer::SetType(LPCWSTR szType)
{
	if (m_hPlayThread)
	{
		m_pModule->Log(PB_LOG_WARNING, L"Unable to set media type while playing",	_T(__FUNCTION__), __LINE__);
	}
	else
	{
		if ((szType == NULL) || (szType[0] == L'\0'))
		{
			m_eType = MP_TYPE_INVALID;
		}
		else if (wcsicmp(L"audio", szType) == 0)
		{
			m_eType = MP_TYPE_AUDIO;
		}
		else if (wcsicmp(L"video", szType) == 0)
		{
			m_eType = MP_TYPE_VIDEO;
		}
		else
		{
			m_pModule->Log(PB_LOG_WARNING, L"Unrecognised media type",	_T(__FUNCTION__), __LINE__);
		}
	}
}


/**
* \author	John Heywood (JH, VBC863)
* \date		November 2012 Initial Creation, JH
*/
void CMediaPlayer::SetType(MP_TYPE eType)
{
	if (m_hPlayThread)
	{
		m_pModule->Log(PB_LOG_WARNING, L"Unable to set media type while playing",	_T(__FUNCTION__), __LINE__);
	}
	else
	{
		m_eType = eType;
	}
}


/**
* \author	John Heywood (JH, VBC863)
* \date		November 2012 Initial Creation, JH
*/
BOOL CMediaPlayer::ApplicationFocusChange(bool bActivated)
{
	// ignore loss of focus during video playback.. we expect to lose focus because we are launching 
	// WMPlayer/CEPlayer.  If we're playing audio when we lose focus we need to stop playback.

	// if we've regained focus during video playback, assume the user has closed the external player
	// so we need to kill the player to unlock the file.  No specific action required for audio.

	if ((!bActivated && m_bAppHasFocus && (m_eType == MP_TYPE_AUDIO)) ||
		(bActivated && !m_bAppHasFocus && m_hPlayThread && (m_eType == MP_TYPE_VIDEO)))
	{
		Stop();
	}

	m_bAppHasFocus = bActivated;
	return TRUE;
}
