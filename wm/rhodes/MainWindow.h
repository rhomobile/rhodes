// MainWindow.h: Defines main window for this application.

#pragma once

#include "stdafx.h"
#include "resource.h"
#include "GetURLDialog.h"

static const UINT ID_BROWSER = 1;

class CMainWindow :
    public CWindowImpl<CMainWindow, CWindow, CWinTraits<WS_CLIPCHILDREN | WS_CLIPSIBLINGS> >,
    public IDispEventImpl<ID_BROWSER, CMainWindow>
{
public:
    CMainWindow();
    ~CMainWindow();
    //
	void Navigate2(BSTR URL);
    void Navigate(BSTR URL);

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
		COMMAND_ID_HANDLER(IDM_RELOADRHOBUNDLE, OnReloadRhobundleCommand)
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
	LRESULT OnReloadRhobundleCommand(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
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

    // main menu bar for application
    CWindow m_menuBar;

    // Used to manage SIP state. Also used to adjust window for SIP.
    SHACTIVATEINFO m_sai;

	bool m_bLoading;
	bool m_bRhobundleReloadEnabled;
};
