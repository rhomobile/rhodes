#include "stdafx.h"

#if defined(_WIN32_WCE)&& !defined( OS_PLATFORM_CE )
#include <aygshell.h>
#endif

#include <atltime.h>
#if defined(_WIN32_WCE)&& !defined( OS_PLATFORM_CE )
#include <msinkaut_i.c>
#include <imaging.h>
#endif
#include "ext/rho/rhoruby.h"
#include "../MainWindow.h"
#include "Signature.h"
#include "common/RhodesApp.h"


#ifdef _MSC_VER
// warning C4800: 'int' : forcing to bool 'true' or 'false' (performance warning)
#pragma warning ( disable : 4800 )
#endif

extern "C" HWND getMainWnd();

//TODO: 
//      - review for memory leaks
//		- expose generic functions to utility class

static void prepare_filesystem(LPTSTR pszFilename, LPCWSTR szFormat, LPTSTR pszFullName);
static LPTSTR generate_filename(LPTSTR filename, LPCTSTR szExt);
static void create_folder(LPTSTR Path);
HRESULT Imaging_SaveToFile(HBITMAP source, LPTSTR filename, LPCTSTR format);

CRhoTakeSignatureDlg::CRhoTakeSignatureDlg() {
	m_pInkOverlay = NULL;
}

CRhoTakeSignatureDlg::~CRhoTakeSignatureDlg() {}

LRESULT CRhoTakeSignatureDlg::OnDestroyDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
#if defined(_WIN32_WCE)&& !defined( OS_PLATFORM_CE )
	if (m_pInkOverlay != NULL) {
		m_pInkOverlay->Release();
		m_pInkOverlay = NULL;
	}
#endif
	return FALSE;

}
LRESULT CRhoTakeSignatureDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
#if defined(_WIN32_WCE)&& !defined( OS_PLATFORM_CE )
	SetWindowText(_T("Take signature"));

	SHINITDLGINFO shidi = { SHIDIM_FLAGS, m_hWnd, SHIDIF_SIZEDLGFULLSCREEN };
	RHO_ASSERT(SHInitDialog(&shidi));

	SHMENUBARINFO mbi = { sizeof(mbi), 0 };
	mbi.hwndParent = m_hWnd;
	mbi.nToolBarId = IDR_GETURL_MENUBAR;
	mbi.hInstRes = _AtlBaseModule.GetResourceInstance();
	RHO_ASSERT(SHCreateMenuBar(&mbi));

	HRESULT hr = S_OK;

	HRESULT co_init_result = CoInitializeEx(NULL, 0/*COINIT_APARTMENTTHREADED*/);
	if ( (co_init_result == S_OK) || (co_init_result == S_FALSE)  ) {

		hr = ::CoCreateInstance(CLSID_InkOverlay,
								NULL,
								CLSCTX_INPROC_SERVER,
								IID_IInkOverlay,
								(void **)&m_pInkOverlay);

		//ASSERT(SUCCEEDED(hr));
		if (m_pInkOverlay != NULL) {
			// Attach the inkoverlay object to the window and enable it to start collecting ink
			m_pInkOverlay->put_hWnd((long)m_hWnd);
			//ASSERT(SUCCEEDED(hr));
			hr = m_pInkOverlay->put_Enabled(VARIANT_TRUE);
		}
		else {
			RAWLOG_ERROR("ERROR: Can not get Ink Overlay in Signature Capture !");
		}
	}
	else {
		RAWLOG_ERROR("ERROR: Can not Signature CoInitialize !");
	}
#endif

	return FALSE;
}

LRESULT CRhoTakeSignatureDlg::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND hwnd, BOOL& /*bHandled*/)
{
	EndDialog(wID);
	return 0;
}


LRESULT CRhoTakeSignatureDlg::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog(wID);
	return 0;
}


Signature::Signature(void) {
}

Signature::~Signature(void) {
}
HBITMAP Signature::getScreenHBITMAP() {
	// get screen rectangle 
	RECT windowRect; 
	GetWindowRect(getMainWnd(), &windowRect); 

	// bitmap dimensions 
	int bitmap_dx = windowRect.right - windowRect.left; 
	int bitmap_dy = windowRect.bottom - windowRect.top; 

	// create bitmap info header 
	BITMAPINFOHEADER infoHeader; 
	infoHeader.biSize          = sizeof(infoHeader); 
	infoHeader.biWidth         = bitmap_dx; 
	infoHeader.biHeight        = bitmap_dy; 
	infoHeader.biPlanes        = 1; 
	infoHeader.biBitCount      = 24;
	infoHeader.biCompression   = BI_RGB; 
	infoHeader.biSizeImage     = 0;
	infoHeader.biXPelsPerMeter = 0;
	infoHeader.biYPelsPerMeter = 0;
	infoHeader.biClrUsed       = 0;
	infoHeader.biClrImportant  = 0;

	// dibsection information 
	BITMAPINFO info; 
	info.bmiHeader = infoHeader; 
	HDC winDC = GetWindowDC(getMainWnd()); 
	HDC memDC = CreateCompatibleDC(winDC); 
	BYTE* memory = 0; 
	HBITMAP bitmap = CreateDIBSection(winDC, &info, DIB_RGB_COLORS, (void**)&memory, 0, 0); 
	HBITMAP old_selected = (HBITMAP)SelectObject(memDC, bitmap); 
	// Copies screen upside down (as it is already upside down) - if need normal layout, change to BitBlt function call
	StretchBlt(memDC, 0, 0, bitmap_dx, bitmap_dy, winDC, 0, bitmap_dy, bitmap_dx, bitmap_dy * -1, SRCCOPY); 
	SelectObject(memDC, old_selected);
	DeleteDC(memDC); 
	ReleaseDC(getMainWnd(), winDC); 

	return bitmap;
}
HRESULT Signature::takeSignature(HWND hwndOwner,LPTSTR pszFilename,LPCWSTR szFormat)
{
	int retVal = dlg.DoModal(getMainWnd());

	if (retVal != IDOK) {
		return S_FALSE;
	}

	HRESULT hResult = S_OK;
	
	TCHAR pszFullName[MAX_PATH];
	prepare_filesystem(pszFilename, szFormat, pszFullName);

	HBITMAP bitmap = Signature::getScreenHBITMAP();
	hResult = Imaging_SaveToFile(bitmap, pszFullName, szFormat);
	DeleteObject(bitmap);
	
	return hResult;
}
// TODO: move to some general utility class
void prepare_filesystem(LPTSTR pszFilename, LPCWSTR szFormat, LPTSTR pszFullName) {
	StringW strBlobRoot = convertToStringW(RHODESAPP().getBlobsDirPath());

	wsprintf(pszFilename, L"%s", strBlobRoot.c_str());

	TCHAR filename[MAX_PATH];
	generate_filename(filename, szFormat);

	int len = strBlobRoot.length() + wcslen(L"\\") + wcslen(filename);
	wchar_t* full_name = (wchar_t*) malloc((len+2)*sizeof(wchar_t));
	wsprintf(full_name,L"%s\\%s",strBlobRoot.c_str(),filename);

	create_folder(pszFilename);
	
	wcscpy(pszFullName, full_name);
	wcscpy(pszFilename, filename);

	free(full_name);
}
// TODO: move to some general utility class
// Same as in Camera.cpp
void create_folder(LPTSTR Path)
{
	TCHAR DirName[256];
	LPTSTR p = Path;
	LPTSTR q = DirName; 
	while(*p)
	{
		if (('\\' == *p) || ('/' == *p))
		{
			if (':' != *(p-1))
			{
				CreateDirectory(DirName, NULL);
			}
		}
		*q++ = *p++;
		*q = '\0';
	}
	CreateDirectory(DirName, NULL);
}
// TODO: move to some general utility class
// Same as in Camera.cpp, but extension (szExt) without dot
LPTSTR generate_filename(LPTSTR filename, LPCTSTR szExt) {
	RHO_ASSERT(filename);

	CTime time(CTime::GetCurrentTime());
	tm tl, tg;
	time.GetLocalTm(&tl);
	time.GetGmtTm(&tg);
	int tz = tl.tm_hour-tg.tm_hour; //TBD: fix tz

    wsprintf(filename, L"Image_%02i-%02i-%0004i_%02i.%02i.%02i_%c%03i.%s", 
		tg.tm_mon, tg.tm_mday, 1900+tg.tm_year, tg.tm_hour, tg.tm_min, tg.tm_sec,  
        tz>0?'_':'-',abs(tz),(szExt?szExt:L""));

	return filename;
}
// TODO: move to some general utility class
// Saves HBITMAP using encoder
HRESULT Imaging_SaveToFile(HBITMAP handle, LPTSTR filename, LPCTSTR format)
{
	HRESULT res = S_OK;
#if defined(_WIN32_WCE)&& !defined( OS_PLATFORM_CE )
	res = CoInitializeEx(NULL, 0/*COINIT_MULTITHREADED*/);
	if ((res == S_OK) || (res == S_FALSE)) {
		IImagingFactory* factory=NULL;
		if (CoCreateInstance(CLSID_ImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_IImagingFactory, (void**)&factory) == S_OK) {
			UINT count;
			ImageCodecInfo* imageCodecInfo=NULL;
			if (factory->GetInstalledEncoders(&count, &imageCodecInfo) == S_OK) {
				// Get the particular encoder to use
				LPTSTR formatString;
				if (wcscmp(format, L"png") == 0) {
					formatString = _T("image/png");
				} else if (wcscmp(format, L"jpg") == 0) {
					formatString = _T("image/jpeg");
				} else if (wcscmp(format, L"gif") == 0) {
					formatString = _T("image/gif");
				} else if (wcscmp(format, L"bmp") == 0) {
					formatString = _T("image/bmp");
				} else {
					factory->Release();
					CoUninitialize();
					return S_FALSE;
				}
				CLSID encoderClassId;
				if (count == 0) {
					factory->Release();
					CoUninitialize();
					return S_FALSE;
				}
   				for(int i=0; i < (int)count; i++) {
      				if (wcscmp(imageCodecInfo[i].MimeType, formatString) == 0) {
         				encoderClassId= imageCodecInfo[i].Clsid;
         				free(imageCodecInfo);
         				break;
					} else {
						continue;
					}
					factory->Release();
					CoUninitialize();
					return S_FALSE;
      			} 
				IImageEncoder* imageEncoder=NULL;
				if (factory->CreateImageEncoderToFile(&encoderClassId, filename, &imageEncoder) == S_OK) {
					IImageSink* imageSink = NULL;
					res = imageEncoder->GetEncodeSink(&imageSink);
					
					if (res != S_OK) {
						imageEncoder->TerminateEncoder();
						imageEncoder->Release();
						factory->Release();
						CoUninitialize();
						return res;
					}

					BITMAP bm;
					GetObject (handle, sizeof(BITMAP), &bm);
					PixelFormatID pixelFormat;
					switch (bm.bmBitsPixel) {
						case 1: {
							pixelFormat = PixelFormat1bppIndexed;
							break;
						}
						case 4: {
							pixelFormat = PixelFormat4bppIndexed;
							break;
						}
						case 8: {
							pixelFormat = PixelFormat8bppIndexed;
							break;
						}
						case 24: {
							pixelFormat = PixelFormat24bppRGB;
							break;
						}
						default: {
							pixelFormat = PixelFormat32bppARGB;
							break;
						}
					}
							
					BitmapData* bmData = new BitmapData();
					bmData->Height = bm.bmHeight;
					bmData->Width = bm.bmWidth;
					bmData->Scan0 = bm.bmBits;
					bmData->PixelFormat = pixelFormat;

					UINT bitsPerLine = bm.bmWidth * bm.bmBitsPixel;
					UINT bitAlignment = sizeof(LONG) * 8;
					UINT bitStride = bitAlignment * (bitsPerLine / bitAlignment);	// The image buffer is always padded to LONG boundaries
					if ((bitsPerLine % bitAlignment) != 0) bitStride += bitAlignment; // Add a bit more for the leftover values
					bmData->Stride = (bitStride / 8);

					IBitmapImage* pBitmap;
					factory->CreateBitmapFromBuffer(bmData, &pBitmap);
					IImage* pImage;
					pBitmap->QueryInterface(IID_IImage, (void**)&pImage); 
					res = pImage->PushIntoSink(imageSink);
					if (res != S_OK) {
						delete bmData;
						pBitmap->Release();
						pImage->Release();
						if (imageSink != NULL) {
							imageSink->Release();
						}
						imageEncoder->TerminateEncoder();
						imageEncoder->Release();
						factory->Release();
						CoUninitialize();
						return res;
					}
					
					delete bmData;
					pBitmap->Release();
					pImage->Release();
					imageSink->Release();
					imageEncoder->TerminateEncoder();
					imageEncoder->Release();
				}
			}
		}
		else {
			RAWLOG_ERROR("ERROR: Can not Signature Bitmap preparing get Image Factory !");
		}
		factory->Release();
		CoUninitialize();
	} else {
		RAWLOG_ERROR("ERROR: Can not Signature Bitmap preparing CoInitialize !");
		return res;
	}
#endif

	return res;
}
void rho_signature_take_signature(char* callback_url, char* image_format) {
	HWND main_wnd = getMainWnd();
	String callback = callback_url;
	String format = image_format ? image_format : "png"; // default PNG
	Signature::Params* params = new Signature::Params(callback, format);
	::PostMessage(main_wnd,WM_TAKESIGNATURE,0,(LPARAM)params);
}