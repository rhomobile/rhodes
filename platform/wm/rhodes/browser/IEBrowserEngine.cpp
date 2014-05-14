#include "stdafx.h"

#include <windows.h>
#include <wininet.h>

#include "htmlctrl.h"

#include "IEBrowserEngine.h"
#include "common/RhoConf.h"
#include "MainWindow.h"

IMPLEMENT_LOGCLASS(CIEBrowserEngine,"IEBrowser");

//////////////////////////////////////////////////////////////////////////

#define NM_PIE_TITLE            (WM_USER + 104)  ///< Message ID indicating the associated message defines the page title.
#define NM_PIE_META             (WM_USER + 105)  ///< Message ID indicating the associated message defines a Meta Tag (HTTP-Equiv only for WM).
#define NM_PIE_BEFORENAVIGATE   (WM_USER + 109)  ///< Message ID indicating the associated message indicates a BeforeNavigate Event.
#define NM_PIE_DOCUMENTCOMPLETE	(WM_USER + 110)  ///< Message ID indicating the associated message defines a DocumentComplete Event.
#define NM_PIE_NAVIGATECOMPLETE (WM_USER + 111)  ///< Message ID indicating the associated message defines a NavigateComplete Event.
#define NM_PIE_KEYSTATE			(WM_USER + 112)  ///< Message ID indicating the associated message notifies that the key state has changed.
#define NM_PIE_ALPHAKEYSTATE	(WM_USER + 113)  ///< Message ID indicating the associated message notifies that the alpha key state has changed.

#define HTML_CONTAINER_NAME		TEXT("HTMLContainer")  ///< Name of the Window which is parent to all HTML components and handles notifications from the components.

//////////////////////////////////////////////////////////////////////////

HWND CIEBrowserEngine::m_hwndTabHTMLContainer = NULL;

//////////////////////////////////////////////////////////////////////////

CIEBrowserEngine::CIEBrowserEngine(HWND hParentWnd, HINSTANCE hInstance) :
        m_parentHWND(NULL),
        m_hparentInst(NULL),
        m_bPageLoaded(FALSE),
        m_hNavigated(NULL),
        m_dwNavigationTimeout(0)
{
    m_parentHWND = hParentWnd;    
    m_hparentInst = hInstance;
    m_tabID = 0; //tabID;

    GetWindowRect(hParentWnd, &m_rcViewSize);

    m_tcNavigatedURL[0] = 0;

    CreateEngine();
}

CIEBrowserEngine::~CIEBrowserEngine()
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

HRESULT CIEBrowserEngine::RegisterWindowClass(HINSTANCE hInstance, WNDPROC appWndProc) 
{
    WNDCLASS    wc = { 0 };
    HRESULT     hrResult = 0;

    if (!GetClassInfo(hInstance, HTML_CONTAINER_NAME, &wc))
    {
        wc.style            = CS_HREDRAW | CS_VREDRAW;
        wc.lpfnWndProc      = appWndProc;
        wc.hInstance        = hInstance;
        wc.hIcon			= NULL;
        wc.lpszClassName    = HTML_CONTAINER_NAME;
        wc.hCursor          = LoadCursor(NULL, IDC_ARROW);
        wc.hbrBackground    = (HBRUSH)(COLOR_WINDOW + 1);

        hrResult = (RegisterClass(&wc) ? S_OK : E_FAIL);
    }
    else
        hrResult = S_OK;

    return hrResult;
}

LRESULT CIEBrowserEngine::CreateEngine()
{
    //  Create an HTML container to listen for Engine Events if one does not 
    //  already exist
    if (m_hwndTabHTMLContainer == NULL)
    {
        //register the main window
        if (FAILED(RegisterWindowClass(m_hparentInst, &CIEBrowserEngine::WndProc)))
            return S_FALSE;

        //create the main window
        m_hwndTabHTMLContainer = CreateWindowEx( 0, HTML_CONTAINER_NAME, NULL, 
            WS_POPUP | WS_VISIBLE, 
            CW_USEDEFAULT, CW_USEDEFAULT, 
            CW_USEDEFAULT, CW_USEDEFAULT, 
            m_parentHWND, NULL, m_hparentInst, 0);

        if(!m_hwndTabHTMLContainer)
            return S_FALSE;


        if (FAILED(rho::browser::InitHTMLControl(m_hparentInst)))
            return S_FALSE;
    }

    m_hwndTabHTML = CreateWindow(WC_HTML, NULL, 
        WS_POPUP | WS_VISIBLE | HS_NOSCROLL, 
        m_rcViewSize.left, m_rcViewSize.top, 
        (m_rcViewSize.right-m_rcViewSize.left), 
        (m_rcViewSize.bottom-m_rcViewSize.top), 
        m_hwndTabHTMLContainer, NULL, m_hparentInst, NULL);    

    if (!m_hwndTabHTML)
        return S_FALSE;

    return S_OK;
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

    if (wcsicmp(tcURL, L"history:back") == 0)
    {
    }
    else
    {
        //  Engine component does not accept Navigate(page.html), it needs
        //  the absolute URL of the page, add that here (if the user puts a .\ before)
        TCHAR tcDereferencedURL[MAX_URL];
        memset(tcDereferencedURL, 0, MAX_URL * sizeof(TCHAR));
        if (rho::browser::IsRelativeURL(tcURL))
        {
            if (!rho::browser::DereferenceURL(tcURL, tcDereferencedURL, m_tcNavigatedURL))
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
    }

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
    m_dwNavigationTimeout = dwMilliseconds;
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



HWND CIEBrowserEngine::GetHTMLWND(int /*iTabID*/)
{
    return m_hwndTabHTML;
}

//////////////////////////////////////////////////////////////////////////
// invoke methods

void CIEBrowserEngine::InvokeEngineEventMetaTag(LPTSTR tcHttpEquiv, LPTSTR tcContent)
{
	//if (m_EngineEvents[EEID_METATAG])
	{
		EngineMETATag metaTag;
		memset(&metaTag, 0, sizeof(metaTag));
		metaTag.tcHTTPEquiv = tcHttpEquiv;
		metaTag.tcContents = tcContent;
		//m_EngineEvents[EEID_METATAG]
		//	(EEID_METATAG, 
		//	(LPARAM)&metaTag,
		//	m_tabID);
	}	
}

void CIEBrowserEngine::InvokeEngineEventLoad(LPTSTR tcURL, EngineEventID eeEventID)
{
	//  Engine component has indicated a load event, this should be 
	//  one of BeforeNavigate, NavigateComplete or DocumentComplete.
	wcscpy(m_tcNavigatedURL, tcURL);
	switch (eeEventID)
	{
		case EEID_BEFORENAVIGATE:
			m_bPageLoaded = FALSE;
			SetEvent(m_hNavigated);
			CloseHandle(m_hNavigated);
			m_hNavigated = NULL;

			//  Do not start the Navigation Timeout Timer if the 
			//  navigation request is a script call.
			if((!_memicmp(tcURL, L"javascript:", 11 * sizeof(TCHAR)))
				|| (!_memicmp(tcURL, L"jscript:", 8 * sizeof(TCHAR)))
				|| (!_memicmp(tcURL, L"vbscript:", 9 * sizeof(TCHAR)))
				|| (!_memicmp(tcURL, L"res://\\Windows\\shdoclc.dll/navcancl.htm", 35 * sizeof(TCHAR))))
			{
					break;
			}

			//  Test if the user has attempted to navigate back in the history
			if (wcsicmp(tcURL, L"history:back") == 0)
			{
			}
			
			CloseHandle (CreateThread(NULL, 0, 
									&CIEBrowserEngine::NavigationTimeoutThread, 
									(LPVOID)this, 0, NULL));

			//if(m_EngineEvents[EEID_BEFORENAVIGATE])
			//{
			//	m_EngineEvents[EEID_BEFORENAVIGATE](EEID_BEFORENAVIGATE, 
			//										(LPARAM)tcURL, m_tabID);
			//}
			break;
		case EEID_DOCUMENTCOMPLETE:
			m_bPageLoaded = TRUE;
			//if(m_EngineEvents[EEID_DOCUMENTCOMPLETE])
			//{
			//	m_EngineEvents[EEID_DOCUMENTCOMPLETE](EEID_DOCUMENTCOMPLETE, 
			//											(LPARAM)tcURL, m_tabID);
			//}
			break;
		case EEID_NAVIGATECOMPLETE:
			SetEvent(m_hNavigated);
			CloseHandle(m_hNavigated);
			m_hNavigated = NULL;

			////Validate that there is an event handler
			//if(m_EngineEvents[EEID_NAVIGATECOMPLETE])
			//{
			//	m_EngineEvents[EEID_NAVIGATECOMPLETE](EEID_NAVIGATECOMPLETE, 
			//											(LPARAM)tcURL, m_tabID);
			//}
			
			break;
	}
}

void CIEBrowserEngine::InvokeEngineEventTitleChange(LPTSTR tcTitle)
{
	////  Notify the core the page title has changed, if not specified between 
	////  <TITLE> </TITLE> tags this should be set to the page URL.
	//if(m_EngineEvents[EEID_TITLECHANGE])
	//{
	//	TCHAR tcURL[MAX_URL];
	//	memset(tcURL, 0, sizeof(MAX_URL) * sizeof(TCHAR));
	//	wcsncpy(tcURL, tcTitle, MAX_URL);
	//	//m_EngineEvents[EEID_TITLECHANGE](EEID_TITLECHANGE, 
	//	//								(LPARAM)tcURL, 
	//	//								m_tabID);
	//	//wcscpy(m_tcCurrentPageTitle, tcURL);
	//}
}

//////////////////////////////////////////////////////////////////////////

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		October 2009
*/
LRESULT CIEBrowserEngine::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult = S_OK;
	switch (uMsg) 
	{
		case WM_NOTIFY:
		{
			//  Received a message from the Pocket Internet Explorer component
			//  to indicate something has happened (BeforeNavigate / DocumentComplete etc).
			//  The lParam contains an NM_HTMLVIEWW containing information about
			//  the event, parse it.
			NM_HTMLVIEWW * pnmHTML;
			LPNMHDR pnmh;
			pnmHTML = (NM_HTMLVIEWW *) lParam;
			pnmh = (LPNMHDR) &(pnmHTML->hdr);
			
			//  Check the message originated from an expected window, i.e. 
			//  it came from a PIE component we had created whose HWND is identical
			//  to one of the tabs
			HWND hwndOriginatingPIEHTML = pnmh->hwndFrom;
			//  When we created the HTML tab it created a child window which 
			//  originates these messages, therefore find the parent of the 
			//  originator and ensure it has the correct class name
			HWND hwndParentOfOriginator = GetParent(hwndOriginatingPIEHTML);
			TCHAR* tcOriginatingClassName = new TCHAR[wcslen(WC_HTML)+1];
			GetClassName(hwndParentOfOriginator, tcOriginatingClassName, wcslen(WC_HTML)+1);
			if (wcscmp(tcOriginatingClassName, WC_HTML) != 0)
			{
				delete[] tcOriginatingClassName;
				break;
			}
			delete[] tcOriginatingClassName;

			//  The message has originated from one of the tabs we created, determine
			//  the tab ID from the hwnd
			//IETab* tab = GetSpecificTab(hwndParentOfOriginator);
			CIEBrowserEngine* mobileTab = (CIEBrowserEngine*)0; //tab->pEngine;

			//  Invoke the appropriate tab with the event.  The data will vary
			//  depending on which event has been received
			TCHAR tcTarget[MAX_URL + 1];
			memset (tcTarget, 0, sizeof(TCHAR) * MAX_URL + 1);
			switch (pnmh->code)
			{
			case NM_PIE_TITLE:
				//  The Page Title has been received from the Page, convert 
				//  the content to a wide string
				if (pnmHTML->szTarget)
					mbstowcs(tcTarget, (LPSTR)pnmHTML->szTarget, MAX_URL);
				if (tcTarget)
					mobileTab->InvokeEngineEventTitleChange(tcTarget);
				break;
			case NM_PIE_META:
				//  A Meta Tag has been received from the Page, convert the content
				//  and data to wide strings.
				if (pnmHTML->szTarget)
					mbstowcs(tcTarget, (LPSTR)pnmHTML->szTarget, MAX_URL);
				TCHAR tcData[MAX_URL+1];
				memset(tcData, 0, sizeof(TCHAR) * MAX_URL + 1);
				if (pnmHTML->szData)
					mbstowcs(tcData, (LPSTR)pnmHTML->szData, MAX_URL);
				//  If there is both an HTTP Equiv and some Content to the Meta
				//  tag then invoke it
				//if (tcTarget && tcData)
				//	mobileTab->InvokeEngineEventMetaTag(tcTarget, tcData);			
				break;
			case NM_PIE_BEFORENAVIGATE:
				if (pnmHTML->szTarget)
					mbstowcs(tcTarget, (LPSTR)pnmHTML->szTarget, MAX_URL);

				// GD - stop navigation if target starts with file:// and ends with '\'.
				// This is the generated target when using <a href=""> from a local page.
				// If we don't stop it then the File Explorer is launched.
				if (wcslen (tcTarget) >= 8)
					if (!wcsnicmp (tcTarget, L"file://", 7))
						if (tcTarget [wcslen (tcTarget) - 1] == '\\')
						{
							LOG(TRACE) + "Navigation to file folder aborted\n";
							return S_FALSE;
						}
				//if (tcTarget)
				//	mobileTab->InvokeEngineEventLoad(tcTarget, EEID_BEFORENAVIGATE);
				break;
			case NM_PIE_DOCUMENTCOMPLETE:
				if (pnmHTML->szTarget)
					mbstowcs(tcTarget, (LPSTR)pnmHTML->szTarget, MAX_URL);
				//  If the network is available but the server being reached
				//  is inaccessible the browser component appears to immediately
				//  give a document complete with the current page URL (not the
				//  page being navigated to) which is hiding the hourglass, 
				//  stop this behaviour.
				//if (tcTarget && !wcsicmp(tcTarget, mobileTab->m_tcNavigatedURL))
				//	mobileTab->InvokeEngineEventLoad(tcTarget, EEID_DOCUMENTCOMPLETE);
				break;
			case NM_PIE_NAVIGATECOMPLETE:
				if (pnmHTML->szTarget)
					mbstowcs(tcTarget, (LPSTR)pnmHTML->szTarget, MAX_URL);
				//if (tcTarget)
				//	mobileTab->InvokeEngineEventLoad(tcTarget, EEID_NAVIGATECOMPLETE);
				break;
			case NM_PIE_KEYSTATE:
			case NM_PIE_ALPHAKEYSTATE:
				//  Not Used
				break;

			}
		}	
//		lResult = DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	return lResult;
}

DWORD WINAPI CIEBrowserEngine::NavigationTimeoutThread( LPVOID lpParameter )
{
	LOG(TRACE) + "Mobile NavThread Started\n";

	CIEBrowserEngine* pIEEng = reinterpret_cast<CIEBrowserEngine*>(lpParameter);

	if(pIEEng->m_hNavigated==NULL)
		pIEEng->m_hNavigated = CreateEvent(NULL, TRUE, FALSE, L"PB_IEENGINE_NAVIGATION_IN_PROGRESS");

	if(WaitForSingleObject(pIEEng->m_hNavigated, pIEEng->m_dwNavigationTimeout) != WAIT_OBJECT_0)
	{
		//no point in doing anything as there is no event handler
		pIEEng->StopOnTab(0);
		CloseHandle(pIEEng->m_hNavigated);
		pIEEng->m_hNavigated = NULL;
	
		//if(pIEEng->m_EngineEvents[EEID_NAVIGATIONTIMEOUT])
		//	pIEEng->m_EngineEvents[EEID_NAVIGATIONTIMEOUT](EEID_NAVIGATIONTIMEOUT, (LPARAM)pIEEng->m_tcNavigatedURL, pIEEng->m_tabID);
	}

	LOG(TRACE) + "NavThread Ended\n";

	return 0;
}