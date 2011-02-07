

#include "stdafx.h"


#include <common/RhodesApp.h>
#include <logging/RhoLogConf.h>


#include "Graphics.h"


#if 0

#include <initguid.h>
#include <imgguids.h>

#else


extern "C" HWND getMainWnd();

#undef DEFINE_GUID
#define DEFINE_GUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
	EXTERN_C const GUID name \
	= { l, w1, w2, { b1, b2,  b3,  b4,  b5,  b6,  b7,  b8 } }

DEFINE_GUID(IID_IImagingFactory, 0x327abda7,0x072b,0x11d3,0x9d,0x7b,0x00,0x00,0xf8,0x1e,0xf3,0x2e);
DEFINE_GUID(CLSID_ImagingFactory, 0x327abda8,0x072b,0x11d3,0x9d,0x7b,0x00,0x00,0xf8,0x1e,0xf3,0x2e);

#endif

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "WM MapView Graphics"



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
	RHO_MAP_TRACE1("create DrawingImage with buffer length = %d", size);
	init(NULL, p, size, NULL);
}

DrawingImageImpl::DrawingImageImpl(const char* path) {
	RHO_MAP_TRACE1("create DrawingImage with filename = %s", path);
	init(path, NULL, 0, NULL);
}

DrawingImageImpl::DrawingImageImpl(WMBitmap* bitmap) {
	init(NULL, NULL, 0, bitmap);	
}


static int ourDrawingImageID = 0;

DrawingImageImpl::~DrawingImageImpl() {
	RHO_MAP_TRACE1("DrawingImage destroy with ID = %d", mID);
	if (mBitmap != NULL) {
		mBitmap->release();
		mBitmap = NULL;
	}
}

void DrawingImageImpl::init(const char* path, void const *p, int size, WMBitmap* bitmap) {
	mID = ++ourDrawingImageID;
	RHO_MAP_TRACE1("DrawingImage create with ID = %d", mID);

	IImagingFactory *pImgFactory = NULL;
	IImage *pImage = NULL;

	mWidth = 0;
	mHeight = 0;
	mBitmap = NULL;

	if (bitmap != NULL) {
		mBitmap = bitmap;
		mBitmap->addRef();
		mWidth = bitmap->width();
		mHeight = bitmap->height();
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
				IImage* mimage = pImage;
				ImageInfo imgInfo;
				mimage->GetImageInfo(&imgInfo);
				mWidth = imgInfo.Width;
				mHeight = imgInfo.Height;
				RHO_MAP_TRACE2("Drawing Image was created with WIDTH = %d, HEIGHT = %d", mWidth, mHeight);
				mBitmap = new WMBitmap(mimage);
				mimage->Release();
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
	RHO_MAP_TRACE1("clone DrawingImage from ID = %d", mID);
	return new DrawingImageImpl(mBitmap);
}


void DrawingImageImpl::draw(HDC hdc, int x, int y) {
	RHO_MAP_TRACE2("draw DrawingImage with x = %d, y = %d", x, y);
	if (mBitmap == NULL) {
		return;
	}
	mBitmap->draw(hdc, x, y);
}



DrawingContextImpl::DrawingContextImpl(HDC hdc, int width, int height) {
	RHO_MAP_TRACE2("DrawingContext create with WIDTH = %d, HEIGHT = %d", width, height);
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
	RHO_MAP_TRACE2("DrawingContext drawText with x = %d, y = %d", x, y);
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


WMBitmap::WMBitmap(IImage* img/*unsigned short* buf, int width, int height, int row_byte_size*/) {
	mReferenceCount = 1;

	ImageInfo imgInfo;
	img->GetImageInfo(&imgInfo);
	mWidth = imgInfo.Width;
	mHeight = imgInfo.Height;


	HDC windowDC = ::GetDC(getMainWnd());
	HGDIOBJ resObj;
	BITMAP bmp;

	mMemoryDC = CreateCompatibleDC(windowDC);

	int mRowByteSize = mWidth*2;
	if (((mWidth*2) & 0x3) != 0) {
		mRowByteSize = ((mWidth*2) & (~0x3)) + 0x4;
	}

	char	buf[sizeof(BITMAPINFOHEADER) + 100];
	BITMAPINFO* bmi = (BITMAPINFO*)buf;

	bmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi->bmiHeader.biWidth = mWidth;
	bmi->bmiHeader.biHeight = mHeight;
	bmi->bmiHeader.biPlanes = 1;
	bmi->bmiHeader.biBitCount = 16;
	bmi->bmiHeader.biCompression = BI_BITFIELDS;
	bmi->bmiHeader.biSizeImage = mRowByteSize  * mHeight;
	bmi->bmiHeader.biXPelsPerMeter = 0;
	bmi->bmiHeader.biYPelsPerMeter = 0;
	bmi->bmiHeader.biClrUsed = 0;
	bmi->bmiHeader.biClrImportant = 0;

	*((DWORD*)&(bmi->bmiColors[0])) = 0x1F << 11;
	*((DWORD*)&(bmi->bmiColors[1])) = 0x3F << 5;
	*((DWORD*)&(bmi->bmiColors[2])) = 0x1F;

	mMemoryBitmap = CreateDIBSection(	mMemoryDC,
		bmi,
		DIB_RGB_COLORS,
		(void**)&mBuf,
		NULL,
		0);

	resObj = ::SelectObject(mMemoryDC, mMemoryBitmap);

	::GetObject( mMemoryBitmap, sizeof(BITMAP), &bmp );

	RECT r;
	r.left = 0;
	r.right = mWidth;
	r.top = 0;
	r.bottom = mHeight;
	img->Draw(mMemoryDC, &r, NULL);
}

WMBitmap::~WMBitmap() {
	if (mMemoryBitmap != NULL) {
		DeleteObject(mMemoryBitmap);
		mMemoryBitmap = NULL;
	}
	if (mMemoryDC != NULL) {
		DeleteDC(mMemoryDC);
		mMemoryDC = NULL;
	}
}

void WMBitmap::draw(HDC hdc, int x, int y) {
	::BitBlt(	hdc,
		x, y,
		mWidth, mHeight,
		mMemoryDC,
		0,0,
		SRCCOPY);
}

void WMBitmap::addRef() {
	mReferenceCount++;
}

void WMBitmap::release() {
	mReferenceCount--;
	if (mReferenceCount <= 0) {
		delete this;
	}
}
