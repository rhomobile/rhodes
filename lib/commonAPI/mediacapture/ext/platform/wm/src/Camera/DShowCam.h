//-------------------------------------------------------------------------------------------------
// FILENAME: DShowCam.h
//
// Copyright(c) 2003 Symbol Technologies Inc. All rights reserved.
//
// DESCRIPTION:	This is a C++ class used for Video and Audio stream property adjustments, 
//				and for Camera related property settings via making calls to DirectShow Interfaces
//				and this class wrapes DirectShow.
/////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "logging/RhoLog.h"
#include "dshow.h"
#include "Dmodshow.h"
#include "dmoreg.h"
#include <atlbase.h>

using namespace std;




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
									//CameraControl_Flash			//Index 7
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

class CDShowCam
{
public:

	CDShowCam();
	~CDShowCam();

private:
	// Declare pointers to DirectShow interfaces
	CComPtr<ICaptureGraphBuilder2>  pCaptureGraphBuilder;
	CComPtr<IBaseFilter>            pVideoCap;
	CComPtr<IDMOWrapperFilter>      pVideoWrapperFilter;
	CComPtr<IPersistPropertyBag>    pPropertyBag;
	CComPtr<IGraphBuilder>          pGraph;
	CComPtr<IMediaControl>          pMediaControl;
	CComPtr<IMediaEvent>            pMediaEvent;
	CComPtr<IMediaSeeking>          pMediaSeeking;
	CComPtr<IVideoWindow>           pVideoWindow;
	CComPtr<IBaseFilter>            pStillSink;
	CComPtr<IBaseFilter>            pVideoRender;
	CComPtr<IBaseFilter>            pVideoEncoder;
	CComPtr<IBaseFilter>            m_pMux;
	CComPtr<IFileSinkFilter>        pFileSink;
	CComPtr<IAMStreamConfig>        m_pStrConf;


private:

	BOOL initCaptureDevice(wstring szDeviceName);
	BOOL SetupStill();


public:
	BOOL initFilterGraph();
	BOOL BuildFilterGraph(CamConfig& ptCamcfg);
	BOOL SetupPreview(HWND hViewerWnd, RECT& pos);
	HRESULT Get_PropRng(HANDLE hPropTbl);
	HRESULT Get_PropVal(HANDLE hPropTbl);
	HRESULT Set_PropVal(HANDLE hPropTbl);
	HRESULT Set_Resolution(ImageRes* ptRes, PinType ePType);
	BOOL StopGrp();
	BOOL RunGrp();
	BOOL ResizePreview(DWORD dwWidth, DWORD dwHeight);
	HRESULT CaptureStill(wstring wsSFName);
	
};

