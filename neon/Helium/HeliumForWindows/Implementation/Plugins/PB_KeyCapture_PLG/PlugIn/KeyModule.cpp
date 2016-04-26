/******************************************************************************/
#include <windows.h>
#include "KeyModule.h"
#include "..\..\..\Common\Private\SIP.h"
#include "rcmcapi.h"
#include "..\..\..\Common\Public\PB_Defines.h"

/******************************************************************************/
pBrowserFunc CKeyModule::pBrowserHome;

/******************************************************************************/
CKeyModule::CKeyModule (PPBCORESTRUCT pPBCoreStructure)
{
	m_pPBCoreStructure = pPBCoreStructure;
	hTriggerEvent = INVALID_HANDLE_VALUE;
	hTriggerQuit = INVALID_HANDLE_VALUE;
	hTriggerThread = 0;
	hTriggerNotification = INVALID_HANDLE_VALUE;
	m_bMinimized = FALSE;
	m_hDisplayClass = NULL;
	m_pRcm = new CRcmLoader();
	if (m_pRcm && m_pRcm->LoadRcmDLL())
		m_bRcmLoaded = true;
	else
		m_bRcmLoaded = false;
}

/******************************************************************************/
CKeyModule::~CKeyModule ()
{
	StopTriggerWatch ();
	delete m_pRcm;
	m_pRcm = NULL;
}

/******************************************************************************/

typedef BOOL (__stdcall *UnregisterFunc1Proc)( UINT, UINT );

/**
*  \Author Darryn Campbell (JRQ768, DCC)
*  \Date February 2010 (DCC, Initial Creation, ported from PB2.22)
*/
BOOL CKeyModule::BlockHotKeys(HWND hwndToRegisterFor)
{
	//  This code should only be executed on Windows Mobile
	OSVERSIONINFO osvi;
	memset(&osvi, 0, sizeof(OSVERSIONINFO));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&osvi);

	//  condition on RhoElements to encompass CE6
	bool isWM = (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2) ||
			     (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1) ||
			     (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0) ||
				 (osvi.dwMajorVersion >= 6);
	if (isWM)
	{
		
		//  18/08/2011: DCC & GD: Removed this line and updated the help file, this was
		//                        disabling the windows behaviour.
		//  Additional Issue 11/11/11: Apparently the customers were not happy not being
		//  able to capture Function Keys, only set AllKeys(TRUE) if 
		LPCTSTR pAllKeysEnabled = 
				GetSettingPtr(L"Configuration\\DeviceKeys\\FunctionKeysCapturable", L"Value");
		if (pAllKeysEnabled != NULL && *pAllKeysEnabled != L'0')
		{
			AllKeys(TRUE);
		}
	
		const int NUM_F_KEYS = 24;
		const int NUM_APP_KEYS = 16;
		DWORD dwType = 0;
		DWORD dwIndex = 0;
		HKEY hKey = NULL;
		LONG lRes = 0;

		//set up some arrays to hold the dont block keys
		//and initialize them to zero
		DWORD dwNoBlockFKeys[NUM_F_KEYS];		//Function Keys  F1 to F24
		DWORD dwNoBlockAppKeys[NUM_APP_KEYS];	//App Keys  App1 to App16 
													//  (calendar, outlook etc.)

		memset(&dwNoBlockFKeys, 0, NUM_F_KEYS * sizeof(DWORD));
		memset(&dwNoBlockAppKeys, 0, NUM_APP_KEYS * sizeof(DWORD));

		//  Read the Config Settings for EnableFunctionKey_FX and
		//  EnableApplicationKey_AX to pupulate the two NoBlock arrays.
		//  Modify these if EnableFunctionKey_FX and EnableApplicationKey_AX 
		//  are moved.

		//  First the Function Keys
		TCHAR tcFunctionKeyTag[50];
		memset(tcFunctionKeyTag, 0, sizeof(TCHAR) * wcslen(tcFunctionKeyTag));
		LPCTSTR pFunctionKeyEnabled;
		for (int i = 1; i <= NUM_F_KEYS; i++)
		{
			wsprintf(tcFunctionKeyTag, L"Configuration\\DeviceKeys\\EnableFunctionKey_F%i", i);
			pFunctionKeyEnabled = 
				GetSettingPtr(tcFunctionKeyTag, L"Value");
			if (pFunctionKeyEnabled != NULL)
			{
				//  Configuration setting found for this function key, see if it
				//  is anything other than zero
				if (*pFunctionKeyEnabled != L'0')
				{
					//  Function key is specified not to be blocked
					dwNoBlockFKeys[i-1] = 1;
				}
			}
		}

		//  Now the Application Keys
		TCHAR tcApplicationKeyTag[50];
		memset(tcApplicationKeyTag, 0, sizeof(TCHAR) * wcslen(tcApplicationKeyTag));
		LPCTSTR pApplicationKeyEnabled;
		for (int i = 1; i <= NUM_APP_KEYS; i++)
		{
			wsprintf(tcApplicationKeyTag, L"Configuration\\DeviceKeys\\EnableApplicationKey_A%i", i);
			pApplicationKeyEnabled = 
				GetSettingPtr(tcApplicationKeyTag, L"Value");
			if (pApplicationKeyEnabled != NULL)
			{
			//  Configuration setting found for this Application key, see if it
				//  is anything other than zero
				if (*pApplicationKeyEnabled != L'0')
				{
					//  Application key is specified not to be blocked
					dwNoBlockAppKeys[i-1] = 1;
				}
			}
		}

		//  Which keys to left unblocked now reside in the two arrays, block 
		//  all other keys by registering them to Browser (they are 
		//  currently registered with the Operating System).  Note this will 
		//  persist across Browser starts but not across system boots.
		HINSTANCE hCoreDll;
		UnregisterFunc1Proc procUndergisterFunc;
		hCoreDll = LoadLibrary(_T("coredll.dll"));
			
		if(hCoreDll)
		{
			procUndergisterFunc = (UnregisterFunc1Proc)GetProcAddress(
				hCoreDll, _T("UnregisterFunc1"));
			if(procUndergisterFunc)
			{
				BOOL bRet;
				//  Iterate through the function keys
				for (int i=VK_F1; i<=VK_F24; i++) 
				{
					if(dwNoBlockFKeys[i-VK_F1])
						continue;

					//unregister the key then register it with our app
					procUndergisterFunc(0, i);
					procUndergisterFunc(MOD_WIN, i);	//  Necessary to block OK button
					UnregisterHotKey(0, i);
					bRet = RegisterHotKey(hwndToRegisterFor, i, 0, i);
				}

				//  Iterate through the app keys
				for (int i=0xc1; i<=0xcf; i++) 
				{
					//  if it's blocked then skip the block
					if(dwNoBlockAppKeys[i-0xc1])
						continue;

					//unregister the key then register it with our app
					procUndergisterFunc(0, i);
					UnregisterHotKey(0, i);
					bRet = RegisterHotKey(hwndToRegisterFor, i, 0, i);
				}
			}
			else
			{		
				LOG (PB_LOG_WARNING, L"Unable to load COREDLL");
				return FALSE;
			}
			FreeLibrary(hCoreDll);
		}
		else
		{
			LOG (PB_LOG_WARNING, L"Unable to load COREDLL");
			return FALSE;
		}
	}
	//  CE will just execute this line
	return TRUE;
}



/******************************************************************************/
BOOL CKeyModule::onInit(PPBSTRUCT pPBStructure)
{
	wcscpy (m_szModName, L"KeyCapture");
	RegisterForEvent (PB_APPFOCUSEVENT);
	RegisterForEvent (PB_BROWSER_BEFORE_NAV_EVENT);
	RegisterForEvent (PB_WINDOWCHANGED_EVENT);
	RegisterForEvent (RHO_APPFOCUS_EVENT);

	BlockHotKeys (pPBStructure->hWnd);

	return true;
}

/******************************************************************************/
void CKeyModule::onDeInit(PPBSTRUCT pPBStructure)
{
	UnRegisterForEvent (PB_APPFOCUSEVENT);
	UnRegisterForEvent (PB_BROWSER_BEFORE_NAV_EVENT);
	UnRegisterForEvent (PB_WINDOWCHANGED_EVENT);
	UnRegisterForEvent (PB_PRIMARY_MESSAGE);
	UnRegisterForEvent (RHO_APPFOCUS_EVENT);
}

/******************************************************************************/
BOOL CKeyModule::onAttachInstance(PPBSTRUCT pPBStructure, PPBINSTSTRUCT pInstStruct)
{
	LPCWSTR pvalue;
	WCHAR key, setting [64];
	int index;

	// Create new instance data
	INSTANCE_DATA *pdata = new INSTANCE_DATA;
	pdata->nID = pInstStruct->instID;
	pdata->pKeyMap = new CKeyMap ();
	pdata->psAllKeysNavigate = NULL;
	pdata->psTriggerNavigate = NULL;
	ClearAll (pdata, FALSE);

	// Get list of enabled control keys, start with empty list
	pdata->pControlKeys = new int [1];
	pdata->pControlKeys [0] = 0;

	// Count enabled control keys
	pdata->nControlKeys = 0;
	for (key = L'A'; key <= L'Z'; key++)
	{
		wsprintf (setting, L"DeviceKeys\\EnableCtrlKey_%c", key);
		pvalue = GetAppSettingPtr (pInstStruct->instID, setting, L"Value");
		if (pvalue && pvalue [0] == L'1')
			pdata->nControlKeys++;
	}

	// Allocate control key list, if needed, and fill it
	if (pdata->nControlKeys == 0)
		pdata->pControlKeys = NULL;
	else
	{
		pdata->pControlKeys = new int [pdata->nControlKeys];

		index = 0;
		for (key = L'A'; key <= L'Z'; key++)
		{
			wsprintf (setting, L"DeviceKeys\\EnableCtrlKey_%c", key);
			pvalue = GetAppSettingPtr (pInstStruct->instID, setting, L"Value");
			if (pvalue && pvalue [0] == L'1')
				pdata->pControlKeys [index++] = (int) key;
		}
	}
	
	pdata->bAppHasFocus = (pPBStructure->bLaunchingAppHasFocus == TRUE);

	// Pass to plugin framework
	pInstStruct->pWrappedObj = pdata;

	// Also keep a pointer to the isntance data for thread functions to use
	pInstanceData = pdata;

	// Register for keyboard messages for this window
	/*RegisterForMessage (WM_KEYDOWN);
	RegisterForMessage (WM_CHAR);
	RegisterForMessage (WM_KEYUP);*/
	RegisterForEvent (PB_PRIMARY_MESSAGE);
	RegisterForPrimaryMessage (WM_KEYDOWN);
	RegisterForPrimaryMessage (WM_CHAR);
	RegisterForPrimaryMessage (WM_KEYUP);
	

	ResetDefinition ();

	return TRUE;
}

/******************************************************************************/
BOOL CKeyModule::onReleaseInstance(PPBSTRUCT pPBStructure, PPBINSTSTRUCT pInstStruct)
{
	INSTANCE_DATA *pdata = (INSTANCE_DATA*) (pInstStruct->pWrappedObj);

	if (pdata)
	{
		if (pdata->pKeyMap)
			delete pdata->pKeyMap;

		// 'free' can accept a NULL pointer without error
		free (pdata->psAllKeysNavigate);
		free (pdata->psTriggerNavigate);

		if (pdata->pControlKeys)
			delete [] pdata->pControlKeys;

		delete pdata;
	}
	UnRegisterForPrimaryMessage (WM_KEYDOWN);
	UnRegisterForPrimaryMessage (WM_CHAR);
	UnRegisterForPrimaryMessage (WM_KEYUP);

	return TRUE;
}

/******************************************************************************/
BOOL CKeyModule::onBeforeNavigate (int iInstID)
{
	// Clear all settings so this page's tags can be processed

	// Get the instance data - problem if it's not available
	INSTANCE_DATA *pdata = (INSTANCE_DATA*) GetObjFromID (iInstID);
	if (!pdata)
	{
		LOG (PB_LOG_WARNING, L"Instance data not available");
		return FALSE;
	}

	ClearAll (pdata, TRUE);

	return TRUE;
}

/******************************************************************************/
BOOL CKeyModule::onAppFocus(int iOldID,int iNewID)
{
	// GD - no need for onAppFocus processing now that trigger is registered one time
	return TRUE;

	//  Ignore transitions to / from the license screen
	if (iOldID == PB_LICENCE_SCREEN || iNewID == PB_LICENCE_SCREEN)
		return TRUE;

	INSTANCE_DATA *pnew_data;

	// Get instance data for new app
	// GetObjFromID() will return NULL if we're activating an instance of a different plugin
	pnew_data = (INSTANCE_DATA*) GetObjFromID (iNewID);

	if (pnew_data)
	{
		// This is our app, start or stop trigger watch depending on navigate string present
		// (StartTriggerWatch() checks for already running)
		if (pnew_data->psTriggerNavigate)
			StartTriggerWatch ();
		else
			StopTriggerWatch ();

		// Hold copy of instance data for threads to use
		pInstanceData = pnew_data;
	}
	else
	{
		// This is another app, stop any active trigger watch
		StopTriggerWatch ();
	}

	return TRUE;
}
/******************************************************************************/
void CKeyModule::ClearAll (INSTANCE_DATA *pdata, BOOL bIsNavigation)
{
	pdata->pKeyMap->Clear ();

	if (pdata->psAllKeysNavigate)
	{
		free (pdata->psAllKeysNavigate);
		pdata->psAllKeysNavigate = NULL;
	}

	pdata->bAllKeysDispatch = FALSE;
	pdata->bSuppressChar = FALSE;

	if (pdata->psTriggerNavigate)
	{
		free (pdata->psTriggerNavigate);
		pdata->psTriggerNavigate = NULL;
	}

	if (!bIsNavigation)
	{
		pdata->nHomeKey = 0;
		pdata->nAccelerateMode = ACCELERATE_NORM;
	}
}

/******************************************************************************/
void CKeyModule::ResetDefinition (void)
{
	nKeyValue = INVALID_KEY;
	bDispatch = FALSE;
}

/******************************************************************************/
int CKeyModule::GetKeyCode (LPCWSTR pkey)
{
	unsigned long ul;
	LPWSTR perror;

	ul = wcstoul (pkey, NULL, 0);

	if (ul == 0 || ul == ULONG_MAX)
	{
		perror = new WCHAR [64];
		StringCchPrintf (perror, 64, L"Invalid key '%s'", pkey);
		LOG (PB_LOG_WARNING, perror);
		delete [] perror;

		return INVALID_KEY;
	}

	return (int) ul;
}

/******************************************************************************/
BOOL CKeyModule::MetaProc(PBMetaStruct *pbMetaStructure, PPBSTRUCT pPBStructure, void *pParam)
{	
	Log(PB_LOG_WARNING, L"The Key Capture API from the RhoElements Extension has been deprecated in 4.0, please transition your applications to use the common Key Capture API for future releases", _T(__FUNCTION__), __LINE__);

	LPWSTR perror;
	int key, remap;
	LPCWSTR pparameter, pvalue;
	CKeyMapEntry *pentry;

	INSTANCE_DATA *pdata = (INSTANCE_DATA*) pParam;

	if (!pdata)
	{
		LOG (PB_LOG_WARNING, L"pParam null");
		return FALSE;
	}

	pparameter = pbMetaStructure->lpParameter;
	pvalue = pbMetaStructure->lpValue;

	if (cmp (pparameter, L"keyvalue"))
	{
		// Get key code
		if (cmp (pvalue, L"all"))
			key = ALL_KEYS;
		else
			if ((key = GetKeyCode (pvalue)) == INVALID_KEY)
				return FALSE;

		// Store for later mapping creation
		nKeyValue = key;
	}
	else if (cmp (pparameter, L"remap"))
	{
		// Get key code
		if ((key = GetKeyCode (pvalue)) == INVALID_KEY)
			return FALSE;

		// Add entry to map
		pentry = pdata->pKeyMap->Find (nKeyValue);
		if (pentry)
			pentry->nRemap = key;
		else
		{
			pentry = new CKeyMapEntry (nKeyValue, key);
			pdata->pKeyMap->AddEntry (pentry);
		}
	}
	else if (cmp (pparameter, L"dispatch"))
	{
		// Store for later mapping creation
		bDispatch = cmp (pvalue, L"true");
	}
	else if (cmp (pparameter, L"triggerevent"))
	{
		// Store trigger navigate in instance data
		if (pdata->psTriggerNavigate)
			free (pdata->psTriggerNavigate);

		if (pvalue && wcslen(pvalue) > 0)
		{
			//  User has specified a value for the trigger event
			pdata->psTriggerNavigate = _wcsdup (pvalue);

			// Start watching for trigger event
			if (!StartTriggerWatch ())
				return FALSE;
		}
		else
		{
			//  User has cancelled the Trigger Event
			pdata->psTriggerNavigate = _wcsdup (L"");
			StopTriggerWatch();
		}
	}
	else if (cmp (pparameter, L"keyevent"))
	{
		// Check key value has been set previously
		if (nKeyValue == 0)
		{
			LOG (PB_LOG_WARNING, L"Key value not set");
			return FALSE;
		}

		if (nKeyValue == ALL_KEYS)
		{
			// Free any existing setting, store new one if any
			if (pdata->psAllKeysNavigate)
				free (pdata->psAllKeysNavigate);

			// TODO Change 1 to 0
			//if (wcslen (pvalue) > 0)
			if (!cmp(pvalue, L""))
			{
				pdata->psAllKeysNavigate = _wcsdup (pvalue);
				pdata->bAllKeysDispatch = bDispatch;
			}
			else
				pdata->psAllKeysNavigate = NULL;
		}
		else
		{
			// Add mapping with specified navigate target
			pentry = pdata->pKeyMap->Find (nKeyValue);
			if (pentry)
			{
				//  Test to see if empty navigate string provided
				if (cmp(pvalue, L""))
				{
					//  Delete the mapping
					pdata->pKeyMap->DeleteEntry(nKeyValue);
				}
				else
				{
					//  End DCC
					pentry->SetNavigate (pvalue);
					pentry->bDispatch = bDispatch;
					pentry->nRemap = 0;
				}
			}
			else
			{
				//  No Current mapping, check the value is valid and add the 
				//  mapping
				if (!cmp(pvalue, L""))
				{
					pentry = new CKeyMapEntry (nKeyValue, pvalue, bDispatch);
					pdata->pKeyMap->AddEntry (pentry);
				}
			}
		}

		// Reset
		ResetDefinition ();
	}
	else if (cmp (pparameter, L"homekeyvalue"))
	{
		if (!pvalue)
			return FALSE;

		if (cmp (pvalue, L"disabled") || cmp (pvalue, L"disable"))
			pdata->nHomeKey = 0;
		else if (cmp (pvalue, L"enabled") | cmp (pvalue, L"enable"))
			pdata->nHomeKey = 0x74;
		else
		{
			if ((key = GetKeyCode (pvalue)) == INVALID_KEY)
				return FALSE;

			pdata->nHomeKey = key;
		}
	}
	else if (cmp (pparameter, L"acceleratekey"))
	{
		if (cmp (pvalue, L"all"))
		{
			pdata->nAccelerateMode = ACCELERATE_ON;
			//  Inform the core of the new Accelerate setting
			if (m_pPBCoreStructure->pBrowserSetAccelerator)
				return m_pPBCoreStructure->pBrowserSetAccelerator(ACCELERATE_ON);
			else
				return FALSE;
		}
		else if (cmp (pvalue, L"norm"))
		{
			pdata->nAccelerateMode = ACCELERATE_NORM;
			//  Inform the core
			if (m_pPBCoreStructure->pBrowserSetAccelerator)
				return m_pPBCoreStructure->pBrowserSetAccelerator(ACCELERATE_NORM);
			else
				return FALSE;
		}
		else if (cmp (pvalue, L"none") || cmp (pvalue, L"off"))  //  Off to be backwardly compatible with EMML 1.0
		{
			pdata->nAccelerateMode = ACCELERATE_OFF;
			//   Inform the core
			if (m_pPBCoreStructure->pBrowserSetAccelerator)
				return m_pPBCoreStructure->pBrowserSetAccelerator(ACCELERATE_OFF);
			else
				return FALSE;
		}
		else
		{
			perror = new WCHAR [64];
			StringCchPrintf (perror, 64, L"Invalid accelerate setting '%s'", pvalue);
			LOG (PB_LOG_WARNING, perror);
			delete [] perror;

			return FALSE;
		}
	}

	// Handle backward-compatible versions
	else if (!wcsnicmp (pparameter, L"keyvalue", 8))
	{
		// Parameter begins with 'keyvalue' - treat rest of parameter as (hex) key value and value as navigate target
		if ((key = GetKeyCode (pparameter + 8)) == INVALID_KEY)
			return FALSE;

		// Check for remapping
		remap = 0;
		if (!wcsnicmp (pvalue, L"remapto", 7))
		{
			if ((remap = GetKeyCode (pvalue + 8)) == INVALID_KEY)
				return FALSE;
		}

		pentry = pdata->pKeyMap->Find (key);
		if (pentry)
		{
			pentry->SetNavigate (pvalue);
			pentry->bDispatch = FALSE;
			pentry->nRemap = remap;
		}
		else
		{
			if (remap)
				pentry = new CKeyMapEntry (key, remap);
			else
				pentry = new CKeyMapEntry (key, pvalue, FALSE);

			pdata->pKeyMap->AddEntry (pentry);
		}
	}
	else if (!wcsnicmp (pparameter, L"dispatch", 8))
	{
		// Parameter begins with 'dispatch' - treat rest of parameter as (hex) key value and value as navigate target
		if ((key = GetKeyCode (pparameter + 8)) == INVALID_KEY)
			return FALSE;

		pentry = pdata->pKeyMap->Find (key);
		if (pentry)
		{
			pentry->SetNavigate (pvalue);
			pentry->bDispatch = TRUE;
			pentry->nRemap = 0;
		}
		else
		{
			pentry = new CKeyMapEntry (key, pvalue, TRUE);
			pdata->pKeyMap->AddEntry (pentry);
		}
	}
	else if (cmp (pparameter, L"allkeys"))
	{
		if (pdata->psAllKeysNavigate)
			free (pdata->psAllKeysNavigate);

		if (!cmp(pvalue, L""))
		{
			pdata->psAllKeysNavigate = _wcsdup (pvalue);
			pdata->bAllKeysDispatch = FALSE;
		}
		else
		{
			//  Remove the OnAllKeys
			pdata->psAllKeysNavigate = NULL;
		}
	}
	else if (cmp (pparameter, L"allkeysdispatch"))
	{
		if (pdata->psAllKeysNavigate)
			free (pdata->psAllKeysNavigate);

		if (!cmp(pvalue, L""))
		{
			pdata->psAllKeysNavigate = _wcsdup (pvalue);
			pdata->bAllKeysDispatch = TRUE;
		}
		else
		{
			//  Remove the OnAllKeysDispatch
			pdata->psAllKeysNavigate = NULL;
		}
	}

	return TRUE;
}

/******************************************************************************/
BOOL CKeyModule::IsAccelerator (int key)
{
	switch (key)
	{
		case VK_LEFT:
		case VK_UP:
		case VK_RIGHT:
		case VK_DOWN:
		case VK_RETURN:
		case VK_TAB:
		case VK_BACK:
			return TRUE;
	}

	return FALSE;
}

/******************************************************************************/
BOOL IsControl ()
{
	SHORT state;

	state = GetKeyState (VK_CONTROL);
	return (state & 0x1000) != 0;
}

/******************************************************************************/
BOOL CKeyModule::ProcessKeyDown (WPARAM wparam, LPARAM lparam, INSTANCE_DATA *pdata)
{
	WCHAR key [8];
	BOOL handled, matched;
	int n;

	/* PRIORITIES
	Specific key
	All keys
	Home key
	Accelerator keys
	*/

	/*
	At load read ctrl key settings from config
	Check for ctrl key, if found then flag as handled by default - allow only if in config list
	*/

	// Get key code from message
	int code = (int) wparam;
	
	if(code == 229)
	{
		return false;
	}

	// Allow the CTRL key itself but intercept CTRL + key
	if (code != VK_CONTROL && IsControl ())
	{
		// Check for key in allowed control key list
		for (n = 0; n < pdata->nControlKeys; n++)
			if (pdata->pControlKeys [n] == code)
				return FALSE;

		// Not in list so mark as handled
		return TRUE;
	}

	// Assume key is not handled
	handled = FALSE;

	// Flag to show if we have found a match for the key
	matched = FALSE;

	// Convert to decimal text for passing to SendPBNavigate
	wsprintf (key, L"%d", code);

	// Check for specific key
	CKeyMapEntry *pentry = pdata->pKeyMap->Find (code);

	if (pentry)
	{
		if (pentry->nRemap)
		{
			// If it's remapped always mark it as handled and don't fire the event
			handled = TRUE;

			// Insert replacement key
			keybd_event (pentry->nRemap, 0, KEYEVENTF_SILENT, 0);
			keybd_event (pentry->nRemap, 0, KEYEVENTF_KEYUP | KEYEVENTF_SILENT, 0);
		}
		else
		{
			SendPBNavigate (tcKeyEventNames, pdata->nID, pentry->psNavigate, key, NULL);
			handled = !pentry->bDispatch;
		}

		matched = TRUE;
	}

	// Check for all keys
	if (!matched)
	{
		if (pdata->psAllKeysNavigate)
		{
			SendPBNavigate (tcKeyEventNames, pdata->nID, pdata->psAllKeysNavigate, key, NULL); 
			handled = !pdata->bAllKeysDispatch;
		}
	}

	// Check for home key
	if (!matched)
	{
		if (pdata->nHomeKey && pdata->nHomeKey == code)
		{
			// Call back to core to request home navigate
			(*pBrowserHome) (pdata->nID, m_szModName);

			handled = TRUE;
			matched = TRUE;
		}
	}

	// Flag that subsequent WM_CHAR should be suppressed if we are handling the key
	pdata->bSuppressChar = handled;

	return handled;
}

/******************************************************************************/
BOOL CKeyModule::ProcessChar (WPARAM wparam, LPARAM lparam, INSTANCE_DATA *pdata)
{
	// Remove key if flagged
	return pdata->bSuppressChar;
}

/******************************************************************************/
BOOL CKeyModule::ProcessKeyUp (WPARAM wparam, LPARAM lparam, INSTANCE_DATA *pdata)
{
	WCHAR key [8];
	BOOL handled, matched;	

	// Get key code from message
	int code = (int) wparam;	

	// Assume key is not handled
	handled = FALSE;

	if(code == VK_F11)
	{
		// Flag to show if we have found a match for the key
		matched = FALSE;

		// Convert to decimal text for passing to SendPBNavigate
		wsprintf (key, L"%d", code);

		// Check for specific key
		CKeyMapEntry *pentry = pdata->pKeyMap->Find (code);

		if (pentry)
		{
			if (pentry->nRemap)
			{
				// If it's remapped always mark it as handled and don't fire the event
				handled = TRUE;

				// Insert replacement key
				keybd_event (pentry->nRemap, 0, KEYEVENTF_SILENT, 0);
				keybd_event (pentry->nRemap, 0, KEYEVENTF_KEYUP | KEYEVENTF_SILENT, 0);
			}
			else
			{
				SendPBNavigate (tcKeyEventNames, pdata->nID, pentry->psNavigate, key, NULL);
				handled = !pentry->bDispatch;
			}

			matched = TRUE;
		}

		// Check for all keys
		if (!matched)
		{
			if (pdata->psAllKeysNavigate)
			{
				SendPBNavigate (tcKeyEventNames, pdata->nID, pdata->psAllKeysNavigate, key, NULL); 
				handled = !pdata->bAllKeysDispatch;
			}
		}

		// Check for home key
		if (!matched)
		{
			if (pdata->nHomeKey && pdata->nHomeKey == code)
			{
				// Call back to core to request home navigate
				(*pBrowserHome) (pdata->nID, m_szModName);

				handled = TRUE;
				matched = TRUE;
			}
		}

		// Flag that subsequent WM_CHAR should be suppressed if we are handling the key
		pdata->bSuppressChar = handled;
	}

	// Clear WM_CHAR suppression
	pdata->bSuppressChar = FALSE;

	return handled;
}

/******************************************************************************/
BOOL CKeyModule::onPrimaryMessage (MSG *pmsg, int ncurrid)
{
	INSTANCE_DATA *pdata = (INSTANCE_DATA*) GetObjFromID (ncurrid);
	if (!pdata)
	{
		//LOG (PB_LOG_WARNING, L"pobject null");
		return FALSE;
	}

	// ignore primary messages while we're in the background
	if (!pdata->bAppHasFocus)
		return FALSE;

	switch (pmsg->message)
	{
		case WM_KEYDOWN:
			return ProcessKeyDown (pmsg->wParam, pmsg->lParam, pdata);

		//  If the user has pressed the Red Phone key give focus back to 
		//  Browser rather than the today screen
		case WM_APP + 16388:
		case WM_APP + 16387:
		{
			//  On WM Simplified Chinese devices WM_APP + 16388 corresponds to 
			//  clicking in a text field and does not behave the same as other WM
			//  devices (localized or otherwise)
			if (pmsg->wParam != 305)
			{
				//  Providing PB is not minimized hide the today screen as it
				//  stole focus
				if (!m_bMinimized)
				{
					//  Previously the logic below was undertaken in before navigate,
					//  moving it here since it is not necessary to do this repeatedly.
					if (m_hDisplayClass == NULL)
					{
						m_hDisplayClass = FindWindow(DISPLAYCLASS, NULL);
						if (m_hDisplayClass == NULL)
							m_hDisplayClass = FindWindow(L"RhoElements", NULL);
					}
					SetForegroundWindow
						(m_hDisplayClass != NULL ? m_hDisplayClass : pmsg->hwnd);
					SetFocus
						(m_hDisplayClass != NULL ? m_hDisplayClass : pmsg->hwnd);
				}
			}
			return FALSE;
		}

		/*case WM_CHAR:
			return ProcessChar (wparam, lparam, pdata);*/

		case WM_KEYUP:
			{
				// Get key code from message
				int virtualKeyCode = (int) pmsg->wParam;
				if(virtualKeyCode == VK_F11)
				{
					return ProcessKeyUp (pmsg->wParam, pmsg->lParam, pdata);
				}
			}
	}

	return FALSE;
}

/******************************************************************************/
BOOL CKeyModule::onMessage (HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam, void *pobject)
{
	INSTANCE_DATA *pdata = (INSTANCE_DATA*) pobject;

	if (!pdata)
	{
		LOG (PB_LOG_WARNING, L"pobject null");
		return FALSE;
	}

	// ignore windows messages while we're in the background
	if (!pdata->bAppHasFocus)
		return FALSE;

	switch (msg)
	{
	case WM_KEYDOWN:
		return ProcessKeyDown (wparam, lparam, pdata);

	case WM_CHAR:
		return ProcessChar (wparam, lparam, pdata);

	case WM_KEYUP:
		return ProcessKeyUp (wparam, lparam, pdata);
	}

	// Shouldn't get here
	return FALSE;
}

/******************************************************************************/
BOOL CKeyModule::StartTriggerWatch (void)
{
	if (!m_bRcmLoaded)
	{
		Log(PB_LOG_WARNING, L"Trigger functionality is not available on this device", _T(__FUNCTION__), __LINE__);
		return FALSE;
	}

	DWORD error;
	LPWSTR perror;

	// Already started?
	if (hTriggerThread)
		return TRUE;

	// Create auto-reset event which system will signal when trigger occurs
	hTriggerEvent = CreateEvent (NULL, FALSE, FALSE, L"KeyTrigger");

	// Create event to signal thread to quit
	hTriggerQuit = CreateEvent (NULL, TRUE, FALSE, NULL);

	// Create thread
	hTriggerThread = CreateThread (NULL, 0, StaticTriggerProc, this, 0, NULL);

	// Register with system for trigger notifications
	if ((error = m_pRcm->lpfn_RCM_RegisterTriggerEvent (TRIGGER_ALL_MASK, FALSE, L"KeyTrigger", &hTriggerNotification)) != E_RCM_SUCCESS)
	{
		perror = new WCHAR [64];
		StringCchPrintf (perror, 64, L"RegisterTriggerEvent failed (%lu)", error);
		LOG (PB_LOG_WARNING, perror);
		delete [] perror;

		return FALSE;
	}

	LOG (PB_LOG_INFO, L"Trigger watch started");

	return TRUE;
}

/******************************************************************************/
void CKeyModule::StopTriggerWatch (void)
{
	if (!m_bRcmLoaded)
	{
		Log(PB_LOG_WARNING, L"Trigger functionality is not available on this device", _T(__FUNCTION__), __LINE__);
		return;
	}

	// Started?
	if (!hTriggerThread)
		return;

	// Unregister for notifications
	m_pRcm->lpfn_RCM_DeregisterTrigger (hTriggerNotification);

	// Stop thread and wait
	SetEvent (hTriggerQuit);
	if (WaitForSingleObject (hTriggerThread, 5000) == WAIT_TIMEOUT)
		TerminateThread (hTriggerThread, 0);
	CloseHandle (hTriggerThread);
	CloseHandle (hTriggerQuit);

	CloseHandle (hTriggerEvent);

	hTriggerThread = 0;

	LOG (PB_LOG_INFO, L"Trigger watch stopped");
}

/******************************************************************************/
void CKeyModule::TriggerProc (void)
{
	if (!m_bRcmLoaded)
	{
		Log(PB_LOG_WARNING, L"Trigger functionality is not available on this device", _T(__FUNCTION__), __LINE__);
		return;
	}

	WCHAR nav_param [16];
	DWORD status, error;
	LPWSTR perror;
	HANDLE wait [2];

	wait [0] = hTriggerQuit;
	wait [1] = hTriggerEvent;

	while (1)
	{
		if (WaitForMultipleObjects (2, wait, FALSE, INFINITE) == WAIT_OBJECT_0)
			break;

		// ignore trigger events if we're in the background
		if (!pInstanceData->bAppHasFocus)
			continue;

		if ((error = m_pRcm->lpfn_RCM_GetTriggerStatus (&status)) != E_RCM_SUCCESS)
		{
			perror = new WCHAR [64];
			StringCchPrintf (perror, 64, L"GetTriggerStatus failed (%lu)", error);
			LOG (PB_LOG_WARNING, perror);
			delete [] perror;

			continue;
		}

		// Send event to core if set
		if (pInstanceData->psTriggerNavigate)
		{
			wsprintf (nav_param, L"%lu", status);
			SendPBNavigate (tcTriggerEventNames, pInstanceData->nID, pInstanceData->psTriggerNavigate, nav_param, NULL);
		}
	}
}

/******************************************************************************/
DWORD CKeyModule::StaticTriggerProc (LPVOID pparam)
{
	CKeyModule *pthis = (CKeyModule*) pparam;
	pthis->TriggerProc ();

	return 0;
}

/**
*  \author	Darryn Campbell (DCC, JRQ768)
*  \date	April 2010 (Initial Creation)
*/
BOOL CKeyModule::onWindowChanged(int iEvent)
{
	//  Keep track of whether Browser is minimised or restored
	if (iEvent == PB_WINMINIMIZED)
	{
		//  Browser has been minimized
		m_bMinimized = TRUE;
	}
	else if (iEvent == PB_WINRESTORED)
	{
		//  Browser has been restored
		m_bMinimized = FALSE;
	}
	return TRUE;
}

BOOL CKeyModule::onRhoAppFocus(bool bActivate, int iInstID)
{
	INSTANCE_DATA *pdata = NULL;
	pdata = (INSTANCE_DATA*)GetObjFromID(iInstID);
	if (pdata != NULL)
	{
		pdata->bAppHasFocus = bActivate;
		return TRUE;
	}
	else
		return FALSE;
};

