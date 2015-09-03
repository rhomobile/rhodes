#include "Camera.h"
#include "Common/RhoUtil.h"
#include "common/RhodesApp.h"
#include "common/ExtManager.h"
#include <atltime.h>
#define DEFAULT_FILENAME L"\\Img"
#define TRIGGER_EVENT_NAME L"CameraTrigger"

bool CCamera::m_IsCameraRunning = false;
bool CCamera::m_bRcmLoaded = false;
bool CCamera::m_bAppHasFocus= true;
rho::StringW CCamera::m_ImageUriPath;
CTriggerMonitor CCamera::m_Rcm;
HANDLE CCamera::m_hTriggerEvents[eTriggerEventMax] = {NULL, NULL};
HANDLE CCamera::m_hRegisterTrigger = NULL;
HANDLE CCamera::m_hTriggerMonitorThread = NULL;
bool CCamera::m_bIsDeprecated = false;

CCamera::CCamera(LPCTSTR szDeviceName)
{
	m_ViewFinder.RegisterCallBack(static_cast<IViewFinderCallBack*>(this));
	_tcscpy(m_szDeviceName, szDeviceName);
	LOG(INFO) + __FUNCTION__ + "Device name set as " + m_szDeviceName; 
	m_PreviewOn = false;
	m_FlashMode = FALSE;
	m_eOutputFormat = eImagePath;
	m_FileName = DEFAULT_FILENAME;
	m_DesiredHeight = -1;
	m_DesiredWidth =-1;
	initializePreviewPos();
	/*if(false == m_bRcmLoaded)
	{
		if(m_Rcm.LoadRcmDLL())
		{
			m_bRcmLoaded = true;
		}
	}*/
}
CCamera::~CCamera()
{
	m_pCameraCb=NULL; //this is just a call back, no need to delete this
	if(m_bRcmLoaded)
	{
		m_Rcm.UnloadRcmDLL();
	}

}
BOOL CCamera::getProperty(LPCTSTR szParameterName, WCHAR* szParameterValue)
{	
	BOOL bRetStatus = TRUE;
	if(cmp(szParameterName, L"desiredWidth"))
	{
		wcscpy(szParameterValue,rho::common::convertToStringW(m_DesiredWidth).c_str());	

	}
	else if(cmp(szParameterName, L"desiredHeight"))
	{
		wcscpy(szParameterValue,rho::common::convertToStringW(m_DesiredHeight).c_str());		
	}
	else if(cmp(szParameterName, L"fileName"))
	{
		wcscpy(szParameterValue,m_FileName.c_str());		
	}
	else if(cmp(szParameterName, L"flashMode"))
	{
		if(m_FlashMode)
		{
			wcscpy(szParameterValue, L"on");	
		}
		else
		{
			wcscpy(szParameterValue, L"off");	
		}

	}
	else if(cmp(szParameterName, L"captureSound"))
	{
		wcscpy(szParameterValue,m_CaptureSound.c_str());	
	}
	else if(cmp(szParameterName, L"previewLeft"))
	{
		wcscpy(szParameterValue,rho::common::convertToStringW(m_PreviewLeft).c_str());	
	}
	else if(cmp(szParameterName, L"previewTop"))
	{
		wcscpy(szParameterValue,rho::common::convertToStringW(m_PreviewTop).c_str());		
	}
	else if(cmp(szParameterName, L"previewWidth"))
	{
		wcscpy(szParameterValue,rho::common::convertToStringW(m_PreviewWidth).c_str());		
	}
	else if(cmp(szParameterName, L"previewHeight"))
	{
		wcscpy(szParameterValue,rho::common::convertToStringW(m_PreviewHeight).c_str());	
	}
	else if(cmp(szParameterName, L"compressionFormat"))
	{
		wcscpy(szParameterValue, L"jpg");
	}
	else if(cmp(szParameterName, L"cameraType"))
	{
		wcscpy(szParameterValue, m_CamType.c_str());
	}
	else if(cmp(szParameterName, L"outputFormat"))
	{
		if(m_eOutputFormat == eImageUri)
		{
			wcscpy(szParameterValue, L"image");
		}
        else if(m_eOutputFormat == eImagePath)
        {
            wcscpy(szParameterValue, L"imagePath");
        }
		else
		{
			wcscpy(szParameterValue, L"dataUri");
		}		
	}
	else
	{
		bRetStatus = FALSE;
	}

	return bRetStatus;
}
BOOL CCamera::setProperty(LPCTSTR szPropertyName, LPCTSTR szPropertyValue)
{
	BOOL bRetStatus = TRUE;
	if(cmp(szPropertyName, L"desiredWidth"))
	{
		m_DesiredWidth = _ttoi(szPropertyValue);
		SetResolution();

	}
	else if(cmp(szPropertyName, L"desiredHeight"))
	{
		m_DesiredHeight = _ttoi(szPropertyValue);
		SetResolution();
	}
	else if(cmp(szPropertyName, L"fileName"))
	{
		int iLen;
		//if the name was set to L"", then the default name will be selected
		if((iLen = wcslen(szPropertyValue))== 0)
		{			
			m_FileName = DEFAULT_FILENAME;
		}
		else
		{	
			if(szPropertyValue[0] == '\\')
			{
				m_FileName = szPropertyValue;
			}
			else
			{
				m_FileName = L"\\" ;
				m_FileName = m_FileName + szPropertyValue;
			}			
			
		}
			
		
	}
	else if(cmp(szPropertyName, L"flashMode"))
	{
		if((cmp(szPropertyValue, L"on")))
		{
			m_FlashMode = TRUE;		
		}
		else
		{
			if((cmp(szPropertyValue, L"off")))
			{
				m_FlashMode = FALSE;
			}

		}
		SetFlashMode();
		
	}
	else if(cmp(szPropertyName, L"captureSound"))
	{
		//TODO sound validation
		m_CaptureSound = szPropertyValue;
	}
	else if(cmp(szPropertyName, L"previewLeft"))
	{
		m_PreviewLeft = _ttoi(szPropertyValue);
		ResetViewerWndPos();
	}
	else if(cmp(szPropertyName, L"previewTop"))
	{
		m_PreviewTop = _ttoi(szPropertyValue);
		ResetViewerWndPos();
	}
	else if(cmp(szPropertyName, L"previewWidth"))
	{
		m_PreviewWidth = _ttoi(szPropertyValue);
		ResetViewerWndPos();
	}
	else if(cmp(szPropertyName, L"previewHeight"))
	{
		m_PreviewHeight = _ttoi(szPropertyValue);
		ResetViewerWndPos();
	}
	else if(cmp(szPropertyName, L"compressionFormat"))
	{
		//always it should be jpg, hence user will not be allowed to mofify
	}
	else if(cmp(szPropertyName, L"cameraType"))
	{
		//this property should be readonly
	}
	else if(cmp(szPropertyName, L"outputFormat"))
	{
		if(cmp(szPropertyValue, L"image"))
		{
			m_eOutputFormat = eImageUri;
		}
		else if(cmp(szPropertyValue, L"dataUri"))
		{
			m_eOutputFormat = eDataUri;		
		}
		else if(cmp(szPropertyValue, L"imagePath"))
		{
			m_eOutputFormat = eImagePath;
		}
	}
	else
	{
		bRetStatus = FALSE;
	}
	return bRetStatus;
}

void CCamera::getSupportedPropertyList(rho::Vector<rho::String>& arrayofNames)
{
	arrayofNames.push_back("desiredWidth");
	arrayofNames.push_back("desiredHeight");	
	arrayofNames.push_back("fileName");
	arrayofNames.push_back("flashMode");	
	arrayofNames.push_back("captureSound");
	arrayofNames.push_back("previewLeft");	
	arrayofNames.push_back("previewTop");
	arrayofNames.push_back("previewWidth");	
	arrayofNames.push_back("previewHeight");
	arrayofNames.push_back("compressionFormat");
	arrayofNames.push_back("cameraType");
	arrayofNames.push_back("outputFormat");

}
void CCamera::cancel()
{	
	hidePreview();
	/*if(m_hTriggerEvents[eCancel])
	{
		SetEvent(m_hTriggerEvents[eCancel]);
	}*/
	UpdateCallbackStatus("cancel","User canceled operation.","");	

}
void CCamera::close()
{
	hidePreview();
}
void CCamera::captureImage()
{
	Capture();
	hidePreview();
}
void CCamera::SetCallback(rho::apiGenerator::CMethodResult& pCallback)
{
	m_pCameraCb = pCallback;
}
BOOL CCamera::cmp(LPCTSTR tc1, LPCTSTR tc2)
{
	if (!tc1 || !tc2)
		return false;
	return !_tcsicmp(tc1, tc2);

}
//below method initializes initial position of the user configurable preview window
void CCamera::initializePreviewPos()
{
	RECT pos;
	rho::common::CRhoExtData rhodes_data = RHODESAPP().getExtManager().makeExtData();  
    GetClientRect(rhodes_data.m_hBrowserWnd,&pos);
	m_PreviewLeft= pos.right/2-50;
	m_PreviewTop = pos.bottom/2-50;
	m_PreviewWidth = pos.right/2+50;	
	m_PreviewHeight = pos.bottom/2+50;
}
void CCamera::UpdateCallbackStatus(rho::String status, rho::String message, rho::String imageUri,int nImageWidth, int nImageHeight)
{
	char imageHeight[6];
	char imageWidth[6];

	rho::Hashtable<rho::String, rho::String> statusData;
	statusData.put( "status", status);	

	imageHeight[0] = 0;
	imageWidth[0] = 0;
	sprintf(imageHeight,"%d",nImageHeight);
	sprintf(imageWidth,"%d",nImageWidth);

	if(false == m_bIsDeprecated)
	{
		statusData.put( "imageHeight",imageHeight);
		statusData.put( "imageWidth", imageWidth);	
	}
	else
	{
		statusData.put( "image_height", imageHeight);
		statusData.put( "image_width", imageWidth);
	}

	if("ok" == status)
	{

		rho::String outputFormat;		
		if(m_eOutputFormat == eImageUri)
		{
			DeleteFile(m_ImageUriPath.c_str());
			rho::String appRootPath;
			rho::String fileName;
			rho::String newFilePath;
			appRootPath = RHODESAPP().getAppRootPath();
			unsigned int index = imageUri.find_last_of("\\");		
			if(index > 0)
			{
				fileName = imageUri.substr(index);
			}
			else
			{
				fileName = imageUri;
			}
			newFilePath = appRootPath + "/" + fileName;
			m_ImageUriPath = rho::common::convertToStringW(newFilePath);
			rho::StringW szExistingPath= rho::common::convertToStringW(imageUri);
			CopyFile(szExistingPath.c_str(), m_ImageUriPath.c_str(), TRUE);
			imageUri = fileName;

		}	
		outputFormat = "jpg";//note, there is a confusion here, outputFormat we use here is to say in what format image saved
		statusData.put( "message", "");

		if(false == m_bIsDeprecated)
		{
			statusData.put( "imageFormat", outputFormat);
			statusData.put( "imageUri", imageUri);
		}
		else
		{
			statusData.put( "image_format", imageUri);
			statusData.put( "image_uri", outputFormat);
		}

	}
	else
	{
		//for cancel or error set only message
		statusData.put( "message", message);
		if(false == m_bIsDeprecated)
		{
			statusData.put( "imageFormat", "");
			statusData.put( "imageUri", "");
		}
		else
		{
			statusData.put( "image_format", "");
			statusData.put( "image_uri", "");	
		}

	}
	m_pCameraCb.set(statusData);		

}
void CCamera::RedrawViewerWnd(RECT& pos)
{
	if(m_PreviewOn)
	{
		m_ViewFinder.RepositionWindow(pos);
	}
}
void CCamera::ResetViewerWndPos()
{
	if(m_PreviewOn)
	{
		RECT pos;
		pos.left = m_PreviewLeft;
		pos.top = m_PreviewTop;
		pos.right = m_PreviewWidth;
		pos.bottom = m_PreviewHeight;
		RedrawViewerWnd(pos);

	}
}
void CCamera::ResetViewerWndPos(RECT& pos)
{
	if(m_PreviewOn)
	{
		RedrawViewerWnd(pos);
	}
}
void CCamera::createTriggerMonitorThread(LPVOID pparam)
{
	if(m_bRcmLoaded)
	{
		UnregisterTriggerMonitor();
		// Create event to signal thread to quit
		m_hTriggerEvents[0] = CreateEvent (NULL, TRUE, FALSE, NULL);
		// Create auto-reset event which system will signal when trigger occurs
		m_hTriggerEvents[1] = CreateEvent (NULL, FALSE, FALSE, TRIGGER_EVENT_NAME);
		DWORD dwRes;
		// Register with system for trigger notifications
		if ((dwRes = m_Rcm.lpfn_RCM_RegisterTriggerEvent (TRIGGER_ALL_MASK, FALSE, TRIGGER_EVENT_NAME , &m_hRegisterTrigger)) != E_RCM_SUCCESS)
		{
			WCHAR error[100];
			wsprintf (error, L"RegisterTriggerEvent failed (%lu)", dwRes);
			LOG(WARNING) + error;	
			closeTriggerEvents();

		}
		else
		{
			// Create thread
			m_hTriggerMonitorThread = CreateThread (NULL, 0, TriggerMonitorProc, pparam, 0, NULL);
		}
	}
	else
	{
		LOG(WARNING) + L"This device doesn't support camera capture using trigger button";
	}

	
}
DWORD CCamera::TriggerMonitorProc (LPVOID pparam)
{ 
	bool bContinue = true;
	CCamera* pCam = (CCamera*)pparam;
	DWORD dwEvent = WAIT_OBJECT_0;
	while(bContinue)
	{
		dwEvent = WaitForMultipleObjects(2, m_hTriggerEvents, FALSE, INFINITE);

		// ignore trigger events if we're in the background
		if (false == CCamera::m_bAppHasFocus)
			continue;
        
		// Unregister for notifications
		UnregisterTriggerMonitor();		
		DWORD dwEventIndex = dwEvent - WAIT_OBJECT_0; 
		switch(dwEventIndex)
		{

		case eCancel:
			{					
				bContinue = false;
				break;
			}
		case eTrigger:
			{
				pCam->DisableFullScreenButtons();
				pCam->captureImage();
				bContinue = false;
				break;

			}
		default:
			break;
		}
	}
	return 0;
}
void CCamera::closeTriggerEvents()
{
	for(int nIndex = 0; nIndex < eTriggerEventMax; nIndex++)
	{
		if(m_hTriggerEvents[nIndex])
		{
			CloseHandle(m_hTriggerEvents[nIndex]);
			m_hTriggerEvents[nIndex] = NULL;
		}
	}
	
}
void CCamera::ApplicationFocusChange(bool bAppHasFocus)
{
	m_bAppHasFocus = bAppHasFocus;	
}
void CCamera::DisableFullScreenButtons()
{
	m_ViewFinder.DisableFullScreenButtons();
}
rho::StringW CCamera::getFileName()
{
	rho::StringW szExtn = L".jpg";
	if(cmp(m_FileName.c_str(), DEFAULT_FILENAME))
	{
		//get time stamp along with extension for default file names
		szExtn = generate_filename(szExtn.c_str());
	}
	return m_FileName + szExtn;

}

rho::StringW CCamera::generate_filename(LPCTSTR szExt) 
{
    TCHAR filename[256];

	CTime time(CTime::GetCurrentTime());
	tm tl, tg;
	time.GetLocalTm(&tl);
	time.GetGmtTm(&tg);
	int tz = tl.tm_hour-tg.tm_hour; //TBD: fix tz

    wsprintf(filename, L"_%02i-%02i-%0004i_%02i.%02i.%02i_%c%03i%s", 
		tg.tm_mon, tg.tm_mday, 1900+tg.tm_year, tg.tm_hour, tg.tm_min, tg.tm_sec,  
        tz>0?'_':'-',abs(tz),(szExt?szExt:L""));

	return filename;
}
eImageFilePathErrorType CCamera::isImageFilePathValid()
{	
	eImageFilePathErrorType retVal = eUnknownFilePathErrorType;
	rho::StringW filePath;
	unsigned int index = m_FileName.find_last_of(L"\\");
	if(index > 0)
	{
		//we reach here if user given path contains any folder eg: \\folderName\\img or \\Application\\img
		filePath= m_FileName.substr(0,index);
		DWORD dwResult = GetFileAttributes(filePath.c_str());
		switch(dwResult)
		{
		case 0xFFFFFFFF:
			{
				retVal = eFileNotExist;
				break;
			}
		case FILE_ATTRIBUTE_READONLY:
			{
				retVal = eFileReadOnly;
				break;
			}
		default:
			{
				retVal = eFilePathValid;
				break;
			}
		}
	}
	else
	{
		//we reach here if user given path is root directory (eg: \\img or img)
		retVal = eFilePathValid;		
	}
	
	return retVal;

}
void CCamera::UnregisterTriggerMonitor()
{
	if(m_bRcmLoaded)
	{
		m_Rcm.lpfn_RCM_DeregisterTrigger(m_hRegisterTrigger);
		m_hRegisterTrigger = NULL;
		closeTriggerEvents();
	}
}
void CCamera::SetAPICallType(bool bIsDeprecated)
{
	m_bIsDeprecated = bIsDeprecated;
}