// MainWindow.cpp: Defines main window for this application.

#include "stdafx.h"

#if defined(_WIN32_WCE)
#include <webvw.h>
#endif
#include <string>
#if defined(OS_WINDOWS)
#pragma warning(disable : 4995)
#include <strsafe.h>
#endif
#include "resource.h"
#include "MainWindow.h"
#include "common/RhodesApp.h"
#include "common/StringConverter.h"
#include "AppManager.h"
#include "ext/rho/rhoruby.h"
#if defined(_WIN32_WCE)
#include "camera/Camera.h"
#endif
#include "rho/net/NetRequest.h"
#include "sync/SyncThread.h"

IMPLEMENT_LOGCLASS(CMainWindow,"MainWindow");
//char* canonicalizeURL(const char* path);
//const char* strip_local_domain(const char* url);

//extern "C" wchar_t* wce_mbtowc(const char* a);
//extern "C" char* wce_wctomb(const wchar_t* w);

//extern "C" void pause_sync( int nPause );

#if defined(_WIN32_WCE)
#include <regext.h>

// Global Notification Handle
extern HREGNOTIFY g_hNotify;

#endif

extern "C" int g_rho_net_has_network;
using namespace rho::common;

#if !defined(_WIN32_WCE)
int CMainWindow::m_screenWidth;
int CMainWindow::m_screenHeight;
#endif

CMainWindow::CMainWindow()
{
	m_bLoading = true;
	m_bRhobundleReloadEnabled = true;
#if defined(_WIN32_WCE)
    memset(&m_sai, 0, sizeof(m_sai));
    m_sai.cbSize = sizeof(m_sai);
#endif
//	m_current_url = NULL;
//    m_szStartPage = NULL;
}

CMainWindow::~CMainWindow()
{
//    if ( m_current_url )
//	    free(m_current_url);

//    if ( m_szStartPage )
//        free(m_szStartPage);
}

void CMainWindow::Navigate2(BSTR URL) {
  m_spIWebBrowser2->Navigate2(&CComVariant(URL), NULL, &CComVariant(L"_self"), NULL, NULL);
}

void CMainWindow::Navigate(BSTR URL) {
  m_spIWebBrowser2->Navigate(URL, NULL, &CComVariant(L"_self"), NULL, NULL);
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
	RECT rcMainWindow = { 0,0,320,470 };

    // In one step, create an "AtlAxWin" window for the PIEWebBrowser control,
    // and also create the control itself. (AtlAxWin is a window class that
    // ATL uses to support containment of controls in windows.)
#if defined(_WIN32_WCE)
    m_browser.Create(m_hWnd,
                     CWindow::rcDefault, // proper sizing is done in CMainWindow::OnSize
					 TEXT("Microsoft.PIEDocView"), // ProgID of the control
                     WS_CHILD | WS_VISIBLE | WS_BORDER, 0,
                     ID_BROWSER);
#else
	LOGCONF().setLogView(&m_logView);

	rcMainWindow.left = getIniInt(_T("main_view_left"),0);
	rcMainWindow.top = getIniInt(_T("main_view_top"),0);
	int width = RHOCONF().getInt("client_area_width");
	if (width <= 0) width = 320;
	rcMainWindow.right = rcMainWindow.left+width;
	int height = RHOCONF().getInt("client_area_height");
	if (height <= 0) height = 470;
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
    m_menuBar = mbi.hwndMB; // save menu bar HWND

	// Compute RECT for initial size and position.
    // The following code should compute RECT appropriately
    // on both Pocket PC and Smartphone. It should function correctly
    // whether SIP is on or off, and
    // whether device is in portrait or landscape mode.
    // (rcMainWindow was initialized above)
    RHO_ASSERT(SystemParametersInfo(SPI_GETWORKAREA, 0, &rcMainWindow, 0));

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
#endif

#if !defined(_WIN32_WCE)
	m_screenWidth = rcMainWindow.right - rcMainWindow.left;
	m_screenHeight = rcMainWindow.bottom - rcMainWindow.top;
#endif

    MoveWindow(&rcMainWindow);

    RHO_ASSERT(SUCCEEDED(hr));
Error:
    return SUCCEEDED(hr) ? 0 : -1;
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
#if defined(OS_WINDOWS)
	USES_CONVERSION;
	LOG(TRACE) + "Seting browser client area size to: " + (int)LOWORD(lParam) + " x " + (int)(HIWORD(lParam)-m_menuBarHeight);
	m_browser.MoveWindow(0, 0, LOWORD(lParam), HIWORD(lParam)-m_menuBarHeight);
	if (m_menuBar.m_hWnd) {
		m_menuBar.MoveWindow(0, HIWORD(lParam)-m_menuBarHeight, LOWORD(lParam), m_menuBarHeight);
	}
#else
	m_browser.MoveWindow(0, 0, LOWORD(lParam), HIWORD(lParam));
#endif

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
#endif

    return 0;
}

LRESULT CMainWindow::OnSettingChange(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
#if defined(_WIN32_WCE)
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

LRESULT CMainWindow::OnBackCommand(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
//    if ( m_szStartPage && m_current_url && _stricmp(m_current_url,m_szStartPage) != 0 )
    rho::String strAppUrl = RHODESAPP().getAppBackUrl();

    if ( strAppUrl.length() > 0 )
    	m_spIWebBrowser2->Navigate( const_cast<wchar_t*>(convertToStringW(strAppUrl).c_str()), NULL, NULL, NULL, NULL);
    else if ( _stricmp(RHODESAPP().getCurrentUrl().c_str(),RHODESAPP().getStartUrl().c_str()) != 0 )
        m_spIWebBrowser2->GoBack();

    return 0;
}

LRESULT CMainWindow::OnForwardCommand(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    m_spIWebBrowser2->GoForward();
    return 0;
}

void CMainWindow::SetRhobundleReloadMenu() {
	if ( m_bRhobundleReloadEnabled )
	{
		m_bRhobundleReloadEnabled = false;

		#if defined(ENABLE_DYNAMIC_RHOBUNDLE) && defined(_WIN32_WCE)
		if ( !CHttpServer::Instance()->GetRhobundleReloadUrl() ) //disable RhoBundle reload url
		{
			HMENU hMenu = (HMENU)m_menuBar.SendMessage(SHCMBM_GETSUBMENU, 0, IDM_SK2_MENU);
			RemoveMenu(hMenu, IDM_RELOADRHOBUNDLE, MF_BYCOMMAND);
		}
		#elif defined(_WIN32_WCE)			
			HMENU hMenu = (HMENU)m_menuBar.SendMessage(SHCMBM_GETSUBMENU, 0, IDM_SK2_MENU);
			RemoveMenu(hMenu, IDM_RELOADRHOBUNDLE, MF_BYCOMMAND);
		#endif
	}
}

LRESULT CMainWindow::OnHomeCommand(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    //TODO: show menu on navigate to start page
	SetRhobundleReloadMenu();
	m_spIWebBrowser2->Navigate( 
        const_cast<wchar_t*>(convertToStringW(RHODESAPP().getStartUrl()).c_str()), NULL, NULL, NULL, NULL);
	return 0;
}
#if 0
LRESULT CMainWindow::OnLoadStartPageCommand(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	SetRhobundleReloadMenu();
	
	LPTSTR startPage = NULL;
    LPTSTR lastPageW = NULL;

	if ( RHOCONF().getBool("KeepTrackOfLastVisitedPage") ) 
    {
	    rho::String lastPage = RHOCONF().getString("LastVisitedPage");
	    if (lastPage.length() > 0) {
		    char* _page = canonicalizeURL(lastPage.c_str());
		    startPage = lastPageW = wce_mbtowc(_page);
		    free(_page);
        }
    }

	if ( !startPage )
		startPage = CHttpServer::Instance()->GetStartPage();

	m_spIWebBrowser2->Navigate(startPage, NULL, NULL, NULL, NULL);
	if (lastPageW) {
		free(lastPageW);
	}
	
	return 0;
}
#endif //0

LRESULT CMainWindow::OnOpenURLCommand(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    CGetURLDialog getURLDialog;
    int nRetCode = getURLDialog.DoModal(m_hWnd); // display the 'Get URL' dialog box
    if (IDOK == nRetCode)
    {
        m_spIWebBrowser2->Navigate(getURLDialog.GetURL(), NULL, NULL, NULL, NULL);
    }
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

LRESULT CMainWindow::OnRefreshCommand(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
//    m_spIWebBrowser2->Refresh();

//	LPTSTR wcurl = wce_mbtowc(GetCurrentLocation());

    rho::StringW strCurrentUrlW;
    rho::common::convertToStringW(RHODESAPP().getCurrentUrl().c_str(), strCurrentUrlW);
    Navigate2(const_cast<wchar_t*>(strCurrentUrlW.c_str()));

//	free(wcurl);
    return 0;
}

LRESULT CMainWindow::OnNavigateCommand(WORD /*wNotifyCode*/, WORD /*wID*/, HWND hWndCtl, BOOL& /*bHandled*/)
{
	LPTSTR wcurl = (LPTSTR)hWndCtl;
	if (wcurl) {
		Navigate2(wcurl);
		free(wcurl);
	}
    return 0;
}

LRESULT CMainWindow::OnStopCommand(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    m_spIWebBrowser2->Stop();
    return 0;
}


LRESULT CMainWindow::OnSyncCommand(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	rho_sync_doSyncAllSources(TRUE);
    return 0;
}

LRESULT CMainWindow::OnOptionsCommand(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	m_spIWebBrowser2->Navigate(const_cast<wchar_t*>(convertToStringW(RHODESAPP().getOptionsUrl()).c_str()), NULL, NULL, NULL, NULL);
	return 0;
}

LRESULT CMainWindow::OnReloadRhobundleCommand(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
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

#if defined(OS_WINDOWS)
LRESULT CMainWindow::OnPopupMenuCommand(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	CMenu menu;
	CMenu sub;
	RECT  rect;

	m_browser.GetWindowRect(&rect);

	VERIFY(menu.LoadMenu(IDR_MAIN_MENU));
	sub.Attach(menu.GetSubMenu(0));
    sub.TrackPopupMenu(
            TPM_RIGHTALIGN | TPM_BOTTOMALIGN | TPM_LEFTBUTTON | TPM_VERNEGANIMATION, 
			rect.right-1, 
			rect.bottom-1,
			m_hWnd);
	sub.Detach();

	return 0;
}

LRESULT CMainWindow::OnPosChanged(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled) {
	LPWINDOWPOS lp = (LPWINDOWPOS)lParam;
	setIniInt(_T("main_view_left"),lp->x);
	setIniInt(_T("main_view_top"),lp->y);
	bHandled = FALSE;
	return 0;
}
#endif
/*
void CMainWindow::SendCameraCallbackRequest(HRESULT status, LPTSTR image_name, char* callback_url) {

    rho::String callback = RHODESAPP().canonicalizeRhoUrl(callback_url);

	char* imageuri = NULL, *message;
	if (status==S_OK) {
		imageuri = wce_wctomb(image_name);
		int len = 256+strlen(imageuri);
		message = (char*) malloc(len);
		sprintf(message,"status=ok&image_uri=%s",imageuri);
	} else {
		char* status_message = (status==S_FALSE?"User canceled operation":"Error");
		int len = 256+strlen(status_message);
		message = (char*) malloc(len);
		sprintf(message,"status=%s&message=%s",
			(status==S_FALSE?"cancel":"error"),status_message);
	}

//	char* headers = "Content-Type: application/x-www-form-urlencoded\r\n";
	//char* res = m_callbackRequest.doRequest(L"POST",callback,headers,strlen(headers),message,strlen(message));
	//if ( res ) free(res);
    rho::net::CNetRequest oNetReq;
    oNetReq.pushData( callback, message, null );

	free(message);
	if (imageuri) 
        free(imageuri);
}*/

LRESULT CMainWindow::OnTakePicture(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/) {
#if defined (_WIN32_WCE)
	Camera camera;
	TCHAR image_uri[MAX_PATH];
	HRESULT status = camera.takePicture(this->m_hWnd,image_uri);

    RHODESAPP().callCameraCallback( (const char*)lParam, rho::common::convertToStringA(image_uri),
        (status!= S_OK && status != S_FALSE ? "Error" : ""), status == S_FALSE);
#endif
	return 0;
}

LRESULT CMainWindow::OnConnectionsNetworkCount(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
#if defined (_WIN32_WCE)

	g_rho_net_has_network = (int)wParam;

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
    wsprintf( image_uri, L"%s", L"dashboard.PNG");
#endif

    RHODESAPP().callCameraCallback( (const char*)lParam, rho::common::convertToStringA(image_uri),
        (status!= S_OK && status != S_FALSE ? "Error" : ""), status == S_FALSE);

	return 0;
}

// **************************************************************************
//
// event handlers
//
// **************************************************************************

void __stdcall CMainWindow::OnBeforeNavigate2(IDispatch* pDisp, VARIANT * pvtURL,
                                              VARIANT * /*pvtFlags*/, VARIANT * pvtTargetFrameName,
                                              VARIANT * /*pvtPostData*/, VARIANT * /*pvtHeaders*/,
                                              VARIANT_BOOL * /*pvbCancel*/)
{
    USES_CONVERSION;
    LPCTSTR szURL = OLE2CT(V_BSTR(pvtURL));

    LOG(TRACE) + "OnBeforeNavigate2: " + szURL ;

    SetWindowText(TEXT("Untitled"));

    RHO_ASSERT(SetEnabledState(IDM_STOP, TRUE));
}

void __stdcall CMainWindow::OnBrowserTitleChange(BSTR bstrTitleText)
{
    USES_CONVERSION;
    LOG(TRACE) + "OnBrowserTitleChange: " + OLE2CT(bstrTitleText);

    SetWindowText(OLE2CT(bstrTitleText));
}

void __stdcall CMainWindow::OnNavigateComplete2(IDispatch* pDisp, VARIANT * pvtURL)
{
    USES_CONVERSION;
    LOG(TRACE) + "OnNavigateComplete2: " + OLE2CT(V_BSTR(pvtURL));
}

#if defined(_WIN32_WCE)
std::wstring& loadLoadingHtml(std::wstring& str) {
	FILE *file;
	wchar_t	buf[1024];

    rho::String fname = RHODESAPP().getLoadingPagePath(); 
	file = fopen(fname.c_str(), "r");

	if(file==NULL) {
		str.append(L"<html><head><title>Loading...</title></head><body><h1>Loading...</h1></body></html>");
	} else {
		while(fgetws(buf, sizeof(buf), file) != NULL) {
			str.append(buf);
		}
		fclose(file);
	}
	return str;
}

void writeToTheDoc(IPIEHTMLDocument2 *document) {
	HRESULT hresult = S_OK;
	VARIANT *param;
	SAFEARRAY *sfArray;
	std::wstring html;
	BSTR bstr = SysAllocString(loadLoadingHtml(html).c_str());

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
		IPIEHTMLDocument2* pDoc;
		hr = pHtmlDoc->QueryInterface(__uuidof(IPIEHTMLDocument2),  (void**)&pDoc );
        if ( SUCCEEDED(hr) )
        {
			// Write to the document
			writeToTheDoc(pDoc);
			pDoc->Release();
        }
    }
}
#endif //_WIN32_WCE

void __stdcall CMainWindow::OnDocumentComplete(IDispatch* pDisp, VARIANT * pvtURL)
{
    USES_CONVERSION;
	
	//BOOL store_current_url = !m_bLoading;
	LPCTSTR url = OLE2CT(V_BSTR(pvtURL));
	if (m_bLoading && wcscmp(url,_T("about:blank"))==0) {
		LOG(TRACE) + "Show loading page";
#if defined(_WIN32_WCE)
		ShowLoadingPage(pDisp, pvtURL);
#endif //_WIN32_WCE
		m_bLoading = false; //show loading page only once
    }else
    {
//        if ( m_current_url && strcmp(m_current_url,"about:blank") ==0 )
//            m_szStartPage = wce_wctomb(url);
    }

	/*if (m_current_url) {
		free(m_current_url);
	}

	m_current_url = wce_wctomb(url);*/
	
	//if( store_current_url ) 
    //    RHODESAPP().keepLastVisitedUrlW(url);

    LOG(TRACE) + "OnDocumentComplete: " + url;

    RHO_ASSERT(SetEnabledState(IDM_STOP, FALSE));
}

void __stdcall CMainWindow::OnCommandStateChange(long lCommand, BOOL bEnable)
{
    /*if (CSC_NAVIGATEBACK == lCommand)
    {
        RHO_ASSERT(SetEnabledState(IDM_BACK, bEnable));
    }
    else */if (CSC_NAVIGATEFORWARD == lCommand)
    {
        RHO_ASSERT(SetEnabledState(IDM_FORWARD, bEnable));
    }
}

// **************************************************************************
//
// utility function
//
// **************************************************************************
BOOL CMainWindow::SetEnabledState(UINT uMenuItemID, BOOL bEnable)
{
#if defined(_WIN32_WCE)
    HMENU hMenu = (HMENU)m_menuBar.SendMessage(SHCMBM_GETSUBMENU, 0, IDM_SK2_MENU);
    BOOL bRetVal = EnableMenuItem(hMenu, uMenuItemID, bEnable ? MF_ENABLED : MF_GRAYED);
    return (bRetVal != 0xFFFFFFFF);
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
                    PostMessageW(WM_COMMAND,MAKEWPARAM(IDM_BACK,1),NULL);

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
