#include "Keyboard.h"
#include "sipapi.h"
#include "keybd.h"
namespace rho {
//#pragma comment (lib,"aygshell.lib")

/**
*  \author Darryn Campbell (DCC, JRQ768)
*  \date February 2010 (Initial Creation, DCC)
*/
void HideSIPButton()
{
	HWND hg_sipbut = FindWindow(L"MS_SIPBUTTON", NULL);
	if (hg_sipbut)
		ShowWindow(hg_sipbut, SW_HIDE);
}

/**
*  \author Dave Sheppard (DS)
*  \author Darryn Campbell (DCC, JRQ768)
*  \date March 2005 (First Created, DS)
*  \date November 2009 (Converted from static to non-static.  Made Compatible
*                       with PocketBrowser 3.0)
*/
CSIP::CSIP()
{
	currentStatus = SIP_CONTROL_MANUAL;
	InitSIP();
}

CSIP::~CSIP()
{
	// GD Use FreeLibrary instead of CloseHandle
	FreeLibrary(hKeybdDriver);
	//CloseHandle(hKeybdDriver);

	hKeybdDriver = NULL;
	pfnSetKeyState = NULL;
}


/**
*  \author Dave Sheppard (DS)
*  \author Darryn Campbell (DCC, JRQ768)
*  \date March 2005 (First Created, DS)
*  \date November 2009 (Converted from static to non-static.  Made Compatible
*                       with PocketBrowser 3.0)
*/
void CSIP::CloseSIP()
{
	ResetSIP();
}


/**
*  \author Dave Sheppard (DS)
*  \author Darryn Campbell (DCC, JRQ768)
*  \date March 2005 (First Created, DS)
*  \date November 2009 (Converted from static to non-static.  Made Compatible
*                       with PocketBrowser 3.0)
*/
void CSIP::InitSIP()
{
	l_bEscape = FALSE;
//	m_bSipControl = false;
	m_bAllowSIP = true;
	m_bSIPOn = false;
	m_bTitleShown = false;

	//reset the key state to unshifted
	hKeybdDriver = LoadLibrary(KEYBOARD_DLL);
	pfnSetKeyState = NULL;

	if (hKeybdDriver)
	{
		pfnSetKeyState = (LPFNSETKEYSTATE)GetProcAddress(hKeybdDriver, SETKEYSTATE);
		if (pfnSetKeyState)
		{
			//EMBPD00180707
			//pfnSetKeyState(UN_SHIFTED, 0, true);
		}
	}
	
	// Find the SIP window
	hSipWnd = FindWindow(L"SipWndClass", NULL);	
	if (hSipWnd) {
		// Find and store the original position
		GetWindowRect(hSipWnd, &m_rcOriginalPos);

		m_rcPosition = m_rcOriginalPos;
	
		// Get the style
		m_dwOriginalStyle = GetWindowLong(hSipWnd, GWL_STYLE);
		//  DCC (16/06/2012) Removing the SIP caption is not compatible with
		//  CE7, it gives undesired results, specifically when resizing the SIP
		//  following removal of the caption and also moving the SIP where the
		//  task bar used to be is a bad idea.  This seems unrelated to debug 
		//  buttons (as previously observed with other OS') and is particularly
		//  evident when toggling the SIP.  Conceivably this could be made a config
		//  option if required, I know the Nano team were asking for this at one point.
		OSVERSIONINFO osvi;
		memset(&osvi, 0, sizeof(OSVERSIONINFO));
		osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
		GetVersionEx(&osvi);
		bool bIsCE7 = (osvi.dwMajorVersion == 7);
		if (!bIsCE7)
		{
			if (m_dwOriginalStyle & WS_CAPTION) {
				m_bTitleShown = true;
				// It has a caption bar so must be CE .NET
				// Now see if its a big one
				m_rcPosition.top += 26;		// Move the SIP down by the CAPTION size
				if (m_rcOriginalPos.right - m_rcOriginalPos.left > 242) 
					m_rcPosition.bottom = m_rcPosition.top+194;
				else 
					m_rcPosition.bottom = m_rcPosition.top+80; 
				LONG lStyle = m_dwOriginalStyle ^ WS_CAPTION;	
				SetWindowLong(hSipWnd, GWL_STYLE, lStyle);	// Remove the CAPTION
			}
			//else m_rcPosition.bottom++;			// Else its WM2003 so add one line of pixels to the bottom
		}
		else
			DEBUGMSG(true, (L"Device is CE7, not hiding SIP caption"));

		// Put the Window in the correct place at the correct size
		MoveWindow(hSipWnd, m_rcPosition.left, m_rcPosition.top, m_rcPosition.right-m_rcPosition.left, m_rcPosition.bottom-m_rcPosition.top, TRUE);
	}
//	m_bSipControl = RetrieveSIPControlConfigValue(hSipWnd);

}

/**
*  \author Dave Sheppard (DS)
*  \author Darryn Campbell (DCC, JRQ768)
*  \date March 2005 (First Created, DS)
*  \date November 2009 (Converted from static to non-static.  Made Compatible
*                       with PocketBrowser 3.0)
*/
void CSIP::ResetSIP()
{
	if (hSipWnd) SetWindowLong(hSipWnd, GWL_STYLE, m_dwOriginalStyle);
	MoveWindow(hSipWnd, m_rcOriginalPos.left, m_rcOriginalPos.top, m_rcOriginalPos.right-m_rcOriginalPos.left, m_rcOriginalPos.bottom-m_rcOriginalPos.top, TRUE);

}

void CSIP::ShowTitle(bool bShowTitle)
{
	//  If the SIP didn't have a title bar when we launched then do nothing
	if (!m_bTitleShown)
	{
		ResetSIP();
		return;
	}

	HWND sipHWND = FindWindow(L"SipWndClass", NULL);
	RECT rOriginalPos;
	GetWindowRect(sipHWND, &rOriginalPos);
	DWORD currentStyle = GetWindowLong(sipHWND, GWL_STYLE);

	if ((currentStyle & WS_CAPTION) && (!bShowTitle))
	{
		//  If there is currently a caption and we're being asked to hide the title
		//  then hide it
		MoveWindow(sipHWND, rOriginalPos.left, rOriginalPos.top + SIP_TITLE_HEIGHT, rOriginalPos.right-rOriginalPos.left, 
			rOriginalPos.bottom-(rOriginalPos.top + SIP_TITLE_HEIGHT), TRUE);
		SetWindowLong(sipHWND, GWL_STYLE, currentStyle ^ WS_CAPTION);
	}
	else if (!(currentStyle & WS_CAPTION) && bShowTitle)
	{
		//  There is currently NOT a caption and we are being asked to
		//  show it
		MoveWindow(sipHWND, rOriginalPos.left, rOriginalPos.top - SIP_TITLE_HEIGHT, rOriginalPos.right-rOriginalPos.left, 
			rOriginalPos.bottom-(rOriginalPos.top - SIP_TITLE_HEIGHT), TRUE);
		SetWindowLong(sipHWND, GWL_STYLE, currentStyle | WS_CAPTION);
	}
	
}



////////////////////////////////////////////////////////////////////////
// Function:	SendKey
// Description:	Sends the Keystroke to the keyboard buffer.  Used by the
//				scanner and MSR to 'fake' keystrokes from the input devices
// Author:		Dave Sheppard
// Date:		2/3/2005
////////////////////////////////////////////////////////////////////////
void CSIP::SendKey(TCHAR cKey)
{
	BOOL bEscape = FALSE;
	int nHex = 0;
	BYTE bHex = 0;
	BOOL bVirtual = FALSE;
	const BYTE	CharToVk[256] =
		{
				//     0    1    2    3    4    5    6    7    8    9    A    B    C    D    E    F
				/*0*/ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x09,0x00,0x00,0x00,0x0D,0x00,0x00,
				/*1*/ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1B,0x00,0x00,0x00,0x00,
				/*2*/ 0x20,0x31,0xDE,0x33,0x34,0x35,0x37,0xDE,0x39,0x30,0x38,0xBB,0xBC,0xBD,0xBE,0xBF,
				/*3*/ 0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0xBA,0xBA,0xBC,0xBB,0xBE,0xBF,
				/*4*/ 0x32,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4A,0x4B,0x4C,0x4D,0x4E,0x4F,
				/*5*/ 0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5A,0xDB,0x5C,0xDD,0x36,0xBD,
				/*6*/ 0x60,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4A,0x4B,0x4C,0x4D,0x4E,0x4F,
				/*7*/ 0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5A,0xDB,0x5C,0xDD,0x60,0x60,
				/*8*/ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
				/*9*/ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
				/*A*/ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
				/*B*/ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
				/*C*/ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
				/*D*/ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
				/*E*/ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
				/*F*/ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
				};


	if ( l_bEscape )
	{
		if ( cKey == ESCAPE_CHAR )
		{
			bEscape = !bEscape;

			if ( bEscape )
			{
				return;
			}
		}
	}
	
	if ( bEscape )
	{
		bEscape = FALSE;

		switch(cKey)
		{
			case TEXT('b'):
				
				cKey = TEXT('\b');
				
				break;

			case TEXT('f'):
				
				cKey = TEXT('\f');
				
				break;

			case TEXT('n'):
				
				cKey = TEXT('\n');;
				
				break;

			case TEXT('r'):
				
				cKey = TEXT('\r');;
				
				break;

			case TEXT('t'):
				
				cKey = TEXT('\t');
				
				break;

			case TEXT('x'):
				
				bVirtual = FALSE;

				nHex = 1;
				
				bHex = 0;
				
				return;

			case TEXT('v'):
				
				bVirtual = TRUE;

				nHex = 1;
				
				bHex = 0;
				
				return;
		}
	}

	switch(nHex)
	{
		case 1:
		case 2:
			
			if ( iswxdigit(cKey) )
			{
				bHex *= 16;

				if ( iswdigit(cKey) )
				{
					bHex += (BYTE) ( cKey & 0xF );
				}
				else
				{
					bHex += (BYTE)( ( cKey &0xF ) + 9 );
				}

				if ( nHex == 1 )
				{
					nHex++;
				
					return;
				}
				else
				{
					nHex = 0;

					cKey = (TCHAR)bHex;

					break;
				}
			}

			nHex = 0;

			bVirtual = FALSE;
			
			return;
		
		default:
			
			break;
	}
	
	if ( bVirtual )
	{
		UINT State1[1] = {KeyStateDownFlag|KeyShiftNoCharacterFlag};
		UINT State2[1] = {KeyStateToggledFlag|KeyShiftNoCharacterFlag};
		UINT Shift1[1] = {KeyStateDownFlag|KeyShiftNoCharacterFlag};
		UINT Shift2[1] = {KeyShiftNoCharacterFlag};
		UINT Key1[1] = {0};
		UINT Key2[1] = {0};
		BYTE bVK;
		bVK = (BYTE)cKey;

		PostKeybdMessage((HWND) -1, bVK, State1[0], 0, Shift1, Key1);
		PostKeybdMessage((HWND) -1, bVK, State2[0], 0, Shift2, Key2);
	}
	else
	{
		UINT State1[1] = {KeyStateDownFlag};
		UINT State2[1] = {KeyStateToggledFlag|KeyShiftNoCharacterFlag};
		UINT Shift1[1] = {KeyStateDownFlag};
		UINT Shift2[1] = {KeyShiftNoCharacterFlag};
		UINT Key1[1] = {cKey};
		UINT Key2[1] = {0};
		BYTE bVK;
	
		bVK = CharToVk[(BYTE)cKey];

		PostKeybdMessage((HWND) -1, bVK, State1[0], 1, Shift1, Key1);
		PostKeybdMessage((HWND) -1, bVK, State2[0], 1, Shift2, Key2);
	}

	bVirtual = FALSE;
}

////////////////////////////////////////////////////////////////////////
// Function:	SendString
// Description:	Repeatedly calls the SendKey to send an entire data
//				string to the keyboard buffer.
// Author:		Dave Sheppard
// Date:		2/3/2005
////////////////////////////////////////////////////////////////////////
void CSIP::SendString(LPCTSTR lpszString)
{
	DWORD i;

	HWND hWndFocus = GetForegroundWindow();
	if ( lpszString == NULL )
	{
		return;
	}
	
	for(i=0;i<wcslen(lpszString);i++)
	{
		SendKey(lpszString[i]);
	}
}

////////////////////////////////////////////////////////////////////////
// Function:	ToggleSystemSIP
// Description:	Toggles the SIP state between ON and OFF.  Gets the window 
//				to the System suip then clicks it using a pretend stylus 
//				down and up
// Author:		James Morley-Smith
// Date:		10/3/2006
////////////////////////////////////////////////////////////////////////
BOOL CSIP::ToggleSystemSIP()
{

	//reset the keystate
	
	if (hKeybdDriver)
	{
		if (pfnSetKeyState)
		{
			//EMBPD00180707
			//pfnSetKeyState(UN_SHIFTED, 0, true);
		}
	}

	HWND hwndsipbut = FindWindow(L"MS_SIPBUTTON", NULL);

	if(hwndsipbut)
	{
		SendMessage(GetWindow(hwndsipbut, GW_CHILD), WM_LBUTTONDOWN, 1, 0xA0009);
		SendMessage(GetWindow(hwndsipbut, GW_CHILD), WM_LBUTTONUP, 0, 0x5000B);
		return TRUE;
	}
	
	return FALSE;
}

////////////////////////////////////////////////////////////////////////
// Function:	ToggleSIP
// Description:	Toggles the SIP state between ON and OFF.  Reads the current
//				one to decide on what to do.  Also, if ManualSIP is set it
//				reduces the SIP window size to 0,0 when hiding the SIP
// Author:		Dave Sheppard
// Date:		2/3/2005
////////////////////////////////////////////////////////////////////////
void CSIP::ToggleSIP()
{
	//first try the system sip (ie windows mobile)
	if(ToggleSystemSIP())
	{
		return;
	}

	//failing that we'll use the api

	SIPINFO sipinfo;
	HRESULT hr = 0;
	memset(&sipinfo, 0, sizeof(SIPINFO));
	sipinfo.cbSize = sizeof(SIPINFO);
	sipinfo.dwImDataSize = 0;

	SipGetInfo(&sipinfo);

	if(sipinfo.fdwFlags & SIPF_ON){
		SipShowIM(SIPF_OFF);				
	}
	else{
		SipShowIM(SIPF_ON);
	}
}


////////////////////////////////////////////////////////////////////////
// Function:	ShowSIP
// Description:	Does what it says on the tin
// Author:		Dave Sheppard
// Date:		2/3/2005
////////////////////////////////////////////////////////////////////////
void CSIP::ShowSIP(HWND hwndHTML, bool bDoShowSip)
{
	//  Removing references to AYGShell (tested and works on WM and CE)
//	if(bDoShowSip)
//		SHSipPreference(hwndHTML, SIP_UP);
		//SipShowIM(SIPF_ON);
	
	MoveWindow(hSipWnd, m_rcPosition.left, m_rcPosition.top, 
			m_rcPosition.right-m_rcPosition.left, m_rcPosition.bottom-m_rcPosition.top, TRUE);
	m_bAllowSIP = true;
	m_bSIPOn = true;
	//CSpbLog::Log(LOW,SPB_LOG_INFORMATION, L"Shown SIP", L"CSIP", DEVICE_ID, __LINE__); 
}



/**
*  \author Dave Sheppard (DS)
*  \author Darryn Campbell (DCC, JRQ768)
*  \date March 2005 (First Created, DS)
*  \date November 2009 (Converted from static to non-static.  Made Compatible
*                       with PocketBrowser 3.0)
*/
bool CSIP::MoveSip(int x, int y)
{
	int iTemp;
	SIPINFO sipinfo;
	memset(&sipinfo, 0, sizeof(SIPINFO));
	sipinfo.cbSize = sizeof(SIPINFO);
	sipinfo.dwImDataSize = 0;

	SipGetInfo(&sipinfo);
	
	if (!hSipWnd)
		return false;

	GetWindowRect(hSipWnd, &m_rcPosition);

	// Only alter the x value if not -9999
	if (x != -9999) {
		iTemp = m_rcPosition.right - m_rcPosition.left;
		m_rcPosition.left = x;
		m_rcPosition.right = m_rcPosition.left + iTemp;
	}
	
	// Only alter the y value if not -9999
	if (y != -9999) {
		iTemp = m_rcPosition.bottom - m_rcPosition.top;
		m_rcPosition.top = y;
		m_rcPosition.bottom = m_rcPosition.top + iTemp;
	}

	if (hSipWnd) 
		MoveWindow(hSipWnd, m_rcPosition.left, m_rcPosition.top, m_rcPosition.right-m_rcPosition.left, m_rcPosition.bottom-m_rcPosition.top, TRUE);
	

	else 
		return false;

	sipinfo.rcSipRect = m_rcPosition;

	SipSetInfo(&sipinfo);

	return true;
}


///////////////////////////////////////////
//
//  New Functions for PocketBrowser 3.0
//
///////////////////////////////////////////

/**
*  \author Darryn Campbell (DCC, JRQ768)
*  \date November 2009 (First Created)
*/
RECT CSIP::GetOriginalPosition()
{
	return m_rcOriginalPos;
}

/*
SIP_CONTROL_VALUES CSIP::GetSipControl()
{
	if (m_bSipControl)
		return SIP_CONTROL_MANUAL;
	else
		return SIP_CONTROL_AUTOMATIC;
}
*/

//////////////////////////////
//
//  Stubbed Functions
//
//////////////////////////////

/**
*  \author Darryn Campbell (DCC, JRQ768)
*  \date November 2009 (First Created)
*/
HWND CSIP::GetAnimWindow()
{
	return NULL;
}

/**
*  \author Darryn Campbell (DCC, JRQ768)
*  \date November 2009 (First Created)
*/
BOOL CSIP::GetUseSystemSIP()
{
	return TRUE;
}

/**
*  \author Darryn Campbell (DCC, JRQ768)
*  \date November 2009 (First Created)
*/
bool CSIP::RetrieveSIPControlConfigValue(HWND hwndSIP)
{
	bool returnVal = false;
	DWORD Type;
	DWORD RetSize = 999;
	HKEY hRegKey;
	if (ERROR_SUCCESS == RegOpenKeyEx(HKEY_CURRENT_USER, _T("Software\\Symbol\\SymbolPB"), 0, 0, &hRegKey)) 
	{
		DWORD dwManSIP = 0;
		RetSize = 999;
		RegQueryValueEx(hRegKey, _T("SIP Control"), NULL, &Type, (BYTE *) &dwManSIP, &RetSize);
		if((RetSize > 0) && (dwManSIP>0))
		{
			// Manual SIP is true so make the Window 0,0.
			returnVal = true;
			if (hwndSIP) 
				MoveWindow(hwndSIP, m_rcPosition.left, m_rcPosition.top, 0, 0, TRUE);
		}
		RegCloseKey(hRegKey);
	}
	return returnVal;
}

void CSIP::ToggleSIPReliably(bool bUserRequestToShow)
{
	bool bSIPVisible = false;
	HWND sipHWND = FindWindow(L"SipWndClass", NULL);
	if (sipHWND && IsWindowVisible(sipHWND))
		bSIPVisible = true;
	if (bUserRequestToShow)
	{
		if (!bSIPVisible)	
			ToggleSIP();
	}
	else
	{
		if (bSIPVisible)	
			ToggleSIP();
	}
}

}
