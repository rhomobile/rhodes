/*------------------------------------------------------------------------
* (The MIT License)
* 
* Copyright (c) 2008-2011 Rhomobile, Inc.
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
* 
* http://rhomobile.com
*------------------------------------------------------------------------*/

#include "stdafx.h"

#if defined(_WIN32_WCE)

#if !defined( OS_PLATFORM_MOTCE )
#include <webvw.h>
#endif

//#include <soundfile.h>
//#include <nled.h>
#endif

#include <string>
#include <hash_map>

#include "resource.h"
#include "MainWindow.h"
#include "common/StringConverter.h"
#include "AppManager.h"
#include "ext/rho/rhoruby.h"
#include "rubyext/WebView.h"
#include "camera/Camera.h"
#include "sync/RhoconnectClientManager.h"
#include "common/RhoFilePath.h"
#include "common/RhoFile.h"
#include "bluetooth/Bluetooth.h"
#include "statistic/RhoProfiler.h"
#include "coreapi/ext/shared/Intent.h"
#include "coreapi/ext/platform/wm/src/Intents.h"
#include "browser/BrowserFactory.h"

#ifndef APP_BUILD_CAPABILITY_WEBKIT_BROWSER
#include "MetaHandler.h"
#endif

IMPLEMENT_LOGCLASS(CMainWindow,"MainWindow");
UINT WM_LICENSE_SCREEN		= ::RegisterWindowMessage(L"RHODES_WM_LICENSE_SCREEN");
UINT WM_INTENTMSG			= ::RegisterWindowMessage(L"RHODES_WM_INTENTMSG");
UINT WM_CREATE_SHORTCUT		= ::RegisterWindowMessage(L"RHODES_WM_CREATE_SHORTCUT");	

#include "DateTimePicker.h"

extern "C" void rho_sysimpl_sethas_network(int nValue);
extern "C" void rho_sysimpl_sethas_cellnetwork(int nValue);
extern "C" void rho_geoimpl_turngpsoff();
extern "C" LRESULT rho_wmimpl_draw_splash_screen(HWND hWnd);

extern "C" double rho_wmimpl_get_pagezoom();

//Reads the "SplashScreenPath" & "SplashScreenDuration" value from config.xml
extern "C" const wchar_t* rho_wmimpl_getSplashScreenPathVal();
extern "C" const wchar_t* rho_wmimpl_getSplashScreenDuration();

bool Rhodes_WM_ProcessBeforeNavigate(LPCTSTR url);
bool m_SuspendedThroughPowerButton = false;

using namespace rho::common;
using namespace rho;
using namespace stdext;

#if !defined(_WIN32_WCE)
int CMainWindow::m_screenWidth;
int CMainWindow::m_screenHeight;
#else
extern "C" bool rho_wmimpl_get_resize_on_sip();
#endif

extern "C" int rho_wm_impl_CheckLicense();

CMainWindow::CMainWindow()
{
    m_bLicenseScreenShownFirsttime=true;
    mIsBrowserViewHided = false;
    mNativeView = NULL;
    mNativeViewFactory = NULL;
    mNativeViewType = "";
    g_hWndCommandBar = 0;
    m_pBrowserEng = NULL;

	
	
#if defined(OS_WINCE)
	
    m_bFullScreen = false;
    m_bFullScreenBeforeLicense = m_bFullScreen;
#endif

    mIsOpenedByURL = false;

    m_bLoading = true;
    //m_spIWebBrowser2 = NULL;
#if defined(_WIN32_WCE)
    memset(&m_sai, 0, sizeof(m_sai));
    m_sai.cbSize = sizeof(m_sai);
#endif
    m_pageCounter = 0;
    m_menuBarHeight = 0;

    m_alertDialog = 0;
	m_isMinimized = 0;
}

CMainWindow::~CMainWindow()
{
//    if ( m_current_url )
//	    free(m_current_url);

//    if ( m_szStartPage )
//        free(m_szStartPage);
}

void CMainWindow::Navigate2(BSTR URL, int index) 
{
	String cleared_url = processForNativeView(convertToStringA(OLE2CT(URL)));
	if ( m_pBrowserEng && !cleared_url.empty()) 
    {
		StringW cw = convertToStringW(cleared_url);
        m_pBrowserEng->Navigate(cw.c_str(), m_oTabBar.GetTabID(index)); 
	}
}

void CMainWindow::Navigate(BSTR URL, int index) 
{
	String cleared_url = processForNativeView(convertToStringA(OLE2CT(URL)));
	if (m_pBrowserEng && !cleared_url.empty()) 
    {
		StringW cw = convertToStringW(cleared_url);
		//BSTR cleared_url_bstr = SysAllocString(cw.c_str());
	    //m_spIWebBrowser2->Navigate(cleared_url_bstr, NULL, &CComVariant(L"_self"), NULL, NULL);

        m_pBrowserEng->Navigate(cw.c_str(), m_oTabBar.GetTabID(index) );
	}
}

// **************************************************************************
//
// WM_xxx handlers
//
// **************************************************************************
#if defined(OS_WINCE)

void hideSIPButton()
{
    HWND hg_sipbut = FindWindow(L"MS_SIPBUTTON", NULL);
    if (hg_sipbut)
        ::ShowWindow(hg_sipbut, SW_HIDE);
}

void CMainWindow::showTaskBar(bool bShow)
{
    HWND hTaskBar = FindWindow(_T("HHTaskBar"), NULL);
    if ( hTaskBar )
    {
        ::EnableWindow(hTaskBar, !bShow ? FALSE : TRUE ); 
        ::ShowWindow(hTaskBar, !bShow ? SW_HIDE : SW_SHOW );
    }
}

void CMainWindow::RhoSetFullScreen(bool bFull, bool bDestroy /*=false*/)
{
    LOG(INFO) + "RhoSetFullScreen: " + (bFull ? 1 : 0);
    m_bFullScreen = bFull;

    if( !IsWindowVisible() )
    {
        LOG(INFO) + "Main Window is invisible.";// Skip full screen method";
        //return;
    }

    showTaskBar(!bFull);

	if(RHO_IS_CEDEVICE)
	{
	    //if(g_hWndCommandBar)
		    //::ShowWindow(g_hWndCommandBar, !bFull ? SW_SHOW : SW_HIDE);
        //    CommandBar_Show(g_hWndCommandBar, !bFull ? TRUE : FALSE);

        if ( m_bFullScreen )
        {
            if ( g_hWndCommandBar )
            {
                ::DestroyWindow(g_hWndCommandBar);
                g_hWndCommandBar = NULL;
            }
        }else
        {
            if ( !g_hWndCommandBar )
            {
                g_hWndCommandBar = CommandBar_Create(_AtlBaseModule.GetResourceInstance(), m_hWnd, 1);

                TBBUTTON oBtn = {0};
                oBtn.iBitmap = -1;
                oBtn.idCommand = IDM_POPUP_MENU;
                oBtn.fsState = TBSTATE_ENABLED;
                oBtn.iString = (int)L"File";

                CommandBar_InsertButton(g_hWndCommandBar, 0, &oBtn);

                CommandBar_AddAdornments(g_hWndCommandBar, 0, 0);
                CommandBar_Show(g_hWndCommandBar, TRUE);
            }
        }
	}

    if (!bDestroy)
    {
#if !defined(_DEBUG)
        //With MC3000c50b full screen functionality is not working in debug mode
		if(RHO_IS_WMDEVICE)
			SetFullScreen(bFull);
#endif

		if ( bFull )
			hideSIPButton();

		if(RHO_IS_CEDEVICE)
		{
			CRect rcMainWindow;
			calculateMainWindowRect(rcMainWindow);
			MoveWindow(&rcMainWindow);
		}
    }

}
#endif //OS_WINCE

LRESULT CMainWindow::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    m_pBrowserEng = 0;
    int xScreenSize = GetSystemMetrics(SM_CXSCREEN);
    int yScreenSize = GetSystemMetrics(SM_CYSCREEN);

    LOG(INFO)  + "Screen size: x=" + xScreenSize + ";y=" + yScreenSize;

	RECT rcMainWindow = { 0,0,320,470 };

#if defined(OS_WINCE)
	if(RHO_IS_WMDEVICE)
	{
        if(m_mainMenu.CreateMenu())
        {
            m_mainMenu.AppendMenu(MF_BYCOMMAND | MF_ENABLED | MF_STRING,
                                IDM_SK1_EXIT, _T("Back"));
            m_mainMenu.AppendMenu(MF_BYCOMMAND | MF_ENABLED | MF_STRING,
                                IDM_SK2_MENU, _T("Menu"));

            SHMENUBARINFO mbi = { 0 };
            mbi.cbSize        = sizeof(mbi);
            mbi.hwndParent    = m_hWnd;
            mbi.dwFlags       = SHCMBF_HMENU;
            mbi.nToolBarId    = (UINT)(HMENU)m_mainMenu;
            mbi.hInstRes      = ModuleHelper::GetResourceInstance();
            mbi.nBmpId        = 0;
            mbi.cBmpImages    = 0;
            mbi.hwndMB        = NULL;

            BOOL bRet = ::SHCreateMenuBar(&mbi);
            if(bRet != FALSE)
            {
                m_hWndCECommandBar = mbi.hwndMB;
                m_menuBar = m_hWndCECommandBar;
                SizeToMenuBar();
            }
        }
	}else if(RHO_IS_CEDEVICE)
	{
/*      g_hWndCommandBar = CommandBar_Create(_AtlBaseModule.GetResourceInstance(), m_hWnd, 1);

        TBBUTTON oBtn = {0};
        oBtn.iBitmap = -1;
        oBtn.idCommand = IDM_POPUP_MENU;
        oBtn.fsState = TBSTATE_ENABLED;
        oBtn.iString = (int)L"File";

        CommandBar_InsertButton(g_hWndCommandBar, 0, &oBtn);

        CommandBar_AddAdornments(g_hWndCommandBar, 0, 0);
        CommandBar_Show(g_hWndCommandBar, TRUE);
*/
	}
#endif

#if defined(OS_WINCE)
	//Set fullscreen after window resizing
    m_bFullScreen = RHOCONF().getBool("full_screen");
    RhoSetFullScreen(m_bFullScreen);

    //calculateMainWindowRect(rcMainWindow);
    //MoveWindow(&rcMainWindow);

#endif //OS_WINCE

	rho_rhodesapp_callUiCreatedCallback();

	RHODESAPP().setNetworkStatusMonitor(&m_networkStatusMonitor);

    rho::createIntentEvent();

    return 0;
}

void CMainWindow::calculateMainWindowRect(RECT& rcMainWindow)
{
	// Compute RECT for initial size and position.
    // The following code should compute RECT appropriately
    // on both Pocket PC and Smartphone. It should function correctly
    // whether SIP is on or off, and
    // whether device is in portrait or landscape mode.
    // (rcMainWindow was initialized above)

    SystemParametersInfo(SPI_GETWORKAREA, 0, &rcMainWindow, 0);
    LOG(INFO)  + "SPI_GETWORKAREA: x=" + rcMainWindow.right + ";y=" + rcMainWindow.bottom;

#if defined(OS_WINCE)
	if(RHO_IS_WMDEVICE)
	{
        RECT rcMenuBar = { 0 };
        // (rcMenuBar was initialized above)
        m_menuBar.GetWindowRect(&rcMenuBar);
        rcMainWindow.bottom = rcMenuBar.top;

        SIPINFO si = { sizeof(si), 0 };
        // SIP state
        // (si was initialized above)
        if (rho_wmimpl_get_resize_on_sip() && SHSipInfo(SPI_GETSIPINFO, 0, &si, 0) &&
            (si.fdwFlags & SIPF_ON) && (si.fdwFlags & SIPF_DOCKED))
        {
            rcMainWindow.bottom = si.rcVisibleDesktop.bottom;
        }
    	
    }else if(RHO_IS_CEDEVICE)
	{
	    if (m_bFullScreen)
		    rcMainWindow.bottom =  GetSystemMetrics(SM_CYSCREEN);
	}
#endif

}

void CMainWindow::initBrowserWindow()
{
    m_pBrowserEng = rho::BrowserFactory::getInstance()->create(m_hWnd);

    CRect rect;
    GetWindowRect(&rect);
    resizeWindow(rect.Width(), rect.Height());
    //m_pBrowserEng->ResizeOnTab(0, rect);

    HRESULT hr = S_OK;
#if !defined(APP_BUILD_CAPABILITY_WEBKIT_BROWSER) && !defined(OS_PLATFORM_MOTCE)
    // set up connection point
    hr = AtlAdviseSinkMap(this, true);
#endif

}

void CMainWindow::performOnUiThread(rho::common::IRhoRunnable* pTask)
{
	PostMessage(WM_EXECUTE_RUNNABLE, 0, (LPARAM)pTask);
}

LRESULT CMainWindow::OnExecuteRunnable(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/) 
{
    rho::common::IRhoRunnable* pTask = (rho::common::IRhoRunnable*)lParam;
	if (pTask)
    {
		pTask->runObject();
        delete pTask;
    }
	return 0;
}

LRESULT CMainWindow::OnSetText(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
    return TRUE;
}

LRESULT CMainWindow::OnNotify(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled)
{
    LPNMHDR pnmh = (LPNMHDR) lParam;
    if(pnmh->hwndFrom == m_toolbar.m_hWnd)
    {
        LPNMCUSTOMDRAW lpNMCustomDraw = (LPNMCUSTOMDRAW) lParam;
        CRect rect;
        m_toolbar.GetClientRect(rect);
        FillRect(lpNMCustomDraw->hdc, rect, (HBRUSH)GetStockObject(WHITE_BRUSH));
        SetBkColor(lpNMCustomDraw->hdc, RGB(255, 0,0 ) );
        SetTextColor(lpNMCustomDraw->hdc, RGB(255, 0,0 ) );
        bHandled = TRUE;
    }else
        bHandled = FALSE;
    return 1;
}

HWND CMainWindow::getWebViewHWND() 
{
    if (!m_pBrowserEng)
        return 0;

    return m_pBrowserEng->GetHTMLWND( m_oTabBar.GetCurrentTabID() );
}

void CMainWindow::hideWebView() 
{
    if ( m_pBrowserEng )
    {
        ::ShowWindow( m_pBrowserEng->GetHTMLWND(m_oTabBar.GetCurrentTabID()), SW_HIDE );
	    mIsBrowserViewHided = true;
    }
}

void CMainWindow::showWebView() 
{
    if ( m_pBrowserEng )
    {
        ::ShowWindow( m_pBrowserEng->GetHTMLWND(m_oTabBar.GetCurrentTabID()), SW_SHOW );
	    mIsBrowserViewHided = false;
    }
}

LRESULT CMainWindow::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
#if defined(APP_BUILD_CAPABILITY_SHARED_RUNTIME)
	RHODESAPP().getExtManager().OnQuittingTheApplication();
#endif

	rho_rhodesapp_callUiDestroyedCallback();

#if defined (_WIN32_WCE)
    m_menuBar = NULL;
    RhoSetFullScreen(false, true);

#endif
    

#if !defined(APP_BUILD_CAPABILITY_WEBKIT_BROWSER) && !defined(OS_PLATFORM_MOTCE)
    // Tear down connection points while controls are still alive.
    RHO_ASSERT(SUCCEEDED(AtlAdviseSinkMap(this, false)));
#endif

    if ( m_pBrowserEng )
        delete m_pBrowserEng;

    m_pBrowserEng = NULL;
	
    PostQuitMessage(0);

    bHandled = FALSE; // Allow ATL's default processing (e.g. NULLing m_hWnd)

    return 0;
}

LRESULT CMainWindow::OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
    resizeWindow(LOWORD(lParam), HIWORD(lParam));
    return 0;
}

#if defined(_WIN32_WCE)
int getSIPVisibleTop() {
	SIPINFO pSipInfo;
	memset(&pSipInfo, 0, sizeof(SIPINFO));
	pSipInfo.cbSize = sizeof(SIPINFO);
	pSipInfo.dwImDataSize = 0;
	if (SipGetInfo(&pSipInfo))
		return (pSipInfo.fdwFlags & SIPF_ON) ? pSipInfo.rcSipRect.top : -1;
	return -1;
}
#endif

void CMainWindow::resizeWindow( int xSize, int ySize)
{
    LOG(INFO)  + "resizeWindow: xSize=" + xSize + ";ySize=" + ySize;

//#if defined(_WIN32_WCE)
//	int SIPtop = getSIPVisibleTop();
//	if (m_bFullScreen && (SIPtop>=0))
//		ySize = SIPtop;
//#endif

    RECT rect = {0, 0, xSize, ySize };//- m_toolbar.getHeight()};

    if ( m_toolbar.m_hWnd )
        rect.bottom -= m_toolbar.getHeight();

	if(RHO_IS_CEDEVICE)
	{
        if (!m_bFullScreen && g_hWndCommandBar)
        {
            CRect rcCmdBar;
            ::GetWindowRect(g_hWndCommandBar, &rcCmdBar);
            m_menuBarHeight = rcCmdBar.Height();

            rect.top += m_menuBarHeight;

            rcCmdBar.right = rcCmdBar.left + ySize;

            ::SetWindowPos(g_hWndCommandBar, NULL, 0,0, rcCmdBar.Width(), rcCmdBar.Height(), SWP_FRAMECHANGED|SWP_NOMOVE|SWP_NOOWNERZORDER|SWP_NOZORDER);
            //::MoveWindow( g_hWndCommandBar, rcCmdBar.left, rcCmdBar.top, rcCmdBar.Width(), rcCmdBar.Height(), TRUE );
        }
    }

    if ( m_pBrowserEng && m_pBrowserEng->GetHTMLWND(m_oTabBar.GetCurrentTabID()) )
    {
        //if(rho::BrowserFactory::getCurrentBrowserType() != eIE)
        //{
        //m_pBrowserEng->ResizeOnTab(m_oTabBar.GetCurrentTabID(), rect);
        //}
        

		if(rho::BrowserFactory::getCurrentBrowserType() != eIE)//webkit
		{
		 LOG(INFO)+"ResizeOnTab..";
		 m_pBrowserEng->ResizeOnTab(m_oTabBar.GetCurrentTabID(), rect);
		}
		else//IE
		{

			if(RHO_IS_CEDEVICE)//CE IE Engine
			{
			 LOG(INFO)+"ResizeOnTab.";
			 m_pBrowserEng->ResizeOnTab(m_oTabBar.GetCurrentTabID(), rect);
			}

		}
     }

    if ( m_toolbar.m_hWnd )
        m_toolbar.MoveWindow(0, ySize-m_toolbar.getHeight(), xSize, m_toolbar.getHeight());

}

LRESULT CMainWindow::OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
    bHandled = TRUE;
    LRESULT retCode;

    LOG(INFO) + "START load png";
    
    //SR:EMBPD00150338 - Fixed SIP issue & SR:EMBPD00155397 - IE Coke issue
    if(rho::BrowserFactory::getCurrentBrowserType() != eIE)
    { 
	if(m_bLoading)
	{
		retCode = rho_wmimpl_draw_splash_screen(m_hWnd);
	}
    }
    else
    {
	retCode = rho_wmimpl_draw_splash_screen(m_hWnd);
    }
    
    //SPR 23830 - Fix - Do loading.html On load png failure
    if (retCode == 0 && m_bLoading)
    {
        LOG(INFO) + "CMainWindow::OnPaint: loading.png cannot be found. Try loading.html";

        String strLoadingPagePath = RHODESAPP().getLoadingPagePath();
        if ( String_startsWith( strLoadingPagePath, "file://" ) )
            strLoadingPagePath = strLoadingPagePath.substr(7);

        if (CRhoFile::isFileExist( strLoadingPagePath.c_str()))
        {
            StringW pathW = convertToStringW(RHODESAPP().getLoadingPagePath());

            //If engine is ready then try the following method to navigate to loading.html
            bool bNavigate = true;
            if ( m_pBrowserEng )
                bNavigate = m_pBrowserEng->NavigateToHtml(pathW.c_str()) ? false : true;
            
            //It does not make any sense to show loading.html, since first page will be loaded anyway, just after browser will be created.
            /*if (bNavigate)
            {
                LOG(INFO) + "CMainWindow::OnPaint: m_pBrowserEng is NULL, navigate to loading.html";

                //If engine is not ready then queue Navigate request for loading.html
                rho_webview_navigate(RHODESAPP().getLoadingPagePath().c_str(), 0 );
            }*/
        }else
            LOG(INFO) + "CMainWindow::OnPaint: loading.html cannot be found";
    }
    
    LOG(INFO) + "END load png";

    return 0;
}

LRESULT CMainWindow::OnWebKitMessages(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    if ( !m_pBrowserEng )
        return 0;

    return m_pBrowserEng->OnWebKitMessages(uMsg, wParam, lParam, bHandled);
}

LRESULT CMainWindow::OnTitleChangeCommand (WORD /*wNotifyCode*/, WORD /*wID*/, HWND hwnd, BOOL& /*bHandled*/)
{
    LOG(INFO) + "OnTitleChangeCommand";

    ProcessTitleChange( (LPCTSTR)hwnd );

    free( (void*)(LPCTSTR)hwnd );
	return 0;
};

#if defined(APP_BUILD_CAPABILITY_WEBKIT_BROWSER)
LRESULT CMainWindow::OnBrowserDocumentComplete (UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
	rho::fireIntentEvent();

	ProcessDocumentComplete( (LPCTSTR)lParam );

	free((void*)lParam);
    return 0;
}

LRESULT CMainWindow::OnNavigateComplete(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
    ProcessNavigateComplete( (LPCTSTR)lParam );

    free((void*)lParam);
    return 0;
}

LRESULT CMainWindow::OnTitleChange (UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
    ProcessTitleChange( (LPCTSTR)lParam );

    free((void*)lParam);
    return 0;
}

LRESULT CMainWindow::OnBeforeNavigate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    Rhodes_WM_ProcessBeforeNavigate((LPCTSTR)lParam);

#ifdef APP_BUILD_CAPABILITY_WEBKIT_BROWSER
    if (( m_bLoading ==true)&&(m_bLicenseScreenShownFirsttime==true))
	{
        	LOG(INFO) + "Showing License screen";
		m_bLicenseScreenShownFirsttime = false;
		rho_wm_impl_CheckLicense();
	}
#endif

    free((void*)lParam);
    return 0;
}

LRESULT CMainWindow::OnNavigateTimeout (UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
    PROF_STOP("BROWSER_PAGE");
    LOG(INFO) + "Calling OnNavigateTimeout";
    LRESULT lRes =  RHODESAPP().getExtManager().OnNavigateTimeout((LPCTSTR)lParam);

    free((void*)lParam);

    return lRes;
}

LRESULT CMainWindow::OnNavigateError (UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
    PROF_STOP("BROWSER_PAGE");
    LRESULT lRes =  RHODESAPP().getExtManager().OnNavigateError((LPCTSTR)lParam);

    free((void*)lParam);

    return lRes;
}

LRESULT CMainWindow::OnSetSIPState (UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
    return RHODESAPP().getExtManager().OnSIPState(lParam != 0 ? true:false);
}

LRESULT CMainWindow::OnAlertPopup (UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
    return RHODESAPP().getExtManager().OnAlertPopup(wParam, (void*)lParam);
}

LRESULT CMainWindow::OnAuthenticationRequest (UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
    return RHODESAPP().getExtManager().OnAuthenticationRequest(wParam, (void*)lParam);
}

//LRESULT CMainWindow::OnGeolocationData (UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
//{
//    return RHODESAPP().getExtManager().OnGeolocationData(wParam, (void*)lParam);
//}

#endif //APP_BUILD_CAPABILITY_WEBKIT_BROWSER

LRESULT CMainWindow::OnWindowMinimized (UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
    ProcessActivate( FALSE, MAKEWPARAM(0,1), 0 );

	//SetForegroundWindow(m_hWnd);
	LOG(INFO)+"Window getting minimized-Show taskbar..";	
	showTaskBar(true);
	::ShowWindow( m_hWnd, SW_MINIMIZE );
	HWND hwnd = ::GetForegroundWindow();
	if(hwnd!=NULL)
	{
		wchar_t szBuf[200];
		::GetWindowText(hwnd,szBuf,199);
		if(wcscmp(szBuf, L"RhoElements") == 0)
			::ShowWindow(hwnd, SW_MINIMIZE);
	}

	m_isMinimized = true;
	
	//Title bar was seen when application was minimized at the top in WM device
	if(RHO_IS_WMDEVICE)
	{
		INPUT ip; 
		POINT pos;
		GetCursorPos(&pos);
		// Set up a generic mouse event.
		ip.type = INPUT_MOUSE;
		ip.mi.dx = pos.x;
		ip.mi.dy = pos.y;
		ip.mi.dwFlags = MOUSEEVENTF_RIGHTUP|MOUSEEVENTF_RIGHTDOWN;
		ip.mi.time = 0;
		SendInput(1, &ip, sizeof(INPUT));
	}
	
    return 0;
}

bool GetPowerButtonPressedValue(HWND hwnd)
{
	bool powerButtonPressed=false;
	wchar_t szBuf[200];
	if(hwnd!=NULL)
	{
		//LOG(INFO) + "Inside if(hwnd!=NULL)";
		GetWindowText(hwnd,szBuf,199);
		//LOG(INFO) + "After GetWindowText";
		LOG(INFO) + szBuf;
	}
	if(0==wcscmp(szBuf,L"PowerKey Action"))
	{
		//LOG(INFO) + "Before powerButtonPressed=true;";
		powerButtonPressed=true;
		m_SuspendedThroughPowerButton=true;
		//LOG(INFO) + "After m_SuspendedThroughPowerButton=true;";
	}
	else
	{
		
		//LOG(INFO) + "Before powerButtonPressed=False;";
		powerButtonPressed=false;
	}
	return powerButtonPressed;
}


LRESULT CMainWindow::OnActivate(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
    int fActive = LOWORD(wParam);
    LOG(INFO) + "ACTIVATE: " + fActive;
	bool bNativeWindow = false;

    if ( m_bLoading && !fActive )
    {
        LOG(INFO) + "Get deactivate before first navigate. Skip it.";
        return 0;
    }

	if(m_isMinimized && fActive)
	{
		m_isMinimized = false;
		SendMessage( m_hWnd, PB_WINDOW_RESTORE, NULL, TRUE);
	}

#if defined(_WIN32_WCE) 
	wchar_t szClassName[15];
	if (lParam && GetClassName((HWND)lParam, szClassName, 14))
	{
		if(wcsncmp(szClassName, L"PB_ADDRESSBAR", 13) == 0 ||
			wcsncmp(szClassName, L"PB_BUTTON", 9) == 0 ||
			wcsncmp(szClassName, L"PB_INDICATOR", 12) == 0)
		{
			bNativeWindow = true;
		}
	}
#endif

    if (lParam) //We get activate from some internal window
    {
#if defined(_WIN32_WCE) 
        if (m_bFullScreen && fActive && (getSIPVisibleTop()<0))
	        RhoSetFullScreen(true);
#endif
		//  Ignore the activate msg from the child window if
		//  1. If it is from any window other than a debug window or
		//  2. It is a de-activate
		if (!(bNativeWindow && fActive))
		{
	        LOG(INFO) + "Get activate from child window. Skip it.";
			return 0;
		}
    }

    ProcessActivate( fActive, wParam, lParam );    
    return 0;
}



void CMainWindow::ProcessActivate( BOOL fActive, WPARAM wParam, LPARAM lParam )
{
#if defined(_WIN32_WCE) 
	if (m_bFullScreen)
    {
	if(fActive!=0)//if activated Hide taskbar
	{
	LOG(INFO)+"Hide taskbar";	
	showTaskBar(false);
	}
    }
#endif
	rho_rhodesapp_callAppActiveCallback(fActive);

	if(m_SuspendedThroughPowerButton==true)//Activation is due to Suspend Through PowerButton and resume
	{
	 m_SuspendedThroughPowerButton=false;
	 LOG(INFO) + "Activation is due to powerbutton press suspend and resume.Skip it";
     RHODESAPP().getExtManager().OnPowerButton(true);
	 //return 0;
	}
	else
	{
		HWND currentForeGroundWindowHandle;
		//LOG(INFO) + "Before GetForegroundWindow";
		currentForeGroundWindowHandle = GetForegroundWindow();
		//LOG(INFO) + "After GetForegroundWindow";
		bool powerButtonPressed = GetPowerButtonPressedValue(currentForeGroundWindowHandle);

		if(true ==powerButtonPressed)//Deactivation is not due to Suspend Through PowerButton
		{
			 LOG(INFO) + "Deactivation is due to powerbutton press.Skip it";
			 RHODESAPP().getExtManager().OnPowerButton(false);
		}
		else
		{
			//LOG(INFO) + "Before ProcessActivate";
			RHODESAPP().getExtManager().OnAppActivate(fActive!=0);
		}
		//RHODESAPP().getExtManager().OnAppActivate(fActive!=0);
	}
#if defined(_WIN32_WCE)
	// Notify shell of our WM_ACTIVATE message
	if(RHO_IS_WMDEVICE)
		SHHandleWMActivate(m_hWnd, wParam, lParam, &m_sai, 0);
#endif

    if (!fActive)
        rho_geoimpl_turngpsoff();
}

LRESULT CMainWindow::OnSetCookieCommand(WORD /*wNotifyCode*/, WORD /*wID*/, HWND hWndCtl, BOOL& /*bHandled*/)
{
    TCookieData* cd = (TCookieData*)hWndCtl;
    if (cd) {
        if (m_pBrowserEng && cd->url && cd->cookie) {
            m_pBrowserEng->SetCookie(cd->url, cd->cookie);
        }
        delete cd;
    }
    return 0;
}

void CMainWindow::openNativeView(	NativeViewFactory* nativeViewFactory, 
					NativeView* nativeView,
					String nativeViewType) 
{
	mNativeView = nativeView;
	mNativeViewFactory = nativeViewFactory;
	mNativeViewType = nativeViewType;

	HWND nvh = (HWND)mNativeView->getView();

	::SetParent(nvh, m_hWnd);

	RECT rect;
	::GetWindowRect(getWebViewHWND(),&rect);

	int x = 0;
	int y = 0;
	int w = rect.right - rect.left;
	int h = rect.bottom - rect.top;

	::SetWindowPos(nvh, HWND_TOP, x, y, w, h, SWP_SHOWWINDOW);

	::ShowWindow(nvh, SW_SHOWNORMAL);
	hideWebView();
}

void CMainWindow::closeNativeView() {
	restoreWebView();
}




// return true if NativeView was created
String CMainWindow::processForNativeView(String _url) {

	String url = _url.c_str();
	String callback_prefix = "call_stay_native";

	// find protocol:navto pairs

	int last = -1;
	int cur = url.find_first_of(':', last+1);
	while (cur > 0) {
		String protocol = url.substr(last+1, cur - last - 1);
		String navto = url.substr(cur+1, url.size() - cur);
		
		if (callback_prefix.compare(protocol) == 0) {
			// navigate but still in native view
			String cleared_url = url.substr(callback_prefix.size()+1, url.size() - callback_prefix.size());
			return cleared_url;
		}
		// check protocol for nativeView
		NativeViewFactory* nvf = RhoNativeViewManagerWM::getFactoryByViewType(protocol.c_str());
		if (nvf != NULL) {
			// we should switch to NativeView
			if (mNativeView != NULL) {
				if (protocol.compare(mNativeViewType) == 0) {
					mNativeView->navigate(navto.c_str());
					return "";
				}
			}

			restoreWebView();
			NativeView* nv = nvf->getNativeView(protocol.c_str());
			if (nv != NULL) {

				openNativeView(nvf, nv, protocol);

				nv->navigate(navto.c_str());

				mIsOpenedByURL = true;

				return "";
			}
			restoreWebView();
			return url;
		}
		last = cur;
		int c1 = url.find_first_of(':', last+1);
		int c2 = url.find_first_of('/', last+1);
		if ((c1 < c2)) {
			if (c1 <= 0) {
				cur = c2;
			}
			else {
				cur = c1;
			}
		}
		else {
			if (c2 <= 0) {
				cur = c1;
			}
			else {
				cur = c2;
			}
		}
	}
	if (mIsOpenedByURL) {
		restoreWebView();
	}
	return url;
}


void CMainWindow::restoreWebView() {
	if (mNativeView != NULL) {
		mNativeViewFactory->destroyNativeView(mNativeView);
		mNativeView = NULL;
		mNativeViewFactory = NULL;
		mNativeViewType = "";
		mIsOpenedByURL = false;
		showWebView();
	}
}


LRESULT CMainWindow::OnSettingChange(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
    LOG(INFO) + "OnSettingChange: " + wParam;
#if defined(_WIN32_WCE)
	
	//handle sreen rotation
	int width  = GetSystemMetrics(SM_CXSCREEN);	
	int height = GetSystemMetrics(SM_CYSCREEN);
	
	if (wParam == SETTINGCHANGE_RESET || (!rho_wmimpl_get_resize_on_sip() && (wParam == SPI_SETSIPINFO)) ) {

		rho_rhodesapp_callScreenRotationCallback(width, height, (m_bFullScreen ? 0 : 90) );

//        if (m_pBrowserEng)
//            m_pBrowserEng->OnWebKitMessages(PB_SCREEN_ORIENTATION_CHANGED, wParam, lParam, bHandled);

		if(RHO_IS_CEDEVICE)
		{
			RECT rcMain;    
			calculateMainWindowRect(rcMain);
			MoveWindow( rcMain.left, rcMain.top, rcMain.right-rcMain.left, rcMain.bottom-rcMain.top, TRUE );
		}

	//} else if (wParam == SPI_SIPMOVE) {
	} else if (rho_wmimpl_get_resize_on_sip() && (wParam == SPI_SETSIPINFO)) {
		SIPINFO pSipInfo;
		memset(&pSipInfo, 0, sizeof(SIPINFO));
		pSipInfo.cbSize = sizeof(SIPINFO);
		pSipInfo.dwImDataSize = 0;
		if (SipGetInfo(&pSipInfo)) {
			bool isHiding = (pSipInfo.fdwFlags & SIPF_ON) == 0;

            if ( isHiding && !m_bFullScreen )
            {
                RECT rcMain;    
                calculateMainWindowRect(rcMain);
                MoveWindow( rcMain.left, rcMain.top, rcMain.right-rcMain.left, rcMain.bottom-rcMain.top, TRUE );
                return 0;
            }

			if (m_bFullScreen)
				pSipInfo.rcVisibleDesktop.top = 0;

			int bottom = height - ( m_bFullScreen ? 0 : GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYFIXEDFRAME) );

			bool doSIPmove = false;
			if (isHiding) {
				// when hiding SIP, main window is expanded to cover an entire screen
				pSipInfo.rcVisibleDesktop.bottom = bottom;
			} else {
				LONG deltaY = bottom - pSipInfo.rcSipRect.bottom;
				if (deltaY != 0) {
					pSipInfo.rcSipRect.top += deltaY;
					pSipInfo.rcSipRect.bottom += deltaY;
					doSIPmove = true;
				}
				if(RHO_IS_CEDEVICE)
				{
					if ((pSipInfo.rcSipRect.left != 0) || (pSipInfo.rcSipRect.right != width)) {
						pSipInfo.rcSipRect.left = 0;
						pSipInfo.rcSipRect.right = width;
						doSIPmove = true;
					}
				}
				pSipInfo.rcVisibleDesktop.bottom = pSipInfo.rcSipRect.top;
			}
			CRect cRect;
			this->GetClientRect(&cRect);
			if ((cRect.bottom != pSipInfo.rcVisibleDesktop.bottom) ||
				(cRect.top != pSipInfo.rcVisibleDesktop.top) ||
				(cRect.left != pSipInfo.rcVisibleDesktop.left) ||
				(cRect.right != pSipInfo.rcVisibleDesktop.right)) 
			{
				MoveWindow(&pSipInfo.rcVisibleDesktop, TRUE);
			}
			if (doSIPmove && (!isHiding)) {
				HWND sipHWND = FindWindow(L"SipWndClass", NULL);
				if (sipHWND) {
					::SetWindowPos(sipHWND, 0, pSipInfo.rcSipRect.left, pSipInfo.rcSipRect.top,
						pSipInfo.rcSipRect.right-pSipInfo.rcSipRect.left, pSipInfo.rcSipRect.bottom-pSipInfo.rcSipRect.top,
						SWP_SHOWWINDOW);
				}
			}

			if(RHO_IS_CEDEVICE && m_toolbar.m_hWnd)
				m_toolbar.MoveWindow(0, (isHiding ? bottom : pSipInfo.rcSipRect.top) - m_toolbar.getHeight(), width, m_toolbar.getHeight());
			else if(RHO_IS_WMDEVICE && m_bFullScreen && m_toolbar.m_hWnd)
				m_toolbar.MoveWindow(0, (isHiding ? bottom : pSipInfo.rcSipRect.top) - m_toolbar.getHeight(), width, m_toolbar.getHeight());

            if ( m_bFullScreen && isHiding )
                hideSIPButton();
		}
	}
	
	// Notify shell of our WM_SETTINGCHANGE message
	//SHHandleWMSettingChange(m_hWnd, wParam, lParam, &m_sai);

#endif
    return 0;
}

LRESULT CMainWindow::OnHotKey (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    LOG(INFO) + "OnHotKey : " + wParam;
    bHandled = TRUE;

    return 1;
}

extern "C" HWND getMainWnd();
BOOL EnumChildProc(HWND hwnd,LPARAM lParam)
{
    if ( getMainWnd() != ::GetParent(hwnd) )
        return TRUE;

    wchar_t szBuf[200];
    GetWindowText(hwnd, szBuf, 199);
    if (!*szBuf)
        return TRUE;

    DWORD dwStyles = GetWindowLong(hwnd, GWL_STYLE);    
    //LOG(INFO) + "Child: " + szBuf + "Styles: " + LOGFMT("0x%X") + dwStyles + "Popup: " + (((dwStyles&WS_POPUP) != 0) ? "1" : "0");

	if ( *((HWND*)lParam) != hwnd && (dwStyles&WS_POPUP) != 0 )
	{
		HWND* pWnd = (HWND*)lParam;
		*pWnd = hwnd;
		return FALSE;
	}

	return TRUE;
}

LRESULT CMainWindow::OnSetFocus (UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
    HWND hWndLostFocus = (HWND)wParam;
	WCHAR wWindowName[30];
	if (hWndLostFocus && GetClassName(hWndLostFocus, wWindowName, 30))
	{
		//  Allow the following windows to remain having focus, avoids
		//  the bug where we're unable to select anything from a combo 
		//  box
		if (wcscmp(wWindowName, L"PopupWindowClass") == 0)
	        return 0;
	}

    //Look for popup window
    HWND hChildPopUp = hWndLostFocus;
    EnumWindows( EnumChildProc, (LPARAM)&hChildPopUp);
    if ( hChildPopUp && hChildPopUp != hWndLostFocus)
    {
        ::SetFocus(hChildPopUp);
        return 0;
    }

    HWND hBrowserWnd = m_pBrowserEng ? m_pBrowserEng->GetHTMLWND(m_oTabBar.GetCurrentTabID()) : NULL;
    if (hBrowserWnd && ::IsWindowVisible(m_hWnd) ) //!::IsIconic(m_hWnd))
	{
		if (hWndLostFocus == hBrowserWnd)
	        return 0;
        ::SetFocus(hBrowserWnd);
	}

    return 0;
}

// **************************************************************************
//
// WM_COMMAND handlers
//
// **************************************************************************

LRESULT CMainWindow::OnExitCommand(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    SendMessage(WM_CLOSE);
    return 0;
}

LRESULT CMainWindow::OnNavigateBackCommand(WORD /*wNotifyCode*/, WORD /*wID*/, HWND hWndCtl, BOOL& /*bHandled*/)
{
	restoreWebView();

    if ( m_pBrowserEng )
        m_pBrowserEng->BackOnTab(m_oTabBar.GetTabID((int)hWndCtl), 1);

    return 0;
}

LRESULT CMainWindow::OnNavigateForwardCommand(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	restoreWebView();

    if ( m_pBrowserEng )
        m_pBrowserEng->ForwardOnTab(m_oTabBar.GetCurrentTabID());

    return 0;
}

LRESULT CMainWindow::OnLogCommand(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CLogView oLogView;
    oLogView.DoModal(m_hWnd);

    return 0;
}

LRESULT CMainWindow::OnFullscreenCommand (WORD /*wNotifyCode*/, WORD /*wID*/, HWND hwnd, BOOL& /*bHandled*/)
{
    LOG(INFO) + "OnFullscreenCommand";
#if defined (_WIN32_WCE)
	RhoSetFullScreen(m_bFullScreen = (hwnd != 0 ? true : false));
#endif
	return 0;
};

LRESULT CMainWindow::OnRefreshCommand(WORD /*wNotifyCode*/, WORD /*wID*/, HWND hWndCtl, BOOL& /*bHandled*/)
{

    if ( m_pBrowserEng )
        m_pBrowserEng->ReloadOnTab(false, m_oTabBar.GetTabID((int)hWndCtl));

    return 0;
}

LRESULT CMainWindow::OnNavTimeout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND hWndCtl, BOOL& /*bHandled*/)
{
    if ( m_pBrowserEng )
        m_pBrowserEng->setNavigationTimeout((int)hWndCtl);

    return 0;
}

LRESULT CMainWindow::OnNavigateCommand(WORD /*wNotifyCode*/, WORD /*wID*/, HWND hWndCtl, BOOL& /*bHandled*/)
{
    TNavigateData* nd = (TNavigateData*)hWndCtl;
    if (nd) {
        LPTSTR wcurl = (LPTSTR)(nd->url);
        if (wcurl)
          {
            LOG(INFO) + "Call of Navigate2 function";
            Navigate2(wcurl, nd->index);
          }

        delete nd;
    }
    return 0;
}

LRESULT CMainWindow::OnExecuteJSCommand(WORD /*wNotifyCode*/, WORD /*wID*/, HWND hWndCtl, BOOL& /*bHandled*/)
{
    TNavigateData* nd = (TNavigateData*)hWndCtl;
    if (nd) {
        LPTSTR wcurl = ((LPTSTR)(_tcsdup(nd->url)));
        if (wcurl) 
        {
            if ( m_pBrowserEng )
                m_pBrowserEng->executeJavascript(wcurl, m_oTabBar.GetTabID(nd->index) );
        }
        delete nd;
        free(wcurl);
    }
    return 0;
}

LRESULT CMainWindow::OnStopNavigate(WORD /*wNotifyCode*/, WORD /*wID*/, HWND hWndCtl, BOOL& /*bHandled*/)
{
    if ( m_pBrowserEng )
        m_pBrowserEng->StopOnTab( m_oTabBar.GetTabID((int)hWndCtl) );

    return 0;
}

LRESULT CMainWindow::OnZoomPage(WORD /*wNotifyCode*/, WORD /*wID*/, HWND hWndCtl, BOOL& /*bHandled*/)
{
    CRhoFloatData* pFloatData = (CRhoFloatData*)(hWndCtl);
    float fZoom = pFloatData->m_fValue;
    delete pFloatData;
    if ( m_pBrowserEng )
    {
        BOOL bRes = m_pBrowserEng->ZoomPageOnTab(fZoom, m_oTabBar.GetCurrentTabID());
        LOG(INFO) + "ZoomPageOnTab return : " + bRes;
        if (bRes)
            InvalidateRect(NULL, TRUE);
    }
    return 0;
}

LRESULT CMainWindow::OnZoomText(WORD /*wNotifyCode*/, WORD /*wID*/, HWND hWndCtl, BOOL& /*bHandled*/)
{
    int nZoom = (int)hWndCtl;
    if ( m_pBrowserEng )
    {
        BOOL bRes = m_pBrowserEng->ZoomTextOnTab(nZoom, m_oTabBar.GetCurrentTabID());
        LOG(INFO) + "ZoomTextOnTab return : " + bRes;
        if (bRes)
            InvalidateRect(NULL, TRUE);
    }

    return 0;
}

LRESULT CMainWindow::OnPopupMenuCommand(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) 
{
	if(RHO_IS_CEDEVICE)
		createCustomMenu();
	return 0;
}

LRESULT CMainWindow::OnTakePicture(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/) 
{
	TCHAR image_uri[MAX_PATH];
	//TCHAR file_name[MAX_PATH];
    HRESULT status;
#if defined (_WIN32_WCE)
	Camera camera;
#ifdef APP_BUILD_CAPABILITY_WEBKIT_BROWSER
	RHODESAPP().m_cameraOpened = true;
#endif
	status = camera.takePicture(this->m_hWnd,image_uri);
#ifdef APP_BUILD_CAPABILITY_WEBKIT_BROWSER
	RHODESAPP().m_cameraOpened = false;
#endif
#else
    //TODO: show browse file dialog
    wsprintf( image_uri, L"%s", L"dashboard.PNG");
    status = S_OK;
#endif

    RHODESAPP().callCameraCallback( (const char*)lParam, rho::common::convertToStringA(image_uri),
       (status!= S_OK && status != S_FALSE ? "Error" : ""), status == S_FALSE);

    free ((void *)lParam);
	return 0;
}

LRESULT CMainWindow::OnConnectionsNetworkCount(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/) 
{
    LOG(INFO) + "OnConnectionsNetworkCount: " + wParam;

#if defined (_WIN32_WCE)
	// TODO: the absolutely correct way to check WiFi connection is: ((wParam & 0x10) != 0) - instead of just (wParam > 1)
	// (but this code needs to be tested on all supported WM/CE platforms first; it seems to work fine on WM6Pro)
	rho_sysimpl_sethas_network( (wParam > 1) ? 1 : 0 );
	m_networkStatusMonitor.notifyReceiver( (wParam > 1) ? rho::common::networkStatusConnected : rho::common::networkStatusDisconnected );

#endif
	return 0;
}

LRESULT CMainWindow::OnConnectionsNetworkCell(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/) 
{
    LOG(INFO) + "OnConnectionsNetworkCell: " + wParam;

#if defined (_WIN32_WCE)
	rho_sysimpl_sethas_cellnetwork( (int)wParam );
	m_networkStatusMonitor.notifyReceiver( (wParam!=0)?rho::common::networkStatusConnected:rho::common::networkStatusDisconnected );

#endif
	return 0;
}

LRESULT CMainWindow::OnSelectPicture(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/) 
{
	TCHAR image_uri[MAX_PATH];
	//TCHAR file_name[MAX_PATH];
    HRESULT status = S_OK;
	Camera camera;
	status = camera.selectPicture(this->m_hWnd,image_uri);

    RHODESAPP().callCameraCallback( (const char*)lParam, rho::common::convertToStringA(image_uri),
         (status!= S_OK && status != S_FALSE ? "Error" : ""), status == S_FALSE);
    
    free ((void *)lParam);
    
	return 0;
}

#if 0
LRESULT CMainWindow::OnTakeSignature(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/) 
{
    if ( wParam == 0 )
    {
	    TCHAR signature_uri[MAX_PATH];
        HRESULT status = S_OK;
	    Signature::Params* params = (Signature::Params *)lParam;
	    status = Signature::takeSignature(this->m_hWnd, signature_uri, params);

        RHODESAPP().callSignatureCallback(params->m_callback_url.c_str(), rho::common::convertToStringA(signature_uri),
            (status!= S_OK && status != S_FALSE ? "Error" : ""), status == S_FALSE);
        
        free ((void *)lParam);
    }else
    {
	    Signature::Params* params = (Signature::Params *)lParam;
        Signature::showSignature(m_hWnd/*m_pBrowserEng->GetHTMLWND()*/, params);
    }

	return 0;
}
#endif

LRESULT CMainWindow::OnAlertShowPopup (UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
	//LOG(INFO) + __FUNCTION__;
    StringW strAppName = RHODESAPP().getAppNameW();
	CAlertDialog::Params *params = (CAlertDialog::Params *)lParam;

   	if (params->m_dlgType == CAlertDialog::Params::DLG_STATUS) 
    {
        m_SyncStatusDlg.setStatusText(convertToStringW(params->m_message).c_str());
        m_SyncStatusDlg.setTitle( convertToStringW(params->m_title).c_str() );
        if ( !m_SyncStatusDlg.m_hWnd )
            m_SyncStatusDlg.Create(m_hWnd, 0); 
        //else
        //{
        m_SyncStatusDlg.ShowWindow(SW_SHOW);
        m_SyncStatusDlg.BringWindowToTop();
        //}
    }else if (params->m_dlgType == CAlertDialog::Params::DLG_DEFAULT) {
		MessageBox(convertToStringW(params->m_message).c_str(), strAppName.c_str(), MB_ICONWARNING | MB_OK);
        RHODESAPP().callPopupCallback(params->m_callback, "ok", "ok");
	} else if (params->m_dlgType == CAlertDialog::Params::DLG_CUSTOM) 
    {
        if ( params->m_buttons.size() == 1 && strcasecmp(params->m_buttons[0].m_strCaption.c_str(), "ok") == 0)
        {
            MessageBox(convertToStringW(params->m_message).c_str(), convertToStringW(params->m_title).c_str(), MB_ICONWARNING | MB_OK);
            RHODESAPP().callPopupCallback(params->m_callback, params->m_buttons[0].m_strID, params->m_buttons[0].m_strCaption);
        }
        else if (params->m_buttons.size() == 2 && strcasecmp(params->m_buttons[0].m_strCaption.c_str(), "ok") == 0 &&
            strcasecmp(params->m_buttons[1].m_strCaption.c_str(), "cancel") == 0)
        {
            int nRes = MessageBox(convertToStringW(params->m_message).c_str(), convertToStringW(params->m_title).c_str(), 
                    MB_ICONWARNING | MB_OKCANCEL);
            int nBtn = nRes == IDCANCEL ? 1 : 0;
            RHODESAPP().callPopupCallback(params->m_callback, params->m_buttons[nBtn].m_strID, params->m_buttons[nBtn].m_strCaption);
        }
        else
        {
		    if (m_alertDialog == NULL) 
            {
			    m_alertDialog = new CAlertDialog(params);
			    m_alertDialog->DoModal();
			    delete m_alertDialog;
			    m_alertDialog = NULL;
		    } else {
			    LOG(WARNING) + "Trying to show alert dialog while it exists.";
		    }
        }
	}

    delete params;
    return 0;
}

LRESULT CMainWindow::OnAlertHidePopup (UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
//Cannot close modal dialog
    /*if (m_alertDialog != NULL) {
        m_alertDialog->EndDialog(0);
        m_alertDialog = NULL;
    }*/

    if ( m_SyncStatusDlg.m_hWnd )
        m_SyncStatusDlg.ShowWindow(SW_HIDE);

	return 0;
}

LRESULT CMainWindow::OnBluetoothDiscover (UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/) {
#if defined( OS_WINCE)
	if(RHO_IS_WMDEVICE)
	{
		RhoDiscoverDlg* dlg = RhoBluetoothManager::getInstance()->getDiscoverDlg();
		dlg->openDialog(RhoBluetoothManager::getInstance());
	}
#endif //
	return 0;
}

LRESULT CMainWindow::OnBluetoothDiscovered (UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/) {
#if defined( OS_WINCE)
	if(RHO_IS_WMDEVICE)
	{
		RhoDiscoveredDlg* dlg = RhoBluetoothManager::getInstance()->getDiscoveredDlg();
		dlg->openDialog(RhoBluetoothManager::getInstance());
	}
#endif // 
	return 0;
}

LRESULT CMainWindow::OnExecuteCommand(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/) {
	RhoNativeViewRunnable* command = (RhoNativeViewRunnable*)wParam;
	if (command != NULL) {
		command->run();
	}
	return 0;
}	
LRESULT CMainWindow::OnLicenseWarning (UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
    ::MessageBoxW( m_hWnd, L"Please provide RhoElements license key.", L"Symbol License", MB_ICONERROR | MB_OK);

    return 0;
}
LRESULT CMainWindow::OnCreateShortcutViaXML(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
	if(RHO_IS_WMDEVICE)
	{
		RHODESAPP().getExtManager().OnCreateShortcutViaXML(true);
	}
	else if(RHO_IS_CEDEVICE)
	{
		RHODESAPP().getExtManager().OnCreateShortcutViaXML(false);
	}
	return 0;
}
LRESULT CMainWindow::OnLicenseScreen(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
    LOG(INFO) + "OnLicenseScreen";
#if defined( OS_WINCE )
    if ( wParam != 0 )
    {
        m_bFullScreenBeforeLicense = m_bFullScreen;
	    if (!m_bFullScreen)
		    RhoSetFullScreen(true);
    }else
    {
        if ( !m_bFullScreenBeforeLicense )
            RhoSetFullScreen(false);
    }
	//send messages to extention which looks for license screens
	RHODESAPP().getExtManager().OnLicenseScreen(static_cast<bool>(wParam));

/*		HWND hTaskBar = FindWindow(_T("HHTaskBar"), NULL);
		if(hTaskBar) {
			bool bEnableTaskBar = (wParam == 0);
			::ShowWindow(hTaskBar, (bEnableTaskBar ? SW_SHOW : SW_HIDE));
			::EnableWindow(hTaskBar, bEnableTaskBar);
		}*/
#endif

    //Fix issue with lost focus after License Screen hides
    HWND hBrowserWnd = m_pBrowserEng ? m_pBrowserEng->GetHTMLWND(m_oTabBar.GetCurrentTabID()) : NULL;
    if (hBrowserWnd && !::IsIconic(m_hWnd))
        ::SetFocus(hBrowserWnd);

	return 0;
}	

LRESULT CMainWindow::OnBluetoothCallback(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/) {
	char* callback_url = (char*)wParam;
	char* body = (char*)lParam;
	rho_rhodesapp_callBluetoothCallback(callback_url, body);
	delete callback_url;
	delete body;
	return 0;
}	


LRESULT CMainWindow::OnDateTimePicker (UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
	CDateTimeMessage *msg = (CDateTimeMessage *)lParam;
	int retCode	= -1;
	time_t ret_time = 0;

	if (msg->m_format == CDateTimeMessage::FORMAT_TIME) {
		CTimePickerDialog timeDialog(msg);
		retCode = timeDialog.DoModal(m_hWnd);
		ret_time = timeDialog.GetTime();
	} else {
		CDateTimePickerDialog dateTimeDialog(msg);
		retCode = dateTimeDialog.DoModal(m_hWnd);
		ret_time = dateTimeDialog.GetTime();
	}

	rho_rhodesapp_callDateTimeCallback( msg->m_callback, 
										retCode == IDOK ? (long )ret_time : 0,
										msg->m_data,
										retCode == IDOK ? 0 : 1);
	delete msg;

	return 0;
}

// **************************************************************************
//
// event handlers
//
// **************************************************************************
extern "C" void rho_wmsys_run_app(const char* szPath, const char* szParams );

bool Rhodes_WM_ProcessBeforeNavigate(LPCTSTR url)
{
    LOG(TRACE) + "OnBeforeNavigate2: " + url;

    PROF_START("BROWSER_PAGE");

    RHODESAPP().getExtManager().onBeforeNavigate(url);

    const wchar_t *to_remove;
    if ( (to_remove = wcsstr(url, L"rho_open_target=_blank")) != 0)
    {
        wcscpy((wchar_t*)to_remove, (wchar_t*)to_remove+22);

        LOG(INFO) + "Open external browser: " + url;
#ifdef OS_WINCE
        rho_wmsys_run_app(convertToStringA(url).c_str(), 0 );
#else
        rho_wmsys_run_app(convertToStringA(url).c_str(), 0 );
#endif
        return true;
    }
    return false;
}

void __stdcall CMainWindow::OnBeforeNavigate2(IDispatch* pDisp, VARIANT * pvtURL,
                                              VARIANT * /*pvtFlags*/, VARIANT * pvtTargetFrameName,
                                              VARIANT * /*pvtPostData*/, VARIANT * /*pvtHeaders*/,
                                              VARIANT_BOOL * pvbCancel)
{
    USES_CONVERSION;
    LPCTSTR szURL = OLE2CT(V_BSTR(pvtURL));

    if (Rhodes_WM_ProcessBeforeNavigate(szURL))
        *pvbCancel = VARIANT_TRUE;

/*
    String strTitle = RHOCONF().getString("title_text");
    if ( strTitle.length() > 0 )
        SetWindowText(convertToStringW(strTitle).c_str());
    else
        SetWindowText(TEXT("Untitled"));*/
	
    //RHO_ASSERT(SetMenuItemEnabled(IDM_STOP, TRUE));
}

void __stdcall CMainWindow::OnBrowserTitleChange(BSTR bstrTitleText)
{
    USES_CONVERSION;
    ProcessTitleChange(OLE2CT(bstrTitleText));
}

void CMainWindow::ProcessTitleChange(LPCTSTR title)
{
     TCHAR *szWindowTitle = new TCHAR[MAX_PATH];
     ZeroMemory(szWindowTitle, MAX_PATH);

    LOG(TRACE) + "OnBrowserTitleChange: " + title;
    GetWindowText(szWindowTitle, MAX_PATH);
    //return;
    String strTitle = RHOCONF().getString("title_text");
    if(strTitle.length() <=0)
    {
		CHAR szWindowTitleA[MAX_PATH*2];
		ZeroMemory(szWindowTitleA,sizeof(szWindowTitleA));
		sprintf(szWindowTitleA,"%ls",szWindowTitle);
		strTitle.clear();
		strTitle.append(szWindowTitleA);
     }
     RHOCONF().setString("title_text", strTitle.c_str(), false);
	
    if ( strTitle.length() > 0 )
        SetWindowText(convertToStringW(strTitle).c_str());
    else
    {
        LPCTSTR szTitle = title;
        if ( szTitle && 
            (_tcsncmp(szTitle, _T("http:"), 5) == 0 || _tcscmp(szTitle, _T("about:blank"))==0 ))
        {
            delete [] szWindowTitle;
            return;
        }

        SetWindowText(szTitle);

        delete [] szWindowTitle;
    }
}

void __stdcall CMainWindow::OnNavigateComplete2(IDispatch* pDisp, VARIANT * pvtURL)
{
    USES_CONVERSION;
    ProcessNavigateComplete( OLE2CT(V_BSTR(pvtURL)) );
}

void CMainWindow::ProcessNavigateComplete(LPCTSTR url)
{
	if ( !m_bLoading && !mIsBrowserViewHided && m_pBrowserEng )
        ::ShowWindow(m_pBrowserEng->GetHTMLWND(m_oTabBar.GetCurrentTabID()), SW_SHOW);


    RAWLOGC_INFO("WebView", "Page load complete." );

    PROF_STOP("BROWSER_PAGE");
	RHODESAPP().getExtManager().zoomPage( (float)rho_wmimpl_get_pagezoom());
    RHODESAPP().getExtManager().onNavigateComplete(url);
}

void CMainWindow::ShowLoadingPage()
{
	String fname = RHODESAPP().getLoadingPagePath();

	size_t pos = fname.find("file://");
	if (pos == 0 && pos != std::string::npos)
		fname.erase(0, 7);

    CRhoFile oFile;
    StringW strTextW;
    if ( oFile.open( fname.c_str(), common::CRhoFile::OpenReadOnly) )
        oFile.readStringW(strTextW);
    else
    {
		LOG(ERROR) + "failed to open loading page \"" + fname + "\"";
		strTextW = L"<!DOCTYPE html><html><head><title>Loading...</title></head><body><h1>Loading...</h1></body></html>";
    }

    if ( m_pBrowserEng )
        m_pBrowserEng->NavigateToHtml(strTextW.c_str());
}

void __stdcall CMainWindow::OnDocumentComplete(IDispatch* pDisp, VARIANT * pvtURL)
{
	USES_CONVERSION;
	
	ProcessDocumentComplete( OLE2CT(V_BSTR(pvtURL)) );
}

void CMainWindow::ProcessDocumentComplete(LPCTSTR url)
{
	if (m_bLoading && wcscmp(url,_T("about:blank"))==0) 
    {
		LOG(TRACE) + "Show loading page";
		ShowLoadingPage();
		m_bLoading = false; //show loading page only once
    }else
    {
    }
    m_bLoading = false;

    LOG(TRACE) + "OnDocumentComplete: " + url;

#if defined (_WIN32_WCE)
	if(RHO_IS_WMDEVICE)
	{
		//createCustomMenu();
		if (m_pageCounter >= 0)
		{
			m_pageCounter++;
			if (m_pageCounter > 1) //"loading" page + first page
			{
				//Do it once
				SetToolbarButtonEnabled(IDM_SK1_EXIT, TRUE);
				m_pageCounter = -1;
			}
		}
	}
#endif	

    //CMetaHandler oHandler(m_spIWebBrowser2);

    if ( m_pBrowserEng )
        m_pBrowserEng->OnDocumentComplete(url);

    RHODESAPP().getExtManager().onDocumentComplete(url);

}

void __stdcall CMainWindow::OnCommandStateChange(long lCommand, BOOL bEnable)
{
    /*if (CSC_NAVIGATEBACK == lCommand)
    {
        RHO_ASSERT(SetMenuItemEnabled(IDM_BACK, bEnable));
    }
    else */
    /*if (CSC_NAVIGATEFORWARD == lCommand)
    {
        RHO_ASSERT(SetMenuItemEnabled(IDM_FORWARD, bEnable));
    }*/
}

// **************************************************************************
//
// utility function
//
// **************************************************************************
BOOL CMainWindow::SetMenuItemEnabled(UINT uMenuItemID, BOOL bEnable)
{
	return TRUE;
}

BOOL CMainWindow::SetToolbarButtonEnabled(UINT uTbbID, BOOL bEnable)
{

#if defined(_WIN32_WCE)
	TBBUTTONINFO tbbi;
    tbbi.cbSize = sizeof(tbbi);
    tbbi.dwMask = TBIF_STATE;
    tbbi.fsState = bEnable ? TBSTATE_ENABLED : TBSTATE_INDETERMINATE;
    ::SendMessage (m_menuBar, TB_SETBUTTONINFO, uTbbID, (LPARAM)&tbbi);
	return TRUE;
#else
	return TRUE;
#endif

}

BOOL CMainWindow::SetToolbarButtonName(UINT uTbbID, LPCTSTR szLabel)
{

#if defined(_WIN32_WCE)
    TBBUTTONINFO tbbi = {0};
    tbbi.cbSize = sizeof(tbbi);
    tbbi.dwMask = TBIF_TEXT;
    tbbi.pszText = (LPTSTR)szLabel;
    ::SendMessage (m_menuBar, TB_SETBUTTONINFO, uTbbID, (LPARAM)&tbbi);
	return TRUE;
#else
	return TRUE;
#endif

}

// **************************************************************************
//
// CMainWindow::TranslateAccelerator
//
// Required to forward messages to the PIEWebBrowser control (and any other
// ActiveX controls that may be added to the main window's design).
//
// **************************************************************************
BOOL CMainWindow::TranslateAccelerator(MSG* pMsg)
{
    if(RHO_IS_WMDEVICE)
	{
		if (pMsg->message == WM_CONTEXTMENU){
			return TRUE;
		}

		//if (m_bFullScreen && pMsg->message == WM_KEYUP && (pMsg->wParam == VK_F1 ||  pMsg->wParam == VK_F2))
		//	RhoSetFullScreen(false);
	}

    // Accelerators are only keyboard or mouse messages
    UINT uMsg = pMsg->message;
    if (!(WM_KEYFIRST   <= uMsg && uMsg <= WM_KEYLAST) &&
        !(WM_MOUSEFIRST <= uMsg && uMsg <= WM_MOUSELAST))
    {

#ifdef OS_WINCE			
        if ( uMsg == WM_HOTKEY )
        {
            int idHotKey = (int) pMsg->wParam; 
            int fuModifiers = (UINT) LOWORD(pMsg->lParam); 
            int uVirtKey = (UINT) HIWORD(pMsg->lParam);

            if ( uVirtKey == VK_ESCAPE ){
                if ( fuModifiers&MOD_KEYUP )
                    RHODESAPP().navigateBack();
                    //PostMessageW(WM_COMMAND,MAKEWPARAM(IDM_BACK,1),NULL);

                return TRUE;
            }
        }
#endif //OS_WINCE

        return FALSE;
    }

    if (NULL == m_hWnd)
    {
        return FALSE;
    }

	// workaround for escape key in text fields on CE:
	if (RHO_IS_CEDEVICE && (pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_ESCAPE))
		return TRUE;

    // Find a direct child of this window from the window that has focus.
    // This will be AtlAxWin window for the hosted control.
    CWindow control = ::GetFocus();

	// workaround for backspace key in text fields:
	//if (control.m_hWnd && (pMsg->message == WM_KEYUP) && (pMsg->wParam == VK_BACK))
	//{
	//	if(RHO_IS_CEDEVICE)
	//	{
	//		control.SendMessage(WM_KEYDOWN, VK_BACK, 0);
	//		control.SendMessage(WM_KEYUP, VK_BACK, 0);
	//	}
	//	else
	//			control.SendMessage(WM_CHAR, VK_BACK, 1);
		//return TRUE;
	//}

    if (IsChild(control) && m_hWnd != control.GetParent())
    {
        do
        {
            control = control.GetParent();
        } while (m_hWnd != control.GetParent());
    }

    // Give the control (via the AtlAxWin) a chance to translate this message
    if (control.m_hWnd && control.SendMessage(WM_FORWARDMSG, 0, (LPARAM)pMsg))
    {
        return TRUE;
    }

    // If the main window used accelerators, we could have called the global
    // ::TranslateAccelerator() function here, instead of simply returning FALSE.
    return FALSE;
}

LRESULT CMainWindow::OnUpdateMenuCommand(WORD /*wNotifyCode*/, WORD /*wID*/, HWND hWndCtl, BOOL& /*bHandled*/)
{
	if(RHO_IS_WMDEVICE)
    {
        CAppMenuItem oLeftItem = RHODESAPP().getAppMenu().getLeftButton();
        CAppMenuItem oRightItem = RHODESAPP().getAppMenu().getRightButton();

        if ( oLeftItem.m_strLabel.length() > 0 )
        {
            SetToolbarButtonName( IDM_SK1_EXIT, convertToStringW(oLeftItem.m_strLabel).c_str() );
        }

        if (oLeftItem.m_isEnable)
            SetToolbarButtonEnabled(IDM_SK1_EXIT, TRUE);
        else
            SetToolbarButtonEnabled(IDM_SK1_EXIT, FALSE);

        if ( oRightItem.m_strLabel.length() > 0 )
        {
            SetToolbarButtonName( IDM_SK2_MENU, convertToStringW(oRightItem.m_strLabel).c_str() );
        }

        if (oRightItem.m_isEnable)
          SetToolbarButtonEnabled(IDM_SK2_MENU, TRUE);
        else
          SetToolbarButtonEnabled(IDM_SK2_MENU, FALSE);

        createCustomMenu();
    }

    return 0;
}

void CMainWindow::showMenuBarMenu(const CAppMenuItem& menuButton, bool isLeft)
{
    if ( menuButton.m_strLink.length() == 0)
    {
    	RHODESAPP().getAppMenu().copyMenuItems(m_arAppMenuItems, isLeft);

        if (m_arAppMenuItems.size() == 0 )
        {
            if (isLeft) {
                RHODESAPP().navigateBack();
            }
            else {
                // ???
            }
        }
        else
        {
            HMENU hMenu = CreatePopupMenu();
			if(RHO_IS_WMDEVICE)
				createCustomMenuEx( hMenu, m_arAppMenuItems );

            RECT  rcBar = {0}; 
	        m_menuBar.GetWindowRect(&rcBar);

            RECT rcBtn = {0};
            if (isLeft) {
                m_menuBar.SendMessage( TB_GETITEMRECT, 0, (LPARAM)&rcBtn );
            }
            else {
                m_menuBar.SendMessage( TB_GETITEMRECT, 1, (LPARAM)&rcBtn );
            }

            ::TrackPopupMenuEx( hMenu, TPM_CENTERALIGN|TPM_BOTTOMALIGN, rcBtn.left+(rcBtn.right-rcBtn.left)/2, rcBar.top, m_hWnd, 0 );

            DestroyMenu(hMenu);
        }
    }
    else {
        RHODESAPP().loadUrl(menuButton.m_strLink);
    }
}

LRESULT CMainWindow::OnLeftMenuCommand(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{  
    CAppMenuItem oLeftItem = RHODESAPP().getAppMenu().getLeftButton();
    showMenuBarMenu(oLeftItem, true);

    return 0;
}

LRESULT CMainWindow::OnRightMenuCommand(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{    
    CAppMenuItem oRightItem = RHODESAPP().getAppMenu().getRightButton();
    showMenuBarMenu(oRightItem, false);

    return 0;
}

void CMainWindow::createCustomMenu()
{
	if(RHO_IS_CEDEVICE)
	{
	    CMenu menu;
	    CMenu sub;
	    CMenu popup;
    	
        if (!m_pBrowserEng || !m_pBrowserEng->GetHTMLWND(m_oTabBar.GetCurrentTabID()))
            return;

	    VERIFY(menu.CreateMenu());
	    VERIFY(popup.CreatePopupMenu());
	    menu.AppendMenu(MF_POPUP, (UINT) popup.m_hMenu, _T(""));

	    RHODESAPP().getAppMenu().copyMenuItems(m_arAppMenuItems);

    #ifdef ENABLE_DYNAMIC_RHOBUNDLE
        String strIndexPage = CFilePath::join(RHODESAPP().getStartUrl(),"index"RHO_ERB_EXT);
        if ( RHODESAPP().getCurrentUrl().compare(RHODESAPP().getStartUrl()) == 0 ||
             RHODESAPP().getCurrentUrl().compare(strIndexPage) == 0 )
            m_arAppMenuItems.addElement(CAppMenuItem("Reload RhoBundle","reload_rhobundle"));
    #endif //ENABLE_DYNAMIC_RHOBUNDLE

	    //update UI with custom menu items
	    USES_CONVERSION; 
        for ( int i = m_arAppMenuItems.size() - 1; i >= 0; i--)
        {
            CAppMenuItem& oItem = m_arAppMenuItems.elementAt(i);
            if (oItem.m_eType == CAppMenuItem::emtSeparator) 
			    popup.InsertMenu(0, MF_BYPOSITION | MF_SEPARATOR, (UINT_PTR)0, (LPCTSTR)0);
		    else
            {
                StringW strLabelW = convertToStringW(oItem.m_strLabel);

			    popup.InsertMenu(0, MF_BYPOSITION, ID_CUSTOM_MENU_ITEM_FIRST + i, 
                    oItem.m_eType == CAppMenuItem::emtClose ? _T("Exit") : strLabelW.c_str() );

                if (!oItem.m_isEnable)
                    popup.EnableMenuItem(0, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
            }
        }

	    RECT  rect; 
	    GetWindowRect(&rect);
	    rect.right = 1;
	    rect.bottom = m_menuBarHeight+1;

	    sub.Attach(menu.GetSubMenu(0));
	    sub.TrackPopupMenu( 
            TPM_LEFTALIGN, 
	        rect.right-1, 
	        rect.bottom-1,
	        m_hWnd);
	    sub.Detach();
	}
	else if(RHO_IS_WMDEVICE)
	{
		RHODESAPP().getAppMenu().copyMenuItems(m_arAppMenuItems);
		m_arAppMenuItems.push_back(rho::common::CAppMenuItem("aaa", "bbb"));
		HMENU hMenu = (HMENU)m_menuBar.SendMessage(SHCMBM_GETSUBMENU, 0, IDM_SK2_MENU);
		createCustomMenuEx( hMenu, m_arAppMenuItems );
	}
}

void CMainWindow::createCustomMenuEx(HMENU hMenu, rho::Vector<rho::common::CAppMenuItem>& arAppMenuItems)
{
	if(RHO_IS_CEDEVICE)
	{

	}else if(RHO_IS_WMDEVICE)
	{
	    //except exit item
	    int num = GetMenuItemCount (hMenu);
	    for (int i = 0; i < (num/* - 1*/); i++)	
		    DeleteMenu(hMenu, 0, MF_BYPOSITION);

	    //update UI with cusom menu items
	    USES_CONVERSION;
        for ( int i = arAppMenuItems.size() - 1; i >= 0; i--)
        {
            CAppMenuItem& oItem = arAppMenuItems.elementAt(i);
            StringW strLabelW = convertToStringW(oItem.m_strLabel);

            if (oItem.m_eType == CAppMenuItem::emtSeparator) 
                InsertMenu(hMenu, 0, MF_BYPOSITION | MF_SEPARATOR, 0, 0);
            else if (oItem.m_eType != CAppMenuItem::emtClose)
                InsertMenu(hMenu, 0, MF_BYPOSITION, ID_CUSTOM_MENU_ITEM_FIRST + i, strLabelW.c_str() );
            else
                InsertMenu(hMenu, 0, MF_BYPOSITION, ID_CUSTOM_MENU_ITEM_FIRST + i, L"Exit" );

            if (oItem.m_isEnable)
                EnableMenuItem(hMenu, 0, MF_BYPOSITION | MF_ENABLED);
            else
                EnableMenuItem(hMenu, 0, MF_BYPOSITION | MF_GRAYED);
        }
    }
}

LRESULT CMainWindow::OnCustomMenuItemCommand (WORD /*wNotifyCode*/, WORD  wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{	
    int nItemPos = wID-ID_CUSTOM_MENU_ITEM_FIRST;
    if ( nItemPos < 0 || nItemPos >= (int)m_arAppMenuItems.size() )
        return 0;

	CAppMenuItem& oMenuItem = m_arAppMenuItems.elementAt(nItemPos);
    if ( oMenuItem.m_strLink == "reload_rhobundle" )
    {
    #ifdef ENABLE_DYNAMIC_RHOBUNDLE
        if ( RHODESAPP().getRhobundleReloadUrl().length()>0 ) {
	        CAppManager::ReloadRhoBundle(m_hWnd,RHODESAPP().getRhobundleReloadUrl().c_str(), NULL);
        } else {
	        MessageBox(_T("Path to the bundle is not defined."),_T("Information"), MB_OK | MB_ICONINFORMATION );
        }
    #endif
        return 0;
    }

    oMenuItem.processCommand();

    return 0;
}

LRESULT CMainWindow::OnCustomToolbarItemCommand (WORD /*wNotifyCode*/, WORD  wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{	
    int nItemPos = wID-ID_CUSTOM_TOOLBAR_ITEM_FIRST;
    m_toolbar.processCommand(nItemPos);
    return 0;
}

extern "C" LRESULT rho_wmimpl_draw_splash_screen(HWND hWnd)
{
	//Commented this portion as we are not claiming support for Splash Screen Duration on WM/CE platform
	//May be we can claim in future if required. As of now it will not be supported.
  	//long lSplashScreenDuration = 0;  		
	//convertFromStringW(rho_wmimpl_getSplashScreenDuration(),lSplashScreenDuration);
	//CSplashScreen& splash = RHODESAPP().getSplashScreen();	
	//if(lSplashScreenDuration != 0 ){
	//	splash.setDuration(lSplashScreenDuration,TRUE);	
	//}
    CSplashScreen& splash = RHODESAPP().getSplashScreen();	
    splash.start();

    PAINTSTRUCT ps;
	HDC hDC = BeginPaint(hWnd, &ps);

    StringW pathW;

	LPCTSTR szSplashScreenPath = rho_wmimpl_getSplashScreenPathVal();
	if (szSplashScreenPath)
	{
		pathW = szSplashScreenPath;
		if (pathW.substr(0,7).compare(_T("file://"))==0)
		{
			pathW.erase(0,7);
		}
	}
	else
	{
		pathW = convertToStringW(RHODESAPP().getLoadingPngPath());
	}

	HBITMAP hbitmap = SHLoadImageFile(pathW.c_str());

	if(!hbitmap)
	{
		pathW.clear();
		DeleteObject(hbitmap);
		LOG(WARNING) + "Specified splashscreen image is not found or supported. Setting the default splashscreen image.";
		pathW = convertToStringW(RHODESAPP().getLoadingPngPath());
		hbitmap = SHLoadImageFile(pathW.c_str());
	}
		
	if (hbitmap)
    {
	    BITMAP bmp;
	    GetObject(hbitmap, sizeof(bmp), &bmp);

        RECT rcClient;
        GetClientRect(hWnd, &rcClient);

	    CDC hdcMem = CreateCompatibleDC(hDC);
        hdcMem.FillSolidRect(&rcClient, RGB(255,255,255));

	    HGDIOBJ resObj = SelectObject(hdcMem, hbitmap);

        int nLeft = rcClient.left, nTop=rcClient.top, nWidth = GetSystemMetrics(SM_CXSCREEN), nHeight=GetSystemMetrics(SM_CYSCREEN), Width = rcClient.right - rcClient.left, Height = rcClient.bottom - rcClient.top;
        if (splash.isFlag(CSplashScreen::HCENTER) )
		    nLeft = (Width-nWidth)/2;
	    if (splash.isFlag(CSplashScreen::VCENTER) )
		    nTop = (Height-nHeight)/2;
	    if (splash.isFlag(CSplashScreen::VZOOM) )
		    nHeight = Height;
	    if (splash.isFlag(CSplashScreen::HZOOM) )
		    nWidth = Width;

	    StretchBlt(hDC, nLeft, nTop, nWidth, nHeight,
		    hdcMem, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);
	    //BitBlt(hDC, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), hdcMem, 0, 0, SRCCOPY);
	    
	    //Commented this portion as we donot want to show the splash screen for specified duration
	    //In WM/CE device, with webkit engine, the splash screen remains for around 4 sec(minimum)
	    //depending on all the dll's which are required for the browser to work normally.
	    //Hence this portion of the code has been commented and not claiming support for splash screen duration.
	    //splash.hide();

        SelectObject(hdcMem, resObj);
	    DeleteObject(hbitmap);
	    //DeleteObject(hdcMem);
    }
    else //SPR 23830 - Fix
    {
        LOG(INFO) + "SHLoadImageFile() failed: missing loading.png";

        EndPaint(hWnd, &ps);
        return 0; // return failure code
    }

	EndPaint(hWnd, &ps);
	return 1;
}

LRESULT CMainWindow::OnTimer (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    LOG(INFO) + "OnTimer : " + wParam;
    bHandled = FALSE;

    if ( wParam == CNativeTabbar::TABBAR_TIMER_ID )
    {
        bHandled = TRUE;
        return m_oTabBar.OnTimer(uMsg, wParam, lParam);
    }

    return 0;
}

LRESULT CMainWindow::OnIntentMsg(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
	COPYDATASTRUCT* pcds = (COPYDATASTRUCT*)lParam;
    if (pcds->dwData == COPYDATA_INTERPROCESSMESSAGE){
        InterprocessMessage *ipmsg = reinterpret_cast<InterprocessMessage*>(pcds->lpData);
        LOG(INFO) + "INTERPROCESSMESSAGE : " + rho::String(ipmsg->appName) + rho::String(ipmsg->params);
        rho::Intent::addApplicationMessage(ipmsg->appName, ipmsg->params);
		LOG(INFO) + L"INTERPROCESSMESSAGE IS DONE.";
    }
    else if ( (LPCSTR)(pcds->lpData) && *(LPCSTR)(pcds->lpData)){
        m_oTabBar.SwitchTabByName((LPCSTR)(pcds->lpData), true);
    }
    return 0;
}

LRESULT CMainWindow::OnCopyData (UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
    COPYDATASTRUCT* pcds = (COPYDATASTRUCT*)lParam;

    if (pcds->dwData == COPYDATA_INTERPROCESSMESSAGE)
    {
        InterprocessMessage *ipmsg = reinterpret_cast<InterprocessMessage*>(pcds->lpData);
        LOG(INFO) + "INTERPROCESSMESSAGE : " + rho::String(ipmsg->appName) + rho::String(ipmsg->params);


        rho::Intent::addApplicationMessage(ipmsg->appName, ipmsg->params);
        return 0;
    }
    else if ( (LPCSTR)(pcds->lpData) && *(LPCSTR)(pcds->lpData))
    {
        m_oTabBar.SwitchTabByName((LPCSTR)(pcds->lpData), true);
    }

    return 0;
}

extern "C" const wchar_t* rho_wmimpl_get_window_nameW()
{
    return CMainWindow::GetWndClassInfo().m_wc.lpszClassName;
}
