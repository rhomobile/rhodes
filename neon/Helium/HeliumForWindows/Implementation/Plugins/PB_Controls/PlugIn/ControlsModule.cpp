/******************************************************************************/
#include <windows.h>
#include <stdarg.h>
#include "..\..\..\Common\Public\PBUtil.h"
#include "kbdapi.h"
#include "ControlsModule.h"

/******************************************************************************/
PBCORESTRUCT CControlsModule::PBCoreStruct;
WCHAR CControlsModule::szImageDirectory [];
int CControlsModule::nScreenWidth;
int CControlsModule::nScreenHeight;
int CControlsModule::nDefaultButtonSize;
int CControlsModule::dwKeyState;
int CControlsModule::dwCapsState;

/******************************************************************************/
CControlsModule::CControlsModule ()
{
	// Form default image directory
	StringCchCopy (szImageDirectory, COUNTOF(szImageDirectory) - 16, PBCoreStruct.szInstallDirectory);
	if (PBCoreStruct.bBuiltAsRhodesExtension)
		wcscat(szImageDirectory, L"Image\\");
	else
		wcscat (szImageDirectory, L"Image\\");

	// Get screen size
	nScreenWidth = GetSystemMetrics (SM_CXSCREEN);
	nScreenHeight = GetSystemMetrics (SM_CYSCREEN);
	nDefaultButtonSize = (nScreenHeight / 20) + 2;

	hKeyStateWindow = NULL;
	hKeyboardDriver = NULL;
}

/******************************************************************************/
CControlsModule::~CControlsModule ()
{
	LPFNUNREGKEYNOTIFY unreg_keystate;
	LPFNUNREGKEYBOARDNOTIFY unreg_keyboard;

	// Unregister for key state events
	if (hKeyboardDriver)
	{
		unreg_keystate = (LPFNUNREGKEYNOTIFY) GetProcAddress (hKeyboardDriver, UNREGKEYNOTIFY);
		if (unreg_keystate)
			(*unreg_keystate) (hKeyStateWindow);

		unreg_keyboard = (LPFNUNREGKEYBOARDNOTIFY) GetProcAddress (hKeyboardDriver, UNREGKEYBOARDNOTIFY);
		if (unreg_keyboard)
			(*unreg_keyboard) (hKeyStateWindow);

		FreeLibrary (hKeyboardDriver);
	}

	if (hKeyStateWindow)
		DestroyWindow (hKeyStateWindow);
}

/******************************************************************************/
void CControlsModule::WriteLog (LPCWSTR pfunction, int line, LPCWSTR pformat, ...)
{
	LPWSTR pmessage = new WCHAR [256];
	va_list args;
	va_start (args, pformat);
	StringCchVPrintf (pmessage, 256, pformat, args);
	Log (PB_LOG_WARNING, pmessage, pfunction, line);
	delete [] pmessage;
}

#define WRITELOG(format, ...) WriteLog (_T(__FUNCTION__), __LINE__, format, __VA_ARGS__)

/******************************************************************************/
void CControlsModule::SetControlType (LPCWSTR type)
{
	if (cmp (type, L"quitbutton"))
		nControlType = CT_QUITBUTTON;	
	else if (cmp (type, L"reloadbutton"))
		nControlType = CT_RELOADBUTTON;
	else if (cmp (type, L"addressbar"))
		nControlType = CT_ADDRESSBAR;
	else if (cmp (type, L"topcommandarea"))
		nControlType = CT_TOPCOMMANDAREA;
	else if (cmp (type, L"bottomcommandarea"))
		nControlType = CT_BOTTOMCOMMANDAREA;
	else if (cmp (type, L"backbutton"))
		nControlType = CT_BACKBUTTON;
	else if (cmp (type, L"forwardbutton"))
		nControlType = CT_FORWARDBUTTON;
	else if (cmp (type, L"gobutton"))
		nControlType = CT_GOBUTTON;
	else if (cmp (type, L"homebutton"))
		nControlType = CT_HOMEBUTTON;
	else if (cmp (type, L"minimizebutton"))
		nControlType = CT_MINIMIZEBUTTON;
	else if (cmp (type, L"zoominpagebutton"))
		nControlType = CT_PAGEZOOMINBUTTON;
	else if (cmp (type, L"zoomoutpagebutton"))
		nControlType = CT_PAGEZOOMOUTBUTTON;
	else if (cmp (type, L"sipbutton"))
		nControlType = CT_SIPBUTTON;
	else if (cmp (type, L"stopbutton"))
		nControlType = CT_STOPBUTTON;
	else if (cmp (type, L"zoomtextbutton"))
		nControlType = CT_ZOOMTEXTBUTTON;
	else if (cmp (type, L"keystate"))
		nControlType = CT_KEYSTATE;
	else if (cmp (type, L"custombutton"))
		nControlType = CT_CUSTOMBUTTON;
	else if (cmp (type, L"scroll"))
		nControlType = CT_SCROLL;
	else if (cmp (type, L"battery"))
		nControlType = CT_BATTERY;
	else if (cmp (type, L"signal"))
		nControlType = CT_SIGNAL;
	else
		WRITELOG (L"Unknown control type: %s", type);
}

/******************************************************************************/
void CControlsModule::BringControlsToFront (INSTANCE_DATA *pdata)
{
	// Bring all controls to the front of the z-order except for the command areas
	// So they will be visible even if the command area is created after the buttons

	if (pdata->pQuitButton) BringWindowToTop (pdata->pQuitButton->GetWindow ());
	if (pdata->pReloadButton) BringWindowToTop (pdata->pReloadButton->GetWindow ());
	if (pdata->pAddressBar) BringWindowToTop (pdata->pAddressBar->GetWindow ());
	if (pdata->pBackButton) BringWindowToTop (pdata->pBackButton->GetWindow ());
	if (pdata->pForwardButton) BringWindowToTop (pdata->pForwardButton->GetWindow ());
	if (pdata->pGoButton) BringWindowToTop (pdata->pGoButton->GetWindow ());
	if (pdata->pHomeButton) BringWindowToTop (pdata->pHomeButton->GetWindow ());
	if (pdata->pMinimizeButton) BringWindowToTop (pdata->pMinimizeButton->GetWindow ());
	if (pdata->pPageZoomInButton) BringWindowToTop (pdata->pPageZoomInButton->GetWindow ());
	if (pdata->pPageZoomOutButton) BringWindowToTop (pdata->pPageZoomOutButton->GetWindow ());
	if (pdata->pSIPButton) BringWindowToTop (pdata->pSIPButton->GetWindow ());
	if (pdata->pStopButton) BringWindowToTop (pdata->pStopButton->GetWindow ());
	if (pdata->pZoomTextButton) BringWindowToTop (pdata->pZoomTextButton->GetWindow ());
	if (pdata->pKeyState) BringWindowToTop (pdata->pKeyState->GetWindow ());
	if (pdata->pScroll) BringWindowToTop (pdata->pScroll->GetWindow ());
	if (pdata->pBattery) BringWindowToTop (pdata->pBattery->GetWindow ());
	if (pdata->pSignal) BringWindowToTop (pdata->pSignal->GetWindow ());

	// Custom buttons?
}

/******************************************************************************/
BOOL CControlsModule::onInit(PPBSTRUCT pPBStructure)
{
	WNDCLASS wndclass;
	LPFNREGKEYNOTIFY reg_keystate;
	LPFNREGKEYBOARDNOTIFY reg_keyboard;

	wcscpy (m_szModName, MODULE_NAME);

	// Preload the Fusion library - it crashes if it's loaded after a previous navigation
	LoadLibrary (L"FusionPublicAPI.dll");

	//RegisterForEvent (PB_APPFOCUSEVENT);
	RegisterForEvent (PB_BROWSER_BEFORE_NAV_EVENT);
	RegisterForEvent (PB_BROWSER_DOC_COMPLETE_EVENT);
	RegisterForEvent (PB_PRIMARY_MESSAGE);
	RegisterForEvent (PB_WINDOWCHANGED_EVENT);

	// Create an invisible window to receive key state messages
	// Only a single window needed because there is only a single physical keyboard
	// Return TRUE on error as the user may not be using key state in any case
	memset (&wndclass, 0, sizeof wndclass);
	wndclass.lpfnWndProc = StaticKeyWindowProc;
	wndclass.hInstance = pPBStructure->hInstance;
	wndclass.lpszClassName = L"KeyStateEvent";
	RegisterClass (&wndclass);

	hKeyStateWindow = CreateWindow (L"KeyStateEvent", NULL, 0, 0, 0, 0, 0, NULL, (HMENU) 0, pPBStructure->hInstance, NULL);
	if (!hKeyStateWindow)
	{
		WRITELOG (L"Cannot create key state event window");
		return TRUE;
	}

	// Add a pointer to ourselves to the window
	SetWindowLong (hKeyStateWindow, GWL_USERDATA, (LONG) this);

	// Load the keyboard driver DLL
	hKeyboardDriver = LoadLibrary (KEYBOARD_DLL);
	if (!hKeyboardDriver)
	{
		WRITELOG (L"Cannot load keyboard driver DLL");
		return TRUE;
	}

	// Register for key state events
	reg_keystate = (LPFNREGKEYNOTIFY) GetProcAddress (hKeyboardDriver, REGKEYNOTIFY);
	if (reg_keystate)
		(*reg_keystate) (hKeyStateWindow, KEY_EVENT);

	reg_keyboard = (LPFNREGKEYBOARDNOTIFY) GetProcAddress (hKeyboardDriver, REGKEYBOARDNOTIFY);
	if (reg_keyboard)
		(*reg_keyboard) (hKeyStateWindow, KEYBOARD_EVENT);

	return true;
}

/******************************************************************************/
void CControlsModule::NotifyKeyState (DWORD state)
{
	PPBINSTSTRUCT pInstStruct;
	INSTANCE_DATA *pdata;

	// Notify KeyState controls in all instances of change in key state
	// Using internal structure of PBPlugin...
	for (pInstStruct = m_pInstHead; pInstStruct; pInstStruct = pInstStruct->pNext)
	{
		pdata = (INSTANCE_DATA*) pInstStruct->pWrappedObj;

		if (pdata && pdata->pKeyState)
			pdata->pKeyState->OnKeyStateChange (state);
	}
}

/******************************************************************************/
LRESULT CALLBACK CControlsModule::StaticKeyWindowProc (HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	CControlsModule *pthis;

	// Get a pointer to ourselves
	pthis = (CControlsModule*) GetWindowLong (hwnd, GWL_USERDATA);

	switch (message)
	{
	case KEYBOARD_EVENT:
		dwCapsState = wparam;
		pthis->NotifyKeyState (dwKeyState | dwCapsState);

		return 0;

	case KEY_EVENT:
		dwKeyState = wparam;
		pthis->NotifyKeyState (dwKeyState | dwCapsState);

		return 0;
	}

	return DefWindowProc (hwnd, message, wparam, lparam);
}

/******************************************************************************/
void CControlsModule::onDeInit(PPBSTRUCT pPBStructure)
{
	//UnRegisterForEvent (PB_APPFOCUSEVENT);
	UnRegisterForEvent (PB_BROWSER_BEFORE_NAV_EVENT);
	UnRegisterForEvent (PB_BROWSER_DOC_COMPLETE_EVENT);
	UnRegisterForEvent (PB_PRIMARY_MESSAGE);
	UnRegisterForEvent (PB_WINDOWCHANGED_EVENT);
}

/******************************************************************************/
BOOL CControlsModule::onAttachInstance(PPBSTRUCT pPBStructure, PPBINSTSTRUCT pInstStruct)
{
	// Create new instance data
	INSTANCE_DATA *pdata = new INSTANCE_DATA;
	pdata->nID = pInstStruct->instID;

	// Initialise pointers to controls
	pdata->pQuitButton = NULL;
	pdata->pReloadButton = NULL;
	pdata->pAddressBar = NULL;
	pdata->pTopCommandArea = NULL;
	pdata->pBottomCommandArea = NULL;
	pdata->pBackButton = NULL;
	pdata->pForwardButton = NULL;
	pdata->pGoButton = NULL;
	pdata->pHomeButton = NULL;
	pdata->pMinimizeButton = NULL;
	pdata->pPageZoomInButton = NULL;
	pdata->pPageZoomOutButton = NULL;
	pdata->pSIPButton = NULL;
	pdata->pStopButton = NULL;
	pdata->pZoomTextButton = NULL;
	pdata->pKeyState = NULL;
	pdata->pScroll = NULL;
	pdata->pBattery = NULL;
	pdata->pSignal = NULL;

	pdata->pCustomButtonList = new CControlList ();
	wcscpy (pdata->sControlID, L"");

	// Pass to plugin framework
	pInstStruct->pWrappedObj = pdata;

	// Register for scroll change notifications
	RegisterForMessage (PBM_SCROLL_CHANGE);

	return TRUE;
}

/******************************************************************************/
BOOL CControlsModule::onReleaseInstance(PPBSTRUCT pPBStructure, PPBINSTSTRUCT pInstStruct)
{
	INSTANCE_DATA *pdata = (INSTANCE_DATA*) (pInstStruct->pWrappedObj);

	if (pdata)
	{
		if (pdata->pQuitButton) delete pdata->pQuitButton;
		if (pdata->pReloadButton) delete pdata->pReloadButton;
		if (pdata->pAddressBar) delete pdata->pAddressBar;
		if (pdata->pTopCommandArea) delete pdata->pTopCommandArea;
		if (pdata->pBottomCommandArea) delete pdata->pBottomCommandArea;
		if (pdata->pBackButton) delete pdata->pBackButton;
		if (pdata->pForwardButton) delete pdata->pForwardButton;
		if (pdata->pGoButton) delete pdata->pGoButton;
		if (pdata->pHomeButton) delete pdata->pHomeButton;
		if (pdata->pMinimizeButton) delete pdata->pMinimizeButton;
		if (pdata->pPageZoomInButton) delete pdata->pPageZoomInButton;
		if (pdata->pPageZoomOutButton) delete pdata->pPageZoomOutButton;
		if (pdata->pSIPButton) delete pdata->pSIPButton;
		if (pdata->pStopButton) delete pdata->pStopButton;
		if (pdata->pZoomTextButton) delete pdata->pZoomTextButton;
		if (pdata->pKeyState) delete pdata->pKeyState;
		if (pdata->pScroll) delete pdata->pScroll;
		if (pdata->pBattery) delete pdata->pBattery;
		if (pdata->pSignal) delete pdata->pSignal;
		delete pdata->pCustomButtonList;

		delete pdata;
	}

	return TRUE;
}

/******************************************************************************/
BOOL CControlsModule::MetaProc(PBMetaStruct *pbMetaStructure, PPBSTRUCT pPBStructure, void *pParam)
{
	INSTANCE_DATA *pdata = (INSTANCE_DATA*) pParam;
	if (!pdata)
	{
		WRITELOG (L"pParam null");
		return FALSE;
	}

	LPCWSTR pparameter = pbMetaStructure->lpParameter;
	LPCWSTR pvalue = pbMetaStructure->lpValue;
	CControlListEntry *plist_entry;

	// Get CControl which we're operating on from the array in the instance data
	// Instantiate as necessary

	CControl *pcontrol = NULL;

	switch (nControlType)
	{
		case CT_QUITBUTTON:
			if (!pdata->pQuitButton)
				pdata->pQuitButton = new CQuitButton (pPBStructure->hInstance, pPBStructure->hWnd, pdata->nID);
			pcontrol = pdata->pQuitButton;
			break;

		case CT_RELOADBUTTON:
			if (!pdata->pReloadButton)
				pdata->pReloadButton = new CReloadButton (pPBStructure->hInstance, pPBStructure->hWnd, pdata->nID);
			pcontrol = pdata->pReloadButton;
			break;

		case CT_ADDRESSBAR:
			if (!pdata->pAddressBar)
			{
				pdata->pAddressBar = new CAddressBar (pPBStructure->hInstance, pPBStructure->hWnd, pdata->nID);

				// If there's a go button then pass the address bar to it
				if (pdata->pGoButton)
					pdata->pGoButton->SetAddressBar(pdata->pAddressBar);
			}
			pcontrol = pdata->pAddressBar;
			break;

		case CT_TOPCOMMANDAREA:
			if (!pdata->pTopCommandArea)
			{
				pdata->pTopCommandArea = new CTopCommandArea (pPBStructure->hInstance, pPBStructure->hWnd, pdata->nID);
				BringControlsToFront (pdata);
			}
			pcontrol = pdata->pTopCommandArea;
			break;

		case CT_BOTTOMCOMMANDAREA:
			if (!pdata->pBottomCommandArea)
			{
				pdata->pBottomCommandArea = new CBottomCommandArea (pPBStructure->hInstance, pPBStructure->hWnd, pdata->nID);
				BringControlsToFront (pdata);
			}
			pcontrol = pdata->pBottomCommandArea;
			break;

		case CT_BACKBUTTON:
			if (!pdata->pBackButton)
				pdata->pBackButton = new CBackButton (pPBStructure->hInstance, pPBStructure->hWnd, pdata->nID);
			pcontrol = pdata->pBackButton;
			break;

		case CT_FORWARDBUTTON:
			if (!pdata->pForwardButton)
				pdata->pForwardButton = new CForwardButton (pPBStructure->hInstance, pPBStructure->hWnd, pdata->nID);
			pcontrol = pdata->pForwardButton;
			break;

		case CT_GOBUTTON:
			if (!pdata->pGoButton)
			{
				pdata->pGoButton = new CGoButton (pPBStructure->hInstance, pPBStructure->hWnd, pdata->nID);

				// If there's an address bar then pass a pointer to it
				if (pdata->pAddressBar)
					pdata->pGoButton->SetAddressBar (pdata->pAddressBar);

				// If there's a stop button make sure it's under the go button
				if (pdata->pStopButton)
					SetWindowPos (pdata->pStopButton->GetWindow (), pdata->pGoButton->GetWindow (), 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
			}
			pcontrol = pdata->pGoButton;
			break;

		case CT_HOMEBUTTON:
			if (!pdata->pHomeButton)
				pdata->pHomeButton = new CHomeButton (pPBStructure->hInstance, pPBStructure->hWnd, pdata->nID);
			pcontrol = pdata->pHomeButton;
			break;

		case CT_MINIMIZEBUTTON:
			if (!pdata->pMinimizeButton)
				pdata->pMinimizeButton = new CMinimizeButton (pPBStructure->hInstance, pPBStructure->hWnd, pdata->nID);
			pcontrol = pdata->pMinimizeButton;
			break;

		case CT_PAGEZOOMINBUTTON:
			if (!pdata->pPageZoomInButton)
				pdata->pPageZoomInButton = new CPageZoomInButton (pPBStructure->hInstance, pPBStructure->hWnd, pdata->nID);
			pcontrol = pdata->pPageZoomInButton;
			break;

		case CT_PAGEZOOMOUTBUTTON:
			if (!pdata->pPageZoomOutButton)
				pdata->pPageZoomOutButton = new CPageZoomOutButton (pPBStructure->hInstance, pPBStructure->hWnd, pdata->nID);
			pcontrol = pdata->pPageZoomOutButton;
			break;

		case CT_SIPBUTTON:
			if (!pdata->pSIPButton)
				pdata->pSIPButton = new CSIPButton (pPBStructure->hInstance, pPBStructure->hWnd, pdata->nID);
			pcontrol = pdata->pSIPButton;
			break;

		case CT_STOPBUTTON:
			if (!pdata->pStopButton)
			{
				pdata->pStopButton = new CStopButton (pPBStructure->hInstance, pPBStructure->hWnd, pdata->nID);

				// If there's a go button make sure it's on top of the stop button
				if (pdata->pGoButton)
					SetWindowPos (pdata->pStopButton->GetWindow (), pdata->pGoButton->GetWindow (), 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
			}
			pcontrol = pdata->pStopButton;
			break;

		case CT_ZOOMTEXTBUTTON:
			if (!pdata->pZoomTextButton)
				pdata->pZoomTextButton = new CZoomTextButton (pPBStructure->hInstance, pPBStructure->hWnd, pdata->nID);
			pcontrol = pdata->pZoomTextButton;
			break;

		case CT_KEYSTATE:
			if (!pdata->pKeyState)
				pdata->pKeyState = new CKeyState (pPBStructure->hInstance, pPBStructure->hWnd, pdata->nID);
			pcontrol = pdata->pKeyState;
			break;

		case CT_SCROLL:
			if (!pdata->pScroll)
				pdata->pScroll = new CScroll (pPBStructure->hInstance, pPBStructure->hWnd, pdata->nID);
			pcontrol = pdata->pScroll;
			break;

		case CT_BATTERY:
			if (!pdata->pBattery)
				pdata->pBattery = new CBattery (pPBStructure->hInstance, pPBStructure->hWnd, pdata->nID);
			pcontrol = pdata->pBattery;
			break;

		case CT_SIGNAL:
			if (!pdata->pSignal)
			{
				pdata->pSignal = new CSignal (pPBStructure->hInstance, pPBStructure->hWnd, pdata->nID);

				// Check WLAN available
				if (!pdata->pSignal->OpenWLAN ())
				{
					delete pdata->pSignal;
					pdata->pSignal = NULL;

					WRITELOG (L"WLAN support not available");
					return TRUE;
				}
				
			}
			pcontrol = pdata->pSignal;
			break;

		case CT_CUSTOMBUTTON:
			// Record custom button ID
			if (cmp (pparameter, L"id"))
				WSAFECOPY (pdata->sControlID, pvalue);

			// Find button list entry for most recent ID
			plist_entry = pdata->pCustomButtonList->Find (pdata->sControlID);

			// Use existing control if found
			if (plist_entry)
			{
				pcontrol = plist_entry->pControl;
			}
			else
			{
				// Create new control if not found and add to list
				pcontrol = new CCustomButton (pPBStructure->hInstance, pPBStructure->hWnd, pdata->nID);
				pdata->pCustomButtonList->AddEntry (new CControlListEntry (pcontrol));
			}

			break;
	}

	if (!pcontrol)
	{
		WRITELOG (L"Control type not available: %d", nControlType);
		return FALSE;
	}

	if (cmp (pparameter, L"visibility"))
	{
		if (cmp (pvalue, L"visible"))
			return pcontrol->Show ();
		else if (cmp (pvalue, L"hidden"))
			return pcontrol->Hide ();
		else
		{
			WRITELOG (L"Unknown visibility value: %s", pvalue);
			return FALSE;
		}
	}
	else if (cmp (pparameter, L"id"))
	{
		return pcontrol->SetID (pvalue);
	}
	
	//JMS 5/5/2010
	//add check for null values on numeric parameters
	else if (cmp (pparameter, L"left"))
	{
		if(pvalue == NULL || pvalue[0] == NULL)
			return pcontrol->SetLeft (0);
		else
		{	// JS can pass in integer values as floats -> check if a string is a float and then transform it to int.
			if (isFloat(pvalue)) return pcontrol->SetLeft (_wtoi (pvalue));
			else
			{
				WRITELOG (L"Unknown left value: %s", pvalue);
				return FALSE;
			}
		}
	}
	else if (cmp (pparameter, L"right"))
	{
		if(pvalue == NULL || pvalue[0] == NULL)
			return pcontrol->SetRight (0);
		else
		{
			// JS can pass in integer values as floats -> check if a string is a float and then transform it to int.
			if (isFloat(pvalue)) return pcontrol->SetRight (_wtoi (pvalue));
			else
			{
				WRITELOG (L"Unknown right value: %s", pvalue);
				return FALSE;
			}
		}
	}
	else if (cmp (pparameter, L"top"))
	{
		if(pvalue == NULL || pvalue[0] == NULL)
			return pcontrol->SetTop (0);
		else
		{
			// JS can pass in integer values as floats -> check if a string is a float and then transform it to int.
			if (isFloat(pvalue)) return pcontrol->SetTop (_wtoi (pvalue));
			else
			{
				WRITELOG (L"Unknown top value: %s", pvalue);
				return FALSE;
			}
		}
	}
	else if (cmp (pparameter, L"width"))
	{
		if(pvalue == NULL || pvalue[0] == NULL)
			return pcontrol->SetWidth (0);
		else
		{
			// JS can pass in integer values as floats -> check if a string is a float and then transform it to int.
			if (isFloat(pvalue)) return pcontrol->SetWidth (_wtoi (pvalue));
			else
			{
				WRITELOG (L"Unknown width value: %s", pvalue);
				return FALSE;
			}
		}
	}
	else if (cmp (pparameter, L"height"))
	{
		if(pvalue == NULL || pvalue[0] == NULL)
			return pcontrol->SetHeight (0);
		else
		{
			// JS can pass in integer values as floats -> check if a string is a float and then transform it to int.
			if (isFloat(pvalue)) return pcontrol->SetHeight (_wtoi (pvalue));
			else
			{
				WRITELOG (L"Unknown height value: %s", pvalue);
				return FALSE;
			}
		}
	}
	

	else if (cmp (pparameter, L"image"))
		return pcontrol->SetImage (pvalue);
	else if (cmp (pparameter, L"imageup"))
		return pcontrol->SetImageUp (pvalue);
	else if (cmp (pparameter, L"imagedown"))
		return pcontrol->SetImageDown (pvalue);
	else if (cmp (pparameter, L"color") || cmp (pparameter, L"colour"))
		return pcontrol->SetColour (pvalue);
	else if (cmp (pparameter, L"click"))
		return pcontrol->SetClick (pvalue);
	else if (cmp (pparameter, L"border"))
	{
		if (cmp (pvalue, L"visible"))
			return pcontrol->SetBorder (TRUE);
		else if (cmp (pvalue, L"hidden"))
			return pcontrol->SetBorder (FALSE);
		else
		{
			WRITELOG (L"Unknown border value: %s", pvalue);
			return FALSE;
		}
	}
	else
	{
		// See if the control wants any non-standard parameter not handled above
		if (pcontrol->SetMiscValue (pparameter, pvalue))
			return TRUE;

		WRITELOG (L"Unknown parameter: %s", pparameter);
		return FALSE;
	}

	return TRUE;
}

/******************************************************************************/
BOOL CControlsModule::onBeforeNavigate (int iInstID)
{
	// Get the instance data - problem if it's not available
	INSTANCE_DATA *pdata = (INSTANCE_DATA*) GetObjFromID (iInstID);
	if (!pdata)
	{
		WRITELOG (L"Instance data not available");
		return FALSE;
	}

	if (pdata->pStopButton)
		pdata->pStopButton->Display(TRUE);

	if (pdata->pGoButton)
		pdata->pGoButton->Display (FALSE);

	// GD TEST
	if (pdata->pSignal)
		pdata->pSignal->OnBeforeNavigate ();

	if (pdata->pBattery)
		pdata->pBattery->OnBeforeNavigate ();
	// GD TEST

	return TRUE;
}

/******************************************************************************/
BOOL CControlsModule::onDocumentComplete (int iInstID)
{
	// Get the instance data - problem if it's not available
	INSTANCE_DATA *pdata = (INSTANCE_DATA*) GetObjFromID (iInstID);
	if (!pdata)
	{
		WRITELOG (L"Instance data not available");
		return FALSE;
	}

	if (pdata->pAddressBar)
		pdata->pAddressBar->OnNavigate ();

	if (pdata->pStopButton)
		pdata->pStopButton->Display(FALSE);

	if (pdata->pGoButton)
		pdata->pGoButton->Display (TRUE);

	return TRUE;
}

/******************************************************************************/
BOOL CControlsModule::onMessage (HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam, void *pobject)
{
	if (!pobject)
	{
		WRITELOG (L"Instance data not available");
		return FALSE;
	}
	INSTANCE_DATA *pdata = (INSTANCE_DATA*) pobject;

	/*
	// Send scroll message on to scroll control, to avoid threading problems
	if (pdata->pScroll)
		pdata->pScroll->PostScrollMessage (wparam, lparam);*/

	if (pdata->pScroll)
		pdata->pScroll->OnScrollChange (wparam, lparam);

	// Allow further processing
	return FALSE;
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		April 2010
*/
BOOL CControlsModule::onPrimaryMessage (MSG *pmsg, int ncurrid)
{

	INSTANCE_DATA *pdata = (INSTANCE_DATA*) GetObjFromID (ncurrid);
	if (!pdata)
	{
		return FALSE;
	}

	if (pdata->pAddressBar)
	{
		return pdata->pAddressBar->onPrimaryMessage(pmsg);
	}
	//  Do not block this message from proceeding further in the pump
	return FALSE;
}

/******************************************************************************/
BOOL CControlsModule::onWindowChanged (int iEvent)
{
	//  On some devices minimize does not work
	//  properly when command areas are shown.  This is because the command
	//  areas are being manipulated after minimize, return immediately here
	//  if the window changed event is minimize or restore
	if (iEvent == PB_WINRESTORED || iEvent == PB_WINMINIMIZED)
	{
		return TRUE;
	}
	// Window orientation has changed, get new values for width and height
	nScreenWidth = GetSystemMetrics (SM_CXSCREEN);
	nScreenHeight = GetSystemMetrics (SM_CYSCREEN);

	// Assume we're instance 0 as we don't get the instance id in this callback
	INSTANCE_DATA *pdata = (INSTANCE_DATA*) GetObjFromID (0);
	if (!pdata)
		return FALSE;

	DEBUGMSG(TRUE,(L"Screen now %d x %d\n",nScreenWidth,nScreenHeight));

	if (pdata->pTopCommandArea)
		pdata->pTopCommandArea->OnWindowChanged (iEvent);

	if (pdata->pBottomCommandArea)
		pdata->pBottomCommandArea->OnWindowChanged (iEvent);

	if (pdata->pTopCommandArea || pdata->pBottomCommandArea)
	{
		CCommandArea::ResizeHTML ();
		BringControlsToFront (pdata);
	}

	// Allow further processing
	return FALSE;
}
