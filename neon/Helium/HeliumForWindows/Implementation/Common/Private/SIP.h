#pragma once
/**
*  \file SIP.h
*  \brief Interface for the CSIP class.
* This class provides the full control over the SIP window.  It
* also prepares it for use within PB
*/

#include "windows.h"
#include "keybd.h"
#include "kbdapi.h"

#define ESCAPE_CHAR L'\\'	///< Definition of the escape character
const int SIP_TITLE_HEIGHT = 26;

/**
*  Possible values for the SIP Control
*/
enum SIP_CONTROL_VALUES
{
	SIP_CONTROL_MANUAL,		///< When shown by the SIP Button, will show and hide when text boxes have focus.
	SIP_CONTROL_AUTOMATIC	///< Shows and hides when text boxes have focus regardless of SIP button.
};

/**
*  Finds the window representing the SIP button (Windows SIP button, not the
*  Browser SIP button) and hides it.
*  Note: Ignores value of "Show SIP on Foreground"
*/
void HideSIPButton();

/**
*  Class to control and manipulate the SIP (Soft Input Panel) for RhoElements.
*  Originally created by Dave Sheppard in March 2005 as a static class this 
*  class has been modified to work within the plug-in architecture of PB3, it 
*  is intended to be used by multiple modules, e.g. MoveSIP, CursorPos and 
*  Scanner will all need to interface with the SIP and each will need to 
*  create their own instance of this class.  Care should be taken when 
*  developing with this module with multiple instances, there is only ever one 
*  SIP window to display whereas there will be multiple instances of this class
*  under the PB3 architecture.
*/
class CSIP  
{
public:
	/**
	*  Initialises the SIP.
	*/
	CSIP();
	~CSIP();

	/**
	*  Calls ResetSIP which puts the SIP back in the original
	*  position
	*/
	void CloseSIP();

	/**
	*  Moves the SIP window to the Co-ordinates sent in
	*  \param x The left position of the SIP in pixels
	*  \param y The top position of the SIP in pixels
	*  \return Whether or not the SIP was successfully moved
	*/
	bool MoveSip(int x, int y);

	/**
	*  Sets the ManualSIP parameter according to the value sent.
	*  Also will turn off the SIP if ManualSIP is set and will
	*  turn on the SIP if ManualSIP is not set.
	*  Setting the value to manual will show and hide the SIP when text boxes
	*  have focus (providing the SIP is shown using the SIP button).
	*  Setting the value to automatic will Show and hide the SIP when 
	*  text boxes have focus regardless of whether the SIP button has been 
	*  pressed.
	*  \param hwndHTML The window handle of the HTML window
	*  \param bSet Set to SIP_CONTROL_MANUAL for Manual SIP control, 
	*  set to SIP_CONTROL_AUTOMATIC for 
	*  \return Whether or not the SIP Control attribute was successfully set.
	*/
	BOOL SetSipControl(HWND hwndHTML, SIP_CONTROL_VALUES bSet);

	/**
	*  Shows the SIP
	*/
	void ShowSIP(HWND hwndHTML, bool bDoShowSip = true);
	
	/**
	*  Hides the SIP
	*/
	void HideSIP(HWND hTMLHWND, bool bDoShowSip = true);
	
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

	/**
	*  Repeatedly calls the SendKey to send an entire data
	*  string to the keyboard buffer.
	*/
	void SendString(LPCTSTR lpszString);	
	
	/**
	*  Sends the Keystroke to the keyboard buffer.  Used by the
	*  scanner and MSR to 'fake' keystrokes from the input devices.
	*/
	void SendKey(TCHAR cKey);
	
	/**
	*  Accessor Method for m_rcOriginalPos
	*/
	RECT	GetOriginalPosition();

	/**
	*  Accessor Method for m_bSipControl
	*/
	SIP_CONTROL_VALUES GetSipControl();

	void ShowTitle(bool bShowTitle);

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

	bool	m_bSipControl;		///< State of the 'SIP Control' Registry Entry
	bool	m_bAllowSIP;		///< Whether or not the SIP is allowed to be shown
	bool	m_bSIPOn;			///< Whether or not the SIP is shown
	DWORD	m_dwOriginalStyle;	///< The Original window style of the SIP control
	BOOL	l_bEscape;			///< Unused as far as I can see
	HWND	m_SipWnd;			///< HWND of the SIP
	RECT	m_rcOriginalPos;	///< Original Location of the SIP when this class is created
	RECT	m_rcPosition;		///< Current Location of the SIP
	bool	m_bTitleShown;		///< True if the system SIP had a title bar when RhoElements launched

	HMODULE hKeybdDriver;		///< Handle to the keyboard driver
	LPFNSETKEYSTATE pfnSetKeyState; ///< pointer to the SetKeyState function
	HWND hSipWnd; ///< handle to the SIP window
	///////////////////////////////////
	//  Stubbed
	///////////////////////////////////

	/**
	*  Stub Function to retrieve the Cursor window's handle.
	*/
	HWND		GetAnimWindow();

	/**
	*  Stub Function to retrieve what used to be the 'Use System SIP' registry
	*  entry.
	*/
	BOOL		GetUseSystemSIP();

	/**
	*  Stub Function to retrieve what used to be the 'SIP Control' registry
	*  entry.  Currently the value is retrieved from the registry.
	*/
	bool RetrieveSIPControlConfigValue(HWND hwndSIP);
};
