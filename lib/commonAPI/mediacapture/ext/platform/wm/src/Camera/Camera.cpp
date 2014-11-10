#include "Camera.h"

CCamera::CCamera()
{
//	m_pImageCaptureCb = NULL;
}
CCamera::~CCamera()
{
	//m_pImageCaptureCb=NULL //this is just a call back, no need to delete this
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
		wcscpy(szParameterValue, m_FlashMode.c_str());		
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
		//TODO file name validation
		m_FileName = szPropertyValue;
	}
	else if(cmp(szPropertyName, L"flashMode"))
	{
		//TODO mode validation
		m_FlashMode = szPropertyValue;
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
}
BOOL CCamera::cmp(LPCTSTR tc1, LPCTSTR tc2)
{
	if (!tc1 || !tc2)
		return false;
	return !_tcsicmp(tc1, tc2);

}
