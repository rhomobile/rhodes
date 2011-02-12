

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




DrawingImageImpl::DrawingImageImpl(void const *p, int size, bool useAlpha) {
	RHO_MAP_TRACE1("create DrawingImage with buffer length = %d", size);
	init(NULL, p, size, NULL, useAlpha);
}

DrawingImageImpl::DrawingImageImpl(const char* path, bool useAlpha) {
	RHO_MAP_TRACE1("create DrawingImage with filename = %s", path);
	init(path, NULL, 0, NULL, useAlpha);
}

DrawingImageImpl::DrawingImageImpl(WMBitmap* bitmap) {
	init(NULL, NULL, 0, bitmap, false);	
}


static int ourDrawingImageID = 0;

DrawingImageImpl::~DrawingImageImpl() {
	RHO_MAP_TRACE1("DrawingImage destroy with ID = %d", mID);
	if (mBitmap != NULL) {
		mBitmap->release();
		mBitmap = NULL;
	}
}

void DrawingImageImpl::init(const char* path, void const *p, int size, WMBitmap* bitmap, bool useAlpha) {
	mID = ++ourDrawingImageID;
	RHO_MAP_TRACE1("DrawingImage create with ID = %d", mID);

#if defined(_WIN32_WCE)
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
				mBitmap = new WMBitmap(mimage, useAlpha);
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
#endif //#if defined(_WIN32_WCE)
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

void DrawingContextImpl::drawText(int x, int y,  int nWidth, int nHeight, String const &text, int color) {
	RHO_MAP_TRACE2("DrawingContext drawText with x = %d, y = %d", x, y);
	StringW pathW = convertToStringW(text);
	SetBkMode(mHDC, TRANSPARENT);
	SetTextColor(mHDC, color);
	//TextOut(mHDC, x, y, pathW.c_str(), pathW.length());
	RECT r;
	r.left = x;
	r.top = y;
	r.right = x+nWidth;
	r.bottom = y + nHeight;
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

	SelectObject(mHDC, hOldBrush);
	DeleteObject(hBrush);
}

void DrawingContextImpl::getTextRect(int x, int y, String &text, RECT* resultRect) {
	//GetTextExtentPoint32
}

void DrawingContextImpl::drawLine(int x1, int y1, int x2, int y2, int color) {

}


WMBitmap::WMBitmap(IImage* img, bool useAlpha) {
	mReferenceCount = 1;

#if defined(_WIN32_WCE)
	ImageInfo imgInfo;
	img->GetImageInfo(&imgInfo);
	mWidth = imgInfo.Width;
	mHeight = imgInfo.Height;

	mAlphaBitmap = NULL;
	mBuf = NULL;

	HDC windowDC = ::GetDC(getMainWnd());
	BITMAP bmp;

	HDC mMemoryDC = CreateCompatibleDC(windowDC);

	mRowByteSize = mWidth*2;
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

	HGDIOBJ resObj = ::SelectObject(mMemoryDC, mMemoryBitmap);

	::GetObject( mMemoryBitmap, sizeof(BITMAP), &bmp );

	RECT r;
	r.left = 0;
	r.right = mWidth;
	r.top = 0;
	r.bottom = mHeight;

	if (useAlpha) {
		mAlphaBitmap = new WMAlphaBitmap(img);
	}
	else {
		img->Draw(mMemoryDC, &r, NULL);
	}

    ::SelectObject(mMemoryDC, resObj);
    DeleteDC(mMemoryDC);
#endif //#if defined(_WIN32_WCE)
}

WMBitmap::~WMBitmap() {
	if (mMemoryBitmap != NULL) {
		DeleteObject(mMemoryBitmap);
		mMemoryBitmap = NULL;
	}
/*	if (mMemoryDC != NULL) {
		DeleteDC(mMemoryDC);
		mMemoryDC = NULL;
	}*/
	if (mAlphaBitmap != NULL) {
		delete mAlphaBitmap;
		mAlphaBitmap = NULL;
	}
}

void WMBitmap::draw(HDC hdc, int x, int y) 
{
    HDC mMemoryDC = CreateCompatibleDC(hdc);
    HGDIOBJ resObj = ::SelectObject(mMemoryDC, mMemoryBitmap);

	if (mAlphaBitmap != NULL) {
		::BitBlt(	mMemoryDC,
					0, 0,
					mWidth, mHeight,
					hdc,
					x, y,
					SRCCOPY);
		mAlphaBitmap->draw(mBuf, mRowByteSize);
	}
	::BitBlt( hdc,
		x, y,
		mWidth, mHeight,
		mMemoryDC,
		0,0,
		SRCCOPY);

    ::SelectObject(mMemoryDC, resObj);
    DeleteDC(mMemoryDC);
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

WMAlphaBitmap::WMAlphaBitmap(IImage* img) 
{
#if defined(_WIN32_WCE)	
	IImagingFactory *pImgFactory = NULL;

	mWidth = 0;
	mHeight = 0;
	mImgBuf = NULL;

	HRESULT co_init_result = CoInitializeEx(NULL, 0/*COINIT_APARTMENTTHREADED*/);
	if ( (co_init_result == S_OK) || (co_init_result == S_FALSE)  ) {
		msg_out("CoInitializeEx OK");
		if (SUCCEEDED(CoCreateInstance (CLSID_ImagingFactory,
			NULL,
			CLSCTX_INPROC_SERVER,
			IID_IImagingFactory,
			(void **)&pImgFactory)))
		{
			ImageInfo imgInfo;
			img->GetImageInfo(&imgInfo);
			mWidth = imgInfo.Width;
			mHeight = imgInfo.Height;
			IBitmapImage* pBitmap = 0;

			if ( SUCCEEDED(pImgFactory->CreateBitmapFromImage(	img,
																mWidth,
																mHeight,
																PixelFormat32bppARGB,
																InterpolationHintDefault,
																&pBitmap) )) {
				msg_out("Create Bitmap OK");
				RECT rc = { 0, 0, mWidth, mHeight};
				BitmapData bitmap_data;
				if ( SUCCEEDED(pBitmap->LockBits(	&rc,
													ImageLockModeRead,
													PixelFormatDontCare,
													&bitmap_data))) {
					//
					msg_out("Lock Bits OK");
					void* src_buf = bitmap_data.Scan0;
					int stride = bitmap_data.Stride;
					int w = bitmap_data.Width;
					int h = bitmap_data.Height;
					mImgBuf = new unsigned int[w*h];
					if (mImgBuf != 0) {
						msg_out("Img buffer allocated OK");
						// start convert
						{
							unsigned int* dst = mImgBuf;
							unsigned int* src;
							int x;
							int y;
							for (y = 0 ; y < h; y++) {
								if (stride < 0) {
									src = (unsigned int*)(((unsigned char*)src_buf) + (h-1-y)*(-stride));
								}
								else {
									src = (unsigned int*)(((unsigned char*)src_buf) + y*(stride));
								}
								for (x = w; x > 0; x--) {
									*dst++ = *src++;
								}
							}
						}
						msg_out("Convert to img buffer finished OK");
						// finish convert
					}
					else {
						err_out("Image Buffer not allocated !");
					}
					pBitmap->UnlockBits(&bitmap_data);
				}
				else {
					err_out("Bitmap bits not locked !");
				}
				pBitmap->Release();
			}
		}
		CoUninitialize();
	}
	else {
		err_out("CoInitializeEx not initialized !");
	}
#endif //#if defined(_WIN32_WCE)
}

WMAlphaBitmap::~WMAlphaBitmap() {
	if (mImgBuf != NULL) {
		delete mImgBuf;
		mImgBuf = NULL;
	}
}

#define GET_R_32(dw)  (((dw) & 0x00FF0000) >> 16)
#define GET_G_32(dw)  (((dw) & 0x0000FF00) >> 8)
#define GET_B_32(dw)  ((dw) & 0x000000FF)
#define GET_A_32(dw)  (((dw) & 0xFF000000) >> 24)

#define GET_R_16(dw)  (((dw) & 0xF800) >> 8)
#define GET_G_16(dw)  (((dw) & 0x7E0) >> 3)
#define GET_B_16(dw)  (((dw) & 0x1F) << 3)

#define SET_R_16(dw,dwc)  dw = (unsigned short)(((dw) & (~0xF800)) | (((unsigned short)(dwc & 0xF8)) << 8))
#define SET_G_16(dw,dwc)  dw = (unsigned short)(((dw) & (~0x7E0)) | (((unsigned short)(dwc & 0xF8)) << 3))
#define SET_B_16(dw,dwc)  dw = (unsigned short)(((dw) & (~0x1F)) | (((unsigned short)(dwc & 0xF8)) >> 3))

// Normalize - stretch from 0..255 to 0..256
#define NORMALIZE_ALPHA(a)				a = (unsigned int)(a) + ((unsigned int)(a) >> 7)
#define DENORMALIZE_ALPHA(a)			a = (unsigned int)(a) - ((unsigned int)(a - 1) >> 7)

#define RENDER_R_16_32(dest, src, a)			SET_R_16( (dest), ( ((int)GET_R_16(dest) << 8) + ((int)GET_R_32(src) - (int)GET_R_16(dest))*(int)(a) ) >> 8 )
#define RENDER_G_16_32(dest, src, a)			SET_G_16( (dest), ( ((int)GET_G_16(dest) << 8) + ((int)GET_G_32(src) - (int)GET_G_16(dest))*(int)(a) ) >> 8 )
#define RENDER_B_16_32(dest, src, a)			SET_B_16( (dest), ( ((int)GET_B_16(dest) << 8) + ((int)GET_B_32(src) - (int)GET_B_16(dest))*(int)(a) ) >> 8 )
#define RENDER_A_16_32(dest, src, a)			SET_A_16( (dest), ( ((int)GET_A_16(dest) << 8) + ((int)GET_A_32(src) - (int)GET_A_16(dest))*(int)(a) ) >> 8 )

#define COLOR_32_TO_16(c)    ((unsigned short)(  ((c & 0xF80000) >> 8) | ((c & 0xF800) >> 5) | ((c & 0xF8) >> 3)))


void RENDER_DST16_SRC32(unsigned short* pdst, unsigned int* psrc)
{
	register unsigned int src= *psrc;
	register unsigned int srca = GET_A_32(src);
	if (srca == 255)
	{
		*pdst = COLOR_32_TO_16(src);
	}
	else
	{
		if (srca != 0)
		{
			register unsigned short dst = *pdst;
			NORMALIZE_ALPHA(srca);
			RENDER_R_16_32(dst, src, srca);
			RENDER_G_16_32(dst, src, srca);			
			RENDER_B_16_32(dst, src, srca);			
			*pdst = dst;
		}
	}
}

void WMAlphaBitmap::draw(unsigned short int *buf, int lineSizeInBytes) {
	unsigned short* dst;
	unsigned int* src = mImgBuf;

	int x;
	int y;

	for (y = 0; y < mHeight; y++) {
		src = mImgBuf + (mHeight-1-y)*mWidth;
		dst = (unsigned short*)( ((unsigned char*)buf) + (lineSizeInBytes*y) );
		
		for (x = 0; x < mWidth; x++) {
			RENDER_DST16_SRC32(dst++, src++);
		}
	}
}
