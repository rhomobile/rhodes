

#include "common/spirit_comtypes.h"

#if defined SPIRIT_OS_WIN32_WCE && _WIN32_WCE>=0x501
#include <windows.h>
#include <tchar.h>

#include <commctrl.h>
#include <aygshell.h>

#include <stdlib.h>
#include <stdarg.h>

#include "system/spirit_system.h"
#include "wce_ui.h"

#pragma comment(lib,"aygshell.lib")


#define MAX_LOADSTRING 100
#define ID_DISCONNECT 1001

#ifndef TBIF_BYINDEX
  #define TBIF_BYINDEX   0x80000000ul
#endif

// Global Variables:
HINSTANCE			g_hInst;			// current instance
HWND				g_hWndMenuBar;		// menu bar handle

// Forward declarations of functions included in this code module:
ATOM			MyRegisterClass(HINSTANCE, LPTSTR);
BOOL			InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
ATOM MyRegisterClass(HINSTANCE hInstance, LPTSTR szWindowClass)
{
	WNDCLASS wc;

	wc.style         = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc   = WndProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = hInstance;
	wc.hIcon         = NULL;
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
HWND InitInstance(HINSTANCE hInstance, int nCmdShow, tWCE_UI *pUI)
{
    HWND hWnd;
    //TCHAR szTitle[MAX_LOADSTRING] = _T("mm_step_01");		// title bar text
    TCHAR szWindowClass[MAX_LOADSTRING] = _T("wince_ui_class");	// main window class name

    g_hInst = hInstance; // Store instance handle in our global variable


    //If it is already running, then focus on the window, and exit
    hWnd = FindWindow(szWindowClass, pUI->GetWindowName());	
    if (hWnd) 
    {
        // set focus to foremost child window
        // The "| 0x00000001" is used to bring any owned windows to the foreground and
        // activate them.
        SetForegroundWindow((HWND)((ULONG) hWnd | 0x00000001));
        return 0;
    } 

    if (!MyRegisterClass(hInstance, szWindowClass))
    {
    	return 0;
    }

    hWnd = CreateWindow(szWindowClass, pUI->GetWindowName(), WS_VISIBLE|WS_MAXIMIZEBOX,
      CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, pUI);

    if (!hWnd)
    {
        return 0;
    }


    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);


    return hWnd;
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
#define IDR_MENU 100
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    int wmId, wmEvent;
    PAINTSTRUCT ps;
    HDC hdc;

	
    switch (message) 
    {
        case WM_COMMAND:
            wmId    = LOWORD(wParam); 
            wmEvent = HIWORD(wParam); 
            // Parse the menu selections:
/*            switch (wmId)
            {
                case IDM_OK:
                    DestroyWindow(hWnd);
                    break;
                default:
                    return DefWindowProc(hWnd, message, wParam, lParam);
            }*/
            return DefWindowProc(hWnd, message, wParam, lParam);
            break;
        case WM_SYSCOMMAND:

          return DefWindowProc(hWnd, message, wParam, lParam);
          
        case WM_KEYUP: {
          tWCE_UI *pUI = (tWCE_UI *)::GetWindowLong(hWnd,GWL_USERDATA);
          //pUI->OnKeyUp();
          return DefWindowProc(hWnd, message, wParam, lParam);
                       }

        case WM_NOTIFY:
          {
            tWCE_UI *pUI = (tWCE_UI *)::GetWindowLong(hWnd,GWL_USERDATA);
            //pUI->OnKeyUp();
            return DefWindowProc(hWnd, message, wParam, lParam);

          }

        case WM_CREATE: {
            CREATESTRUCT *lpcs = (CREATESTRUCT *) lParam;


            ::SetWindowLong(hWnd,GWL_USERDATA,(LONG) lpcs->lpCreateParams);

            SHMENUBARINFO mbi;
            memset(&mbi, 0, sizeof(SHMENUBARINFO));
            mbi.cbSize     = sizeof(SHMENUBARINFO);
            mbi.hwndParent = hWnd;
            mbi.nToolBarId = IDR_MENU;
            mbi.hInstRes   = g_hInst;
            mbi.dwFlags    = SHCMBF_EMPTYBAR;
            int client_height =0;

            if (!SHCreateMenuBar(&mbi)) 
            {
                g_hWndMenuBar = NULL;
            }
            else
            {

                g_hWndMenuBar = mbi.hwndMB;
                TBBUTTONINFO tbbi = {0};
                tbbi.cbSize       = sizeof(tbbi);
                tbbi.dwMask       = TBIF_LPARAM | TBIF_BYINDEX;

                SendMessage(g_hWndMenuBar, TB_GETBUTTONINFO,0, (LPARAM)&tbbi);
                HMENU g_hMenu           = (HMENU)tbbi.lParam;

                // When the main window is created using CW_USEDEFAULT the height of the menubar (if one
                // is created is not taken into account). So we resize the window after creating it
                // if a menubar is present
                if (g_hWndMenuBar)
                {
                    RECT rc, rcMenuBar;
                    GetWindowRect(hWnd, &rc);
                    GetWindowRect(g_hWndMenuBar, &rcMenuBar);
                    if (rc.bottom < rcMenuBar.bottom) {
                      //smartphone branch
                      client_height = (rc.bottom - rc.top); 
                    }
                    else  {
                      //pocket pc branch
                      client_height = (rc.bottom - rc.top) - (rcMenuBar.bottom - rcMenuBar.top);
                    }
                    
                }

            }

            RECT rect;
            GetClientRect(hWnd,&rect);
            if (client_height) rect.bottom = client_height;

            HWND hListBoxWnd = ::CreateWindow(_T("LISTBOX"),_T("Log"),WS_CHILD  |WS_TABSTOP |LBS_USETABSTOPS |WS_HSCROLL | WS_VSCROLL|LBS_NOINTEGRALHEIGHT,0,0,rect.right-rect.left,rect.bottom-rect.top,hWnd,0,0,lpcs->lpCreateParams);
            SendMessage(hListBoxWnd ,LB_SETHORIZONTALEXTENT,850,0);

            tWCE_UI *pUI = (tWCE_UI *) lpcs->lpCreateParams;
            pUI->SetListHWND(hListBoxWnd);
            SetFocus(hListBoxWnd);
            //correct the List Box window size
            ::SetWindowPos(hListBoxWnd,0,0,0,rect.right-rect.left,rect.bottom-rect.top,SWP_NOOWNERZORDER|SWP_NOZORDER|SWP_SHOWWINDOW);
                        }

            break;
        case WM_PAINT:
            hdc = BeginPaint(hWnd, &ps);
            
            // TODO: Add any drawing code here...
            
            EndPaint(hWnd, &ps);
            break;
        case WM_ACTIVATE: {
          tWCE_UI *pUI = (tWCE_UI *)::GetWindowLong(hWnd,GWL_USERDATA);
          if (WA_ACTIVE == LOWORD(wParam)) {
            SetFocus(pUI->GetListHWND());
          }

                          }
          break;
        case WM_DESTROY:
            //CommandBar_Destroy(g_hWndMenuBar);
            PostQuitMessage(0);
            break;


        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}



DWORD WINAPI wnd_message_proc(LPVOID pvarg)
{
  tWCE_UI *pUI = (tWCE_UI *) pvarg;
  MSG msg;
	// Perform application initialization:
  if (!pUI->SetHWND(InitInstance(0, SW_SHOW| SW_MAXIMIZE,pUI))) {
    return 0;
  };

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
    if ((msg.message == WM_KEYDOWN ||msg.message == WM_KEYUP) && msg.wParam == VK_RETURN) {
      pUI->OnKeyUp();
    }
		TranslateMessage(&msg);
		DispatchMessage(&msg);

	}



	return (int) msg.wParam;

}

tWCE_UI::~tWCE_UI()
{

  Stop();
  Event_Destroy(m_hUIEvent);
  CriticalSection_Destroy(m_textCS);
}

tWCE_UI::tWCE_UI()
{
  m_hUIThread = INVALID_HANDLE_VALUE;
  m_hUIEvent = Event_Create(0,0);
  m_textCS = CriticalSection_Create();
  m_hWnd = m_hListWnd = 0;
  m_bWaitingUserInput = false;
}


bool tWCE_UI::Create(const char *name)
{

  char sbuf[16] = {0};
  //get the mm_step_XX part from the filename
  if (strlen(name) < sizeof("mm_step_XX.cpp")) strcpy(sbuf,"mm_step_XX");
  strncpy(sbuf,name + strlen(name) - sizeof("mm_step_XX.cpp")+1,sizeof("mm_step_XX")-1);
  swprintf(m_strName,_T("%S"),sbuf);

  
  Event_Reset(m_hUIEvent);
  m_hUIThread = ::CreateThread(NULL,0,wnd_message_proc,this,0,&m_dwThreadId);
  if (m_hUIThread == INVALID_HANDLE_VALUE) {
    return false;
  }
  Event_Wait(m_hUIEvent,SPIRIT_INFINITE);
  if (!m_hWnd) return false;

  return true;
}
void tWCE_UI::Stop()
{
  if (m_hUIThread != INVALID_HANDLE_VALUE) {
    if (m_hListWnd) {

      FILE *ofp = fopen("mm_step_ui.log.txt","w"); 
      if (ofp) {
        int nMessages = ::SendMessage(m_hListWnd,LB_GETCOUNT,0,0);
        TCHAR sbuf[512];
        for (int i=0; i < nMessages; i++) {
          SendMessage(m_hListWnd,LB_GETTEXT,i,(LPARAM)sbuf);
          fprintf(ofp,"%S\n",sbuf);
        }
        fclose(ofp);
      }

    }
    if (m_hWnd != 0)   ::DestroyWindow(m_hWnd);
    else ::PostThreadMessage(m_dwThreadId,WM_QUIT,0,0);

    ::WaitForSingleObject(m_hUIThread,INFINITE);
    ::CloseHandle(m_hUIThread);
    m_hUIThread = INVALID_HANDLE_VALUE;

  }
}

bool tWCE_UI::SetHWND(HWND hWnd)
{
  m_hWnd = hWnd;
  Event_Set(m_hUIEvent);

  if (!m_hWnd) return false;
  return true;
}
void tWCE_UI::OnKeyUp()
{
  printf("%s\n",__FUNCTION__);
  if (!m_bWaitingUserInput) return ;
  Event_Set(m_hUIEvent);

}
void tWCE_UI::WaitForUserInput()
{
  
  Event_Reset(m_hUIEvent);
  m_bWaitingUserInput = true;
  Event_Wait(m_hUIEvent,SPIRIT_INFINITE);
  m_bWaitingUserInput = false;
}

static TCHAR *get_next_line(TCHAR *pStr,TCHAR **ppnext)
{
  TCHAR *p= pStr;
  while (*p && (*p!= '\n')) {
    p++;
  }
  if (*p == '\n') {
    *p = 0;
    if (*(p+1))  *ppnext = p+1;
    else *ppnext = 0;
  }
  else if (!*p) {
    *ppnext= 0;
  }
  return pStr;
}
void tWCE_UI::cPrintf(const char* format, ...) 
{
  va_list argptr;
  va_start(argptr, format);
  char sbuf[512];

  vsprintf(sbuf,format,argptr);
  va_end(argptr);
  Printf(_T("%S"),sbuf);
}
void tWCE_UI::Printf(const TCHAR* format, ...) 
{
  va_list argptr;
  va_start(argptr, format);
  TCHAR sbuf[512];

  _vstprintf(sbuf,format,argptr);
  va_end(argptr);

  CriticalSection_Lock(m_textCS);

  TCHAR *pnext, *p;
  pnext = p = sbuf;
  do {

    p = get_next_line(pnext,&pnext);
    int idx = ::SendMessage(m_hListWnd,LB_ADDSTRING,0, (LPARAM) p);
    if (idx >= 0) {
      /*RECT rect;
      static int max_width = 0;
      ::SendMessage(m_hListWnd,LB_GETITEMRECT,idx,(LPARAM)&rect);
      if (rect.right-rect.left > max_width) {
        max_width = rect.right-rect.left ;
        
      }*/
      ::SendMessage(m_hListWnd,LB_SETCURSEL,idx,0);
    }


  } while (pnext);

  CriticalSection_Unlock(m_textCS);
   
}

#endif
