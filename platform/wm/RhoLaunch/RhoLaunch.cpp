// RhoLaunch.cpp : Defines the entry point for the application.
//

#include "windows.h"
#include "RhoLaunch.h"


#define MAX_LOADSTRING 100

// Global Variables:
//HINSTANCE			g_hInst;			// current instance
//#ifdef SHELL_AYGSHELL
//HWND				g_hWndMenuBar;		// menu bar handle
//#else // SHELL_AYGSHELL
//HWND				g_hWndCommandBar;	// command bar handle
//#endif // SHELL_AYGSHELL


int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPTSTR    lpCmdLine,
                   int       nCmdShow)
{
	
	//validate the command line
	int iLen = wcslen(lpCmdLine);
	if(iLen < 3){
		return 1;
	}

	LPWSTR pAppName = new WCHAR[iLen + 3];//allow space for the index
	LPWSTR pTemp = pAppName;
	
	
	LPWSTR pTabID;
	int iLoop,iTabID;
	bool bRead = false;

	
	for(iLoop = 0;iLoop < iLen ;iLoop++)
	{
		if(lpCmdLine[iLoop] == L' '){
			if(bRead){
				*pTemp= NULL;
				for(;iLoop < iLen && lpCmdLine[iLoop] == L' ';iLoop++);
				pTabID = &lpCmdLine[iLoop+1];
				iTabID = _wtoi(pTabID);
				
				break;
			}
			continue;
		}
		
		*pTemp = lpCmdLine[iLoop];
		pTemp++;
		bRead = true;
	}
			

	
	HWND hwndRunningRE = FindWindow(NULL, pAppName);
	if (hwndRunningRE){
		//  Found the running instance
				
		
		//send a message to inform Rho of the requested index
		/*****TODO***********/
		//SendMessage(hwndRunningRE, WM_WINDOW_RESTORE, (WPARAM)iTabID, TRUE);	

		//  switch to it
		ShowWindow(hwndRunningRE, SW_RESTORE);
		SetForegroundWindow(hwndRunningRE);
	}
	delete[]pAppName;
		
	return 0;
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//ATOM MyRegisterClass(HINSTANCE hInstance, LPTSTR szWindowClass)
//{
//	WNDCLASS wc;
//
//	wc.style         = CS_HREDRAW | CS_VREDRAW;
//	wc.lpfnWndProc   = WndProc;
//	wc.cbClsExtra    = 0;
//	wc.cbWndExtra    = 0;
//	wc.hInstance     = hInstance;
//	wc.hIcon         = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_RHOLAUNCH));
//	wc.hCursor       = 0;
//	wc.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
//	wc.lpszMenuName  = 0;
//	wc.lpszClassName = szWindowClass;
//
//	return RegisterClass(&wc);
//}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
//BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
//{
//    HWND hWnd;
//    TCHAR szTitle[MAX_LOADSTRING];		// title bar text
//    TCHAR szWindowClass[MAX_LOADSTRING];	// main window class name
//
//    g_hInst = hInstance; // Store instance handle in our global variable
//
//#if defined(WIN32_PLATFORM_PSPC) || defined(WIN32_PLATFORM_WFSP)
//    // SHInitExtraControls should be called once during your application's initialization to initialize any
//    // of the device specific controls such as CAPEDIT and SIPPREF.
//    SHInitExtraControls();
//#endif // WIN32_PLATFORM_PSPC || WIN32_PLATFORM_WFSP
//
//    LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING); 
//    LoadString(hInstance, IDC_RHOLAUNCH, szWindowClass, MAX_LOADSTRING);
//
//#if defined(WIN32_PLATFORM_PSPC) || defined(WIN32_PLATFORM_WFSP)
//    //If it is already running, then focus on the window, and exit
//    hWnd = FindWindow(szWindowClass, szTitle);	
//    if (hWnd) 
//    {
//        // set focus to foremost child window
//        // The "| 0x00000001" is used to bring any owned windows to the foreground and
//        // activate them.
//        SetForegroundWindow((HWND)((ULONG) hWnd | 0x00000001));
//        return 0;
//    } 
//#endif // WIN32_PLATFORM_PSPC || WIN32_PLATFORM_WFSP
//
//    if (!MyRegisterClass(hInstance, szWindowClass))
//    {
//    	return FALSE;
//    }
//
//    hWnd = CreateWindow(szWindowClass, szTitle, WS_VISIBLE,
//        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
//
//    if (!hWnd)
//    {
//        return FALSE;
//    }
//
//#ifdef WIN32_PLATFORM_PSPC
//    // When the main window is created using CW_USEDEFAULT the height of the menubar (if one
//    // is created is not taken into account). So we resize the window after creating it
//    // if a menubar is present
//    if (g_hWndMenuBar)
//    {
//        RECT rc;
//        RECT rcMenuBar;
//
//        GetWindowRect(hWnd, &rc);
//        GetWindowRect(g_hWndMenuBar, &rcMenuBar);
//        rc.bottom -= (rcMenuBar.bottom - rcMenuBar.top);
//		
//        MoveWindow(hWnd, rc.left, rc.top, rc.right-rc.left, rc.bottom-rc.top, FALSE);
//    }
//#endif // WIN32_PLATFORM_PSPC
//
//    ShowWindow(hWnd, nCmdShow);
//    UpdateWindow(hWnd);
//
//#ifndef SHELL_AYGSHELL
//    if (g_hWndCommandBar)
//    {
//        CommandBar_Show(g_hWndCommandBar, TRUE);
//    }
//#endif // !SHELL_AYGSHELL
//
//    return TRUE;
//}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
//LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
//{
//    int wmId, wmEvent;
//    PAINTSTRUCT ps;
//    HDC hdc;
//
//#if defined(SHELL_AYGSHELL) && !defined(WIN32_PLATFORM_WFSP)
//    static SHACTIVATEINFO s_sai;
//#endif // SHELL_AYGSHELL && !WIN32_PLATFORM_WFSP
//	
//    switch (message) 
//    {
//        case WM_COMMAND:
//            wmId    = LOWORD(wParam); 
//            wmEvent = HIWORD(wParam); 
//            // Parse the menu selections:
//            switch (wmId)
//            {
//                case IDM_HELP_ABOUT:
//                    MessageBox(hWnd,g_Msg,L"Testing...",MB_OK);
//					//DialogBox(g_hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, About);
//                    break;
//#ifndef SHELL_AYGSHELL
//                case IDM_FILE_EXIT:
//                    DestroyWindow(hWnd);
//                    break;
//#endif // !SHELL_AYGSHELL
//#ifdef WIN32_PLATFORM_PSPC
//                case IDM_OK:
//                    SendMessage (hWnd, WM_CLOSE, 0, 0);				
//                    break;
//#endif // WIN32_PLATFORM_PSPC
//                default:
//                    return DefWindowProc(hWnd, message, wParam, lParam);
//            }
//            break;
//        case WM_CREATE:
//#ifndef SHELL_AYGSHELL
//            g_hWndCommandBar = CommandBar_Create(g_hInst, hWnd, 1);
//            CommandBar_InsertMenubar(g_hWndCommandBar, g_hInst, IDR_MENU, 0);
//            CommandBar_AddAdornments(g_hWndCommandBar, 0, 0);
//#endif // !SHELL_AYGSHELL
//#ifdef SHELL_AYGSHELL
//            SHMENUBARINFO mbi;
//
//            memset(&mbi, 0, sizeof(SHMENUBARINFO));
//            mbi.cbSize     = sizeof(SHMENUBARINFO);
//            mbi.hwndParent = hWnd;
//            mbi.nToolBarId = IDR_MENU;
//            mbi.hInstRes   = g_hInst;
//
//            if (!SHCreateMenuBar(&mbi)) 
//            {
//                g_hWndMenuBar = NULL;
//            }
//            else
//            {
//                g_hWndMenuBar = mbi.hwndMB;
//            }
//
//            // Initialize the shell activate info structure
//            memset(&s_sai, 0, sizeof (s_sai));
//            s_sai.cbSize = sizeof (s_sai);
//#endif // SHELL_AYGSHELL
//            break;
//        case WM_PAINT:
//            hdc = BeginPaint(hWnd, &ps);
//            
//            // TODO: Add any drawing code here...
//            
//            EndPaint(hWnd, &ps);
//            break;
//        case WM_DESTROY:
//#ifndef SHELL_AYGSHELL
//            CommandBar_Destroy(g_hWndCommandBar);
//#endif // !SHELL_AYGSHELL
//#ifdef SHELL_AYGSHELL
//            CommandBar_Destroy(g_hWndMenuBar);
//#endif // SHELL_AYGSHELL
//            PostQuitMessage(0);
//            break;
//
//#if defined(SHELL_AYGSHELL) && !defined(WIN32_PLATFORM_WFSP)
//        case WM_ACTIVATE:
//            // Notify shell of our activate message
//            SHHandleWMActivate(hWnd, wParam, lParam, &s_sai, FALSE);
//            break;
//        case WM_SETTINGCHANGE:
//            SHHandleWMSettingChange(hWnd, wParam, lParam, &s_sai);
//            break;
//#endif // SHELL_AYGSHELL && !WIN32_PLATFORM_WFSP
//
//        default:
//            return DefWindowProc(hWnd, message, wParam, lParam);
//    }
//    return 0;
//}

// Message handler for about box.
//INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
//{
//    switch (message)
//    {
//        case WM_INITDIALOG:
//#ifndef SHELL_AYGSHELL
//            RECT rectChild, rectParent;
//            int DlgWidth, DlgHeight;	// dialog width and height in pixel units
//            int NewPosX, NewPosY;
//
//            // trying to center the About dialog
//            if (GetWindowRect(hDlg, &rectChild)) 
//            {
//                GetClientRect(GetParent(hDlg), &rectParent);
//                DlgWidth	= rectChild.right - rectChild.left;
//                DlgHeight	= rectChild.bottom - rectChild.top ;
//                NewPosX		= (rectParent.right - rectParent.left - DlgWidth) / 2;
//                NewPosY		= (rectParent.bottom - rectParent.top - DlgHeight) / 2;
//				
//                // if the About box is larger than the physical screen 
//                if (NewPosX < 0) NewPosX = 0;
//                if (NewPosY < 0) NewPosY = 0;
//                SetWindowPos(hDlg, 0, NewPosX, NewPosY,
//                    0, 0, SWP_NOZORDER | SWP_NOSIZE);
//            }
//#endif // !SHELL_AYGSHELL
//#ifdef SHELL_AYGSHELL
//            {
//                // Create a Done button and size it.  
//                SHINITDLGINFO shidi;
//                shidi.dwMask = SHIDIM_FLAGS;
//                shidi.dwFlags = SHIDIF_DONEBUTTON | SHIDIF_SIPDOWN | SHIDIF_SIZEDLGFULLSCREEN | SHIDIF_EMPTYMENU;
//                shidi.hDlg = hDlg;
//                SHInitDialog(&shidi);
//            }
//#endif // SHELL_AYGSHELL
//
//            return (INT_PTR)TRUE;
//
//        case WM_COMMAND:
//#ifndef SHELL_AYGSHELL
//            if ((LOWORD(wParam) == IDOK) || (LOWORD(wParam) == IDCANCEL))
//#endif // !SHELL_AYGSHELL
//#ifdef SHELL_AYGSHELL
//            if (LOWORD(wParam) == IDOK)
//#endif
//            {
//                EndDialog(hDlg, LOWORD(wParam));
//                return (INT_PTR)TRUE;
//            }
//            break;
//
//        case WM_CLOSE:
//            EndDialog(hDlg, message);
//            return (INT_PTR)TRUE;
//
//    }
//    return (INT_PTR)FALSE;
//}
