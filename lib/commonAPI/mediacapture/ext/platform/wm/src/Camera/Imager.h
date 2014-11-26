#ifndef _IMAGER_H
#define _IMAGER_H
#include "ImgCAPI.h"
#include "logging/RhoLog.h"
#include "common/RhoStd.h"
#include "Camera.h"

 /* Function pointer for Image Capture APIs (Exposed by image capture library ImgAPI32.dll)
 */
typedef DWORD (*IMAGE_FINDFIRSTPROC)(LPIMAGE_FINDINFO, PHANDLE);
typedef DWORD (*IMAGE_FINDNEXTPROC)(LPIMAGE_FINDINFO, HANDLE);
typedef DWORD (*IMAGE_FINDCLOSEPROC)(HANDLE);
typedef DWORD (*IMAGE_OPENPROC)(PCTSTR, PHANDLE);
typedef DWORD (*IMAGE_CLOSEPROC)(HANDLE);
typedef DWORD (*IMAGE_STARTACQUISITIONPROC)(HANDLE);
typedef DWORD (*IMAGE_STOPACQUISITIONPROC)(HANDLE);
typedef DWORD (*IMAGE_STARTVIEWFINDERPROC)(HANDLE);
typedef DWORD (*IMAGE_STOPVIEWFINDERPROC)(HANDLE);
typedef DWORD (*IMAGE_GETIMAGEPROC)(HANDLE, LPDWORD, LPVOID);
typedef DWORD (*IMAGE_SETCAPCURRVALUEPROC) (HANDLE, DWORD, DWORD, LPVOID);
typedef DWORD (*IMAGE_UNLOCKIMAGERPROC)(HANDLE);


class CImager : public CCamera
{
private:
	static HINSTANCE m_hImagerLibHandle;
	HANDLE m_hImager;///< Handle to image capture device returned by Image_Open API, used to invoke other APIs
	static bool m_bImagerLoadFailed;
protected:
	BOOL m_AimMode;
public:
    CImager(LPCTSTR szDeviceName);
    ~CImager();	
	virtual BOOL getProperty(LPCTSTR szParameterName, WCHAR* szParameterValue);
    virtual BOOL setProperty(LPCTSTR szPropertyName, LPCTSTR szPropertyValue);
	virtual void getSupportedPropertyList(rho::Vector<rho::String>& arrayofNames);
	static BOOL enumerate(rho::Vector<rho::String>& arIDs, rho::Hashtable<rho::String, eCamType>& camLookUp);
	virtual void takeFullScreen();
    virtual BOOL showPreview();
    virtual BOOL hidePreview();
	virtual void Capture();
private:
	static IMAGE_FINDCLOSEPROC Image_FindClose;///<Function pointer to Image Capture API
	static IMAGE_FINDFIRSTPROC Image_FindFirst;///<Function pointer to Image Capture API
	static IMAGE_FINDNEXTPROC Image_FindNext;///<Function pointer to Image Capture API
	static IMAGE_OPENPROC Image_Open;///<Function pointer to Image Capture API
	static IMAGE_CLOSEPROC Image_Close;///<Function pointer to Image Capture API, closes Imae Capture Device 
	static IMAGE_STARTACQUISITIONPROC Image_StartAcquisition;///<Function pointer to Image Capture API, Starts Acquisition
	static IMAGE_STOPACQUISITIONPROC Image_StopAcquisition;///<Function pointer to Image Capture API, stops acquisition
	static IMAGE_SETCAPCURRVALUEPROC Image_SetCapCurrValue;///<Function pointer to Image Capture API, Sets a capability value
	static IMAGE_UNLOCKIMAGERPROC Image_UnLockImager;///<Function pointer to Image Capture API, unlocks imager device

	static IMAGE_GETIMAGEPROC Image_GetImage;///< Function pointer to Image Capture API, for Getting captured image
	static IMAGE_STARTVIEWFINDERPROC Image_StartViewfinder;///< Function pointer to Image Capture API,to start View Finder
	static IMAGE_STOPVIEWFINDERPROC Image_StopViewfinder;///< Function pointer to Image Capture API,to stop View Finder

	DWORD InitImager();
	DWORD StartViewer(RECT& pos, eViewrWndMode eMode);
	BOOL startPreview(RECT& pos, eViewrWndMode eMode);
	DWORD StopViewer();
	static DWORD fullScreenProc (LPVOID pparam);
protected:
	virtual void SetFlashMode();
	virtual void SetReolution();
	virtual void RedrawViewerWnd(RECT& pos);
};
#endif