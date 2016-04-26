/******************************************************************************/
#include <windows.h>
#include "ControlsModule.h"
#include "CommandArea.h"

/******************************************************************************/
extern CControlsModule *pControlsModule;

/******************************************************************************/
#define COMMANDAREA_CLASS_NAME		L"PB_COMMANDAREA"

/******************************************************************************/
BOOL CCommandArea::bRegistered = FALSE;
int CCommandArea::nTopHeight = 0;
int CCommandArea::nBottomHeight = 0;
BOOL CCommandArea::bTopVisible = FALSE;
BOOL CCommandArea::bBottomVisible = FALSE;

/******************************************************************************/
CCommandArea::CCommandArea (HINSTANCE hinstance, HWND hparent, int instance_id, BOOL top) :
	CControl (instance_id, hparent)
{
	// Register class if needed
	if (!bRegistered)
	{
		WNDCLASS wndclass;
		memset (&wndclass, 0, sizeof wndclass);
		wndclass.lpfnWndProc = StaticWindowProc;
		wndclass.hInstance = hinstance;
		wndclass.lpszClassName = COMMANDAREA_CLASS_NAME;
		RegisterClass (&wndclass);

		bRegistered = TRUE;
	}

	nWidth = CControlsModule::nScreenWidth;
	nHeight = CControlsModule::nDefaultButtonSize + 3;

	// Set position depending on whether the 'top' flag is set
	if (top)
	{
		nLeft = 0;
		nTop = 0;
	}
	else
	{
		nLeft = 0;
		nTop = CControlsModule::nScreenHeight - nHeight;
	}

	// Create the window
	hWindow = CreateWindowEx (WS_EX_NOACTIVATE, COMMANDAREA_CLASS_NAME, NULL, WS_POPUP, 
		nLeft, nTop, nWidth, nHeight, hparent, (HMENU) -1, hinstance, NULL);

	// Add a pointer to ourselves to the window
	SetWindowLong (hWindow, GWL_USERDATA, (LONG) this);

	// Create DCs to hold scaled images
	hdcImage = CreateCompatibleDC (NULL);

	// Border by default
	bBorder = TRUE;

	// Use brush by default
	hBrush = CreateSolidBrush (RGB (192, 192, 192));
	hbmOriginalImage = 0;

	imoImage = 0;
	imoImageFilename = NULL;
}

/******************************************************************************/
CCommandArea::~CCommandArea ()
{
	HBITMAP hbitmap;

	hbitmap = (HBITMAP) GetCurrentObject (hdcImage, OBJ_BITMAP);
	if (hbitmap)
		DeleteObject (hbitmap);

	DeleteDC (hdcImage);

	if (hbmOriginalImage)
		DeleteObject (hbmOriginalImage);

	if (hBrush)
		DeleteObject (hBrush);

	if (imoImage)
		pControlsModule->DeleteIMO (imoImage);
}
	
/******************************************************************************/
void CCommandArea::ResizeHTML ()
{
	int top, bottom;

	// Resize to full screen by default, reduce as necessary if top and bottom areas are visible
	top = (bTopVisible ? nTopHeight : 0);
	bottom = (bBottomVisible ? CControlsModule::nScreenHeight - 1 - nBottomHeight : CControlsModule::nScreenHeight - 1);

	CControlsModule::PBCoreStruct.pBrowserResize (0, MODULE_NAME, 0, top,
		CControlsModule::nScreenWidth, bottom - top + 1);
}

/******************************************************************************/
BOOL CCommandArea::SetImageFromFile (LPCWSTR filename)
{
	HBITMAP hbitmap;

	if (!filename)
		return FALSE;

	if ((hbitmap = LoadImageFile (filename)) == NULL)
		return FALSE;

	// Free any existing bitmap
	if (hbmOriginalImage)
		DeleteObject (hbmOriginalImage);

	// Store new one
	hbmOriginalImage = hbitmap;

	// Scale into DC
	ScaleImage (hbmOriginalImage, hdcImage);

	// Repaint
	InvalidateRect (hWindow, NULL, FALSE);

	// Remove any brush
	if (hBrush)
	{
		DeleteObject (hBrush);
		hBrush = 0;
	}

	return TRUE;
}

/******************************************************************************/
BOOL CCommandArea::SetImage (LPCWSTR url)
{
	if (!url)
		return FALSE;

	// Check URL is long enough
	if (wcslen (url) < 8)
		return FALSE;

	// Load 'file' protocol URLs directly
	if (wcslen (url) > 7 && !wcsnicmp (url, L"file://", 7))
	{
		return SetImageFromFile (url + 7);
	}

	// Should be 'http' or 'ftp' otherwise
	if (wcsnicmp (url, L"http://", 7) && wcsnicmp (url, L"ftp://", 6))
		return FALSE;

	// Start IMO to fetch file
	// Create IMO if not already existing
	if (!imoImage)
		imoImage = pControlsModule->CreateIMO (L"filetransfer");

	// Create filename
	imoImageFilename = new WCHAR [wcslen (CControlsModule::szImageDirectory) + 32];

	do
		wsprintf (imoImageFilename, L"%sdownload%u.bmp", CControlsModule::szImageDirectory, (DWORD) rand ());
	while (GetFileAttributes (imoImageFilename) != 0xFFFFFFFF);

	pControlsModule->SetIMOCallBack (imoImage, StaticIMOCallback, (LPARAM) this);
	pControlsModule->SetIMOProperty (imoImage, L"transferevent", L"Javascript://IMO");
	pControlsModule->SetIMOProperty (imoImage, L"source", url);
	pControlsModule->SetIMOProperty (imoImage, L"destination", imoImageFilename);
	pControlsModule->SetIMOProperty (imoImage, L"overwrite", L"true");
	pControlsModule->CallIMOMethod (imoImage, L"transfer");

	return TRUE;
}

/******************************************************************************/
int CCommandArea::StaticIMOCallback (PVARSTRUCT pvars, int id, LPARAM lparam)
{
	CCommandArea *pthis = (CCommandArea*) lparam;

	if (pvars && !wcsnicmp (pvars->pStr, L"OK", 2))
		pthis->SetImageFromFile (pthis->imoImageFilename);

	DeleteFile (pthis->imoImageFilename);
	delete [] pthis->imoImageFilename;

	return 1;
}

/******************************************************************************/
BOOL CCommandArea::SetHeight (int height)
{
	nHeight = height;
	MoveControl ();

	if (hbmOriginalImage)
		ScaleImage (hbmOriginalImage, hdcImage);

	ResizeHTML ();

	InvalidateRect (hWindow, NULL, FALSE);

	return TRUE;
}

/******************************************************************************/
BOOL CCommandArea::SetBorder (BOOL show)
{
	// Resize window if necessary
	if (bBorder && !show)
	{
		bBorder = FALSE;
		SetHeight (nHeight - 1);
	}
	else if (!bBorder && show)
	{
		bBorder = TRUE;
		SetHeight (nHeight + 1);
	}

	return TRUE;
}

/******************************************************************************/
BOOL CCommandArea::SetColour (LPCWSTR colour)
{
	// Colour string is in format #RR,GG,BB
	int red, green, blue;

	if (!colour)
		return FALSE;

	if (colour [0] != L'#')
		return FALSE;

	if (wcslen (colour) != 7)
		return FALSE;

	if (hBrush)
		DeleteObject (hBrush);

	swscanf (colour + 1, L"%2X%2X%2X", &red, &green, &blue);
	hBrush = CreateSolidBrush (RGB (red, green, blue));
	InvalidateRect (hWindow, NULL, FALSE);

	// Remove any image bitmap
	if (hbmOriginalImage)
	{
		DeleteObject (hbmOriginalImage);
		hbmOriginalImage = 0;
	}

	return TRUE;
}

/******************************************************************************/
void CCommandArea::Paint (HDC hdc)
{
	RECT rect;

	// Get client area to fill
	GetClientRect (hWindow, &rect);

	// Paint with image bitmap if available
	if (hbmOriginalImage)
	{
		BitBlt (hdc, 0, 0, rect.right, rect.bottom, hdcImage, 0, 0, SRCCOPY);
		return;
	}

	// Try brush otherwise
	if (hBrush)
	{
		FillRect (hdc, &rect, hBrush);
	}
}

/******************************************************************************/
LRESULT CALLBACK CCommandArea::StaticWindowProc (HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	CCommandArea *pthis;
	HDC hdc;
	PAINTSTRUCT ps;

	// Get a pointer to ourselves
	pthis = (CCommandArea*) GetWindowLong (hwnd, GWL_USERDATA);

	switch (message)
	{
		case WM_CREATE:
			return 0;

		case WM_PAINT:
			// Paint region using object
			hdc = BeginPaint (hwnd, &ps);
			pthis->Paint (hdc);
			EndPaint (hwnd, &ps);

			return 0;
	}

	return DefWindowProc (hwnd, message, wparam, lparam);
}

/******************************************************************************/
CTopCommandArea::CTopCommandArea (HINSTANCE hinstance, HWND hparent, int instance_id) :
	CCommandArea (hinstance, hparent, instance_id, TRUE)
{
	nTopHeight = nHeight;
	bTopVisible = FALSE;
	ResizeHTML ();
}

/******************************************************************************/
BOOL CTopCommandArea::Show ()
{
	CCommandArea::Show ();

	bTopVisible = TRUE;
	ResizeHTML ();

	return TRUE;
}

/******************************************************************************/
BOOL CTopCommandArea::Hide ()
{
	CCommandArea::Hide ();

	bTopVisible = FALSE;
	ResizeHTML ();

	return TRUE;
}

/******************************************************************************/
BOOL CTopCommandArea::SetHeight (int height)
{
	nTopHeight = height;
	CCommandArea::SetHeight (height);

	return TRUE;
}

/******************************************************************************/
void CTopCommandArea::Paint (HDC hdc)
{
	RECT rect;

	CCommandArea::Paint (hdc);

	if (bBorder)
	{
		GetClientRect (hWindow, &rect);
		MoveToEx (hdc, 0, rect.bottom - 1, NULL);
		LineTo (hdc, rect.right, rect.bottom - 1);
	}
}

/******************************************************************************/
BOOL CTopCommandArea::OnWindowChanged (int event)
{
	// Window orientation has changed, resize command area to match new width, leaving height unchanged
	nWidth = CControlsModule::nScreenWidth;
	MoveControl ();

	if (hbmOriginalImage)
		ScaleImage (hbmOriginalImage, hdcImage);

	InvalidateRect (hWindow, NULL, FALSE);

	return TRUE;
}

/******************************************************************************/
CBottomCommandArea::CBottomCommandArea (HINSTANCE hinstance, HWND hparent, int instance_id) :
	CCommandArea (hinstance, hparent, instance_id, FALSE)
{
	nBottomHeight = nHeight;
	ResizeHTML ();
}

/******************************************************************************/
BOOL CBottomCommandArea::Show ()
{
	CCommandArea::Show ();

	bBottomVisible = TRUE;
	ResizeHTML ();

	return TRUE;
}

/******************************************************************************/
BOOL CBottomCommandArea::Hide ()
{
	CCommandArea::Hide ();

	bBottomVisible = FALSE;
	ResizeHTML ();

	return TRUE;
}

/******************************************************************************/
BOOL CBottomCommandArea::SetHeight (int height)
{
	nBottomHeight = height;
	nTop = CControlsModule::nScreenHeight - height;
	CCommandArea::SetHeight (height);

	return TRUE;
}

/******************************************************************************/
void CBottomCommandArea::Paint (HDC hdc)
{
	RECT rect;

	CCommandArea::Paint (hdc);

	if (bBorder)
	{
		GetClientRect (hWindow, &rect);
		MoveToEx (hdc, 0, 0, NULL);
		LineTo (hdc, rect.right, 0);
	}
}

/******************************************************************************/
BOOL CBottomCommandArea::OnWindowChanged (int event)
{
	// Window orientation has changed, resize command area to match new width, leaving height unchanged
	nWidth = CControlsModule::nScreenWidth;

	// Move command area to be at bottom of screen
	nTop = CControlsModule::nScreenHeight - nHeight;

	MoveControl ();

	if (hbmOriginalImage)
		ScaleImage (hbmOriginalImage, hdcImage);

	InvalidateRect (hWindow, NULL, FALSE);

	return TRUE;
}
