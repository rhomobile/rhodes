#include "stdafx.h"
#include <windows.h>
#include "pwdPopUp.h"

_PWDHANDLES pwdHandles;
_PWDAXIS pwdBtns[2];
_PWDAXIS pwdEditInputField[1];
_PWDCOORDINATES pwdCoordinates;
TCHAR windowTitle[MAXCHAR] = L"";
TCHAR drawTextMessage[MAXCHAR] = L"";
TCHAR passwordValue[MAXCHAR] = L"";
bool isExitPasswordNeedToBeDone = false;
bool isEnteredPasswordCorrect = false;
bool isWMDevice = false;

BOOL IsDeviceInLandScape(){
  DEVMODE devmode;
  ZeroMemory(&devmode, sizeof(DEVMODE));
  devmode.dmSize = sizeof(DEVMODE);
  devmode.dmDisplayOrientation = DMDO_0;
  devmode.dmFields = DM_DISPLAYORIENTATION;
  ChangeDisplaySettingsEx(NULL, &devmode, NULL, CDS_TEST, NULL);

  return ((devmode.dmDisplayOrientation == DMDO_90) || (devmode.dmDisplayOrientation == DMDO_270));
}
BOOL IsDeviceInPortrait(){
  DEVMODE devmode;
  ZeroMemory(&devmode, sizeof(DEVMODE));
  devmode.dmSize = sizeof(DEVMODE);
  devmode.dmDisplayOrientation = DMDO_0;
  devmode.dmFields = DM_DISPLAYORIENTATION;
  ChangeDisplaySettingsEx(NULL, &devmode, NULL, CDS_TEST, NULL);

  return ((devmode.dmDisplayOrientation == DMDO_0) || (devmode.dmDisplayOrientation == DMDO_180));
}

ATOM PwdMyRegisterClass(HINSTANCE hInstance){
	WNDCLASS wcx; // WINDOW class information
	if (!GetClassInfo(hInstance, L"CheckPassword", &wcx))
	{
		//Initialize the struct to zero
		ZeroMemory(&wcx,sizeof(WNDCLASS));
		wcx.style = 0; //CS_HREDRAW|CS_VREDRAW |CS_DBLCLKS ; // Class styles
		wcx.lpfnWndProc = (WNDPROC)PwdMainWndProc; // Pointer to the callback procedure
		wcx.cbClsExtra = 0; // Extra byte to allocate following the wndclassex structure
		wcx.cbWndExtra = 0; // Extra byte to allocate following an instance of the structure
		wcx.hInstance = hInstance; // Instance of the application
		wcx.hIcon = NULL; // Class Icon
		wcx.hCursor = 0; //LoadCursor(NULL, IDC_ARROW); // Class Cursor
		wcx.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH); // Background brush
		wcx.lpszMenuName = NULL; // Menu resource
		wcx.lpszClassName = L"CheckPassword"; // Name of this class
		wcx.hIcon = NULL; // Small icon for this class
		return RegisterClass(&wcx);	
	}
	return TRUE;
}

BOOL PwdInitInstance(HWND hWnd, HINSTANCE hInstance){	
	
	// Create the window
	pwdHandles.parent = CreateWindow(L"CheckPassword", // Window class name
			windowTitle, // Window title
			WS_POPUP | WS_CAPTION, // Window style
			(pwdCoordinates.xCenter-pwdCoordinates.mainWndXpos/2),(pwdCoordinates.yCenter-pwdCoordinates.mainWndYpos/2),//1200,730, // (x,y) pos of the window
			pwdCoordinates.mainWndXpos,pwdCoordinates.mainWndYpos + 30, // Width and height of the window
			hWnd,//HWND_DESKTOP, // HWND of the parent window (can be null also)
			NULL, // Handle to menu
			pwdHandles.hInstance, // Handle to application instance
			NULL); // Pointer to window creation data

	// Check if window creation was successful
	if (!pwdHandles.parent)
		return 0;

	// Make the window visible
	ShowWindow(pwdHandles.parent,SW_SHOW);
	UpdateWindow(pwdHandles.parent);

	return TRUE;
}


VOID PwdCreateButtons(HWND hwnd){
	pwdHandles.parent = hwnd;
	HWND *buttons = &(pwdHandles.okButton);
	for(int i = 0; i < 2; i++){
		*buttons = CreateWindowEx(WS_EX_CLIENTEDGE,
								   L"button", pwdBtns[i].buttonName,
								   WS_VISIBLE | WS_CHILD | WS_TABSTOP, 
								   pwdBtns[i].x, pwdBtns[i].y,
								   pwdBtns[i].width,pwdBtns[i].height,
								   hwnd,     
								   (HMENU) (PWD_INITIAL_BUTTON+i),
								   pwdHandles.hInstance, NULL) ;	

		buttons++;
	}
}

VOID PwdCreateInputFieldWindow(HWND hwnd){
	pwdHandles.parent = hwnd;
	HWND *editInputFieldWindow = &(pwdHandles.inputField);	
	*editInputFieldWindow = CreateWindowEx(WS_EX_CLIENTEDGE,
						L"edit", L"",
						WS_VISIBLE | WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL | ES_PASSWORD, 
						pwdEditInputField[PWD_AXIS_INPUTFIELD].x, pwdEditInputField[PWD_AXIS_INPUTFIELD].y,
						pwdEditInputField[PWD_AXIS_INPUTFIELD].width,pwdEditInputField[PWD_AXIS_INPUTFIELD].height, 
						hwnd, 
						NULL, 
						pwdHandles.hInstance, NULL);	
}

VOID PwdDrawTextOnSubTitleArea(HWND bgHandle, LPCWSTR message, int x, int y)
{
	RECT rect;
	HDC wdc = GetWindowDC(bgHandle);
	GetClientRect (bgHandle, &rect) ;
	SetTextColor(wdc, 0x00000000);
	SetBkMode(wdc,TRANSPARENT);
	rect.left = x;
	rect.top = y;
	DrawText( wdc, message, -1, &rect, DT_NOCLIP | DT_WORDBREAK | DT_NOPREFIX ) ;
	DeleteDC(wdc);	
}

LRESULT CALLBACK PwdMainWndProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam)
 {
	switch (msg)
	{
		case WM_CREATE:		
			PwdCreateButtons(hwnd);	
			PwdCreateInputFieldWindow(hwnd);
			SetFocus(pwdHandles.inputField);
		    break;
		
		case WM_ACTIVATE:
			//ShowWindow(pwdHandles.mainParentWnd,SW_SHOW);
			break;

		case WM_PAINT:
			PAINTSTRUCT ps;
			HDC hdc;
			hdc = BeginPaint(hwnd, &ps);	
			DeleteDC (hdc);
			EndPaint(hwnd, &ps);
			if(isWMDevice){
				PwdDrawTextOnSubTitleArea(hwnd, drawTextMessage, 20, 50);
			}else{
				PwdDrawTextOnSubTitleArea(hwnd, drawTextMessage, 20, 25);
			}
			break;	

		case WM_COMMAND:		
			TCHAR enteredPassWord[MAXCHAR];
			switch (LOWORD(wParam))
			{
				case VK_RETURN:
				case PWD_IDB_BTN_OK:
					{
						SendMessage(pwdHandles.inputField,WM_GETTEXT,MAXCHAR,(LPARAM)(enteredPassWord));
						if(isExitPasswordNeedToBeDone){
							if(_tcscmp(passwordValue,enteredPassWord) == 0)
							{
								isEnteredPasswordCorrect = true;
								DestroyWindow(pwdHandles.parent);
							}
							else
							{
								PwdDrawTextOnSubTitleArea(hwnd, L"InValid Password", 20, pwdCoordinates.mainWndYpos);
								SendMessage(pwdHandles.inputField,WM_SETTEXT,MAXCHAR,(LPARAM)L"");
								isEnteredPasswordCorrect = false;
							}
						}
						else
						{
							//Currently Do Nothing
						}
					}
					break;		
				case PWD_IDB_BTN_CANCEL:
					{
						isEnteredPasswordCorrect = false;
						PostMessage(pwdHandles.parent , WM_KEYDOWN, VK_ESCAPE, 0);
					}
					break;
			}
			break;			

		case WM_DESTROY:			
			EnableWindow(pwdHandles.mainParentWnd, TRUE);
			SetForegroundWindow(pwdHandles.mainParentWnd);
			DestroyWindow(hwnd);
			PostQuitMessage(0);
			break;

		default:
			return DefWindowProc(hwnd,msg,wParam,lParam);// Call the default window handler
	}
	return 0;
 } 
VOID PwdInitialiseCoordinates()
{	
	bool isCheckCoordinatesDifferent = false;
	pwdCoordinates.width  = GetSystemMetrics(SM_CXVIRTUALSCREEN);
	pwdCoordinates.height = GetSystemMetrics(SM_CYVIRTUALSCREEN);
	if(pwdCoordinates.width <480 && isWMDevice){
		isCheckCoordinatesDifferent = true;
		pwdCoordinates.width = 380;
	}
	if(pwdCoordinates.height <480 && isWMDevice){
		isCheckCoordinatesDifferent = true;
		pwdCoordinates.height = 440;
	}
	pwdCoordinates.xCenter = (pwdCoordinates.width/2);
	if(pwdCoordinates.height <480 && isWMDevice)
	{
		pwdCoordinates.yCenter = (pwdCoordinates.height/2) - 50;
	}
	else
	{
		pwdCoordinates.yCenter = (pwdCoordinates.height/2);
	}
	if(IsDeviceInPortrait())
	{
		pwdCoordinates.mainWndXpos = (pwdCoordinates.width * 8)/10;
		pwdCoordinates.mainWndYpos = (pwdCoordinates.height * 3)/10;	
	}
	else if(isCheckCoordinatesDifferent && isWMDevice)
	{
		pwdCoordinates.mainWndXpos = (pwdCoordinates.width * 8)/10;
		pwdCoordinates.mainWndYpos = (pwdCoordinates.height * 3)/10;	
	}
	else
	{
		pwdCoordinates.mainWndXpos = ((pwdCoordinates.width * 8)/10) - 80;
		pwdCoordinates.mainWndYpos = ((pwdCoordinates.height * 3)/10) + 20;
	}

	//Edit Input Field 
	pwdEditInputField[PWD_AXIS_INPUTFIELD].x = 20;
	if(isWMDevice){
		pwdEditInputField[PWD_AXIS_INPUTFIELD].y = 40;
	}
	else{
		pwdEditInputField[PWD_AXIS_INPUTFIELD].y = 20;
	}
	pwdEditInputField[PWD_AXIS_INPUTFIELD].width = (pwdCoordinates.mainWndXpos * 8)/10;
	pwdEditInputField[PWD_AXIS_INPUTFIELD].height = (pwdCoordinates.mainWndYpos * 2)/10;
	pwdEditInputField[PWD_AXIS_INPUTFIELD].buttonName = L"";	

	//Ok Button
	pwdBtns[PWD_AXIS_OK].x = 20;
	if(isWMDevice){
		pwdBtns[PWD_AXIS_OK].y = ((pwdCoordinates.mainWndYpos * 2)/10) + 50;
	}
	else{
		pwdBtns[PWD_AXIS_OK].y = ((pwdCoordinates.mainWndYpos * 2)/10) + 30;
	}
	pwdBtns[PWD_AXIS_OK].width = (pwdCoordinates.mainWndXpos * 3)/10;
	pwdBtns[PWD_AXIS_OK].height = (pwdCoordinates.mainWndYpos * 2)/10;
	pwdBtns[PWD_AXIS_OK].buttonName = L"OK";

	//Cancel Button
	pwdBtns[PWD_AXIS_CANCEL].x = ((pwdCoordinates.mainWndXpos * 3)/10) + 20 + 35;
	if(isWMDevice){
		pwdBtns[PWD_AXIS_CANCEL].y = ((pwdCoordinates.mainWndYpos * 2)/10) + 50;
	}
	else{
		pwdBtns[PWD_AXIS_CANCEL].y = ((pwdCoordinates.mainWndYpos * 2)/10) + 30;
	}
	pwdBtns[PWD_AXIS_CANCEL].width= (pwdCoordinates.mainWndXpos * 3)/10;
	pwdBtns[PWD_AXIS_CANCEL].height = (pwdCoordinates.mainWndYpos * 2)/10;
	pwdBtns[PWD_AXIS_CANCEL].buttonName = L"CANCEL";
}

bool ShowPasswordDialog(bool setDeviceType, bool isExiting, HWND hWnd, HINSTANCE hInstance, LPCTSTR szWindowTitle, LPCTSTR szDrawTextMessage, LPCTSTR szPasswordValue)
{
	isExitPasswordNeedToBeDone = isExiting;

	//Window Title
	memset(windowTitle, 0,sizeof(windowTitle));
	_tcscpy(windowTitle,szWindowTitle);

	//DrawTextMessage
	memset(drawTextMessage, 0,sizeof(drawTextMessage));
	_tcscpy(drawTextMessage,szDrawTextMessage);

	//Password Value
	memset(passwordValue, 0,sizeof(passwordValue));
	_tcscpy(passwordValue,szPasswordValue);

	isWMDevice = setDeviceType;
	pwdHandles.mainParentWnd = hWnd;
	pwdHandles.hInstance = hInstance;
	PwdInitialiseCoordinates();

	MSG msg; // MSG structure to store messages	

	// Register Window Class
	if (!PwdMyRegisterClass(hInstance))
	{
		isEnteredPasswordCorrect = false;
		return isEnteredPasswordCorrect;
	}

	// Perform application initialization:
	if (!PwdInitInstance (hWnd,hInstance))
	{
		isEnteredPasswordCorrect = false;
		return isEnteredPasswordCorrect;
	}

	// Main message loop: Process messages coming to this window
	while (GetMessage(&msg,NULL,0,0))
	{
		if (msg.message == WM_KEYDOWN) 
		{
			//if pressed Enter button
			if (msg.wParam == VK_RETURN)
            {
				PostMessage(pwdHandles.parent , WM_COMMAND, VK_RETURN, 0);
			}
			//if pressed Cancel button
			if (msg.wParam == VK_ESCAPE)
            {
				SendMessage(pwdHandles.parent , WM_DESTROY, 0, 0);
                isEnteredPasswordCorrect = false;
            }
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return isEnteredPasswordCorrect;
}
