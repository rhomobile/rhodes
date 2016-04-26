/******************************************************************************/
#include <windows.h>
#include "ControlsModule.h"
#include "KeyState.h"

/******************************************************************************/
extern CControlsModule *pControlsModule;

/******************************************************************************/
#define KEYSTATE_CLASS_NAME		L"PB_KEYSTATE"

/******************************************************************************/
BOOL CKeyState::bRegistered = FALSE;

/******************************************************************************/
CKeyState::CKeyState (HINSTANCE hinstance, HWND hparent, int instance_id) :
	CControl (instance_id, hparent)
{
	pControlsModule->Log(PB_LOG_WARNING, L"The KeyState API from the RhoElements Extension has been deprecated in 4.0, please transition your applications to use the common KeyState API for future releases", _T(__FUNCTION__), __LINE__);

	// Register class if needed
	if (!bRegistered)
	{
		WNDCLASS wndclass;
		memset (&wndclass, 0, sizeof wndclass);
		wndclass.lpfnWndProc = StaticWindowProc;
		wndclass.hInstance = hinstance;
		wndclass.lpszClassName = KEYSTATE_CLASS_NAME;
		RegisterClass (&wndclass);

		bRegistered = TRUE;
	}

	// Set default indicator size
	nIndicatorWidth = CControlsModule::nDefaultButtonSize;
	nIndicatorHeight = CControlsModule::nDefaultButtonSize;

	// Set size of overall window starting with zero width
	nWidth = 0;
	nHeight = nIndicatorHeight;

	// Set default position - leave Left at zero since Width is also zero
	nRight = CControlsModule::nScreenWidth - (2 * CControlsModule::nDefaultButtonSize) - 3;
	nLeft = 0;
	nTop = CControlsModule::nScreenHeight - nHeight - 1;

	// Create the window
	hWindow = CreateWindowEx (WS_EX_NOACTIVATE, KEYSTATE_CLASS_NAME, NULL, WS_POPUP, 
		nLeft, nTop, nWidth, nHeight, hparent, (HMENU) -1, hinstance, NULL);

	// Add a pointer to ourselves to the window
	SetWindowLong (hWindow, GWL_USERDATA, (LONG) this);

	// Make sure it's on top of other controls
	SetWindowPos (hWindow, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

	// Bordered by default
	bBorder = TRUE;

	// Load state images
	LoadImages ();
}

/******************************************************************************/
CKeyState::~CKeyState ()
{
	if (hbmShift) DeleteObject (hbmShift);
	if (hbmShiftLock) DeleteObject (hbmShiftLock);
	if (hbmControl) DeleteObject (hbmControl);
	if (hbmAlt) DeleteObject (hbmAlt);
	if (hbmBlue) DeleteObject (hbmBlue);
	if (hbmBlueLock) DeleteObject (hbmBlueLock);
	if (hbmOrange) DeleteObject (hbmOrange);
	if (hbmOrangeLock) DeleteObject (hbmOrangeLock);
	if (hbmGray) DeleteObject (hbmGray);
}
	
/******************************************************************************/
LRESULT CALLBACK CKeyState::StaticWindowProc (HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	CKeyState *pthis;
	HDC hdc;
	PAINTSTRUCT ps;

	// Get a pointer to ourselves
	pthis = (CKeyState*) GetWindowLong (hwnd, GWL_USERDATA);

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
void CKeyState::Paint (HDC hdc)
{
	RECT rect;
	HDC hdc_temp;
	int index;

	// Create a temporary DC to hold a bitmap that needs painting
	hdc_temp = CreateCompatibleDC (NULL);

	// Get client area but don't fill so that existing screen shows through
	GetClientRect (hWindow, &rect);

	for (index = 0; index < nIndicators; index++)
	{
		SelectObject (hdc_temp, hbmIndicators [index]);

		BitBlt (hdc, nWidth - ((index + 1) * nIndicatorWidth) - index, 0, nIndicatorWidth, nIndicatorHeight,
			hdc_temp, 0, 0, SRCCOPY);
	}

	DeleteDC (hdc_temp);
}

/******************************************************************************/
void CKeyState::OnKeyStateChange (DWORD state)
{
	BOOL locked;

	// Key locked?
	locked = (state & LOCKED);

	// Index into array of bitmaps to show
	nIndicators = 0;

	if (state & SHIFT)
		hbmIndicators [nIndicators++] = (locked ? hbmShiftLock : hbmShift);

	if (state & CONTROL)
		hbmIndicators [nIndicators++] = hbmControl;

	if (state & ALT)
		hbmIndicators [nIndicators++] = hbmAlt;

	if (state & BLUE)
		hbmIndicators [nIndicators++] = (locked ? hbmBlueLock : hbmBlue);

	// CAPS_LOCK means Orange on a WTXXXX device?
	//  CAPS_LOCK on VC6090 is received and representated as orange key
	if (state & ORANGE)
		hbmIndicators [nIndicators++] = (locked ? hbmOrangeLock : hbmOrange);

	if (state & CAPS_LOCK)
		hbmIndicators [nIndicators++] = (hbmOrangeLock);

	if (state & GRAY)
		hbmIndicators [nIndicators++] = hbmGray;

	// Set window width to fit number of indicators, allowing for gap between each
	nWidth = (nIndicators * nIndicatorWidth) + (nIndicators - 1);
	nLeft = nRight - nWidth;
	MoveControl ();

	RedrawWindow (hWindow, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
}

/******************************************************************************/
void CKeyState::LoadImages (void)
{
	// Load all indicator images and scale
	LPWSTR pfilename;

	pfilename = new WCHAR [wcslen (CControlsModule::szImageDirectory) + 64];

	wsprintf (pfilename, L"%s%s", CControlsModule::szImageDirectory, KSI_SHIFT);
	hbmShift = LoadScaledImage (pfilename);

	wsprintf (pfilename, L"%s%s", CControlsModule::szImageDirectory, KSI_SHIFT_LOCK);
	hbmShiftLock = LoadScaledImage (pfilename);

	wsprintf (pfilename, L"%s%s", CControlsModule::szImageDirectory, KSI_CONTROL);
	hbmControl = LoadScaledImage (pfilename);

	wsprintf (pfilename, L"%s%s", CControlsModule::szImageDirectory, KSI_ALT);
	hbmAlt = LoadScaledImage (pfilename);

	wsprintf (pfilename, L"%s%s", CControlsModule::szImageDirectory, KSI_BLUE);
	hbmBlue = LoadScaledImage (pfilename);

	wsprintf (pfilename, L"%s%s", CControlsModule::szImageDirectory, KSI_BLUE_LOCK);
	hbmBlueLock = LoadScaledImage (pfilename);

	wsprintf (pfilename, L"%s%s", CControlsModule::szImageDirectory, KSI_ORANGE);
	hbmOrange = LoadScaledImage (pfilename);

	wsprintf (pfilename, L"%s%s", CControlsModule::szImageDirectory, KSI_ORANGE_LOCK);
	hbmOrangeLock = LoadScaledImage (pfilename);

	wsprintf (pfilename, L"%s%s", CControlsModule::szImageDirectory, KSI_GRAY);
	hbmGray = LoadScaledImage (pfilename);

	delete [] pfilename;
}

/******************************************************************************/
HBITMAP CKeyState::LoadScaledImage (LPCWSTR filename)
{
	HBITMAP hbm_original, hbm_scaled;
	HDC hdc_original, hdc_scaled;
	int width_original, height_original;
	BITMAP bitmap;

	// Load image from file
	hbm_original = LoadImageFile (filename);
	if (!hbm_original)
		return NULL;

	// Get details of original image
	GetObject (hbm_original, sizeof bitmap, &bitmap);
	width_original = bitmap.bmWidth;
	height_original = bitmap.bmHeight;

	// Create new bitmap with same colour depth but size of indicator
	hbm_scaled = CreateBitmap (nIndicatorWidth, nIndicatorHeight, bitmap.bmPlanes, bitmap.bmBitsPixel, NULL);

	// Create DC to with scaled bitmap
	hdc_scaled = CreateCompatibleDC (NULL);
	SelectObject (hdc_scaled, hbm_scaled);

	// Create DC with original bitmap
	hdc_original = CreateCompatibleDC (NULL);
	SelectObject (hdc_original, hbm_original);

	if (bBorder)
	{
		// Scale original image into new DC, within border
		// DC is filled with black by default
		StretchBlt (hdc_scaled, 1, 1, nIndicatorWidth - 2, nIndicatorHeight - 2,
			hdc_original, 0, 0, width_original, height_original, SRCCOPY);
	}
	else
	{
		// Scale original image into new DC
		StretchBlt (hdc_scaled, 0, 0, nIndicatorWidth, nIndicatorHeight,
			hdc_original, 0, 0, width_original, height_original, SRCCOPY);
	}

	// Clean up
	DeleteDC (hdc_original);
	DeleteDC (hdc_scaled);

	return hbm_scaled;
}

/******************************************************************************/
BOOL CKeyState::SetBorder (BOOL show)
{
	bBorder = show;
	LoadImages ();
	InvalidateRect (hWindow, NULL, FALSE);

	return TRUE;
}

/******************************************************************************/
BOOL CKeyState::SetHeight (int height)
{
	// Specified height is for individual indicator
	nIndicatorHeight = height;

	// Set overall window height to match
	nHeight = nIndicatorHeight;

	// Re-load and scale indicator images
	LoadImages ();

	MoveControl ();

	return TRUE;
}

/******************************************************************************/
BOOL CKeyState::SetWidth (int width)
{
	// Specified width is for individual indicator
	nIndicatorWidth = width;

	// Re-load and scale indicator images
	LoadImages ();

	MoveControl ();

	return TRUE;
}

/******************************************************************************/
BOOL CKeyState::SetRight (int right)
{
	nRight = right;
	nLeft = nRight - nWidth;
	MoveControl ();

	return TRUE;
}

/******************************************************************************/
BOOL CKeyState::SetLeft (int left)
{
	// Treat 'left' setting as 'right'
	return SetRight (left);
}