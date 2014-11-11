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
typedef enum 
{
	eCancelEvent=0,
	eCaptureEvent,
	eMaxEventCount
}eEventIndex;
class ICam
{
public:	
	virtual BOOL getProperty(LPCTSTR szParameterName, WCHAR* szParameterValue)=0;
	virtual BOOL setProperty(LPCTSTR szPropertyName, LPCTSTR szPropertyValue)=0;
	virtual void getSupportedPropertyList(rho::Vector<rho::String>& arrayofNames) =0;
	virtual void takeFullScreen() = 0;
    virtual void showPreview() = 0;
    virtual void hidePreview() = 0;
	virtual void Capture() = 0;
	virtual void SetCallback(rho::apiGenerator::CMethodResult* pCallback);
};

class CCamera : public ICam, public IViewFinderCallBack
{
protected:
	CViewFinder m_ViewFinder;
	int m_DesiredWidth;
	int m_DesiredHeight;
	int m_PreviewLeft;
	int m_PreviewTop;
	int m_PreviewWidth;
	int m_PreviewHeight;
	rho::StringW m_CaptureSound;
	rho::StringW m_FileName;
	rho::StringW m_FlashMode;
	HANDLE m_hEvents[2];
	rho::apiGenerator::CMethodResult* m_pCameraCb; //Status Event: Will give the status that the audio has been recorded succesfully or not  
public:
	CCamera();
	virtual ~CCamera();	
    virtual BOOL getProperty(LPCTSTR szParameterName, WCHAR* szParameterValue);
    virtual BOOL setProperty(LPCTSTR szPropertyName, LPCTSTR szPropertyValue);
	virtual void getSupportedPropertyList(rho::Vector<rho::String>& arrayofNames);
	virtual void cancel();
	virtual void captureImage();
	virtual void SetCallback(rho::apiGenerator::CMethodResult* pCallback);
protected:
	/**
	* checks if two strings are equal
	*/
	BOOL cmp(LPCTSTR tc1, LPCTSTR tc2);
    


};
#endif