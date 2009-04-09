// MainWindow.h: Defines main window for this application.

#pragma once

#include "stdafx.h"
#if !defined(_WIN32_WCE)
#include <exdispid.h>
#include <exdisp.h>
#endif
#include "resource.h"
#include "GetURLDialog.h"
#include "NetRequest.h"
#include "logging/RhoLog.h"
#if defined(WIN32)
#include "menubar.h"
#endif

static const UINT ID_BROWSER = 1;
static UINT WM_TAKEPICTURE = ::RegisterWindowMessage(L"RHODES_WM_TAKEPICTURE");
static UINT WM_SELECTPICTURE = ::RegisterWindowMessage(L"RHODES_WM_SELECTPICTURE");

class CMainWindow :
#if defined(_WIN32_WCE)
    public CWindowImpl<CMainWindow, CWindow, CWinTraits<WS_CLIPCHILDREN | WS_CLIPSIBLINGS> >,
#else
    public CWindowImpl<CMainWindow, CWindow, CWinTraits<WS_BORDER | WS_SYSMENU | WS_MINIMIZEBOX | WS_CLIPCHILDREN | WS_CLIPSIBLINGS> >,
#endif
    public IDispEventImpl<ID_BROWSER, CMainWindow>
{
    DEFINE_LOGCLASS;
public:
    CMainWindow();
    ~CMainWindow();
    //
	void Navigate2(BSTR URL);
    void Navigate(BSTR URL);
	
	char* GetCurrentLocation() { return m_current_url; }

    // Required to forward messages to the PIEWebBrowser control
    BOOL TranslateAccelerator(MSG* pMsg);

    DECLARE_WND_CLASS(TEXT("Rhodes.MainWindow"))

    BEGIN_MSG_MAP(CMainWindow)
        MESSAGE_HANDLER(WM_CREATE, OnCreate)
        MESSAGE_HANDLER(WM_SIZE, OnSize)
        MESSAGE_HANDLER(WM_ACTIVATE, OnActivate)
        MESSAGE_HANDLER(WM_SETTINGCHANGE, OnSettingChange)
        MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
        COMMAND_ID_HANDLER(IDM_SK1_EXIT, OnExitCommand)
        COMMAND_ID_HANDLER(IDM_BACK, OnBackCommand)
        COMMAND_ID_HANDLER(IDM_FORWARD, OnForwardCommand)
        COMMAND_ID_HANDLER(IDM_HOME, OnHomeCommand)
        COMMAND_ID_HANDLER(IDM_OPENURL, OnOpenURLCommand)
        COMMAND_ID_HANDLER(IDM_REFRESH, OnRefreshCommand)
        COMMAND_ID_HANDLER(IDM_STOP, OnStopCommand)
        COMMAND_ID_HANDLER(IDM_SYNC, OnSyncCommand)
        COMMAND_ID_HANDLER(IDM_OPTIONS, OnOptionsCommand)
		COMMAND_ID_HANDLER(IDM_RELOADRHOBUNDLE, OnReloadRhobundleCommand)
#if !defined(_WIN32_WCE)
		COMMAND_ID_HANDLER(IDM_POPUP_MENU, OnPopupMenuCommand)
#endif
		MESSAGE_HANDLER(WM_TAKEPICTURE, OnTakePicture)
		MESSAGE_HANDLER(WM_SELECTPICTURE, OnSelectPicture)
    END_MSG_MAP()

private:
    // WM_xxx handlers
    LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
    LRESULT OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/);
    LRESULT OnActivate(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/);
    LRESULT OnSettingChange(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/);
    LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);

    // WM_COMMAND handlers
    LRESULT OnExitCommand(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
    LRESULT OnBackCommand(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
    LRESULT OnForwardCommand(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
    LRESULT OnHomeCommand(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
    LRESULT OnOpenURLCommand(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
    LRESULT OnRefreshCommand(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
    LRESULT OnStopCommand(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
    LRESULT OnSyncCommand(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnOptionsCommand(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnReloadRhobundleCommand(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
#if !defined(_WIN32_WCE)
	LRESULT OnPopupMenuCommand(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
#endif

	LRESULT OnTakePicture(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/);
	LRESULT OnSelectPicture(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/);

public:
    BEGIN_SINK_MAP(CMainWindow)
        SINK_ENTRY(ID_BROWSER, DISPID_BEFORENAVIGATE2, &CMainWindow::OnBeforeNavigate2)
        SINK_ENTRY(ID_BROWSER, DISPID_TITLECHANGE, &CMainWindow::OnBrowserTitleChange)
        SINK_ENTRY(ID_BROWSER, DISPID_NAVIGATECOMPLETE2, &CMainWindow::OnNavigateComplete2)
        SINK_ENTRY(ID_BROWSER, DISPID_DOCUMENTCOMPLETE, &CMainWindow::OnDocumentComplete)
        SINK_ENTRY(ID_BROWSER, DISPID_COMMANDSTATECHANGE, &CMainWindow::OnCommandStateChange)
    END_SINK_MAP()

private:
    // event handlers
    void __stdcall OnBeforeNavigate2(IDispatch* pDisp, VARIANT * pvtURL, 
                                     VARIANT * /*pvtFlags*/, VARIANT * pvtTargetFrameName,
                                     VARIANT * /*pvtPostData*/, VARIANT * /*pvtHeaders*/, 
                                     VARIANT_BOOL * /*pvbCancel*/);
    void __stdcall OnBrowserTitleChange(BSTR bstrTitleText);
    void __stdcall OnNavigateComplete2(IDispatch* pDisp, VARIANT * pvtURL);
    void __stdcall OnDocumentComplete(IDispatch* pDisp, VARIANT * pvtURL);
    void __stdcall OnCommandStateChange(long lCommand, BOOL bEnable);

    // utility functions
    BOOL SetEnabledState(UINT uMenuItemID, BOOL bEnable);
	void ShowLoadingPage(LPDISPATCH pDisp, VARIANT* URL);

private:
    // Represents the PIEWebBrowser control contained in the main application.
    // window. m_browser is used to manage the control and its associated 
    // "AtlAxWin" window. (AtlAxWin is a window class that ATL uses to support 
    // containment of controls in windows.)
    CAxWindow m_browser;

    // cached copy of hosted control's IWebBrowser2 interface pointer
    CComPtr<IWebBrowser2> m_spIWebBrowser2;

#if defined(_WIN32_WCE)
    // main menu bar for application
    CWindow m_menuBar;
#elif defined (WIN32)
	CMenuBar m_menuBar;
	int m_menuBarHeight;
#endif //_WIN32_WCE

#if defined(_WIN32_WCE)
    // Used to manage SIP state. Also used to adjust window for SIP.
    SHACTIVATEINFO m_sai;
#endif

	bool m_bLoading;
	bool m_bRhobundleReloadEnabled;
	char* m_current_url;

private:
	CNetRequest m_callbackRequest;
	void SendCameraCallbackRequest(HRESULT status, LPTSTR image_name, char* callback_url);
};
