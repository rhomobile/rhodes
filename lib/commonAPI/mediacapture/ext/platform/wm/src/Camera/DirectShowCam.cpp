#include "DirectShowCam.h"
#include "Common/RhoUtil.h"


#define COLOR_CAM_ID L"CAM" //eg CAM1:
#define COLOR_CAM_ID_LENGTH 3

CDirectShowCam::CDirectShowCam(LPCTSTR szDeviceName):CCamera(szDeviceName)
{
	m_CamType = L"color";
	getCameraHWDetails();
}
CDirectShowCam::~CDirectShowCam()
{

		
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
	if(false == m_IsCameraRunning)
	{
		if(m_PreviewOn == false)
		{
			RECT pos;						
			HWND hWndViewer = m_ViewFinder.CreateViewerWindow(pos, eFullScreen);
			if(-1 != InitDirectShow(hWndViewer,  pos ))
			{
				m_PreviewOn = true;
				m_IsCameraRunning = true;
				setCameraProperties();
				//createTriggerMonitorThread(this);
			}
			else
			{
				LOG(ERROR)+ L"InitDshow failed";
				m_ViewFinder.DestroyViewerWindow();
				CloseDShow();
			}
		}
	}
}
BOOL CDirectShowCam::showPreview()
{
	BOOL bPreviewStatus = FALSE;
	LOG(INFO) + __FUNCTION__ ; 
	if(m_IsCameraRunning == false)
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
			if(-1 != InitDirectShow(hWndViewer, pos ))
			{
				m_PreviewOn = true;
				m_IsCameraRunning = true;
				setCameraProperties();
			}
			else
			{
				m_ViewFinder.DestroyViewerWindow();
				CloseDShow();
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
		Stop();
		CloseDShow();	
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
			hr= CaptureStill(fileName.c_str(), false);
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
						bool fileReadSuccess = true;
						DWORD dwImageBufSize =0;///< Variable for Buffer size of captured image
						LPVOID pImageBuffer =NULL;///< Buffer to save captured image
						if (dwFileSize > 0)
						{
					
							pImageBuffer = new BYTE[dwFileSize];
							dwImageBufSize = dwFileSize;
							DWORD dwBytesRead = 0;
							do
							{
								if (!ReadFile(hFile, pImageBuffer, dwFileSize, &dwBytesRead, NULL))
								{
									//  Some error has occured reading the file
									LOG(ERROR) + L"Unable to send image data as URI, could not read data";		
									UpdateCallbackStatus("error","Unable to send image data as URI, could not read data","");
									delete[] pImageBuffer;
									pImageBuffer = NULL;
									fileReadSuccess = false;
									break;
								}
							}
							while (dwBytesRead != 0);
						}
						CloseHandle(hFile);
						if(fileReadSuccess)
						{								
							//rho::common::GetJpegResolution((BYTE*)pImageBuffer, dwImageBufSize, nImageWidth, nImageHeight);
							//rho::common::GetDataURI((BYTE*)pImageBuffer, dwImageBufSize, imageUri);
							delete[] pImageBuffer;
							pImageBuffer = NULL;
							//update callback
							UpdateCallbackStatus("ok","",imageUri,nImageWidth, nImageHeight );

						}
						else
						{
							LOG(ERROR) + L"Unable to send image data as URI, an error occured during file read";		
							UpdateCallbackStatus("error","Unable to send image data as URI, an error occured during file read","");				
						}
					}
					else
					{
						LOG(ERROR) + L"Unable to send image data as URI, could not find captured image";		
						UpdateCallbackStatus("error", "Unable to send image data as URI, could not find captured image","");

					}
				}
				else
				{
					//rho::common::GetJpegResolution( fileName.c_str(), nImageWidth, nImageHeight);
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
		SetFlash(setting);
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
		ResizePreview(pos.right, pos.bottom);	
	}
}
void CDirectShowCam::SetResolution()
{
	if(m_PreviewOn)
	{
		//  Set the resolution of the saved image
		if (!(m_DesiredHeight == -1 && m_DesiredWidth == -1))
		{
			CameraSetting setting = GetNearestResolution();
			SetCameraResolution(setting);
		}
	}
}
void CDirectShowCam::getCameraHWDetails()
{  
	GetResolution(supportedResln, m_szDeviceName, S);
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