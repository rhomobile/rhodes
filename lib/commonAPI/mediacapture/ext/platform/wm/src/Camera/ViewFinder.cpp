#include <windows.h>
#include "common/RhodesApp.h"
#include "common/ExtManager.h"
#include "ViewFinder.h"


#define CMD_CANCEL	0x6001		///< HMENU id for the cancel button
#define CMD_CAPTURE	0x6002		///< HMENU id for the capture button

HWND CViewFinder::m_hwndPreview = NULL;
HWND CViewFinder::m_PreviewWndParent = NULL;
HWND CViewFinder::m_CancelButton = NULL;
HWND CViewFinder::m_CaptureButton = NULL;

CViewFinder::CViewFinder()
{
    m_pCallBack = NULL;	
    m_hwndPreview = NULL;
	m_PreviewWndParent = NULL;
	m_CancelButton = NULL;
	m_CaptureButton = NULL;
    // Get the parent window handle, i.e. the webkit window, and the application instance
    rho::common::CRhoExtData rhodes_data = RHODESAPP().getExtManager().makeExtData();
    m_appInstance = (HINSTANCE) (rhodes_data.m_hInstance);
    m_appMainWnd =rhodes_data.m_hBrowserWnd;

}
void CViewFinder::RegisterCallBack(IViewFinderCallBack* pCallBack)
{
    m_pCallBack = pCallBack;
}
HWND CViewFinder::CreateViewerWindow(RECT& pos, eViewrWndMode eMode)
{	
    if(m_hwndPreview != NULL)
    {		
        DestroyViewerWindow();
    }
    m_PreviewWndParent = m_appMainWnd;
    if(eFullScreen == eMode)
    {
        //structure pos will be calculated in below call
        m_PreviewWndParent = createFullScreenWindow(m_appMainWnd, pos);
    }
    m_hwndPreview = createPreviewWindow(m_PreviewWndParent, pos);
    return m_hwndPreview;
}
void CViewFinder::RepositionWindow(const RECT& pos)
{
    if (IsWindow(m_hwndPreview))
    {
        MoveWindow(m_hwndPreview,
            pos.left,
            pos.top,
            pos.right,
            pos.bottom, 
            FALSE);
    }
}
void CViewFinder::DestroyViewerWindow()
{   
    if(m_PreviewWndParent != m_appMainWnd)
    {
        DestroyWindow(m_hwndPreview);
        DestroyWindow(m_PreviewWndParent);
    }
    else
    {
        DestroyWindow(m_hwndPreview);
    }
	m_PreviewWndParent = NULL;
	m_hwndPreview = NULL;
	m_CancelButton = NULL;
	m_CaptureButton = NULL;
}
LRESULT CALLBACK CViewFinder::FullScreenWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    IViewFinderCallBack* pObj = (IViewFinderCallBack *)GetWindowLong(hWnd, GWL_USERDATA);
    switch(message)
    {
	case WM_COMMAND:
		{
			switch(LOWORD(wParam))
			{

			case CMD_CAPTURE:
				{
					pObj->captureImage();
					break;
				}
			case CMD_CANCEL:
				{
					pObj->cancel();				
					break;
				}
			}

			break;
		}
	case WM_KEYDOWN:
		{
			if(wParam == VK_RETURN)
			{
				pObj->captureImage();
			}
			break;
		}
	case WM_SETTINGCHANGE:
		{
			// Determine whether wParam's contains 
			// the value SETTINGCHANGE_RESET.
			if (wParam == SETTINGCHANGE_RESET)
			{
				RECT pos;
				ReposFullScreen(pos);		
			    pObj->ResetViewerWndPos(pos);
			}
			break;
		}
    default:
        {
            return DefWindowProc(hWnd, message, wParam, lParam);
        }		
    }
    return TRUE;
}
HWND CViewFinder::createFullScreenWindow(HWND hwndParent, RECT& pos)
{	

	int screenwidth=GetSystemMetrics(SM_CXSCREEN);
	int screenheight=GetSystemMetrics(SM_CYSCREEN);

	HWND hwndFullScreen = CreateWindowEx(WS_EX_NOANIMATION|WS_EX_TOPMOST, L"Dialog", L"", WS_POPUP | WS_VISIBLE ,
		0, 0, screenwidth, screenheight, hwndParent, NULL, m_appInstance, NULL);
	if(hwndFullScreen)
	{
		//get the viewerwnd position
		GetClientRect (hwndFullScreen, &pos);
		//calculate the button size
		HWND hwnd = CreateWindow (L"BUTTON", L"", WS_CHILD, 0, 0, 0, 0, hwndFullScreen, (HMENU) 0, m_appInstance, NULL);
		HDC hdc = GetDC (hwnd);
		SIZE size;
		GetTextExtentPoint32 (hdc, L"Capture", 7, &size);
		ReleaseDC (hwnd, hdc);
		DestroyWindow (hwnd);
		DEBUGMSG (TRUE, (L"Button size %d x %d\r\n", size.cx, size.cy));

		int buttonWidth = (size.cx + 18);
		int buttonHeight = size.cy+6;

		int buttonTop = pos.bottom - buttonHeight - 16;

		int buttonLeft = (int)(screenwidth*0.40);

		//draw buttons

		m_CaptureButton=CreateWindow (L"BUTTON", L"Capture", WS_CHILD | WS_VISIBLE,
			buttonLeft, buttonTop, buttonWidth, buttonHeight, hwndFullScreen, (HMENU) CMD_CAPTURE, m_appInstance, NULL);

		buttonLeft = buttonLeft + buttonWidth +5; //move cancel button bit right of capture

		m_CancelButton=CreateWindow (L"BUTTON", L"Cancel", WS_CHILD | WS_VISIBLE,
			buttonLeft, buttonTop, buttonWidth, buttonHeight, hwndFullScreen, (HMENU)CMD_CANCEL, m_appInstance, NULL);

		SetWindowLong(hwndFullScreen,GWL_USERDATA,(long) m_pCallBack);
		SetWindowLong(hwndFullScreen, GWL_WNDPROC, (LONG) FullScreenWndProc);

	}
	return hwndFullScreen;

}
HWND CViewFinder::createPreviewWindow(HWND hwndParent, const RECT& pos)
{
    HWND hwndPreviewWnd;
    if(hwndParent)//this hwnd could be main wnd/ a full screen window which is a child of mainWnd
    {
        // Create the viewer window
        hwndPreviewWnd = CreateWindowEx(WS_EX_NOANIMATION|WS_EX_TOPMOST, L"static", NULL, WS_VISIBLE | WS_CHILD, 
            pos.left,
            pos.top,
            pos.right,
            pos.bottom, 
            hwndParent, NULL, m_appInstance, NULL);
    }
    return hwndPreviewWnd;
}
HWND CViewFinder::getViewerWndHandle()
{
	return m_hwndPreview;
}
void CViewFinder::ReposFullScreen(RECT& pos)
{
	int screenwidth=GetSystemMetrics(SM_CXSCREEN);
	int screenheight=GetSystemMetrics(SM_CYSCREEN);

	//reset full screen wnd pos
	//MoveWindow(m_PreviewWndParent, 0,0, screenwidth, screenheight, FALSE);
	SetWindowPos(m_PreviewWndParent, HWND_TOPMOST, 0, 0 , screenwidth, screenheight,SWP_SHOWWINDOW);
	
	//get the viewerwnd position	
	GetClientRect (m_PreviewWndParent, &pos);	

	RECT buttonPos;
	GetClientRect(m_CancelButton, &buttonPos);
	int buttonTop = pos.bottom - buttonPos.bottom - 16;
	int buttonLeft = (int)(screenwidth*0.40);
	
	//draw buttons	
	SetWindowPos(m_CaptureButton, m_PreviewWndParent, buttonLeft, buttonTop , buttonPos.right, buttonPos.bottom,SWP_SHOWWINDOW);
    buttonLeft = buttonLeft + buttonPos.right +5; //move cancel button bit right of capture
	SetWindowPos(m_CancelButton, m_PreviewWndParent, buttonLeft, buttonTop , buttonPos.right, buttonPos.bottom,SWP_SHOWWINDOW);	
	
}
//below method is called from camera , when user presses trigger button to capture image in fullscreen
void CViewFinder::DisableFullScreenButtons()
{
	ShowWindow(m_CaptureButton, SW_HIDE);
	ShowWindow(m_CancelButton, SW_HIDE);
}

