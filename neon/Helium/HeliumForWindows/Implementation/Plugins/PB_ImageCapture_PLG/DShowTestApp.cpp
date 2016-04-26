//------------------------------------------------------------------
// FILENAME: DShowTestApp.cpp
//
// Copyright(c) 2003 Symbol Technologies Inc. All rights reserved.
//
// DESCRIPTION:	Defines the entry point for the application.
//
// NOTES:
//
// %IF Symbol_Internal
// AUTHOR: Wajra Attale	
// CREATION DATE:	01/25/2007
// DERIVED FROM:	New File
//
// EDIT HISTORY: No PVCS history for this file
// $Log:   T:/MPA2.0_sandbox/archives/SymbolValueAdd_mpa2/CCL/Camera/DShowCam/test/DShowTestApp/DShowTestApp.cpp-arc  $
//
//   Rev 1.0   Apr 18 2008 10:17:30   cheung
//Initial revision.
//
//   Rev 1.0   Dec 14 2007 07:21:00   sepalas
//Initial revision.
//
//%End
//------------------------------------------------------------------
#include "stdafx.h"
#include "DShowTestApp.h"
#include "DShowCam.h"
#include <windows.h>
#include <commctrl.h>
#include <Winuser.h>
#include <winbase.h>

// Global Variables:
CDShowCam		g_Prop;							// Create an Instance of DShowCam API
HWND			g_hwndMain;						// Main Window handler
HINSTANCE		g_hinstCurr;					// Current instance
HWND			g_hwndMenuBar;					// Menu bar handle
RECT			g_tRc;							// To store screen working area
HWND			g_hwndTemp;						// To hold Windoow handler temporary
wstring			g_wsaCamIDs[NO_OF_CAMs];		// Contains enumurated Camera IDs
ImgPropTbl		g_tImgTbl;						// Holds status of an Image property
CamPropTbl		g_tCamTbl;						// Holds status of a Camera property
CusPropTbl		g_tCusTbl;						// Holds status of a Custom property
PropType		g_ePType;						// Holds globally selected property type, IMG, CAM or CUS
HMENU			g_hmnuCurr;						// Holds the handler for Current Menu
HMENU			g_hmnuStillSub;					// Holds the handler for Still Resolution Sub Menu
HMENU			g_hmnuVideoSub;					// Holds the handler for Video Resolution Sub Menu
HMENU*          g_ahmnuVdoColorFmts;
HMENU*          g_ahmnuStillColorFmts;
BOOL			g_bVideoMode;					// Holds the mode of operation Video or Still
INT				g_nNoOfVdoCap;					// Holds number of enumurated Video capabilities
INT				g_nNoOfStillCap;				// Holds number of enumurated Still capabilities
ImageRes        g_tImgRes;						// To keep track of the selected resolution on popup menu
BOOL			g_bVMod;
wstring         g_wsColorFmt;
INT             g_ResIdx;
BOOL			g_bStressTest;
INT				g_nDuration;
INT				g_nDueTime;
BOOL			g_bNowRecording;				// This is TRUE if a video stream is being recorded


// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE, LPTSTR);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
HWND				MyCreateTrackBar(HWND hWndParent, DWORD dwStyles, WORD wID, HINSTANCE hInst,\
						int x, int y, int width, int height, LONG lPageSize, LONG lPosition, LONG lRange,\
						LONG lFreq, LONG lSelection, BOOL bRedraw);
void	Invoke(HWND hWnd, RECT g_tRc, wstring Cam);
void	BuildMenuEx(ImgFmt tIFmt0, ImgFmt* ptIFmts, INT nNoOfCap, BOOL bIsVideo);
void	UpdateUI(LONG lVal, LONG lMin, LONG lMax, LONG lStep);
BOOL	Set_ImgRes(INT nIdx);
BOOL	Set_ImgColorFmt(INT nIdx);
BOOL	Set_Timers(HWND hWin, INT nDuration);
BOOL	Set_Duration(INT nDue, INT nPos);
HRESULT Get_CamProp(INT nProp, INT nId);
HRESULT Get_ImgProp(INT nProp, INT nId);

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPTSTR    lpCmdLine,
                   int       nCmdShow)
{
	MSG msg;
	
	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow)) 
	{
		return FALSE;
	}
		
	HACCEL hAccelTable;
	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DSHOWTESTAPP));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance, LPTSTR szWindowClass)
{
	WNDCLASS wc;

	wc.style         = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc   = WndProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = hInstance;
	wc.hIcon         = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DSHOWTESTAPP_WIN32));
	wc.hCursor       = 0;
	wc.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName  = 0;
	wc.lpszClassName = szWindowClass;

	return RegisterClass(&wc);
}

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
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	INT nNo;
	RECT rc;
	ImgFmt fmt0;
	ImgFmt* fmt1;
	ImageRes tRes;
	TCHAR szTitle[MAX_LOADSTRING];			// Title bar text
    TCHAR szWindowClass[MAX_LOADSTRING];	// Main window class name
	INT nX, nY, nSWidth, nSHeight;			// These cordinates are used to align windows controls over the 
											// available screen area irrespective of the LCD quality
	
	//Initailize global variavles
	g_hinstCurr = hInstance;		 // Store instance handle in our global variable
	g_ahmnuVdoColorFmts = NULL;
	g_ahmnuStillColorFmts = NULL;
	g_bVideoMode = TRUE;			 // Set to VideoMode as the default mode of operation
	g_bVMod = TRUE;
	g_wsColorFmt = L"";
	g_tImgRes.nWidth = 0;
	g_tImgRes.nHeight = 0;
	g_bStressTest = FALSE;
	g_bNowRecording = FALSE;
	g_hwndMain = NULL;
	g_hwndMenuBar = NULL;
		
	
	// SHInitExtraControls should be called once during your application's 
	// initialization to initialize any of the device specific controls
	// such as CAPEDIT and SIPPREF.
    SHInitExtraControls();

    LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING); 
    LoadString(hInstance, IDC_DSHOWTESTAPP, szWindowClass, MAX_LOADSTRING);

    //If it is already running, then focus on the window, and exit
    g_hwndMain = FindWindow(szWindowClass, szTitle);	
    if(g_hwndMain) 
    {
        // set focus to foremost child window
        // The "| 0x00000001" is used to bring any owned windows to the foreground and
        // activate them.
        SetForegroundWindow((HWND)((ULONG) g_hwndMain | 0x00000001));
        return 0;
    } 

    if(!MyRegisterClass(hInstance, szWindowClass))
    {
    	return FALSE;
    }

	// Creating the main window
    g_hwndMain = CreateWindow(szWindowClass, szTitle, WS_VISIBLE,\
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,\
														NULL, NULL, hInstance, NULL);

    if (!g_hwndMain)
    {
        return FALSE;
    }

	// When the main window is created using CW_USEDEFAULT the height of the menubar (if one
    // is created is not taken into account). So we resize the window after creating it
    // if a menubar is present
	if (g_hwndMenuBar)
    {
        RECT rc;
        RECT rcMenuBar;

        GetWindowRect(g_hwndMain, &rc);
        GetWindowRect(g_hwndMenuBar, &rcMenuBar);
		rc.bottom -= (rcMenuBar.bottom - rcMenuBar.top);
		
        MoveWindow(g_hwndMain, rc.left, rc.top, rc.right-rc.left, rc.bottom-rc.top, FALSE);
		
		g_tRc.top = rc.top;
		g_tRc.left = rc.left;
		g_tRc.bottom = rc.bottom;
		g_tRc.right = rc.right;
    }
	else
	{
		//Get the allowed work area for UI
		SystemParametersInfo(SPI_GETWORKAREA, 0, &g_tRc, 0);
	}

	//Initialize the screen aligning cordinates 
	nX = MARGIN_GAP;
	nY = MARGIN_GAP;
	nSWidth = (g_tRc.right-g_tRc.left-2*MARGIN_GAP);
	nSHeight = (g_tRc.bottom-g_tRc.top-2*MARGIN_GAP);
		
	//Locate the camera device, Implement and Run Directshow filter graph
	g_Prop.FindFirstCam(&g_wsaCamIDs[0]);
	g_Prop.FindNextCam(&g_wsaCamIDs[1]);
	rc.left = nX;
	rc.top = nY;
	rc.bottom = nY+(nSHeight-2*MARGIN_GAP)*3/4;
	rc.right = nX+nSWidth;

	//Here prompt user to select the camera from a drop down and 
	//........
	//........
	//........
	// Here the First found camera is taken as the default capture device
	Invoke(g_hwndMain, rc, g_wsaCamIDs[0].c_str()); 
		
	LoadString(hInstance, STR_EDIT, szWindowClass, MAX_LOADSTRING); 
    LoadString(hInstance, STR_BLANK, szTitle, MAX_LOADSTRING);
	CreateWindow(szWindowClass, szTitle, WS_CHILD|WS_VISIBLE|ES_LEFT|WS_BORDER|WS_DISABLED, nX, nY, nSWidth, (nSHeight-2*MARGIN_GAP)*3/4, g_hwndMain, (HMENU)IDC_PREVIEW, hInstance, NULL);
	nY +=  (nSHeight-2*MARGIN_GAP)*3/4 + MARGIN_GAP;
	
	LoadString(hInstance, STR_BUTTON, szWindowClass, MAX_LOADSTRING); 
    LoadString(hInstance, STR_CAPTURE, szTitle, MAX_LOADSTRING);
	CreateWindow(szWindowClass, szTitle, WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON, nX, nY, (nSWidth-2*MARGIN_GAP)/4 , (nSHeight-2*MARGIN_GAP)/12, g_hwndMain, (HMENU)IDC_CAPTURE, hInstance, NULL);
	nX += (nSWidth-2*MARGIN_GAP)/4 + MARGIN_GAP;
		
	LoadString(hInstance, STR_STATIC, szWindowClass, MAX_LOADSTRING); 
    LoadString(hInstance, STR_VMODE, szTitle, MAX_LOADSTRING);
	CreateWindow(szWindowClass, szTitle, WS_CHILD|WS_VISIBLE|SS_CENTER, nX, nY, (nSWidth-2*MARGIN_GAP)/2, (nSHeight-2*MARGIN_GAP)/12, g_hwndMain, (HMENU)IDC_MODE, hInstance, NULL);
	nX += (nSWidth-2*MARGIN_GAP)/2 + MARGIN_GAP;
	
	LoadString(hInstance, STR_COMBOBOX, szWindowClass, MAX_LOADSTRING); 
    LoadString(hInstance, STR_BLANK, szTitle, MAX_LOADSTRING);
	CreateWindow(szWindowClass, szTitle, WS_CHILD|WS_VISIBLE|SS_LEFT, nX, nY, (nSWidth-2*MARGIN_GAP)/4, (nSHeight-2*MARGIN_GAP)/12, g_hwndMain, (HMENU)IDC_CAMID, hInstance, NULL);
	
	nX = MARGIN_GAP;
	nY += (nSHeight-2*MARGIN_GAP)/12 + MARGIN_GAP;

	LoadString(hInstance, STR_BLANK, szTitle, MAX_LOADSTRING);
	CreateWindowEx(WS_EX_STATICEDGE, TRACKBAR_CLASS, szTitle, WS_CHILD|WS_VISIBLE|TBS_HORZ, nX, nY, nSWidth/2, (nSHeight-2*MARGIN_GAP)/12, g_hwndMain, (HMENU)IDC_SLIDER, hInstance, NULL);
	nX += nSWidth/2 + MARGIN_GAP;

	LoadString(hInstance, STR_EDIT, szWindowClass, MAX_LOADSTRING); 
    LoadString(hInstance, STR_BLANK, szTitle, MAX_LOADSTRING);
	CreateWindow(szWindowClass, szTitle, WS_CHILD|WS_VISIBLE|ES_LEFT|WS_BORDER|ES_NUMBER|WS_DISABLED, nX, nY, nSWidth/4, (nSHeight-2*MARGIN_GAP)/12, g_hwndMain, (HMENU)IDC_POS, hInstance, NULL);

	nY += (nSHeight-2*MARGIN_GAP)/12;
	nX = MARGIN_GAP;
	LoadString(hInstance, STR_STATIC, szWindowClass, MAX_LOADSTRING); 
    LoadString(hInstance, PRO_BRIGHTNESS, szTitle, MAX_LOADSTRING);
	CreateWindowEx(WS_EX_STATICEDGE, szWindowClass, szTitle, WS_CHILD|WS_VISIBLE|SS_CENTER, nX, nY, nSWidth/2, (nSHeight-2*MARGIN_GAP)/12, g_hwndMain, (HMENU)IDC_PROP, hInstance, NULL);
	
	nY = nY;
	nX += nSWidth/2 + MARGIN_GAP;
	CreateWindow(L"BUTTON", L"Stress ", WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX|WS_DISABLED, nX, nY, nSWidth/4, (nSHeight-2*MARGIN_GAP)/12, g_hwndMain, (HMENU)IDC_STRESS, hInstance, NULL);
	
	nY += 2*MARGIN_GAP;
	nX += nSWidth/4 + MARGIN_GAP;
	CreateWindow(L"EDIT", L"00:00:00", WS_CHILD|WS_VISIBLE|ES_LEFT|WS_DISABLED|ES_READONLY|WS_BORDER, nX, nY, (nSWidth-nX+MARGIN_GAP), (nSHeight-2*MARGIN_GAP)/15, g_hwndMain, (HMENU)IDC_TIMER, hInstance, NULL);

	// Retrive Video and Still capabilities and populate Dynamic menu extension
	g_Prop.EnumFirstCap(V, &fmt0, &nNo);
	g_nNoOfVdoCap = nNo;
	fmt1 = new ImgFmt[nNo-1];
	g_Prop.EnumOtherCap(V, fmt1, nNo);
	BuildMenuEx(fmt0, fmt1, nNo, TRUE);
	Set_ImgRes(0);							//Setting CAPTURE_PIN to Res. Idx zero
	delete [] fmt1;	

	g_Prop.EnumFirstCap(S, &fmt0, &nNo);
	g_nNoOfStillCap = nNo;
	fmt1 = new ImgFmt[nNo-1];
	g_Prop.EnumOtherCap(S, fmt1, nNo);
	BuildMenuEx(fmt0, fmt1, nNo, FALSE);
	tRes.nHeight=fmt0.nMaxHeight;
	tRes.nWidth=fmt0.nMaxWidth;
	g_Prop.Set_Resolution(&tRes, S);		//Setting STILL_PIN to Res. Idx zero
	delete [] fmt1;	
	
	//Enumuration of PREVIEW_PIN First capability and keep it always 
	//with the lowest resolution
	g_Prop.EnumFirstCap(P, &fmt0, &nNo);
	tRes.nHeight=fmt0.nMaxHeight;
	tRes.nWidth=fmt0.nMaxWidth;
	g_Prop.Set_Resolution(&tRes, P);		//Setting PREVIEW_PIN to Res. Idx zero
		
	//Checking the Image properties supported by the camera driver
	for(int i=0; i<NO_OF_IMAGE_PROP; i++)
	{
		TCHAR waTemp[BUF_LENGTH];
		HRESULT hr;

		g_tImgTbl.vProp = IProp[i];
		hr = g_Prop.Get_PropRng(IMG, &g_tImgTbl);
		if(hr == E_PROP_ID_UNSUPPORTED)
		{
			wsprintf(waTemp, L"\n--Image Property Id:-%d NOT Supported----\n",i);
			OutputDebugString(waTemp);
			
			g_hmnuCurr = (HMENU) SendMessage(g_hwndMenuBar, SHCMBM_GETSUBMENU, 0, IDM_SETTINGS);
			EnableMenuItem(GetSubMenu(g_hmnuCurr, 9), i, MF_BYPOSITION|MF_GRAYED);
		}
	}

	//Checking the Camera properties supported by the camera driver
	for(int i=0; i<NO_OF_CAMERA_PROP; i++)
	{
		TCHAR waTemp[BUF_LENGTH];
		HRESULT hr;

		g_tCamTbl.p = CProp[i];
		hr = g_Prop.Get_PropRng(CAM, &g_tCamTbl);
		if(hr == E_PROP_ID_UNSUPPORTED)
		{
			wsprintf(waTemp, L"\n--Camera Property Id:-%d NOT Supported----\n",i);
			OutputDebugString(waTemp);
			
			g_hmnuCurr = (HMENU) SendMessage(g_hwndMenuBar, SHCMBM_GETSUBMENU, 0, IDM_SETTINGS);
			EnableMenuItem(GetSubMenu(g_hmnuCurr, 8), i, MF_BYPOSITION|MF_GRAYED);
		}
	}

	//Set Initial property as Brightness
	Get_ImgProp(PRO_BRIGHTNESS, 0);
	
	//Check menu Items for Video Resolution and ColorFormat menu
	/*
	g_hmnuCurr = g_hmnuVideoSub;
	CheckMenuRadioItem(g_hmnuCurr, 0, (g_nNoOfVdoCap-1), 0, MF_BYPOSITION);
	g_hmnuCurr = g_ahmnuVdoColorFmts[0];
	CheckMenuRadioItem(g_hmnuCurr, 0, (g_nNoOfVdoCap-1), 0, MF_BYPOSITION);
	*/
	//Check menu Items for Still Resolution and ColorFormat menu
	g_hmnuCurr = g_hmnuStillSub;
	CheckMenuRadioItem(g_hmnuCurr, 0, (g_nNoOfStillCap-1), 0, MF_BYPOSITION);
	g_hmnuCurr = g_ahmnuStillColorFmts[0];
	CheckMenuRadioItem(g_hmnuCurr, 0, (g_nNoOfStillCap-1), 0, MF_BYPOSITION);
	
	//Display the selected Camera Name
	g_hwndTemp = GetDlgItem(g_hwndMain,IDC_CAMID);
	SendMessage(g_hwndTemp, WM_SETTEXT, 0, (LPARAM)g_wsaCamIDs[0].c_str());	

	// Redraw
    InvalidateRect(g_hwndMain, NULL, TRUE);
    UpdateWindow(g_hwndMain);
	

	ShowWindow(g_hwndMain, nCmdShow);
    UpdateWindow(g_hwndMain);
    SetForegroundWindow(g_hwndMain);

	return TRUE;
}

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
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    INT wmId, wmEvent;
	HRESULT hr;
    PAINTSTRUCT ps;
    HDC hdc;
	wstring CapMode, Capture;
	TCHAR waText[BUF_LENGTH], text2[BUF_LENGTH], title[BUF_LENGTH];
	static SHACTIVATEINFO s_sai;
	DWORD dTickCount;
	
    switch (message) 
    {
		case WM_SIZE:
			 if(wParam == SIZE_MINIMIZED)
			 	 SendMessage (hWnd, WM_CLOSE, 0, 0);	
			 break;
		 
		case WM_COMMAND:
			wmId    = LOWORD(wParam); 
            wmEvent = HIWORD(wParam); 
			
			// Parse the menu selections:
            switch (wmId)
            {
				case IDC_CAPTURE:
					if (wmEvent == BN_CLICKED) 
					{
						//Handle the CAPTURE button click here 
						g_hwndTemp = GetDlgItem(g_hwndMain, IDC_CAPTURE);
						if(!g_bVideoMode)
						{
							EnableWindow(g_hwndTemp, FALSE);
							dTickCount=GetTickCount();
							wsprintf(waText, L"\n **** Time Stamp[%d]: Before Calling the API CaptureStill() function\n", dTickCount);
							OutputDebugString(waText);
							hr = g_Prop.CaptureStill();							
							dTickCount=GetTickCount();
							wsprintf(waText, L"\n **** Time Stamp[%d]: After API CaptureStill() function return\n", dTickCount);
							OutputDebugString(waText);
							if(SUCCEEDED(hr))
							{	
								LoadString(g_hinstCurr, IDS_OK, waText, MAX_LOADSTRING);
								LoadString(g_hinstCurr, STR_STILL, title, MAX_LOADSTRING);
								EnableWindow(g_hwndTemp, TRUE);								
								if(!g_bStressTest)
									MessageBox(hWnd, waText, title, MB_ICONEXCLAMATION|MB_OK); 
							}
						
						}
						else if(!g_bNowRecording)
						{
							g_bNowRecording = TRUE;
							g_hmnuCurr = (HMENU) SendMessage(g_hwndMenuBar, SHCMBM_GETSUBMENU, 0, IDM_SETTINGS);
							EnableMenuItem(g_hmnuCurr, ID_SETTINGS_FILE, MF_BYCOMMAND|MF_GRAYED);
							hr=g_Prop.StartCapture();
							//if(SUCCEEDED(hr))
							{
								LoadString(g_hinstCurr, STR_STOP, waText, MAX_LOADSTRING);
								SendMessage(g_hwndTemp, WM_SETTEXT, 0, (LPARAM)(LPCTSTR)waText);
							}
						}
						else
						{
							g_bNowRecording = FALSE;
							EnableWindow(g_hwndTemp, FALSE);								
							g_Prop.StopCapture();
							EnableMenuItem(g_hmnuCurr, ID_SETTINGS_FILE, MF_BYCOMMAND);
							LoadString(g_hinstCurr, STR_CAPTURE, waText, MAX_LOADSTRING);
							SendMessage(g_hwndTemp, WM_SETTEXT, 0, (LPARAM)(LPCTSTR)waText);
							EnableWindow(g_hwndTemp, TRUE);								
						}	
			
					}
					break;
				
				case IDC_STRESS:
					if(wmEvent == BN_CLICKED)
					{
						g_hwndTemp = GetDlgItem(g_hwndMain,IDC_STRESS);
						LRESULT lResult = SendMessage((HWND) g_hwndTemp, (UINT) BM_GETCHECK, 0, 0);  
						g_hwndTemp = GetDlgItem(g_hwndMain,IDC_CAPTURE);

						if(lResult == BST_CHECKED)
						{
							g_bStressTest = TRUE;
							EnableWindow(g_hwndTemp, FALSE);   //Disable the CAPTURE button
							Set_Timers(hWnd, g_nDuration); 
						}
						else if(lResult == BST_UNCHECKED)
						{
							TCHAR waTemp[BUF_LENGTH];

							g_bStressTest = FALSE;
							KillTimer(hWnd, MY_TIMER);
							KillTimer(hWnd, MY_TIMER2);
							EnableWindow(g_hwndTemp, TRUE);		//Enable the CAPTURE button
							
							g_hwndTemp = GetDlgItem(g_hwndMain, IDC_TIMER);
							wsprintf(waTemp, L"00:00:%.2d", g_nDuration);
							SendMessage(g_hwndTemp, WM_SETTEXT, 0, (LPARAM)waTemp);
							g_nDueTime = g_nDuration;
						}
			
					}
					break;
				
				case ID_MODE_VIDEO:
					g_hmnuCurr = (HMENU) SendMessage(g_hwndMenuBar, SHCMBM_GETSUBMENU, 0, IDM_SETTINGS);
					EnableMenuItem(g_hmnuCurr, ID_SETTINGS_FILE, MF_BYCOMMAND);
					g_hwndTemp = GetDlgItem(g_hwndMain,IDC_MODE);
					LoadString(g_hinstCurr, STR_VMODE, waText, MAX_LOADSTRING);
					SendMessage(g_hwndTemp, WM_SETTEXT, 0, (LPARAM)(LPCTSTR)waText);
					LoadString(g_hinstCurr, STR_MTITLE, text2, MAX_LOADSTRING);
#ifdef DEBUG_MODE
					MessageBox(hWnd, waText, text2, MB_ICONEXCLAMATION|MB_OK); 
#endif
					g_bVideoMode = TRUE;
					RemoveMenu(g_hmnuCurr, 11, MF_BYPOSITION);
					RemoveMenu(g_hmnuCurr, 11, MF_BYPOSITION);
					AppendMenu(g_hmnuCurr, MF_STRING | MF_POPUP, (UINT)g_hmnuVideoSub, L"&Resolution");
					AppendMenu(g_hmnuCurr, MF_STRING | MF_POPUP, (UINT)g_ahmnuVdoColorFmts[0], L"&ColorFmts");	
					//Set_ImgRes(0);
					break; 
				
				case ID_MODE_STILL:
					g_hmnuCurr = (HMENU) SendMessage(g_hwndMenuBar, SHCMBM_GETSUBMENU, 0, IDM_SETTINGS);
					EnableMenuItem(g_hmnuCurr, ID_SETTINGS_FILE, MF_BYCOMMAND|MF_GRAYED);
					g_hwndTemp = GetDlgItem(g_hwndMain,IDC_MODE);
					LoadString(g_hinstCurr, STR_SMODE, waText, MAX_LOADSTRING);
					SendMessage(g_hwndTemp, WM_SETTEXT, 0, (LPARAM)(LPCTSTR)waText);	
					LoadString(g_hinstCurr, STR_MTITLE, text2, MAX_LOADSTRING);
#ifdef DEBUG_MODE
					MessageBox(hWnd, waText, text2, MB_ICONEXCLAMATION|MB_OK); 
#endif
					g_bVideoMode = FALSE;
					RemoveMenu(g_hmnuCurr, 11, MF_BYPOSITION);
					RemoveMenu(g_hmnuCurr, 11, MF_BYPOSITION);
					AppendMenu(g_hmnuCurr, MF_STRING | MF_POPUP, (UINT)g_hmnuStillSub, L"&Resolution");
					AppendMenu(g_hmnuCurr, MF_STRING | MF_POPUP, (UINT)g_ahmnuStillColorFmts[0], L"&ColorFmts");	
					//Set_ImgRes(0);
					break;

				//Setting a Time duration(in Seconds)for Stress testing
				case ID_DURATION_10:
					Set_Duration(10, 0);
					break;
				case ID_DURATION_15:
					Set_Duration(15, 1);
					break;
				case ID_DURATION_20:
					Set_Duration(20, 2);
					break;
				case ID_DURATION_25:
					Set_Duration(25, 3);
					break;
				case ID_DURATION_30:
					Set_Duration(30, 4);
					break;
				case ID_DURATION_35:
					Set_Duration(35, 5);
					break;
				case ID_DURATION_40:
					Set_Duration(40, 6);
					break;
				case ID_DURATION_45:
					Set_Duration(45, 7);
					break;
				case ID_DURATION_60:
					Set_Duration(60, 8);
					break;
				case ID_DURATION_NONE:
					Set_Duration(-1, -1);
					break;
		
				case ID_SETTINGS_FILE:
					TCHAR szFilter[MAX_FILTER];
					TCHAR szTitle[MAX_TITLE];
					TCHAR szExt[MAX_EXT];
					TCHAR szFile[MAX_PATH];
					OPENFILENAME ofn;
					
					wsprintf(szFile,TEXT("\0"));
					memset(&ofn, 0, sizeof (ofn));
					ofn.lStructSize  = sizeof (OPENFILENAME);
					ofn.hwndOwner = hWnd;
					ofn.lpstrFile = szFile;
					ofn.nMaxFile = MAX_PATH;  
					LoadString(g_hinstCurr, STR_FEXT, szExt, MAX_LOADSTRING);
					ofn.lpstrDefExt = szExt;
					LoadString(g_hinstCurr, STR_FFILTER, szFilter, MAX_LOADSTRING);
					wcscat(&szFilter[wcslen(szFilter)+1], szExt);
					wcscat(&szFilter[wcslen(szFilter)+1], L"\0");
					ofn.lpstrFilter =  szFilter; //TEXT("WMV Files (*.wmv)\0*.wmv\0\0");
					LoadString(g_hinstCurr, STR_FOTITLE, szTitle, MAX_LOADSTRING);
					ofn.lpstrTitle = szTitle;
					ofn.Flags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST; 
										
					if(GetSaveFileName (&ofn))  
					{
#ifdef DEBUG_MODE
						    MessageBox (hWnd, ofn.lpstrFile, szTitle, MB_OK);
#endif
							g_Prop.Set_VdoFileName(ofn.lpstrFile);
	 				}
    				else
					{
							LoadString(g_hinstCurr, STR_FALSE, waText, MAX_LOADSTRING);
							MessageBox (hWnd, waText, szTitle, MB_OK);
					}
					
					break;

				case ID_SETTINGS_HORIZONTAL:
					g_hmnuCurr = (HMENU) SendMessage(g_hwndMenuBar, SHCMBM_GETSUBMENU, 0, IDM_SETTINGS);
					hr = g_Prop.Set_Flip(TRUE);
					if(SUCCEEDED(hr))
					{
						CheckMenuItem(g_hmnuCurr, ID_SETTINGS_HORIZONTAL, MF_BYCOMMAND|MF_CHECKED);
						CheckMenuItem(g_hmnuCurr, ID_SETTINGS_VERTICAL, MF_BYCOMMAND|MF_UNCHECKED);
					}
					break;
				case ID_SETTINGS_VERTICAL:
					g_hmnuCurr = (HMENU) SendMessage(g_hwndMenuBar, SHCMBM_GETSUBMENU, 0, IDM_SETTINGS);
					hr = g_Prop.Set_Flip(FALSE);
					if(SUCCEEDED(hr))
					{
						CheckMenuItem(g_hmnuCurr, ID_SETTINGS_VERTICAL, MF_BYCOMMAND|MF_CHECKED);
						CheckMenuItem(g_hmnuCurr, ID_SETTINGS_HORIZONTAL, MF_BYCOMMAND|MF_UNCHECKED);
					}
					break;

				//Camera Property
				case ID_CAMERA_PAN:
					Get_CamProp(PRO_PAN, 0);
					break;
				case ID_CAMERA_TILT:
					Get_CamProp(PRO_TILT, 1);
					break;
				case ID_CAMERA_ROLL:
					Get_CamProp(PRO_ROLL, 2);
					break;
				case ID_CAMERA_ZOOM:
					Get_CamProp(PRO_ZOOM, 3);
					break;
				case ID_CAMERA_EXPOSURE:
					Get_CamProp(PRO_EXPOSURE, 4);
					break;
				case ID_CAMERA_IRIS:
					Get_CamProp(PRO_IRIS, 5);
					break;
				case ID_CAMERA_FOCUS:
					Get_CamProp(PRO_FOCUS, 6);
					break;
				case ID_SETTINGS_FLASH:
					//g_hmnuCurr = (HMENU) SendMessage(g_hwndMenuBar, SHCMBM_GETSUBMENU, 0, IDM_SETTINGS);
					//CheckMenuRadioItem(g_hmnuCurr, 0, 12, 2, MF_BYPOSITION);
					Get_CamProp(PRO_FLASH, 7);
					break;

				//Image Property
				case ID_IMAGE_BRIGHTNESS:
					Get_ImgProp(PRO_BRIGHTNESS, 0);
					break;
				case ID_IMAGE_CONTRAST:
					Get_ImgProp(PRO_CONTRAST, 1);
					break;
				case ID_IMAGE_HUE:
					Get_ImgProp(PRO_HUE, 2);
					break;
				case ID_IMAGE_SATURATION:
					Get_ImgProp(PRO_SATURATION, 3);
					break;
				case ID_IMAGE_SHARPNESS:
					Get_ImgProp(PRO_SHARPNESS, 4);
					break;
				case ID_IMAGE_GAMMA:
					Get_ImgProp(PRO_GAMMA, 5);
					break;
				case ID_IMAGE_COLORENABLE:
					Get_ImgProp(PRO_COLORENABLE, 6);
					break;
				case ID_IMAGE_WHITEBALANCE:	
					Get_ImgProp(PRO_WHITEBALANCE, 7);
					break;
				case ID_IMAGE_BACKLIGHTCOMP:
					Get_ImgProp(PRO_BACKLIGHTCOMP, 8);
					break;
				case ID_IMAGE_GAIN:
					Get_ImgProp(PRO_GAIN, 9);
					break;
		
				//Setting Image Resolution
				case ID_RESOLUTION_0:
					Set_ImgRes(0);
					break;
				case ID_RESOLUTION_1:
					Set_ImgRes(1);
					break;
				case ID_RESOLUTION_2:
					Set_ImgRes(2);
					break;
				case ID_RESOLUTION_3:
					Set_ImgRes(3);
					break;
				case ID_RESOLUTION_4:
					Set_ImgRes(4);
					break;
				case ID_RESOLUTION_5:
					Set_ImgRes(5);
					break;
				case ID_RESOLUTION_6:
					Set_ImgRes(6);
					break;
				case ID_RESOLUTION_7:
					Set_ImgRes(7);
					break;
				case ID_RESOLUTION_8:
					Set_ImgRes(8);
					break;
				case ID_RESOLUTION_9:
					Set_ImgRes(9);
					break;
				
				//Setting Image Color Format
				case ID_RESOLUTION_X_0:
					Set_ImgColorFmt(0);
					break;
				case ID_RESOLUTION_X_1:
					Set_ImgColorFmt(1);
					break;
				case ID_RESOLUTION_X_2:
					Set_ImgColorFmt(2);
					break;
				case ID_RESOLUTION_X_3:
					Set_ImgColorFmt(3);
					break;

				//Symbol Custom property
				case ID_CUSTOM_AIMING:
					g_hwndTemp = GetDlgItem(g_hwndMain,IDC_PROP);
					LoadString(g_hinstCurr, PRO_AIM, waText, MAX_LOADSTRING);
					SendMessage(g_hwndTemp, WM_SETTEXT, 0, (LPARAM)(LPCTSTR)waText);

					g_tCusTbl.p = CUS_PROPERTY_AIMING;
					g_ePType = CUS;
					g_Prop.Get_PropRng(CUS, &g_tCusTbl);
					UpdateUI(g_tCusTbl.plVal, g_tCusTbl.plMin, g_tCusTbl.plMax, g_tCusTbl.plDelta);
					break;
				case ID_CUSTOM_THUMBNAILS:
					
					SYSTEMTIME st;
					GetLocalTime(&st);
					TCHAR day[10];
					TCHAR DName[1024];
					wsprintf(day, L"%.2d%.2d%.2d", st.wMonth, st.wDay, st.wYear%100);
					wsprintf(DName, L"\\My Documents\\My Pictures\\%s", day);
	
					TCHAR szFileName[MAX_PATH];
					OPENFILENAMEEX ofnex;// = {0};
		
					ofnex.lStructSize     = sizeof(ofnex);
					ofnex.hwndOwner       = hWnd;
					ofnex.lpstrFile       = szFileName;
					ofnex.nMaxFile        = sizeof(szFileName) / sizeof(szFileName[0]);
					ofnex.lpstrFilter     = TEXT("JPG Files (*.jpg)\0*.jpg\0");
					ofnex.lpstrTitle      = TEXT("Photos of the day");
					ofnex.ExFlags         = OFN_EXFLAG_THUMBNAILVIEW;
					ofnex.lpstrInitialDir = DName;//L"\\My Documents\\My Pictures\\010107";//NULL
					
					if(GetOpenFileNameEx(&ofnex))
					{
						// The selected file name is in szFileName
					}
					break;

	            default:
                    return DefWindowProc(hWnd, message, wParam, lParam);
            }
            break;
        
		case WM_HSCROLL:
			if(wParam == TB_ENDTRACK) 
			{
				HRESULT hr;
				wstring PropStr;
				WCHAR temp[BUF_LENGTH];

				g_hwndTemp = GetDlgItem(g_hwndMain, IDC_PROP);
				SendMessage(g_hwndTemp, WM_GETTEXT, (WPARAM)STR_LENGHT, (LPARAM)PropStr.c_str());

				g_hwndTemp = GetDlgItem(g_hwndMain, IDC_SLIDER);
				DWORD pos = SendMessage(g_hwndTemp, TBM_GETPOS, 0, 0);
				
				g_hwndTemp = GetDlgItem(g_hwndMain, IDC_POS);
				wsprintf(temp,L"%d", pos);
				SendMessage(g_hwndTemp, WM_SETTEXT, 0, (LPARAM)temp);

#ifdef DEBUG_MODE
				LoadString(g_hinstCurr, STR_DEMARKATE, waText, MAX_LOADSTRING);
				OutputDebugString(waText);
				OutputDebugString(L"|");
				OutputDebugString(PropStr.c_str());
				OutputDebugString(L"|");
				OutputDebugString(waText);
#endif
				if(g_ePType == IMG)
				{
					g_tImgTbl.plVal = (LONG)pos;
					hr = g_Prop.Set_PropVal(IMG, &g_tImgTbl);
				}
				else if(g_ePType == CAM)
				{	
					g_tCamTbl.plVal = (LONG)pos;
					hr = g_Prop.Set_PropVal(CAM, &g_tCamTbl);
				}
				else 
				{
					g_tCusTbl.plVal = (LONG)pos;
					hr = g_Prop.Set_PropVal(CUS, &g_tCusTbl);
				}

				if(SUCCEEDED(hr))
					OutputDebugString(L"\n--------Property Value Set OK----------");
				else
					OutputDebugString(L"\n--------Property Value Set FAIL--------");

			}
			break;

		case WM_CREATE:
			SHMENUBARINFO info;
			memset(&info, 0, sizeof(SHMENUBARINFO));
			info.cbSize = sizeof(info);
			info.hwndParent = hWnd;
			info.dwFlags = SHCMBF_HIDESIPBUTTON|SHCMBF_COLORBK;
			info.nToolBarId = IDR_MENU;
			info.hInstRes = g_hinstCurr;
					
			if (!SHCreateMenuBar(&info)) 
            {
                g_hwndMenuBar = NULL;
            }
            else
            {
                g_hwndMenuBar = info.hwndMB;
		    }
			// Initialize the shell activate info structure
            memset(&s_sai, 0, sizeof (s_sai));
            s_sai.cbSize = sizeof (s_sai);
			break;
        
		case WM_TIMER:
			if(wParam==MY_TIMER2)
			{
				TCHAR waTemp[BUF_LENGTH];

				if(!g_bVideoMode&&g_bStressTest)				
				{
					g_hwndTemp = GetDlgItem(g_hwndMain, IDC_TIMER);
					wsprintf(waTemp,L"00:00:%.2d",(--g_nDueTime));
					SendMessage(g_hwndTemp, WM_SETTEXT, 0, (LPARAM)waTemp);
				}
			}
			else if(wParam==MY_TIMER)
			{
				if(!g_bVideoMode&&g_bStressTest)
				{
					g_nDueTime = g_nDuration;
					g_Prop.CaptureStill();
				}
			}
			break;

		case WM_PAINT:
            hdc = BeginPaint(hWnd, &ps);
#ifdef DEBUG_MODE
			LoadString(g_hinstCurr, STR_WM_PAINT, waText, MAX_LOADSTRING);
			OutputDebugString(waText);
#endif
            // TODO: Add any drawing code here...
            EndPaint(hWnd, &ps);
            break;
        
		case WM_DESTROY:
			CommandBar_Destroy(g_hwndMenuBar);
            PostQuitMessage(0);
			break;

        case WM_ACTIVATE:
            // Notify shell of our activate message
            SHHandleWMActivate(hWnd, wParam, lParam, &s_sai, FALSE);
            break;

		case WM_SETTINGCHANGE:
            SHHandleWMSettingChange(hWnd, wParam, lParam, &s_sai);
            break;

        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
return 0;
}

// Function that creates a trackbar.
// Parameters:
//    HWND hWndParent - Parent window of the trackbar.
//    DWORD dwStyles - Window styles for the trackbar.
//    WORD wID - ID of the trackbar.
//    HINSTANCE hInst - Current instance.
//    RECT rclTrack - Size and position of the trackbar.
//    LONG lPageSize - Sets the page size; use 0 to ignore.
//    LONG lPosition - Sets the initial position.
//    LONG lRange - Sets the range.
//    LONG lFreq - Sets the frequency of ticks; use 0 to ignore.
//    LONG lSelection - Sets the selection; use 0 to ignore.
//    BOOL bRedraw - Redraw flag: TRUE to redraw; otherwise, FALSE.
//

HWND MyCreateTrackBar(HWND hWndParent, DWORD dwStyles, WORD wID, HINSTANCE hInst,\
					  int x, int y, int width, int height, LONG lPageSize, LONG lPosition, LONG lRange,\
					  LONG lFreq, LONG lSelection, BOOL bRedraw)
{
  HWND hWndSlider;

  // Create the trackbar.
  hWndSlider  = CreateWindowEx(
    0L,              // No extended styles.
    TRACKBAR_CLASS,	     // Trackbar class name.
    L"",              // No default text.
	WS_CHILD | WS_VISIBLE | dwStyles,   // Window styles.
	x, y, width, height,				// Size and position.
    hWndParent,      // Parent window handle.
    (HMENU)wID,      // ID for the trackbar.
    hInst,           // Current instance.
    NULL);           // No class data.

  if (hWndSlider == NULL)
    return NULL;

  // Set the page size.
  if (lPageSize)
      SendMessage(hWndSlider, TBM_SETPAGESIZE, 0L, lPageSize);

  // Set the tick frequency.
  if (lFreq)
      SendMessage(hWndSlider, TBM_SETTICFREQ, lFreq, 0L);

  // Set the initial range.
  SendMessage(hWndSlider, TBM_SETRANGE, FALSE, lRange);

  // Set the initial position.
  SendMessage(hWndSlider, TBM_SETPOS, FALSE, lPosition);

  // Set the selection.
  if (lSelection)
      SendMessage(hWndSlider, TBM_SETSEL, FALSE, lSelection);

  // Redraw if specified.
  if (bRedraw)
  {
	  InvalidateRect(hWndSlider, NULL, TRUE);
      UpdateWindow(hWndSlider);
  }

  return hWndSlider;
}

void Invoke(HWND hWnd, RECT rc, wstring Cam)
{
	CamConfig cfg;

	if(SUCCEEDED(g_Prop.InitFilterGrp()))
	{	
		cfg.sCamID = Cam;	// Pass that selected Camera Id to the "BuildFilterGraph(...)" function.
		cfg.rc = rc;
		cfg.hwndOwnerWnd = hWnd;
		cfg.bIsAudioEnb = TRUE;	
		cfg.bIsCapEnb = TRUE;
		cfg.bIsPrvEnb = TRUE;
		cfg.bIsStillEnb = TRUE;
		
		if(SUCCEEDED(g_Prop.BuildFilterGrp(&cfg)))			
		{
			OutputDebugString(L"\n------FGraph Build OK------\n");
			if(SUCCEEDED(g_Prop.RunGrp()))				
			{
				OutputDebugString(L"\n------FGraph Run OK------\n");
			}
			else
			{
				OutputDebugString(L"\n------FGraph Run FAIL------\n");
			}
		}
		else
		{
			OutputDebugString(L"\n------FGraph Build FAIL------\n");
		}
		
	}		
	else
	{
		OutputDebugString(L"\n------Init FGraph FAIL------\n");
	}

}

void BuildMenuEx(ImgFmt tIFmt0, ImgFmt* ptIFmts, INT nNoOfCap, BOOL bIsVideo)
{
	WCHAR wsMenuItemStr[BUF_LENGTH];
	WCHAR waTemp[BUF_LENGTH];
	INT nCounter=0;
	INT nIndex=0;
	
	g_hmnuCurr = (HMENU) SendMessage(g_hwndMenuBar, SHCMBM_GETSUBMENU, 0, IDM_SETTINGS);
	
	if(bIsVideo)
	{
		g_ahmnuVdoColorFmts = new HMENU[nNoOfCap];
		g_hmnuVideoSub = CreatePopupMenu();
		g_ahmnuVdoColorFmts[nCounter] = CreatePopupMenu();
		wsprintf(wsMenuItemStr, L"%d X %d", tIFmt0.nMaxWidth, tIFmt0.nMaxHeight);
		AppendMenu(g_hmnuVideoSub, MF_STRING, ID_RESOLUTION_0, wsMenuItemStr);
		wcscpy(waTemp, wsMenuItemStr);
		OutputDebugString(tIFmt0.wsColorFmt.c_str());
		OutputDebugString(L"\n");
		AppendMenu(g_ahmnuVdoColorFmts[nCounter], MF_STRING, ID_RESOLUTION_X_0, tIFmt0.wsColorFmt.c_str());

		for(int i=1;i<nNoOfCap;i++)
		{
			wsprintf(wsMenuItemStr, L"%d X %d", ptIFmts->nMaxWidth, ptIFmts->nMaxHeight);
			OutputDebugString(ptIFmts->wsColorFmt.c_str());
			OutputDebugString(L"\n");
			
			if(wcscmp(waTemp, wsMenuItemStr))
			{
				AppendMenu(g_hmnuVideoSub, MF_STRING, (ID_RESOLUTION_0+(i-nIndex)), wsMenuItemStr);
				wcscpy(waTemp, wsMenuItemStr);

				nCounter++;
				g_ahmnuVdoColorFmts[nCounter] = CreatePopupMenu();
				AppendMenu(g_ahmnuVdoColorFmts[nCounter], MF_STRING, ID_RESOLUTION_X_0, ptIFmts->wsColorFmt.c_str());
				nIndex=0;
			}
			else
			{
				nIndex++;
				AppendMenu(g_ahmnuVdoColorFmts[nCounter], MF_STRING, ID_RESOLUTION_X_0+nIndex, ptIFmts->wsColorFmt.c_str());

			}
			ptIFmts++;
		}
		AppendMenu(g_hmnuCurr, MF_STRING | MF_POPUP, (UINT)g_hmnuVideoSub, L"&Resolution");
		AppendMenu(g_hmnuCurr, MF_STRING | MF_POPUP, (UINT)g_ahmnuVdoColorFmts[0], L"&ColorFmts");
	}
	else
	{
		g_ahmnuStillColorFmts = new HMENU[nNoOfCap];
		g_hmnuStillSub = CreatePopupMenu();
		g_ahmnuStillColorFmts[nCounter] = CreatePopupMenu();
		wsprintf(wsMenuItemStr, L"%d X %d", tIFmt0.nMaxWidth, tIFmt0.nMaxHeight);
		AppendMenu(g_hmnuStillSub, MF_STRING, ID_RESOLUTION_0, wsMenuItemStr);
		wcscpy(waTemp, wsMenuItemStr);
		OutputDebugString(tIFmt0.wsColorFmt.c_str());
		OutputDebugString(L"\n");
		AppendMenu(g_ahmnuStillColorFmts[nCounter], MF_STRING, ID_RESOLUTION_X_0, tIFmt0.wsColorFmt.c_str());

		for(int i=1;i<nNoOfCap;i++)
		{
			wsprintf(wsMenuItemStr, L"%d X %d", ptIFmts->nMaxWidth, ptIFmts->nMaxHeight);
			OutputDebugString(ptIFmts->wsColorFmt.c_str());
			OutputDebugString(L"\n");
			
			if(wcscmp(waTemp, wsMenuItemStr))
			{
				AppendMenu(g_hmnuStillSub, MF_STRING, (ID_RESOLUTION_0+(i-nIndex)), wsMenuItemStr);
				wcscpy(waTemp, wsMenuItemStr);

				nCounter++;
				g_ahmnuStillColorFmts[nCounter] = CreatePopupMenu();
				AppendMenu(g_ahmnuStillColorFmts[nCounter], MF_STRING, ID_RESOLUTION_X_0, ptIFmts->wsColorFmt.c_str());
				nIndex=0;
			}
			else
			{
				nIndex++;
				AppendMenu(g_ahmnuStillColorFmts[nCounter], MF_STRING, ID_RESOLUTION_X_0+nIndex, ptIFmts->wsColorFmt.c_str());
			}
			ptIFmts++;
		}
	}
	DrawMenuBar(g_hwndMenuBar);
}

void UpdateUI(LONG lVal, LONG lMin, LONG lMax, LONG lStep)
{
	wstring sPos;
	TCHAR waTemp[BUF_LENGTH];
	TCHAR waText[BUF_LENGTH];

		
	//Redraw
	UpdateWindow(g_hwndMain);

	wsprintf((LPWSTR)sPos.c_str(),L"%d", lVal);
	g_hwndTemp = GetDlgItem(g_hwndMain,IDC_POS);
	SendMessage(g_hwndTemp, WM_SETTEXT, 0, (LPARAM)(LPCTSTR)sPos.c_str());	
	
	//Redraw
	InvalidateRect(g_hwndTemp, NULL, TRUE);
	UpdateWindow(g_hwndTemp);

#ifdef DEBUG_MODE
	LoadString(g_hinstCurr, STR_DEMARKATE, waText, MAX_LOADSTRING);
	OutputDebugString(waText);
	LoadString(g_hinstCurr, STR_MAX, waText, MAX_LOADSTRING);
	wsprintf(waTemp, waText, lMax);
	OutputDebugString(waTemp);
	LoadString(g_hinstCurr, STR_MIN, waText, MAX_LOADSTRING);
	wsprintf(waTemp, waText, lMin);
	OutputDebugString(waTemp);
	LoadString(g_hinstCurr, STR_VAL, waText, MAX_LOADSTRING);
	wsprintf(waTemp, waText, lVal);
	OutputDebugString(waTemp);
	LoadString(g_hinstCurr, STR_STEP, waText, MAX_LOADSTRING);
	wsprintf(waTemp, waText, lStep);
	OutputDebugString(waTemp);
	LoadString(g_hinstCurr, STR_DEMARKATE, waText, MAX_LOADSTRING);
	OutputDebugString(waText);
#endif
	
	g_hwndTemp=GetDlgItem(g_hwndMain,IDC_SLIDER);
	SendMessage(g_hwndTemp, TBM_CLEARSEL, TRUE, 0L);
	SendMessage(g_hwndTemp, TBM_SETPAGESIZE, 0L, lStep);
	SendMessage(g_hwndTemp, TBM_SETRANGE, TRUE, MAKELONG(lMin, lMax));
	SendMessage(g_hwndTemp, TBM_SETSELEND, TRUE, lMax);	
	SendMessage(g_hwndTemp, TBM_SETSELSTART, TRUE, lMin);		
	SendMessage(g_hwndTemp, TBM_SETPOS, TRUE, lVal);
		
	//Redraw
	InvalidateRect(g_hwndTemp, NULL, TRUE);
	UpdateWindow(g_hwndTemp);
}

BOOL Set_ImgRes(INT nIdx)
{
	TCHAR waMenuItemStr[BUF_LENGTH];
	TCHAR waWidth[BUF_LENGTH], waHeight[BUF_LENGTH];
	TCHAR* pwChar;
	ImageRes tIRes;

	g_ResIdx = nIdx;	// Store the current resolution point globally
	g_hmnuCurr = (HMENU) SendMessage(g_hwndMenuBar, SHCMBM_GETSUBMENU, 0, IDM_SETTINGS);
	RemoveMenu(g_hmnuCurr, 12, MF_BYPOSITION);

	if(g_bVideoMode)
	{
		AppendMenu(g_hmnuCurr, MF_STRING | MF_POPUP, (UINT)g_ahmnuVdoColorFmts[nIdx], L"&ColorFmts");	
		
		g_hmnuCurr = g_ahmnuVdoColorFmts[nIdx];
		CheckMenuRadioItem(g_hmnuCurr, 0, (g_nNoOfVdoCap-1), 0, MF_BYPOSITION);
		GetMenuString(g_hmnuCurr, 0, waMenuItemStr, BUF_LENGTH, MF_BYPOSITION);
		g_wsColorFmt = waMenuItemStr;

		g_hmnuCurr = g_hmnuVideoSub;
		CheckMenuRadioItem(g_hmnuCurr, 0, (g_nNoOfVdoCap-1), nIdx, MF_BYPOSITION);
	}
	else
	{
		AppendMenu(g_hmnuCurr, MF_STRING | MF_POPUP, (UINT)g_ahmnuStillColorFmts[nIdx], L"&ColorFmts");	

		g_hmnuCurr = g_ahmnuStillColorFmts[nIdx];
		CheckMenuRadioItem(g_hmnuCurr, 0, (g_nNoOfStillCap-1), 0, MF_BYPOSITION);
		GetMenuString(g_hmnuCurr, 0, waMenuItemStr, BUF_LENGTH, MF_BYPOSITION);
		g_wsColorFmt = waMenuItemStr;

		g_hmnuCurr = g_hmnuStillSub;
		CheckMenuRadioItem(g_hmnuCurr, 0, (g_nNoOfStillCap-1), nIdx, MF_BYPOSITION);
	}

	GetMenuString(g_hmnuCurr, nIdx, waMenuItemStr, BUF_LENGTH, MF_BYPOSITION);
	OutputDebugString(waMenuItemStr);
	pwChar = wcsstr(waMenuItemStr, L"X");
	wcscpy(waHeight, (pwChar+2));
	wmemcpy(waWidth, waMenuItemStr, ((pwChar-1)-waMenuItemStr));
	waWidth[(pwChar-1)-waMenuItemStr]='\0';

#ifdef DEBUG_MODE
	OutputDebugString(L"\n---------------------\n");
	OutputDebugString(waMenuItemStr);
	OutputDebugString(L"\n|");
	OutputDebugString(waWidth);
	OutputDebugString(L"|");
	OutputDebugString(L"\n|");
	OutputDebugString(waHeight);
	OutputDebugString(L"|\n---------------------");
#endif
	tIRes.nWidth = atoi(waWidth);
	tIRes.nHeight = _wtoi(waHeight);		//tIRes.nHeight = atoi(waHeight);

	if(!((tIRes.nWidth==g_tImgRes.nWidth)&&(tIRes.nHeight==g_tImgRes.nHeight)&&
		(g_bVMod==g_bVideoMode)))
	{
		//Record the selected resolution gloablly
		g_tImgRes.nWidth = tIRes.nWidth;
		g_tImgRes.nHeight = tIRes.nHeight;
		g_bVMod = g_bVideoMode;

		//wsprintf(test, L"\nWidth :- %d Height :- %d", tIRes.nWidth, tIRes.nHeight);
		//OutputDebugString(test);
		//g_Prop.Set_Resolution(&tIRes, P);
		/*
		ImageRes ptRes;
		ptRes.nHeight=240;
		ptRes.nWidth=320;
		g_Prop.Set_Resolution(&ptRes, P);
		*/
		if(g_bVideoMode)
			g_Prop.Set_Resolution(&tIRes, V);
		else
			g_Prop.Set_Resolution(&tIRes, S);

		OutputDebugString(L"\n-----------Resolution Change OK-----------");
	}
	else
	{
		OutputDebugString(L"\n-----------Same Resolution-----------");
	}
	
return TRUE;
}

BOOL Set_ImgColorFmt(INT nIdx)
{
	TCHAR waMenuItemStr[BUF_LENGTH];
	wstring wsColorFmt;

	if(g_bVideoMode)
	{
		g_hmnuCurr = g_ahmnuVdoColorFmts[g_ResIdx];
		CheckMenuRadioItem(g_hmnuCurr, 0, (g_nNoOfVdoCap-1), nIdx, MF_BYPOSITION);
	}
	else
	{
		g_hmnuCurr = g_ahmnuStillColorFmts[g_ResIdx];
		CheckMenuRadioItem(g_hmnuCurr, 0, (g_nNoOfStillCap-1), nIdx, MF_BYPOSITION);
	}

	GetMenuString(g_hmnuCurr, nIdx, waMenuItemStr, BUF_LENGTH, MF_BYPOSITION);
	OutputDebugString(waMenuItemStr);

	wsColorFmt = waMenuItemStr;
	
	if(!(g_wsColorFmt==wsColorFmt&&g_bVMod==g_bVideoMode))
	{
		//Record the selected resolution gloablly
		g_wsColorFmt = 	wsColorFmt;	
		g_bVMod = g_bVideoMode;
	
		//wsprintf(test, L"\nWidth :- %d Height :- %d", tIRes.nWidth, tIRes.nHeight);
		//OutputDebugString(test);
		g_Prop.Set_ColorFmt(&g_tImgRes, &wsColorFmt, P);
		if(g_bVideoMode)
			g_Prop.Set_ColorFmt(&g_tImgRes, &wsColorFmt, V);
		else
			g_Prop.Set_ColorFmt(&g_tImgRes, &wsColorFmt, S);

		OutputDebugString(L"\n-----------Color Format Change OK-----------");
	}
	else
	{
		OutputDebugString(L"\n-----------Same Color Format-----------");
	}
	
	return TRUE;
}

BOOL Set_Timers(HWND hWin, INT nDuration)
{
	KillTimer(hWin, MY_TIMER);
	KillTimer(hWin, MY_TIMER2);
	SetTimer(hWin, MY_TIMER, nDuration*1000, NULL);
	SetTimer(hWin, MY_TIMER2, 1000, NULL);

return TRUE;
}
BOOL Set_Duration(INT nDue, INT nPos)
{
	TCHAR waTemp[BUF_LENGTH];
	LRESULT lResult;
	
	g_hmnuCurr = (HMENU) SendMessage(g_hwndMenuBar, SHCMBM_GETSUBMENU, 0, IDM_SETTINGS);
	CheckMenuRadioItem(GetSubMenu(g_hmnuCurr,0), 0, 8, nPos, MF_BYPOSITION);
	if(nDue>0)
	{
		g_nDuration = g_nDueTime = nDue;
		wsprintf(waTemp,L"00:00:%.2d",nDue);
		g_hwndTemp = GetDlgItem(g_hwndMain,IDC_STRESS);
		EnableWindow(g_hwndTemp, TRUE);
		g_hwndTemp = GetDlgItem(g_hwndMain,IDC_TIMER);
		EnableWindow(g_hwndTemp, TRUE);
	}
	else
	{
		wsprintf(waTemp,L"00:00:%.2d",0);
		g_hwndTemp = GetDlgItem(g_hwndMain,IDC_STRESS);
		EnableWindow(g_hwndTemp, FALSE);
		g_hwndTemp = GetDlgItem(g_hwndMain,IDC_TIMER);
		EnableWindow(g_hwndTemp, FALSE);
	}
	g_hwndTemp = GetDlgItem(g_hwndMain, IDC_TIMER);
	SendMessage(g_hwndTemp, WM_SETTEXT, 0, (LPARAM)waTemp);

	g_hwndTemp = GetDlgItem(g_hwndMain,IDC_STRESS);
	lResult = SendMessage((HWND) g_hwndTemp, (UINT) BM_GETCHECK, 0, 0);  
	if(lResult == BST_CHECKED)
	{
		Set_Timers(g_hwndMain, g_nDuration); 
	}

return TRUE;
}

HRESULT Get_CamProp(INT nProp, INT nId)
{
	HRESULT hr;
	TCHAR wsText[BUF_LENGTH];

	//Check the Camera property menu and uncheck Image property menu
	g_hmnuCurr = (HMENU) SendMessage(g_hwndMenuBar, SHCMBM_GETSUBMENU, 0, IDM_SETTINGS);
	CheckMenuRadioItem(GetSubMenu(g_hmnuCurr, 8), 0, (NO_OF_CAMERA_PROP-2), nId, MF_BYPOSITION);
	CheckMenuRadioItem(GetSubMenu(g_hmnuCurr, 9), 0, (NO_OF_IMAGE_PROP-1), -1, MF_BYPOSITION);


	g_hwndTemp = GetDlgItem(g_hwndMain, IDC_PROP);
	LoadString(g_hinstCurr, nProp, wsText, MAX_LOADSTRING);
	SendMessage(g_hwndTemp, WM_SETTEXT, 0, (LPARAM)(LPCTSTR)wsText);
				
	g_tCamTbl.p = CProp[nId];
	g_ePType = CAM;
	hr = g_Prop.Get_PropRng(CAM, &g_tCamTbl);
	UpdateUI(g_tCamTbl.plVal, g_tCamTbl.plMin, g_tCamTbl.plMax, g_tCamTbl.plDelta);

return hr;
}
HRESULT Get_ImgProp(INT nProp, INT nId)
{
	HRESULT hr;
	TCHAR wsText[BUF_LENGTH];

	//Check the Image property menu and uncheck Cmaera property menu
	g_hmnuCurr = (HMENU) SendMessage(g_hwndMenuBar, SHCMBM_GETSUBMENU, 0, IDM_SETTINGS);
	CheckMenuRadioItem(GetSubMenu(g_hmnuCurr, 9), 0, (NO_OF_IMAGE_PROP-1), nId, MF_BYPOSITION);
	CheckMenuRadioItem(GetSubMenu(g_hmnuCurr, 8), 0, (NO_OF_CAMERA_PROP-1), -1, MF_BYPOSITION);
	
	g_hwndTemp = GetDlgItem(g_hwndMain, IDC_PROP);
	LoadString(g_hinstCurr, nProp, wsText, MAX_LOADSTRING);
	SendMessage(g_hwndTemp, WM_SETTEXT, 0, (LPARAM)(LPCTSTR)wsText);
				
	g_tImgTbl.vProp = IProp[nId];
	g_ePType = IMG;
	hr = g_Prop.Get_PropRng(IMG, &g_tImgTbl);
	UpdateUI(g_tImgTbl.plVal, g_tImgTbl.plMin, g_tImgTbl.plMax, g_tImgTbl.plDelta);

return hr;
}