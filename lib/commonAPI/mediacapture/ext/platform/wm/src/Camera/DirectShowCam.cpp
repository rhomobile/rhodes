#include "DirectShowCam.h"
#include "Common/RhoUtil.h"


#define COLOR_CAM_ID L"CAM" //eg CAM1:
#define COLOR_CAM_ID_LENGTH 3

CDirectShowCam::CDirectShowCam(LPCTSTR szDeviceName):CCamera(szDeviceName)
{
	m_CamType = L"color";
	m_hDshowDLL = NULL;
	lpfn_DSHOW_Init = NULL;				
	lpfn_DSHOW_Close= NULL;		
	lpfn_DSHOW_Stop= NULL;			
	lpfn_DSHOW_SetFlash= NULL;		
	lpfn_DSHOW_SetResolution= NULL;			
	lpfn_DSHOW_Capture= NULL;	
	lpfn_DSHOW_RedrawPreview= NULL;			
	lpfn_DSHOW_GetResolution= NULL;	
	lpfn_DSHOW_Run = NULL;
	isDshowDllPresent = false;
	if(InitDShow())
	{
		isDshowDllPresent = true;
		getCameraHWDetails();
	}
	else
	{
		LOG(ERROR)+ L"InitDshow failed, check DShowCamModule.dll present";
	}
}
CDirectShowCam::~CDirectShowCam()
{
	if(m_hDshowDLL)
	{
		FreeLibrary(m_hDshowDLL);
	}

}
bool CDirectShowCam::InitDShow()
{
	m_hDshowDLL = LoadLibrary(DSHOW_DLL);
	if (!m_hDshowDLL)
	{
		return false;
	}
	//  Map the function pointers to functions exported from the DLL
	lpfn_DSHOW_Init = (LPFN_DSHOW_INIT)GetProcAddress
		(m_hDshowDLL, L"InitDirectShow");
	lpfn_DSHOW_Close = (LPFN_DSHOW_CLOSE)GetProcAddress
		(m_hDshowDLL, L"CloseDShow");
	lpfn_DSHOW_Stop = (LPFN_DSHOW_STOP)GetProcAddress
		(m_hDshowDLL, L"Stop");
	lpfn_DSHOW_Run = (LPFN_DSHOW_RUN)GetProcAddress
		(m_hDshowDLL, L"Run");
	lpfn_DSHOW_SetFlash = (LPFN_DSHOW_SET_FLASH)GetProcAddress
		(m_hDshowDLL, L"SetFlash");
	lpfn_DSHOW_SetResolution = (LPFN_DSHOW_SET_RESOLUTION)GetProcAddress
		(m_hDshowDLL, L"SetCameraResolution");
	lpfn_DSHOW_Capture = (LPFN_DSHOW_CAPTURE)GetProcAddress
		(m_hDshowDLL, L"CaptureStill");
	lpfn_DSHOW_RedrawPreview = (LPFN_DSHOW_REDRAW_PREVIEW)GetProcAddress
		(m_hDshowDLL, L"ResizePreview");
	lpfn_DSHOW_GetResolution = (LPFN_DSHOW_GET_RESOLUTION)GetProcAddress
		(m_hDshowDLL, L"GetResolution");
	if( lpfn_DSHOW_Init == NULL ||
		lpfn_DSHOW_Close == NULL ||
		lpfn_DSHOW_Stop == NULL ||
		lpfn_DSHOW_Run == NULL ||
		lpfn_DSHOW_SetFlash == NULL ||
		lpfn_DSHOW_SetResolution == NULL ||
		lpfn_DSHOW_Capture == NULL ||
		lpfn_DSHOW_RedrawPreview == NULL ||
		lpfn_DSHOW_GetResolution == NULL)
	{
		return false;
	}
	return true;
	
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
		if(0 == wcsncmp(di.szLegacyName, COLOR_CAM_ID, COLOR_CAM_ID_LENGTH ))//in devices with no color cam, we need this check whther ID is proper
		{
			arIDs.addElement(rho::common::convertToStringA(di.szLegacyName));
			camLookUp.put(rho::common::convertToStringA(di.szLegacyName), eColorCam );
			//search for next device in a loop	
			while(FindNextDevice(hFirstDevice, &di))
			{
				arIDs.addElement(rho::common::convertToStringA(di.szLegacyName));
				camLookUp.put(rho::common::convertToStringA(di.szLegacyName), eColorCam );
				FindClose(di.hDevice);

			}
		}
		FindClose(hFirstDevice);
	} 
	return bRetStatus;
	
}
BOOL CDirectShowCam::getProperty(LPCTSTR szParameterName, WCHAR* szParameterValue)
{
    BOOL bRetStatus = TRUE;
    bRetStatus = CCamera::getProperty(szParameterName, szParameterValue);
    if(FALSE == bRetStatus)
    {
		bRetStatus = TRUE;//set again to true as we couldn't find the variable in base class impl,
        if(cmp(szParameterName, L"maxWidth"))
        {		
			if(supportedResln.size() > 0)
			{
				ImageRes res = supportedResln[supportedResln.size() -1];
				wcscpy(szParameterValue,rho::common::convertToStringW(res.nWidth).c_str());
			}		
            
        }
		else if(cmp(szParameterName, L"maxHeight"))
		{			
			if(supportedResln.size() > 0)
			{				
				ImageRes res = supportedResln[supportedResln.size() -1];
				wcscpy(szParameterValue,rho::common::convertToStringW(res.nHeight).c_str());
			}		
		}		
        else
        {
            bRetStatus = FALSE;
        }
    }
    return bRetStatus;
}
void CDirectShowCam::takeFullScreen()
{
	LOG(INFO) + __FUNCTION__ ; 	
	if(false == m_IsCameraRunning && isDshowDllPresent)
	{
		if(m_PreviewOn == false)
		{
			RECT pos;						
			HWND hWndViewer = m_ViewFinder.CreateViewerWindow(pos, eFullScreen);
			if(-1 != lpfn_DSHOW_Init(hWndViewer,  pos ))
			{
				m_PreviewOn = true;
				m_IsCameraRunning = true;
				setCameraProperties();
				/*if(!IsMPA3())
				{
					createTriggerMonitorThread(this);
				}*/
			}
			else
			{
				LOG(ERROR)+ L"InitDshow failed";
				m_ViewFinder.DestroyViewerWindow();
				lpfn_DSHOW_Close();
			}
		}
	}
}
BOOL CDirectShowCam::showPreview()
{
	BOOL bPreviewStatus = FALSE;
	LOG(INFO) + __FUNCTION__ ; 
	if(m_IsCameraRunning == false && isDshowDllPresent)
	{
		if(m_PreviewOn == false)
		{

			RECT pos;
			pos.left = m_PreviewLeft;
			pos.top = m_PreviewTop;
			pos.right = m_PreviewWidth;	
			pos.bottom = m_PreviewHeight;
			HWND hWndViewer = m_ViewFinder.CreateViewerWindow(pos, eConfigurable);	
			//set pos.left and pos.top to zero
			//renderer window always wants to fit into viewer wnd client area
			pos.left=0;
			pos.top=0;						
			if(-1 != lpfn_DSHOW_Init(hWndViewer, pos ))
			{
				m_PreviewOn = true;
				m_IsCameraRunning = true;
				setCameraProperties();
			}
			else
			{
				m_ViewFinder.DestroyViewerWindow();
				lpfn_DSHOW_Close();
			}




		}
	}
	return true;

}
BOOL CDirectShowCam::hidePreview()
{
	LOG(INFO) + __FUNCTION__ 	;
	BOOL bRetStatus = FALSE;
	if (m_PreviewOn)
	{
		m_ViewFinder.DestroyViewerWindow();
		lpfn_DSHOW_Stop();
		lpfn_DSHOW_Close();	
		m_PreviewOn = false;
		m_IsCameraRunning = false;
		bRetStatus = TRUE;


	}
	return bRetStatus;
}
void CDirectShowCam::Capture()
{
    LOG(INFO) + __FUNCTION__ ;
    HRESULT hr;
    if(m_PreviewOn)
    {   
        eImageFilePathErrorType filePathStatus; 
        filePathStatus = isImageFilePathValid();
        if( eFilePathValid == filePathStatus)
        {

			PlaySound(m_CaptureSound.c_str(),NULL,SND_FILENAME|SND_ASYNC);


			rho::StringW fileName = getFileName();
			hr= lpfn_DSHOW_Capture(fileName.c_str(), false);
			if(SUCCEEDED(hr))
			{
				rho::String imageUri;
				int nImageWidth = 0;
				int nImageHeight =0;
				if(m_eOutputFormat == eDataUri)
				{
					//  Rather than get bogged down the Direct Show again we'll just
					//  read the file back in from disk.
					HANDLE hFile = CreateFile(fileName.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, 
						FILE_ATTRIBUTE_NORMAL, NULL);			

					if(hFile)
					{
						DWORD dwFileSize = GetFileSize(hFile, NULL);
						bool bCanSupportDataUri = false;
						bCanSupportDataUri = CAN_SUPPORT_DATA_URI(dwFileSize) ? true : false;

						if(bCanSupportDataUri)//filesize with in limit
						{
							bool fileReadSuccess = false;
							DWORD dwImageBufSize =0;///< Variable for Buffer size of captured image
							LPVOID pImageBuffer =NULL;///< Buffer to save captured image
							if (dwFileSize > 0)
							{

								pImageBuffer = new BYTE[dwFileSize];
								if(pImageBuffer)
								{
									dwImageBufSize = dwFileSize;
									DWORD dwBytesRead = 0;
									do
									{
										if (!ReadFile(hFile, pImageBuffer, dwFileSize, &dwBytesRead, NULL))
										{
											//  Some error has occured reading the file
											LOG(ERROR) + L"Unable to send image data as URI, could not read data";		
											UpdateCallbackStatus("error","Unable to send image data as URI, could not read data","");									
											fileReadSuccess = false;
											break;
										}
										else
										{
											fileReadSuccess = true;
										}
									}
									while (dwBytesRead != 0);
								}
							}
							if(fileReadSuccess)
							{	
								if(pImageBuffer)
								{
									rho::common::GetJpegResolution((BYTE*)pImageBuffer, dwImageBufSize, nImageWidth, nImageHeight);
									rho::common::GetDataURI((BYTE*)pImageBuffer, dwImageBufSize, imageUri);
								}							
								//update callback						
								UpdateCallbackStatus("ok","",imageUri,nImageWidth, nImageHeight );

							}
							else
							{
								LOG(ERROR) + L"Unable to send image data as URI, an error occured during file read";		
								UpdateCallbackStatus("error","Unable to send image data as URI, an error occured during file read","");				
							}
							delete[] pImageBuffer;
							pImageBuffer = NULL;
						}
						CloseHandle(hFile);
						if(!bCanSupportDataUri)
						{
							//enter if cannot support data uri
							rho::common::GetJpegResolution( fileName.c_str(), nImageWidth, nImageHeight);
							UpdateCallbackStatus("error","Failed to prepare dataUri, Image saved succesfully.",imageUri,nImageWidth, nImageHeight );
						}
						
					}
					else
					{
						LOG(ERROR) + L"Unable to send image data as URI, could not find captured image";		
						UpdateCallbackStatus("error", "Unable to send image as dataUri, could not find captured image","");

					}
				}
				else
				{
					rho::common::GetJpegResolution( fileName.c_str(), nImageWidth, nImageHeight);
					imageUri = rho::common::convertToStringA(fileName).c_str();
					//update callback
					UpdateCallbackStatus("ok","",imageUri, nImageWidth, nImageHeight );
				}
			}
			else
			{
				UpdateCallbackStatus("error","Image Capture operation failed.","");
			}

		}
        else
        {
            switch(filePathStatus)
            {
            case eFileNotExist:
                {
                    UpdateCallbackStatus("error","File path is invalid.","");
                    break;
                }
            case eFileReadOnly:
                {
                    UpdateCallbackStatus("error","File path is readonly.","");
                    break;
                }
            }
        }

    }
}
void CDirectShowCam::getSupportedPropertyList(rho::Vector<rho::String>& arrayofNames)
{
	CCamera::getSupportedPropertyList(arrayofNames);
	arrayofNames.push_back("maxWidth");
	arrayofNames.push_back("maxHeight");
	arrayofNames.push_back("supportedSizeList");
}
void CDirectShowCam::getSupportedSizeList(StringifyVector& supportedSizeList)
{
	
	if(supportedResln.size() > 0)
	{
		
		ImageRes res;
		for(int index=0; index < supportedResln.size(); index++)
		{
			res = supportedResln[index];			
			StringifyHash resl;
			resl.set("width",res.nWidth);
			resl.set("height",res.nHeight);
			supportedSizeList.push_back(resl);			

		}


	}
}
void CDirectShowCam::SetFlashMode()
{
	if(m_PreviewOn)
	{
		FlashSetting setting;
		setting = m_FlashMode? FlashSetting::On : FlashSetting::Off;
		lpfn_DSHOW_SetFlash(setting);
	}
}

void CDirectShowCam::setCameraProperties()
{
	if(m_PreviewOn)
	{
		SetFlashMode();
		SetResolution();
	}
	
}
void CDirectShowCam::RedrawViewerWnd(RECT& pos)
{
	if(m_PreviewOn)
	{
		CCamera::RedrawViewerWnd(pos);
		lpfn_DSHOW_RedrawPreview(pos.right, pos.bottom);	
	}
}
void CDirectShowCam::SetResolution()
{
	if(m_PreviewOn)
	{
		//  Set the resolution of the saved image

		CameraSetting setting = GetNearestResolution();
		lpfn_DSHOW_SetResolution(setting);
		//SetTestResolution(m_DesiredWidth, m_DesiredHeight);

	}
}
void CDirectShowCam::getCameraHWDetails()
{  
	lpfn_DSHOW_GetResolution(supportedResln, m_szDeviceName, S);
}
CameraSetting CDirectShowCam::GetNearestResolution()
{
	ImageRes res;
	CameraSetting setting = Low;
	int index =0;
	for(index=0; index < supportedResln.size(); index++)
	{		
		res = supportedResln[index];
		if(m_DesiredHeight <= res.nHeight)
		{
			setting = (CameraSetting)index;
			break;
		}
				

	}
	if(index==supportedResln.size())
	{
		setting = (CameraSetting)(index-1);
	}
	return setting;

}
void CDirectShowCam::ApplicationFocusChange(bool bAppHasFocus)
{
	LOG(INFO) + L"Application focus change called";
	CCamera::ApplicationFocusChange(bAppHasFocus);
	if(m_PreviewOn)
	{
		if(bAppHasFocus)
		{
			if(m_FlashMode)
			{
				SetFlashMode();
			}

		}
		else
		{
			LOG(INFO) + L"Application lost focus, stop camera";				
			if(m_FlashMode)
			{
				lpfn_DSHOW_SetFlash(FlashSetting::Off);
			}

		}
	}
}
void CDirectShowCam::OnPowerButton(bool bPowerOn)
{
	if(!bPowerOn)//kill the camer on power down
	{
		if(m_PreviewOn)
		{
			hidePreview();
		}
	}
}