#include "Imager.h"

HINSTANCE CImager::m_hImagerLibHandle = NULL;
bool CImager::m_bImagerLoadFailed = false;

CImager::CImager()
{
}
CImager::~CImager()
{
	if(NULL != m_hImagerLibHandle)
	{
		FreeLibrary(m_hImagerLibHandle);
		m_hImagerLibHandle = NULL;
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

			//Remove column from end of Device Name	, IMG1: renamed to IMG1			
			wcscpy(szModifiedDeviceName, ImageFindInfo.tszDeviceName);
			if (wcslen(szModifiedDeviceName) > 0 &&
				szModifiedDeviceName[wcslen(szModifiedDeviceName) - 1] == L':')
				szModifiedDeviceName[wcslen(szModifiedDeviceName) - 1] = L'\0';
			arIDs.addElement(convertToStringA(szModifiedDeviceName));		
			do{

				if(!_memicmp(g_ListDevName->DeviceFriendlyName, L"Image Capture Driver for Color Camera", wcslen(L"Image Capture Driver for Color Camera")*sizeof(TCHAR)))
				{
					camLookUp.put(convertToStringA(szModifiedDeviceName), eColorCam );
				}							
				else
				{
					camLookUp.put(convertToStringA(szModifiedDeviceName), eImager );
				}
			}
			while(	E_IMG_SUCCESS == Image_FindNext( &ImageFindInfo, FindHandle ));
			Image_FindClose(FindHandle);
		}
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
			wcscpy(szParameterValue,m_FileName);			
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
			m_FileName = szPropertyValue;		
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

