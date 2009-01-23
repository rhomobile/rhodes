// MainWindow.cpp: Defines main window for this application.

#include "stdafx.h"
#include <webvw.h> 
#include <string>
#include "resource.h"
#include "MainWindow.h"
#include "HttpServer.h"
#include "AppManager.h"
#include "rhoruby/rhoruby.h"

CMainWindow::CMainWindow()
{
	m_bLoading = true;
	m_bRhobundleReloadEnabled = true;
    memset(&m_sai, 0, sizeof(m_sai));
    m_sai.cbSize = sizeof(m_sai);
}

CMainWindow::~CMainWindow()
{
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
    SHMENUBARINFO mbi = { sizeof(mbi), 0 };
    RECT rcMainWindow = { 0 };
    RECT rcMenuBar = { 0 };
    SIPINFO si = { sizeof(si), 0 };

    // In one step, create an "AtlAxWin" window for the PIEWebBrowser control, 
    // and also create the control itself. (AtlAxWin is a window class that 
    // ATL uses to support containment of controls in windows.)
    m_browser.Create(m_hWnd,
                     CWindow::rcDefault, // proper sizing is done in CMainWindow::OnSize
                     TEXT("Microsoft.PIEDocView"), // ProgID of the control
                     WS_CHILD | WS_VISIBLE | WS_BORDER, 0,
                     ID_BROWSER);
    CBR(m_browser.m_hWnd != NULL);

    // cache IWebBrowser2 interface pointer
    hr = m_browser.QueryControl(&m_spIWebBrowser2);
    CHR(hr);

    // set up connection point
    hr = AtlAdviseSinkMap(this, true);
    CHR(hr);

    // set initial properties for the control
    m_spIWebBrowser2->put_AddressBar(VARIANT_TRUE);

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

    MoveWindow(&rcMainWindow);

    ASSERT(SUCCEEDED(hr));
Error:
    return SUCCEEDED(hr) ? 0 : -1;
}

LRESULT CMainWindow::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
    m_menuBar = NULL;

    // Tear down connection points while controls are still alive.
    VERIFY(SUCCEEDED(AtlAdviseSinkMap(this, false)));

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
    // Notify shell of our WM_ACTIVATE message
    SHHandleWMActivate(m_hWnd, wParam, lParam, &m_sai, 0);
    return 0;
}

LRESULT CMainWindow::OnSettingChange(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
    // Notify shell of our WM_SETTINGCHANGE message
    SHHandleWMSettingChange(m_hWnd, wParam, lParam, &m_sai);
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
		#else
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
    m_spIWebBrowser2->Refresh();
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

LRESULT CMainWindow::OnReloadRhobundleCommand(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
#ifdef ENABLE_DYNAMIC_RHOBUNDLE
	if ( CHttpServer::Instance()->GetRhobundleReloadUrl() )
		CAppManager::ReloadRhoBundle(CHttpServer::Instance()->GetRhobundleReloadUrl(), NULL);
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

    VERIFY(SetEnabledState(IDM_STOP, TRUE));
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

void __stdcall CMainWindow::OnDocumentComplete(IDispatch* pDisp, VARIANT * pvtURL)
{
    USES_CONVERSION;
    TCHAR szOutput[128];
	
	LPCTSTR url = OLE2CT(V_BSTR(pvtURL)); 
	if (m_bLoading && wcscmp(url,_T("about:blank"))==0) {
		OutputDebugString(L"Show loading page\n");
		ShowLoadingPage(pDisp, pvtURL);
		m_bLoading = false; //show loading page only once
	}

    StringCchPrintf(szOutput, ARRAYSIZE(szOutput), 
                    TEXT("0x%08p %s\n"), pDisp, url);
    OutputDebugString(szOutput);

    VERIFY(SetEnabledState(IDM_STOP, FALSE));
}

void __stdcall CMainWindow::OnCommandStateChange(long lCommand, BOOL bEnable)
{
    if (CSC_NAVIGATEBACK == lCommand)
    {
        VERIFY(SetEnabledState(IDM_BACK, bEnable));
    }
    else if (CSC_NAVIGATEFORWARD == lCommand)
    {
        VERIFY(SetEnabledState(IDM_FORWARD, bEnable));
    }
}

// **************************************************************************
//
// utility function
//
// **************************************************************************
BOOL CMainWindow::SetEnabledState(UINT uMenuItemID, BOOL bEnable)
{
    HMENU hMenu = (HMENU)m_menuBar.SendMessage(SHCMBM_GETSUBMENU, 0, IDM_SK2_MENU);
    BOOL bRetVal = EnableMenuItem(hMenu, uMenuItemID, bEnable ? MF_ENABLED : MF_GRAYED);
    return (bRetVal != 0xFFFFFFFF);
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
