#include "stdafx.h"
#include <imaging.h>

#if defined(_WIN32_WCE)
#include <aygshell.h>
#endif
#include <atltime.h>
#include <msinkaut_i.c>
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
//		- fix Imaging_SaveToFile saving to JPG and GIF

static void prepare_filesystem(LPTSTR pszFilename, LPCWSTR szFormat, LPTSTR pszFullName);
static LPTSTR generate_filename(LPTSTR filename, LPCTSTR szExt);
static void create_folder(LPTSTR Path);
HRESULT Imaging_SaveToFile(HBITMAP source, LPTSTR filename, LPCTSTR format);

CRhoTakeSignatureDlg::CRhoTakeSignatureDlg() {}

CRhoTakeSignatureDlg::~CRhoTakeSignatureDlg() {}

LRESULT CRhoTakeSignatureDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	SetWindowText(_T("Take signature"));

	SHINITDLGINFO shidi = { SHIDIM_FLAGS, m_hWnd, SHIDIF_SIZEDLGFULLSCREEN };
	RHO_ASSERT(SHInitDialog(&shidi));

	SHMENUBARINFO mbi = { sizeof(mbi), 0 };
	mbi.hwndParent = m_hWnd;
	mbi.nToolBarId = IDR_GETURL_MENUBAR;
	mbi.hInstRes = _AtlBaseModule.GetResourceInstance();
	RHO_ASSERT(SHCreateMenuBar(&mbi));

	HRESULT hr = S_OK;
	hr = ::CoCreateInstance(CLSID_InkOverlay,
                            NULL,
                            CLSCTX_INPROC_SERVER,
                            IID_IInkOverlay,
                            (void **)&m_pInkOverlay);

	// Attach the inkoverlay object to the window and enable it to start collecting ink
    m_pInkOverlay->put_hWnd((long)m_hWnd);
    ASSERT(SUCCEEDED(hr));
    hr = m_pInkOverlay->put_Enabled(VARIANT_TRUE);
    ASSERT(SUCCEEDED(hr));

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
HRESULT Signature::saveInkOverlay(LPTSTR pszFilename, LPCTSTR szFormat) {
	TCHAR pszFullName[MAX_PATH];
	prepare_filesystem(pszFilename, szFormat, pszFullName);

	IInkDisp* pInkDisp = NULL;
	dlg.m_pInkOverlay->get_Ink(&pInkDisp);

	HANDLE hFileTemp;
	hFileTemp = CreateFile(pszFullName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	
	VARIANT Data;
	VariantInit( &Data );
	pInkDisp->Save(IPF_GIF, IPCM_Default, &Data);

	SAFEARRAY *pSA = V_ARRAY(&Data);
	unsigned char *pBytes = NULL;
	
	::SafeArrayAccessData(pSA, (void HUGEP **)&pBytes);
	
	DWORD dwWrittenSize = 0;
	WriteFile( hFileTemp, pBytes, pSA->rgsabound->cElements, &dwWrittenSize, NULL );

	::SafeArrayUnaccessData(pSA);

	CloseHandle(hFileTemp);

	return S_OK;
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
	SelectObject(memDC, bitmap); 
	// Copies screen upside down (as it is already upside down) - if need normal layout, change to BitBlt function call
	StretchBlt(memDC, 0, 0, bitmap_dx, bitmap_dy, winDC, 0, bitmap_dy, bitmap_dx, bitmap_dy * -1, SRCCOPY); 
	DeleteDC(memDC); 
	ReleaseDC(getMainWnd(), winDC); 

	return bitmap;
}
HRESULT Signature::saveScreen(LPTSTR pszFilename, LPCTSTR szFormat) {
	TCHAR pszFullName[MAX_PATH];
	prepare_filesystem(pszFilename, szFormat, pszFullName);

	HBITMAP bitmap = Signature::getScreenHBITMAP();
	HRESULT hResult = Imaging_SaveToFile(bitmap, pszFullName, szFormat);
	DeleteObject(bitmap);

	return hResult;
}
HRESULT Signature::takeSignature(HWND hwndOwner,LPTSTR pszFilename,LPCWSTR szFormat)
{
	int retVal = dlg.DoModal(getMainWnd());

	if (retVal != IDOK) {
		return S_FALSE;
	}

	HRESULT hResult = S_OK;

	if(wcscmp(szFormat, L"gif") == 0) { // save ink overlay as GIF from ink data
		hResult = Signature::saveInkOverlay(pszFilename, szFormat);
	}
	else { // save screenshot as BMP, PNG (if possible also JPG) using encoder
		hResult = Signature::saveScreen(pszFilename, szFormat); 
	}

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
// TODO: move to some general utility class or remove if not neccessary
// Saves screen as bitmap directly writing to file
void Screen_Bitmap_SaveToFile(HWND window, const char* filename){ 
	// get screen rectangle 
	RECT windowRect; 
	GetWindowRect(window, &windowRect); 
	// bitmap dimensions 
	int bitmap_dx = windowRect.right - windowRect.left; 
	int bitmap_dy = windowRect.bottom - windowRect.top; 
	// save bitmap file headers 
	BITMAPFILEHEADER fileHeader; 
	BITMAPINFOHEADER infoHeader; 
	fileHeader.bfType      = 0x4d42; 
	fileHeader.bfSize      = 0; 
	fileHeader.bfReserved1 = 0; 
	fileHeader.bfReserved2 = 0;
	fileHeader.bfOffBits   = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER); 

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
	
	// ------------------ // THE IMPORTANT CODE // ------------------ 
	// create a dibsection and blit the window contents to the bitmap 
	HDC winDC = GetWindowDC(window); 
	HDC memDC = CreateCompatibleDC(winDC); 
	BYTE* memory = 0; 
	HBITMAP bitmap = CreateDIBSection(winDC, &info, DIB_RGB_COLORS, (void**)&memory, 0, 0); 
	SelectObject(memDC, bitmap); 
	BitBlt(memDC, 0, 0, bitmap_dx, bitmap_dy, winDC, 0, 0, SRCCOPY); 
	DeleteDC(memDC); 
	ReleaseDC(window, winDC); 
	
	// save dibsection data 
	int bytes = (((24*bitmap_dx + 31) & (~31))/8)*bitmap_dy; 

	HANDLE hFileTemp;
	hFileTemp = CreateFile((LPCWSTR)filename, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	DWORD dwWrittenSize = 0;
	WriteFile( hFileTemp, &fileHeader, sizeof (BITMAPFILEHEADER), &dwWrittenSize, NULL );
	WriteFile( hFileTemp, &infoHeader, sizeof (BITMAPINFOHEADER), &dwWrittenSize, NULL );
	WriteFile( hFileTemp, memory, bytes, &dwWrittenSize, NULL );

	CloseHandle(hFileTemp);
	DeleteObject(bitmap);
}
// TODO: saves PNG and BMP, but JPG and GIF fails - should be revised
// TODO: move to some general utility class
// Saves HBITMAP using encoder
HRESULT Imaging_SaveToFile(HBITMAP handle, LPTSTR filename, LPCTSTR format){
	HRESULT res;

	res = CoInitializeEx(NULL, COINIT_MULTITHREADED);
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
					CoUninitialize();
					return S_FALSE;
				}
				CLSID encoderClassId;
				if (count == 0) {
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
					CoUninitialize();
					return S_FALSE;
      			} 
				IImageEncoder* imageEncoder=NULL;
				if (factory->CreateImageEncoderToFile(&encoderClassId, filename, &imageEncoder) == S_OK) {
					IImageSink* imageSink = NULL;
					res = imageEncoder->GetEncodeSink(&imageSink);
					if (res != S_OK) {
						CoUninitialize();
						return res;
					}
					BITMAP bm;
					GetObject (handle, sizeof(BITMAP), &bm);

					ImageInfo* imageInfo = new ImageInfo();
					imageInfo->Width = bm.bmWidth;
					imageInfo->Height = bm.bmHeight;
					imageInfo->RawDataFormat = IMGFMT_MEMORYBMP; //ImageFormatMemoryBMP;
					imageInfo->Flags |= SinkFlagsTopDown | SinkFlagsFullWidth;
					// Get pixel format from hBitmap
					PixelFormatID pixelFormat;
					int numColors = 0;
					switch (bm.bmBitsPixel) {
						case 1: {
							pixelFormat = PixelFormat1bppIndexed;
							numColors = 1;
							break;
						}
						case 4: {
							pixelFormat = PixelFormat4bppIndexed;
							numColors = 16;
							break;
						}
						case 8: {
							pixelFormat = PixelFormat8bppIndexed;
							numColors = 256;
							break;
						}
						case 24: {
							pixelFormat = PixelFormat24bppRGB;
							break;
						}
						default: {
							pixelFormat = PixelFormat32bppARGB;
							numColors = 3; // according to MSDN 16 and 32 bpp numColors should be 3
							break;
						}
					}
					imageInfo->PixelFormat = pixelFormat;
					if (pixelFormat == PixelFormat32bppARGB) imageInfo->Flags |= SinkFlagsHasAlpha;
					res = imageSink->BeginSink(imageInfo, NULL);
					if (res != S_OK) {
						CoUninitialize();
						return res;
					}
					ColorPalette* palette = NULL;
					if (numColors > 0) {
						palette = (ColorPalette*)malloc(sizeof(ColorPalette) + (numColors - 1) * sizeof(ARGB));
						palette->Count = numColors;
						for (int i=0; i<numColors; i++) {
							int rgb = i*64;
							int red = rgb & 0x00FF;
							int green = (rgb >> 8) & 0x00FF;
							int blue = (rgb >> 16) & 0x00FF;
							palette->Entries[i] = MAKEARGB(0, red, green, blue);
						}
					} else {
						palette = (ColorPalette*)malloc(sizeof(ColorPalette));
						palette->Count = 0;
						if (pixelFormat == PixelFormat32bppARGB) palette->Flags = PALFLAG_HASALPHA;
					}
					res = imageSink->SetPalette(palette);
					if (res != S_OK) {
						CoUninitialize();
						return res;
					}
					
					BitmapData* bmData = new BitmapData();
					bmData->Height = bm.bmHeight;
					bmData->Width = bm.bmWidth;
					bmData->Scan0 = bm.bmBits;
					bmData->PixelFormat = pixelFormat;

					UINT bitsPerLine = imageInfo->Width * bm.bmBitsPixel;
					UINT bitAlignment = sizeof(LONG) * 8;
					UINT bitStride = bitAlignment * (bitsPerLine / bitAlignment);	// The image buffer is always padded to LONG boundaries
					if ((bitsPerLine % bitAlignment) != 0) bitStride += bitAlignment; // Add a bit more for the leftover values
					bmData->Stride = (bitStride / 8);

					RECT rect;
					rect.top = 0;
					rect.bottom = bm.bmHeight;
					rect.left = 0;
					rect.right = bm.bmWidth;

					res = imageSink->PushPixelData(&rect, bmData, TRUE);
					if (res != S_OK) {
						CoUninitialize();
						return res;
					}

					res = imageSink->EndSink(S_OK);
					if (res != S_OK) {
						CoUninitialize();
						return res;
					}
					imageSink->Release();
					res = imageEncoder->TerminateEncoder();
					if (res != S_OK) {
						CoUninitialize();
						return res;
					}
				}
			}
		}
		CoUninitialize();
	} else {
		return res;
	}

	return res;
}
void rho_signature_take_signature(char* callback_url, char* image_format) {
	HWND main_wnd = getMainWnd();
	String callback = callback_url;
	String format = image_format ? image_format : "png"; // default PNG
	Signature::Params* params = new Signature::Params(callback, format);
	::PostMessage(main_wnd,WM_TAKESIGNATURE,0,(LPARAM)params);
}