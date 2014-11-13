#include <windows.h>
#include "common/RhodesApp.h"
#include "common/ExtManager.h"
#include "ViewFinder.h"


#define CMD_CANCEL	0x6001		///< HMENU id for the cancel button
#define CMD_CAPTURE	0x6002		///< HMENU id for the capture button


CViewFinder::CViewFinder()
{
    m_pCallBack = NULL;	
    m_hwndPreview = NULL;
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
    HWND hwndViewerParent = m_appMainWnd;
    if(eFullScreen == eMode)
    {
        //structure pos will be calculated in below call
        hwndViewerParent = createFullScreenWindow(m_appMainWnd, pos);
    }
    m_hwndPreview = createPreviewWindow(hwndViewerParent, pos);
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
    HWND hwndParent = GetParent(m_hwndPreview);
    if(hwndParent != m_appMainWnd)
    {
        DestroyWindow(m_hwndPreview);
        m_hwndPreview = NULL;
        DestroyWindow(hwndParent);
    }
    else
    {
        DestroyWindow(m_hwndPreview);
        m_hwndPreview = NULL;
    }
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
    default:
        {
            return DefWindowProc(hWnd, message, wParam, lParam);
        }		
    }
    return TRUE;
}
int m_iH = GetSystemMetrics(SM_CYSCREEN);
int iXCoord = GetSystemMetrics(SM_CXSCREEN);
float iRatio;
int m_iX;
int m_iY;
bool m_bIsLandscape;
int CViewFinder::scaledpx( int size)
{
    int retSize = (int)(m_iH/((double)m_iY/(double)size));
    return retSize;
}

int CViewFinder::scaleForY( int size)
{
    if (m_bIsLandscape)
        return (int)((float)size / 1.4);
    else
        return size;
}
int CViewFinder::scaleForMargain(int size)
{
    if (m_bIsLandscape)
        return size + 30;
    else
        return size;
}
HWND CViewFinder::createFullScreenWindow(HWND hwndParent, RECT& pos)
{	
    int iXCoord = GetSystemMetrics(SM_CXSCREEN);
    float iRatio;
    if (iXCoord > m_iH)
        iRatio = (float)iXCoord / (float)m_iH;
    else
        iRatio = (float)m_iH / (float)iXCoord;

    //  Only currently considered ratios are 1:1.333333 and 1:1.66666
    int iSmallSide = 240;
    int iLongSide = 320;
    if (iRatio < 1.7f && iRatio >1.6f)
    {
        //  Screen is 1.666666 ratio
        iSmallSide = 240;
        iLongSide = 400;
    }

    if (iXCoord > m_iH)
    {
        //  The device is in landscape mode
        m_iX = iLongSide;
        m_iY = iSmallSide;
        m_bIsLandscape = true;
    }
    else
    {
        m_iX = iSmallSide;
        m_iY = iLongSide;
    }

    int iLeftPos = (GetSystemMetrics(SM_CXSCREEN)/2) - scaledpx(m_iX/2);
    int iTopPos = (GetSystemMetrics(SM_CYSCREEN)/2) - scaledpx(m_iY/2);

    HWND hwndFullScreen = CreateWindowEx(WS_EX_NOANIMATION|WS_EX_TOPMOST, L"Dialog", L"", WS_POPUP | WS_VISIBLE ,
        iLeftPos, iTopPos, scaledpx(m_iX), scaledpx(m_iY), hwndParent, NULL, m_appInstance, NULL);

    //ShowWindow(hwndParent, SW_HIDE);
    if (hwndFullScreen)
    {		
        //set viewer window position in case of full screen
        pos.left = 0;
        pos.top = 0;
        pos.right = GetSystemMetrics(SM_CXSCREEN);
        pos.bottom =  scaledpx(scaleForY(228));

        //create child controls
        HWND m_hwndCancel = CreateWindowEx(WS_EX_NOANIMATION,_T("Button"), L"Cancel", 
            WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE , 
            scaledpx(scaleForMargain(180)), scaledpx(scaleForY(240)), scaledpx(55), scaledpx(scaleForY(22)), hwndFullScreen, (HMENU)CMD_CANCEL, m_appInstance, NULL);

        HWND m_hwndCapture = CreateWindowEx(WS_EX_NOANIMATION,_T("Button"), L"Capture", 
            WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE , 
            scaledpx(scaleForMargain(120)), scaledpx(scaleForY(240)), scaledpx(55), scaledpx(scaleForY(22)), hwndFullScreen, (HMENU)CMD_CAPTURE, m_appInstance, NULL);

        SetWindowLong(hwndFullScreen, GWL_WNDPROC, (LONG) FullScreenWndProc);
		SetWindowLong(hwndFullScreen,GWL_USERDATA,(long) m_pCallBack);
   

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

