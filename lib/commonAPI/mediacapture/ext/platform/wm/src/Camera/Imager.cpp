#include <windows.h>
#include "Imager.h"


HINSTANCE CImager::m_hImagerLibHandle = NULL;
bool CImager::m_bImagerLoadFailed = false;
IMAGE_FINDCLOSEPROC CImager::Image_FindClose= NULL;
IMAGE_FINDFIRSTPROC CImager::Image_FindFirst= NULL;
IMAGE_FINDNEXTPROC CImager::Image_FindNext= NULL;
IMAGE_OPENPROC CImager::Image_Open= NULL;
IMAGE_CLOSEPROC CImager::Image_Close= NULL;
IMAGE_STARTACQUISITIONPROC CImager::Image_StartAcquisition= NULL;
IMAGE_STOPACQUISITIONPROC CImager::Image_StopAcquisition= NULL;
IMAGE_SETCAPCURRVALUEPROC CImager::Image_SetCapCurrValue= NULL;
IMAGE_UNLOCKIMAGERPROC CImager::Image_UnLockImager= NULL;

IMAGE_GETIMAGEPROC CImager::Image_GetImage= NULL;
IMAGE_STARTVIEWFINDERPROC CImager::Image_StartViewfinder= NULL;
IMAGE_STOPVIEWFINDERPROC CImager::Image_StopViewfinder= NULL;

#pragma

CImager::CImager(LPCTSTR szDeviceName):CCamera(szDeviceName)
{
    m_AimMode = FALSE;
	m_CamType = L"imager";
	m_hImager = INVALID_HANDLE_VALUE;
	InitImager();
}
CImager::~CImager()
{
    if(NULL != m_hImagerLibHandle)
    {
        FreeLibrary(m_hImagerLibHandle);
        m_hImagerLibHandle = NULL;
    }
	if(m_hImager != INVALID_HANDLE_VALUE)
	{
		Image_UnLockImager(m_hImager);
		Image_Close(m_hImager);
		m_hImager=INVALID_HANDLE_VALUE;
	}
}
BOOL CImager::enumerate(rho::Vector<rho::String>& arIDs, rho::Hashtable<rho::String, eCamType>& camLookUp)
{
    BOOL bRetStatus = FALSE;
    if(NULL != m_hImagerLibHandle)
    {
        FreeLibrary(m_hImagerLibHandle);
        m_hImagerLibHandle = NULL;
    }
    m_hImagerLibHandle = LoadLibraryEx(_T("ImgAPI32.dll"), NULL, 0);
    // Find the Imager
    if (NULL != m_hImagerLibHandle)
    {
        DWORD dwRes;
        m_bImagerLoadFailed = false;
        if (NULL == (Image_FindFirst = (IMAGE_FINDFIRSTPROC) GetProcAddress(m_hImagerLibHandle, _T("IMAGE_FindFirst")))) m_bImagerLoadFailed = true;
        if (NULL == (Image_FindNext = (IMAGE_FINDNEXTPROC) GetProcAddress(m_hImagerLibHandle, _T("IMAGE_FindNext")))) m_bImagerLoadFailed = true;
        if (NULL == (Image_FindClose = (IMAGE_FINDCLOSEPROC) GetProcAddress(m_hImagerLibHandle, _T("IMAGE_FindClose")))) m_bImagerLoadFailed = true;
        if (NULL == (Image_Open = (IMAGE_OPENPROC) GetProcAddress(m_hImagerLibHandle, _T("IMAGE_Open")))) m_bImagerLoadFailed = true;
        if (NULL == (Image_Close = (IMAGE_CLOSEPROC) GetProcAddress(m_hImagerLibHandle, _T("IMAGE_Close")))) m_bImagerLoadFailed = true;
        if (NULL == (Image_StartAcquisition = (IMAGE_STARTACQUISITIONPROC) GetProcAddress(m_hImagerLibHandle, _T("IMAGE_StartAcquisition")))) m_bImagerLoadFailed = true;
        if (NULL == (Image_StopAcquisition = (IMAGE_STOPACQUISITIONPROC) GetProcAddress(m_hImagerLibHandle, _T("IMAGE_StopAcquisition")))) m_bImagerLoadFailed = true;
        if (NULL == (Image_StartViewfinder = (IMAGE_STARTVIEWFINDERPROC) GetProcAddress(m_hImagerLibHandle, _T("IMAGE_StartViewfinder")))) m_bImagerLoadFailed = true;
        if (NULL == (Image_StopViewfinder = (IMAGE_STOPVIEWFINDERPROC) GetProcAddress(m_hImagerLibHandle, _T("IMAGE_StopViewfinder")))) m_bImagerLoadFailed = true;
        if (NULL == (Image_GetImage = (IMAGE_GETIMAGEPROC) GetProcAddress(m_hImagerLibHandle, _T("IMAGE_GetImage")))) m_bImagerLoadFailed = true; 		
        if (NULL == (Image_SetCapCurrValue = (IMAGE_SETCAPCURRVALUEPROC) GetProcAddress(m_hImagerLibHandle, _T("IMAGE_SetCapCurrValue")))) m_bImagerLoadFailed = true;
        if (NULL == (Image_UnLockImager = (IMAGE_UNLOCKIMAGERPROC) GetProcAddress(m_hImagerLibHandle, _T("IMAGE_UnLockImager")))) m_bImagerLoadFailed = true;

        IMAGE_FINDINFO ImageFindInfo;
        HANDLE FindHandle;

        memset(&ImageFindInfo, 0, sizeof(ImageFindInfo));
        SI_INIT(&ImageFindInfo);
        SI_SET_USED(&ImageFindInfo, tszRegistryBasePath);

        TCHAR szModifiedDeviceName[MAX_PATH + 1];
        if (E_IMG_SUCCESS == (dwRes = Image_FindFirst(&ImageFindInfo, &FindHandle))) 
        {
            bRetStatus = TRUE;//found atleast one occurance
            do
            {

                //Remove column from end of Device Name	, IMG1: renamed to IMG1			
                wcscpy(szModifiedDeviceName, ImageFindInfo.tszDeviceName);
                /*if (wcslen(szModifiedDeviceName) > 0 &&
                szModifiedDeviceName[wcslen(szModifiedDeviceName) - 1] == L':')
                szModifiedDeviceName[wcslen(szModifiedDeviceName) - 1] = L'\0';*/
                arIDs.addElement(rho::common::convertToStringA(szModifiedDeviceName));	

                if(!_memicmp(ImageFindInfo.tszFriendlyName, L"Image Capture Driver for Color Camera", wcslen(L"Image Capture Driver for Color Camera")*sizeof(TCHAR)))
                {
                    //camLookUp.put(rho::common::convertToStringA(szModifiedDeviceName), eColorCam );
                }
                else
                {
                    camLookUp.put(rho::common::convertToStringA(szModifiedDeviceName), eImager );
                }



            }while(	E_IMG_SUCCESS == Image_FindNext( &ImageFindInfo, FindHandle ));
        }

        Image_FindClose(FindHandle);

    }

    return bRetStatus;
}
BOOL CImager::getProperty(LPCTSTR szParameterName, WCHAR* szParameterValue)
{
    BOOL bRetStatus = TRUE;
    bRetStatus = CCamera::getProperty(szParameterName, szParameterValue);
    if(FALSE == bRetStatus)
    {
        if(cmp(szParameterName, L"aimMode"))
        {
            if(m_AimMode)
            {
                wcscpy(szParameterValue, L"on");	
            }
            else
            {
                wcscpy(szParameterValue, L"off");	
            }			
        }
        else
        {
            bRetStatus = FALSE;
        }
    }
    return bRetStatus;
}
BOOL CImager::setProperty(LPCTSTR szPropertyName, LPCTSTR szPropertyValue)
{
    BOOL bRetStatus = TRUE;
    bRetStatus = CCamera::setProperty(szPropertyName, szPropertyValue);
    if(FALSE == bRetStatus)
    {
        if(cmp(szPropertyName, L"aimMode"))
        {
            if((cmp(szPropertyValue, L"on")))
            {				
                m_AimMode = TRUE;
				Image_SetCapCurrValue(m_hImager, IMG_ACQCAP_AIMING, sizeof(BOOL), &m_AimMode);
            }
            else
            {
                if((cmp(szPropertyValue, L"off")))
                {
                    m_AimMode = FALSE;
					Image_SetCapCurrValue(m_hImager, IMG_ACQCAP_AIMING, sizeof(BOOL), &m_AimMode);
                }

            }		
        }
        else
        {
            bRetStatus = FALSE;
        }
    }
    return bRetStatus;
}
void CImager::getSupportedPropertyList(rho::Vector<rho::String>& arrayofNames)
{
    CCamera::getSupportedPropertyList(arrayofNames);
    arrayofNames.push_back("aimMode");	
}


void CImager::takeFullScreen()
{
	LOG(INFO) + __FUNCTION__ ; 
	RECT pos;	
	if(false == m_IsCameraRunning)
	{
		if(m_PreviewOn == false)
		{
			if(m_hImager != INVALID_HANDLE_VALUE)
			{
				if(startPreview(pos, eFullScreen))
				{

					m_IsCameraRunning = true;
					m_PreviewOn = true;
				}
			}
			else
			{
				LOG(INFO) + __FUNCTION__  + L"Unable to open imager";
				UpdateCallbackStatus("error","takeFullScreen failed", "");
			}
		}
	}
}
BOOL CImager::showPreview()
{
	BOOL bPreviewStatus = FALSE;
    LOG(INFO) + __FUNCTION__ ; 
	if(m_IsCameraRunning == false)
	{
		if(m_PreviewOn == false)
		{
			if(m_hImager != INVALID_HANDLE_VALUE)
			{
				RECT pos;
				pos.left = m_PreviewLeft;
				pos.top = m_PreviewTop;
				pos.right = m_PreviewWidth;	
				pos.bottom = m_PreviewHeight;
				bPreviewStatus = startPreview(pos, eConfigurable);
				if(TRUE == bPreviewStatus)
				{
					m_PreviewOn = true;
					m_IsCameraRunning = true;
				}
				else
				{
					m_PreviewOn = false;
					m_IsCameraRunning = false;
				}

			}
		}
	}
	return bPreviewStatus;
}
BOOL CImager::startPreview(RECT& pos, eViewrWndMode eMode )
{
	BOOL bPreviewStatus = FALSE;
	DWORD dwRes = E_IMG_SUCCESS;
	// Imager is enabled so try to start it
	if ( (dwRes = StartViewer(pos, eMode)) != E_IMG_SUCCESS)
	{
		LOG(WARNING) + __FUNCTION__ + "StartViewer failed, retrying"  ; 
		// if we're switching between apps with the camera active this can happen
		// so retry a few times
		int i=0;
		do 
		{
			if (++i >= 5)
			{
				LOG(WARNING) + __FUNCTION__ + L"StartViewer failed 5 times" ;				
				break;
			}			
			Sleep(100);
		} while ((dwRes = StartViewer(pos, eMode)) != E_IMG_SUCCESS);

		
	}
	if(E_IMG_SUCCESS != dwRes)
	{
		UpdateCallbackStatus("error", "Failed to set the imager properties", ""); 	
	}
	else
	{
		bPreviewStatus = TRUE;
	}
	return bPreviewStatus;
}
////////////////////////////////////////////////////////////////////////
// Function:	InitImager
// Description:	Finds first Imager device and opens handle to image capture device, 
//              image capture device handle is used to call supported Image capture APIs.			

////////////////////////////////////////////////////////////////////////
DWORD CImager::InitImager()
{	
	DWORD dwRes = E_IMG_SUCCESS;
	if(m_hImager == INVALID_HANDLE_VALUE)
	{		
		IMAGE_FINDINFO ImageFindInfo;
		memset(&ImageFindInfo, 0, sizeof(ImageFindInfo));
		SI_INIT(&ImageFindInfo);
		SI_SET_USED(&ImageFindInfo, tszRegistryBasePath);
		dwRes = Image_Open(m_szDeviceName, &m_hImager);
		if(dwRes != E_IMG_SUCCESS)
		{
			LOG(ERROR) + __FUNCTION__ + "Image open failed for device: " + m_szDeviceName ; 
		}
	}
	return dwRes;
}
////////////////////////////////////////////////////////////////////////
// Function:	StartViewer
// Description:	This function starts Viewfinder for opened image capture device.
//              image capture device handle is used to call supported Image capture APIs.
//				Sets JPEG as default image format. Sets the Light and Aim to default state FALSE
////////////////////////////////////////////////////////////////////////
DWORD CImager::StartViewer(RECT& pos, eViewrWndMode eMode)
{
    DWORD dwCapValue;
    DWORD dwRes = E_IMG_SUCCESS; 
    bool bErrorFlag = true;   
    HWND hWndViewer = m_ViewFinder.CreateViewerWindow(pos, eMode);

    // Set the width of the target window for the video viewfinder
    dwCapValue = pos.right;// - pos.left + 1;
    if((dwRes = Image_SetCapCurrValue(m_hImager, IMG_VFCAP_WINWIDTH, sizeof(dwCapValue), &dwCapValue) == E_IMG_SUCCESS))
    {
        // Set the height of the target window for the video viewfinder
        dwCapValue = pos.bottom;// - pos.top + 1;
        if(Image_SetCapCurrValue(m_hImager, IMG_VFCAP_WINHEIGHT, sizeof(dwCapValue), &dwCapValue) == E_IMG_SUCCESS)
        {
            BOOL bCrop = TRUE;
            if(Image_SetCapCurrValue(m_hImager, IMG_IMGCAP_MATCHVFCAPCROP, sizeof(BOOL), &bCrop) == E_IMG_SUCCESS)
            {
                // Specifies the window handle to be used to display the viewfinder
                dwCapValue = (DWORD)hWndViewer;
                if(Image_SetCapCurrValue(m_hImager, IMG_VFCAP_WINHANDLE, sizeof(dwCapValue), &dwCapValue) == E_IMG_SUCCESS)
                {
                    dwCapValue = TWFF_JPEG; 
                    if(Image_SetCapCurrValue(m_hImager, IMG_IMGCAP_FILEFORMAT, sizeof(dwCapValue), &dwCapValue) == E_IMG_SUCCESS)
                    {
                        // Start acquiring images and Turn on the viewfinder
                        if ((dwRes = Image_StartAcquisition(m_hImager)) == E_IMG_SUCCESS) 
                        {
                            if ((dwRes = Image_StartViewfinder(m_hImager)) == E_IMG_SUCCESS) 
                            {
                                Image_SetCapCurrValue(m_hImager, IMG_ACQCAP_LAMPSTATE, sizeof(BOOL), &m_FlashMode);
                                Image_SetCapCurrValue(m_hImager, IMG_ACQCAP_AIMING, sizeof(BOOL), &m_AimMode);
                                bErrorFlag = false;//we reach here if no error found


                            }

                        }

                    }

                }

            }

        }

    }

    if(bErrorFlag)
    {
        TCHAR errorMessage[100];
        wsprintf(errorMessage, L"Imager_SetCapCurrValue error %d", dwRes);
		LOG(ERROR) + errorMessage;
        m_ViewFinder.DestroyViewerWindow();		       
    }

    /*if (m_hParentWnd)
    SetFocus(m_hParentWnd);*/

    return dwRes;
}

BOOL CImager::hidePreview()
{
	LOG(INFO) + __FUNCTION__ 	;
	BOOL bRetStatus = FALSE;
	if (m_PreviewOn)
	{	
		if(E_IMG_SUCCESS == StopViewer())
		{
			m_PreviewOn = false;
		    m_IsCameraRunning = false;
			bRetStatus = TRUE;
		}

	}
	return bRetStatus;
	
}
////////////////////////////////////////////////////////////////////////
// Function:	StopViewer
// Description:	This function stop Viewfinder for opened image capture device.
////////////////////////////////////////////////////////////////////////
DWORD CImager::StopViewer()
{
	LOG(INFO) + __FUNCTION__;
	DWORD dwRes = E_IMG_SUCCESS;
	TCHAR message[100];
	// Stop the viewfinder first
	if (E_IMG_SUCCESS != (dwRes = Image_StopViewfinder(m_hImager)))
	{
		wsprintf(message, L"Imager_StopViewer error %d \n", dwRes);
		LOG(ERROR) + __FUNCTION__ + message;
		return dwRes;

	}
	// Stop the acquisition
	if (E_IMG_SUCCESS != (dwRes = Image_StopAcquisition(m_hImager))) 
	{
		wsprintf(message, L"Imager_StopAcquisition error %d \n", dwRes);
		LOG(ERROR) + __FUNCTION__ + message;
		return dwRes;
	}
		
	m_ViewFinder.DestroyViewerWindow();
	return E_IMG_SUCCESS;
}


void CImager::Capture()
{
    LOG(INFO) + __FUNCTION__ ;
    DWORD dwRes;
    DWORD dwBytesWritten;
    if(m_PreviewOn)
    {
        if (m_hImager != INVALID_HANDLE_VALUE)
            Image_StopViewfinder(m_hImager);

        PlaySound(m_CaptureSound.c_str(),NULL,SND_FILENAME|SND_ASYNC);

        DWORD dwImageBufSize;///< Variable for Buffer size of captured image
        LPVOID pImageBuffer;///< Buffer to save captured image

        if ((dwRes = Image_GetImage(m_hImager, &dwImageBufSize, &pImageBuffer)) != E_IMG_SUCCESS) 
        {
            TCHAR message[100];
            wsprintf(message, L"Image_GetImage error %d \n", dwRes);
            LOG(ERROR) + __FUNCTION__ + message;

            UpdateCallbackStatus("error","Image Capture operation failed.","");


        }
        else
        {
			rho::StringW fileName = m_FileName + L".jpg";
            HANDLE hFile = 
                CreateFile(fileName.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
            if(hFile)
            {
                if(WriteFile(hFile,pImageBuffer,dwImageBufSize,&dwBytesWritten,NULL))
                {
                    rho::String imageUri;
                    if(m_eOutputFormat == eDataUri)
                    {
                        GetDataURI((BYTE*)pImageBuffer, dwBytesWritten, imageUri);
                    }
                    else
                    {
                        imageUri = rho::common::convertToStringA(fileName).c_str();
                    }
                    //update callback
                    UpdateCallbackStatus("ok","",imageUri);
                }
                else
                {
                    UpdateCallbackStatus("error","Unable to save image","");
                }
                CloseHandle(hFile);

            }
            else
            {
                UpdateCallbackStatus("error","Unable to save image","");
            }

        }	


        if (m_hImager != INVALID_HANDLE_VALUE)
            Image_StartViewfinder(m_hImager);	
    }
	
}

void CImager::SetFlashMode()
{
  Image_SetCapCurrValue(m_hImager, IMG_ACQCAP_LAMPSTATE, sizeof(BOOL), &m_FlashMode);		
}
void CImager::SetReolution()
{
}
void CImager::RedrawViewerWnd(RECT& pos)
{
	TCHAR message[100];
	DWORD dwRes;
	if(m_PreviewOn)
	{
		CCamera::RedrawViewerWnd(pos);
		// Set the width of the target window for the video viewfinder
		DWORD dwCapValue = pos.right;// - m_rcWinPos.left + 1;
		if((dwRes = Image_SetCapCurrValue(m_hImager, IMG_VFCAP_WINWIDTH, sizeof(dwCapValue), &dwCapValue) != E_IMG_SUCCESS))
		{
			wsprintf(message, L"Imager_SetCapCurrValue error %d", dwRes);
			LOG(ERROR) + __FUNCTION__ + message;	
		}
		// Set the height of the target window for the video viewfinder
		dwCapValue = pos.bottom;// - m_rcWinPos.top + 1;
		if(Image_SetCapCurrValue(m_hImager, IMG_VFCAP_WINHEIGHT, sizeof(dwCapValue), &dwCapValue) != E_IMG_SUCCESS)
		{
			wsprintf(message, L"Imager_SetCapCurrValue error %d", dwRes);
			LOG(ERROR) + __FUNCTION__ + message;	
		}
		BOOL bCrop = TRUE;
		if(Image_SetCapCurrValue(m_hImager, IMG_IMGCAP_MATCHVFCAPCROP, sizeof(BOOL), &bCrop) != E_IMG_SUCCESS)
		{

			wsprintf(message, L"Imager_SetCapCurrValue error %d", dwRes);
			LOG(ERROR) + __FUNCTION__ + message;	
		}
		// Specifies the window handle to be used to display the viewfinder
		dwCapValue = (DWORD)m_ViewFinder.getViewerWndHandle();
		if(Image_SetCapCurrValue(m_hImager, IMG_VFCAP_WINHANDLE, sizeof(dwCapValue), &dwCapValue) != E_IMG_SUCCESS)
		{

			wsprintf(message, L"Imager_SetCapCurrValue error %d", dwRes);
			LOG(ERROR) + __FUNCTION__ + message;	
		}
	}
}