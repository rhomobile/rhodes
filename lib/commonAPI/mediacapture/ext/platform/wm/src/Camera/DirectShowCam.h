#ifndef _DIRECTSHOW_H
#define _DIRECTSHOW_H
#include "Camera.h"
#include "..\..\DirectShow\DShowCamModule\DShowCamModule\DShowCam.h"

class CDirectShowCam : public CCamera
{
public:
	CDirectShowCam(LPCTSTR szDeviceName);
	~CDirectShowCam();
	static BOOL enumerate(rho::Vector<rho::String>& arIDs, rho::Hashtable<rho::String, eCamType>& camLookUp);
	virtual void takeFullScreen();
	virtual BOOL showPreview();
	virtual BOOL hidePreview();
	virtual void Capture();
	BOOL getProperty(LPCTSTR szParameterName, WCHAR* szParameterValue);
	virtual void getSupportedSizeList(StringifyVector& supportedSizeList);
	virtual void getSupportedPropertyList(rho::Vector<rho::String>& arrayofNames);
protected:
	void SetFlashMode();
	void SetResolution();
	void setCameraProperties();
	virtual void RedrawViewerWnd(RECT& pos);
	void ResetResolution();
	std::vector<ImageRes> supportedResln;
private:
	//CDShowCam* m_pDSCam;
	void getCameraHWDetails();
	CameraSetting GetNearestResolution();

};
#endif