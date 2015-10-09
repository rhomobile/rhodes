#ifndef MAIN_H
#define MAIN_H

#define PWD_AXIS_OK				0
#define PWD_AXIS_CANCEL			1
#define PWD_AXIS_INPUTFIELD		0

#define PWD_IDB_BTN_OK                  300
#define PWD_IDB_BTN_CANCEL              301
#define PWD_ID_EDIT_FIELD				302

#define PWD_INITIAL_BUTTON			PWD_IDB_BTN_OK
#define PWD_LAST_BUTTON				PWD_IDB_BTN_CANCEL

struct _PWDCOORDINATES
{
	short int width;
	short int height;
	short int xCenter;
	short int yCenter;
	short int mainWndXpos;
	short int mainWndYpos;
};

struct _PWDHANDLES{
	HINSTANCE hInstance;
	HWND parent;
	HWND mainParentWnd;
	HWND okButton;
	HWND cancelButton;
	HWND inputField;
};

struct _PWDAXIS{
	short int x;
	short int y;
	short int width;
	short int height;
	LPWSTR buttonName;
};

extern bool isExitPasswordNeedToBeDone;
extern bool isEnteredPasswordCorrect;
extern bool isWMDevice;
extern _PWDCOORDINATES pwdCoordinates;
extern _PWDHANDLES pwdHandles;
extern _PWDAXIS pwdBtns[2];
extern _PWDAXIS pwdEditInputField[1];
extern TCHAR windowTitle[MAXCHAR];
extern TCHAR drawTextMessage[MAXCHAR];
extern TCHAR passwordValue[MAXCHAR];
extern BOOL IsDeviceInLandScape();
extern BOOL IsDeviceInPortrait();

extern VOID PwdInitialiseCoordinates();
extern VOID PwdCreateButtons(HWND);
extern VOID PwdCreateInputFieldWindow(HWND);
extern VOID PwdDrawTextOnSubTitleArea(HWND bgHandle, LPCWSTR message, int x, int y);

//Callback function
extern LRESULT CALLBACK PwdMainWndProc(HWND,UINT,WPARAM,LPARAM); 

// Forward declarations of functions included in this code module:
extern ATOM	PwdMyRegisterClass(HINSTANCE hInstance);
extern BOOL	PwdInitInstance(HWND, HINSTANCE);

extern bool ShowPasswordDialog(bool setDeviceType, bool isExiting, HWND hWnd, HINSTANCE hInstance, LPCTSTR szWindowTitle, LPCTSTR szDrawTextMessage, LPCTSTR szPasswordValue);

#endif