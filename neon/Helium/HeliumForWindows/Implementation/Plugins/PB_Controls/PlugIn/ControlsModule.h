/******************************************************************************/
#pragma once

/******************************************************************************/
#include "..\..\..\Common\Public\PBPlugin.h"
//#include "..\..\..\Common\Public\PBUtil.h"
#include "Control.h"
#include "CommandArea.h"
#include "KeyState.h"
#include "AddressBar.h"
#include "Button.h"
#include "ControlList.h"
#include "Scroll.h"
#include "Indicator.h"

/******************************************************************************/
#define MODULE_NAME			L"Controls"

#define CT_QUITBUTTON			1
#define CT_RELOADBUTTON			2
#define CT_ADDRESSBAR			3
#define CT_TOPCOMMANDAREA		4
#define CT_BOTTOMCOMMANDAREA	5
#define CT_BACKBUTTON			6
#define CT_FORWARDBUTTON		7
#define CT_GOBUTTON				8
#define CT_HOMEBUTTON			9
#define CT_MINIMIZEBUTTON		10
#define CT_PAGEZOOMINBUTTON		11
#define CT_PAGEZOOMOUTBUTTON	12
#define CT_SIPBUTTON			13
#define CT_STOPBUTTON			14
#define CT_ZOOMTEXTBUTTON		15
#define CT_KEYSTATE				16
#define CT_CUSTOMBUTTON			17
#define CT_SCROLL				18
#define CT_BATTERY				19
#define CT_SIGNAL				20

#define KEY_EVENT				(WM_USER + 1)
#define KEYBOARD_EVENT			(WM_USER + 2)

/******************************************************************************/
struct INSTANCE_DATA
{
	int nID;					///< App ID from PBModule

	CQuitButton *pQuitButton;
	CReloadButton *pReloadButton;
	CAddressBar *pAddressBar;
	CTopCommandArea *pTopCommandArea;
	CBottomCommandArea *pBottomCommandArea;
	CBackButton *pBackButton;
	CForwardButton *pForwardButton;
	CGoButton *pGoButton;
	CHomeButton *pHomeButton;
	CMinimizeButton *pMinimizeButton;
	CPageZoomInButton *pPageZoomInButton;
	CPageZoomOutButton *pPageZoomOutButton;
	CSIPButton *pSIPButton;
	CStopButton *pStopButton;
	CZoomTextButton *pZoomTextButton;
	CKeyState* pKeyState;
	CControlList *pCustomButtonList;
	CScroll *pScroll;
	CBattery *pBattery;
	CSignal *pSignal;

	WCHAR sControlID [64];	// Most recent control ID
};

/******************************************************************************/
class CControlsModule: public PBModule
{
public:
	CControlsModule ();
	~CControlsModule ();

	void SetControlType (LPCWSTR type);

	// Copy of core struct as received during preload
	static PBCORESTRUCT PBCoreStruct;

	// Directory for default images
	static WCHAR szImageDirectory [148];

	static int nScreenWidth, nScreenHeight;
	static int nDefaultButtonSize;

	void WriteLog (LPCWSTR pfunction, int line, LPCWSTR pformat, ...);

private:
	/** \name Overrides of functions in PBModule base class */
	//@{
	BOOL onInit(PPBSTRUCT pPBStructure);
	void onDeInit(PPBSTRUCT pPBStructure);
	BOOL onAttachInstance(PPBSTRUCT pPBStructure, PPBINSTSTRUCT pInstStruct);
	BOOL onReleaseInstance(PPBSTRUCT pPBStructure, PPBINSTSTRUCT pInstStruct);
	BOOL MetaProc(PBMetaStruct *pbMetaStructure, PPBSTRUCT pPBStructure, void *pParam);
	BOOL onBeforeNavigate (int iInstID);
	BOOL onDocumentComplete (int iInstID);
	BOOL onMessage (HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam, void *pobject);
	BOOL onPrimaryMessage (MSG *pmsg, int ncurrid);
	BOOL onWindowChanged (int iEvent);
	//@}

	int nControlType;

	// Invisible window to receive key state messages
	HWND hKeyStateWindow;
	HMODULE hKeyboardDriver;
	static LRESULT CALLBACK StaticKeyWindowProc (HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
	void NotifyKeyState (DWORD state);
	static int dwKeyState, dwCapsState;
	void BringControlsToFront (INSTANCE_DATA *pdata);
};
