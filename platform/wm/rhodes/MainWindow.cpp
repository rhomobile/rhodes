// MainWindow.cpp: Defines main window for this application.

#include "stdafx.h"

#if defined(_WIN32_WCE)
#include <webvw.h>
//#include <soundfile.h>
//#include <nled.h>
#endif
#include <string>
#if defined(OS_WINDOWS)
#pragma warning(disable : 4995)
#include <strsafe.h>
#endif
#include "resource.h"
#include "MainWindow.h"
#include "common/StringConverter.h"
#include "AppManager.h"
#include "ext/rho/rhoruby.h"
#include "rubyext/WebView.h"
#if defined(_WIN32_WCE)
#include "camera/Camera.h"
#endif
#include "rho/net/NetRequest.h"
#include "sync/SyncThread.h"
#include "common/RhoFilePath.h"
#include "common/RhoFile.h"
#include "bluetooth/Bluetooth.h"
#include "MetaHandler.h"
#include <hash_map>

IMPLEMENT_LOGCLASS(CMainWindow,"MainWindow");

#if defined(_WIN32_WCE)
#include <regext.h>

// Global Notification Handle
extern HREGNOTIFY g_hNotify;

#endif

#include "DateTimePicker.h"

extern "C" void rho_sysimpl_sethas_network(int nValue);
extern "C" void rho_geoimpl_turngpsoff();

using namespace rho::common;
using namespace rho;
using namespace stdext;

#if !defined(_WIN32_WCE)
int CMainWindow::m_screenWidth;
int CMainWindow::m_screenHeight;
#endif

CMainWindow::CMainWindow()
{
	mIsBrowserViewHided = false;
	mNativeView = NULL;
	mNativeViewFactory = NULL;
	mNativeViewType = "";

	m_bLoading = true;
#if defined(_WIN32_WCE)
    memset(&m_sai, 0, sizeof(m_sai));
    m_sai.cbSize = sizeof(m_sai);
#endif
	m_pageCounter = 0;
}

CMainWindow::~CMainWindow()
{
//    if ( m_current_url )
//	    free(m_current_url);

//    if ( m_szStartPage )
//        free(m_szStartPage);
}

void CMainWindow::Navigate2(BSTR URL) {
	String cleared_url = processForNativeView(convertToStringA(OLE2CT(URL)));
	if (!cleared_url.empty()) {
		StringW cw = convertToStringW(cleared_url);
		BSTR cleared_url_bstr = SysAllocString(cw.c_str());
		m_spIWebBrowser2->Navigate2(&CComVariant(cleared_url_bstr), NULL, &CComVariant(L"_self"), NULL, NULL);
	}
}

void CMainWindow::Navigate(BSTR URL) {
	String cleared_url = processForNativeView(convertToStringA(OLE2CT(URL)));
	if (!cleared_url.empty()) {
		StringW cw = convertToStringW(cleared_url);
		BSTR cleared_url_bstr = SysAllocString(cw.c_str());
		m_spIWebBrowser2->Navigate(cleared_url_bstr, NULL, &CComVariant(L"_self"), NULL, NULL);
	}
}

// **************************************************************************
//
// WM_xxx handlers
//
// **************************************************************************

LRESULT CMainWindow::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    HRESULT hr = S_OK;
#if defined(_WIN32_WCE)
    SHMENUBARINFO mbi = { sizeof(mbi), 0 };
    SIPINFO si = { sizeof(si), 0 };
    RECT rcMenuBar = { 0 };
#else
	NONCLIENTMETRICS ncm = { sizeof(NONCLIENTMETRICS) };
	int nSpiBorder = 0;
#endif
    int xScreenSize = GetSystemMetrics(SM_CXSCREEN);
    int yScreenSize = GetSystemMetrics(SM_CYSCREEN);

    LOG(INFO)  + "Screen size: x=" + xScreenSize + ";y=" + yScreenSize;

	RECT rcMainWindow = { 0,0,320,470 };

    // In one step, create an "AtlAxWin" window for the PIEWebBrowser control,
    // and also create the control itself. (AtlAxWin is a window class that
    // ATL uses to support containment of controls in windows.)
#if defined(_WIN32_WCE)
    m_browser.Create(m_hWnd,
                     CWindow::rcDefault, // proper sizing is done in CMainWindow::OnSize
					 TEXT("Microsoft.PIEDocView"), // ProgID of the control
                     WS_CHILD | WS_BORDER, 0,
                     ID_BROWSER);
#else
	LOGCONF().setLogView(&m_logView);

	rcMainWindow.left = getIniInt(_T("main_view_left"),0);
	rcMainWindow.top = getIniInt(_T("main_view_top"),0);
    if ( rcMainWindow.left < 0 || rcMainWindow.left > xScreenSize )
        rcMainWindow.left = 0;
    if ( rcMainWindow.top < 0 || rcMainWindow.top > yScreenSize )
        rcMainWindow.top = 0;

	int width = RHOCONF().getInt("client_area_width");
    if (width <= 0) 
        width = rcMainWindow.right;
	rcMainWindow.right = rcMainWindow.left+width;
	int height = RHOCONF().getInt("client_area_height");
	if (height <= 0) 
        height = rcMainWindow.bottom;
	rcMainWindow.bottom = rcMainWindow.top+height;

	m_browser.Create(m_hWnd,
                     CWindow::rcDefault, // proper sizing is done in CMainWindow::OnSize
					 TEXT("Shell.Explorer"), // ProgID of the control
                     WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0,
                     ID_BROWSER);
	m_menuBar.Create(m_hWnd,CWindow::rcDefault);

	SystemParametersInfo ( SPI_GETNONCLIENTMETRICS, 0, &ncm, false );
	m_menuBarHeight = ncm.iMenuHeight+ncm.iBorderWidth*4+2;
	rcMainWindow.bottom += ncm.iCaptionHeight+ncm.iBorderWidth*8+m_menuBarHeight;
	rcMainWindow.right += ncm.iScrollWidth;
	rcMainWindow.right += ncm.iBorderWidth*6;
#endif

    CBR(m_browser.m_hWnd != NULL);

    // cache IWebBrowser2 interface pointer
    hr = m_browser.QueryControl(&m_spIWebBrowser2);
    CHR(hr);

    // set up connection point
    hr = AtlAdviseSinkMap(this, true);
    CHR(hr);

    // set initial properties for the control
    //m_spIWebBrowser2->put_AddressBar(VARIANT_TRUE);
    m_spIWebBrowser2->put_AddressBar(VARIANT_FALSE);

    if ( !RHOCONF().getBool("wm_show_statusbar") )
        m_spIWebBrowser2->put_StatusBar(VARIANT_FALSE);

    //m_spIWebBrowser2->put_Offline(VARIANT_TRUE);
#if defined(_WIN32_WCE)
    // Create a menubar
    // (mbi was initialized above)
    mbi.hwndParent = m_hWnd;
    mbi.nToolBarId = IDR_MAIN_MENUBAR; // ID of toolbar resource
    mbi.hInstRes   = _AtlBaseModule.GetResourceInstance();
    CBR(SHCreateMenuBar(&mbi));
	m_hWndCECommandBar = mbi.hwndMB;
	m_menuBar = m_hWndCECommandBar;

	// Compute RECT for initial size and position.
    // The following code should compute RECT appropriately
    // on both Pocket PC and Smartphone. It should function correctly
    // whether SIP is on or off, and
    // whether device is in portrait or landscape mode.
    // (rcMainWindow was initialized above)
    RHO_ASSERT(SystemParametersInfo(SPI_GETWORKAREA, 0, &rcMainWindow, 0));

    LOG(INFO)  + "SPI_GETWORKAREA: x=" + rcMainWindow.right + ";y=" + rcMainWindow.bottom;

    // (rcMenuBar was initialized above)
    m_menuBar.GetWindowRect(&rcMenuBar);
    rcMainWindow.bottom = rcMenuBar.top;

    // SIP state
    // (si was initialized above)
    if (SHSipInfo(SPI_GETSIPINFO, 0, &si, 0) &&
        (si.fdwFlags & SIPF_ON) && (si.fdwFlags & SIPF_DOCKED))
    {
        rcMainWindow.bottom = si.rcVisibleDesktop.bottom;
    }
	
	SetToolbarButtonEnabled(IDM_SK1_EXIT, FALSE);
#endif

#if !defined(_WIN32_WCE)
	m_screenWidth = rcMainWindow.right - rcMainWindow.left;
	m_screenHeight = rcMainWindow.bottom - rcMainWindow.top;
#endif

    MoveWindow(&rcMainWindow);

#if defined(_WIN32_WCE)
	//Set fullscreen after window resizing
	if ( RHOCONF().getBool("full_screen"))
   	    SetFullScreen(true);
#endif

	RHO_ASSERT(SUCCEEDED(hr));
Error:

    return SUCCEEDED(hr) ? 0 : -1;
}

LRESULT CMainWindow::OnSetText(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
    return TRUE;
}

HWND CMainWindow::getWebViewHWND() {
	return m_browser.m_hWnd;
}

void CMainWindow::hideWebView() {
	//::ShowWindow(m_browser.m_hWnd, SW_HIDE);
	m_browser.ShowWindow(SW_HIDE);
	mIsBrowserViewHided = true;
	//m_browser.DestroyWindow();
}

void CMainWindow::showWebView() {
	//::ShowWindow(m_browser.m_hWnd, SW_SHOW);
	m_browser.ShowWindow(SW_SHOW);
	mIsBrowserViewHided = false;
}

LRESULT CMainWindow::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
#if defined (_WIN32_WCE)
    m_menuBar = NULL;
#endif

#if defined(OS_WINDOWS)
	if(m_logView.IsWindow()) {
		m_logView.DestroyWindow();
	}
	LOGCONF().setLogView(NULL);
#endif

    // Tear down connection points while controls are still alive.
    RHO_ASSERT(SUCCEEDED(AtlAdviseSinkMap(this, false)));

    m_spIWebBrowser2 = NULL;
    m_browser = NULL;

    PostQuitMessage(0);

    bHandled = FALSE; // Allow ATL's default processing (e.g. NULLing m_hWnd)
    return 0;
}

LRESULT CMainWindow::OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
    if ( !m_browser.m_hWnd )
        return 0;

#if defined(OS_WINDOWS)
	USES_CONVERSION;
	LOG(TRACE) + "Seting browser client area size to: " + (int)LOWORD(lParam) + " x " + (int)(HIWORD(lParam)-m_menuBarHeight);
	m_browser.MoveWindow(0, 0, LOWORD(lParam), HIWORD(lParam)-m_menuBarHeight);
	if (m_menuBar.m_hWnd) {
		m_menuBar.MoveWindow(0, HIWORD(lParam)-m_menuBarHeight, LOWORD(lParam), m_menuBarHeight);
	}
#else

    LOG(INFO)  + "OnSize: x=" + (int)(LOWORD(lParam)) + ";y=" + (int)(HIWORD(lParam));

	m_browser.MoveWindow(0, 0, LOWORD(lParam), HIWORD(lParam));
#endif

	return 0;
}

LRESULT CMainWindow::OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	PAINTSTRUCT ps;
	HDC hDC = BeginPaint(&ps);

  	CSplashScreen& splash = RHODESAPP().getSplashScreen();
    splash.start();
#ifdef _WIN32_WCE	
    StringW pathW = convertToStringW(RHODESAPP().getLoadingPngPath());

	StringW path_wm_W = pathW.substr(0, pathW.length() - 3);
	String wm_png = "wm.png";
	path_wm_W.append(convertToStringW(wm_png));

    HBITMAP hbitmap = SHLoadImageFile(path_wm_W.c_str());
	if (!hbitmap) {
		hbitmap = SHLoadImageFile(pathW.c_str());
	}
		
	if (!hbitmap)
		return 0;

	BITMAP bmp;
	GetObject(hbitmap, sizeof(bmp), &bmp);

	HDC hdcMem = CreateCompatibleDC(hDC);
	SelectObject(hdcMem, hbitmap);

    CRect rcClient;
    GetClientRect(&rcClient);
    int nLeft = rcClient.left, nTop=rcClient.top, nWidth = bmp.bmWidth, nHeight=bmp.bmHeight;
    if (splash.isFlag(CSplashScreen::HCENTER) )
        nLeft = (rcClient.Width()-nWidth)/2;
	if (splash.isFlag(CSplashScreen::VCENTER) )
        nTop = (rcClient.Height()-nHeight)/2;
	if (splash.isFlag(CSplashScreen::VZOOM) )
		nHeight = rcClient.Height();
	if (splash.isFlag(CSplashScreen::HZOOM) )
		nWidth = rcClient.Width();

	StretchBlt(hDC, nLeft, nTop, nWidth, nHeight,
		hdcMem, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);
	//BitBlt(hDC, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), hdcMem, 0, 0, SRCCOPY);

	DeleteObject(hbitmap);
#endif //_WIN32_WCE

	EndPaint(&ps);
	return 0;
}

LRESULT CMainWindow::OnActivate(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
    int fActive = LOWORD(wParam);
#if defined(_WIN32_WCE)
    // Notify shell of our WM_ACTIVATE message
    SHHandleWMActivate(m_hWnd, wParam, lParam, &m_sai, 0);

    //pause_sync(!fActive);
/* TBD: Commented this out because it was called before http server started 
        We need to fix this properly
    if ( fActive )
        CHttpServer::Instance()->ResumeThread();
    else
        CHttpServer::Instance()->FreezeThread();
*/
    //activate calls after javascript alerts, so we have viciouse cycle if alert is display in home page
    //if ( rho::common::CRhodesApp::getInstance() != null )
    //    RHODESAPP().callAppActiveCallback(fActive!=0);

#endif

    if (!fActive)
        rho_geoimpl_turngpsoff();

    return 0;
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
				mNativeView = nv;
				mNativeViewFactory = nvf;
				mNativeViewType = protocol;

				HWND nvh = (HWND)mNativeView->getView();


				

				::SetParent(nvh, m_hWnd);

				RECT rect;
				::GetWindowRect(getWebViewHWND(),&rect);

				int x = 0;
				int y = 0;
				int w = rect.right - rect.left;
				int h = rect.bottom - rect.top;

				::SetWindowPos(nvh, HWND_TOP, x, y, w, h, SWP_SHOWWINDOW);

				nv->navigate(navto.c_str());
				
				::ShowWindow(nvh, SW_SHOWNORMAL);
				hideWebView();

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
	restoreWebView();
	return url;
}


void CMainWindow::restoreWebView() {
	if (mNativeView != NULL) {
		mNativeViewFactory->destroyNativeView(mNativeView);
		mNativeView = NULL;
		mNativeViewFactory = NULL;
		mNativeViewType = "";
		showWebView();
	}
}


LRESULT CMainWindow::OnSettingChange(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
#if defined(_WIN32_WCE)
	
	//handle sreen rotation
	int width  = GetSystemMetrics(SM_CXSCREEN);	
	int height = GetSystemMetrics(SM_CYSCREEN);
	
	if (wParam == SETTINGCHANGE_RESET) {
		rho_rhodesapp_callScreenRotationCallback(width, height, 90);
	}
	
	// Notify shell of our WM_SETTINGCHANGE message
	SHHandleWMSettingChange(m_hWnd, wParam, lParam, &m_sai);
#endif
    return 0;
}

// **************************************************************************
//
// WM_COMMAND handlers
//
// **************************************************************************

LRESULT CMainWindow::OnExitCommand(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
#if defined(_WIN32_WCE)
	if ( g_hNotify )
		RegistryCloseNotification(g_hNotify);
#endif

    SendMessage(WM_CLOSE);
    return 0;
}

LRESULT CMainWindow::OnNavigateBackCommand(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	restoreWebView();
    m_spIWebBrowser2->GoBack();
    return 0;
}

LRESULT CMainWindow::OnBackCommand(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    RHODESAPP().navigateBack();
    return 0;
}

LRESULT CMainWindow::OnLogCommand(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
#if defined(OS_WINDOWS)
	if ( !m_logView.IsWindow() ) {
		LoadLibrary(_T("riched20.dll"));
		m_logView.Create(NULL);
	}
	m_logView.ShowWindow(SW_SHOWNORMAL);
#else
	CLogView oLogView;
    oLogView.DoModal(m_hWnd);
#endif

    return 0;
}

LRESULT CMainWindow::OnFullscreenCommand (WORD /*wNotifyCode*/, WORD /*wID*/, HWND hwnd, BOOL& /*bHandled*/)
{
#if defined (_WIN32_WCE) 
	SetFullScreen(!m_bFullScreen);
#endif
	return 0;
};

LRESULT CMainWindow::OnRefreshCommand(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{

    if (m_spIWebBrowser2)
        m_spIWebBrowser2->Refresh();
    return 0;
}

LRESULT CMainWindow::OnNavigateCommand(WORD /*wNotifyCode*/, WORD /*wID*/, HWND hWndCtl, BOOL& /*bHandled*/)
{
	LPTSTR wcurl = (LPTSTR)hWndCtl;
	if (wcurl) 
    {
		Navigate2(wcurl);
		free(wcurl);
	}
    return 0;
}

#if defined(OS_WINDOWS)
LRESULT CMainWindow::OnPopupMenuCommand(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) 
{
	createCustomMenu();
	return 0;
}

LRESULT CMainWindow::OnPosChanged(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled) 
{
	LPWINDOWPOS lp = (LPWINDOWPOS)lParam;
	setIniInt(_T("main_view_left"),lp->x);
	setIniInt(_T("main_view_top"),lp->y);
	bHandled = FALSE;
	return 0;
}
#endif

LRESULT CMainWindow::OnTakePicture(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/) 
{
	TCHAR image_uri[MAX_PATH];
    HRESULT status;
#if defined (_WIN32_WCE)
	Camera camera;
	status = camera.takePicture(this->m_hWnd,image_uri);
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
#if defined (_WIN32_WCE)

	rho_sysimpl_sethas_network( (int)wParam );

#endif
	return 0;
}

LRESULT CMainWindow::OnSelectPicture(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/) 
{
	TCHAR image_uri[MAX_PATH];
    HRESULT status = S_OK;
#if defined (_WIN32_WCE)
	Camera camera;
	status = camera.selectPicture(this->m_hWnd,image_uri);
#else
    //TODO: show browse file dialog
    wsprintf( image_uri, L"%s", L"dashboard.PNG");
#endif

    RHODESAPP().callCameraCallback( (const char*)lParam, rho::common::convertToStringA(image_uri),
        (status!= S_OK && status != S_FALSE ? "Error" : ""), status == S_FALSE);
    
    free ((void *)lParam);
    
	return 0;
}

/*static*/ StringW CMainWindow::getRhodesAppName()
{
    String path = rho_native_rhopath();
    int last, pre_last;

    last = path.find_last_of('\\');
    pre_last = path.substr(0, last).find_last_of('\\');
    return convertToStringW( path.substr(pre_last + 1, last - pre_last - 1) );
}

LRESULT CMainWindow::OnAlertShowPopup (UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
	LOG(INFO) + __FUNCTION__;
    StringW strAppName = getRhodesAppName();
	CAlertDialog::Params *params = (CAlertDialog::Params *)lParam;

	if (params->m_dlgType == CAlertDialog::Params::DLG_DEFAULT) {
		MessageBox(convertToStringW(params->m_message).c_str(), strAppName.c_str(), MB_ICONWARNING | MB_OK);
	} else if (params->m_dlgType == CAlertDialog::Params::DLG_CUSTOM) 
    {
        if ( params->m_buttons.size() == 1 && strcasecmp(params->m_buttons[0].m_strCaption.c_str(), "ok") == 0)
            MessageBox(convertToStringW(params->m_message).c_str(), convertToStringW(params->m_title).c_str(), MB_ICONWARNING | MB_OK);
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
	if (m_alertDialog != NULL) {
		m_alertDialog->EndDialog(0);
		m_alertDialog = NULL;
	}

	return 0;
}

LRESULT CMainWindow::OnBluetoothDiscover (UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/) {
#ifndef OS_WINDOWS
	RhoDiscoverDlg* dlg = RhoBluetoothManager::getInstance()->getDiscoverDlg();
	dlg->openDialog(RhoBluetoothManager::getInstance());
#endif // OS_WINDOWS
	return 0;
}

LRESULT CMainWindow::OnBluetoothDiscovered (UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/) {
#ifndef OS_WINDOWS
	RhoDiscoveredDlg* dlg = RhoBluetoothManager::getInstance()->getDiscoveredDlg();
	dlg->openDialog(RhoBluetoothManager::getInstance());
#endif // OS_WINDOWS
	return 0;
}

LRESULT CMainWindow::OnExecuteCommand(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/) {
	RhoNativeViewRunnable* command = (RhoNativeViewRunnable*)wParam;
	if (command != NULL) {
		command->run();
	}
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

void __stdcall CMainWindow::OnBeforeNavigate2(IDispatch* pDisp, VARIANT * pvtURL,
                                              VARIANT * /*pvtFlags*/, VARIANT * pvtTargetFrameName,
                                              VARIANT * /*pvtPostData*/, VARIANT * /*pvtHeaders*/,
                                              VARIANT_BOOL * pvbCancel)
{
    USES_CONVERSION;
    LPCTSTR szURL = OLE2CT(V_BSTR(pvtURL));

    LOG(TRACE) + "OnBeforeNavigate2: " + szURL;

    if ( wcsstr(szURL, L"rho_open_target=_blank") != 0)
    {
        LOG(INFO) + "Open external browser: " + szURL;
#ifdef OS_WINCE
        rho_wmsys_run_app(convertToStringA(szURL).c_str(), 0 );
#else
        rho_wmsys_run_app(convertToStringA(szURL).c_str(), 0 );
#endif
        *pvbCancel = VARIANT_TRUE;
    }

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
    LOG(TRACE) + "OnBrowserTitleChange: " + OLE2CT(bstrTitleText);

    String strTitle = RHOCONF().getString("title_text");
    if ( strTitle.length() > 0 )
        SetWindowText(convertToStringW(strTitle).c_str());
    else
    {
        LPCTSTR szTitle = OLE2CT(bstrTitleText);
        if ( szTitle && 
            (_tcsncmp(szTitle, _T("http:"), 5) == 0 || _tcscmp(szTitle, _T("about:blank"))==0 ))
            return;

        SetWindowText(szTitle);
    }
}

void __stdcall CMainWindow::OnNavigateComplete2(IDispatch* pDisp, VARIANT * pvtURL)
{
    USES_CONVERSION;
	if (!m_bLoading)
		if (!mIsBrowserViewHided) 
			m_browser.ShowWindow(SW_SHOW);
    LOG(TRACE) + "OnNavigateComplete2: " + OLE2CT(V_BSTR(pvtURL));
}

BSTR loadLoadingHtml()
{
	rho::String fname = RHODESAPP().getLoadingPagePath();

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
		strTextW = L"<html><head><title>Loading...</title></head><body><h1>Loading...</h1></body></html>";
    }

    return SysAllocString(strTextW.c_str());
}

void writeToTheDoc (
#if defined(_WIN32_WCE)
					IPIEHTMLDocument2 *document
#else
					IHTMLDocument2 *document
#endif
					)
{
	HRESULT hresult = S_OK;
	VARIANT *param;
	SAFEARRAY *sfArray;
	//std::wstring html;
	BSTR bstr = loadLoadingHtml();

	// Creates a new one-dimensional array
	sfArray = SafeArrayCreateVector(VT_VARIANT, 0, 1);

	if (sfArray == NULL || document == NULL) {
		goto cleanup;
	}

	hresult = SafeArrayAccessData(sfArray,(LPVOID*) & param);
	param->vt = VT_BSTR;
	param->bstrVal = bstr;
	hresult = SafeArrayUnaccessData(sfArray);
	hresult = document->write(sfArray);

cleanup:
	SysFreeString(bstr);
	if (sfArray != NULL) {
		SafeArrayDestroy(sfArray);
	}
}

void CMainWindow::ShowLoadingPage(LPDISPATCH pDisp, VARIANT* URL)
{
	HRESULT hr;
	IDispatch* pHtmlDoc = NULL;

    // Retrieve the document object.
    hr = m_spIWebBrowser2->get_Document( &pHtmlDoc );
    if ( SUCCEEDED(hr) )
    {
#if defined(_WIN32_WCE)
		IPIEHTMLDocument2* pDoc;
		hr = pHtmlDoc->QueryInterface(__uuidof(IPIEHTMLDocument2),  (void**)&pDoc );
#else
		IHTMLDocument2* pDoc;
		hr = pHtmlDoc->QueryInterface(__uuidof(IHTMLDocument2),  (void**)&pDoc );
#endif
        if ( SUCCEEDED(hr) )
        {
			// Write to the document
			writeToTheDoc(pDoc);
			pDoc->Release();
        }
    }
}

void __stdcall CMainWindow::OnDocumentComplete(IDispatch* pDisp, VARIANT * pvtURL)
{
    USES_CONVERSION;
	
	LPCTSTR url = OLE2CT(V_BSTR(pvtURL));
	if (m_bLoading && wcscmp(url,_T("about:blank"))==0) {
		LOG(TRACE) + "Show loading page";
		ShowLoadingPage(pDisp, pvtURL);
		m_bLoading = false; //show loading page only once
    }else
    {
    }
    m_bLoading = false;

    LOG(TRACE) + "OnDocumentComplete: " + url;

#if defined (_WIN32_WCE)
	createCustomMenu();
	
	m_pageCounter++;
	if (m_pageCounter > 2) //"loading" page + first page
		SetToolbarButtonEnabled(IDM_SK1_EXIT, TRUE);
#endif	
    
    //CMetaHandler oHandler(m_spIWebBrowser2);
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
#ifdef OS_WINCE
	if (pMsg->message == WM_CONTEXTMENU){
		/*
		CMenuHandle menu;
		menu.LoadMenu(IDR_MAIN_MENU);
		menu = menu.GetSubMenu(0);
		return menu.TrackPopupMenu( TPM_CENTERALIGN | TPM_VERTICAL, LOWORD(pMsg->lParam), HIWORD(pMsg->lParam), m_hWnd);
		*/
		
		return TRUE;
	}

	if (m_bFullScreen && pMsg->message == WM_KEYUP && 
		(pMsg->wParam == VK_F1 ||  pMsg->wParam == VK_F2))
	        SetFullScreen(false);
#endif

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

    // Find a direct child of this window from the window that has focus.
    // This will be AtlAxWin window for the hosted control.
    CWindow control = ::GetFocus();
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

#if defined (OS_WINDOWS)
void CMainWindow::createCustomMenu()
{
	CMenu menu;
	CMenu sub;
	CMenu popup;
	
    if (!m_browser.m_hWnd)
        return;

	VERIFY(menu.CreateMenu());
	VERIFY(popup.CreatePopupMenu());
	menu.AppendMenu(MF_POPUP, (UINT) popup.m_hMenu, _T(""));

	RHODESAPP().getAppMenu().copyMenuItems(m_arAppMenuItems);

#ifdef ENABLE_DYNAMIC_RHOBUNDLE
    String strIndexPage = CFilePath::join(RHODESAPP().getStartUrl(),"index_erb.iseq");
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
			popup.InsertMenu(0, MF_BYPOSITION, ID_CUSTOM_MENU_ITEM_FIRST + i, 
                oItem.m_eType == CAppMenuItem::emtClose ? _T("Exit") : A2T(oItem.m_strLabel.c_str()) );
    }

	RECT  rect; 
	m_browser.GetWindowRect(&rect);
	sub.Attach(menu.GetSubMenu(0));
	sub.TrackPopupMenu( TPM_RIGHTALIGN | TPM_BOTTOMALIGN | TPM_LEFTBUTTON | TPM_VERNEGANIMATION, 
						rect.right-1, 
						rect.bottom-1,
						m_hWnd);
	sub.Detach();
}

#endif//OS_WINDOWS

#if defined (OS_WINCE)

void CMainWindow::createCustomMenu()
{
	HMENU hMenu = (HMENU)m_menuBar.SendMessage(SHCMBM_GETSUBMENU, 0, IDM_SK2_MENU);
	
	//except exit item
	int num = GetMenuItemCount (hMenu);
	for (int i = 0; i < (num - 1); i++)	
		DeleteMenu(hMenu, 0, MF_BYPOSITION);

	RHODESAPP().getAppMenu().copyMenuItems(m_arAppMenuItems);
 	
	//update UI with cusom menu items
	USES_CONVERSION;
    for ( int i = m_arAppMenuItems.size() - 1; i >= 0; i--)
    {
        CAppMenuItem& oItem = m_arAppMenuItems.elementAt(i);
		if (oItem.m_eType == CAppMenuItem::emtSeparator) 
			InsertMenu(hMenu, 0, MF_BYPOSITION | MF_SEPARATOR, 0, 0);
		else if (oItem.m_eType != CAppMenuItem::emtExit && oItem.m_eType != CAppMenuItem::emtClose)
    		InsertMenu(hMenu, 0, MF_BYPOSITION, ID_CUSTOM_MENU_ITEM_FIRST + i, A2T(oItem.m_strLabel.c_str()));
	}
}
#endif //OS_WINCE

LRESULT CMainWindow::OnCustomMenuItemCommand (WORD /*wNotifyCode*/, WORD  wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{	
    int nItemPos = wID-ID_CUSTOM_MENU_ITEM_FIRST;
    if ( nItemPos < 0 || nItemPos >= (int)m_arAppMenuItems.size() )
        return 0;

	CAppMenuItem& oMenuItem = m_arAppMenuItems.elementAt(nItemPos);
    if ( oMenuItem.m_eType == CAppMenuItem::emtUrl )
    {
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
    }

    oMenuItem.processCommand();

    return 0;
}
