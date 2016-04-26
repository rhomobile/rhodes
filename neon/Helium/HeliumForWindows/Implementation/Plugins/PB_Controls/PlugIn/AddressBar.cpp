/******************************************************************************/
#include <windows.h>
#include "ControlsModule.h"
#include "AddressBar.h"

/******************************************************************************/
extern CControlsModule *pControlsModule;

/******************************************************************************/
#define ADDRESSBAR_CLASS_NAME		L"PB_ADDRESSBAR"
#define ID_COMBOBOX					1

/******************************************************************************/
CAddressBar::CAddressBar (HINSTANCE hinstance, HWND hparent, int instance_id) :
	CControl (instance_id, hparent)
{
	WNDCLASS wndclass;
	RECT rect;
	bGotFocus = false;

	memset (&wndclass, 0, sizeof wndclass);
	wndclass.lpfnWndProc = StaticWindowProc;
	wndclass.hInstance = hinstance;
	wndclass.lpszClassName = ADDRESSBAR_CLASS_NAME;
	RegisterClass (&wndclass);

	// Set default width of containing window based on parent window size
	nWidth = CControlsModule::nScreenWidth - (5 * CControlsModule::nDefaultButtonSize) - 7;

	// Default position
	nLeft = 1;
	nTop = 1;

	// Create the window
	// Use zero height until we set it later once the combobox has been created
	hWindow = CreateWindowEx (0, ADDRESSBAR_CLASS_NAME, NULL, WS_POPUP, 
		nLeft, nTop, nWidth, 0, hparent, (HMENU) -1, hinstance, NULL);

	// Add a pointer to ourselves to the window
	SetWindowLong (hWindow, GWL_USERDATA, (LONG) this);

	// Make sure it's on top of other controls
	SetWindowPos (hWindow, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

	// Calculate height of opened combobox
	nOpenHeight = (int) (CControlsModule::nScreenHeight / 1.6);

	// Create combobox as a child of containing window
	// Height specified is height of dropped-down combobox
	hComboBox = CreateWindow (L"COMBOBOX", NULL, WS_VISIBLE | WS_CHILD | CBS_DROPDOWN | CBS_AUTOHSCROLL, 
		0, 0, nWidth, nOpenHeight, hWindow, (HMENU) ID_COMBOBOX, hinstance, NULL);

	// Get combobox info, in particular the handle of its edit control
	COMBOBOXINFO info;
	info.cbSize = sizeof info;
	SendMessage (hComboBox, CB_GETCOMBOBOXINFO, 0, (LPARAM) &info);

	// Add a pointer to ourselves to the combobox's edit control
	SetWindowLong (info.hwndItem, GWL_USERDATA, (LONG) this);

	// Subclass the edit control within the combobox so we can get the Enter key
	lpfnComboBoxProc = (WNDPROC) GetWindowLong (info.hwndItem, GWL_WNDPROC);
	SetWindowLong (info.hwndItem, GWL_WNDPROC, (DWORD) StaticComboBoxProc);

	// Find height of closed combobox, set containing window height to match
	GetWindowRect (hComboBox, &rect);
	nHeight = rect.bottom - rect.top;
	MoveControl ();
}

/******************************************************************************/
CAddressBar::~CAddressBar ()
{
	DestroyWindow (hComboBox);
}

/******************************************************************************/
LRESULT CALLBACK CAddressBar::StaticComboBoxProc (HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	CAddressBar *pthis = (CAddressBar*) GetWindowLong (hwnd, GWL_USERDATA);

	switch (message)
	{
		case WM_SETFOCUS:
			pthis->bGotFocus = true;
			HideSIPButton();
			pthis->pSIPControl->ShowSIP (pthis->hwndTopLevel, TRUE);
			break;

		case WM_KILLFOCUS:
			pthis->bGotFocus = false;
			pthis->pSIPControl->HideSIP (pthis->hwndTopLevel, TRUE);
			break;

//		case WM_CHAR:
//			// If Enter was pressed then get the text and navigate to it
//			if (wparam == 0x0D)
//			{
//				pthis->NavigateText ();
//				return 0;
//			}
	}

	return CallWindowProc (pthis->lpfnComboBoxProc, hwnd, message, wparam, lparam);
}

/******************************************************************************/
LRESULT CALLBACK CAddressBar::StaticWindowProc (HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	CAddressBar *pthis;
	LPCWSTR purl;

#if DEBUG
	// Paint window in red to highlight extent of contained combobox
	HDC hdc;
	PAINTSTRUCT ps;
	static HBRUSH hbrush;
#endif

	// Get a pointer to ourselves
	pthis = (CAddressBar*) GetWindowLong (hwnd, GWL_USERDATA);

	switch (message)
	{
		case WM_CREATE:
#if DEBUG
			hbrush = CreateSolidBrush (RGB (255, 0, 0));
#endif
			return 0;

		case WM_DESTROY:
#if DEBUG
			DeleteObject (hbrush);
#endif
			break;
#if DEBUG
		case WM_PAINT:
			hdc = BeginPaint (hwnd, &ps);
			FillRect (hdc, &(ps.rcPaint), hbrush);
			EndPaint (hwnd, &ps);
			return 0;
#endif

		case WM_ADDURL:
			// Add URL passed in lparam to combobox and free memory
			purl = (WCHAR*) lparam;
			pthis->AddURL (purl);
			delete [] purl;
			return 0;

		case WM_COMMAND:
			if (LOWORD (wparam) != ID_COMBOBOX)
				break;

			switch (HIWORD (wparam))
			{
				case CBN_SELENDOK:
					pthis->NavigateSelected ();
					break;
			}

			return 0;
	}

	return DefWindowProc (hwnd, message, wparam, lparam);
}

/******************************************************************************/
void CAddressBar::AddURL (LPCWSTR purl)
{
	int index;

	// Add URL if not already present and select
	index = SendMessage (hComboBox, CB_FINDSTRINGEXACT, -1, (LPARAM) purl);
	if (index == CB_ERR)
		index = SendMessage (hComboBox, CB_INSERTSTRING, 0, (LPARAM) purl);

	SendMessage (hComboBox, CB_SETCURSEL, index, 0);
}

/******************************************************************************/
void CAddressBar::RemoveEncoding (LPCWSTR psource, LPWSTR pdest)
{
	WCHAR hex [3];
	int state;

	state = 0;

	while (*psource != L'\0')
	{
		switch (state)
		{
		case 0:			// Normal
			if (*psource == L'+')
				*pdest++ = L' ';
			else if (*psource == L'%')
				state = 1;
			else
				*pdest++ = *psource;
			break;

		case 1:			// First hex digit
			hex [0] = *psource;
			state = 2;
			break;

		case 2:			// Second hex digit
			hex [1] = *psource;
			hex [2] = 0;
			*pdest++ = (WCHAR) wcstol (hex, NULL, 16);
			state = 0;
			break;
		}

		psource++;
	}

	*pdest = L'\0';
}

/******************************************************************************/
void CAddressBar::OnNavigate ()
{
//	LPCWSTR ps, pd;
//	int index;
	WCHAR *pbuffer;

	pbuffer = new WCHAR [MAX_URL];

	// Get new URL and remove URL encoded characters (like %20 for 'space')
	RemoveEncoding ((*CControlsModule::PBCoreStruct.pGetAppCurrURL) (nInstanceID), pbuffer);

	if (wcslen (pbuffer) > 0)
	{
		// PostMessage to control window which will in turn send to combobox and free allocated memory.
		// If we SendMessage to combobox there will be threadlock because we are not in the same thread as the control's message pump.
		PostMessage (hWindow, WM_ADDURL, 0, (LPARAM) pbuffer);
	}
	else
		delete [] pbuffer;
}

/******************************************************************************/
void CAddressBar::NavigateText ()
{
	int length;
	LPWSTR purl;

	length = GetWindowTextLength (hComboBox);
	if (length > 0)
	{
		purl = new WCHAR [length + 1];

		if (GetWindowText (hComboBox, purl, length + 1) > 0)
		{
			(*CControlsModule::PBCoreStruct.pNavigateFunc) (tcAddressBarEventNames, nInstanceID, purl, NULL, MODULE_NAME);

			// Save the url to the list if not already present
			// GD
			// This is now done by the onNavigate function
			/*if (SendMessage (hComboBox, CB_FINDSTRINGEXACT, -1, (LPARAM) purl) == CB_ERR)
				SendMessage (hComboBox, CB_INSERTSTRING, 0, (LPARAM) purl);*/
		}

		delete [] purl;
	}
}

void CAddressBar::DeleteCharacter()
{
	//  If we send the backspace key to the address bar it will be intercepted
	//  by the engine and Translate Accelerated, therefore we implement our own
	HWND hEditControl = ::GetWindow(hComboBox, GW_CHILD);
	SendMessage(hEditControl, WM_KEYDOWN, 0x08, 0x00660001);
	SendMessage(hEditControl, WM_CHAR, 0x08, 0x00660001);
	SendMessage(hEditControl, WM_KEYUP, 0x08, 0x00660001);
	return;
}

/******************************************************************************/
void CAddressBar::NavigateSelected ()
{
	int selection, length;
	LPWSTR purl;

	selection = SendMessage (hComboBox, CB_GETCURSEL, 0, 0);
	if (selection == CB_ERR)
		return;

	length = SendMessage (hComboBox, CB_GETLBTEXTLEN, selection, 0);
	if (length == CB_ERR)
		return;

	purl = new WCHAR [length + 1];
	SendMessage (hComboBox, CB_GETLBTEXT, selection, (LPARAM) purl);
	(*CControlsModule::PBCoreStruct.pNavigateFunc) (tcAddressBarEventNames, nInstanceID, purl, NULL, MODULE_NAME);
	delete [] purl;
}

/******************************************************************************/
BOOL CAddressBar::SetWidth (int width)
{
	// Set width of containing window and of combobox
	CControl::SetWidth (width);
	MoveWindow (hComboBox, 0, 0, nWidth, nOpenHeight, FALSE);

	return TRUE;
}

/******************************************************************************/
BOOL CAddressBar::SetHeight (int height)
{
	return FALSE;
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		April 2010
*/
BOOL CAddressBar::onPrimaryMessage (MSG *pmsg)
{

	//  If the address bar does not have focus just return and don't handle the message
	if (!bGotFocus)
		return FALSE;

	switch (pmsg->message)
	{
		case WM_KEYUP:
			// If Enter was pressed then get the text and navigate to it
			if (pmsg->wParam == 0x0D)
			{
				CAddressBar *pthis = (CAddressBar*) GetWindowLong (pmsg->hwnd, GWL_USERDATA);
				if (pthis)
					pthis->NavigateText ();
				return TRUE;
			}
			else if(pmsg->wParam == 0x08)
			{
				CAddressBar *pthis = (CAddressBar*) GetWindowLong (pmsg->hwnd, GWL_USERDATA);
				if (pthis)
					pthis->DeleteCharacter();
				return TRUE;
			}
		case WM_KEYDOWN:
			//  If the Delete key was pressed delete a character
			if(pmsg->wParam == 0x08)
			{
				return TRUE;
			}
		case WM_CHAR:
			if (pmsg->wParam == 0x08)
			{
				return TRUE;
			}
	}
	//  Do not block this message from proceeding further in the pump
	return FALSE;
}