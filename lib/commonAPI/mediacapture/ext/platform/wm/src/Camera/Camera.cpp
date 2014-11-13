#include "Camera.h"
#include "PBUtil.h"

#define DEFAULT_FILENAME L"Img"
//HANDLE CCamera::m_hEvents[2] ={NULL, NULL};

CCamera::CCamera(LPCTSTR szDeviceName)
{
	//m_hEvents[eCancelEvent] = CreateEvent(NULL, FALSE, FALSE, NULL);	
	//m_hEvents[eCaptureEvent] = CreateEvent(NULL, TRUE, FALSE, NULL);	
	m_ViewFinder.RegisterCallBack(static_cast<IViewFinderCallBack*>(this));
	_tcscpy(m_szDeviceName, szDeviceName);
	LOG(INFO) + __FUNCTION__ + "Device name set as " + m_szDeviceName; 
	m_PreviewOn = false;
	m_FlashMode = FALSE;
	m_eOutputFormat = eImageUri;
	m_FileName = DEFAULT_FILENAME;
	initializePreviewPos();
}
CCamera::~CCamera()
{
	m_pCameraCb=NULL; //this is just a call back, no need to delete this

	//for(int nIndex = 0; nIndex < eMaxEventCount; nIndex++)
	//{
	//	if(m_hEvents[nIndex])
	//	{
	//		CloseHandle(m_hEvents[nIndex]);
	//		m_hEvents[nIndex] = NULL;
	//	}
	//}
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

	}
	else if(cmp(szPropertyName, L"desiredHeight"))
	{
		m_DesiredHeight = _ttoi(szPropertyValue);
	}
	else if(cmp(szPropertyName, L"fileName"))
	{
		int iLen;
		//if the name was set to L"", then the default name will be selected
		if((iLen = wcslen(szPropertyValue))== 0)
		{
			m_FileName = L"\\" ;
			m_FileName = m_FileName + DEFAULT_FILENAME;
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
	}
	else if(cmp(szPropertyName, L"captureSound"))
	{
		//TODO sound validation
		m_CaptureSound = szPropertyValue;
	}
	else if(cmp(szPropertyName, L"previewLeft"))
	{
		m_PreviewLeft = _ttoi(szPropertyValue);
	}
	else if(cmp(szPropertyName, L"previewTop"))
	{
		m_PreviewTop = _ttoi(szPropertyValue);
	}
	else if(cmp(szPropertyName, L"previewWidth"))
	{
		m_PreviewWidth = _ttoi(szPropertyValue);
	}
	else if(cmp(szPropertyName, L"previewHeight"))
	{
		m_PreviewHeight = _ttoi(szPropertyValue);
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
		else
		{
			if(cmp(szPropertyValue, L"dataUri"))
			{
				m_eOutputFormat = eDataUri;

			}
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
	/*SetEvent(m_hEvents[eCancelEvent]);*/
		
	hidePreview();
	UpdateCallbackStatus("cancel","User cancelled preview","");	

}
void CCamera::captureImage()
{
	//SetEvent(m_hEvents[eCaptureEvent]);
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
	m_PreviewLeft= GetSystemMetrics(SM_CXSCREEN)/2-50;
	m_PreviewTop = GetSystemMetrics(SM_CYSCREEN)/2-50;
	m_PreviewWidth = GetSystemMetrics(SM_CXSCREEN)/2+50;	
	m_PreviewHeight = GetSystemMetrics(SM_CYSCREEN)/2+50;
}
void CCamera::GetDataURI (BYTE* bData, int iLength, rho::String& data)
{
	//  Convert the Signature to base 64, this representation is about 1/3
	//  larger than the binary input
	char* szEncodedString = new char[iLength * 2 + 30];
	memset(szEncodedString, 0, iLength * 2 + 30);

	// Start with the data header
	strcpy(szEncodedString, "data:image/bmp;base64,");

	// Now append the encoded data itself
	EncodeToBase64(bData, iLength, szEncodedString + strlen (szEncodedString));

	// Copy it to the caller
	data = szEncodedString;

	// Clean up
	delete[] szEncodedString;
}
void CCamera::UpdateCallbackStatus(rho::String status, rho::String message, rho::String imageUri)
{
	//if(m_pCameraCb != NULL)
	{	
		rho::Hashtable<rho::String, rho::String> statusData;
		statusData.put( "status", status);	
		if("ok" == status)
		{
			/*statusData.put( "imageHeight","");	
			statusData.put( "imageWidth","");	*/
			rho::String outputFormat;
			if(m_eOutputFormat == eImageUri)
			{
				outputFormat = "image";
				//for image path, set file:// as well so that user can access the link
				rho::String pathPrefix = "file://";
				imageUri= pathPrefix + imageUri;

			}
			else
			{
				outputFormat = "dataUri";
			}		
			statusData.put( "imageFormat", outputFormat);
			statusData.put( "imageUri", imageUri);
			statusData.put( "message", "");
		}
		else
		{
			//for cancel or error set only message
			statusData.put( "message", message);
			statusData.put( "imageFormat", "");
			statusData.put( "imageUri", "");			
		}
		m_pCameraCb.set(statusData);		
	}	
}
