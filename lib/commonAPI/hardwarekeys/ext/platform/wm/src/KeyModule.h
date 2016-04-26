/** 
* \file KeyModule.h
*/

#pragma once

#include "KeyMap.h"
#include "RcmLoader.h"
#include "logging/RhoLog.h"

#define INVALID_KEY		-2
#define KEY_EMPTYSTRING		0
#define ALL_KEYS		-1
#define DISPLAYCLASS L"DISPLAYCLASS"

class CKeyModule;

struct INSTANCE_DATA
{
	CKeyMap *pKeyMap;
	BOOL bAllKeysDispatch;
	rho::apiGenerator::CMethodResult psAllKeysNavigate;
	rho::apiGenerator::CMethodResult psTriggerNavigate;
	bool bSuppressChar;
	int nHomeKey;
	int nAccelerateMode;
//	int nControlKeys, *pControlKeys;	///< Enabled control keys
	bool bAppHasFocus;
};

struct TRIGGER_DATA
{
	CKeyModule *pModule;
	LPCWSTR psNavigate;
};

/******************************************************************************/
/**
* Main Key plugin module class.
*/
/******************************************************************************/
class CKeyModule
{
public:
	CKeyModule ();
	~CKeyModule ();
	BOOL Initialize();
	/**
	*  Reads the Configuration file to determine which Function and Application
	*  Keys should not be blocked and blocks all unspecified keys from 
	*  functioning.  These are Windows Hot Keys, which send WM_HOTKEY.  
	*  \param HWND of the new application to receive the hot keys.
	*  \return TRUE if the keys were successfully registered for by Browser
	*          or FALSE if the registration failed
	*  \todo ES400 device has a dedicated calendar button which is not blocked
	*        by this function.  Reported key down code is 0x3D
	*/
	static BOOL BlockHotKeys(HWND hwndToRegisterFor);
	bool onPrimaryMessage (MSG *pmsg);
	void setHomeKey(int nHomeKey) {pInstanceData->nHomeKey = nHomeKey;}
	void setHomeKey(LPCWSTR pkey);
	int getHomeKey() {return pInstanceData->nHomeKey;}
	
	int parseKeyValue(rho::String szKeyValue);
	void setRemap(int iKeyFrom, int iKeyTo);
	void setTriggerCallback(rho::apiGenerator::CMethodResult pCallback);
	void setKeyCallback(bool bDispatch, int iKeyValue, rho::apiGenerator::CMethodResult pCallback);
	BOOL onRhoAppFocus(bool bActivate);

private:
	BOOL onBeforeNavigate (int iInstID);
	BOOL onWindowChanged(int iEvent);

	INSTANCE_DATA *pInstanceData;

	HANDLE hTriggerEvent;
	HANDLE hTriggerQuit;
	HANDLE hTriggerThread;
	HANDLE hTriggerNotification;
	static DWORD StaticTriggerProc (LPVOID pparam);
	void TriggerProc (void);
	BOOL StartTriggerWatch (void);
	void StopTriggerWatch (void);

	BOOL IsAccelerator (int key);
	void ClearAll (INSTANCE_DATA *pdata, BOOL bIsNavigation);
	int GetKeyCode (LPCWSTR pkey);
	bool ProcessKeyDown (WPARAM wparam, LPARAM lparam, INSTANCE_DATA *pdata);
	bool ProcessChar (WPARAM wparam, LPARAM lparam, INSTANCE_DATA *pdata);
	bool ProcessKeyUp (WPARAM wparam, LPARAM lparam, INSTANCE_DATA *pdata);
	void ClearRegistrationsFromOOPEngine();
	void RegisterFromOOPEngine();

	CRcmLoader* m_pRcm;		///<  EMDK Rcm DLL loaded dynamically
	BOOL m_bRcmLoaded;		///<  Whether or not the Rcm DLL has been loaded
	rho::apiGenerator::CMethodResult blankCallback; //  Default callback
	bool m_bOOPMessagesRegistered;
};
