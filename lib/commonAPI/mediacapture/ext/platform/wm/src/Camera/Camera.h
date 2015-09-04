#ifndef _CAMERA_H
#define _CAMERA_H
#include "logging/RhoLog.h"
#include "common/RhoStd.h"
#include "api_generator/MethodResult.h"
#include "api_generator/StringifyHelper.h"
#include "ViewFinder.h"
#include "commdlg.h"
#include "TriggerMonitor.h"
using namespace rho::apiGenerator;

#define CAN_SUPPORT_DATA_URI(FILE_SIZE) (FILE_SIZE/1024) <= 250 //support upto 250 kb

enum eCamType
{
	eColorCam,
	eImager,
	eUnknownCamType
};
enum eTriggerEvents
{
	eCancel =0,
	eTrigger,
	eTriggerEventMax

};
typedef enum 
{
	eImagePath=0,
	eImageUri,
	eDataUri	
}eImageOutputFormat;
enum eImageFilePathErrorType
{
	eFileNotExist,
	eFileReadOnly,
	eFilePathValid,
	eUnknownFilePathErrorType
};
class ICam
{
public:	
	virtual BOOL getProperty(LPCTSTR szParameterName, WCHAR* szParameterValue)=0;
	virtual BOOL setProperty(LPCTSTR szPropertyName, LPCTSTR szPropertyValue)=0;
	virtual void getSupportedPropertyList(rho::Vector<rho::String>& arrayofNames) =0;
	virtual void takeFullScreen() = 0;
    virtual BOOL showPreview() = 0;
    virtual BOOL hidePreview() = 0;
	virtual void getSupportedSizeList(StringifyVector& supportedSizeList)=0;
	virtual void Capture() = 0;
	virtual void SetCallback(rho::apiGenerator::CMethodResult& pCallback)=0;
	virtual void ApplicationFocusChange(bool bAppHasFocus)=0;
	virtual void OnPowerButton(bool bPowerOn)=0;
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
	static rho::StringW m_ImageUriPath; //hold the imageUri path, needs to delete during every capture operation
	rho::StringW m_CamType;
	bool m_PreviewOn; 
	static bool m_IsCameraRunning;
	static CTriggerMonitor m_Rcm;		///<  EMDK Rcm DLL loaded dynamically
	static HANDLE m_hTriggerEvents[eTriggerEventMax];
	static HANDLE m_hRegisterTrigger;
	static HANDLE m_hTriggerMonitorThread;
	static bool m_bRcmLoaded;
	static bool m_bAppHasFocus;	
	rho::apiGenerator::CMethodResult m_pCameraCb; //Status Event: Will give the status 
	static bool m_bIsDeprecated;
public:
	CCamera(LPCTSTR szDeviceName);
	virtual ~CCamera();	
    virtual BOOL getProperty(LPCTSTR szParameterName, WCHAR* szParameterValue);
    virtual BOOL setProperty(LPCTSTR szPropertyName, LPCTSTR szPropertyValue);
	virtual void getSupportedPropertyList(rho::Vector<rho::String>& arrayofNames);
	virtual void cancel();
	virtual void close();
	virtual void captureImage();
	virtual void ResetViewerWndPos(RECT& pos);//viewerwnd callback
	void ResetViewerWndPos();//called when user update position manually
	void DisableFullScreenButtons();//when trigger is used in full screen, then button should be greyed out
	virtual void SetCallback(rho::apiGenerator::CMethodResult& pCallback);
	static void SetAPICallType(bool bIsDeprecated);
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
	virtual void SetResolution()=0;
	static void createTriggerMonitorThread(LPVOID pparam);
	static DWORD TriggerMonitorProc (LPVOID pparam);
	rho::StringW getFileName();
	eImageFilePathErrorType isImageFilePathValid();
private:
	static void closeTriggerEvents();
    rho::StringW generate_filename(LPCTSTR szExt) ;
	static void UnregisterTriggerMonitor();
};

#endif