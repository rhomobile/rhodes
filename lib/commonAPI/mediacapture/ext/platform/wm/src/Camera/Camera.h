#ifndef _CAMERA_H
#define _CAMERA_H
#include "logging/RhoLog.h"
#include "common/RhoStd.h"
#include "api_generator/MethodResult.h"
#include "ViewFinder.h"
#include "commdlg.h"
#include "RcmLoader.h"

enum eCamType
{
	eColorCam,
	eImager
};
enum eTriggerEvents
{
	eCancel =0,
	eTrigger,
	eTriggerEventMax

};
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
	virtual void ApplicationFocusChange(bool bAppHasFocus)=0;
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
	static CRcmLoader m_Rcm;		///<  EMDK Rcm DLL loaded dynamically
	static HANDLE m_hTriggerEvents[eTriggerEventMax];
	static HANDLE m_hRegisterTrigger;
	static HANDLE m_hTriggerMonitorThread;
	static bool m_bRcmLoaded;
	static bool m_bAppHasFocus;	
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
	void DisableFullScreenButtons();//when trigger is used in full screen, then button should be greyed out
	virtual void SetCallback(rho::apiGenerator::CMethodResult& pCallback);
	virtual void ApplicationFocusChange(bool bAppHasFocus);
protected:
	/**
	* checks if two strings are equal
	*/
	BOOL cmp(LPCTSTR tc1, LPCTSTR tc2);
	void initializePreviewPos();	
	void UpdateCallbackStatus(rho::String status, rho::String message, rho::String imageUri, int nImageWidth=0, int ImageHeight=0);
	virtual void RedrawViewerWnd(RECT& pos);
	virtual void SetFlashMode()=0;
	virtual void SetReolution()=0;
	static void createTriggerMonitorThread(LPVOID pparam);
	static DWORD TriggerMonitorProc (LPVOID pparam);
private:
	static void closeTriggerEvents();
};

#endif