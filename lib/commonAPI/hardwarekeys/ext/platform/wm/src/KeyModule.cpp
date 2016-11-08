#include <windows.h>
#include "KeyModule.h"
#include "common/RhodesApp.h"
#include "common/RhoConf.h"
#include <algorithm>
extern "C" bool rho_wmimpl_get_function_key_enabled(int iFKey)
{
	//TODO TAU
	return false;
}
extern "C" bool rho_wmimpl_get_application_key_enabled(int iAKey)
{
	//TODO TAU
	return false;
}
extern "C" bool rho_wmimpl_get_function_keys_capturable()
{
	//TODO TAU
	return false;
}


CKeyModule::CKeyModule ()
{
	hTriggerEvent = INVALID_HANDLE_VALUE;
	hTriggerQuit = INVALID_HANDLE_VALUE;
	hTriggerThread = 0;
	hTriggerNotification = INVALID_HANDLE_VALUE;
	m_bOOPMessagesRegistered = false;
	m_pRcm = new CRcmLoader();
	if (m_pRcm && m_pRcm->LoadRcmDLL())
		m_bRcmLoaded = true;
	else
		m_bRcmLoaded = false;
}

CKeyModule::~CKeyModule ()
{
	StopTriggerWatch ();
	delete m_pRcm;
	m_pRcm = NULL;
}

typedef BOOL (__stdcall *UnregisterFunc1Proc)( UINT, UINT );

/**
*  \Author Darryn Campbell (JRQ768, DCC)
*  \Date February 2010 (DCC, Initial Creation)
*/
BOOL CKeyModule::BlockHotKeys(HWND hwndToRegisterFor)
{
	//  This code should only be executed on Windows Mobile
	OSVERSIONINFO osvi;
	memset(&osvi, 0, sizeof(OSVERSIONINFO));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&osvi);
	//  CE devices also have Function keys, extending this 
	//  condition on RhoElements to encompass CE6
	bool isWM = (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2) ||
			     (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1) ||
				 (osvi.dwMajorVersion >= 6);
	if (isWM)
	{
		//  Capturing F1 & F2 keys on MC9500.
		//  18/08/2011: DCC & GD: Removed this line and updated the help file, this was
		//                        disabling the windows behaviour.
		//  Additional Issue 11/11/11: Apparently the customers were not happy not being
		//  able to capture Function Keys, only set AllKeys(TRUE) if 
		if (rho_wmimpl_get_function_keys_capturable())
		{
			AllKeys(TRUE);
		}
		
		DWORD dwType = 0;
		DWORD dwIndex = 0;
		HKEY hKey = NULL;
		LONG lRes = 0;

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
					if(rho_wmimpl_get_function_key_enabled(i-VK_F1))
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
					if(rho_wmimpl_get_application_key_enabled(i-0xc1))
						continue;

					//unregister the key then register it with our app
					procUndergisterFunc(0, i);
					UnregisterHotKey(0, i);
					bRet = RegisterHotKey(hwndToRegisterFor, i, 0, i);
				}
			}
			else
			{		
				LOG(WARNING) + "Unable to load COREDLL";
				return FALSE;
			}
			FreeLibrary(hCoreDll);
		}
		else
		{
			LOG(WARNING) + "Unable to load COREDLL";
			return FALSE;
		}
	}
	//  CE will just execute this line
	return TRUE;
}

BOOL CKeyModule::Initialize()
{
	// Create new instance data
	INSTANCE_DATA *pdata = new INSTANCE_DATA;
	pdata->pKeyMap = new CKeyMap ();
	pdata->psAllKeysNavigate = NULL;
	pdata->psTriggerNavigate = NULL;
	ClearAll (pdata, FALSE);

	pdata->bAppHasFocus = true;

	// Also keep a pointer to the isntance data for thread functions to use
	pInstanceData = pdata;

	return TRUE;
}

//  This function is not (currently) used in RE4
BOOL CKeyModule::onBeforeNavigate (int iInstID)
{
	ClearAll (pInstanceData, TRUE);

	return TRUE;
}
void CKeyModule::ClearAll (INSTANCE_DATA *pdata, BOOL bIsNavigation)
{
	pdata->pKeyMap->Clear ();

	pdata->psAllKeysNavigate = blankCallback;

	pdata->bAllKeysDispatch = FALSE;
	pdata->bSuppressChar = FALSE;

	pdata->psTriggerNavigate = blankCallback;

	if (!bIsNavigation)
	{
		pdata->nHomeKey = 0;
	}
}

int CKeyModule::GetKeyCode (LPCWSTR pkey)
{
	if(*pkey== _T('\0'))
	{
		LOG(WARNING) + L"Key value passed is empty string";
		return KEY_EMPTYSTRING;
	}
	
	unsigned long ul;
	LPWSTR perror;

	ul = wcstoul (pkey, NULL, 0);

	if (ul == 0 || ul == ULONG_MAX)
	{
		perror = new WCHAR [64];
		StringCchPrintf (perror, 64, L"Invalid key '%s'", pkey);
		LOG(WARNING) + perror;
		delete [] perror;

		return INVALID_KEY;
	}

	return (int) ul;
}


//  Accelerator keys are deprecated in RE4, this function left for posterity
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



bool CKeyModule::ProcessKeyDown (WPARAM wparam, LPARAM lparam, INSTANCE_DATA *pdata)
{
	char key [8];
	bool handled, matched;

	// Get key code from message
	int code = (int) wparam;

	if(code==229)
	{
	LOG(INFO) + "KeyCode is invalid in Keydown for F11 key.Handle F11 in Keyup";
	return false;
	}
	// Assume key is not handled
	handled = FALSE;

	// Flag to show if we have found a match for the key
	matched = FALSE;

	// Convert to decimal text 
	sprintf (key, "%d", code);

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
			if (pentry->psNavigate.hasCallback())
			{
				rho::Hashtable<rho::String, rho::String> keyData;
				keyData.put("keyValue", key);
				pentry->psNavigate.set(keyData);
			}
			handled = !pentry->bDispatch;
		}

		matched = TRUE;
	}

	// Check for all keys
	if (!matched)
	{
		if (pdata->psAllKeysNavigate.hasCallback())
		{
			rho::Hashtable<rho::String, rho::String> keyData;
			keyData.put("keyValue", key);
			pdata->psAllKeysNavigate.set(keyData);
			handled = !pdata->bAllKeysDispatch;
		}
	}

	// Check for home key
	if (!matched)
	{
		if (pdata->nHomeKey && pdata->nHomeKey == code)
		{
			// Call back to core to request home navigate
			LOG(INFO) + "Home Key has been pressed, navigating to application start page";
			RHODESAPP().navigateToUrl(RHODESAPP().getStartUrl());

			handled = TRUE;
			matched = TRUE;
		}
	}

	// Flag that subsequent WM_CHAR should be suppressed if we are handling the key
	pdata->bSuppressChar = handled;

	return handled;
}

bool CKeyModule::ProcessChar (WPARAM wparam, LPARAM lparam, INSTANCE_DATA *pdata)
{
	// Remove key if flagged
	return pdata->bSuppressChar;
}

bool CKeyModule::ProcessKeyUp (WPARAM wparam, LPARAM lparam, INSTANCE_DATA *pdata)
{
	
	
	char key [8];
	bool handled, matched;

		// Assume key is not handled
	handled = FALSE;
	// Get key code from message
	int code = (int) wparam;


	if(code == VK_F11)
	{

	LOG(INFO) + "F11 Handled in Keyup";
	// Flag to show if we have found a match for the key
	matched = FALSE;

	// Convert to decimal text 
	sprintf (key, "%d", code);

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
			if (pentry->psNavigate.hasCallback())
			{
				rho::Hashtable<rho::String, rho::String> keyData;
				keyData.put("keyValue", key);
				pentry->psNavigate.set(keyData);
			}
			handled = !pentry->bDispatch;
		}

		matched = TRUE;
	}

	// Check for all keys
	if (!matched)
	{
		if (pdata->psAllKeysNavigate.hasCallback())
		{
			rho::Hashtable<rho::String, rho::String> keyData;
			keyData.put("keyValue", key);
			pdata->psAllKeysNavigate.set(keyData);
			handled = !pdata->bAllKeysDispatch;
		}
	}

	// Check for home key
	if (!matched)
	{
		if (pdata->nHomeKey && pdata->nHomeKey == code)
		{
			// Call back to core to request home navigate
			LOG(INFO) + "Home Key has been pressed, navigating to application start page";
			RHODESAPP().navigateToUrl(RHODESAPP().getStartUrl());

			handled = TRUE;
			matched = TRUE;
		}
	}

	// Flag that subsequent WM_CHAR should be suppressed if we are handling the key
	pdata->bSuppressChar = handled;

	//return handled;

	}

	// Clear WM_CHAR suppression
	pdata->bSuppressChar = FALSE;

	return handled;
}


bool CKeyModule::onPrimaryMessage (MSG *pmsg)
{
	INSTANCE_DATA *pdata = pInstanceData;//(INSTANCE_DATA*) GetObjFromID (ncurrid);
	if (!pdata)
	{		
		return FALSE;
	}

	// ignore primary messages while we're in the background
	if (!pdata->bAppHasFocus)
		return FALSE;

	switch (pmsg->message)
	{
		case WM_KEYDOWN:
			return ProcessKeyDown (pmsg->wParam, pmsg->lParam, pdata);

		case WM_CHAR:
			return ProcessChar (pmsg->wParam, pmsg->lParam, pdata);

		case WM_KEYUP:
			return ProcessKeyUp (pmsg->wParam, pmsg->lParam, pdata);
	}

	return FALSE;
}


BOOL CKeyModule::StartTriggerWatch (void)
{
	if (!m_bRcmLoaded)
	{
		LOG(WARNING) + "Trigger functionality is not available on this device";
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
		LOG(WARNING) + perror;
		delete [] perror;

		return FALSE;
	}

	LOG(INFO) + "Trigger watch started";

	return TRUE;
}

void CKeyModule::StopTriggerWatch (void)
{
	if (!m_bRcmLoaded)
	{
		LOG(WARNING) + "Trigger functionality is not available on this device";
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

	LOG(INFO) + "Trigger watch stopped";
}

void CKeyModule::TriggerProc (void)
{
	if (!m_bRcmLoaded)
	{
		LOG(WARNING) + "Trigger functionality is not available on this device";
		return;
	}

	char nav_param [16];
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
		{
			LOG(INFO) + "Trigger event ignored as the application is in the background";
			continue;
		}

		if ((error = m_pRcm->lpfn_RCM_GetTriggerStatus (&status)) != E_RCM_SUCCESS)
		{
			perror = new WCHAR [64];
			StringCchPrintf (perror, 64, L"GetTriggerStatus failed (%lu)", error);
			LOG(WARNING) + perror;
			delete [] perror;

			continue;
		}

		// Send event to core if set
		if (pInstanceData->psTriggerNavigate.hasCallback())
		{
			sprintf (nav_param, "%lu", status);
			rho::Hashtable<rho::String, rho::String> triggerData;
			triggerData.put("triggerFlag", nav_param);
			pInstanceData->psTriggerNavigate.set(triggerData);
		}
	}
}

DWORD CKeyModule::StaticTriggerProc (LPVOID pparam)
{
	CKeyModule *pthis = (CKeyModule*) pparam;
	pthis->TriggerProc ();

	return 0;
}

BOOL CKeyModule::onRhoAppFocus(bool bActivate)
{
	INSTANCE_DATA *pdata = NULL;
	pdata = pInstanceData;//(INSTANCE_DATA*)GetObjFromID(iInstID);
	if (pdata != NULL)
	{
		pdata->bAppHasFocus = bActivate;
		return TRUE;
	}
	else
		return FALSE;
};

//  Functions newly added for RE4 and transition to native extension  //

void CKeyModule::setHomeKey(LPCWSTR pkey) 
{
	int iTemp = GetKeyCode(pkey);
	if ((iTemp != INVALID_KEY)&&(iTemp != KEY_EMPTYSTRING))
	{
		setHomeKey(iTemp);
		RegisterFromOOPEngine();
	}
	else
		LOG(WARNING) + "Unrecognised Key Value " + pkey;
}


int CKeyModule::parseKeyValue(rho::String szKeyValue)
{
	int nKeyValue = INVALID_KEY;
	rho::String comparitor;
	comparitor.assign(szKeyValue);
	std::transform(comparitor.begin(), comparitor.end(), comparitor.begin(), ::tolower);

	if (comparitor.compare("all") == 0)
		nKeyValue = ALL_KEYS;
	else
	{
		nKeyValue = GetKeyCode(rho::common::convertToStringW(szKeyValue).c_str());
		if ((nKeyValue == INVALID_KEY)||(nKeyValue==KEY_EMPTYSTRING))
		{
			LOG(INFO) + "Unrecognised Key Code";
		}
	}
	return nKeyValue;
}

void CKeyModule::setRemap(int iKeyFrom, int iKeyTo)
{
	CKeyMapEntry *pentry;
	pentry = pInstanceData->pKeyMap->Find (iKeyFrom);
	if (pentry)
	{
		pentry->nRemap = iKeyTo;
		if(KEY_EMPTYSTRING==iKeyTo)
		{
			pentry->bDispatch=true;
		}
	}
	else
	{
		pentry = new CKeyMapEntry (iKeyFrom, iKeyTo);
		pInstanceData->pKeyMap->AddEntry(pentry);
	}
}

void CKeyModule::setTriggerCallback(rho::apiGenerator::CMethodResult pCallback)
{
	rho::apiGenerator::CMethodResult blank;
	if (pCallback.hasCallback())
	{
		//  User has provided a callback to listen for trigger presses
		if (!StartTriggerWatch())
		{
			LOG(WARNING) + "Unable to listen for the device trigger";
			pInstanceData->psTriggerNavigate = blank;
		}
		else
		{
			pInstanceData->psTriggerNavigate = pCallback;
		}
	}
	else
	{
		//  User has asked to stop listening for Trigger callbacks
		StopTriggerWatch();
		pInstanceData->psTriggerNavigate = blank;
	}
}

void CKeyModule::setKeyCallback(bool bDispatch, int iKeyValue, rho::apiGenerator::CMethodResult pCallback)
{
	CKeyMapEntry *pentry;
	if (iKeyValue == ALL_KEYS)
	{
		if (pCallback.hasCallback())
		{
			pInstanceData->psAllKeysNavigate = pCallback;
			pInstanceData->bAllKeysDispatch = bDispatch;
		}
		else
		{
			//  Blank out the existing callback
			pInstanceData->psAllKeysNavigate = pCallback;
		}
	}
	else
	{
		pentry = pInstanceData->pKeyMap->Find(iKeyValue);
		if (pentry)
		{
				//  If the user has not specified a callback then delete the mapping
				if (!pCallback.hasCallback())
					pInstanceData->pKeyMap->DeleteEntry(iKeyValue);
				else
				{
					pentry->SetNavigate(pCallback);
					pentry->bDispatch = bDispatch;
				}
		
		}
		else
		{
			//  No Current mapping, add the mapping
			pentry = new CKeyMapEntry(iKeyValue, pCallback , bDispatch);
			pInstanceData->pKeyMap->AddEntry(pentry);
		}
	}

	if (!pInstanceData->psAllKeysNavigate.hasCallback() && 
		!pInstanceData->pKeyMap->HasContents() && 
		pInstanceData->nHomeKey == 0)
		ClearRegistrationsFromOOPEngine();
	else
		RegisterFromOOPEngine();
}

//  The Out of process engine requires the Key Module to register to recieve messages 
//  (keydown / keyup / keychar) however doing so can reduce the performance of RE (per JH)
//  Therefore only register for these if we need them
void CKeyModule::ClearRegistrationsFromOOPEngine()
{
	if (m_bOOPMessagesRegistered)
	{
		RHODESAPP().getExtManager().DeRegisterForPrimaryBrowserMessage(WM_KEYDOWN);
		RHODESAPP().getExtManager().DeRegisterForPrimaryBrowserMessage(WM_CHAR);
		RHODESAPP().getExtManager().DeRegisterForPrimaryBrowserMessage(WM_KEYUP);
		m_bOOPMessagesRegistered = false;
	}
}

void CKeyModule::RegisterFromOOPEngine()
{
	if (!m_bOOPMessagesRegistered)
	{
		RHODESAPP().getExtManager().RegisterForPrimaryBrowserMessage(WM_KEYDOWN);
		RHODESAPP().getExtManager().RegisterForPrimaryBrowserMessage(WM_CHAR);
		RHODESAPP().getExtManager().RegisterForPrimaryBrowserMessage(WM_KEYUP);
		m_bOOPMessagesRegistered = true;
	}
}
