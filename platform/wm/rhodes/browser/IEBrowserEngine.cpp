#include "stdafx.h"

#include <wininet.h>

#include "IEBrowserEngine.h"
#include "common/RhoConf.h"
#include "MainWindow.h"
#include "EngineDefines.h"

IMPLEMENT_LOGCLASS(CIEBrowserEngine,"IEBrowser");

//////////////////////////////////////////////////////////////////////////

#define HS_NOSCROLL             0x0020  // Disable scrolling
#define HS_NOFITTOWINDOW        0x0001  // Disable "fit to window" feature
#define HS_NOSELECTION          0x0200  // Disable content selection

#define DTM_NAVIGATE            (WM_USER + 120)

//////////////////////////////////////////////////////////////////////////

HWND CIEMobileEngineTab::m_hwndTabHTMLContainer = NULL;

//////////////////////////////////////////////////////////////////////////

CIEBrowserEngine::CIEBrowserEngine(HWND hParentWnd, HINSTANCE hInstance)// :
{
    m_parentHWND = hParentWnd;
    m_hparentInst = hInstance;
    m_tabID = 0; //tabID;
    //m_bsvScrollBars = bsvScrollbars;

    //get the parents window size for the default size of the HTML
    //container.
    GetWindowRect(hParentWnd, &m_rcViewSize);

    for(int i=0; i < EEID_MAXEVENTID; i++)
        m_EngineEvents[i] = NULL;

    //memset(m_tcIconURL, NULL, sizeof(TCHAR) * MAX_URL);
    //wcsncpy(m_tcIconURL, tcIconURL, MAX_URL);
    //memset(m_tcCurrentPageTitle, NULL, sizeof(TCHAR) * MAXURL);

    CreateEngine();
}

CIEBrowserEngine::CIEBrowserEngine(HWND hParentWnd, HINSTANCE hInstance)// :
{
    //  Remove the browser history object from memory
    //delete m_BrowserHistory;
    //m_BrowserHistory = NULL;

    //  Destroy the Browser Object
    DestroyWindow(m_hwndTabHTML);
    m_hwndTabHTML = NULL;

    //  Destroy the Browser Object's parent if it exists
    if (m_hwndTabHTMLContainer)
    {
        DestroyWindow(m_hwndTabHTMLContainer);
        m_hwndTabHTMLContainer = NULL;
    }
}

LRESULT CIEBrowserEngine::CreateEngine()
{
    //  Create an HTML container to listen for Engine Events if one does not 
    //  already exist
    if (m_hwndTabHTMLContainer == NULL)
    {
        //register the main window
        if (S_OK != RegisterWindowClass(m_hparentInst, &CIEMobileEngineTab::WndProc))
            return S_FALSE;

        //create the main window
        m_hwndTabHTMLContainer = CreateWindowEx( 0, HTML_CONTAINER_NAME, NULL, 
            WS_POPUP | WS_VISIBLE, 
            CW_USEDEFAULT, CW_USEDEFAULT, 
            CW_USEDEFAULT, CW_USEDEFAULT, 
            m_parentHWND, NULL, m_hparentInst, 0);

        if(!m_hwndTabHTMLContainer)
            return S_FALSE;


        ////initialize the HTML Control
        //if(!InitHTMLControl(m_hparentInst))
        //    return S_FALSE;
    }

    HRESULT dwResult = 0;

    //create the HTML window
    DWORD scrollStyle;
    if (m_bsvScrollBars == SETTING_ON)
        scrollStyle = 0;
    else
        scrollStyle = HS_NOSCROLL;

    //  Read the FitToScreenEnabled Config Setting
    TCHAR tcConfigSetting[MAX_URL];
    memset(tcConfigSetting, 0, sizeof(TCHAR) * MAX_URL);
    //  By default the window is created with FitToScreen true, so if it is 
    //  false use the HS_NOFITTOWINDOW windowstyle
    DWORD dwFitToWindowStyle, dwTextSelectionStyle;
    dwFitToWindowStyle = 0;
    dwTextSelectionStyle = 0;
    //if (configFunction != NULL)
    {
        //configFunction(m_tabID, L"HTMLStyles\\FitToScreenEnabled", tcConfigSetting);
        if (tcConfigSetting)
        {
            if (*tcConfigSetting == L'0')
                dwFitToWindowStyle = HS_NOFITTOWINDOW;
        }

        //configFunction(m_tabID, L"HTMLStyles\\TextSelectionEnabled", tcConfigSetting);
        if (tcConfigSetting)
        {
            if (*tcConfigSetting == L'0')
                dwTextSelectionStyle = HS_NOSELECTION;
        }
    }
    m_hwndTabHTML = CreateWindow(WC_HTML, NULL, 
        WS_POPUP | WS_VISIBLE | scrollStyle | dwFitToWindowStyle | dwTextSelectionStyle, 
        m_rcViewSize.left, m_rcViewSize.top, 
        (m_rcViewSize.right-m_rcViewSize.left), 
        (m_rcViewSize.bottom-m_rcViewSize.top), 
        m_hwndTabHTMLContainer, NULL, m_hparentInst, NULL);

    /*
    if (m_hwndTabHTML)
    {
        if (configFunction != NULL)
        {
            //  Read the JavascriptEnabled configuration setting
            memset(tcConfigSetting, 0, sizeof(TCHAR) * MAX_URL);
            configFunction(m_tabID, L"HTMLStyles\\JavascriptEnabled", tcConfigSetting);
            if (tcConfigSetting)
            {
                if (*tcConfigSetting == L'0')
                    SetJavaScript(SETTING_OFF);
                else
                    SetJavaScript(SETTING_ON);
            }
            //  Read the ClearType Configuration Setting
            memset(tcConfigSetting, 0, sizeof(TCHAR) * MAX_URL);
            configFunction(m_tabID, L"HTMLStyles\\ClearTypeEnabled", tcConfigSetting);
            if (tcConfigSetting)
            {
                if (*tcConfigSetting == L'0')
                    SetClearType(SETTING_OFF);
                else
                    SetClearType(SETTING_ON);
            }
        }
    }
    else
    {
        //  Failed to create Tab window
        return S_FALSE;
    }
    */

    //  Create the associated History Object
    m_BrowserHistory = new CHistory(this);

    return S_OK;
}

CIEBrowserEngine::~CIEBrowserEngine(void)
{
    //TODO: destroy browser
}

BOOL CIEBrowserEngine::Navigate(LPCTSTR tcURL, int iTabID)
{
    //  On Windows Mobile devices it has been observed that attempting to 
    //  navigate to a Javascript function before the page is fully loaded can 
    //  crash PocketBrowser (specifically when using Reload).  This condition
    //  prevents that behaviour.
    if (!m_bPageLoaded && (wcsnicmp(tcURL, L"JavaScript:", wcslen(L"JavaScript:")) == 0))
    {
        LOG(TRACE) + "Failed to Navigate, Navigation in Progress\n";
        return S_FALSE;
    }

    LRESULT retVal = S_FALSE;

    //  Engine component does not accept Navigate(page.html), it needs
    //  the absolute URL of the page, add that here (if the user puts a .\ before)
    TCHAR tcDereferencedURL[MAX_URL];
    memset(tcDereferencedURL, 0, MAX_URL * sizeof(TCHAR));
    if (IsRelativeURL(tcURL))
    {
        if (!DereferenceURL(tcURL, tcDereferencedURL, m_tcNavigatedURL, NULL))
            return S_FALSE;
    }
    else
        wcscpy(tcDereferencedURL, tcURL);

    //  Test to see if the navigation URL starts with a '\', if it does
    //  then prepend 'file://'
    if (tcDereferencedURL[0] == L'\\')
    {
        if (wcslen(tcDereferencedURL) <= (MAX_URL - wcslen(L"file://")))
        {
            TCHAR tcNewURL[MAX_URL + 1];
            wsprintf(tcNewURL, L"file://%s", tcDereferencedURL);
            retVal = SendMessage(m_hwndTabHTML, DTM_NAVIGATE, 0, (LPARAM) (LPCTSTR)tcNewURL);
        }
    }
    else if (wcslen(tcDereferencedURL) > wcslen(L"www") && wcsnicmp(tcURL, L"www", 3) == 0)
    {
        if (wcslen(tcDereferencedURL) <= (MAX_URL - wcslen(L"http://")))
        {
            TCHAR tcNewURL[MAX_URL + 1];
            wsprintf(tcNewURL, L"http://%s", tcDereferencedURL);
            retVal = SendMessage(m_hwndTabHTML, DTM_NAVIGATE, 0, (LPARAM) (LPCTSTR)tcNewURL);
        }
    }
    else
        retVal = SendMessage(m_hwndTabHTML, DTM_NAVIGATE, 0, (LPARAM) (LPCTSTR)tcDereferencedURL);

    return retVal;
}

BOOL CIEBrowserEngine::ResizeOnTab(int iInstID,RECT rcNewSize)
{
    return TRUE;
}

BOOL CIEBrowserEngine::BackOnTab(int iInstID,int iPagesBack /*= 1*/)
{
    return TRUE;
}

BOOL CIEBrowserEngine::ForwardOnTab(int iInstID)
{
    return TRUE;
}

BOOL CIEBrowserEngine::ReloadOnTab(bool bFromCache, UINT iTab)
{
    return TRUE; 
}

BOOL CIEBrowserEngine::StopOnTab(UINT iTab)
{
    return FALSE;
}

BOOL CIEBrowserEngine::ZoomPageOnTab(float fZoom, UINT iTab)
{
    return FALSE;
}

BOOL CIEBrowserEngine::ZoomTextOnTab(int nZoom, UINT iTab)
{
    return FALSE;
}

int CIEBrowserEngine::GetTextZoomOnTab(UINT iTab)
{
    return 2; //Normal
}

BOOL CIEBrowserEngine::GetTitleOnTab(LPTSTR szURL, UINT iMaxLen, UINT iTab)
{
    return FALSE;
}

BOOL CIEBrowserEngine::NavigateToHtml(LPCTSTR szHtml)
{
    return TRUE;
}

LRESULT CIEBrowserEngine::OnWebKitMessages(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    return 0;
}

void CIEBrowserEngine::RunMessageLoop(CMainWindow& mainWnd)
{
	MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        if ( RHODESAPP().getExtManager().onWndMsg(msg) )
            continue;

        if (!mainWnd.TranslateAccelerator(&msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
}

bool CIEBrowserEngine::isExistJavascript(const wchar_t* szJSFunction, int index)
{
    return true;
}

void CIEBrowserEngine::executeJavascript(const wchar_t* szJSFunction, int index)
{
}

void CIEBrowserEngine::SetCookie(char* url, char* cookie)
{
}

void CIEBrowserEngine::OnDocumentComplete(LPCTSTR url)
{
}

void CIEBrowserEngine::setBrowserGesturing(bool bEnableGesturing)
{
}

void CIEBrowserEngine::NotifyEngineOfSipPosition()
{
}

void CIEBrowserEngine::setNavigationTimeout(unsigned int dwMilliseconds)
{

}

bool CIEBrowserEngine::RegisterForMessage(unsigned int iMsgId)
{
	return true;
}

bool CIEBrowserEngine::DeRegisterForMessage(unsigned int iMsgId)
{
	return true;
}

bool CIEBrowserEngine::RegisterForPrimaryMessage(unsigned int iMsgId)
{
	return true;
}

bool CIEBrowserEngine::DeRegisterForPrimaryMessage(unsigned int iMsgId)
{
	return true;
}

int CIEBrowserEngine::NewTab() { return 0; }
int CIEBrowserEngine::SwitchTab(int iTabID){ return 0;}
virtual HWND GetHTMLWND(int /*iTabID*/){ return 0;}
BOOL CIEBrowserEngine::CloseTab(int iTabID) { return false; }