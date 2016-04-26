/** 
* \file KeyModule.h
*/

/******************************************************************************/
#pragma once

/******************************************************************************/
#include "..\..\..\Common\Public\PBPlugin.h"
#include "..\..\..\Common\Public\PBUtil.h"
#include "KeyMap.h"
#include "../../../common/Private/RcmLoader.h"

/******************************************************************************/
#define INVALID_KEY		0
#define ALL_KEYS		-1

/******************************************************************************/
class CKeyModule;

static TCHAR* tcKeyEventNames[] = 
{
	L"keyValue",
	L"keyCode",
	NULL
};

static TCHAR* tcTriggerEventNames[] = 
{
	L"triggerFlag",
	NULL
};


/******************************************************************************/
struct INSTANCE_DATA
{
	int nID;					///< App ID from PBModule
	CKeyMap *pKeyMap;
	BOOL bAllKeysDispatch;
	WCHAR *psAllKeysNavigate;
	WCHAR *psTriggerNavigate;
	BOOL bSuppressChar;
	int nHomeKey;
	int nAccelerateMode;
	int nControlKeys, *pControlKeys;	///< Enabled control keys
	bool bAppHasFocus;
};

/******************************************************************************/
struct TRIGGER_DATA
{
	CKeyModule *pModule;
	LPCWSTR psNavigate;
};

/******************************************************************************/
/**
* Main Key plugin module class derived from PBModule.
*/
/******************************************************************************/
class CKeyModule: public PBModule
{
public:
	CKeyModule (PPBCORESTRUCT pPBCoreStructure);
	~CKeyModule ();

	static pBrowserFunc pBrowserHome;

private:
	/** \name Overrides of functions in PBModule base class */
	//@{
	BOOL onInit(PPBSTRUCT pPBStructure);
	void onDeInit(PPBSTRUCT pPBStructure);
	BOOL onAttachInstance(PPBSTRUCT pPBStructure, PPBINSTSTRUCT pInstStruct);
	BOOL onReleaseInstance(PPBSTRUCT pPBStructure, PPBINSTSTRUCT pInstStruct);

	BOOL MetaProc(PBMetaStruct *pbMetaStructure, PPBSTRUCT pPBStructure, void *pParam);
	BOOL onMessage (HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam, void *pobject);
	BOOL onBeforeNavigate (int iInstID);
	BOOL onPrimaryMessage (MSG *pmsg, int ncurrid);
	BOOL onWindowChanged(int iEvent);

	/**  
	*  \todo Implement focus change for key capture.  This can not be 
	*  done at the present time as the Multiple Instance Test Container is 
	*  out of commission and application switching has not yet been implemented
	*  in the core.
	*/
	BOOL onAppFocus(int iOldID,int iNewID);
	//@}

	/**
	*  This function is inherited from PBModule::onRhoAppFocus.
	*  Please see that function for further definition.
	*/
	BOOL onRhoAppFocus(bool bActivate, int iInstID);

	/** \name Variables to hold tag values before 'event' tag received */
	//@{
	int nKeyValue;
	BOOL bDispatch;
	//@}

	INSTANCE_DATA *pInstanceData;
	PPBCORESTRUCT m_pPBCoreStructure;		///< Pointer to callback functions provided by core

	HANDLE hTriggerEvent;
	HANDLE hTriggerQuit;
	HANDLE hTriggerThread;
	HANDLE hTriggerNotification;
	BOOL m_bMinimized;						///< Boolean to indicate whether or not the entire Browser exe is minimized
	HWND m_hDisplayClass;
	static DWORD StaticTriggerProc (LPVOID pparam);
	void TriggerProc (void);
	BOOL StartTriggerWatch (void);
	void StopTriggerWatch (void);

	BOOL IsAccelerator (int key);
	void ClearAll (INSTANCE_DATA *pdata, BOOL bIsNavigation);
	void ResetDefinition (void);
	int GetKeyCode (LPCWSTR pkey);
	BOOL ProcessKeyDown (WPARAM wparam, LPARAM lparam, INSTANCE_DATA *pdata);
	BOOL ProcessChar (WPARAM wparam, LPARAM lparam, INSTANCE_DATA *pdata);
	BOOL ProcessKeyUp (WPARAM wparam, LPARAM lparam, INSTANCE_DATA *pdata);

	/**
	*  Reads the Configuration file to determine which Function and Application
	*  Keys should not be blocked and blocks all unspecified keys from 
	*  functioning.  These are Windows Hot Keys, which send WM_HOTKEY.  Note
	*  this registration will persist across PB launches but will not persist 
	*  across boots.
	*  \param HWND of the new application to receive the hot keys.
	*  \return TRUE if the keys were successfully registered for by Browser
	*          or FALSE if the registration failed
	*  \todo ES400 device has a dedicated calendar button which is not blocked
	*        by this function.  Reported key down code is 0x3D
	*  \todo Does this method belong somewhere else?  It only needs to be executed
	*        for the first loaded application.
	*/
	BOOL BlockHotKeys(HWND hwndToRegisterFor);
	CRcmLoader* m_pRcm;		///<  EMDK Rcm DLL loaded dynamically
	BOOL m_bRcmLoaded;		///<  Whether or not the Rcm DLL has been loaded
};
