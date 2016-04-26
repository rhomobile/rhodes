#include "JSPopup.h"

extern "C" const char* rho_native_get_appname();

BOOL	Log				(LogTypeInterface logSeverity,LPCTSTR pLogComment, 
						LPCTSTR pFunctionName, DWORD dwLineNumber,LPCTSTR pCallingModule);
//Sip is not shown while trying to enter text in fields of Authentication screen
// On Authentication window SIP has been shown now on webkit Engine
static WNDPROC m_hInputFieldProc_1;
static WNDPROC m_hInputFieldProc_2;
//  Constructor for Popup, also initialises variables
CJSPopup::CJSPopup(void* pMessageData, int type)
{
	m_pOtherData = NULL;
	m_hDialog = NULL;
	m_hOKButton = NULL;
	m_hCancelButton = NULL;
	m_hInputField = NULL;
	m_hInputField2 = NULL;
	m_hParentWnd = NULL;
	m_hTextFont = NULL;
	m_hButtonFont = NULL;
	m_hInst = NULL;
	m_iType = type;
	
	m_hInputFieldProc_1 = NULL;
	m_hInputFieldProc_2 = NULL;
	//  Some of the variable initialisation will depend on the type of dialog
	//  being created
	switch (type)
	{
	case(EEID_JSPOPUP_ALERT):
		{
			m_iButtonsCount = 1;	
			m_szMessageToDisplay = new WCHAR[wcslen((LPCTSTR)pMessageData) + 1];
			wcscpy(m_szMessageToDisplay, (LPCTSTR)pMessageData);
		}
		break;
	case(EEID_JSPOPUP_CONFIRM):
		{
		m_iButtonsCount = 2;	
		m_szMessageToDisplay = new WCHAR[wcslen((LPCTSTR)pMessageData) + 1];
		wcscpy(m_szMessageToDisplay, (LPCTSTR)pMessageData);
		}
		break;
	case (EEID_JSPOPUP_PROMPT):
		{
			m_iButtonsCount = 2;	
			EngineJSPopupPrompt* pPopupInfo;
			pPopupInfo = (EngineJSPopupPrompt*)pMessageData;
			m_szMessageToDisplay = new WCHAR[wcslen(pPopupInfo->tcMessage) + 1];
			wcscpy(m_szMessageToDisplay, pPopupInfo->tcMessage);
			m_pOtherData = pPopupInfo;
		}
		break;
	case (EEID_JSPOPUP_AUTHENTICATION):
		{
			m_iButtonsCount = 2;
			EngineJSPopupAuthentication* pPopupInfo;
			pPopupInfo = (EngineJSPopupAuthentication*)pMessageData;
			m_szMessageToDisplay = new WCHAR[wcslen(L"Please enter credentials") + 1];
			wcscpy(m_szMessageToDisplay, L"Please enter credentials");
			m_pOtherData = pPopupInfo;
		}
	}
}

CJSPopup::~CJSPopup()
{
	delete[] m_szMessageToDisplay;
}

//  Display the Dialog and block until the user presses OK or otherwise
//  dismisses the dialog.
LRESULT CJSPopup::ShowJSDialog(HWND hParentWnd)
{
	LRESULT lReturnValue = S_OK;
	m_hParentWnd = hParentWnd;
	LayoutDialog();
	WNDCLASS wcex;

	//register class window
	if (!GetClassInfo(m_hInst, L"REJSPopup", &wcex))
	{
		wcex.style			= CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc	= (WNDPROC)PopupWndProc;
		wcex.cbClsExtra		= 0;
		wcex.cbWndExtra		= 0;
		wcex.hInstance		= m_hInst;
		wcex.hIcon			= NULL;
		wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
		wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW + 1);
		wcex.lpszMenuName	= NULL;
		wcex.lpszClassName	= L"REJSPopup";
		if (RegisterClass(&wcex) == 0)
		{
			return S_FALSE;
		}
	}
		
	RECT r;
	GetWindowRect(GetDesktopWindow(), &r);

    const char* appName = rho_native_get_appname();
    wchar_t* appNameW = new wchar_t[strlen(appName)+1];

    int nSize = MultiByteToWideChar(CP_UTF8, 0, appName, -1, NULL, 0);
    if ( nSize > 1 )
    {
        MultiByteToWideChar(CP_UTF8, 0, appName, -1, &appNameW[0], nSize);
    }

	//create modal window (invisible at first, will be resized in WM_PAINT)
	m_hDialog = CreateWindowEx(NULL,
					L"REJSPopup", appNameW,
					WS_POPUP | WS_CAPTION , 
					100, 100, 1, 1,
					hParentWnd,	NULL, m_hInst, this);

    delete [] appNameW;

	if(m_hDialog == NULL)
		return S_FALSE;

	//  Add a pointer to ourselves to the Window
	SetWindowLong(m_hDialog, GWL_USERDATA, (LONG)this);

	//  If this is a prompt dialog, set the default text
	EngineJSPopupPrompt* pPopupInfoPrompt;
	EngineJSPopupAuthentication* pPopupInfoAuthentication;
	if (m_iType == EEID_JSPOPUP_PROMPT)
	{
		pPopupInfoPrompt = (EngineJSPopupPrompt*)m_pOtherData;
		SetWindowText(m_hInputField, pPopupInfoPrompt->tcDefaultValue);
	}
	else if (m_iType == EEID_JSPOPUP_AUTHENTICATION)
	{
		pPopupInfoAuthentication = (EngineJSPopupAuthentication*)m_pOtherData;
//		SetWindowText(m_hInputField, L"username");
//		SetWindowText(m_hInputField2, L"password");
	}

	//show the popup window and disable the browser beneath.
	SetForegroundWindow(m_hDialog);
	EnableWindow(m_hParentWnd, FALSE);
	ShowWindow(m_hDialog, SW_SHOW); 
	UpdateWindow(m_hDialog);

	MSG msg;
	HWND hWndFocused = 0;
	bool bAuthenticationError = false;

    while (GetMessage(&msg, NULL, 0, 0)) 
    {       
		if (msg.message == WM_KEYDOWN) 
		{
			//if pressed Cancel button
			if (msg.wParam == VK_ESCAPE)
            {
				SendMessage(m_hDialog , WM_DESTROY, 0, 0);
                lReturnValue = S_FALSE;
            }
			//if pressed Enter button on keyboard or clicked button on screen.
			if (msg.wParam == VK_RETURN)
            {
				hWndFocused = GetFocus();
				if (((hWndFocused == m_hInputField) || (hWndFocused == m_hInputField2))&& m_iType == EEID_JSPOPUP_AUTHENTICATION)
				{
					
					hWndFocused = m_hOKButton;
				}
				//send message
				if (hWndFocused == m_hOKButton)
				{
					//  Special behaviour for Prompts when OK is pressed is to 
					//  return the entered text back to the engine
					if (m_iType == EEID_JSPOPUP_PROMPT)
					{
						memset(pPopupInfoPrompt->tcValue, 0, pPopupInfoPrompt->valueLength * sizeof(WCHAR));
						int iUserDataLen = GetWindowTextLength(m_hInputField);
						iUserDataLen++;
						LPTSTR wcInputData = new WCHAR[iUserDataLen];
						//get the user entered text
						GetWindowText(m_hInputField, wcInputData, iUserDataLen);
						//verify whether the InputData is null or empty or provided some input
						if((wcInputData == NULL) || (_tcscmp(wcInputData,L"") == 0))
						{
							delete[] wcInputData;
							wcInputData = NULL;
							wcInputData = new WCHAR[wcslen(L" ") + 1];
							_tcscpy(wcInputData,L" ");
						}
						//verify the user data doesn't exceed the size set by the engine
						if (iUserDataLen >= pPopupInfoPrompt->valueLength)
						{
							//need to truncate the user data 
							wcsncpy(pPopupInfoPrompt->tcValue,wcInputData,pPopupInfoPrompt->valueLength-1);
						}
						else
							wcsncpy(pPopupInfoPrompt->tcValue,wcInputData,pPopupInfoPrompt->valueLength);
				
						delete[] wcInputData;

					}
					else if (m_iType == EEID_JSPOPUP_AUTHENTICATION)
					{
						bAuthenticationError = false;
						memset(pPopupInfoAuthentication->szUsername, 0, pPopupInfoAuthentication->iUsernameLength* sizeof(WCHAR));
						memset(pPopupInfoAuthentication->szPassword, 0, pPopupInfoAuthentication->iPasswordLength * sizeof(WCHAR));
						//  Get the enterred username
						unsigned int iUsernameLen = GetWindowTextLength(m_hInputField);
						iUsernameLen++;
						LPTSTR wcUsernameData = new WCHAR[iUsernameLen];
						GetWindowText(m_hInputField, wcUsernameData, iUsernameLen);
						if (iUsernameLen >= pPopupInfoAuthentication->iUsernameLength)
						{
							//  Username is too long
							bAuthenticationError = true;
						}
						else
							wcsncpy(pPopupInfoAuthentication->szUsername, wcUsernameData, pPopupInfoAuthentication->iUsernameLength);
						//  Get the user specified password
						unsigned int iPasswordLen = GetWindowTextLength(m_hInputField2);
						iPasswordLen++;
						LPTSTR wcPasswordData = new WCHAR[iPasswordLen];
						GetWindowText(m_hInputField2, wcPasswordData, iPasswordLen);
						if (iPasswordLen >= pPopupInfoAuthentication->iPasswordLength)
						{
							//  Password is too long
							bAuthenticationError = true;
						}
						else
							wcsncpy(pPopupInfoAuthentication->szPassword, wcPasswordData, pPopupInfoAuthentication->iPasswordLength);
						
						delete[] wcUsernameData;
						delete[] wcPasswordData;

					}
					//  As per EV: Looks strange to me.  So this messge will come again in your message loop. Is it by design ?
					PostMessage(m_hDialog , WM_KEYDOWN, VK_RETURN, 0);
					if (bAuthenticationError)
						lReturnValue = S_FALSE;
					else
						lReturnValue = S_OK;
				}
                else if (hWndFocused == m_hCancelButton)
				{
                    PostMessage(m_hDialog , WM_KEYDOWN, VK_ESCAPE, 0);
					lReturnValue = S_FALSE;
				}
				SendMessage(m_hDialog , WM_DESTROY, 0, 0);
            }
			if (msg.wParam == VK_TAB)
            {
				//  User pressed tab button, consideration is required here
				//  for the type of dialog being displayed
				hWndFocused = GetFocus();
				//  Only prompts / authentication will have an input field
				if (hWndFocused == m_hInputField && m_iType == EEID_JSPOPUP_PROMPT)
					SetFocusToControl(m_hOKButton);
				else if (hWndFocused == m_hInputField && m_iType == EEID_JSPOPUP_AUTHENTICATION)
					SetFocusToControl(m_hInputField2);
				if (hWndFocused == m_hInputField2)
					SetFocusToControl(m_hOKButton);
                if (hWndFocused == m_hOKButton)
				{
					//  All three dialog types have an OK button but alert 
					//  dialog has no where to do on tab so do not move focus
					if (m_iType != EEID_JSPOPUP_ALERT)
						SetFocusToControl(m_hCancelButton);
				}
				if (hWndFocused == m_hCancelButton)
				{
					//  Prompt and Confirm have cancel buttons but both have a 
					//  different next field on tab.
					if (m_iType == EEID_JSPOPUP_PROMPT || 
						m_iType == EEID_JSPOPUP_AUTHENTICATION)
						SetFocusToControl(m_hInputField);
					else
						SetFocusToControl(m_hOKButton);
				}
            }
	     }
        TranslateMessage(&msg);
		DispatchMessage(&msg);      
    }
	return lReturnValue;
}

void CJSPopup::SetFocusToControl(HWND hwndControl)
{
	//  Windows Mobile gives no visual indication that controls have
	//  focus so we will give that indication ourselves.
	//  Setting the Window style to WS_EX_STATICEDGE looks OK on both
	//  WM and CE (though I'm not a designer!)
	if (hwndControl == m_hInputField || hwndControl == m_hInputField2)
	{
		//  Only possible on Prompt dialogs, remove style from all buttons
		SetWindowLong(m_hOKButton, GWL_EXSTYLE, NULL);
		SetWindowLong(m_hCancelButton, GWL_EXSTYLE, NULL);
	}
	//  For OK / Cancel, remove the style from the opposing button
	else if (hwndControl == m_hCancelButton)
		SetWindowLong(m_hOKButton, GWL_EXSTYLE, NULL);
	else if (hwndControl == m_hOKButton)
		SetWindowLong(m_hCancelButton, GWL_EXSTYLE, NULL);

	//  No special styling required for the input field, it already has a different style
	if (hwndControl != m_hInputField)
		SetWindowLong(hwndControl, GWL_EXSTYLE, WS_EX_STATICEDGE);
	if (hwndControl != m_hInputField2)
		SetWindowLong(hwndControl, GWL_EXSTYLE, WS_EX_STATICEDGE);
	//  Need to SetWindowPos to get SetWindowLong to have any effect here.
	SetWindowPos(m_hOKButton, NULL, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE);
	SetWindowPos(m_hCancelButton, NULL, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE);
	//  Finally actually give the requested control focus
	SetFocus(hwndControl);
}

//  Move the controls around, depending on the type of dialog and size
//  of the screen.  Also does further variable initialisation.
BOOL CJSPopup::LayoutDialog()
{
    LOGFONT lfont;
	m_hInst = GetModuleHandle(NULL);
    memset(&lfont, 0, sizeof(lfont));
	//  Font attributes common for all text on the dialog
    lstrcpy(lfont.lfFaceName, _T("Tahoma"));
    lfont.lfWeight = FW_NORMAL;
    lfont.lfItalic = FALSE;
    lfont.lfCharSet = DEFAULT_CHARSET;
    lfont.lfOutPrecision = OUT_DEFAULT_PRECIS;
    lfont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
    lfont.lfQuality = DEFAULT_QUALITY;
    lfont.lfPitchAndFamily = DEFAULT_PITCH;
	//  Determine component sizes based on the screen size
	m_iInputBoxHeight = 0;
	m_iInputBoxTopMargin = 0;
	m_iInputBox2Height = 0;
	m_iInputBox2TopMargin = 0;
	int iScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	if (iScreenWidth < 480)
	{
		//  Smaller than VGA
	    lfont.lfHeight = 16;
		m_iDialogWidth = iScreenWidth - 20;
		m_iDialogLeft = 10;
		m_iButtonWidth = 90;
		m_iButtonHeight = 25;
		m_iGapBetweenButtons = 10;
		m_iButtonVerticalGap = 10;
		m_iTextLeftMargin = 10;
		m_iTextTopMargin = 10;
		if (m_iType == EEID_JSPOPUP_PROMPT || 
			m_iType == EEID_JSPOPUP_AUTHENTICATION)
		{
			m_iInputBoxHeight = lfont.lfHeight + 4;
			m_iInputBoxTopMargin = 10;
		}
		if (m_iType == EEID_JSPOPUP_AUTHENTICATION)
		{
			m_iInputBox2Height = lfont.lfHeight + 4;
			m_iInputBox2TopMargin = 10;
		}
		m_iInputBoxHorizontalMargin = 10;
	}
	else
	{
		//  VGA
	    lfont.lfHeight = 28;
		m_iDialogWidth = 400;
		m_iDialogLeft = ((iScreenWidth - m_iDialogWidth) / 2);
		m_iButtonWidth = 100;
		m_iButtonHeight = 40;
		m_iGapBetweenButtons = 20;
		m_iButtonVerticalGap = 20;
		m_iTextLeftMargin = 10;
		m_iTextTopMargin = 10;
		if (m_iType == EEID_JSPOPUP_PROMPT ||
			m_iType == EEID_JSPOPUP_AUTHENTICATION)
		{
			m_iInputBoxHeight = lfont.lfHeight + 12;
			m_iInputBoxTopMargin = 10;
		}
		if (m_iType == EEID_JSPOPUP_AUTHENTICATION)
		{
			m_iInputBox2Height = lfont.lfHeight + 12;
			m_iInputBox2TopMargin = 10;
		}
		m_iInputBoxHorizontalMargin = 10;
	}
	//  Font used in OK / Cancel buttons is different
	//  from the font used in the rest of the dialog
	m_hTextFont = CreateFontIndirect(&lfont);
	lfont.lfWeight = FW_BOLD;
	m_hButtonFont = CreateFontIndirect(&lfont);
	m_iLeftmostButtonStart = (m_iDialogWidth - (m_iButtonsCount * m_iButtonWidth) - 
		((m_iButtonsCount - 1) * m_iGapBetweenButtons)) / 2;
	m_iCaptionHeight = GetSystemMetrics(SM_CYCAPTION);
	m_iScreenHeight = GetSystemMetrics(SM_CYSCREEN);
	return TRUE;
}

LRESULT CALLBACK CJSPopup::PopupWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	//  For all calls other than WM_CREATE the associated CJSPopup* is stored
	//  in the user data area of the window
	CJSPopup* pDialog = (CJSPopup*)GetWindowLong(hwnd, GWL_USERDATA);
	if (!pDialog && message != WM_CREATE)
		return S_FALSE;

	switch (message) 
	{
		case WM_CREATE:
			{
				//  We pass a pointer to the current dialog object when we create
				//  the window
				pDialog = (CJSPopup*)(((LPCREATESTRUCT)lParam)->lpCreateParams);
				//  Create the Input box for Prompt dialogs
				if (pDialog->m_iType == EEID_JSPOPUP_PROMPT ||
					pDialog->m_iType == EEID_JSPOPUP_AUTHENTICATION)
				{
					pDialog->m_hInputField = CreateWindowEx(WS_EX_CLIENTEDGE,
						L"edit", L"",
						WS_VISIBLE | WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL, 
						0, 0, 0, 0, 
						hwnd, NULL, pDialog->m_hInst, NULL);				

					//  Add a pointer to ourselves to the Window and set the font
					SetWindowLong(pDialog->m_hInputField, GWL_USERDATA, (LONG)pDialog);
					SendMessage(pDialog->m_hInputField, WM_SETFONT, (WPARAM)(pDialog->m_hTextFont), 0);
					m_hInputFieldProc_1= (WNDPROC)SetWindowLong(pDialog->m_hInputField, GWL_WNDPROC, (LONG)EditWndProc_1);
					TCHAR szWindowText[MAX_PATH];
					ZeroMemory(szWindowText,sizeof(szWindowText));
					_stprintf(szWindowText,TEXT("%s - %d"),((m_hInputFieldProc_1!=NULL)?TEXT("Valid"):TEXT("Error")),GetLastError());
					Log(PB_LOG_ERROR,szWindowText,L"windowproc",__LINE__,L"Core");

				}

				//  Create a second input box for the authentication password
				//  Setting password field to mask input
				if (pDialog->m_iType == EEID_JSPOPUP_AUTHENTICATION)
				{
					pDialog->m_hInputField2 = CreateWindowEx(WS_EX_CLIENTEDGE,
						L"edit", L"",
						WS_VISIBLE | WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL | ES_PASSWORD, 
						0, 0, 0, 0, 
						hwnd, NULL, pDialog->m_hInst, NULL);				

					//  Add a pointer to ourselves to the Window and set the font
					SetWindowLong(pDialog->m_hInputField2, GWL_USERDATA, (LONG)pDialog);
					SendMessage(pDialog->m_hInputField2, WM_SETFONT, (WPARAM)(pDialog->m_hTextFont), 0);
					m_hInputFieldProc_2= (WNDPROC)SetWindowLong(pDialog->m_hInputField2, GWL_WNDPROC, (LONG)EditWndProc_2);
					TCHAR szWindowText[MAX_PATH];
					ZeroMemory(szWindowText,sizeof(szWindowText));
					_stprintf(szWindowText,TEXT("%s - %d"),((m_hInputFieldProc_2!=NULL)?TEXT("Valid"):TEXT("Error")),GetLastError());
					Log(PB_LOG_ERROR,szWindowText,L"windowproc",__LINE__,L"Core");

				}

				//  OK Button, this is present on every dialog type
				pDialog->m_hOKButton = CreateWindowEx(NULL,
					L"button",L"OK",
					WS_VISIBLE | WS_CHILD | WS_TABSTOP, 
					0, 0, 0, 0, 
					hwnd, NULL, pDialog->m_hInst, NULL);				

				//  Add a pointer to ourselves to the Window and set the font.
				SetWindowLong(pDialog->m_hOKButton, GWL_USERDATA, (LONG)pDialog);
				SendMessage(pDialog->m_hOKButton, WM_SETFONT, (WPARAM)(pDialog->m_hButtonFont), 0);

				// Cancel Button, this is not present on the Alert dialog
				if (pDialog->m_iType == EEID_JSPOPUP_PROMPT ||
					pDialog->m_iType == EEID_JSPOPUP_CONFIRM ||
					pDialog->m_iType == EEID_JSPOPUP_AUTHENTICATION)
				{
					pDialog->m_hCancelButton = CreateWindowEx(NULL,
						L"button",L"Cancel",
						WS_VISIBLE | WS_CHILD | WS_TABSTOP, 
						0, 0, 0, 0, 
						hwnd, NULL, pDialog->m_hInst, NULL); 

					//  Add a pointer to ourselves to the Window and set the font.
					SetWindowLong(pDialog->m_hCancelButton, GWL_USERDATA, (LONG)pDialog);
					SendMessage(pDialog->m_hCancelButton, WM_SETFONT, (WPARAM)(pDialog->m_hButtonFont), 0);
				}
				
				//  Control to intiaially get focus depends on the type of dialog
				if (pDialog->m_iType == EEID_JSPOPUP_PROMPT ||
					pDialog->m_iType == EEID_JSPOPUP_AUTHENTICATION)
					pDialog->SetFocusToControl(pDialog->m_hOKButton);
				else
					pDialog->SetFocusToControl(pDialog->m_hOKButton);
			}
      		break;
		case WM_DESTROY:
			DeleteObject(pDialog->m_hTextFont);
			DeleteObject(pDialog->m_hButtonFont);

			EnableWindow(pDialog->m_hParentWnd, TRUE);
			SetForegroundWindow(pDialog->m_hParentWnd);
			DestroyWindow(hwnd);
			PostQuitMessage(0);
			break;
		case WM_PAINT:
			{
				//  Paint routine draws the user message on the dialog and moves the
				//  controls as appropriate.  User is prevented from moving the dialog around.
				PAINTSTRUCT ps;
				HDC hdc = BeginPaint(hwnd, &ps);
				pDialog->ResizePopupDialog(hwnd, hdc, pDialog);
				HFONT hfOld = (HFONT)SelectObject(hdc, pDialog->m_hTextFont);
				SelectObject(hdc, hfOld);
				DeleteDC (hdc);
				EndPaint(hwnd, &ps);
			}
			break;

        case WM_COMMAND:
            switch (HIWORD(wParam))
            {
                case BN_CLICKED:
                    if ((HWND)lParam == pDialog->m_hOKButton)
                        PostMessage(pDialog->m_hDialog , WM_KEYDOWN, VK_RETURN, 0);
                    if ((HWND)lParam == pDialog->m_hCancelButton)
                        PostMessage(pDialog->m_hDialog , WM_KEYDOWN, VK_ESCAPE, 0);
                    break;
            }
            break;

		default:
			return DefWindowProc(hwnd, message, wParam, lParam);
   }
   return 0;
}

//  Resizes the popup dialog taking into account the height of the message being displayed.
void CJSPopup::ResizePopupDialog(HWND hwnd, HDC hdc, CJSPopup* pDialog)
{
	RECT rect;
	GetClientRect (hwnd, &rect);
	rect.left += pDialog->m_iTextLeftMargin;
	rect.top += pDialog->m_iTextTopMargin;
	int iMessageHeight = DrawText(hdc, pDialog->m_szMessageToDisplay, 
		-1, &rect, DT_NOCLIP | DT_WORDBREAK | DT_END_ELLIPSIS | DT_WORD_ELLIPSIS | DT_NOPREFIX);
	iMessageHeight += pDialog->m_iTextTopMargin;
	//  'Complex' maths to move all the components, allows flexibility of 
	//  screen sizes and dialog types...  Sorry it's not too clear!
	int iDialogHeight = pDialog->m_iCaptionHeight + 
		iMessageHeight + pDialog->m_iButtonHeight + 
		(pDialog->m_iButtonVerticalGap * 2) + 
		(pDialog->m_iInputBoxTopMargin) + pDialog->m_iInputBoxHeight + 
		(pDialog->m_iInputBox2TopMargin) + pDialog->m_iInputBox2Height;
	MoveWindow(hwnd, pDialog->m_iDialogLeft, 
		(pDialog->m_iScreenHeight - iDialogHeight) / 2, 
		pDialog->m_iDialogWidth, 
		iDialogHeight, true);
	//  Move the Input field (not all dialogs have an input field)
	if (pDialog->m_hInputField)
	{
		MoveWindow(pDialog->m_hInputField,
			pDialog->m_iInputBoxHorizontalMargin,
			iMessageHeight + pDialog->m_iInputBoxTopMargin,
			pDialog->m_iDialogWidth - (2 * pDialog->m_iInputBoxHorizontalMargin),
			pDialog->m_iInputBoxHeight, true);
	}
	//  Move the second Input field (not all dialogs have a second input field)
	if (pDialog->m_hInputField2)
	{
		MoveWindow(pDialog->m_hInputField2,
			pDialog->m_iInputBoxHorizontalMargin,
			iMessageHeight + pDialog->m_iInputBoxTopMargin + 
			pDialog->m_iInputBoxHeight + pDialog->m_iInputBox2TopMargin,
			pDialog->m_iDialogWidth - (2 * pDialog->m_iInputBoxHorizontalMargin),
			pDialog->m_iInputBox2Height, true);
	}
	//  Move the OK and Cancel Buttons (Not all dialogs have a cancel button)
	MoveWindow(pDialog->m_hOKButton, 
		pDialog->m_iLeftmostButtonStart, 
		iMessageHeight + pDialog->m_iButtonVerticalGap + 
		(pDialog->m_iInputBoxTopMargin) + pDialog->m_iInputBoxHeight + 
		(pDialog->m_iInputBox2TopMargin) + pDialog->m_iInputBox2Height, 
		pDialog->m_iButtonWidth, 
		pDialog->m_iButtonHeight, true);
	if (pDialog->m_hCancelButton)
	{
		MoveWindow(pDialog->m_hCancelButton, 
			pDialog->m_iLeftmostButtonStart + 
				pDialog->m_iButtonWidth + pDialog->m_iGapBetweenButtons, 
			iMessageHeight + pDialog->m_iButtonVerticalGap + 
			(pDialog->m_iInputBoxTopMargin) + pDialog->m_iInputBoxHeight + 
			(pDialog->m_iInputBox2TopMargin) + pDialog->m_iInputBox2Height, 
			pDialog->m_iButtonWidth, 
			pDialog->m_iButtonHeight, true);
	}
}
// Sip is not shown while trying to enter text in fields of Authentication screen
// On Authentication window SIP has been shown now on webkit Engine
void CJSPopup::ShowSIPWnd(BOOL bShow)
{
	CSIP* m_pSip = NULL;
		if (!m_pSip)
			m_pSip = new CSIP();

	if(!bShow)
	{
		HWND sipHWND = FindWindow(L"SipWndClass", NULL);
		if (sipHWND && IsWindowVisible(sipHWND))
		{
			m_pSip->ToggleSIP();
		}
	}
	if(bShow)
	{
		bool bSIPVisible = false;
		HWND sipHWND = FindWindow(L"SipWndClass", NULL);
		if (sipHWND && IsWindowVisible(sipHWND))
			bSIPVisible = true;
		if(!bSIPVisible)
		{
			m_pSip->ToggleSIP();
			
		}
	}
	

}

LRESULT CALLBACK CJSPopup::EditWndProc_1(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
			CJSPopup* pDialog = (CJSPopup*)GetWindowLong(GetParent(hwnd), GWL_USERDATA);

			switch(message)
			{
			case WM_SETFOCUS:
				{
					pDialog->ShowSIPWnd(TRUE);
				}
				break;
			case WM_KILLFOCUS:
				{
					pDialog->ShowSIPWnd(FALSE);
				}
				break;
			}
			
			return CallWindowProc(m_hInputFieldProc_1, hwnd, message, wParam, lParam); 
}

LRESULT CALLBACK CJSPopup::EditWndProc_2(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
			CJSPopup* pDialog = (CJSPopup*)GetWindowLong(GetParent(hwnd), GWL_USERDATA);

			switch(message)
			{
			case WM_SETFOCUS:
				{
					pDialog->ShowSIPWnd(TRUE);
				}
				break;
			case WM_KILLFOCUS:
				{
					pDialog->ShowSIPWnd(FALSE);
				}
				break;
			}
			
			return CallWindowProc(m_hInputFieldProc_2, hwnd, message, wParam, lParam); 
}
