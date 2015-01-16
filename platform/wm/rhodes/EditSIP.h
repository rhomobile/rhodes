#pragma once
#include "..\..\..\..\lib\commonAPI\coreapi\ext\platform\wm\src\EmdkDefines.h"
class CEditSIP  
{
public:
	/**
	*  Initialises the SIP.
	*/
	CEditSIP();
	~CEditSIP();

	

	
	/**
	*  Toggles the SIP state between ON and OFF.  Reads the current
	*  one to decide on what to do.  Also, if ManualSIP is set it
	*  reduces the SIP window size to 0,0 when hiding the SIP
	*/
	void ToggleSIP();

	/**
	*  Toggles the SIP state between ON and OFF.  Gets the window 
	*  to the System suip then clicks it using a pretend stylus 
	*  down and up
	*/
	BOOL ToggleSystemSIP();

	
private:
	/**
	*  Initialises the SIP class.  It removes the Caption bar from 
	*  the top of the SIP window on CE .NET.  Also changes the size 
	*  on .NET so that only the keyboard is shown.  On WM the SIP window
	*  is increased by one pixel so that the top line is repeated on the
	*  bottom of the SIP.  Otherwise there is no closing line.
	*  The function also checks the ManualSIP registry setting, if this is
	*  set the SIP is set to 0 x 0 size when hidden.  This stops the SIP 
	*  appearing when a text field is clicked.
	*/
	void InitSIP();

	/**
	*  Resets the SIP to the original style, size and position
	*/
	void ResetSIP();


	HMODULE hKeybdDriver;		///< Handle to the keyboard driver
	LPFNSETKEYSTATE pfnSetKeyState; ///< pointer to the SetKeyState function
	HWND hSipWnd; ///< handle to the SIP window
	///////////////////////////////////
	//  Stubbed
	///////////////////////////////////


};

void ShowSIPWnd(BOOL bShow);
LRESULT CALLBACK NetworkWndProc(HWND hWnd,LPARAM lParam);
// EMBPD00158491
static WNDPROC g_wpEditWndProc;