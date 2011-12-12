/*------------------------------------------------------------------------
* (The MIT License)
* 
* Copyright (c) 2008-2011 Rhomobile, Inc.
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
* 
* http://rhomobile.com
*------------------------------------------------------------------------*/

#include "stdafx.h"

#include <common/RhodesApp.h>
#include <logging/RhoLogConf.h>

#include "Bitmap.h"

//#ifdef OS_WINCE
//#include <initguid.h>
//#include <imgguids.h>
//#endif //OS_WINCE

extern "C" HWND getMainWnd();

static void msg_out(const char* text) 
{
    RAWLOG_ERROR("MapView Graphics:");
    RAWLOG_ERROR(text);
}

static void err_out(const char* text) 
{
    RAWLOG_ERROR("MapView Graphics:");
    RAWLOG_ERROR(text);
}


WMBitmap::WMBitmap(IImage* img, bool useAlpha) 
{
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

            pImgFactory->Release();
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
