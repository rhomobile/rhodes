#ifndef _DIRECTSHOW_H
#define _DIRECTSHOW_H
#include "Camera.h"
class CDirectShowCam : public CCamera
{
public:
	CDirectShowCam(LPCTSTR szDeviceName);
	~CDirectShowCam();
	static BOOL enumerate(rho::Vector<rho::String>& arIDs, rho::Hashtable<rho::String, eCamType>& camLookUp);
	//virtual void takePicture( const rho::Hashtable<rho::String, rho::String>& propertyMap, rho::apiGenerator::CMethodResult& oResult)m_hTimeOutProcCancelEvent;
    //virtual void showPreview(rho::apiGenerator::CMethodResult& oResult)m_hTimeOutProcCancelEvent;
    //virtual void hidePreview(rho::apiGenerator::CMethodResult& oResult)m_hTimeOutProcCancelEvent;
    //virtual void Capture( const rho::Hashtable<rho::String, rho::String>& propertyMap, rho::apiGenerator::CMethodResult& oResult)m_hTimeOutProcCancelEvent;


};
#endif