#include "../../../shared/generated/cpp/CameraBase.h"
#include <common/RhodesApp.h>	
#include "DShowCam.h"
// Rhodes Application utilities

#pragma comment(lib, "uuid.lib")
#pragma comment(lib, "dmoguids.lib")
#pragma comment(lib, "strmiids.lib") 
#pragma comment(lib, "strmbase.lib")

enum VC_STATE
{
	STATE_INACTIVE = 0,
	STATE_PREVIEW,
	STATE_CAPTURE,
	STATE_STOPPING
};

#define WIN_CLASSNAME L"Camera Preview"

namespace rho {

using namespace apiGenerator;
using namespace common;

class CCameraImpl: public IRhoExtension, public CCameraBase
{
private:
	HWND			hwndMainWindow;
	HINSTANCE		hInstance;
	HWND			m_hWndViewer;
	VC_STATE		m_eState;
	CRITICAL_SECTION m_csLock;
	ImgFmt*			m_pCapsArray;
	CDShowCam*		m_pDSCam;
	RECT			m_rcWinPos;

	// Overridden properties
	unsigned int m_iDesiredHeight;
	unsigned int m_iDesiredWidth;
public:
    CCameraImpl(const rho::String& strID): CCameraBase()
    {
		LOG(INFO) + "2100:Calling CCameraImpl constructor for [" + strID + "] ...";

		m_hashProps.put( "ID", strID);
		m_pCapsArray = NULL;
		m_pDSCam = NULL;
		m_eState = STATE_INACTIVE;
	
		LOG(INFO) + "2110:Initialising Scanner Interface for Camera [" + strID + "]";
		RHODESAPP().getExtManager().registerExtension("Camera", this );
		m_rcWinPos.left		= 0;
		m_rcWinPos.right	= 0;
		m_rcWinPos.top		= 0;
		m_rcWinPos.bottom	= 0;
		InitializeCriticalSection(&m_csLock);

		LOG(INFO) + "2199:CCameraImpl constructor for [" + strID + "] has been called.";
    }

	~CCameraImpl()
	{
		LOG(INFO) + "2210:Shutting down Camera ...";

		LOG(INFO) + "2299:Camera shut down.";
	}

	static BOOL CALLBACK ViewerProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam)
	{
		switch(message)
		{
			case WM_CREATE:
			break;

			default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		return TRUE;
	}

	// Method required to show the image preview
	void startPreview(void) {
		HMODULE hDDrawDll = NULL;

		EnterCriticalSection(&m_csLock);
		if (m_eState == STATE_INACTIVE)
		{
			// Create a window based upon code in the VideoImpl
			LOG(INFO) + __FUNCTION__ + " startPreview() called, Starting the camera capture preview.";
			m_eState = STATE_PREVIEW;

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
				cfg.bIsAudioEnb		= FALSE;		// capture audio
				cfg.bIsCapEnb		= TRUE;		// capture video
				cfg.bIsPrvEnb		= TRUE;		// show the preview
				cfg.bIsStillEnb		= TRUE;	// no still capture required

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

						// Pass S in EnumFirstCap to find best still resolution
						if (SUCCEEDED(m_pDSCam->EnumFirstCap(S, &fmt, &iNumCaps)) && (iNumCaps > 0)) {
							m_pCapsArray = new ImgFmt[iNumCaps+1];
							memcpy(m_pCapsArray, &fmt, sizeof(ImgFmt));

							if (iNumCaps > 1)
							{
								m_pDSCam->EnumOtherCap(S, &m_pCapsArray[1], iNumCaps);
							}

							// Loop through the iNumCaps and output information required.
							int i;
							for (i = 0; i < iNumCaps; i++)
							{
								LOG(INFO) + __FUNCTION__ + "Still Capability [" + m_pCapsArray[i].nMaxWidth + "] x [" + m_pCapsArray[i].nMaxHeight + "] " + m_pCapsArray[i].wsColorFmt; ;
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

	virtual void takePicture( const rho::Hashtable<rho::String, rho::String>& propertyMap, rho::apiGenerator::CMethodResult& oResult)
	{
		LOG(INFO) + "CCameraImpl::Takepicture ...";

		if (!oResult.hasCallback())
		{
			LOG(ERROR) + "CCameraImpl::Takepicture: Callback not defined";
			oResult.set(FALSE);
			return;
		}

		startPreview();
	
		LOG(INFO) + "CCameraImpl::Takepicture: Set callback for camera event";
		//m_pImager->SetImagerCaptureEvent(&oResult);

		LOG(INFO) + "... CCameraImpl::Takepicture";

		oResult.set(false);
	}

	/////////////////////////////////////////////////////////////////////////////////
	virtual bool onWndMsg(MSG& oMsg)
 	///////////////////////////////////////////////////////////////////////////////
    {
        if ((oMsg.message == WM_KEYDOWN) &&
            (oMsg.wParam == VK_RETURN))
        {
			LOG(INFO) + "Return received";
            //return (m_pImager->SoftTrigger() != TRUE);
        }
        return true;
    }

	virtual void getCameraInfo(rho::apiGenerator::CMethodResult& oResult) {}
	virtual void choosePicture( const rho::Hashtable<rho::String, rho::String>& propertyMap, rho::apiGenerator::CMethodResult& oResult) {}
	virtual void saveImageToDeviceGallery(rho::apiGenerator::CMethodResult& oResult) {}
	virtual void getSupportedSizeList(rho::apiGenerator::CMethodResult& oResult) {}

	// Override properties to set desired width and height of image.
	void setDesiredHeight( int desiredHeight, rho::apiGenerator::CMethodResult& oResult)
	{
		LOG(INFO) + __FUNCTION__ + "m_iDesiredHeight is being set to " + desiredHeight;
		setProperty( "desiredHeight", rho::common::convertToStringA(desiredHeight), oResult );
		m_iDesiredHeight = desiredHeight;
		//if (m_pImager) {
		//	m_pImager->SetDesiredHeight(m_iDesiredHeight);
		//}
	}

	void setDesiredWidth( int desiredWidth, rho::apiGenerator::CMethodResult& oResult)
	{
		LOG(INFO) + __FUNCTION__ + "m_iDesiredWidth is being set to " + desiredWidth;
		setProperty( "desiredWidth", rho::common::convertToStringA(desiredWidth), oResult );
		m_iDesiredWidth = desiredWidth;
		//if (m_pImager) {
		//	m_pImager->SetDesiredWidth(m_iDesiredWidth);
		//}
	}
};

class CCameraSingleton: public CCameraSingletonBase
{
public:
	CCameraSingleton(): CCameraSingletonBase() {}
private:
    ~CCameraSingleton(){}
    virtual rho::String getInitialDefaultID();
    virtual void enumerate(CMethodResult& oResult);
	virtual void saveImageToDeviceGallery( const rho::String& pathToImage, rho::apiGenerator::CMethodResult& oResult) {}
	virtual void choosePicture( const rho::Hashtable<rho::String, rho::String>& propertyMap, rho::apiGenerator::CMethodResult& oResult) {}
	virtual void getCameraByType( const rho::String& cameraType, rho::apiGenerator::CMethodResult& oResult) {}
};

class CCameraFactory: public CCameraFactoryBase
{
    ~CCameraFactory(){}
    virtual ICameraSingleton* createModuleSingleton();
    virtual ICamera* createModuleByID(const rho::String& strID);
};

extern "C" void Init_camera_extension()
{
    CCameraFactory::setInstance( new CCameraFactory() );
    Init_Camera_API();
}

ICamera* CCameraFactory::createModuleByID(const rho::String& strID)
{
    return new CCameraImpl(strID);
}

ICameraSingleton* CCameraFactory::createModuleSingleton()
{
    return new CCameraSingleton();
}

void CCameraSingleton::enumerate(CMethodResult& oResult)
{
	rho::Vector<rho::String> arIDs;
    arIDs.addElement("IMG1");
    arIDs.addElement("CAM1");

    oResult.set(arIDs);
}

rho::String CCameraSingleton::getInitialDefaultID()
{
    CMethodResult oRes;
    enumerate(oRes);

    rho::Vector<rho::String>& arIDs = oRes.getStringArray();
        
    return arIDs[0];
}

}