

#include "stdafx.h"


#include <common/RhodesApp.h>
#include <logging/RhoLogConf.h>


#include "Graphics.h"


#if 0

#include <initguid.h>
#include <imgguids.h>

#else

#undef DEFINE_GUID
#define DEFINE_GUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
	EXTERN_C const GUID name \
	= { l, w1, w2, { b1, b2,  b3,  b4,  b5,  b6,  b7,  b8 } }

DEFINE_GUID(IID_IImagingFactory, 0x327abda7,0x072b,0x11d3,0x9d,0x7b,0x00,0x00,0xf8,0x1e,0xf3,0x2e);
DEFINE_GUID(CLSID_ImagingFactory, 0x327abda8,0x072b,0x11d3,0x9d,0x7b,0x00,0x00,0xf8,0x1e,0xf3,0x2e);

#endif




#define IP_PORTION_COUNT 32 

static void msg_out(const char* text) {
	//wchar_t msg[2048];
	//mbstowcs(msg, text, 2048);
	//RAWLOG_ERROR("MapView Graphics:");
	//RAWLOG_ERROR(text);
}

static void err_out(const char* text) {
	//wchar_t msg[2048];
	//mbstowcs(msg, text, 2048);
	RAWLOG_ERROR("MapView Graphics:");
	RAWLOG_ERROR(text);
}




DrawingImageImpl::DrawingImageImpl(void const *p, int size) {
	init(NULL, p, size, NULL);
}

DrawingImageImpl::DrawingImageImpl(const char* path) {
	init(path, NULL, 0, NULL);
}

DrawingImageImpl::DrawingImageImpl(IImage* img) {
	init(NULL, NULL, 0, img);	
}


DrawingImageImpl::~DrawingImageImpl() {
	if (mImage != NULL) {
		mImage->Release();
		mImage = NULL;
	}
}

void DrawingImageImpl::init(const char* path, void const *p, int size, IImage* img) {
	IImagingFactory *pImgFactory = NULL;
	IImage *pImage = NULL;

	mWidth = 0;
	mHeight = 0;
	mImage = NULL;

	if (img != NULL) {
		mImage = img;
		mImage->AddRef();
		ImageInfo imgInfo;
		mImage->GetImageInfo(&imgInfo);
		mWidth = imgInfo.Width;
		mHeight = imgInfo.Height;
		return;
	}

	HRESULT co_init_result = CoInitializeEx(NULL, 0/*COINIT_APARTMENTTHREADED*/);
	if ( (co_init_result == S_OK) || (co_init_result == S_FALSE)  ) {
		msg_out("CoInitializeEx OK");
		if (SUCCEEDED(CoCreateInstance (CLSID_ImagingFactory,
			NULL,
			CLSCTX_INPROC_SERVER,
			IID_IImagingFactory,
			(void **)&pImgFactory)))
		{
			HRESULT res = 0;
			if (p != NULL) {
				// from buf
				res = pImgFactory->CreateImageFromBuffer(
															p,
															size,
															BufferDisposalFlagNone,
															&pImage);
			}
			else {
				// from file
				msg_out("Create Image Factory OK");
				wchar_t wc_filename[2048];
				mbstowcs(wc_filename, path, 2048);
				res = pImgFactory->CreateImageFromFile(
														wc_filename,
														&pImage);
			}
			if (SUCCEEDED(res))
			{
				mImage = pImage;
				ImageInfo imgInfo;
				mImage->GetImageInfo(&imgInfo);
				mWidth = imgInfo.Width;
				mHeight = imgInfo.Height;
				//pImage->Release();
			}
			else {
				err_out("Image not created !");
			}
			pImgFactory->Release();
		}
		else {
			err_out("ImageFactory not created !");
		}
		CoUninitialize();
	}
	else {
		err_out("CoInitializeEx not initialized !");
	}
}

IDrawingImage* DrawingImageImpl::clone() {
	return new DrawingImageImpl(mImage);
}


void DrawingImageImpl::draw(HDC hdc, int x, int y) {
	if (mImage == NULL) {
		return;
	}
	RECT r;
	r.left = x;
	r.right = x+mWidth;
	r.top = y;
	r.bottom = y+mHeight;
	mImage->Draw(hdc, &r, NULL);
}



DrawingContextImpl::DrawingContextImpl(HDC hdc, int width, int height) {
	mHDC = hdc;
	mWidth = width;
	mHeight = height;
}

int DrawingContextImpl::getWidth() {
	return mWidth;
}

int DrawingContextImpl::getHeight() {
	return mHeight;
}

void DrawingContextImpl::drawImage(int x, int y, IDrawingImage* image) {
	DrawingImageImpl* img = (DrawingImageImpl*)image;
	img->draw(mHDC, x, y);
}

void DrawingContextImpl::drawText(int x, int y, String const &text, int color) {
	StringW pathW = convertToStringW(text);
	SetBkMode(mHDC, TRANSPARENT);
	SetTextColor(mHDC, color);
	//TextOut(mHDC, x, y, pathW.c_str(), pathW.length());
	RECT r;
	r.left = x;
	r.top = y;
	r.right = x+10000;
	r.bottom = y + 10000;
	DrawText(mHDC, pathW.c_str(), -1, &r, DT_LEFT | DT_TOP);
}

void DrawingContextImpl::fillRect(int x, int y, int width, int height, int color) {
	RECT r;
	r.left = x;
	r.top = y;
	r.right = x+width;
	r.bottom = y + height;

	HBRUSH hBrush;
	HBRUSH hOldBrush;

	hBrush = CreateSolidBrush(0xFF000000 | color);
	hOldBrush = (HBRUSH)SelectObject(mHDC, hBrush);

	FillRect(mHDC, &r, hBrush);
	
	DeleteObject(hBrush);
	SelectObject(mHDC, hOldBrush);
}

void DrawingContextImpl::getTextRect(int x, int y, String &text, RECT* resultRect) {
	//GetTextExtentPoint32
}

void DrawingContextImpl::drawLine(int x1, int y1, int x2, int y2, int color) {

}


