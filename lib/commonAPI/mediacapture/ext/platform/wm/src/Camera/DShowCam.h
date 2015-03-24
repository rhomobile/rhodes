//-------------------------------------------------------------------------------------------------
// FILENAME: DShowCam.h
//
// Copyright(c) 2003 Symbol Technologies Inc. All rights reserved.
//
// DESCRIPTION:	This is a C++ class used for Video and Audio stream property adjustments, 
//				and for Camera related property settings via making calls to DirectShow Interfaces
//				and this class wrapes DirectShow.
//
// %IF Symbol_Internal
// AUTHOR: Wajra Attale
// CREATION DATE: 01/25/2007
// DERIVED FROM: New File
//
// EDIT HISTORY: No PVCS history for this file
// $Log:   T:/MPA2.0_sandbox/archives/SymbolValueAdd_mpa2/CCL/Camera/DShowCam/test/DShowTestApp/DShowCam.h-arc  $
//
//   Rev 1.0   Apr 18 2008 10:17:30   cheung
//Initial revision.
//
//   Rev 1.0   Dec 14 2007 07:20:58   sepalas
//Initial revision.
//
//%End
//--------------------------------------------------------------------------------------------------
//

#ifndef HEADERFILE_DSC_
#define HEADERFILE_DSC_

#pragma once
#pragma warning (push, 3)
#include "logging/RhoLog.h"
#include <string>
#include "dshow.h"
#include <Dmodshow.h>
#include <dmoreg.h>

#include "wmcodecids.h"
#include "cs.h"
#include "csmedia.h"
#include "evcode.h"
#include <iostream>
#include <atlbase.h>

//#include "../../common/public/PBPlugin.h"
#pragma warning (pop)
#pragma warning (push, 4)

using namespace std;

//define this to get debug logs out of camera plugin
#ifdef LOG_TO_FILE
#define DEBUGIT(__file, __str, ...) { \
	SYSTEMTIME __st;              \
    GetSystemTime(&__st);	      \
	fprintf(__file, "%d:%d:%d-%d:%d:%d.%d " ## __FUNCTION__ ## "():%d -- " ## __str ## "\n", \
			__st.wYear,__st.wMonth,__st.wDay,__st.wHour,__st.wMinute,          \
			__st.wSecond, __st.wMilliseconds, __LINE__, ##__VA_ARGS__);                         \
	fflush(__file); \
}
#else
#define DEBUGIT(__file, __str, ...)
#endif //LOG_TO_FILE


const VideoProcAmpProperty IProp[] = {VideoProcAmp_Brightness,			//Index 0
									VideoProcAmp_Contrast,				//Index 1
									VideoProcAmp_Hue,					//Index 2
								    VideoProcAmp_Saturation,			//Index 3
									VideoProcAmp_Sharpness,				//Index 4
									VideoProcAmp_Gamma,					//Index 5
									VideoProcAmp_ColorEnable,			//Index 6
 									VideoProcAmp_WhiteBalance,			//Index 7
									VideoProcAmp_BacklightCompensation,	//Index 8
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
typedef enum tagPinType
{
	V,
	S,
	P
}PinType;

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

typedef struct tagImageRes
{
	INT nWidth;
	INT nHeight;
}ImageRes;

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

typedef class CDShowCam
{
public:
	CDShowCam();
	~CDShowCam(void);

	//-----------------------------
	BOOL FindFirstCam(wstring* pwsCamID);
	BOOL FindNextCam(wstring* pwsCamID);
	HRESULT InitFilterGrp();
	BOOL BuildFilterGrp(CamConfig* ptCamcfg);
	BOOL Setup_Preview(CamConfig* ptCamcfg);
	BOOL ReBuildGrp();
	BOOL RunGrp();
	BOOL StopGrp();
	BOOL PauseGrp();
	BOOL ReleaseGrp();
	BOOL ResizePreview(DWORD dwWidth, DWORD dwHeight);
	HRESULT Get_PropRng(PropType ePType, HANDLE hPropTbl);
	HRESULT Get_PropVal(PropType ePType, HANDLE hPropTbl);
	HRESULT Set_PropVal(PropType ePType, HANDLE hPropTbl);
	HRESULT CaptureStill();
	HRESULT CaptureStill(wstring wsSFName);
	HRESULT Set_VdoFileName(wstring wsVFName);
	HRESULT StartCapture();
	HRESULT StopCapture();
	HRESULT Set_Flip(BOOL bIsHori);	
	HRESULT EnumFirstCap(PinType ePType, ImgFmt* ptIFmt, INT* pnNoOfCap);
	HRESULT EnumOtherCap(PinType ePType, ImgFmt* ptIFmt, INT nNoOfCap);
	HRESULT Set_Resolution(ImageRes* ptRes, PinType ePType);
	HRESULT Set_ColorFmt(ImageRes* ptRes, wstring* pwsClrFmt, PinType ePType);
	HRESULT Set_FrameRate(LONGLONG qwFrmRate, PinType ePType);
	HRESULT Set_CaptureSound(LPWSTR szSoundFile);
	BOOL HandlesCaptureSound();	
	void Get_Resolution(rho::Vector<ImageRes>& supportedRes, PinType ePType);
	BOOL initFilterGraph(wstring szDeviceName);

	BOOL Get_LastError(LONG* plErrno);
	BOOL Cleanup();
	//----------------------------

private:

	HRESULT SetupStill();
	HRESULT SetupPreview(HWND OwnerWnd, RECT rc);
	HRESULT Cus_GetRange(CameraCustomProperty eProp, LONG* plMin, LONG* plMax, LONG* plDelta, LONG* plDef);
	HRESULT Cus_Get(CameraCustomProperty eProp, LONG* plVal);
	HRESULT Cus_Set(CameraCustomProperty eProp, LONG plVal);
	HRESULT SetupFlip();

	DWORD GunGet(DWORD* value);
	DWORD GunSet(DWORD value);
	

private:
	//-------------
	LONG m_lErrno;
	CamConfig m_tCamcfg;
	IGraphBuilder* m_pGraphBuilder;
	ICaptureGraphBuilder2* m_pCaptureGraphBuilder;
	IMediaControl* m_pMediaControl;
	IVideoWindow* m_pViewWindow;
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
	INT m_nFileAutoCnt;
	//CamCapability* m_ptCamCap[3];
	boolean m_bCaptureSoundRegKeyExists;
	CRITICAL_SECTION m_DSCamCriticalSection;
	//-------------
	CComPtr<ICaptureGraphBuilder2>  pCaptureGraphBuilder;
	CComPtr<IGraphBuilder>          pGraph;
	CComPtr<IMediaControl>          pMediaControl;
	CComPtr<IMediaEvent>            pMediaEvent;
	CComPtr<IMediaSeeking>          pMediaSeeking;
	CComPtr<IBaseFilter>            pVideoCap;
	CComPtr<IPersistPropertyBag>    pPropertyBag;
	CComPtr<IAMStreamConfig>        pStrConf;
	
	
}VIDEOPROPERTY, *PVIDEOPROPERTY;
#endif //HEADERFILE_DSC_;
