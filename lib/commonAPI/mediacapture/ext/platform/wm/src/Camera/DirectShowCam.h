#ifndef _DIRECTSHOW_H
#define _DIRECTSHOW_H
#include "Camera.h"
#include "..\..\DirectShow\DShowCamModule\DShowCamModule\DShowCam.h"

#define DSHOW_DLL L"DshowCamModule.dll"	///<  Name of the DSHOW Library
typedef int (WINAPI* LPFN_DSHOW_INIT)	(HWND hwnd, RECT rc);
typedef int (WINAPI* LPFN_DSHOW_CLOSE)	();
typedef int (WINAPI* LPFN_DSHOW_STOP)	();
typedef int (WINAPI* LPFN_DSHOW_SET_FLASH)	(FlashSetting setting);
typedef int (WINAPI* LPFN_DSHOW_SET_RESOLUTION)	(CameraSetting setting);
typedef int (WINAPI* LPFN_DSHOW_CAPTURE)	(const wchar_t* filename, bool isES400);
typedef int (WINAPI* LPFN_DSHOW_REDRAW_PREVIEW)	(int width, int height);
typedef int (WINAPI* LPFN_DSHOW_GET_RESOLUTION)	(std::vector<ImageRes>& supportedRes, wchar_t* camId, PinType ePType);

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
	bool InitDShow();
	HMODULE	m_hDshowDLL;													///<  HMODULE of the loaded DSHOW dll
	LPFN_DSHOW_INIT						lpfn_DSHOW_Init;					
	LPFN_DSHOW_CLOSE		lpfn_DSHOW_Close;	
	LPFN_DSHOW_STOP		lpfn_DSHOW_Stop;		
	LPFN_DSHOW_SET_FLASH		lpfn_DSHOW_SetFlash;	
	LPFN_DSHOW_SET_RESOLUTION		lpfn_DSHOW_SetResolution;		
	LPFN_DSHOW_CAPTURE		lpfn_DSHOW_Capture;	
	LPFN_DSHOW_REDRAW_PREVIEW		lpfn_DSHOW_RedrawPreview;		
	LPFN_DSHOW_GET_RESOLUTION		lpfn_DSHOW_GetResolution;
	bool isDshowDllPresent;

};
#endif