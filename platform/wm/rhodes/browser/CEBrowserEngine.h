#pragma once
#include "ibrowserengine.h"
#include "EngineEventListner.h"

#include "logging/RhoLog.h"

#if defined(OS_PLATFORM_MOTCE)

#include <exdisp.h>
#include <exdispid.h>
#include <mshtmhst.h>
#include <mshtml.h>

#if defined (_WIN32_WCE) && !defined( OS_PLATFORM_MOTCE )
#include <pvdispid.h>
#include <piedocvw.h>
#endif

#define MAXURL 2049			///< The maximum allowable length of a URL in Internet Explorer		
#ifndef MAX_URL
#define MAX_URL MAXURL	///< Maximimum allowable length of a URL in Internet Explorer
#endif

/**
*  Possible values for the browser's text zoom.  These map to The IWebBrowser2
*  zoom factor.
*/
enum TextZoomValue
{
	TEXT_ZOOM_SMALLEST = 0,	///< Smallest possible text size
	TEXT_ZOOM_SMALLER = 1,	///< Slightly smaller than normal text size
	TEXT_ZOOM_NORMAL = 2,	///< Normal Text size
	TEXT_ZOOM_BIGGER = 3,	///< Slightly larger than normal text size
	TEXT_ZOOM_BIGGEST = 4	///< Largest possible text size
};

class CEBrowserEngine :
	public rho::IBrowserEngine,
	public IOleContainer,
	public IOleClientSite,
	public IOleInPlaceSite,
	public IDocHostUIHandler2,
	public DWebBrowserEvents2,
	public IDocHostShowUI,
	public rho::engineeventlistner::IDOMInjectorEngineInterface
{
    DEFINE_LOGCLASS;

public:
	CEBrowserEngine(HWND hwndParent, HINSTANCE hInstance);
	virtual ~CEBrowserEngine(void);
	
	BOOL Reload(BOOL bFromCache);

private:
    static DWORD WINAPI RegisterWndProcThread(LPVOID lpParameter);
    static DWORD WINAPI NavigationTimeoutThread( LPVOID lpParameter );
    static DWORD WINAPI DocumentTimeoutThread( LPVOID lpParameter );
	// EMBPD00158491
	static DWORD WINAPI NetworkWindowThread( LPVOID lpParameter );

private:
    HRESULT ParseTags();
    LRESULT CreateEngine();
	void InvokeJs(const wchar_t* szJSFunction, int index);

	BOOL GetPageSize (int *pwidth, int *pheight);

	int GetTabID();

	HRESULT InitEvents();
	
	HRESULT GetHostInfo( DOCHOSTUIINFO* pInfo );
	
	HRESULT TranslateAccelerator( LPMSG lpMsg, const GUID __RPC_FAR *pguidCmdGroup, DWORD nCmdID);
	
    // implements if COM interfaces
    HRESULT QueryInterface (REFIID riid, LPVOID * ppv);

	ULONG AddRef(void);

	ULONG Release(void);

	HRESULT Scroll (SIZE scrollExtent);
	
	HRESULT GetWindow(HWND *phwnd);

    HRESULT Invoke(DISPID dispidMember,REFIID riid,LCID lcid,WORD wFlags,DISPPARAMS FAR* pdparams, VARIANT FAR* pvarResult,EXCEPINFO FAR* pexcepinfo,UINT FAR* puArgErr);
	
	HRESULT OnInPlaceActivate(void);
	
	HRESULT GetWindowContext(LPOLEINPLACEFRAME FAR * lplpFrame,LPOLEINPLACEUIWINDOW FAR * lplpDoc, LPRECT lprcPosRect, LPRECT lprcClipRect, LPOLEINPLACEFRAMEINFO lpFrameInfo);

	HRESULT OnInPlaceDeactivate(void);

    // Rho IBrowserEngine implementstion
    virtual BOOL    Navigate(LPCTSTR szURL, int iTabID);
    virtual BOOL    StopOnTab(UINT iTab);
    virtual HWND    GetHTMLWND(int /*iTabID*/){ return m_hwndTabHTML; }
    virtual BOOL    ResizeOnTab(int iInstID,RECT rcNewSize);
    virtual BOOL    BackOnTab(int iInstID,int iPagesBack = 1);
    virtual BOOL    ForwardOnTab(int iInstID);
    virtual BOOL    ReloadOnTab(bool bFromCache, UINT iTab);    
    virtual LRESULT OnWebKitMessages(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    virtual void    RunMessageLoop(CMainWindow& mainWnd);    
    virtual bool    isExistJavascript(const wchar_t* szJSFunction, int index);
    virtual void    executeJavascript(const wchar_t* szJSFunction, int index);     
    virtual void    OnDocumentComplete(LPCTSTR url);  
    virtual bool    RegisterForMessage(unsigned int iMsgId);
    virtual bool    DeRegisterForMessage(unsigned int iMsgId);
    virtual bool RegisterForPrimaryMessage(unsigned int iMsgId);
    virtual bool DeRegisterForPrimaryMessage(unsigned int iMsgId);

    virtual int NewTab();//returns	the new tab ID 
	virtual int SwitchTab(int iTabID);//returns the previous tab ID
	virtual BOOL CloseTab(int iTabID);//returns TRUE if successful

    //IDOMInjectorEngineInterface implementation
    virtual bool executAnonymousJs(wchar_t* szFunctionText, int nTabID);

#pragma region not_implemented_virtual_functions
    virtual BOOL    NavigateToHtml(LPCTSTR szHtml) { return FALSE; }
    
    virtual BOOL    GetTitleOnTab(LPTSTR szURL, UINT iMaxLen, UINT iTab) { return FALSE; }
    virtual BOOL    ZoomPageOnTab(float fZoom, UINT iTab) { return FALSE; }
    virtual BOOL    ZoomTextOnTab(int nZoom, UINT iTab);
    virtual int     GetTextZoomOnTab(UINT iTab);
    virtual void    NotifyEngineOfSipPosition() {}
    virtual void    setNavigationTimeout(unsigned int dwMilliseconds){}
    virtual void    setBrowserGesturing(bool bEnableGesturing) {}
    virtual void    SetCookie(char* url, char* cookie) {}
#pragma endregion

private:	
    //  Attributes

    IWebBrowser2	 *m_pBrowser;	///< Pointer to Microsoft IE Browser component
    IConnectionPoint *m_pCP;			///< Web Browser Connection Point
    IOleObject       *m_pObject;		///< IOleObject reference 
    HWND              m_hwndParent;
    HINSTANCE         m_hInstance;

    HANDLE	          m_hNavigated;     ///< Event handle set on document complete or on navigation error, used to stop the navigation timeout thread.
    HANDLE            m_hDocComp;

    TCHAR	          m_tcNavigatedURL[MAX_URL];	///< The current URL loaded or being navigated to
    ULONG	          m_ulRefs;					///< COM reference counter
    BOOL	          m_bInPlaceActive;			///< Whether or not one of the container's objects is activated in place    
    TCHAR	          m_tcCurrentPageTitle[MAX_URL];///< The title of the currently loaded page    
    int               m_tabID;
    HWND              m_hwndTabHTML;
    BOOL              bRunningOnWM;				///< Whether or not the COM engine is running on Windows Mobile.
    BOOL	          m_bTextSelectionEnabled;
    BOOL	          bDeviceCausesDoubleBackspace;	///< For EMBPD00023872, some devices will cause a double backspace when the backspace key is pressed, this variable is used to determine the result of PreProcessMessage.
    RECT              m_rcViewSize;

    unsigned int      m_dwNavigationTimeout;
    BOOL              m_bLoadingComplete;
    BOOL              m_bNavigationError;
	bool              m_bInitialised;
	bool			  m_bNavigationComplete;



#pragma region not_implemented_virtual_functions
	//virtual functions not implemented
	STDMETHOD(GetOverrideKeyPath)(LPOLESTR __RPC_FAR *pchKey, DWORD dw) { return E_NOTIMPL; }
	STDMETHOD(ShowContextMenu)(DWORD dwID,POINT __RPC_FAR *ppt,IUnknown __RPC_FAR *pcmdtReserved,IDispatch __RPC_FAR *pdispReserved) { return S_OK; }
	STDMETHOD(ShowUI)(DWORD dwID,IOleInPlaceActiveObject __RPC_FAR *pActiveObject,IOleCommandTarget __RPC_FAR *pCommandTarget,IOleInPlaceFrame __RPC_FAR *pFrame,IOleInPlaceUIWindow __RPC_FAR *pDoc) { return E_NOTIMPL; }
    STDMETHOD(HideUI)(void) { return E_NOTIMPL; }
	STDMETHOD(UpdateUI)(void) { return E_NOTIMPL; }
	STDMETHOD(EnableModeless)(BOOL fEnable) { return E_NOTIMPL ; }
	STDMETHOD(OnDocWindowActivate)(BOOL fActivate) { return E_NOTIMPL; }
	STDMETHOD(OnFrameWindowActivate)(BOOL fActivate) { return E_NOTIMPL; }
	STDMETHOD(ResizeBorder)(LPCRECT prcBorder,IOleInPlaceUIWindow __RPC_FAR *pUIWindow,BOOL fRameWindow) { return E_NOTIMPL; }
	STDMETHOD(GetOptionKeyPath)(LPOLESTR __RPC_FAR *pchKey,DWORD dw) { return E_NOTIMPL; }
	STDMETHOD(GetDropTarget)(IDropTarget __RPC_FAR *pDropTarget,IDropTarget __RPC_FAR *__RPC_FAR *ppDropTarget) { return E_NOTIMPL; }
	STDMETHOD(GetExternal)(IDispatch __RPC_FAR *__RPC_FAR *ppDispatch) { return E_NOTIMPL; }
	STDMETHOD(TranslateUrl)(DWORD dwTranslate,OLECHAR __RPC_FAR *pchURLIn,OLECHAR __RPC_FAR *__RPC_FAR *ppchURLOut) { return E_NOTIMPL; }
	STDMETHOD(FilterDataObject)(IDataObject __RPC_FAR *pDO,IDataObject __RPC_FAR *__RPC_FAR *ppDORet) { return E_NOTIMPL; }
	STDMETHOD(ParseDisplayName)(IBindCtx *, LPOLESTR, ULONG *, IMoniker **) { return E_NOTIMPL; }
	STDMETHOD(EnumObjects)(DWORD, IEnumUnknown **) { return E_NOTIMPL; }
	STDMETHOD(LockContainer)(BOOL) { return S_OK; }
	STDMETHOD(SaveObject)(void) { return E_NOTIMPL; }
	STDMETHOD(GetMoniker)(DWORD dwAssign, DWORD dwWhichMoniker, LPMONIKER * ppmk) { return E_NOTIMPL; }
	STDMETHOD(GetContainer)(LPOLECONTAINER * ppContainer) { return E_NOINTERFACE; }
	STDMETHOD(ShowObject)(void) { return E_NOTIMPL; }
	STDMETHOD(OnShowWindow)(BOOL fShow) { return E_NOTIMPL; }
	STDMETHOD(RequestNewObjectLayout)(void) { return E_NOTIMPL; }
	STDMETHOD(ContextSensitiveHelp)(BOOL fEnterMode) { return E_NOTIMPL; }
	STDMETHOD(CanInPlaceActivate)(void) { return S_OK; }
	STDMETHOD(OnUIActivate)(void){ return E_NOTIMPL; }

	STDMETHOD(OnUIDeactivate)(BOOL fUndoable){ return E_NOTIMPL; }
	STDMETHOD(DiscardUndoState)(void){ return E_NOTIMPL; }
	STDMETHOD(DeactivateAndUndo)(void){ return E_NOTIMPL; }
	STDMETHOD(OnPosRectChange)(LPCRECT lprcPosRect){ return E_NOTIMPL; }
	STDMETHOD(GetTypeInfoCount)(UINT FAR* pctinfo){ return E_NOTIMPL; }
	STDMETHOD(GetTypeInfo)(UINT itinfo,LCID lcid,ITypeInfo FAR* FAR* pptinfo){ return E_NOTIMPL; }
	STDMETHOD(GetIDsOfNames)(REFIID riid,OLECHAR FAR* FAR* rgszNames,UINT cNames,LCID lcid, DISPID FAR* rgdispid){ return E_NOTIMPL; }
	STDMETHOD(ShowHelp)( HWND hwnd, LPOLESTR pszHelpFile, UINT uCommand,DWORD dwData, POINT ptMouse, IDispatch *pDispatchObjectHit){ return E_NOTIMPL; }
    STDMETHOD(ShowMessage)(HWND,LPOLESTR,LPOLESTR,DWORD,LPOLESTR,DWORD,LRESULT *) { return E_NOTIMPL; }
#pragma endregion

private:
//	CScrollNotify *pScrollNotify;
};

#endif
