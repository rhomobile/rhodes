#include "DirectShowCam.h"
#include "DShowCam.h"

BOOL CDirectShowCam::enumerate(rho::Vector<rho::String>& arIDs, rho::Hashtable<String, eCamType>& camLookUp)
{
	bool bRetStatus= FALSE;
    CDShowCam dsCam;
	// look for any DirectShow cameras and add them on the end
	wstring wsCamId;
	BOOL bCamFound = dsCam.FindFirstCam(&wsCamId);
	if(bCamFound)
	{
		bRetStatus = TRUE;
		while (bCamFound)
		{
			arIDs.addElement(convertToStringA(wsCamId));
			camLookUp.put(convertToStringA(szModifiedDeviceName), eColorCam );
			bCamFound = m_pDSCam->FindNextCam(&wsCamId);
		}
	}	
	return bRetStatus;
	
}