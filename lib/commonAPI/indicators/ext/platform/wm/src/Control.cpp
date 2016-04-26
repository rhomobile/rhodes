//  DCC (28/03/2013): This code is lifted without significant modification from RhoElements 1

#include <windows.h>
#include "Control.h"


/******************************************************************************/

/******************************************************************************/
/******************************************************************************/
int CControl::nUsageCount = 0;
HMODULE CControl::hImageLibrary = NULL;
HMODULE CControl::hZlib = NULL;
HMODULE CControl::hImaging = NULL;
PLOADIMAGEPROC CControl::pLoadImageProc = NULL;
int CControl::nScreenWidth;
int CControl::nScreenHeight;
int CControl::nDefaultButtonSize;
int CControl::dwKeyState;
int CControl::dwCapsState;

/******************************************************************************/
CControl::CControl (HWND hparent)
{
//	LPWSTR pdll;

	psID = NULL;
	hwndTopLevel = hparent;
	hWindow = NULL;
	nLeft = 0;
	nTop = 0;
	nWidth = 0;
	nHeight = 0;

	// Get screen size
	nScreenWidth = GetSystemMetrics (SM_CXSCREEN);
	nScreenHeight = GetSystemMetrics (SM_CYSCREEN);
	nDefaultButtonSize = (nScreenHeight / 20) + 2;

	// Create static variables one-time
	//  No debug buttons in RE4 therefore no need to paint images on them.  We only
	//  have keystate which doesn't support having different graphics.
	/*
	if (nUsageCount++ == 0)
	{
		// Load pointer to image load function
		// Use Windows Mobile version if available
		// Load image wrapper if possible, will fail if aygshell not on system
		pdll = new WCHAR [wcslen (CControlsModule::PBCoreStruct.szInstallDirectory) + 64];
		wcscpy (pdll, CControlsModule::PBCoreStruct.szInstallDirectory);
		wcscat (pdll, L"Plugin\\WTG_SHWrapper.dll");
		hImageLibrary = LoadLibrary (pdll);
		if (hImageLibrary)
		{
			// Get pointer to wrapped function in DLL
			pLoadImageProc = (PLOADIMAGEPROC) GetProcAddress (hImageLibrary, L"LoadImageFromFile");
		}
		else
		{
			// Use BMP-only function, which is always available
			WRITELOG (L"Image library not available, only BMP supported");
			pLoadImageProc = SHLoadDIBitmap;
		}
		// Preload system libraries to prevent them being repeatedly loaded and unloaded
		hZlib = LoadLibrary(L"zlib.dll");
		hImaging = LoadLibrary(L"Imaging.dll");

		delete [] pdll;
	}
	*/
}

/******************************************************************************/
CControl::~CControl ()
{
	if (hWindow)
		DestroyWindow (hWindow);

	// Free static variables one-time
	if (--nUsageCount == 0)
	{
		FreeLibrary (hImageLibrary);
		hImageLibrary = NULL;
		FreeLibrary(hZlib);
		hZlib = NULL;
		FreeLibrary(hImaging);
		hImaging = NULL;
	}
}

/******************************************************************************/
void CControl::MoveControl ()
{
	if (hWindow)
		MoveWindow (hWindow, nLeft, nTop, nWidth, nHeight, FALSE);
}

/******************************************************************************/
BOOL CControl::Show ()
{
	if (hWindow)
		ShowWindow (hWindow, SW_SHOWNOACTIVATE);

	return TRUE;
}

/******************************************************************************/
BOOL CControl::Hide ()
{
	if (hWindow)
		ShowWindow (hWindow, SW_HIDE);

	return TRUE;
}

/******************************************************************************/
void CControl::ScaleImage (HBITMAP hbm_original, HDC hdc_scaled)
{
	HDC hdc_original;
	int width_original, height_original;
	int width_scaled, height_scaled;
	BITMAP bitmap;
	HBITMAP hbm_scaled, hbm_previous;
	RECT rect;

	if (!hbm_original)
		return;

	// Get client size of control
	GetClientRect (hWindow, &rect);
	width_scaled = rect.right;
	height_scaled = rect.bottom;

	// Get details of original image
	GetObject (hbm_original, sizeof bitmap, &bitmap);
	width_original = bitmap.bmWidth;
	height_original = bitmap.bmHeight;

	// Create new bitmap with same colour depth but size of control
	hbm_scaled = CreateBitmap (width_scaled, height_scaled, bitmap.bmPlanes, bitmap.bmBitsPixel, NULL);

	// Select into DC, deleting any existing bitmap
	hbm_previous = (HBITMAP) SelectObject (hdc_scaled, hbm_scaled);
	if (hbm_previous)
		DeleteObject (hbm_previous);

	// Create DC with original image bitmap
	hdc_original = CreateCompatibleDC (NULL);
	SelectObject (hdc_original, hbm_original);

	// Scale original image into new DC
	StretchBlt (hdc_scaled, 0, 0, width_scaled, height_scaled,
		hdc_original, 0, 0, width_original, height_original, SRCCOPY);

	// Clean up
	if (hdc_original)
		DeleteDC (hdc_original);
}

/******************************************************************************/
HBITMAP CControl::LoadImageFile (LPCWSTR filename)
{
	HBITMAP hbitmap;

	// Use dynamically loaded function, fall back to standard function on failure
	hbitmap = (*pLoadImageProc) (filename);
	if (!hbitmap)
		hbitmap = SHLoadDIBitmap (filename);

	return hbitmap;
}
/******************************************************************************/
BOOL CControl::StringToColour (LPCWSTR string, COLORREF *pcolour)
{
	// Colour string is in format #RRGGBB
	int red, green, blue;

	if (string [0] != L'#')
		return FALSE;

	if (wcslen (string) != 7)
		return FALSE;

	swscanf (string + 1, L"%2X%2X%2X", &red, &green, &blue);
	*pcolour = RGB (red, green, blue);

	return TRUE;
}

/******************************************************************************/
BOOL CControl::SetBorder (BOOL show)
{
	DWORD style;

	if (!hWindow)
		return TRUE;

	style = (DWORD) GetWindowLong (hWindow, GWL_STYLE);

	if (show)
		style |= WS_BORDER;
	else
		style &= (~WS_BORDER);

	SetWindowLong (hWindow, GWL_STYLE, (LONG) style);
	InvalidateRect (hWindow, NULL, FALSE);

	return TRUE;
}

/******************************************************************************/
BOOL CControl::SetID (LPCWSTR id)
{
	if (psID)
	{
		free (psID);
		psID = NULL;
	}

	if (id)
		psID = _wcsdup (id);

	return TRUE;
}
