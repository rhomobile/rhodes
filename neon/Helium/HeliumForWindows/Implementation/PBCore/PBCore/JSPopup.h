#include <windows.h>
#include "..\..\Engine\Common\pbengine_defs.h"

/**
*  CJSPopup defines a popup window to represent an Alert box, confirm dialog
*  or Prompt, originating from Javascript for RhoElements.
*/
class CJSPopup
{
public:
	//  For Confirm dialogs message data is just a string, for prompts the message
	//  data is a structure.
	//  Type is the type of dialog, alert, confirm or prompt.  The same enumeration
	//  is used here as is used for Engine Events
	CJSPopup(void* pMessageData, int type);
	~CJSPopup();
	//  Displays the dialog and blocks until the user presses ok / cancel
	LRESULT ShowJSDialog(HWND hParentWnd);
	//Sip is not shown while trying to enter text in fields of Authentication screen
	// On Authentication window SIP has been shown now on webkit Engine
	//Controlling the SIP window
	void ShowSIPWnd(BOOL bShow);
private:
	//  Determine the layout and size of the elements in the dialog based on
	//  the screen size of the device and which type of dialog is being
	//  displayed
	BOOL LayoutDialog();
	//  Windows Mobile gives no visual indication which button has focus when you 
	//  tab between them, this function will give focus to the specified control
	//  and provide a visual indication.
	void SetFocusToControl(HWND hwndControl);
	//  Functionality to abstract resizing of the popup has been abstracted so it can 
	//  be refactored easier in the future.
	void ResizePopupDialog(HWND hwnd, HDC hdc, CJSPopup* pDialog);
	//  The WNDPROC for the popup window
	static LRESULT CALLBACK PopupWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	//  The string message to display to the user, this is drawn to the screen and the
	//  height of the resulting text will contribute towards the height of the dislog.
	//Sip is not shown while trying to enter text in fields of Authentication screen
	// On Authentication window SIP has been shown now on webkit Engine
	static LRESULT CALLBACK EditWndProc_1(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK EditWndProc_2(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	WCHAR* m_szMessageToDisplay;
	//  Other data assocated with the dialog, for the prompt this will be the 
	//  EngineJSPopupPrompt* structure defining the extra data for the dialog.
	void* m_pOtherData;
	//  HWND for the popup dialog
	HWND m_hDialog;
	//  HWND of the parent window (disabled when the dialog is visible)
	HWND m_hParentWnd;
	HWND m_hOKButton;		///<  HWND of the OK Button
	HWND m_hCancelButton;	///<  HWND of the Cancel button (NULL if no cancel button)
	HWND m_hInputField;		///<  HWND of the edit field (NULL on non-prompt dialogs)
	HWND m_hInputField2;	///<  HWND of the second edit field (NULL on non authentication dialogs)
	HFONT m_hTextFont;		///<  Font to be used in the message box text
	HFONT m_hButtonFont;	///<  Font to be used for the dialog butons
	HINSTANCE m_hInst;		///<  HINSTANDCE to be used for the windows created.
	int m_iType;			///<  Type of the dialog: Alert, Confirm or Prompt
	//  Locations of controls
	int m_iButtonsCount;	///<  Number of buttons on the dialog.
	int m_iDialogWidth;		///<  Width of the dialog
	int m_iDialogLeft;		///<  X position of the Dialog
	int m_iScreenHeight;	///<  Screen Height, used to determine the Y position of the dialog
	int m_iCaptionHeight;	///<  Height of the dialog caption added by Windows and used to determine Dialog Y position.
	int m_iButtonWidth;		///<  Width of the OK / Cancel Buttons
	int m_iButtonHeight;	///<  Height of the OK / Cancel Buttons
	int m_iGapBetweenButtons;	///<  Horizontal gap between the Dialog Buttons
	int m_iButtonVerticalGap;	///<  Gap above and below the Dialog buttons
	int m_iLeftmostButtonStart;	///<  X Position of the leftmost button
	int m_iTextLeftMargin;		///<  Left margin of the dialog text, otherwise it touches the window
	int m_iTextTopMargin;		///<  Top margin of the dialog text, otherwise it touches the window.
	int m_iInputBoxHeight;		///<  Height of the input box for prompt dialogs
	int m_iInputBox2Height;		///<  Height of the password field on authentication dialogs
	int m_iInputBoxHorizontalMargin;	///<  Gap to the left / right of the input box on prompt dialogs
	int m_iInputBoxTopMargin;	///<  Gap above the input box on prompt dialogs
	int m_iInputBox2TopMargin;	///<  Gap above the second input box (authentication only)
};
