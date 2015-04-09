#pragma once

#ifdef DSHOWCAM_EXPORTS
#define DSHOWCAM_API __declspec(dllexport)
#else
#define DSHOWCAM_API __declspec(dllimport)
#endif
#include <vector>
typedef enum tagFlashSetting
{
	On = 0,
	Off,
	Auto
}FlashSetting;
typedef enum tagPinType
{
	V,
	S,
	P
}PinType;
typedef struct tagImageRes
{
	INT nWidth;
	INT nHeight;
}ImageRes;	
typedef enum tagCameraSetting
{
	Low = 0,	
	Medium,	
	MidHigh,
	High
}CameraSetting;

extern "C"
{
	DSHOWCAM_API int InitDirectShow(HWND hwnd, RECT rc);
	DSHOWCAM_API void CloseDShow();
	DSHOWCAM_API int SetFlash(FlashSetting setting);
	DSHOWCAM_API int SetCameraResolution(CameraSetting setting);
	DSHOWCAM_API int SetTestResolution(int width, int height);
	DSHOWCAM_API int CaptureStill(const wchar_t* filename, bool isES400);
	DSHOWCAM_API BOOL ResizePreview(int width, int height);
	DSHOWCAM_API void GetResolution(std::vector<ImageRes>& supportedRes, wchar_t* camId, PinType ePType);
	DSHOWCAM_API int Stop();
	DSHOWCAM_API int Run();
	DSHOWCAM_API BOOL IsMPA3();
}