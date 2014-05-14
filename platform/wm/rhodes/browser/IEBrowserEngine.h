#pragma once
#include "ibrowserengine.h"

#include "logging/RhoLog.h"

#include "EngineDefines.h"

#if defined (_WIN32_WCE) && !defined( OS_PLATFORM_MOTCE )
#include <pvdispid.h>
#include <piedocvw.h>
#endif

class CIEBrowserEngine :  public rho::IBrowserEngine
{
    DEFINE_LOGCLASS;

private:
    static HWND     m_hwndTabHTMLContainer;          ///< HTML Window for this Tab's HTML Component's Parent
    bool            m_bsvScrollBars;                 ///<  Whether scrollbars or visible or not
    HWND            m_hwndTabHTML;                   ///< HTML Window Handle for this Tab's HTML Component
    RECT            m_rcViewSize;
    int             m_tabID;                         ///< The unique PocketBrowser reference for this tab (PocketBrowser Application)
    HWND            m_parentHWND; 
    HINSTANCE       m_hparentInst;
    bool            m_bPageLoaded;
    TCHAR           m_tcNavigatedURL[MAX_URL];		 ///< The current URL loaded or being navigated to
    HANDLE          m_hNavigated;					 ///< Event handle set on document complete or on navigation error, used to stop the navigation timeout thread.
    int             m_dwNavigationTimeout;
private:
    //
    LRESULT CreateEngine();
    //
    HRESULT RegisterWindowClass(HINSTANCE hInstance, WNDPROC appWndProc);
    //
    void InvokeEngineEventTitleChange(LPTSTR tcTitle);
    void InvokeEngineEventMetaTag(LPTSTR tcHttpEquiv, LPTSTR tcContent);
    void InvokeEngineEventLoad(LPTSTR tcURL, EngineEventID eeEventID);

    static DWORD WINAPI NavigationTimeoutThread( LPVOID lpParameter );
    static LRESULT WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

public:
    CIEBrowserEngine(HWND hParentWnd, HINSTANCE hInstance);
    virtual ~CIEBrowserEngine(void);

    virtual BOOL Navigate(LPCTSTR szURL, int iTabID);
    virtual HWND GetHTMLWND(int /*iTabID*/); 
    virtual BOOL ResizeOnTab(int iInstID,RECT rcNewSize);
    virtual BOOL BackOnTab(int iInstID,int iPagesBack = 1);
    virtual BOOL ForwardOnTab(int iInstID);
    virtual BOOL ReloadOnTab(bool bFromCache, UINT iTab);
    virtual BOOL StopOnTab(UINT iTab);
    virtual BOOL NavigateToHtml(LPCTSTR szHtml);
    virtual LRESULT OnWebKitMessages(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    virtual void RunMessageLoop(CMainWindow& mainWnd);
    virtual void SetCookie(char* url, char* cookie);
    virtual bool isExistJavascript(const wchar_t* szJSFunction, int index);
    virtual void executeJavascript(const wchar_t* szJSFunction, int index);
    virtual BOOL ZoomPageOnTab(float fZoom, UINT iTab);
    virtual BOOL ZoomTextOnTab(int nZoom, UINT iTab);
    virtual int GetTextZoomOnTab(UINT iTab);
    virtual BOOL GetTitleOnTab(LPTSTR szURL, UINT iMaxLen, UINT iTab);
    virtual void OnDocumentComplete(LPCTSTR url);
    virtual void setBrowserGesturing(bool bEnableGesturing);
    virtual void NotifyEngineOfSipPosition();
    virtual void setNavigationTimeout(unsigned int dwMilliseconds);
	virtual bool RegisterForMessage(unsigned int iMsgId);
	virtual bool DeRegisterForMessage(unsigned int iMsgId);
	virtual bool RegisterForPrimaryMessage(unsigned int iMsgId);
	virtual bool DeRegisterForPrimaryMessage(unsigned int iMsgId);

    virtual int NewTab() {return 0;}//returns	the new tab ID 
    virtual int SwitchTab(int iTabID) {return 0;}//returns the previous tab ID
    virtual BOOL CloseTab(int iTabID) {return 0;}//returns TRUE if successful
};