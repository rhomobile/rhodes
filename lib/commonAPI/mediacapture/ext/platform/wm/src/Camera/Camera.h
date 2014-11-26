#ifndef _CAMERA_H
#define _CAMERA_H
#include "logging/RhoLog.h"
#include "common/RhoStd.h"
#include "api_generator/MethodResult.h"
#include "ViewFinder.h"

enum eCamType
{
	eColorCam,
	eImager
};
//typedef enum 
//{
//	eCancelEvent=0,
//	eCaptureEvent,
//	eMaxEventCount
//}eEventIndex;
typedef enum 
{
	eImageUri=0,
	eDataUri	
}eImageOutputFormat;
class ICam
{
public:	
	virtual BOOL getProperty(LPCTSTR szParameterName, WCHAR* szParameterValue)=0;
	virtual BOOL setProperty(LPCTSTR szPropertyName, LPCTSTR szPropertyValue)=0;
	virtual void getSupportedPropertyList(rho::Vector<rho::String>& arrayofNames) =0;
	virtual void takeFullScreen() = 0;
    virtual BOOL showPreview() = 0;
    virtual BOOL hidePreview() = 0;
	virtual void Capture() = 0;
	virtual void SetCallback(rho::apiGenerator::CMethodResult& pCallback)=0;
};

class CCamera : public ICam, public IViewFinderCallBack
{
protected:
	TCHAR m_szDeviceName[50];
	CViewFinder m_ViewFinder;
	int m_DesiredWidth;
	int m_DesiredHeight;
	int m_PreviewLeft;
	int m_PreviewTop;
	int m_PreviewWidth;
	int m_PreviewHeight;
	rho::StringW m_CaptureSound;
	rho::StringW m_FileName;
	BOOL m_FlashMode;
	eImageOutputFormat m_eOutputFormat;
	rho::StringW m_CamType;
	bool m_PreviewOn; 
	static bool m_IsCameraRunning;
	rho::apiGenerator::CMethodResult m_pCameraCb; //Status Event: Will give the status that the audio has been recorded succesfully or not  
public:
	CCamera(LPCTSTR szDeviceName);
	virtual ~CCamera();	
    virtual BOOL getProperty(LPCTSTR szParameterName, WCHAR* szParameterValue);
    virtual BOOL setProperty(LPCTSTR szPropertyName, LPCTSTR szPropertyValue);
	virtual void getSupportedPropertyList(rho::Vector<rho::String>& arrayofNames);
	virtual void cancel();
	virtual void captureImage();
	virtual void ResetViewerWndPos(RECT& pos);//viewerwnd callback
	void ResetViewerWndPos();//called when user update position manually
	virtual void SetCallback(rho::apiGenerator::CMethodResult& pCallback);
protected:
	/**
	* checks if two strings are equal
	*/
	BOOL cmp(LPCTSTR tc1, LPCTSTR tc2);
	void initializePreviewPos();
	void GetDataURI (BYTE* bData, int iLength, rho::String& data);	
	void UpdateCallbackStatus(rho::String status, rho::String message, rho::String imageUri);
	virtual void RedrawViewerWnd(RECT& pos);
	virtual void SetFlashMode()=0;
	virtual void SetReolution()=0;
};
#endif