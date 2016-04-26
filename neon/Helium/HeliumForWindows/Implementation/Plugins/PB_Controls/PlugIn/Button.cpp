/******************************************************************************/
#include <windows.h>
#include "..\..\..\Engine\Common\pbengine_defs.h"
#include "ControlsModule.h"
#include "AddressBar.h"
#include "Button.h"

/******************************************************************************/
extern CControlsModule *pControlsModule;

/******************************************************************************/
#define BUTTON_CLASS_NAME		L"PB_BUTTON"

/******************************************************************************/
BOOL CButton::bRegistered = FALSE;

/******************************************************************************/
CButton::CButton (HINSTANCE hinstance, HWND hparent, int instance_id) :
	CControl (instance_id, hparent)
{
	// Register class if needed
	if (!bRegistered)
	{
		WNDCLASS wndclass;
		memset (&wndclass, 0, sizeof wndclass);
		wndclass.lpfnWndProc = StaticWindowProc;
		wndclass.hInstance = hinstance;
		wndclass.lpszClassName = BUTTON_CLASS_NAME;
		RegisterClass (&wndclass);

		bRegistered = TRUE;
	}

	// Set default size based on parent window size
	nWidth = CControlsModule::nDefaultButtonSize;
	nHeight = CControlsModule::nDefaultButtonSize;

	hWindow = CreateWindowEx (WS_EX_NOACTIVATE, BUTTON_CLASS_NAME, NULL, WS_POPUP | WS_BORDER, 
		nLeft, nTop, nWidth, nHeight, hparent, (HMENU) -1, hinstance, NULL);

	// Add a pointer to ourselves to the window
	SetWindowLong (hWindow, GWL_USERDATA, (LONG) this);

	// Create DCs to hold scaled images
	hdcImageUp = CreateCompatibleDC (NULL);
	hdcImageDown = CreateCompatibleDC (NULL);

	bButtonDown = FALSE;
	bCapturing = FALSE;

	hbmOriginalImageUp = 0;
	hbmOriginalImageDown = 0;
	imoImageUp = 0;
	imoImageDown = 0;
	imoImageUpFilename = NULL;
	imoImageDownFilename = NULL;
}

/******************************************************************************/
CButton::~CButton ()
{
	HBITMAP hbitmap;

	hbitmap = (HBITMAP) GetCurrentObject (hdcImageUp, OBJ_BITMAP);
	DeleteDC (hdcImageUp);
	if (hbitmap)
		DeleteObject (hbitmap);

	if (hbmOriginalImageUp)
		DeleteObject (hbmOriginalImageUp);

	hbitmap = (HBITMAP) GetCurrentObject (hdcImageDown, OBJ_BITMAP);
	DeleteDC (hdcImageDown);
	if (hbitmap)
		DeleteObject (hbitmap);

	if (hbmOriginalImageDown)
		DeleteObject (hbmOriginalImageDown);

	if (imoImageUp)
		pControlsModule->DeleteIMO (imoImageUp);

	if (imoImageDown)
		pControlsModule->DeleteIMO (imoImageDown);
}

/******************************************************************************/
BOOL CButton::SetImageUpFromFile (LPCWSTR filename)
{
	HBITMAP hbitmap;

	if ((hbitmap = LoadImageFile (filename)) == NULL)
		return FALSE;

	// Free any existing bitmap
	if (hbmOriginalImageUp)
		DeleteObject (hbmOriginalImageUp);

	// Store new one
	hbmOriginalImageUp = hbitmap;

	// Scale into DC
	ScaleImage (hbmOriginalImageUp, hdcImageUp);

	// Repaint
	InvalidateRect (hWindow, NULL, FALSE);

	return TRUE;
}

/******************************************************************************/
BOOL CButton::SetImageUp (LPCWSTR url)
{
	// Check URL is long enough
	if (wcslen (url) < 8)
		return FALSE;

	// Load 'file' protocol URLs directly
	if (wcslen (url) > 7 && !wcsnicmp (url, L"file://", 7))
	{
		return SetImageUpFromFile (url + 7);
	}

	// Should be 'http' or 'ftp' otherwise
	if (wcsnicmp (url, L"http://", 7) && wcsnicmp (url, L"ftp://", 6))
		return FALSE;

	// Create IMO if not already existing
	if (!imoImageUp)
		imoImageUp = pControlsModule->CreateIMO (L"filetransfer");

	// Create filename
	imoImageUpFilename = new WCHAR [wcslen (CControlsModule::szImageDirectory) + 32];

	do
		wsprintf (imoImageUpFilename, L"%sdownload%u.bmp", CControlsModule::szImageDirectory, (DWORD) rand ());
	while (GetFileAttributes (imoImageUpFilename) != 0xFFFFFFFF);

	pControlsModule->SetIMOCallBack (imoImageUp, StaticIMOCallbackUp, (LPARAM) this);
	pControlsModule->SetIMOProperty (imoImageUp, L"transferevent", L"Javascript://IMO");
	pControlsModule->SetIMOProperty (imoImageUp, L"source", url);
	pControlsModule->SetIMOProperty (imoImageUp, L"destination", imoImageUpFilename);
	pControlsModule->SetIMOProperty (imoImageUp, L"overwrite", L"true");
	pControlsModule->CallIMOMethod (imoImageUp, L"transfer");

	return TRUE;
}

/******************************************************************************/
int CButton::StaticIMOCallbackUp (PVARSTRUCT pvars, int id, LPARAM lparam)
{
	// Critical section

	CButton *pthis = (CButton*) lparam;

	if (pvars && !wcsnicmp (pvars->pStr, L"OK", 2))
		pthis->SetImageUpFromFile (pthis->imoImageUpFilename);

	DeleteFile (pthis->imoImageUpFilename);
	delete [] pthis->imoImageUpFilename;

	return 1;
}

/******************************************************************************/
BOOL CButton::SetImageDownFromFile (LPCWSTR filename)
{
	HBITMAP hbitmap;

	if ((hbitmap = LoadImageFile (filename)) == NULL)
		return FALSE;

	// Free any existing bitmap
	if (hbmOriginalImageDown)
		DeleteObject (hbmOriginalImageDown);

	// Store new one
	hbmOriginalImageDown = hbitmap;

	// Scale into DC
	ScaleImage (hbmOriginalImageDown, hdcImageDown);

	// Repaint
	InvalidateRect (hWindow, NULL, FALSE);

	return TRUE;
}

/******************************************************************************/
BOOL CButton::SetImageDown (LPCWSTR url)
{
	// Check URL is long enough
	if (wcslen (url) < 8)
		return FALSE;

	// Load 'file' protocol URLs directly
	if (wcslen (url) > 7 && !wcsnicmp (url, L"file://", 7))
	{
		return SetImageDownFromFile (url + 7);
	}

	// Should be 'http' or 'ftp' otherwise
	if (wcsnicmp (url, L"http://", 7) && wcsnicmp (url, L"ftp://", 6))
		return FALSE;

	// Create IMO if not already existing
	if (!imoImageDown)
		imoImageDown = pControlsModule->CreateIMO (L"filetransfer");

	// Create filename
	imoImageDownFilename = new WCHAR [wcslen (CControlsModule::szImageDirectory) + 32];

	do
		wsprintf (imoImageDownFilename, L"%sdownload%u.bmp", CControlsModule::szImageDirectory, (DWORD) rand ());
	while (GetFileAttributes (imoImageDownFilename) != 0xFFFFFFFF);

	pControlsModule->SetIMOCallBack (imoImageDown, StaticIMOCallbackDown, (LPARAM) this);
	pControlsModule->SetIMOProperty (imoImageDown, L"transferevent", L"Javascript://IMO");
	pControlsModule->SetIMOProperty (imoImageDown, L"source", url);
	pControlsModule->SetIMOProperty (imoImageDown, L"destination", imoImageDownFilename);
	pControlsModule->SetIMOProperty (imoImageDown, L"overwrite", L"true");
	pControlsModule->CallIMOMethod (imoImageDown, L"transfer");

	return TRUE;
}

/******************************************************************************/
int CButton::StaticIMOCallbackDown (PVARSTRUCT pvars, int id, LPARAM lparam)
{
	// Critical section

	CButton *pthis = (CButton*) lparam;

	if (pvars && !wcsnicmp (pvars->pStr, L"OK", 2))
		pthis->SetImageDownFromFile (pthis->imoImageDownFilename);

	DeleteFile (pthis->imoImageDownFilename);
	delete [] pthis->imoImageDownFilename;

	return 1;
}

/******************************************************************************/
BOOL CButton::SetWidth (int width)
{
	nWidth = width;
	MoveControl ();

	ScaleImage (hbmOriginalImageUp, hdcImageUp);
	ScaleImage (hbmOriginalImageDown, hdcImageDown);
	InvalidateRect (hWindow, NULL, FALSE);

	return TRUE;
}

/******************************************************************************/
BOOL CButton::SetHeight (int height)
{
	nHeight = height;
	MoveControl ();

	ScaleImage (hbmOriginalImageUp, hdcImageUp);
	ScaleImage (hbmOriginalImageDown, hdcImageDown);
	InvalidateRect (hWindow, NULL, FALSE);

	return TRUE;
}

/******************************************************************************/
BOOL CButton::SetBorder (BOOL show)
{
	if (!CControl::SetBorder (show))
		return FALSE;

	ScaleImage (hbmOriginalImageUp, hdcImageUp);
	ScaleImage (hbmOriginalImageDown, hdcImageDown);

	return TRUE;
}

/******************************************************************************/
void CButton::Paint (HDC hdc)
{
	RECT rect;
	int width, height;

	// Get client size of control
	GetClientRect (hWindow, &rect);
	width = rect.right;
	height = rect.bottom;

	if (bButtonDown)
		BitBlt (hdc, 0, 0, width, height, hdcImageDown, 0, 0, SRCCOPY);
	else
		BitBlt (hdc, 0, 0, width, height, hdcImageUp, 0, 0, SRCCOPY);
}

/******************************************************************************/
LRESULT CALLBACK CButton::StaticWindowProc (HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	CButton *pthis;
	HDC hdc;
	PAINTSTRUCT ps;
	int x, y;
	BOOL was_down;

	// Get a pointer to ourselves
	pthis = (CButton*) GetWindowLong (hwnd, GWL_USERDATA);

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

		case WM_LBUTTONDOWN:
			pthis->bButtonDown = TRUE;
			pthis->bCapturing = TRUE;
			SetCapture (hwnd);
			RedrawWindow (hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
			return 0;

		case WM_MOUSEMOVE:
			if (pthis->bCapturing)
			{
				// Get mouse position relative to our client area (so values could be negative)
				x = (short) LOWORD (lparam);
				y = (short) HIWORD (lparam);

				// Change button state if necessary depending on whether mouse is outside our window
				if (x < 0 || x > pthis->nWidth || y < 0 || y > pthis->nHeight)
				{
					// Outside
					if (pthis->bButtonDown)
					{
						pthis->bButtonDown = FALSE;
						RedrawWindow (hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
					}
				}
				else
				{
					// Inside
					if (!pthis->bButtonDown)
					{
						pthis->bButtonDown = TRUE;
						RedrawWindow (hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
					}
				}
			}

			return 0;

		case WM_LBUTTONUP:
			was_down = pthis->bButtonDown;
			pthis->bButtonDown = FALSE;
			pthis->bCapturing = FALSE;
			ReleaseCapture ();
			RedrawWindow (hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

			if (was_down)
				pthis->OnClick();

			return 0;
	}

	return DefWindowProc (hwnd, message, wparam, lparam);
}

/******************************************************************************/
void CButton::LoadDefaultImages (LPCWSTR image_up, LPCWSTR image_down)
{
	LPWSTR pfile;

	pfile = new WCHAR [wcslen (CControlsModule::szImageDirectory) + wcslen (image_up) + 16];
	wcscpy (pfile, L"file://");
	wcscat (pfile, CControlsModule::szImageDirectory);
	wcscat (pfile, image_up);
	SetImageUp (pfile);
	delete [] pfile;

	pfile = new WCHAR [wcslen (CControlsModule::szImageDirectory) + wcslen (image_down) + 16];
	wcscpy (pfile, L"file://");
	wcscat (pfile, CControlsModule::szImageDirectory);
	wcscat (pfile, image_down);
	SetImageDown (pfile);
	delete [] pfile;
}

/******************************************************************************/
CQuitButton::CQuitButton (HINSTANCE hinstance, HWND hparent, int instance_id) :
	CButton (hinstance, hparent, instance_id)
{
	pControlsModule->Log(PB_LOG_WARNING, L"The Quit Button API from the RhoElements Extension has been deprecated in 4.0, please transition your applications to use the common Native Toolbar API for future releases", _T(__FUNCTION__), __LINE__);

	// Set default position
	nLeft = CControlsModule::nScreenWidth - nWidth - 1;
	nTop = CControlsModule::nScreenHeight - nHeight - 1;
	MoveControl ();
	//  I have swapped the quit and sip buttons as the 
	//  SIP now defaults to the bottom of the screen
//	nLeft = CControlsModule::nScreenWidth - nWidth - 1;
//	nTop = 1;
//	MoveControl ();

	LoadDefaultImages (L"QuitButton.bmp", L"QuitButtonDown.bmp");
}

/******************************************************************************/
void CQuitButton::OnClick ()
{
	(*CControlsModule::PBCoreStruct.pBrowserQuit) (nInstanceID, MODULE_NAME);
}

/******************************************************************************/
CReloadButton::CReloadButton (HINSTANCE hinstance, HWND hparent, int instance_id) :
	CButton (hinstance, hparent, instance_id)
{
	pControlsModule->Log(PB_LOG_WARNING, L"The Reload Button API from the RhoElements Extension has been deprecated in 4.0, please transition your applications to use the common Native Toolbar API for future releases", _T(__FUNCTION__), __LINE__);

	// Set default position
	nLeft = CControlsModule::nScreenWidth - (2 * nWidth) - 2;
	nTop = 1;
	MoveControl ();

	LoadDefaultImages (L"ReloadButton.bmp", L"ReloadButtonDown.bmp");
}

/******************************************************************************/
void CReloadButton::OnClick ()
{
	(*CControlsModule::PBCoreStruct.pBrowserReload) (nInstanceID, FALSE, MODULE_NAME);
}

/******************************************************************************/
CBackButton::CBackButton (HINSTANCE hinstance, HWND hparent, int instance_id) :
	CButton (hinstance, hparent, instance_id)
{
	pControlsModule->Log(PB_LOG_WARNING, L"The Back Button API from the RhoElements Extension has been deprecated in 4.0, please transition your applications to use the common Native Toolbar API for future releases", _T(__FUNCTION__), __LINE__);

	// Set default position
	nLeft = CControlsModule::nScreenWidth - (4 * nWidth) - 4;
	nTop = 1;
	MoveControl ();

	LoadDefaultImages (L"BackUp.bmp", L"BackDown.bmp");
}

/******************************************************************************/
void CBackButton::OnClick ()
{
	(*CControlsModule::PBCoreStruct.pBrowserBack) (nInstanceID, MODULE_NAME);
}

/******************************************************************************/
CForwardButton::CForwardButton (HINSTANCE hinstance, HWND hparent, int instance_id) :
	CButton (hinstance, hparent, instance_id)
{
	pControlsModule->Log(PB_LOG_WARNING, L"The Forward Button API from the RhoElements Extension has been deprecated in 4.0, please transition your applications to use the common Native Toolbar API for future releases", _T(__FUNCTION__), __LINE__);

	// Set default position
	nLeft = CControlsModule::nScreenWidth - (3 * nWidth) - 3;
	nTop = 1;
	MoveControl ();

	LoadDefaultImages (L"FWDButton.bmp", L"FWDButtonDown.bmp");
}

/******************************************************************************/
void CForwardButton::OnClick ()
{
	(*CControlsModule::PBCoreStruct.pBrowserForward) (nInstanceID, MODULE_NAME);
}

/******************************************************************************/
CGoButton::CGoButton (HINSTANCE hinstance, HWND hparent, int instance_id) :
	CButton (hinstance, hparent, instance_id)
{
	pControlsModule->Log(PB_LOG_WARNING, L"The Go Button API from the RhoElements Extension has been deprecated in 4.0, please transition your applications to use the common Native Toolbar API for future releases", _T(__FUNCTION__), __LINE__);

	pAddressBar = NULL;

	// Set default position
	nLeft = CControlsModule::nScreenWidth - (5 * nWidth) - 5;
	nTop = 1;
	MoveControl ();

	LoadDefaultImages (L"GoUp.bmp", L"GoDown.bmp");

	bEnabled = FALSE;
}

/******************************************************************************/
void CGoButton::OnClick ()
{
	if (pAddressBar)
		pAddressBar->NavigateText ();
}

/******************************************************************************/
BOOL CGoButton::Show ()
{
	bEnabled = TRUE;
	Display (TRUE);
	return TRUE;
}

/******************************************************************************/
BOOL CGoButton::Hide ()
{
	Display (FALSE);
	bEnabled = FALSE;
	return TRUE;
}

/******************************************************************************/
void CGoButton::Display (BOOL visible)
{
	if (!bEnabled)
		return;

	if (visible)
		CControl::Show ();
	else
		CControl::Hide ();
}

/******************************************************************************/
CHomeButton::CHomeButton (HINSTANCE hinstance, HWND hparent, int instance_id) :
	CButton (hinstance, hparent, instance_id)
{
	pControlsModule->Log(PB_LOG_WARNING, L"The Home Button API from the RhoElements Extension has been deprecated in 4.0, please transition your applications to use the common Native Toolbar API for future releases", _T(__FUNCTION__), __LINE__);

	// Set default position
	nLeft = CControlsModule::nScreenWidth - (6 * nWidth) - 6;
	nTop = 1;
	MoveControl ();

	LoadDefaultImages (L"HomeUp.bmp", L"HomeDown.bmp");
}

/******************************************************************************/
void CHomeButton::OnClick ()
{
	(*CControlsModule::PBCoreStruct.pBrowserHome) (nInstanceID, MODULE_NAME);
}

/******************************************************************************/
CMinimizeButton::CMinimizeButton (HINSTANCE hinstance, HWND hparent, int instance_id) :
	CButton (hinstance, hparent, instance_id)
{
	pControlsModule->Log(PB_LOG_WARNING, L"The Minimize Button API from the RhoElements Extension has been deprecated in 4.0, please transition your applications to use the common Native Toolbar API for future releases", _T(__FUNCTION__), __LINE__);

	// Set default position - same as Quit
	nLeft = CControlsModule::nScreenWidth - nWidth - 1;
	nTop = 1;
	MoveControl ();

	LoadDefaultImages (L"MinButton.bmp", L"MinButtonDown.bmp");
}

/******************************************************************************/
void CMinimizeButton::OnClick ()
{
	(*CControlsModule::PBCoreStruct.pBrowserMininmize) (nInstanceID, MODULE_NAME);
}

/******************************************************************************/
CPageZoomInButton::CPageZoomInButton (HINSTANCE hinstance, HWND hparent, int instance_id) :
	CButton (hinstance, hparent, instance_id)
{
	pControlsModule->Log(PB_LOG_WARNING, L"The Zoom Button API from the RhoElements Extension has been deprecated in 4.0, please transition your applications to use the common Native Toolbar API for future releases", _T(__FUNCTION__), __LINE__);

	// Set default position
	nLeft = CControlsModule::nScreenWidth - (4 * nWidth) - 4;
	nTop = CControlsModule::nScreenHeight - nHeight - 1;
	MoveControl ();

	LoadDefaultImages (L"ZoomInPageButton.bmp", L"ZoomInPageButtonDown.bmp");
}

/******************************************************************************/
void CPageZoomInButton::OnClick ()
{
	// Not implemented in the core; no pointer to callback is set
	//double zoom = (*CControlsModule::PBCoreStruct.pBrowserGetPageZoom) (nInstanceID, MODULE_NAME);
}

/******************************************************************************/
CPageZoomOutButton::CPageZoomOutButton (HINSTANCE hinstance, HWND hparent, int instance_id) :
	CButton (hinstance, hparent, instance_id)
{
	pControlsModule->Log(PB_LOG_WARNING, L"The Zoom Button API from the RhoElements Extension has been deprecated in 4.0, please transition your applications to use the common Native Toolbar API for future releases", _T(__FUNCTION__), __LINE__);

	// Set default position
	nLeft = CControlsModule::nScreenWidth - (3 * nWidth) - 3;
	nTop = CControlsModule::nScreenHeight - nHeight - 1;
	MoveControl ();

	LoadDefaultImages (L"ZoomOutPageButton.bmp", L"ZoomOutPageButtonDown.bmp");
}

/******************************************************************************/
void CPageZoomOutButton::OnClick ()
{
	// Not implemented in the core; no pointer to callback is set
}

/******************************************************************************/
CSIPButton::CSIPButton (HINSTANCE hinstance, HWND hparent, int instance_id) :
	CButton (hinstance, hparent, instance_id)
{
	pControlsModule->Log(PB_LOG_WARNING, L"The SIP Button API from the RhoElements Extension has been deprecated in 4.0, please transition your applications to use the common Native Toolbar API for future releases", _T(__FUNCTION__), __LINE__);

	// Set default position
	nLeft = CControlsModule::nScreenWidth - nWidth - 1;
	nTop = 1;
	MoveControl ();
	//  I have moved the SIP button to where the quit button used to be as
	//  The SIP is placed at the bottom of the screen by default
	//  (of course a user can move it)
//	nLeft = CControlsModule::nScreenWidth - nWidth - 1;
//	nTop = CControlsModule::nScreenHeight - nHeight - 1;
//	MoveControl ();

	LoadDefaultImages (L"SIPUp.bmp", L"SIPDown.bmp");
}

/******************************************************************************/
void CSIPButton::OnClick ()
{
	pSIPControl->ToggleSIP ();
	(*CControlsModule::PBCoreStruct.pSipButtonClick) (nInstanceID, MODULE_NAME);
}

/******************************************************************************/
CStopButton::CStopButton (HINSTANCE hinstance, HWND hparent, int instance_id) :
	CButton (hinstance, hparent, instance_id)
{
	pControlsModule->Log(PB_LOG_WARNING, L"The Stop Button API from the RhoElements Extension has been deprecated in 4.0, please transition your applications to use the common Native Toolbar API for future releases", _T(__FUNCTION__), __LINE__);

	// Set default position - same as Go
	nLeft = CControlsModule::nScreenWidth - (5 * nWidth) - 5;
	nTop = 1;
	MoveControl ();

	LoadDefaultImages (L"StopButton.bmp", L"StopButtonDown.bmp");

	bEnabled = FALSE;
}

/******************************************************************************/
void CStopButton::OnClick ()
{
	(*CControlsModule::PBCoreStruct.pBrowserStop) (nInstanceID, MODULE_NAME);
}

/******************************************************************************/
BOOL CStopButton::Show ()
{
	bEnabled = TRUE;
	return TRUE;
}

/******************************************************************************/
BOOL CStopButton::Hide ()
{
	Display (FALSE);
	bEnabled = FALSE;
	return TRUE;
}

/******************************************************************************/
void CStopButton::Display (BOOL visible)
{
	if (!bEnabled)
		return;

	if (visible)
		CControl::Show ();
	else
		CControl::Hide ();

	/*
	No window processing happens until after document complete when using COM engine on WM?
	*/
}

/******************************************************************************/
CZoomTextButton::CZoomTextButton (HINSTANCE hinstance, HWND hparent, int instance_id) :
	CButton (hinstance, hparent, instance_id)
{
	pControlsModule->Log(PB_LOG_WARNING, L"The Zoom Text Button API from the RhoElements Extension has been deprecated in 4.0, please transition your applications to use the common Native Toolbar API for future releases", _T(__FUNCTION__), __LINE__);

	// Set default position
	nLeft = CControlsModule::nScreenWidth - (2 * nWidth) - 2;
	nTop = CControlsModule::nScreenHeight - nHeight - 1;
	MoveControl ();

	LoadDefaultImages (L"ZoomTextUp.bmp", L"ZoomTextDown.bmp");
}

/******************************************************************************/
void CZoomTextButton::OnClick ()
{
	TextZoomValue zoom;

	// Get current zoom
	zoom = (TextZoomValue) (*CControlsModule::PBCoreStruct.pBrowserGetTxtZoom) (nInstanceID, MODULE_NAME);

	switch (zoom)
	{
		case TEXT_ZOOM_SMALLEST: zoom = TEXT_ZOOM_SMALLER; break;
		case TEXT_ZOOM_SMALLER: zoom = TEXT_ZOOM_NORMAL; break;
		case TEXT_ZOOM_NORMAL: zoom = TEXT_ZOOM_BIGGER; break;
		case TEXT_ZOOM_BIGGER: zoom = TEXT_ZOOM_BIGGEST; break;
		case TEXT_ZOOM_BIGGEST: zoom = TEXT_ZOOM_SMALLEST; break;
	}

	(*CControlsModule::PBCoreStruct.pBrowserSetTxtZoom) (nInstanceID, zoom, MODULE_NAME);
}

/******************************************************************************/
/*
Multiple custom buttons per application instance:
Need ID per button - default to button1, button2, etc., as per gesture
Apply MetaProc calls to most recently specified ID
*/
/******************************************************************************/
CCustomButton::CCustomButton (HINSTANCE hinstance, HWND hparent, int instance_id) :
	CButton (hinstance, hparent, instance_id)
{
	psNavigate = NULL;

	nLeft = 1;
	nTop = 1;
	MoveControl ();

	LoadDefaultImages (L"CustomUp.bmp", L"CustomDown.bmp");
}

/******************************************************************************/
CCustomButton::~CCustomButton ()
{
	if (psNavigate)
		free (psNavigate);
}

/******************************************************************************/
BOOL CCustomButton::SetClick (LPCWSTR navigate)
{
	if (psNavigate)
	{
		free (psNavigate);
		psNavigate = NULL;
	}

	if (navigate)
		psNavigate = _wcsdup (navigate);

	return TRUE;
}

/******************************************************************************/
void CCustomButton::OnClick ()
{
	if (psNavigate)
		pControlsModule->SendPBNavigate (tcCustomButtonEventNames, nInstanceID, psNavigate, psID, NULL);
}
