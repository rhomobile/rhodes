#include "DirectShowCam.h"

CDirectShowCam::CDirectShowCam(LPCTSTR szDeviceName):CCamera(szDeviceName)
{
	m_CamType = L"color";
	m_pDSCam = NULL;
}
CDirectShowCam::~CDirectShowCam()
{
	if (m_pDSCam) 
	{
		delete m_pDSCam;
		m_pDSCam = NULL;
	}
		
}
BOOL CDirectShowCam::enumerate(rho::Vector<rho::String>& arIDs, rho::Hashtable<rho::String, eCamType>& camLookUp)
{
	BOOL bRetStatus= FALSE;
	DEVMGR_DEVICE_INFORMATION di;
	// The driver material doesn't ship as part of the SDK. This GUID is hardcoded
	// here to be able to enumerate the camera drivers and pass the name of the driver to the video capture filter
	GUID guidCamera = {0xCB998A05, 0x122C, 0x4166, 0x84, 0x6A, 0x93, 0x3E, 0x4D, 0x7E, 0x3C, 0x86};
	
	di.dwSize = sizeof(di);
	HANDLE hFirstDevice = FindFirstDevice(DeviceSearchByGuid, &guidCamera, &di);
	if((hFirstDevice != INVALID_HANDLE_VALUE) ||(hFirstDevice != NULL) || (di.hDevice != NULL))
	{
		bRetStatus = TRUE;
		arIDs.addElement(rho::common::convertToStringA(di.szLegacyName));
		camLookUp.put(rho::common::convertToStringA(di.szLegacyName), eColorCam );
		//search for next device in a loop	
		while(FindNextDevice(hFirstDevice, &di))
		{
			arIDs.addElement(rho::common::convertToStringA(di.szLegacyName));
			camLookUp.put(rho::common::convertToStringA(di.szLegacyName), eColorCam );

		}
	} 
	return bRetStatus;
	
}
void CDirectShowCam::takeFullScreen()
{
}
BOOL CDirectShowCam::showPreview()
{
	BOOL bPreviewStatus = FALSE;
    LOG(INFO) + __FUNCTION__ ; 
	if(m_IsCameraRunning == false)
	{
		if(m_PreviewOn == false)
		{
			CamConfig cfg;
			if (m_pDSCam) 
			{
				delete m_pDSCam;
			}
			m_pDSCam = new CDShowCam();			
			if(m_pDSCam)
			{
				if(m_pDSCam->initFilterGraph())
				{
					cfg.sCamID = m_szDeviceName;					
					cfg.bIsAudioEnb = FALSE;	// no audio capture
					cfg.bIsCapEnb = TRUE;		// enable capture
					cfg.bIsPrvEnb = TRUE;		// enable preview
					cfg.bIsStillEnb = TRUE;		// enable still capture					
			
					if(m_pDSCam->BuildFilterGraph(cfg))
					{
						RECT pos;
						pos.left = m_PreviewLeft;
						pos.top = m_PreviewTop;
						pos.right = m_PreviewWidth;	
						pos.bottom = m_PreviewHeight;
						HWND hWndViewer = m_ViewFinder.CreateViewerWindow(pos, eConfigurable);					
						if(m_pDSCam->SetupPreview(hWndViewer, pos))
						{
							m_PreviewOn = true;
							m_IsCameraRunning = true;
						}
						setCameraProperties();


					}
				}

			}
			if(false == m_PreviewOn)
			{
				//if preview failed
				if (m_pDSCam) 
				{
					delete m_pDSCam;
					m_pDSCam = NULL;
				}
			}
		}
	}
	return bPreviewStatus;
	
}
BOOL CDirectShowCam::hidePreview()
{
	LOG(INFO) + __FUNCTION__ 	;
	BOOL bRetStatus = FALSE;
	if (m_PreviewOn)
	{	
		/*if(E_IMG_SUCCESS == StopViewer())
		{
			m_PreviewOn = false;
		    m_IsCameraRunning = false;
			bRetStatus = TRUE;
		}*/

	}
	return bRetStatus;
}
void CDirectShowCam::Capture()
{
}
void CDirectShowCam::SetFlashMode()
{
}
void CDirectShowCam::SetDesiredWidth()
{
}
void CDirectShowCam::SetDesiredHeight()
{
}
void CDirectShowCam::setCameraProperties()
{
	HRESULT hr;

	CamPropTbl propTbl;
	/*propTbl.p = CameraControl_Flash;
	hr = m_pDSCam->Get_PropRng(&propTbl);
	if (hr != E_PROP_ID_UNSUPPORTED)
	{
		propTbl.plVal = m_FlashMode ? propTbl.plMax : propTbl.plMin;
		m_pDSCam->Set_PropVal( &propTbl);
	}*/

	//  Set the resolution of the saved image
	if (!(m_DesiredHeight == -1 && m_DesiredWidth == -1))
	{
		PinType myPIN = S;
		ImageRes myRes;
		myRes.nHeight = m_DesiredHeight;
		myRes.nWidth = m_DesiredWidth;
		if (!m_pDSCam->Set_Resolution(&myRes, myPIN))
		{
			//  Something went wrong setting the desired Image resolution
			LOG(ERROR) + L"Set Image Resolution failed";
		}
	}
	
}
