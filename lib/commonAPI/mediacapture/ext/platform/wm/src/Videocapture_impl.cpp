#include "../../../shared/generated/cpp/VideocaptureBase.h"
#include "DShowCam.h"
#include <common/RhodesApp.h>							// Rhodes Application utilities
//#include "Imager.h"										// ImageCapture original plugin

#pragma comment(lib, "uuid.lib")
#pragma comment(lib, "dmoguids.lib")
#pragma comment(lib, "strmiids.lib") 
#pragma comment(lib, "strmbase.lib")

enum VC_RETURN 
{ 
	VC_SAVED, VC_FAILED 
};

enum VC_STATE
{
	STATE_INACTIVE = 0,
	STATE_PREVIEW,
	STATE_CAPTURE,
	STATE_STOPPING
};

#define VC_DEFAULT_DURATION 5000
#define VC_MIN_DURATION		500
#define VC_DEFAULT_FILENAME L"\\VideoCapture.wmv"

#define ID_START (WM_USER + 1)
#define ID_STOP (WM_USER + 2)
#define ID_REDRAW (WM_USER + 3)
#define WIN_CLASSNAME L"VideoPreview"

namespace rho {

using namespace apiGenerator;
using namespace common;

class CVideocaptureImpl: public IRhoExtension,public CVideocaptureBase
{
private:
	HWND			hwndMainWindow;
	HINSTANCE		hInstance;
	HWND			m_hWndViewer;
	HWND			m_hwndStop;
	HWND			m_hwndStart;
	RECT			m_rcWinPos;
	//CImager *m_pImager; // Main Imager object ported from RE2 plugin
	ImgFmt*			m_pCapsArray;
	CDShowCam*		m_pDSCam;
	LPWSTR			m_lpzFilename;
	HANDLE			m_hStopEvent;
	unsigned int	m_iMaxDuration;
	VC_STATE		m_eState;
	CRITICAL_SECTION m_csLock;

	// Helper function required to set filename.
	LPWSTR setString(LPCWSTR pStr)
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

	/*
	 * Set the filename to be used when recording the video.
	 */
	LPWSTR SetFilename(LPCWSTR lpFilename)
	{
		if(m_eState == STATE_CAPTURE){
			LOG(INFO) + __FUNCTION__ + "Name cannot be changed during capture";
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

public:
    CVideocaptureImpl(const rho::String& strID): CVideocaptureBase()
    {
		m_hashProps.put("ID", strID);
		m_pCapsArray = NULL;
		m_pDSCam = NULL;
		m_eState = STATE_INACTIVE;
		m_lpzFilename = setString(VC_DEFAULT_FILENAME);

		LOG(INFO) + __FUNCTION__ + "Initializing CVideocaptureImpl for [" + strID + "]";

		RHODESAPP().getExtManager().registerExtension("Videocapture", this );

		m_hStopEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
		m_rcWinPos.left		= 0;
		m_rcWinPos.right	= 0;
		m_rcWinPos.top		= 0;
		m_rcWinPos.bottom	= 0;
		m_iMaxDuration = VC_DEFAULT_DURATION;
		InitializeCriticalSection(&m_csLock);
		LOG(INFO) + __FUNCTION__ + "InitializeCriticalSection(&m_csLock)";
    }

	~CVideocaptureImpl() {
		LOG(INFO) + __FUNCTION__ + "Shutting down camera";

		delete[] m_pCapsArray;
		if (m_pDSCam)
		{
			Stop();
			delete m_pDSCam;
			m_pDSCam = NULL;
		}
		//UnregisterClass(WIN_CLASSNAME, hInstance);
		DeleteCriticalSection(&m_csLock);
	}

	// WndProc for Preview window.  Handles calls from Start and Stop buttons
	static BOOL CALLBACK ViewerProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam)
	{
		CVideocaptureImpl *pObj;

		switch(message)
		{

			case WM_COMMAND:
				switch( LOWORD(wParam))
				{
					case ID_STOP:
					pObj = (CVideocaptureImpl *)GetWindowLong(hWnd, GWL_USERDATA);
					pObj->Stop();
					break;

					case ID_START:
					pObj = (CVideocaptureImpl *)GetWindowLong(hWnd, GWL_USERDATA);
					pObj->Start();
					break;
				
					default:
						break;
				}
				break;

		case WM_CREATE:
			break;

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}

		return TRUE;
	}

	// Method required to show the imager preview.
	void startPreview(void) {
		HMODULE hDDrawDll = NULL;

		EnterCriticalSection(&m_csLock);
		if (m_eState == STATE_INACTIVE)
		{
			LOG(INFO) + __FUNCTION__ + " startPreview() called, Starting the Video capture preview.";

			m_eState = STATE_PREVIEW;
			ResetEvent(m_hStopEvent);

			rho::common::CRhoExtData rhodes_data = rho::common::CRhodesApp::getInstance()->getExtManager().makeExtData();

			hwndMainWindow = rhodes_data.m_hBrowserWnd;
			hInstance = (HINSTANCE) (rhodes_data.m_hInstance);

			WNDCLASS wc;
			wc.style = CS_PARENTDC;
			wc.lpfnWndProc = (WNDPROC) ViewerProc;
			wc.cbClsExtra = 0;
			wc.cbWndExtra = 0;
			wc.hInstance = hInstance;
			wc.hIcon = NULL;
			wc.hCursor = LoadCursor(NULL, IDC_CROSS);
			wc.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
			wc.lpszMenuName = 0;
			wc.lpszClassName = WIN_CLASSNAME;

			// Need to put this here, otherwise we have an issue with the preview window.
			if (RegisterClass(&wc) == 0) {
				DWORD iErr = GetLastError();
				if (iErr != ERROR_CLASS_ALREADY_EXISTS) {
					DEBUGMSG (TRUE, (L"Could not set the Register Class [%d]\r\n", GetLastError()));
					return;
				}
			}

			m_hWndViewer = CreateWindowEx(WS_EX_NOANIMATION|WS_EX_TOPMOST, WIN_CLASSNAME, NULL, WS_VISIBLE | WS_CHILD, 
					m_rcWinPos.left,
					m_rcWinPos.top, 
					m_rcWinPos.right,
					m_rcWinPos.bottom, 
					hwndMainWindow, NULL, hInstance, NULL);

			ASSERT(m_pDSCam == NULL);

			// MC67 workaround - prevent the library being unloaded during ddraw initialisation
			hDDrawDll = LoadLibraryEx(L"DDRAW.dll", NULL, DONT_RESOLVE_DLL_REFERENCES);
			m_pDSCam = new CDShowCam();

			if(SUCCEEDED(m_pDSCam->InitFilterGrp())) {
				CamConfig cfg;;
				ZeroMemory(&cfg, sizeof(cfg));

				LOG(INFO) + __FUNCTION__ + "------Init FGraph OK------";

				cfg.sCamID			= L"CAM1:";	// TODO: Hard-coded at the moment. Fix this later
				cfg.rc				= m_rcWinPos;
				cfg.rc.left			= 0;		// we want the preview to start at the far left of the preview window
				cfg.rc.top			= 0;		// we want the preview to start at the very top of the preview window
				cfg.rc.bottom		+= 3;		// add 3 pixels to the depth to get rid of the black line
				cfg.hwndOwnerWnd	= m_hWndViewer;
				cfg.bIsAudioEnb		= TRUE;		// capture audio
				cfg.bIsCapEnb		= TRUE;		// capture video
				cfg.bIsPrvEnb		= TRUE;		// show the preview
				cfg.bIsStillEnb		= FALSE;	// no still capture required#

				if (m_pDSCam->BuildFilterGrp(&cfg)) {
					LOG(INFO) + __FUNCTION__ + "------FGraph Build OK------";

					if (m_pDSCam->RunGrp()) {
						// populate the capabilities array
						ImgFmt fmt;
						int iNumCaps = 0;
						ZeroMemory(&fmt, sizeof(fmt));

						if (m_pCapsArray)
						{
							delete [] m_pCapsArray;
							m_pCapsArray = NULL;
						}

						if (SUCCEEDED(m_pDSCam->EnumFirstCap(V, &fmt, &iNumCaps)) && (iNumCaps > 0)) {
							m_pCapsArray = new ImgFmt[iNumCaps+1];
							memcpy(m_pCapsArray, &fmt, sizeof(ImgFmt));

							if (iNumCaps > 1)
							{
								m_pDSCam->EnumOtherCap(V, &m_pCapsArray[1], iNumCaps);
							}

							// workaround for MC67 negative height issue
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
							// Subtract 100 pixels from the viewer.
							SetWindowPos(m_hWndViewer, HWND_TOPMOST, m_rcWinPos.left, m_rcWinPos.top-120,
								m_rcWinPos.right - m_rcWinPos.left, m_rcWinPos.bottom - (m_rcWinPos.top), SWP_NOZORDER);

							// resize the preview window - add 3 pixels to the bottom to get rid of the black line
							m_pDSCam->ResizePreview(m_rcWinPos.right - m_rcWinPos.left, 
								m_rcWinPos.bottom - m_rcWinPos.top - 40);

							// Create the buttons to Start and Stop the capture of the video.
							RECT rect;
							int width = 128;
							int height = 64;;
							int top, left;

							HWND hwnd = CreateWindow(L"BUTTON", L"", WS_CHILD, 0, 0, 0, 0, m_hWndViewer, (HMENU) 0, hInstance, NULL);
							HDC hdc = GetDC (hwnd);

							SIZE size;
							GetTextExtentPoint32 (hdc, L"START", 6, &size);

							ReleaseDC (hwnd, hdc);
							DestroyWindow (hwnd);

							DEBUGMSG (TRUE, (L"Button size %d x %d\r\n", size.cx, size.cy));
							width = size.cx;
							height = size.cy;

							// Position buttons at the bottom of the video capture window
							GetClientRect(m_hWndViewer, &rect);
							top = rect.bottom - height;
							left = 32;

							// Create the actual button
							m_hwndStop = CreateWindow(L"BUTTON", L"Stop", WS_CHILD | WS_VISIBLE, 
								left, top, width, height, m_hWndViewer, (HMENU) ID_STOP, hInstance, NULL);
							DEBUGMSG (TRUE, (L"Drawing Stop button at [%d,%d,%d,%d]\r\n", left, top, width, height));

							left += (width + 16);

							m_hwndStart = CreateWindow (L"BUTTON", L"Start", WS_CHILD | WS_VISIBLE,
								left, top, width, height, m_hWndViewer, (HMENU) ID_START, hInstance, NULL);
							DEBUGMSG (TRUE, (L"Drawing Start button at [%d,%d,%d,%d]\r\n", left, top, width, height));
							left += (width + 16);

							LOG(INFO) + __FUNCTION__ + "------FGraph Run OK------";

							FreeLibrary(hDDrawDll);
							LOG(INFO) + __FUNCTION__ + " Leaving critical section";
							LeaveCriticalSection(&m_csLock);
							LOG(INFO) + __FUNCTION__ + "Returning successfully from startPreview";

							// Set the window handle the WndProc function
							DWORD retValue = SetWindowLong(m_hWndViewer, GWL_USERDATA, (long) this);

							if (retValue == 0) {
								DEBUGMSG (TRUE, (L"Could not set the window long [%d]\r\n", GetLastError()));
							}

							return;
						}
						else 
						{
							OutputDebugString(L"------FGraph Capabilities Fail------");
						}
					}
					else 
					{
						OutputDebugString(L"------FGraph Run FAIL------");
					}
				}
				else 
				{
					OutputDebugString(L"------FGraph Build FAIL------");
				}
			}
			else 
			{
				OutputDebugString(L"------Init FGraph FAIL------");
				DEBUGMSG(TRUE, (L"The last error is %d", GetLastError()));
			}
		}
		LOG(INFO) + __FUNCTION__ + "Destroying m_hWndViewer";
		if (IsWindow(m_hWndViewer)) DestroyWindow(m_hWndViewer);
		m_eState = STATE_STOPPING;
		if (m_pDSCam) m_pDSCam->StopGrp();
		if (m_pDSCam) m_pDSCam->ReleaseGrp();
		delete m_pDSCam;
		m_pDSCam = NULL;
		m_eState = STATE_INACTIVE;
		FreeLibrary(hDDrawDll);
		LOG(INFO) + __FUNCTION__ + " Leaving critical section";
		LeaveCriticalSection(&m_csLock);
	}

    virtual void enable( const rho::Hashtable<rho::String, rho::String>& propertyMap, CMethodResult& oResult){}

	// Private method called by the Start button on the camera display to actually start recording video.
	void Start() {
		LOG(INFO) + __FUNCTION__ + " Before entering CriticalSection";
		EnterCriticalSection(&m_csLock);
		if (m_eState == STATE_PREVIEW)
		{
			// Disable the start button as we don't want to try and start capturing while capturing.
			EnableWindow(m_hwndStart, FALSE);
			ASSERT(m_pDSCam);
			LOG(INFO) + __FUNCTION__ + "WMV: Starting the video capture";
			m_eState = STATE_CAPTURE;
			m_pDSCam->Set_VdoFileName(m_lpzFilename);

			LOG(INFO) + __FUNCTION__ + "Setting duration to " + m_iMaxDuration;

			if(SUCCEEDED(m_pDSCam->StartCapture()))
			{
				// Create a thread to kill the video player.
				ResetEvent(m_hStopEvent);
			}
			else
			{
				LOG(INFO) + __FUNCTION__ + "WMV: Video capture failed";
				m_eState = STATE_PREVIEW;
			}
		}
		LeaveCriticalSection(&m_csLock);
		LOG(INFO) + __FUNCTION__ + " After leaving CriticalSection";
	}

	// TODO.  When button code added.  Move the code after startPreview to another method that the start() button calls directly.
	// That way the user can choose when to start recording.
	virtual void start(CMethodResult& oResult) {
		if (m_eState == STATE_INACTIVE)
		{
			// Check the registry to see if a camera has been enabled for this device.
			HKEY key;
			DWORD result = RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"System\\State\\Hardware", 0, 0, &key);
			bool bExistsAndSuccess = (result == ERROR_SUCCESS);
			// Query the camera value
			DWORD dwBufferSize(sizeof(DWORD));
			DWORD nResult(0);
			// If this returns correctly, then the device has a camera.
			LONG nError = ::RegQueryValueEx(key, L"Camera", 0, NULL, reinterpret_cast<LPBYTE>(&nResult), &dwBufferSize);
			if (ERROR_SUCCESS == nError) {
				startPreview();
			} else {
				// No camera attached to the device
			}
		}
	}

    virtual void stop(CMethodResult& oResult) {
		// Call stop
		Stop();
	}

	virtual void cancel(rho::apiGenerator::CMethodResult& oResult) {}

	// This is called from the stopTimerProc method.  This will also be called by the stop(CMethodResult& oResult) method.
	void Stop()
	{
		LOG(INFO) + __FUNCTION__ + " Stopping capture of video";
		SetEvent(m_hStopEvent);

		LOG(INFO) + __FUNCTION__ + " Before entering CriticalSection";
		EnterCriticalSection(&m_csLock);
		// Destroy the window
		if(IsWindow(m_hWndViewer)) DestroyWindow(m_hWndViewer);

		LOG(INFO) + __FUNCTION__ + "m_eState = " + (int)m_eState + "";

		if ((m_eState == STATE_PREVIEW) || (m_eState == STATE_CAPTURE))
		{
			LOG(INFO) + __FUNCTION__ + "In state of PREVIEW or CAPTURE";
			m_eState = STATE_STOPPING;
			ASSERT(m_pDSCam);
			m_pDSCam->StopCapture();
			m_pDSCam->StopGrp();
			m_pDSCam->ReleaseGrp();
			delete m_pDSCam;
			m_pDSCam = NULL;

			m_eState = STATE_INACTIVE;

			LOG(INFO) + __FUNCTION__ + " Before leaving CriticalSection";
			LeaveCriticalSection(&m_csLock);

			LOG(INFO) + __FUNCTION__ + " Camera unloaded.";
		}

		UnregisterClass(WIN_CLASSNAME, hInstance);
	}
};

class CVideocaptureSingleton: public CVideocaptureSingletonBase
{
    ~CVideocaptureSingleton(){}
    virtual rho::String getInitialDefaultID();
    virtual void enumerate(CMethodResult& oResult);
};

class CVideocaptureFactory: public CVideocaptureFactoryBase
{
    ~CVideocaptureFactory(){}
    virtual IVideocaptureSingleton* createModuleSingleton();
    virtual IVideocapture* createModuleByID(const rho::String& strID);
};

extern "C" void Init_Videocapture_extension()
{
    CVideocaptureFactory::setInstance( new CVideocaptureFactory() );
    Init_Videocapture_API();
}

IVideocapture* CVideocaptureFactory::createModuleByID(const rho::String& strID)
{
    return new CVideocaptureImpl(strID);
}

IVideocaptureSingleton* CVideocaptureFactory::createModuleSingleton()
{
    return new CVideocaptureSingleton();
}

void CVideocaptureSingleton::enumerate(CMethodResult& oResult)
{
    rho::Vector<rho::String> arIDs;
    arIDs.addElement("SC1");
    arIDs.addElement("SC2");

    oResult.set(arIDs);
}

rho::String CVideocaptureSingleton::getInitialDefaultID()
{
    CMethodResult oRes;
    enumerate(oRes);

    rho::Vector<rho::String>& arIDs = oRes.getStringArray();
        
    return arIDs[0];
}

}