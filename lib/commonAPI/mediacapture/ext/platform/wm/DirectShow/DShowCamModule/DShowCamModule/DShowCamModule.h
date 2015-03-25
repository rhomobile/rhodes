// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the DSHOWCAM_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// DSHOWCAM_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef DSHOWCAM_EXPORTS
#define DSHOWCAM_API __declspec(dllexport)
#else
#define DSHOWCAM_API __declspec(dllimport)
#endif

#pragma once
#pragma warning (push, 3)
#include <string>
#include <dshow.h>
#include <Dmodshow.h>
#include <dmoreg.h>
#include <wmcodecids.h>
#include <cs.h>
#include <csmedia.h>
#include <iostream>
#include <atlbase.h>
#include "Imaging.h"
#include <initguid.h>
#include <imgguids.h>
#include "atlstr.h"
#include <regext.h>
#include "DShowCam.h"

#pragma warning (pop)
#pragma warning (push, 4)

using namespace std;

#define SAFE_RELEASE(x)		if (x) { x->Release(); x = NULL; }

const VideoProcAmpProperty IProp[] = {VideoProcAmp_Brightness,			//Index 0
									VideoProcAmp_Contrast,				//Index 1
									VideoProcAmp_Hue,					//Index 2
								    VideoProcAmp_Saturation,			//Index 3
									VideoProcAmp_Sharpness,				//Index 4
									VideoProcAmp_Gamma,					//Index 5
									VideoProcAmp_ColorEnable,			//Index 6
 									VideoProcAmp_WhiteBalance,			//Index 7
									VideoProcAmp_BacklightCompensation	//Index 8
									//VideoProcAmp_Gain					//Index 9
};

const CameraControlProperty CProp[] = {CameraControl_Pan,		//Index 0
									CameraControl_Tilt,			//Index 1
									CameraControl_Roll,			//Index 2
									CameraControl_Zoom,			//Index 3
									CameraControl_Exposure,		//Index 4
									CameraControl_Iris,			//Index 5
									CameraControl_Focus,		//Index 6
									CameraControl_Flash			//Index 7
};


typedef enum tagPropType
{
	IMG,
	CAM,
	CUS
}PropType;

typedef enum tagCameraCustomProperty
{
	CUS_PROPERTY_AIMING,
	CUS_PROPERTY_TEST
}CameraCustomProperty;





typedef struct tagCameraConfig 
{
	wstring sCamID;
	HWND hwndOwnerWnd;	
	RECT rc;
    BOOL bIsStillEnb;
	BOOL bIsPrvEnb;
	BOOL bIsCapEnb;
    BOOL bIsAudioEnb;
} CamConfig;



typedef struct tagImgFmt
{
	INT nMaxWidth;
	INT nMaxHeight;
	wstring wsColorFmt;
	LONGLONG qwMaxFrameRate;
}ImgFmt;


typedef struct tagImageProptable
{
	VideoProcAmpProperty vProp;
	LONG plVal;
	LONG plMin;
	LONG plMax;
	LONG plDelta;
	LONG plDef;
	LONG plFlag;
	HRESULT hr;
} ImgPropTbl;

typedef struct tagCamProptable
{
	CameraControlProperty p;
	LONG plVal;
	LONG plMin;
	LONG plMax;
	LONG plDelta;
	LONG plDef;
	LONG plFlag;
	HRESULT hr;
}CamPropTbl;

typedef struct tagCusProptable
{
	CameraCustomProperty p;
	LONG plVal;
	LONG plMin;
	LONG plMax;
	LONG plDelta;
	LONG plDef;
	LONG plFlag;
	HRESULT hr;
}CusPropTbl;

typedef struct tagMEDIA_SUBTYPE_LIST
{
	wstring wsClrName;
	const GUID *pMSubType;
}MEDIA_SUBTYPE_LIST;

typedef struct tagCamCapability
{
	INT nIndex;
	ImageRes tImgRes;
	wstring wsColorFmt;
	LONGLONG qwMaxFrameRate;
}CamCapability;

// This class is exported from the DShowCamModule.dll
typedef class CDShowCamModule
{
public:
	CDShowCamModule(void);
	~CDShowCamModule(void);

	//-----------------------------
	BOOL FindFirstCam(wstring* pwsCamID);
	//BOOL FindNextCam(wstring* pwsCamID);
	HRESULT InitFilterGrp();
	BOOL BuildFilterGrp(CamConfig* ptCamcfg);
	BOOL FreeFilterGrp();
	BOOL ReBuildGrp();
	BOOL RunGrp();
	BOOL ReleaseGrp();
	BOOL ResizePreview(int width, int height);
	BOOL StopGrp();
	BOOL PauseGrp();
	HRESULT Get_PropRng(PropType ePType, HANDLE hPropTbl);
	HRESULT Get_PropVal(PropType ePType, HANDLE hPropTbl);
	HRESULT Set_PropVal(PropType ePType, HANDLE hPropTbl);
	HRESULT CaptureStill(WCHAR *filename);
	HRESULT CaptureStill(wstring wsSFName);
	HRESULT Set_VdoFileName(wstring wsVFName);
	HRESULT ShowPreview();
	HRESULT HidePreview();
	HRESULT ClosePreview();
	HRESULT StartCapture();
	HRESULT StopCapture();
	HRESULT Set_Flip(BOOL bIsHori);	
	HRESULT EnumFirstCap(PinType ePType, ImgFmt* ptIFmt, INT* pnNoOfCap);
	HRESULT EnumOtherCap(PinType ePType, ImgFmt* ptIFmt, INT nNoOfCap);
	HRESULT Set_Resolution(ImageRes* ptRes, PinType ePType, BOOL bAutoControlGraph);
	HRESULT Set_ColorFmt(ImageRes* ptRes, wstring* pwsClrFmt, PinType ePType);
	HRESULT Set_FrameRate(LONGLONG qwFrmRate, PinType ePType);

	BOOL Get_LastError(LONG* plErrno);
	BOOL Cleanup();
	CamPropTbl m_tCamTblFlash;
	ImgFmt* m_tStillImgFmtArray;
	ImgPropTbl m_tImgTblBrt;
	int m_nStillResCnt;
	BOOL bAutoFlash;
	CameraSetting m_tImgRes;
	void EnablePowerKeyIAC(BOOL bEnablePowerKey);
	static void RegistryNotifyCallbackFuncIAC(HREGNOTIFY hNotify, DWORD dwUserData, const PBYTE pData, const UINT cbData);
	void EnablePowerKeyMPA(BOOL bEnablePowerKey);
	static void RegistryNotifyCallbackFuncMPA(HREGNOTIFY hNotify, DWORD dwUserData, const PBYTE pData, const UINT cbData);
	BOOL IsIACDEvice();
	//----------------------------

private:
	HRESULT SetupStill();
	HRESULT UndoSetupStill();
	HRESULT SetupPreview(HWND OwnerWnd, RECT rc);
	HRESULT UndoSetupPreview();
	HRESULT Cus_GetRange(CameraCustomProperty eProp, LONG* plMin, LONG* plMax, LONG* plDelta, LONG* plDef);
	HRESULT Cus_Get(CameraCustomProperty eProp, LONG* plVal);
	HRESULT Cus_Set(CameraCustomProperty eProp, LONG plVal);
	HRESULT SetupFlip();

	DWORD GunGet(DWORD* value);
	DWORD GunSet(DWORD value);
	HRESULT InitVdoCapFilter(ImageRes* ptRes, PinType ePType); // MPA3

private:
	//-------------
	//HANDLE	m_hCamHdl;
	LONG m_lErrno;
	CamConfig m_tCamcfg;
	IGraphBuilder* m_pGraphBuilder;
	ICaptureGraphBuilder2* m_pCaptureGraphBuilder;
	IMediaControl* m_pMediaControl;
	//IVideoWindow* m_pViewWindow;
	IMediaEventEx* m_pMediaEventEx;
	IMediaSeeking* m_pMediaSeeking;
	IBaseFilter* m_pVideoCaptureFilter;
	IBaseFilter* m_pAudioCaptureFilter;
	IAMVideoProcAmp* m_pImgCtrl;
	IAMCameraControl* m_pCamCtrl;
	IAMStreamConfig* m_pStrConf;
	IBaseFilter* m_pMux;
	IFileSinkFilter* m_pSink;
	IBaseFilter* m_pVideoEncoder;
	IDMOWrapperFilter* m_pVideoWrapperFilter;
	IBaseFilter* m_pStillSink;
	IAMVideoControl* m_pVideoCtrl;
	IPin* m_pPrvPin;
	wstring m_wsVFName;
	IBaseFilter* m_pVideoRenderer;
	IBaseFilter* m_pColorConverter; // MPA3
	INT m_nFileAutoCnt;
	BOOL m_bInited;
	//CamCapability* m_ptCamCap[3];
	//-------------
	

}VIDEOPROPERTY, *PVIDEOPROPERTY;

typedef struct _jpegMetadata
{
	LONG ulLatitude[6];
	LONG ulLongitude[6];
	CHAR szDateTime[20];
}JpegMetadata;





//extern DSHOWCAM_API int nDShowCamModule;
int Invoke(HWND hWnd, RECT rc, wstring Cam);
bool UpdateImageFileWithMetaData(LPCTSTR lpszPictureFilePath, JpegMetadata *jmMetadata);
bool ReadImageFileForMetaData(LPCTSTR lpszPictureFilePath, JpegMetadata *jmMetadata);
bool Degrees2DegreesMinutesSeconds(double coordinate, LONG *rationalCoordinate);
bool DegreesMinutesSeconds2Degrees(double *coordinate, LONG *rationalCoordinate);
IStream* CreateStreamByFileName(const CString& strFileName);
void WorkerThread();

extern "C"
{
	DSHOWCAM_API int InitDShow(int hWnd, wchar_t* CamId, RECT& rc);		
	DSHOWCAM_API int Run();
	DSHOWCAM_API int Pause();
	DSHOWCAM_API int SetBrightness(CameraSetting setting);	
	DSHOWCAM_API int GetMetaDataFromImage(WCHAR *filename, double *latitude, double *longitude, LPTSTR datetime);
	DSHOWCAM_API int SetMetaDataToImage(double latitude, double longitude, LPTSTR filename);
}