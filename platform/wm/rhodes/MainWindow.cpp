// MainWindow.cpp: Defines main window for this application.

#include "stdafx.h"
#if defined(_WIN32_WCE)
#include <webvw.h>
#endif
#include <string>
#if !defined(_WIN32_WCE)
#pragma warning(disable : 4995)
#include <strsafe.h>
#endif
#include "resource.h"
#include "MainWindow.h"
#include "HttpServer.h"
#include "AppManager.h"
#include "rhoruby/rhoruby.h"
#if defined(_WIN32_WCE)
#include "camera/Camera.h"
#endif
char* canonicalizeURL(char* path);

extern "C" wchar_t* wce_mbtowc(const char* a);
extern "C" char* wce_wctomb(const wchar_t* w);

extern "C" void pause_sync( int nPause );

CMainWindow::CMainWindow()
{
	m_bLoading = true;
	m_bRhobundleReloadEnabled = true;
#if defined(_WIN32_WCE)
    memset(&m_sai, 0, sizeof(m_sai));
    m_sai.cbSize = sizeof(m_sai);
#endif
	m_current_url = NULL;
}

CMainWindow::~CMainWindow()
{
	free(m_current_url);
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
	m_browser.Create(m_hWnd,
                     CWindow::rcDefault, // proper sizing is done in CMainWindow::OnSize
					 TEXT("Shell.Explorer"), // ProgID of the control
                     WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0,
                     ID_BROWSER);
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
    VERIFY(SystemParametersInfo(SPI_GETWORKAREA, 0, &rcMainWindow, 0));

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

    MoveWindow(&rcMainWindow);

    _ASSERT(SUCCEEDED(hr));
Error:
    return SUCCEEDED(hr) ? 0 : -1;
}

LRESULT CMainWindow::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
#if defined (_WIN32_WCE)
    m_menuBar = NULL;
#endif

    // Tear down connection points while controls are still alive.
    ATLVERIFY(SUCCEEDED(AtlAdviseSinkMap(this, false)));

    m_spIWebBrowser2 = NULL;
    m_browser = NULL;

    PostQuitMessage(0);

    bHandled = FALSE; // Allow ATL's default processing (e.g. NULLing m_hWnd)
    return 0;
}

LRESULT CMainWindow::OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
    m_browser.MoveWindow(0, 0, LOWORD(lParam), HIWORD(lParam));
    return 0;
}

LRESULT CMainWindow::OnActivate(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
    int fActive = LOWORD(wParam);

#if defined(_WIN32_WCE)
    // Notify shell of our WM_ACTIVATE message
    SHHandleWMActivate(m_hWnd, wParam, lParam, &m_sai, 0);

    //pause_sync(!fActive);

    if ( fActive )
        CHttpServer::Instance()->ResumeThread();
    else
        CHttpServer::Instance()->FreezeThread();
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
    SendMessage(WM_CLOSE);
    return 0;
}

LRESULT CMainWindow::OnBackCommand(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    m_spIWebBrowser2->GoBack();
    return 0;
}

LRESULT CMainWindow::OnForwardCommand(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    m_spIWebBrowser2->GoForward();
    return 0;
}

LRESULT CMainWindow::OnHomeCommand(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	if ( m_bRhobundleReloadEnabled )
	{
		m_bRhobundleReloadEnabled = false;

		#ifdef ENABLE_DYNAMIC_RHOBUNDLE
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

	m_spIWebBrowser2->Navigate(CHttpServer::Instance()->GetStartPage(), NULL, NULL, NULL, NULL);
	return 0;
}

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

LRESULT CMainWindow::OnRefreshCommand(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
//    m_spIWebBrowser2->Refresh();

	LPTSTR wcurl = wce_mbtowc(GetCurrentLocation());
	Navigate2(wcurl);
	free(wcurl);
    return 0;
}

LRESULT CMainWindow::OnStopCommand(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    m_spIWebBrowser2->Stop();
    return 0;
}

extern "C" void dosync();

LRESULT CMainWindow::OnSyncCommand(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	dosync();
    return 0;
}

LRESULT CMainWindow::OnOptionsCommand(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	m_spIWebBrowser2->Navigate(CHttpServer::Instance()->GetOptionsPage(), NULL, NULL, NULL, NULL);
	return 0;
}

LRESULT CMainWindow::OnReloadRhobundleCommand(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
#ifdef ENABLE_DYNAMIC_RHOBUNDLE
	if ( CHttpServer::Instance()->GetRhobundleReloadUrl() )
		CAppManager::ReloadRhoBundle(CHttpServer::Instance()->GetRhobundleReloadUrl(), NULL);
#endif
	return 0;
}

void CMainWindow::SendCameraCallbackRequest(HRESULT status, LPTSTR image_name, char* callback_url) {

	char* callback = canonicalizeURL(callback_url);

	char* imageuri = NULL, *message;
	if (status==S_OK) {
		imageuri = wce_wctomb(image_name);
		int len = 256+strlen(imageuri);
		message = (char*) malloc(len);
		sprintf(message,"status=ok&image_uri=%%2Fpublic%%2Fdb-files%%2F%s",imageuri);
	} else {
		char* status_message = (status==S_FALSE?"User canceled operation":"Error");
		int len = 256+strlen(status_message);
		message = (char*) malloc(len);
		sprintf(message,"status=%s&message=%s",
			(status==S_FALSE?"cancel":"error"),status_message);
	}

	char* headers = "Content-Type: application/x-www-form-urlencoded\r\n";
	char* res = m_callbackRequest.doRequest(L"POST",callback,headers,strlen(headers),message,strlen(message));
	if ( res ) free(res);

	free(message);
	if (imageuri) free(imageuri);
	free(callback);

}

LRESULT CMainWindow::OnTakePicture(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/) {
#if defined (_WIN32_WCE)
	Camera camera;
	TCHAR image_uri[MAX_PATH];
	HRESULT status = camera.takePicture(this->m_hWnd,image_uri);
	SendCameraCallbackRequest(status, image_uri, (char*)lParam);
#endif
	return 0;
}

LRESULT CMainWindow::OnSelectPicture(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/) {
#if defined (_WIN32_WCE)
	Camera camera;
	TCHAR image_uri[MAX_PATH];
	HRESULT status = camera.selectPicture(this->m_hWnd,image_uri);
	SendCameraCallbackRequest(status, image_uri, (char*)lParam);
#endif
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
    TCHAR szOutput[128];

    StringCchPrintf(szOutput, ARRAYSIZE(szOutput),
                    TEXT("0x%08p %s %s\n"), pDisp, OLE2CT(V_BSTR(pvtURL)),
                    VT_ERROR != V_VT(pvtTargetFrameName) ? OLE2CT(V_BSTR(pvtTargetFrameName)) : TEXT("(frame name error)"));
    OutputDebugString(szOutput);

    SetWindowText(TEXT("Untitled"));

    ATLVERIFY(SetEnabledState(IDM_STOP, TRUE));
}

void __stdcall CMainWindow::OnBrowserTitleChange(BSTR bstrTitleText)
{
    USES_CONVERSION;
    TCHAR szOutput[128];

    StringCchPrintf(szOutput, ARRAYSIZE(szOutput),
                    TEXT("%s\n"), OLE2CT(bstrTitleText));
    OutputDebugString(szOutput);

    SetWindowText(OLE2CT(bstrTitleText));
}

void __stdcall CMainWindow::OnNavigateComplete2(IDispatch* pDisp, VARIANT * pvtURL)
{
    USES_CONVERSION;
    TCHAR szOutput[128];

    StringCchPrintf(szOutput, ARRAYSIZE(szOutput),
                    TEXT("0x%08p %s\n"), pDisp, OLE2CT(V_BSTR(pvtURL)));
    OutputDebugString(szOutput);
}

#if defined(_WIN32_WCE)
std::wstring& loadLoadingHtml(std::wstring& str) {
	FILE *file;
	wchar_t	buf[1024];
	std::string fname = RhoGetRootPath();

	fname.append("apps\\loading.html");
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
    TCHAR szOutput[256];

	LPCTSTR url = OLE2CT(V_BSTR(pvtURL));
	if (m_bLoading && wcscmp(url,_T("about:blank"))==0) {
		OutputDebugString(L"Show loading page\n");
#if defined(_WIN32_WCE)
		ShowLoadingPage(pDisp, pvtURL);
#endif //_WIN32_WCE
		m_bLoading = false; //show loading page only once
	}

	if (m_current_url) {
		free(m_current_url);
	}
	m_current_url = wce_wctomb(url);

    StringCchPrintf(szOutput, ARRAYSIZE(szOutput),
		TEXT("Current URL: %s\n"), url);
    OutputDebugString(szOutput);

    ATLVERIFY(SetEnabledState(IDM_STOP, FALSE));
}

void __stdcall CMainWindow::OnCommandStateChange(long lCommand, BOOL bEnable)
{
    if (CSC_NAVIGATEBACK == lCommand)
    {
        ATLVERIFY(SetEnabledState(IDM_BACK, bEnable));
    }
    else if (CSC_NAVIGATEFORWARD == lCommand)
    {
        ATLVERIFY(SetEnabledState(IDM_FORWARD, bEnable));
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

#if !defined(_WIN32_WCE)
/* char -> wchar_t */
wchar_t* wce_mbtowc(const char* a)
{
	int length;
	wchar_t *wbuf;

	length = MultiByteToWideChar(CP_ACP, 0, 
		a, -1, NULL, 0);
	wbuf = (wchar_t*)malloc( (length+1)*sizeof(wchar_t) );
	MultiByteToWideChar(CP_ACP, 0,
		a, -1, wbuf, length);

	return wbuf;
}

/* wchar_t -> char */
char* wce_wctomb(const wchar_t* w)
{
	DWORD charlength;
	char* pChar;

	charlength = WideCharToMultiByte(CP_ACP, 0, w,
					-1, NULL, 0, NULL, NULL);
	pChar = (char*)malloc(charlength+1);
	WideCharToMultiByte(CP_ACP, 0, w,
		-1, pChar, charlength, NULL, NULL);

	return pChar;
}
#endif //_WIN32_WCE