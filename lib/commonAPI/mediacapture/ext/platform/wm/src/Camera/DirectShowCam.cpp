#include "DirectShowCam.h"
//#include "DShowCam.h"

CDirectShowCam::CDirectShowCam(LPCTSTR szDeviceName):CCamera(szDeviceName)
{
	m_CamType = L"color";
}
CDirectShowCam::~CDirectShowCam()
{
}
BOOL CDirectShowCam::enumerate(rho::Vector<rho::String>& arIDs, rho::Hashtable<rho::String, eCamType>& camLookUp)
{
	bool bRetStatus= FALSE;
 //   CDShowCam dsCam;
	//// look for any DirectShow cameras and add them on the end
	//wstring wsCamId;
	//BOOL bCamFound = dsCam.FindFirstCam(&wsCamId);
	//if(bCamFound)
	//{
	//	bRetStatus = TRUE;
	//	while (bCamFound)
	//	{
	//		arIDs.addElement(convertToStringA(wsCamId));
	//		camLookUp.put(convertToStringA(szModifiedDeviceName), eColorCam );
	//		bCamFound = m_pDSCam->FindNextCam(&wsCamId);
	//	}
	//}	
	return bRetStatus;
	
}