// NSEdit.cpp : Implementation of CNSEdit
#include "stdafx.h"
#include "NSEdit.h"
#include "kbdapi.h"


// CNSEdit

/******************************************************************************/
LRESULT CALLBACK CNSEdit::NewWndProc (HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	CNSEdit *pthis;
	POINT pt;
	HWND hexplorer;
	int r;

//		varResult.scode = 0;

	// Get pointer to related object
	pthis = (CNSEdit*) ::GetWindowLong (hwnd, GWL_USERDATA);

	switch (msg)
	{
	case WM_KEYDOWN:
		pthis->Fire_OnKeyDown ((int) wparam, LOWORD (lparam), HIWORD (lparam));
		break;

	case WM_KEYUP:
		pthis->Fire_OnKeyUp ((int) wparam, LOWORD (lparam), HIWORD (lparam));
		break;

	case WM_CHAR:
		r = pthis->Fire_OnKeyPress (wparam);

		// Check return value from handler, -ve => cancel
		if (r < 0) return 0;

		// +ve => replace key with return value
		if (r > 0) wparam = r;

		break;

	case WM_LBUTTONDOWN:
		// Map cursor position to Explorer-relative co-ordinates
		pt.x = LOWORD (lparam);
		pt.y = HIWORD (lparam);
		hexplorer = pthis->GetParent ();
		::MapWindowPoints (hwnd, hexplorer, &pt, 1);

		pthis->Fire_OnClick (pt.x, pt.y);

		break;
	}

	return CallWindowProc (pthis->pOldWndProc, hwnd, msg, wparam, lparam);
}

/******************************************************************************/
void CNSEdit::CreateEditWindow (void)
{
	RECT rect;
	int w, h;// count;
	HWND hnew;
	//WCHAR *pws;
	DWORD style;

	if(m_hWnd==NULL) return;
	style = 0;

	// Get our window size
	GetWindowRect (&rect);

	// Get width and height
	w = rect.right - rect.left;
	h = rect.bottom - rect.top;

	// Get alignment style
	if ((_wcsicmp ((BSTR) strAlignment, L"CENTRE") == 0) ||
		(_wcsicmp ((BSTR) strAlignment, L"CENTER") == 0)) style |= ES_CENTER;
	else if (_wcsicmp ((BSTR) strAlignment, L"RIGHT") == 0) style |= ES_RIGHT;
	else style |= ES_LEFT;

	// Password field cannot be multiline (so centre/right alignment won't work either)
	if (bPassword) style |= ES_PASSWORD;
	else style |= ES_MULTILINE;

	// Border the edit control
	if (bBorder) style |= WS_BORDER;

	// Create new edit control to fit
	//hnew = CreateWindow (L"EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL | style,
	hnew = CreateWindow (L"EDIT", NULL, WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | style,
		0, 0, w, h, m_hWnd, (HMENU) EDIT_ID, _AtlBaseModule.GetModuleInstance(), NULL);

	// Store pointer to ourselves along with window
	::SetWindowLong (hnew, GWL_USERDATA, (LONG) this);

	// Subclass edit control
	pOldWndProc = (WNDPROC) ::GetWindowLong (hnew, GWL_WNDPROC);
	::SetWindowLong (hnew, GWL_WNDPROC, (LONG) NewWndProc);

	//set the font
	LOGFONT lf;
	HFONT hh = (HFONT)GetStockObject( SYSTEM_FONT );
	GetObject( hh, sizeof(lf), &lf );

	lf.lfHeight = m_iHeight; 
	if(m_bBold) 
		lf.lfWeight = FW_BOLD; 
	else
		lf.lfWeight = FW_NORMAL; 

	lf.lfUnderline = m_bUnderLine;
	lf.lfItalic = m_bItalic;
	
	lf.lfPitchAndFamily = m_bFixed;
	_tcscpy( lf.lfFaceName, m_bstrFont);
	HFONT hFnt = CreateFontIndirect( &lf ); 
	//don't compile this, the hFnt will leak memory above DS
	::SendMessage( hnew, WM_SETFONT, (WPARAM)hFnt, 0 );
    //::InvalidateRect( hnew, NULL, true );  // force a re-draw

	// Get text from any existing control
	if (::IsWindow (hEdit))
	{
		/*count = ::GetWindowTextLength (hEdit);
		if (count > 0)
		{
			pws = new WCHAR [count + 1];
			::GetWindowText (hEdit, pws, count + 1);

			// Set new control to the same text
			::SetWindowText (hnew, pws);

			delete [] pws;
		}
		*/
		//::SetWindowText (hnew, m_bstrValue);
		// Replace old control with new
		::DestroyWindow (hEdit);
	}

	::SetWindowText (hnew, m_bstrValue);
	hEdit = hnew;
	
	// Set any other current properties
	::SendMessage (hEdit, EM_LIMITTEXT, nMaxLength, 0);
	::SendMessage( hEdit, EM_SETSEL, m_iStart, m_iEnd);
}

/******************************************************************************/
LRESULT CNSEdit::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// Create internal edit control, default left align
	CreateEditWindow ();

	return 0;
}

/******************************************************************************/
LRESULT CNSEdit::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	int w, h;

	// Resize our edit control
	w = LOWORD (lParam);
	h = HIWORD (lParam);
	::SetWindowPos (hEdit, NULL, 0,	0, w, h, SWP_NOZORDER | SWP_NOACTIVATE);

	return 0;
}

/******************************************************************************/
STDMETHODIMP CNSEdit::get_Value(BSTR *pVal)
{
	WCHAR *pws;
	int count;

	pws = 0;

	if (::IsWindow (hEdit))
	{
		count = ::GetWindowTextLength (hEdit);
		if (count > 0)
		{
			pws = new WCHAR [count + 1];
			::GetWindowText (hEdit, pws, count + 1);
		}
	}

	if (pws)
	{
		*pVal = SysAllocString (pws);
		delete [] pws;
	}
	else
		*pVal = SysAllocString (L"");
	
	m_bstrValue = *pVal;
	
	//*pVal = m_bstrValue.Copy ();

	return S_OK;
}

/******************************************************************************/
STDMETHODIMP CNSEdit::put_Value(BSTR newVal)
{
	m_bstrValue = newVal;

	// Replace edit control with newly aligned one
	//CreateEditWindow ();

	if (::IsWindow (hEdit)) 
		::SetWindowText (hEdit, newVal);

	return S_OK;
}

/******************************************************************************/
STDMETHODIMP CNSEdit::get_MaxLength(int *pVal)
{
	*pVal = nMaxLength;

	return S_OK;
}

/******************************************************************************/
STDMETHODIMP CNSEdit::put_MaxLength(int newVal)
{
	nMaxLength = newVal;

	if (::IsWindow (hEdit)) ::SendMessage (hEdit, EM_LIMITTEXT, nMaxLength, 0);

	return S_OK;
}

/******************************************************************************/
STDMETHODIMP CNSEdit::get_Password(VARIANT_BOOL *pVal)
{
	*pVal = bPassword;

	return S_OK;
}

/******************************************************************************/
STDMETHODIMP CNSEdit::put_Password(VARIANT_BOOL newVal)
{
	bPassword = newVal;

	CreateEditWindow ();

	return S_OK;
}

/******************************************************************************/
STDMETHODIMP CNSEdit::SetFocus(VARIANT_BOOL bSelect /*=FASLE*/)
{
	int end, start;

	if (::IsWindow (hEdit))
	{
		if(bSelect){
			end = -1;
			start = 0;
		}
		else{
			end = ::GetWindowTextLength (hEdit);
			start = -1;
		}
		::SendMessage (hEdit, EM_SETSEL, start, end);
		::SetFocus (hEdit);

	}

	return S_OK;
}

/******************************************************************************/
STDMETHODIMP CNSEdit::get_Alignment(BSTR *pVal)
{
	*pVal = strAlignment.Copy ();

	return S_OK;
}

/******************************************************************************/
STDMETHODIMP CNSEdit::put_Alignment(BSTR newVal)
{
	strAlignment = newVal;

	// Replace edit control with newly aligned one
	CreateEditWindow ();

	return S_OK;
}

/******************************************************************************/
LRESULT CNSEdit::HandleCommand (WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	switch (wNotifyCode)
	{
	case EN_CHANGE:
		Fire_OnChange ();
		break;

	case EN_SETFOCUS:
		Fire_OnFocus ();
		break;

	case EN_KILLFOCUS:
		Fire_OnLostFocus ();
		break;
	}

	return 0;
}

/******************************************************************************/
STDMETHODIMP CNSEdit::ShowSIP(VARIANT_BOOL bShow)
{

	HKEY hRegKey;
	DWORD dwRetVal = 0, dwRetSize = sizeof(DWORD);
	DWORD dwType;
	BOOL bUseSystemSIP = TRUE;

	dwRetVal = RegOpenKeyEx(HKEY_CURRENT_USER, L"Software\\Symbol\\SymbolPB", 0, 0, &hRegKey);
	if (ERROR_SUCCESS == dwRetVal) {
		// If opened then query the value and close
		dwRetVal = RegQueryValueEx(hRegKey, L"Use System SIP", NULL, &dwType, (BYTE *) bUseSystemSIP, &dwRetSize);
		RegCloseKey(hRegKey);
	}
	
	//get the m_bUseSystemSIP  value
	if(!dwRetSize || dwRetVal)
		bUseSystemSIP = FALSE;
	
	if(!bUseSystemSIP)
		return SipShowIM (bShow ? SIPF_ON : SIPF_OFF);
	else
	{
		SIPINFO sipinfo;
		memset(&sipinfo, 0, sizeof(SIPINFO));
		sipinfo.cbSize = sizeof(SIPINFO);
		SipGetInfo(&sipinfo);

		
		if((sipinfo.fdwFlags & SIPF_ON && !bShow) || (!(sipinfo.fdwFlags & SIPF_ON) && bShow))
		{
			HMODULE hKeybdDriver = LoadLibrary(KEYBOARD_DLL);
			LPFNSETKEYSTATE pfnSetKeyState = NULL;

			if (hKeybdDriver)
			{
					pfnSetKeyState = (LPFNSETKEYSTATE)GetProcAddress(hKeybdDriver, SETKEYSTATE);
					if (pfnSetKeyState)
					{
						
						//pfnSetKeyState(UN_SHIFTED, 0, true);
					}
					CloseHandle(hKeybdDriver);
					hKeybdDriver = NULL;
					pfnSetKeyState = NULL;
			}


			HWND hwndsipbut = FindWindow(L"MS_SIPBUTTON", NULL);

			if(hwndsipbut)
			{
				SendMessage(::GetWindow(hwndsipbut, GW_CHILD), WM_LBUTTONDOWN, 1, 0xA0009);
				SendMessage(::GetWindow(hwndsipbut, GW_CHILD), WM_LBUTTONUP, 0, 0x5000B);
			}
		}

		return TRUE;
	}
}

STDMETHODIMP CNSEdit::put_Border(VARIANT_BOOL newVal)
{
	bBorder = newVal;

	CreateEditWindow ();

	return S_OK;
}


STDMETHODIMP CNSEdit::put_FontHeight(int newVal)
{
	m_iHeight = newVal;
	CreateEditWindow ();

	return S_OK;
}

STDMETHODIMP CNSEdit::put_FontBold(VARIANT_BOOL newVal)
{
	m_bBold = newVal;

	CreateEditWindow ();

	return S_OK;
}


STDMETHODIMP CNSEdit::get_FontBold(VARIANT_BOOL *pVal)
{
	*pVal = m_bBold;

	return S_OK;
}


STDMETHODIMP CNSEdit::put_FontUnderline(VARIANT_BOOL newVal)
{
	m_bUnderLine = newVal;

	CreateEditWindow ();

	return S_OK;
}


STDMETHODIMP CNSEdit::put_FontItalic(VARIANT_BOOL newVal)
{
	m_bItalic = newVal;

	CreateEditWindow ();

	return S_OK;
}

STDMETHODIMP CNSEdit::put_FontName(BSTR newVal)
{
	m_bstrFont = newVal;

	CreateEditWindow ();

	return S_OK;
}

STDMETHODIMP CNSEdit::put_Font(BSTR *newVal)
{

	CreateEditWindow ();

	return S_OK;
}

STDMETHODIMP CNSEdit::put_FontFixedPitch(VARIANT_BOOL newVal)
{
	if(newVal)
		m_bFixed = FIXED_PITCH;
	else
		m_bFixed = DEFAULT_PITCH;

	CreateEditWindow ();
	return S_OK;
}


STDMETHODIMP CNSEdit::SetSelection(int iStart, int iEnd)
{
	m_iEnd = iEnd;
	m_iStart = iStart;
	if (::IsWindow (hEdit)){
		::SendMessage( hEdit, EM_SETSEL, iStart, iEnd);
		::SetFocus (hEdit);
	}
	

	return S_OK;
}

STDMETHODIMP CNSEdit::get_SelStartPos(int *pVal)
{
	DWORD retval = 0;
 
	if (::IsWindow (hEdit)){
 		retval = ::SendMessage( hEdit, EM_GETSEL, (WPARAM)pVal, NULL);
		if(retval > 0){
			m_iEnd = HIWORD(retval);
			m_iStart = LOWORD(retval);
		}
	}

	return S_OK;
}

STDMETHODIMP CNSEdit::get_SelEndPos(int *pVal)
{
	DWORD retval = 0;
 
 	if (::IsWindow (hEdit))
 		retval = ::SendMessage( hEdit, EM_GETSEL, NULL, (LPARAM)pVal);
		if(retval > 0){
			m_iEnd = HIWORD(retval);
			m_iStart = LOWORD(retval);
		}
	return S_OK;
}


//DEL STDMETHODIMP CNSEdit::ReplaceSel(int iStart, int iEnd, BSTR strReplace)
//DEL {
//DEL 	if (::IsWindow (hEdit)){
//DEL  		::SendMessage( hEdit, EM_SETSEL, iStart, iEnd);
//DEL  		::SendMessage( hEdit, EM_REPLACESEL, (WPARAM)true, (LPARAM)strReplace);
//DEL  		::SetFocus (hEdit);
//DEL  		DWORD retval = ::SendMessage( hEdit, EM_GETSEL, NULL, NULL);
//DEL  		if(retval > 0){
//DEL  			m_iEnd = HIWORD(retval);
//DEL  			m_iStart = LOWORD(retval);
//DEL  		}
//DEL  	}
//DEL  
//DEL  	return S_OK;
//DEL }

HRESULT CNSEdit::InPlaceActivate(LONG iVerb, const RECT* /*prcPosRect*/)
{
	HRESULT hr;

	if (m_spClientSite == NULL)
		return S_OK;

	CComPtr<IOleInPlaceObject> pIPO;
	ControlQueryInterface(__uuidof(IOleInPlaceObject), (void**)&pIPO);
	ATLASSERT(pIPO != NULL);

	if (!m_bNegotiatedWnd)
	{
		if (!m_bWindowOnly)
			// Try for windowless site
			hr = m_spClientSite->QueryInterface(__uuidof(IOleInPlaceSiteWindowless), (void **)&m_spInPlaceSite);

		if (m_spInPlaceSite)
		{
			m_bInPlaceSiteEx = TRUE;
			// CanWindowlessActivate returns S_OK or S_FALSE
			if ( m_spInPlaceSite->CanWindowlessActivate() == S_OK )
			{
				m_bWndLess = TRUE;
				m_bWasOnceWindowless = TRUE;
			}
			else
			{
				m_bWndLess = FALSE;
			}
		}
		else
		{
			m_spClientSite->QueryInterface(__uuidof(IOleInPlaceSiteEx), (void **)&m_spInPlaceSite);
			if (m_spInPlaceSite)
				m_bInPlaceSiteEx = TRUE;
			else
				hr = m_spClientSite->QueryInterface(__uuidof(IOleInPlaceSite), (void **)&m_spInPlaceSite);
		}
	}

	ATLASSUME(m_spInPlaceSite);
	if (!m_spInPlaceSite)
		return E_FAIL;

	m_bNegotiatedWnd = TRUE;

	if (!m_bInPlaceActive)
	{

		BOOL bNoRedraw = FALSE;
		if (m_bWndLess)
			m_spInPlaceSite->OnInPlaceActivateEx(&bNoRedraw, ACTIVATE_WINDOWLESS);
		else
		{
			if (m_bInPlaceSiteEx)
				m_spInPlaceSite->OnInPlaceActivateEx(&bNoRedraw, 0);
			else
			{
				hr = m_spInPlaceSite->CanInPlaceActivate();
				// CanInPlaceActivate returns S_FALSE or S_OK
				if (FAILED(hr))
					return hr;
				if ( hr != S_OK )
				{
				   // CanInPlaceActivate returned S_FALSE.
				   return( E_FAIL );
				}
				m_spInPlaceSite->OnInPlaceActivate();
			}
		}
	}

	m_bInPlaceActive = TRUE;

	// get location in the parent window,
	// as well as some information about the parent
	//
	OLEINPLACEFRAMEINFO frameInfo;
	RECT rcPos, rcClip;
	CComPtr<IOleInPlaceFrame> spInPlaceFrame;
	CComPtr<IOleInPlaceUIWindow> spInPlaceUIWindow;
	frameInfo.cb = sizeof(OLEINPLACEFRAMEINFO);
	HWND hwndParent;
	if (m_spInPlaceSite->GetWindow(&hwndParent) == S_OK)
	{
		m_spInPlaceSite->GetWindowContext(&spInPlaceFrame,
			&spInPlaceUIWindow, &rcPos, &rcClip, &frameInfo);

		if (!m_bWndLess)
		{
			if (m_hWndCD)
				::ShowWindow(m_hWndCD, SW_SHOW);
			else
			{
				if (rcPos.right == rcPos.left && rcPos.bottom == rcPos.top)
				{
					rcPos = rcClip = m_rcPos;
				}

				HWND h = CreateControlWindow(hwndParent, rcPos);
				ATLASSERT(h != NULL);	// will assert if creation failed
				ATLASSERT(h == m_hWndCD);
				if(h == NULL)
					return E_FAIL;
			}
		}

		pIPO->SetObjectRects(&rcPos, &rcClip);
	}

	CComPtr<IOleInPlaceActiveObject> spActiveObject;
	ControlQueryInterface(__uuidof(IOleInPlaceActiveObject), (void**)&spActiveObject);

	// Gone active by now, take care of UIACTIVATE
	if (DoesVerbUIActivate(iVerb))
	{
		if (!m_bUIActive)
		{
			m_bUIActive = TRUE;
			hr = m_spInPlaceSite->OnUIActivate();
			if (FAILED(hr))
			{
				m_bUIActive = FALSE;
				return hr;
			}

			SetControlFocus(TRUE);
			// set ourselves up in the host.
			//
			if (spActiveObject)
			{
				if (spInPlaceFrame)
					spInPlaceFrame->SetActiveObject(spActiveObject, NULL);
				if (spInPlaceUIWindow)
					spInPlaceUIWindow->SetActiveObject(spActiveObject, NULL);
			}

			if (spInPlaceFrame)
				spInPlaceFrame->SetBorderSpace(NULL);
			if (spInPlaceUIWindow)
				spInPlaceUIWindow->SetBorderSpace(NULL);
		}
	}

	m_spClientSite->ShowObject();

	return S_OK;
}
