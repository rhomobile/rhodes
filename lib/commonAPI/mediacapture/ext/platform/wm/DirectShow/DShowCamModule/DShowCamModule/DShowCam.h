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

extern "C"
{
	DSHOWCAM_API int InitDirectShow(HWND hwnd, RECT rc);
	DSHOWCAM_API void CloseDShow();
	DSHOWCAM_API int SetFlash(FlashSetting setting);
	DSHOWCAM_API int SetCameraResolution(int height, int width);
	DSHOWCAM_API int CaptureStill(const wchar_t* filename, bool isES400);
	DSHOWCAM_API BOOL ResizePreview(int width, int height);
	DSHOWCAM_API void GetResolution(std::vector<ImageRes>& supportedRes, wchar_t* camId, PinType ePType);
}