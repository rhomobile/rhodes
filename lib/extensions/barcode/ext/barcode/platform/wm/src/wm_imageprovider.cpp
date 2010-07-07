
//#include "rhodes/JNIRhodes.h"
//#include <rhodes.h>
//#include <android/log.h>
#include <common/RhodesApp.h>
#include <logging/RhoLogConf.h>


#include <stdlib.h>
#include <windows.h>
#include <commctrl.h>

#include <imaging.h>
#include <initguid.h>
#include <imgguids.h>



#define IP_PORTION_COUNT 32 

void msg_out(const char* text) {
	//wchar_t msg[2048];
	//mbstowcs(msg, text, 2048);
	//MessageBox(0, msg, L"ImageProvider", MB_OK);
	//RAWLOG_ERROR("rho_platform_image_load_grayscale");
	//RAWLOG_ERROR(text);
}

void err_out(const char* text) {
	//wchar_t msg[2048];
	//mbstowcs(msg, text, 2048);
	//MessageBox(0, msg, L"ImageProvider", MB_OK);
	RAWLOG_ERROR("rho_platform_image_load_grayscale");
	RAWLOG_ERROR(text);
}


extern "C" void rho_platform_image_load_grayscale(const char* url, void** image_pixels, int* pwidth, int* pheight) {
	*image_pixels = 0;
	*pwidth = 0;
	*pheight = 0;

	msg_out(url);

	IImagingFactory *pImgFactory = NULL;
	IImage *pImage = NULL;

	HRESULT co_init_result = CoInitializeEx(NULL, 0/*COINIT_APARTMENTTHREADED*/);
	if ( (co_init_result == S_OK) || (co_init_result == S_FALSE)  ) {
		msg_out("CoInitializeEx OK");
	if (SUCCEEDED(CoCreateInstance (CLSID_ImagingFactory,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_IImagingFactory,
		(void **)&pImgFactory)))
	{
		msg_out("Create Image Factory OK");
		wchar_t wc_filename[2048];
		mbstowcs(wc_filename, url, 2048);
		if (SUCCEEDED(pImgFactory->CreateImageFromFile(
			wc_filename,
			&pImage)))
		{
			msg_out("Create Image OK");
			ImageInfo img_info;
			if ( SUCCEEDED(pImage->GetImageInfo(&img_info))) {
				msg_out("Get Image Info OK");
				IBitmapImage* pBitmap = 0;
				if ( SUCCEEDED(pImgFactory->CreateBitmapFromImage(	pImage,
																	img_info.Width,
																	img_info.Height,
																	PixelFormat16bppRGB565,
																	InterpolationHintDefault,
																	&pBitmap) )) {
					msg_out("Create Bitmap OK");
					RECT rc = { 0, 0, img_info.Width, img_info.Height};
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
						unsigned char* gray_buf = new unsigned char[w*h];
						if (gray_buf != 0) {
							msg_out("Gray buffer allocated OK");
							*image_pixels = gray_buf;
							*pwidth = w;
							*pheight = h;
							// start convert
							{
								// make gray image
								unsigned char* dst = gray_buf;
								unsigned short* src;
								int x;
								int y;
								for (y = 0 ; y < h; y++) {
									int tk = (1<<16)/3;
									if (stride < 0) {
										src = (unsigned short*)(((unsigned char*)src_buf) + (h-1-y)*(-stride));
									}
									else {
										src = (unsigned short*)(((unsigned char*)src_buf) + y*(stride));
									}
									for (x = w; x > 0; x--) {
										unsigned short c = *src++;
										*dst++ = (unsigned char)(((((c & 0x1F)<<3) + ((c & 0x7E0)>>3) + ((c & 0xF800)>>8))*tk)>>16);
									}
								}
							}
							msg_out("Convert to grayscale finished OK");
							// finish convert
						}
						else {
							err_out("Gray Buffer not allocated !");
						}
						pBitmap->UnlockBits(&bitmap_data);
					}
					else {
						err_out("Bitmap bits not locked !");
					}
					pBitmap->Release();
				}
				else {
					err_out("Bitmap not created !");
				}
			}
			else {
				err_out("ImageInfo not received !");
			}
			pImage->Release();
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
	msg_out("Finish !");
}

extern "C" void rho_platform_image_free(void* image_pixels) {
  if (image_pixels != 0) {
    delete image_pixels;
  }
}


