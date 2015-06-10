#pragma once
#include "ibrowserengine.h"

#include "logging/RhoLog.h"

#include "EngineDefines.h"

#if defined (_WIN32_WCE) && !defined( OS_PLATFORM_MOTCE )
#include <pvdispid.h>
#include <piedocvw.h>
#endif
#define MAX_HISTORY 30
class CHistoryElement
{
public:
	LPTSTR tcURL;			///< URL of page in the history, this will never be a JavaScript function.
	CHistoryElement* pPrev;	///< Pointer to previous element in the list of history elements.
	CHistoryElement* pNext;	///< Pointer to next element in the list of history elements.
};
class CIEBrowserEngine :  public rho::IBrowserEngine
{
    DEFINE_LOGCLASS;

private:
    static CIEBrowserEngine* g_hInstance;
    static HWND              g_hwndTabHTMLContainer;          ///< HTML Window for this Tab's HTML Component's Parent

    HWND            m_parentHWND; 
    HWND            m_hwndTabHTML;                   ///< HTML Window Handle for this Tab's HTML Component

    HINSTANCE       m_hparentInst;
    RECT            m_rcViewSize;

    bool            m_bsvScrollBars;                 ///<  Whether scrollbars or visible or not        
    int             m_tabID;                         ///< The unique PocketBrowser reference for this tab (PocketBrowser Application)        
    bool            m_bLoadingComplete;
    TCHAR           m_tcNavigatedURL[MAX_URL];		 ///< The current URL loaded or being navigated to
    HANDLE          m_hNavigated;					 ///< Event handle set on document complete or on navigation error, used to stop the navigation timeout thread.
    int             m_dwNavigationTimeout;
	CHistoryElement* m_urlList;				///< List of all URLs visited by the browser
	CHistoryElement* m_currentPage;			///< Pointer to current position in the History List

	int				m_textZoomValue;		///< The text zoom value of an html page.
	
private:
    //
    LRESULT CreateEngine();
    //
    HRESULT RegisterWindowClass(HINSTANCE hInstance, WNDPROC appWndProc);
    //
    void InvokeEngineEventTitleChange(LPTSTR tcTitle);
    void InvokeEngineEventMetaTag(LPTSTR tcHttpEquiv, LPTSTR tcContent);
    void InvokeEngineEventLoad(LPTSTR tcURL, EngineEventID eeEventID);
	/**
	*  Add a URL to the history list.  The URL will be added to the end 
	*  of the history list, if there are entries after the current pointer 
	*  (because the user has navigated back) all entries after the current 
	*  pointer are deleted.
	*  \param urlNew URL to add to the History List.
	*  \return Whether or not the URL was successfully added to the History
	*/
	BOOL AddNewUrl(LPCTSTR urlNew);

	/**
	*  Instruct the engine to navigate back to a previously visited page.
	*  This function will have no effect if the tab can not be navigated back 
	*  the specified number of pages.
	*  \return Whether or not the tab was able to navigate back
	*/
	LRESULT GetPreviousUrl(LPTSTR tcURL);

	/**
	*  Calculate the number of times the user can navigate back before they 
	*  run out of history.
	*  \return Number of pages in this back history.
	*/
	UINT BackListSize();

	/**
	*  Given a pointer to an element in the history list this function deletes
	*  all list entries forward of this position.
	*  \param fromThisElementOn Element in this history list after which all 
	*  elements should be deleted.
	*/
	void DeleteCascade(CHistoryElement* fromThisElementOn);
    static DWORD WINAPI NavigationTimeoutThread( LPVOID lpParameter );
    static DWORD WINAPI CIEBrowserEngine::RegisterWndProcThread(LPVOID lpParameter);
    static LRESULT WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    
public:

    static CIEBrowserEngine* getInstance();
    static CIEBrowserEngine* getInstance(HWND hParentWnd, HINSTANCE hInstance);

    CIEBrowserEngine(HWND hParentWnd, HINSTANCE hInstance);
    virtual ~CIEBrowserEngine(void);

    virtual BOOL Navigate(LPCTSTR szURL, int iTabID);
    virtual HWND GetHTMLWND(int /*iTabID*/); 
    virtual BOOL ResizeOnTab(int iInstID,RECT rcNewSize);
    virtual BOOL BackOnTab(int iInstID,int iPagesBack = 1);
    virtual BOOL ForwardOnTab(int iInstID);
    virtual BOOL ReloadOnTab(bool bFromCache, UINT iTab);
    virtual BOOL StopOnTab(UINT iTab);
    virtual BOOL NavigateToHtml(LPCTSTR szHtml) {return TRUE;}
    virtual LRESULT OnWebKitMessages(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    virtual void RunMessageLoop(CMainWindow& mainWnd);
    virtual void SetCookie(char* url, char* cookie) {}
    virtual bool isExistJavascript(const wchar_t* szJSFunction, int index){return true;}
    virtual void executeJavascript(const wchar_t* szJSFunction, int index);
    virtual BOOL ZoomPageOnTab(float fZoom, UINT iTab);
    virtual BOOL ZoomTextOnTab(int nZoom, UINT iTab);
    virtual int GetTextZoomOnTab(UINT iTab);
    virtual BOOL GetTitleOnTab(LPTSTR szURL, UINT iMaxLen, UINT iTab);
    virtual void OnDocumentComplete(LPCTSTR url);
    virtual void setBrowserGesturing(bool bEnableGesturing) {}
    virtual void NotifyEngineOfSipPosition() {}
    virtual void setNavigationTimeout(unsigned int dwMilliseconds);
    virtual bool RegisterForMessage(unsigned int iMsgId) {return true;}
    virtual bool DeRegisterForMessage(unsigned int iMsgId) {return true;}
    virtual bool RegisterForPrimaryMessage(unsigned int iMsgId) {return true;}
    virtual bool DeRegisterForPrimaryMessage(unsigned int iMsgId) {return true;}

    virtual int NewTab() {return 0;}//returns	the new tab ID 
    virtual int SwitchTab(int iTabID) {return 0;}//returns the previous tab ID
    virtual BOOL CloseTab(int iTabID) {return 0;}//returns TRUE if successful
};
