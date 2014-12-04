#include "DShowCam.h"
#include "wmcodecids.h"
#include "dshowdef.h"
#include "propertybag.h"


#pragma comment(lib, "uuid.lib")
#pragma comment(lib, "dmoguids.lib")
#pragma comment(lib, "strmiids.lib") 

CDShowCam::CDShowCam()
{
	
}
CDShowCam::~CDShowCam()
{
}
//------------------------------------------------------------------
//
// Prototype:	initFilterGraph
//
// Description:	This is used to create the Filter Graph Manager(FGM) and
//				initialize several filter graph related components.
//
// Parameters:	none
//
// 
// Notes:
//------------------------------------------------------------------
BOOL CDShowCam::initFilterGraph()
{
	BOOL retStatus = FALSE;
	HRESULT hr = E_FAIL;
	// Create the graph builder and the filtergraph
	hr = pCaptureGraphBuilder.CoCreateInstance( CLSID_CaptureGraphBuilder );
	if(SUCCEEDED(hr))
	{
		hr = pGraph.CoCreateInstance( CLSID_FilterGraph );
		if(SUCCEEDED(hr))
		{
			hr = pCaptureGraphBuilder->SetFiltergraph( pGraph );
			if(SUCCEEDED(hr))
			{
				// Query all the interfaces needed later
				hr = pGraph.QueryInterface( &pMediaControl );
				if(SUCCEEDED(hr))
				{
					hr = pGraph.QueryInterface( &pMediaEvent );
					if(SUCCEEDED(hr))
					{
						pGraph.QueryInterface( &pMediaSeeking );
						if(SUCCEEDED(hr))
						{
							retStatus = TRUE;
						}
					}
				}
			}
		}
	}
	if(FALSE == retStatus)
	{ 
		LOG(ERROR) + __FUNCTION__ + L" initialization failed";
	}	
	return retStatus;

}
//------------------------------------------------------------------
//
// Prototype:	BOOL BuildFilterGrp(CamConfig* ptCamcfg)
//
// Description:	This is used to combine all requested objects in a Filter Graph.
//
// Parameters:	Pointer to CamConfig structure.
//
// Returns:	Boolean "TRUE" if Filter Graph is created successfully  otherwise
//			Boolean "FALSE".
// 
// Notes:
//------------------------------------------------------------------
BOOL CDShowCam::BuildFilterGraph(CamConfig& ptCamcfg)
{
	BOOL retStatus = FALSE;
	HRESULT hr = E_FAIL;
	if(initCaptureDevice(ptCamcfg.sCamID))
	{
		if(ptCamcfg.bIsCapEnb)
		{
			// Initialize the Video DMO Wrapper
			hr = pVideoEncoder.CoCreateInstance( CLSID_DMOWrapperFilter );
			if(SUCCEEDED(hr))
			{
				hr = pVideoEncoder.QueryInterface( &pVideoWrapperFilter );
				if(SUCCEEDED(hr))
				{

					// Load the WMV9 encoder in the DMO Wrapper. 
					// To encode in MPEG, replace CLSID_CWMV9EncMediaObject with the 
					// CLSID of your DMO
					hr = pVideoWrapperFilter->Init( CLSID_CWMV9EncMediaObject,
						DMOCATEGORY_VIDEO_ENCODER );
					if(SUCCEEDED(hr))
					{
						hr = pGraph->AddFilter( pVideoEncoder, L"WMV9 DMO Encoder" );
						if(SUCCEEDED(hr))
						{

							hr = pCaptureGraphBuilder->SetOutputFileName(&MEDIASUBTYPE_Asf, DFT_VDO_FN, &m_pMux, &pFileSink);
							if(SUCCEEDED(hr))
							{							
								hr = pCaptureGraphBuilder->RenderStream( &PIN_CATEGORY_CAPTURE,
									&MEDIATYPE_Video, pVideoCap,
									pVideoEncoder, m_pMux );
								if(SUCCEEDED(hr))
								{
									retStatus = TRUE;
								}
							}
						}
					}
				}
			}
		}//if(ptCamcfg.bIsCapEnb)
		if(retStatus)
		{

			if(ptCamcfg.bIsAudioEnb)
			{
			}
			if(ptCamcfg.bIsCapEnb && ptCamcfg.bIsStillEnb)
			{
				//Setup still capture Filters
				retStatus = SetupStill();			
			}
			
		}
		
	
	}//if(initCaptureDevice
	//Setup still capture Filters
	
	return retStatus;
}
BOOL CDShowCam::SetupPreview(HWND hViewerWnd, RECT& pos)
{
	BOOL bRetStatus = FALSE;
	HRESULT hr = E_FAIL;
	hr = pVideoRender.CoCreateInstance(CLSID_VideoRenderer);
	if(SUCCEEDED(hr))
	{
		hr = pGraph->AddFilter(pVideoRender, L"My Video Renderer");
		if(SUCCEEDED(hr))
		{

			hr = pCaptureGraphBuilder->RenderStream( &PIN_CATEGORY_PREVIEW, &MEDIATYPE_Video, pVideoCap, NULL, pVideoRender );
			if(SUCCEEDED(hr))
			{

				hr = pVideoRender->QueryInterface( &pVideoWindow);
				if(SUCCEEDED(hr))
				{
					hr = pVideoWindow->put_Owner((OAHWND) hViewerWnd);
					////---------------
					if(SUCCEEDED(hr))
					{
						hr = pVideoWindow->SetWindowForeground(OATRUE);
						if((SUCCEEDED(hr)))
						{
							hr = pVideoWindow->put_WindowStyle(WS_CHILD | WS_CLIPCHILDREN);
							if((SUCCEEDED(hr)))
							{
								pVideoWindow->SetWindowPosition(pos.left,pos.top,(pos.right - pos.left), (pos.bottom - pos.top));
								pVideoWindow->put_Caption(L"CAM1 preview");
								hr = pVideoWindow->put_Visible(OATRUE);
								if(SUCCEEDED(hr))
								{
									pCaptureGraphBuilder->ControlStream(&PIN_CATEGORY_CAPTURE, &MEDIATYPE_Video, pVideoCap, 0, 0, 0, 0);
									hr = pMediaControl->Run();
									if(SUCCEEDED(hr))
									{
										bRetStatus = TRUE;
										Sleep( 1000 );
									}
								}

							}
						}


					}

				}
			}
		}
	}
	return bRetStatus;

}
BOOL CDShowCam::initCaptureDevice(wstring szDeviceName)
{
	BOOL retStatus = FALSE;
	HRESULT hr = E_FAIL;
	CComVariant varCamName;
	VariantInit(&varCamName);
	varCamName = szDeviceName.c_str();
	LOG(INFO) + __FUNCTION__ + L" init filter with device name:" + szDeviceName ;
	// Initialize the video capture filter
	hr = pVideoCap.CoCreateInstance( CLSID_VideoCapture );	
	if(SUCCEEDED(hr))
	{
		hr = pVideoCap.QueryInterface( &pPropertyBag );
		if(SUCCEEDED(hr))
		{
			CPropertyBag  PropBag;
			hr = PropBag.Write( L"VCapName", &varCamName );   
			if(SUCCEEDED(hr))
			{
				hr = pPropertyBag->Load( &PropBag, NULL );
				if(SUCCEEDED(hr))
				{
					pPropertyBag.Release();
					hr = pGraph->AddFilter( pVideoCap, L"Video capture source" );
					if(SUCCEEDED(hr))
					{
						retStatus = TRUE;
					}
				}
			}
		}
	}
	if(FALSE == retStatus)
	{ 
		LOG(ERROR) + __FUNCTION__ + L" init filter with device name failed";
	}
	return retStatus;

}
BOOL CDShowCam::SetupStill()
{
	BOOL retStatus = FALSE;
	HRESULT hr = E_FAIL;
	SYSTEMTIME st;
	GetLocalTime(&st);WCHAR DName[TXT_LENGTH];
	WCHAR waTemp[TXT_LENGTH];
	//LoadString(hInst, STR_STILL_D_F_FMT, text, MAX_LOADSTRING);
	wsprintf(waTemp, L"%.2d%.2d%.2d", st.wMonth, st.wDay, st.wYear%100);
	//LoadString(hInst, STR_STILL_DIR_PTH, text, MAX_LOADSTRING);
	wsprintf(DName, L"\\My Documents\\My Pictures\\%s", waTemp);
	CreateDirectory(DName, NULL);
	hr = pStillSink.CoCreateInstance( CLSID_IMGSinkFilter );
	if(SUCCEEDED(hr))
	{
		hr = pGraph->AddFilter(pStillSink, L"StillSink");
		if(SUCCEEDED(hr))
		{
			hr = pCaptureGraphBuilder->RenderStream( &PIN_CATEGORY_STILL, &MEDIATYPE_Video, pVideoCap, NULL, pStillSink);
			if(SUCCEEDED(hr))
			{
				retStatus = TRUE;
			}
		}
	}
	if(FALSE == retStatus)
	{ 
		LOG(ERROR) + __FUNCTION__ + L" Setup still failed";
	}
	return retStatus;
}
//------------------------------------------------------------------
//
// Prototype:	HRESULT Get_PropRng( HANDLE hPropTbl)
//
// Description:	This is used to retrieve limit values of either Image, Camera or Custom property.
//
// Parameters:	
//			    bPropTbl is a pointer to the corresponding property structure.
//
// Returns:	S_OK if Property values are retrieved successfully.
// 
// Notes:
//------------------------------------------------------------------

HRESULT CDShowCam::Get_PropRng( HANDLE hPropTbl)
{
	HRESULT hr;	
	CComPtr<IAMCameraControl> pCamCtrl;

	CamPropTbl* tbl = (CamPropTbl* )hPropTbl;

	hr = pVideoCap->QueryInterface(IID_IAMCameraControl, (void **)&pCamCtrl);
	if(SUCCEEDED(hr))
	{
		hr = pCamCtrl->GetRange(tbl->p, &tbl->plMin, &tbl->plMax, &tbl->plDelta, &tbl->plDef, &tbl->plFlag);
		if(SUCCEEDED(hr))
		{
			hr = pCamCtrl->Get(tbl->p, &tbl->plVal, &tbl->plFlag);
			tbl->hr = hr;
		}
	}
	
	return hr;
} //end Get_PropRng
//------------------------------------------------------------------
//
// Prototype:	HRESULT Get_PropVal(PropType ePType, HANDLE hPropTbl)
//
// Description:	This is used to retrieve the value of either Image, Camera or Custom property.
//
// Parameters:	
//			    bPropTbl is a pointer to the corresponding property structure.
//
// Returns:	S_OK if Property value is retrieved successfully.
// 
// Notes:
//------------------------------------------------------------------
HRESULT CDShowCam::Get_PropVal( HANDLE hPropTbl)
{
	HRESULT hr;
	CComPtr<IAMCameraControl> pCamCtrl;


	CamPropTbl* tbl = (CamPropTbl* )hPropTbl;

	hr = pVideoCap->QueryInterface(IID_IAMCameraControl, (void **)&pCamCtrl);
	if(SUCCEEDED(hr))
	{
		hr = pCamCtrl->Get(tbl->p, &tbl->plVal, &tbl->plFlag);	
		tbl->hr=hr;
	}

	return hr;
} //end Get_PropVal

//------------------------------------------------------------------
//
// Prototype:	HRESULT Set_PropVal(PropType ePType, HANDLE hPropTbl)
//
// Description:	This is used to set the value of either Image, Camera or Custom property.
//
// Parameters:	
//			    bPropTbl is a pointer to the corresponding property structure.
//
// Returns:	S_OK if Property value is successfully set.
// 
// Notes:
//------------------------------------------------------------------
HRESULT CDShowCam::Set_PropVal(HANDLE hPropTbl)
{
	HRESULT hr;
	CComPtr<IAMCameraControl> pCamCtrl;



	CamPropTbl* tbl = (CamPropTbl* )hPropTbl;

	hr = pVideoCap->QueryInterface(IID_IAMCameraControl, (void **)&pCamCtrl);
	if(SUCCEEDED(hr))
	{
		hr = pCamCtrl->Set(tbl->p, tbl->plVal, tbl->plFlag);	
		tbl->hr=hr;
	}


	return hr;
} //end Set_PropVal
//------------------------------------------------------------------
//
// Prototype:	HRESULT Set_Resolution(ImageRes *ptRes, PinType ePType)
//
// Description:	This is used to set the resolution of either Preview, 
//				Still or Capture Pins.
//
// Parameters:	ptRes is a pointer to ImageRes struture, which holds the
//				resolution to set and ePType is Pin category enumuration;
//				V:-Capture Pin, S:-Still Pin, P:-Preview Pin
//				iDesiredWidth / iDesiredHeight: The desired size of the
//				output image.
//
// Returns:	S_OK if Image resolution is set successfully.
// 
// Notes:
//------------------------------------------------------------------
HRESULT CDShowCam::Set_Resolution(ImageRes* ptRes, PinType ePType)
{	
	HRESULT hr;
	GUID pType;

	//capture device can support a range of output formats. For example, 
	//a device might support 16-bit RGB, 32-bit RGB, and YUYV. Within each of these formats, 
	//the device can support a range of frame sizes.
	//The IAMStreamConfig interface is used to report which formats the device supports, 
	//and to set the format. The IAMStreamConfig interface is exposed on the capture filter's capture pin,
	//preview pin, or both. Use the ICaptureGraphBuilder2::FindInterface method to get the interface
	//pointer:
		
	switch(ePType)
	{
		case V:
			pType = PIN_CATEGORY_CAPTURE;			
			break;
		case S:
			pType = PIN_CATEGORY_STILL;			
			break;
		case P:
			pType = PIN_CATEGORY_PREVIEW;			
			break;

	}
	
	//PIN type, Any media type, Pointer to the capture filter.
	hr = pCaptureGraphBuilder->FindInterface(&pType, 0, pVideoCap, IID_IAMStreamConfig, (void**)&m_pStrConf);

	
	if(SUCCEEDED(hr))
	{
		VIDEO_STREAM_CONFIG_CAPS scc;
		AM_MEDIA_TYPE *pmtConfig;
		INT iResIdx=0;

	    INT nCount = 0, nSize = 0;

		hr = m_pStrConf->GetNumberOfCapabilities(&nCount, &nSize);
	    if(FAILED(hr))
		{
            return hr;
		}
		
		bool bUserResolutionFound = false;
		for(int i=iResIdx; i<nCount;i++)
		{
			hr = m_pStrConf->GetStreamCaps(i, &pmtConfig, (BYTE*)&scc);
	
			if (SUCCEEDED(hr))
			{
				if((scc.MaxOutputSize.cx >= ptRes->nWidth) && 
					(scc.MaxOutputSize.cy >= ptRes->nHeight))
				{
					bUserResolutionFound = true;
					StopGrp();
					hr = m_pStrConf->SetFormat(pmtConfig);
					if((FAILED(hr)))
						return FALSE;
					//if(ePType==V)
					//	CDShowCam::ReBuildGrp();
					RunGrp();
					WCHAR szLog[512];
					wsprintf(szLog, L"Setting image output resolution to %i (width) x %i (height)", 
						scc.MaxOutputSize.cx, scc.MaxOutputSize.cy);
					LOG(INFO) + szLog;
					break;
				}
			}
			// Delete the media type when you are done.
			//DeleteMediaType(pmtConfig);
		}

		if (!bUserResolutionFound && nCount >= 1)
		{
			//  User specified resolution was not found, default to
			//  the last available resolution (always highest in my tests)
			hr = m_pStrConf->GetStreamCaps(nCount - 1, &pmtConfig, (BYTE*)&scc);
			if (SUCCEEDED(hr))
			{
				StopGrp();
				hr = m_pStrConf->SetFormat(pmtConfig);
				if((FAILED(hr)))
					return FALSE;
				RunGrp();
				WCHAR szLog[512];
				wsprintf(szLog, L"User specified image resolution is too high.  Setting image output resolution to %i (width) x %i (height)", 
					scc.MaxOutputSize.cx, scc.MaxOutputSize.cy);
				LOG(WARNING) + szLog;
			}
		}
	}
	

	return SUCCEEDED(hr);
} //end Set_Resolution(ImageRes *ptRes, PinType ePType)
//------------------------------------------------------------------
//
// Prototype:	BOOL StopGrp()
//
// Description:	This method switches all filters in the filter graph to a stopped state. 
//
// Parameters:	none
//
// Returns:	boolean "TRUE" if Filter Graph has started running successfully otherwise
//			boolean "FALSE".
// 
// Notes:
//------------------------------------------------------------------
BOOL CDShowCam::StopGrp()
{
	HRESULT hr = -1;

	//Stop the Filter Graph
	hr = pMediaControl->Stop();

	if (hr==S_FALSE)
	{		 
		FILTER_STATE fs;
		int timeout= 10000;
		hr = pMediaControl->GetState(timeout, (OAFilterState*)&fs);

	}

	if(SUCCEEDED(hr))
		return TRUE;
	else
		return FALSE;

} //end StopGrp
//------------------------------------------------------------------
//
// Prototype:	BOOL RunGrp()
//
// Description:	This method switches the entire filter graph into a running state. 
//
// Parameters:	none
//
// Returns:	boolean "TRUE" if Filter Graph has started running successfully otherwise
//			boolean "FALSE".
// 
// Notes:
//------------------------------------------------------------------
BOOL CDShowCam::RunGrp()
{	
	BOOL result;	

	HRESULT hr = pCaptureGraphBuilder->ControlStream(&PIN_CATEGORY_CAPTURE, &MEDIATYPE_Video, pVideoCap, 0, 0, 0, 0);
	/*if (m_tCamcfg.bIsAudioEnb)
	hr = m_pCaptureGraphBuilder->ControlStream(&PIN_CATEGORY_CAPTURE, &MEDIATYPE_Audio, m_pAudioCaptureFilter, 0, 0, 0, 0);*/

	if(!(SUCCEEDED(hr)))
	{

		return FALSE;
	}

	//Run the Filter Graph
	hr = pMediaControl->Run();
	// do we have to wait for fillters to change state ?
	if (hr==S_FALSE)
	{


		FILTER_STATE fs;
		int timeout= 10000;
		hr = pMediaControl->GetState(timeout, (OAFilterState*)&fs);

	}
	// if filters changed state immediately or before timeout.
	if(SUCCEEDED(hr))
		result = TRUE;
	else 
	{	

		return  FALSE;
	}



	return result;
} //end RunGrp
BOOL CDShowCam::ResizePreview(DWORD dwWidth, DWORD dwHeight)
{
	HRESULT hr = S_FALSE;
    IVideoWindow* pVideoWindow = NULL;

	if (pVideoRender)
	{
		if(SUCCEEDED(hr = pVideoRender->QueryInterface( IID_IVideoWindow, (void **)&pVideoWindow)))
		{
			if (SUCCEEDED(hr = pVideoWindow->SetWindowPosition(0, 0, dwWidth, dwHeight)))
			{
				pVideoWindow->Release();
				return TRUE;
			}
			pVideoWindow->Release();
		}
	}
	return FALSE;
}
//------------------------------------------------------------------
//
// Prototype:	HRESULT CaptureStill(wstring wsSFName)
//
// Description:	This is used to trigger capture of a still image.
//				Here the still image name is passed as a parameter and it will be
//				saved in "\My Documnets\My Pictures" folder
//
// Parameters:	wsSFName is the name given for the still Image
//
// Returns:	S_OK if Still image is captured successfully.
// 
// Notes:
//------------------------------------------------------------------
HRESULT CDShowCam::CaptureStill(wstring wsSFName)
{
	WCHAR DName[TXT_LENGTH];
	wstring StillFN = L"\\tempimg.jpg";

	IPin* pStillPin = NULL;
    IAMVideoControl* pVideoControl = NULL;
	IFileSinkFilter* pStillFileSink = NULL;
	HRESULT hr = S_OK;


	int DNameLen = (wcsrchr(wsSFName.c_str(), L'\\')+1) - wsSFName.c_str();
	wcsncpy(DName, wsSFName.c_str(), DNameLen);
	DName[DNameLen] = L'\0';

	CreateDirectory(DName, NULL);

	//Set File Name
	if(SUCCEEDED(hr = pStillSink->QueryInterface( IID_IFileSinkFilter, (void **)&pStillFileSink )))
	{
		hr = pStillFileSink->SetFileName((LPCOLESTR)StillFN.c_str(), NULL );
		if(SUCCEEDED(hr))
		{
			LOG(INFO) + __FUNCTION__ + L"set file name succeeded";
			
		}
		else
		{
			LOG(ERROR) + __FUNCTION__ + L"set file name failed";

		}
		
	}

	//Find the still image output pin, retrieve the IAMVideoControl interface, and
	if(SUCCEEDED(hr = pCaptureGraphBuilder->FindPin((IUnknown*)pVideoCap, PINDIR_OUTPUT, &PIN_CATEGORY_STILL, &MEDIATYPE_Video, FALSE, 0, &pStillPin )))
	{
		// Once you have the still image pin, you query it for the
		// IAMVideoControl interface which exposes the SetMode function for 
		// triggering the image capture.
		LOG(INFO) + L"FindPin succeeded";
		if(SUCCEEDED(hr = pVideoCap->QueryInterface( IID_IAMVideoControl, (void **)&pVideoControl )))
		{
	    	// Now that trigger the still image capture.
			LOG(INFO) + L"query interface succeeded";
		
			hr = pVideoControl->SetMode( pStillPin, VideoControlFlag_Trigger); 
		}
		else
		{
			LOG(INFO) + L"query interface failed";
		}
	}
	else
	{
		LOG(INFO) + L"FindPin failed";
	}

	if(SUCCEEDED(hr))
	{
		LOG(INFO) + L"SetMOde succeeded";
		
		LONG lEventCode = 0, lParam1 = 0, lParam2 = 0;
		do{
			// If no events are received within 5 seconds, then we’ll assume 
			// we have an error. Adjust this value as you see fit.
			hr = pMediaEvent->GetEvent( &lEventCode, &lParam1,&lParam2, 5*1000 );

			//DEBUGIT(LogFile,"m_pMediaEventEx->GetEvent %ld \n", lEventCode); 
			if(SUCCEEDED(hr))
			{
				// If we recieve an event, free the event parameters.
				// we can do this immediately because we're not referencing 
				// lparam1 or lparam2.
				if(SUCCEEDED(hr = pMediaEvent->FreeEventParams( lEventCode, lParam1, lParam2 )))
				{
					// If the event was received, then we successfully 
					// captured the still image and can quit looking.
					if(lEventCode == EC_CAP_FILE_COMPLETED)
					{
						//DEBUGIT(LogFile,"  EC_CAP_FILE_COMPLETED \n");
						DeleteFile(wsSFName.c_str());
						MoveFile(StillFN.c_str(), wsSFName.c_str());
						break;
					}
				}
				
			}
			else
			{
					LOG(INFO) + L"timeout waiting for camera event";
					
			}
		// If the still image isn't captured within 60 seconds
		// of the trigger (or possibly longer if other events come in), 
		// then we’ll assume it’s an error. Continue processing events 
		// until we detect the error.
		}while(SUCCEEDED(hr));
	}
	else
	{
		LOG(ERROR) + L"SetMode failed";
	}
	//clean up
	if(pVideoControl)
	{
		pVideoControl->Release();
		pVideoControl = NULL;
	}
	if(pStillPin)
	{
		pStillPin->Release();
		pStillPin = NULL;
	}
	

    return hr;
} //end CaptureStill()
