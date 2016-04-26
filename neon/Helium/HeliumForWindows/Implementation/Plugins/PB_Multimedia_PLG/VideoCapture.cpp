#include "VideoCapture.h"
#include "VideoCaptureModule.h"
#include "MediaPlayer.h"

#define CECAMERA_CAMERAVIEW_CLASSNAME         TEXT("Camera View")
#define WM_ACTIVATE_CAMERAVIEW                WM_USER + 1

TCHAR m_tcImagerSound[MAX_PATH]; // not used for video but needed by DShowCam

CVideoCapture::CVideoCapture(HINSTANCE hInstance, HWND hParent, CVideoCaptureModule *pModule, bool bHasFocus)
{
	m_pModule			= pModule;
	m_hInstance			= hInstance;
	m_hParent			= hParent;
	m_iMaxDuration		= VC_DEFAULT_DURATION;
	m_lpzFilename		= setString(VC_DEFAULT_FILENAME);
	m_bAppHasFocus		= bHasFocus;	
	m_eState			= STATE_INACTIVE;
	m_szCamName			= L"CAM1:";
	m_pCapsArray		= NULL;
	m_hStopEvent		= CreateEvent(NULL, TRUE, FALSE, NULL);
	m_rcWinPos.left		= 0;
	m_rcWinPos.right	= 0;
	m_rcWinPos.top		= 0;
	m_rcWinPos.bottom	= 0;
	m_pDSCam			= NULL;
	FindCamera();
	InitializeCriticalSection(&m_csLock);
}

CVideoCapture::~CVideoCapture()
{
	delete [] m_lpzFilename;
	delete [] m_pCapsArray;
	if (m_pDSCam)
	{
		Cancel();
		delete m_pDSCam;
		m_pDSCam = NULL;
	}
	DeleteCriticalSection(&m_csLock);
}


LPWSTR CVideoCapture::SetFilename(LPCWSTR lpFilename)
{
	if(m_eState == STATE_CAPTURE){
		m_pModule->Log(PB_LOG_WARNING, L"Name cannot be changed during capture",
		_T(__FUNCTION__), __LINE__);
		return NULL;
	}

	BOOL bHasExt = FALSE;

	for(int i=0; lpFilename[i] != L'\0'; i++)
	{
		if (lpFilename[i] == L'.')
		{
			bHasExt = TRUE;
			break;
		}
	}

	if ((lpFilename[0] == L'\\') &&	(bHasExt))
	{
		m_lpzFilename = setString(lpFilename);
	}
	else
	{
		LPWSTR lpNewName = new WCHAR[wcslen(lpFilename)+5];

		if (lpFilename[0] != L'\\')
		{
			// there is no back slash at the front - add one
			wsprintf(lpNewName, L"\\");
		}

		//copy the entire filename
		wcscat(lpNewName, lpFilename);

		// if there's no extension (there shouldn't be) add it
		if (!bHasExt)
			wcscat(lpNewName, L".wmv");

		m_lpzFilename = setString(lpNewName);
		
		delete [] lpNewName;
	}
	return m_lpzFilename;
}


BOOL CVideoCapture::StartPreview(void)
{
	HMODULE hDDrawDll = NULL;

	if (!m_bAppHasFocus) // if we aren't focussed we can't use the hardware so ignore
		return FALSE;

	EnterCriticalSection(&m_csLock);
	if (m_eState == STATE_INACTIVE) 
	{
		m_pModule->Log(PB_LOG_INFO, L"WMV: Starting the Video capture preview",	_T(__FUNCTION__), __LINE__);

		m_eState = STATE_PREVIEW;
		ResetEvent(m_hStopEvent);

		m_hWndViewer = CreateWindowEx(WS_EX_NOANIMATION|WS_EX_TOPMOST, L"static", NULL, WS_VISIBLE | WS_POPUP, 
				m_rcWinPos.left,
				m_rcWinPos.top, 
				m_rcWinPos.right,
				m_rcWinPos.bottom, 
				m_hParent, NULL, m_hInstance, NULL);

		ASSERT(m_pDSCam == NULL);
		// workaround for some devices - prevent the library being unloaded during ddraw initialisation
		hDDrawDll = LoadLibraryEx(L"DDRAW.dll", NULL, DONT_RESOLVE_DLL_REFERENCES);
		m_pDSCam = new CDShowCam(m_pModule);

		if (SUCCEEDED(m_pDSCam->InitFilterGrp()))
		{
			CamConfig cfg;
			ZeroMemory(&cfg, sizeof(cfg));

			OutputDebugString(L"------Init FGraph OK------\n");

			cfg.sCamID			= m_szCamName;
			cfg.rc				= m_rcWinPos;
			cfg.rc.left			= 0;		// we want the preview to start at the far left of the preview window
			cfg.rc.top			= 0;		// we want the preview to start at the very top of the preview window
			cfg.rc.bottom		+= 3;		// add 3 pixels to the depth to get rid of the black line
			cfg.hwndOwnerWnd	= m_hWndViewer;
			cfg.bIsAudioEnb		= TRUE;		// capture audio
			cfg.bIsCapEnb		= TRUE;		// capture video
			cfg.bIsPrvEnb		= TRUE;		// show the preview
			cfg.bIsStillEnb		= FALSE;	// no still capture required

			if (m_pDSCam->BuildFilterGrp(&cfg)) 
			{
				OutputDebugString(L"------FGraph Build OK------\n");
				if (m_pDSCam->RunGrp())
				{
					// populate the capabilities array
					ImgFmt fmt;
					int iNumCaps = 0;
					ZeroMemory(&fmt, sizeof(fmt));

					if (m_pCapsArray)
					{
						delete [] m_pCapsArray;
						m_pCapsArray = NULL;
					}

					if (SUCCEEDED(m_pDSCam->EnumFirstCap(V, &fmt, &iNumCaps)) && (iNumCaps > 0))
					{
						m_pCapsArray = new ImgFmt[iNumCaps+1];
						memcpy(m_pCapsArray, &fmt, sizeof(ImgFmt));
						if (iNumCaps > 1)
						{
							m_pDSCam->EnumOtherCap(V, &m_pCapsArray[1], iNumCaps);
						}

						// workaround for some device negative height issue
						if (m_pCapsArray[0].nMaxHeight < 0)
						{
							// flip all negative heights in the capabilities back to positive
							for (int i=0; i<iNumCaps; i++)
							{
								if (m_pCapsArray[i].nMaxHeight < 0)
									m_pCapsArray[i].nMaxHeight = m_pCapsArray[i].nMaxHeight * -1;
							}
						}

						if ((m_rcWinPos.top | m_rcWinPos.bottom | m_rcWinPos.left | m_rcWinPos.right) == 0)
						{
							// no preview window size/position has been specified
							// choose a window dimension based on default capture resolution

							int iScreenX = GetSystemMetrics(SM_CXSCREEN);
							int iScreenY = GetSystemMetrics(SM_CYSCREEN);

							m_rcWinPos.left = 0;
							m_rcWinPos.top = 0;

							if (m_pCapsArray[0].nMaxWidth > m_pCapsArray[0].nMaxHeight)
							{
								// landscape camera
								m_rcWinPos.right = iScreenX;
								m_rcWinPos.bottom = (iScreenX*m_pCapsArray[0].nMaxHeight)/m_pCapsArray[0].nMaxWidth;
								if (m_rcWinPos.bottom > iScreenY)
								{
									m_rcWinPos.right = (m_rcWinPos.right * m_rcWinPos.bottom)/iScreenY;
									m_rcWinPos.bottom = iScreenY;
								}
							}
							else
							{
								// portrait camera
								m_rcWinPos.bottom = iScreenY;
								m_rcWinPos.right = (iScreenY*m_pCapsArray[0].nMaxWidth)/m_pCapsArray[0].nMaxHeight;
								if (m_rcWinPos.right > iScreenX)
								{
									m_rcWinPos.bottom = (m_rcWinPos.bottom * m_rcWinPos.right)/iScreenX;
									m_rcWinPos.right = iScreenX;
								}
							}

							// if either dimension doesn't fill the screen, centre that axis
							if (m_rcWinPos.right < iScreenX)
							{
								m_rcWinPos.left = (iScreenX - m_rcWinPos.right) / 2;
								m_rcWinPos.right += m_rcWinPos.left;
							}

							if (m_rcWinPos.bottom < iScreenY)
							{
								m_rcWinPos.top = (iScreenY - m_rcWinPos.bottom) / 2;
								m_rcWinPos.bottom += m_rcWinPos.top;
							}
						}

						// move the preview window
						SetWindowPos(m_hWndViewer, HWND_TOPMOST, m_rcWinPos.left, m_rcWinPos.top,
							m_rcWinPos.right - m_rcWinPos.left, m_rcWinPos.bottom - m_rcWinPos.top, SWP_NOZORDER);

						// resize the preview window - add 3 pixels to the bottom to get rid of the black line
						m_pDSCam->ResizePreview(m_rcWinPos.right - m_rcWinPos.left, 
							m_rcWinPos.bottom - m_rcWinPos.top + 3);

						OutputDebugString(L"------FGraph Run OK------\n");
						FreeLibrary(hDDrawDll);
						LeaveCriticalSection(&m_csLock);
						return TRUE;
					}
					else OutputDebugString(L"------FGraph Capabilities Fail------\n");
				}
				else OutputDebugString(L"------FGraph Run FAIL------\n");
			}
			else OutputDebugString(L"------FGraph Build FAIL------\n");
		}
		else OutputDebugString(L"------Init FGraph FAIL------\n");
	}
	if (IsWindow(m_hWndViewer)) DestroyWindow(m_hWndViewer);
	m_eState = STATE_STOPPING;
	if (m_pDSCam) m_pDSCam->StopGrp();
	if (m_pDSCam) m_pDSCam->ReleaseGrp();
	delete m_pDSCam;
	m_pDSCam = NULL;
	m_eState = STATE_INACTIVE;
	FreeLibrary(hDDrawDll);
	LeaveCriticalSection(&m_csLock);
	return FALSE;
}


void CVideoCapture::Start(void)
{
	if (!m_bAppHasFocus) // if we aren't focussed we can't use the hardware so ignore
		return;

	if (m_eState == STATE_INACTIVE)
	{
		// Preview not active... start it
		StartPreview();
	}

	EnterCriticalSection(&m_csLock);
	if (m_eState == STATE_PREVIEW)
	{
		CMediaPlayer::KillPlayer();
		ASSERT(m_pDSCam);
		m_pModule->Log(PB_LOG_INFO, L"WMV: Starting the Video capture",	_T(__FUNCTION__), __LINE__);
		m_eState = STATE_CAPTURE;
		m_pDSCam->Set_VdoFileName(m_lpzFilename);
		if (SUCCEEDED(m_pDSCam->StartCapture()))
		{
			ResetEvent(m_hStopEvent);
			CloseHandle(CreateThread(NULL, 0, &CVideoCapture::stopTimerProc, (LPVOID)this, 0, NULL));
		}
		else
		{
			m_pModule->Log(PB_LOG_INFO, L"WMV: Video capture failed", _T(__FUNCTION__), __LINE__);
			m_eState = STATE_PREVIEW;
		}
	}
	LeaveCriticalSection(&m_csLock);
}

void CVideoCapture::Stop(void)
{
	SetEvent(m_hStopEvent);
	EnterCriticalSection(&m_csLock);
	// Destroy the window
	if (IsWindow(m_hWndViewer)) DestroyWindow(m_hWndViewer);

	if ((m_eState == STATE_PREVIEW)|| (m_eState == STATE_CAPTURE))
	{
		m_eState = STATE_STOPPING;
		ASSERT(m_pDSCam);
		// Close down DirectShow
		m_pDSCam->StopCapture();
		m_pDSCam->StopGrp();
		m_pDSCam->ReleaseGrp();
		delete m_pDSCam;
		m_pDSCam = NULL;

		// start the file transfer
		CloseHandle(CreateThread(NULL, 0, &CVideoCapture::transferProc, (LPVOID)this, 0, NULL));
		m_eState = STATE_INACTIVE;
		LeaveCriticalSection(&m_csLock);
	}
}

void CVideoCapture::Cancel(void)
{
	SetEvent(m_hStopEvent);
	EnterCriticalSection(&m_csLock);
	// Destroy the window
	if (IsWindow(m_hWndViewer)) DestroyWindow(m_hWndViewer);

	if ((m_eState == STATE_PREVIEW)|| (m_eState == STATE_CAPTURE))
	{
		m_eState = STATE_STOPPING;
		ASSERT(m_pDSCam);
		// Close down DirectShow
		m_pDSCam->StopGrp();
		m_pDSCam->ReleaseGrp();
		delete m_pDSCam;
		m_pDSCam = NULL;

		// Delete the file
		DeleteFile(m_lpzFilename);

		m_eState = STATE_INACTIVE;
	}
	LeaveCriticalSection(&m_csLock);
}

DWORD WINAPI CVideoCapture::stopTimerProc(LPVOID lpParam)
{
	CVideoCapture *pVideoCapture = (CVideoCapture*)lpParam;
	if (pVideoCapture)
	{
		if (WaitForSingleObject(pVideoCapture->m_hStopEvent, pVideoCapture->m_iMaxDuration)
			== WAIT_TIMEOUT)
		{
			if (pVideoCapture && (pVideoCapture->m_eState == STATE_CAPTURE))
			{
				pVideoCapture->Stop();
			}
		}
		if (pVideoCapture)
			ResetEvent(pVideoCapture->m_hStopEvent);
	}
	return 0;
}

DWORD WINAPI CVideoCapture::transferProc(LPVOID lpParam)
{
	CVideoCapture *pVideoCapture = (CVideoCapture*)lpParam;
	if (pVideoCapture)
	{
		if(pVideoCapture->m_pModule->SendVideoFile(pVideoCapture->m_lpzFilename))
		{
			return 0;
		}
		pVideoCapture->m_pModule->Log(PB_LOG_WARNING, L"WMV: Could not transfer the file",	_T(__FUNCTION__), __LINE__);
		return 1;
	}
	return 2;
}


//use try/catch for mem fail
LPWSTR CVideoCapture::setString(LPCWSTR pStr)
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


void CVideoCapture::SetDuration(int iMilliSeconds)
{
	if(iMilliSeconds >= VC_MIN_DURATION){
		m_iMaxDuration = iMilliSeconds;
	}

}

BOOL CVideoCapture::ApplicationFocusChange(bool bActivated)
{
	// if we've lost focus during video capture we must release the filter graph
	if (!bActivated && m_bAppHasFocus)
		Cancel();

	m_bAppHasFocus = bActivated;
	return TRUE;
}
void CVideoCapture::FindCamera()
{
    DEVMGR_DEVICE_INFORMATION di;	
    // The driver material doesn't ship as part of the SDK. This GUID is hardcoded
    // here to be able to enumerate the camera drivers and pass the name of the driver to the video capture filter
    GUID guidCamera = {0xCB998A05, 0x122C, 0x4166, 0x84, 0x6A, 0x93, 0x3E, 0x4D, 0x7E, 0x3C, 0x86};

    di.dwSize = sizeof(di);
    HANDLE hCam = FindFirstDevice(DeviceSearchByGuid, &guidCamera, &di);
    if((hCam == INVALID_HANDLE_VALUE) ||(hCam == NULL) || (di.hDevice == NULL))
    {
        m_bCameraPresent = false;
    }
    else 
    {
        m_bCameraPresent = true;
    }

}
bool CVideoCapture::IsCameraPresent()
{	
    return m_bCameraPresent;
}
