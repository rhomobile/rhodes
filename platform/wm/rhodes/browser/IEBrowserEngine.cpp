#include "stdafx.h"

#include <windows.h>
#include <wininet.h>

#include "htmlctrl.h"

#include "IEBrowserEngine.h"
#include "common/RhoConf.h"
#include "MainWindow.h"

IMPLEMENT_LOGCLASS(CIEBrowserEngine,"IEBrowser");

extern "C" const wchar_t* rho_wmimpl_getNavTimeOutVal();
extern "C" HWND rho_wmimpl_get_mainwnd();
extern "C" LRESULT rho_wm_appmanager_ProcessOnTopMostWnd( WPARAM wParam, LPARAM lParam );
extern "C" void rho_wmimpl_create_ieBrowserEngine(HWND hwndParent, HINSTANCE rhoAppInstance);
extern "C" bool rho_wmimpl_get_textselectionenabled();

//Default Text Zoom Value
#define ZOOM_TEXT_MEDIUM 2 ///< The text zoom value is in the range 0 to 4.

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

HWND              CIEBrowserEngine::g_hwndTabHTMLContainer = NULL;
CIEBrowserEngine* CIEBrowserEngine::g_hInstance = NULL;

//////////////////////////////////////////////////////////////////////////

CIEBrowserEngine* CIEBrowserEngine::getInstance()
{
    return g_hInstance;
}

CIEBrowserEngine* CIEBrowserEngine::getInstance(HWND hParentWnd, HINSTANCE hInstance)
{
    if (!g_hInstance)
    {
        g_hInstance = new CIEBrowserEngine(hParentWnd, hInstance);
    }

    return g_hInstance;
}

//////////////////////////////////////////////////////////////////////////

CIEBrowserEngine::CIEBrowserEngine(HWND hParentWnd, HINSTANCE hInstance) :
        m_parentHWND(NULL),
        m_hparentInst(NULL),
        m_bLoadingComplete(FALSE),
        m_hNavigated(NULL),
        m_dwNavigationTimeout(45000),
        m_urlList(NULL),
	m_currentPage(NULL)
{
    m_parentHWND = hParentWnd;    
    m_hparentInst = hInstance;
    m_tabID = 0; //tabID;

	m_textZoomValue = ZOOM_TEXT_MEDIUM; //By default the text zoom value is set to ZOOM_TEXT_MEDIUM

    GetWindowRect(hParentWnd, &m_rcViewSize);

    m_tcNavigatedURL[0] = 0;
	//TODO TAU
	//convertFromStringW(rho_wmimpl_getNavTimeOutVal(),m_dwNavigationTimeout);
	if(m_dwNavigationTimeout<=0)
	{
		LOG(WARNING)+" NavigationTimeout  value  from config.xml not correct "+m_dwNavigationTimeout;
		m_dwNavigationTimeout=45000;
	}
    CreateEngine();
}

CIEBrowserEngine::~CIEBrowserEngine()
{
    //  Destroy the Browser Object
    DestroyWindow(m_hwndTabHTML);
    m_hwndTabHTML = NULL;
    DeleteCascade(m_urlList);
    //  Destroy the Browser Object's parent if it exists
    if (g_hwndTabHTMLContainer)
    {
        DestroyWindow(g_hwndTabHTMLContainer);
        g_hwndTabHTMLContainer = NULL;
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
    if (g_hwndTabHTMLContainer == NULL)
    {
        //register the main window
        if (FAILED(RegisterWindowClass(m_hparentInst, &CIEBrowserEngine::WndProc)))
            return S_FALSE;

        //create the main window
        g_hwndTabHTMLContainer = CreateWindowEx( 0, HTML_CONTAINER_NAME, NULL, 
            WS_POPUP | WS_VISIBLE, 
            CW_USEDEFAULT, CW_USEDEFAULT, 
            CW_USEDEFAULT, CW_USEDEFAULT, 
            m_parentHWND, NULL, m_hparentInst, 0);

        if(!g_hwndTabHTMLContainer)
            return S_FALSE;


        if (FAILED(rho::browser::InitHTMLControl(m_hparentInst)))
            return S_FALSE;
    }

	DWORD dwTextSelectionStyle = 0;
    //TODO TAU
	//if(!rho_wmimpl_get_textselectionenabled()) 
	//	dwTextSelectionStyle = HS_NOSELECTION;
	dwTextSelectionStyle = HS_NOSELECTION;

    m_hwndTabHTML = CreateWindow(WC_HTML, NULL, 
        WS_POPUP | WS_VISIBLE | dwTextSelectionStyle, 
        m_rcViewSize.left, m_rcViewSize.top, 
        (m_rcViewSize.right-m_rcViewSize.left), 
        (m_rcViewSize.bottom-m_rcViewSize.top), 
        g_hwndTabHTMLContainer, NULL, m_hparentInst, NULL);    

    if (!m_hwndTabHTML)
        return S_FALSE;

    CloseHandle (CreateThread(NULL, 0, 
        &CIEBrowserEngine::RegisterWndProcThread, (LPVOID)this, 0, NULL));

    return S_OK;
}

BOOL CIEBrowserEngine::Navigate(LPCTSTR tcURL, int iTabID)
{
    //  On Windows Mobile devices it has been observed that attempting to 
    //  navigate to a Javascript function before the page is fully loaded can 
    //  crash PocketBrowser (specifically when using Reload).  This condition
    //  prevents that behaviour.
    
    if (!m_bLoadingComplete && (wcsnicmp(tcURL, L"JavaScript:", wcslen(L"JavaScript:")) == 0))
    {
        LOG(TRACE) + "Failed to Navigate, Navigation in Progress\n";
        return S_FALSE;
    }

    LRESULT retVal = S_FALSE;

    if (wcsicmp(tcURL, L"history:back") == 0)
    {
		TCHAR tcPreviousURL[MAX_URL];
		LOG(INFO) + "\nDRD history:back tcURL="+tcURL;
		GetPreviousUrl(tcPreviousURL);
		Navigate(tcPreviousURL, iTabID);
		return S_OK;
    }

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
    

    return retVal;
}

BOOL CIEBrowserEngine::ResizeOnTab(int iInstID,RECT rcNewSize)
{    
    m_rcViewSize = rcNewSize;

    if(!m_bLoadingComplete)
        return TRUE;

    if (MoveWindow(m_hwndTabHTML,
                   m_rcViewSize.left, 
                   m_rcViewSize.top, 
                  (m_rcViewSize.right-m_rcViewSize.left), 
                  (m_rcViewSize.bottom-m_rcViewSize.top), 
                   FALSE))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

BOOL CIEBrowserEngine::BackOnTab(int iInstID,int iPagesBack /*= 1*/)
{
    return Navigate(L"history:back", 0);
}

BOOL CIEBrowserEngine::ForwardOnTab(int iInstID)
{
    int iPagesForward = 1;
    //  iPreviousPages must be greater than 0, otherwise we're not doing anything
    if (iPagesForward == 0)
	return TRUE;

    //  Check to see we can go forward this many pages
    int iHistoryCounter = 0;
    CHistoryElement* tempHistoryElement = m_currentPage;
    while(tempHistoryElement != NULL && tempHistoryElement->pNext != NULL)
    {
    	//  We can go to the previous page
		iHistoryCounter++;
		//  Go forward another item in the history
		tempHistoryElement = tempHistoryElement->pNext;
		//  If we have gone forward the specified number of times navigate to the 
		//  page
		if (iPagesForward == iHistoryCounter)
		{			
			m_currentPage = tempHistoryElement;
			Navigate(m_currentPage->tcURL, iInstID);			
		}
    }
    return TRUE;
}

BOOL CIEBrowserEngine::ReloadOnTab(bool bFromCache, UINT iTab)
{
    if(m_currentPage!=NULL)
    	Navigate(m_currentPage->tcURL,iTab);
    return TRUE; 
}

BOOL CIEBrowserEngine::StopOnTab(UINT iTab)
{
    return SendMessage(m_hwndTabHTML, DTM_STOP, 0, 0);
}

BOOL CIEBrowserEngine::ZoomPageOnTab(float fZoom, UINT iTab)
{
    return SendMessage(m_hwndTabHTML, DTM_ZOOMLEVEL, 0, (LPARAM)(DWORD)m_textZoomValue);
}

BOOL CIEBrowserEngine::GetTitleOnTab(LPTSTR szURL, UINT iMaxLen, UINT iTab)
{
    return FALSE;
}

void CIEBrowserEngine::RunMessageLoop(CMainWindow& mainWnd)
{
	MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
		if (RHODESAPP().getExtManager().onWndMsg(msg) )
            continue;

		IDispatch* pDisp;
		SendMessage(m_hwndTabHTML, DTM_BROWSERDISPATCH, 0, (LPARAM) &pDisp); // New HTMLVIEW message
		if (pDisp != NULL) {
			//  If the Key is back we do not want to translate it causing the browser
			//  to navigate back.
			if ( ((msg.message != WM_KEYUP) && (msg.message != WM_KEYDOWN)) || (msg.wParam != VK_BACK) )
			{
				IOleInPlaceActiveObject* pInPlaceObject;
				pDisp->QueryInterface( IID_IOleInPlaceActiveObject, (void**)&pInPlaceObject );
				HRESULT handleKey = pInPlaceObject->TranslateAccelerator(&msg);	
			}
		}

        if (!mainWnd.TranslateAccelerator(&msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

		if(msg.message == WM_PAINT)
			RHODESAPP().getExtManager().onHTMLWndMsg(msg);	
    }
}

void CIEBrowserEngine::executeJavascript(const wchar_t* szJSFunction, int index)
{
    //  Test to see if the passed function starts with "JavaScript:" and 
    //  if it does not then prepend it.
    if (_memicmp(szJSFunction, L"JavaScript:", 22))
    {
        //  Function does not start with JavaScript:
        TCHAR* tcURI = new TCHAR[MAX_URL];
        wsprintf(tcURI, L"JavaScript:%s", szJSFunction);
        LRESULT retVal;
        retVal = Navigate(tcURI, 0);
        delete[] tcURI;
    }
    else
    {
        Navigate(szJSFunction, 0);
    }
}

void CIEBrowserEngine::OnDocumentComplete(LPCTSTR url)
{
    if(!m_bLoadingComplete && wcscmp(url,_T("about:blank")) != 0)
    {
        m_bLoadingComplete = true;

        ResizeOnTab(0, m_rcViewSize);
    }
}

void CIEBrowserEngine::setNavigationTimeout(unsigned int dwMilliseconds)
{
    m_dwNavigationTimeout = dwMilliseconds;
}

HWND CIEBrowserEngine::GetHTMLWND(int /*iTabID*/)
{
    return m_hwndTabHTML;
}

LRESULT CIEBrowserEngine::OnWebKitMessages(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//TODO TAU
	/*
    bHandled = TRUE;

    switch (uMsg) 
    {
	case PB_WINDOW_RESTORE:
		{			
			short m_PB_WINRESTORED = 6;
            RHODESAPP().getExtManager().OnWindowChanged((LPVOID) m_PB_WINRESTORED);
		}
		break;
    case PB_ONMETA:
        {
            EngineMETATag* metaTag2 = (EngineMETATag*)lParam;

            RHODESAPP().getExtManager().onSetPropertiesData( (LPCWSTR)wParam, (LPCWSTR)lParam );
            
            if (metaTag2 && wcscmp(metaTag2->tcHTTPEquiv, L"initialiseRhoElementsExtension") != 0)
            {
                free(metaTag2->tcHTTPEquiv);
                free(metaTag2->tcContents);
                delete metaTag2;
            }
        }
        break;
    case PB_ONTOPMOSTWINDOW:
        LOG(INFO) + "START PB_ONTOPMOSTWINDOW";
        LRESULT rtRes = rho_wm_appmanager_ProcessOnTopMostWnd(wParam, lParam);
        LOG(INFO) + "END PB_ONTOPMOSTWINDOW";
        return rtRes;
        break;
    }
*/
    return 0;
}

//////////////////////////////////////////////////////////////////////////
// invoke methods

void CIEBrowserEngine::InvokeEngineEventMetaTag(LPTSTR tcHttpEquiv, LPTSTR tcContent)
{
    EngineMETATag metaTag;
    LRESULT lRet;

    EngineMETATag *pMeta = new EngineMETATag;
	pMeta->tcHTTPEquiv = tcHttpEquiv;
	pMeta->tcContents = tcContent;

    if(pMeta->tcHTTPEquiv && *pMeta->tcHTTPEquiv){
        metaTag.tcHTTPEquiv = new WCHAR[wcslen(pMeta->tcHTTPEquiv)+1];
        wcscpy(metaTag.tcHTTPEquiv,pMeta->tcHTTPEquiv);
    }

    if(pMeta->tcContents && *pMeta->tcContents){
        metaTag.tcContents = new WCHAR[wcslen(pMeta->tcContents)+1];
        wcscpy(metaTag.tcContents,pMeta->tcContents);
    }

    EngineMETATag* metaTag2 = new EngineMETATag;
    metaTag2->tcHTTPEquiv = _tcsdup(metaTag.tcHTTPEquiv);
    metaTag2->tcContents = _tcsdup(metaTag.tcContents);
    lRet = PostMessage(rho_wmimpl_get_mainwnd(),PB_ONMETA,(WPARAM)m_tabID, (LPARAM)metaTag2);

    delete [] metaTag.tcHTTPEquiv;
    delete [] metaTag.tcContents;
}

void CIEBrowserEngine::InvokeEngineEventLoad(LPTSTR tcURL, EngineEventID eeEventID)
{
	//  Engine component has indicated a load event, this should be 
	//  one of BeforeNavigate, NavigateComplete or DocumentComplete.
	wcscpy(m_tcNavigatedURL, tcURL);

	switch (eeEventID)
	{
		case EEID_BEFORENAVIGATE:
			m_bLoadingComplete = FALSE;
			if(m_hNavigated==NULL)
				m_hNavigated = CreateEvent(NULL, TRUE, FALSE, L"PB_IEENGINE_NAVIGATION_IN_PROGRESS");
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

            PostMessage(m_parentHWND, WM_BROWSER_ONBEFORENAVIGATE, (WPARAM)m_tabID, (LPARAM)_tcsdup(tcURL));
			break;
		case EEID_DOCUMENTCOMPLETE:
			m_bLoadingComplete = TRUE;
            PostMessage(m_parentHWND, WM_BROWSER_ONDOCUMENTCOMPLETE, m_tabID, (LPARAM)_tcsdup(tcURL));
			break;
		case EEID_NAVIGATECOMPLETE:
			SetEvent(m_hNavigated);
			CloseHandle(m_hNavigated);
			m_hNavigated = NULL;
            		SendMessage(m_parentHWND, WM_BROWSER_ONNAVIGATECOMPLETE, (WPARAM)m_tabID, (LPARAM)_tcsdup(tcURL));
        		AddNewUrl(tcURL);
			break;
	}
}

void CIEBrowserEngine::InvokeEngineEventTitleChange(LPTSTR tcTitle)
{
	////  Notify the core the page title has changed, if not specified between 
	////  <TITLE> </TITLE> tags this should be set to the page URL.
    TCHAR tcURL[MAX_URL];
    memset(tcURL, 0, sizeof(MAX_URL) * sizeof(TCHAR));
    wcsncpy(tcURL, tcTitle, MAX_URL);

    SendMessage(m_parentHWND, WM_BROWSER_ONTITLECHANGE, (WPARAM)m_tabID, (LPARAM)tcURL);
}

//////////////////////////////////////////////////////////////////////////
// system handlers

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		October 2009
*/
LRESULT CIEBrowserEngine::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult = S_OK;

	bool m_bFullScreen = RHOCONF().getBool("full_screen");

    switch (uMsg) 
	{
		case WM_KILLFOCUS:
		{
			if (m_bFullScreen)
			{
				HWND taskbarWnd = FindWindow(L"HHTaskBar", NULL);
				ShowWindow(taskbarWnd, SW_HIDE);
				return FALSE;
			}
		}
		case WM_NOTIFY:
		{
			//  Received a message from the Pocket Internet Explorer component
			//  to indicate something has happened (BeforeNavigate / DocumentComplete etc).
			//  The lParam contains an NM_HTMLVIEWW containing information about
			//  the event, parse it.
			NM_HTMLVIEWW * pnmHTML;
			LPNMHDR pnmh;
			pnmHTML = (NM_HTMLVIEWW *) lParam;
			pnmh    = (LPNMHDR) &(pnmHTML->hdr);
			
			//  The message has originated from one of the tabs we created, determine
			//  the tab ID from the hwnd
			//IETab* tab = GetSpecificTab(hwndParentOfOriginator);
            CIEBrowserEngine* mobileTab = CIEBrowserEngine::getInstance();

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
				if (tcTarget && tcData)
					mobileTab->InvokeEngineEventMetaTag(tcTarget, tcData);			
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
				if (tcTarget)
					mobileTab->InvokeEngineEventLoad(tcTarget, EEID_BEFORENAVIGATE);
				break;
			case NM_PIE_DOCUMENTCOMPLETE:
				if (pnmHTML->szTarget)
					mbstowcs(tcTarget, (LPSTR)pnmHTML->szTarget, MAX_URL);
				//  If the network is available but the server being reached
				//  is inaccessible the browser component appears to immediately
				//  give a document complete with the current page URL (not the
				//  page being navigated to) which is hiding the hourglass, 
				//  stop this behaviour.
				if (tcTarget && !wcsicmp(tcTarget, mobileTab->m_tcNavigatedURL))
					mobileTab->InvokeEngineEventLoad(tcTarget, EEID_DOCUMENTCOMPLETE);
				break;
			case NM_PIE_NAVIGATECOMPLETE:
				if (pnmHTML->szTarget)
					mbstowcs(tcTarget, (LPSTR)pnmHTML->szTarget, MAX_URL);
				if (tcTarget)
					mobileTab->InvokeEngineEventLoad(tcTarget, EEID_NAVIGATECOMPLETE);

				break;
			case NM_PIE_KEYSTATE:
			case NM_PIE_ALPHAKEYSTATE:
				//  Not Used
				break;

			}
		}	

		lResult = DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	return lResult;
}

DWORD WINAPI CIEBrowserEngine::NavigationTimeoutThread( LPVOID lpParameter )
{
    CIEBrowserEngine* pIEEng = reinterpret_cast<CIEBrowserEngine*>(lpParameter);
    DWORD dwWaitResult;
    
    bool flag=false;
    HWND authwindowhandle;
    if (pIEEng->m_dwNavigationTimeout != 0)
    {
        LOG(TRACE) + "Mobile NavThread Started\n";
        
        	do
        	{

			dwWaitResult = WaitForSingleObject(pIEEng->m_hNavigated, pIEEng->m_dwNavigationTimeout);

			switch (dwWaitResult) 
			{
				// Event object was signaled
				case WAIT_OBJECT_0: 
					//
					// TODO: Read from the shared buffer
					//
					LOG(INFO) + "NavigationTimeoutThread:Event object was signaled\n";
									
					CloseHandle(pIEEng->m_hNavigated);
					pIEEng->m_hNavigated = NULL;
					flag=false;
					break; 
				case WAIT_TIMEOUT: 
					//
					// TODO: Read from the shared buffer
					//
					LOG(INFO) + "NavigationTimeoutThread:timeout\n";
				
					HWND currentForeGroundWindowHandle;
					currentForeGroundWindowHandle = GetForegroundWindow();
					wchar_t szBuf[200];
					if(currentForeGroundWindowHandle!=NULL)
					{
					GetWindowText(currentForeGroundWindowHandle,szBuf,199);
					LOG(INFO) + szBuf;
					}
					
					authwindowhandle = FindWindow(NULL,L"Enter Network Password");
	
					if(authwindowhandle)
					{
					LOG(INFO) + "Authentication window present";
					LOG(INFO) + "NavigationTimeoutThread:Authentication popup\n";
					flag=true;
					break;
					}
					else
					{
					flag=false;
					LOG(INFO) + "NavigationTimeoutThread:Navigation Timed out\n";
					LOG(INFO) + "Authentication window not present";
					}
					pIEEng->StopOnTab(0);
						
					CloseHandle(pIEEng->m_hNavigated);
					pIEEng->m_hNavigated = NULL;
					SendMessage(pIEEng->m_parentHWND, WM_BROWSER_ONNAVIGATIONTIMEOUT, 
					(WPARAM)pIEEng->m_tabID, (LPARAM)_tcsdup(pIEEng->m_tcNavigatedURL));

					break; 

				// An error occurred
				default: 
					LOG(INFO) + "Wait error  GetLastError()=\n"+ GetLastError();
					flag=false;
					return 0; 
			}
        	

			 LOG(TRACE) + "NavThread Ended\n";
        	}while(flag);
    }

	return 0;
}
BOOL CIEBrowserEngine::AddNewUrl(LPCTSTR urlNew)
{

	CHistoryElement* newElement = new CHistoryElement();
	newElement->pNext = NULL;
	newElement->pPrev = NULL;
	newElement->tcURL = new TCHAR[wcslen(urlNew)+1];
	wcscpy(newElement->tcURL, urlNew);

	//  Base Case, there is no current URL History
	if (m_urlList == NULL)
	{
		//  Start the History List
		m_urlList = newElement;
		m_currentPage = newElement;
	}
	else
	{
		//  There is already at least one item in the history
		//  Check we're not trying to add the same item to the history
		//  twice (Reload)
		if(!wcscmp(m_currentPage->tcURL, urlNew))
		{
			LOG(INFO) + "\nDRD Check we're not trying to add the same item to the history twice";
			delete[] newElement->tcURL;
			delete newElement;
			return FALSE;
		}

		//  Check the History hasn't grown too large
		//  Assume the Maximum history size is sensible, suggest a value of 50
		if (BackListSize() >= MAX_HISTORY && MAX_HISTORY > 2)
		{
			//  History will be too large after the next element is added
			//  Remove the first element in the history list and free the memory
			CHistoryElement* firstElement = m_urlList;
			m_urlList = m_urlList->pNext;
			m_urlList->pPrev = NULL;
			delete[] firstElement->tcURL;
			delete firstElement;
		}

		//  Delete all history items FORWARD of the currentPage
		DeleteCascade(m_currentPage->pNext);

		//  Add the new history item to the List
		m_currentPage->pNext = newElement;
		newElement->pPrev = m_currentPage;
		m_currentPage = newElement;
	}
	return TRUE;
}


void CIEBrowserEngine::DeleteCascade(CHistoryElement* fromThisElementOn)
{
	CHistoryElement* deletingElement = fromThisElementOn;
	while (deletingElement != NULL)
	{
		delete[] deletingElement->tcURL;
		deletingElement->tcURL = NULL;
		CHistoryElement* nextElement = deletingElement->pNext;
		delete deletingElement;
		deletingElement = nextElement;
	}
}


LRESULT  CIEBrowserEngine::GetPreviousUrl(LPTSTR tcURL)
{


	//  Check to see we can go back this many pages
	CHistoryElement* tempHistoryElement = m_currentPage;
	if(tempHistoryElement != NULL && tempHistoryElement->pPrev != NULL)
	{
		//  We can go to the previous page
		//  Go back another item in the history
		tempHistoryElement = tempHistoryElement->pPrev;


			m_currentPage = tempHistoryElement;
			_tcscpy(tcURL,m_currentPage->tcURL);
			return S_OK;


	}

	//  If we exit the While loop we were not able to go back the 
	//  specified number of places
	return S_FALSE;
}



UINT CIEBrowserEngine::BackListSize()
{
	UINT iHistoryCounter = 1;
	CHistoryElement* tempHistoryElement = m_currentPage;
	while (tempHistoryElement != NULL && tempHistoryElement->pPrev != NULL)
	{
		//  We can go back
		iHistoryCounter++;
		tempHistoryElement = tempHistoryElement->pPrev;
	}
	return iHistoryCounter;
}
int CIEBrowserEngine::GetTextZoomOnTab(UINT iTab)
{
	return m_textZoomValue;
}
BOOL CIEBrowserEngine::ZoomTextOnTab(int nZoom, UINT iTab)
{
	m_textZoomValue = nZoom;
	BOOL bRetVal = PostMessage(m_hwndTabHTML, DTM_ZOOMLEVEL, 0, 
								(LPARAM)(DWORD) nZoom);

	if (bRetVal)
	{
		//m_dwCurrentTextZoomLevel = dwZoomLevel;
		return S_OK;
	}
	else
		return S_FALSE;
}

#define  PB_ENGINE_IE_MOBILE

DWORD WINAPI CIEBrowserEngine::RegisterWndProcThread(LPVOID lpParameter)
{
    //  We are passed a pointer to the engine we are interested in.
    CIEBrowserEngine* pEngine = (CIEBrowserEngine*)lpParameter;

    //  The window tree appears as follows on CE:
    //  +--m_htmlHWND
    //     |
    //     +--child of m_htmlHWND
    //        |
    //        +--child which receives Windows Messages

    //  The window tree appears as follows on WM:
    //  +--m_htmlHWND
    //     |
    //     +--child of m_htmlHWND which receives Windows Messages

    //  Obtain the created HTML HWND
    HWND hwndHTML = NULL;
    HWND hwndHTMLMessageWindow = NULL;

    while (hwndHTMLMessageWindow == NULL)
    {
        hwndHTML = pEngine->GetHTMLWND(0);

        if (hwndHTML != NULL)
        {
            hwndHTMLMessageWindow = GetWindow(hwndHTML, GW_CHILD);
        }

        //  Failed to find the desired child window, take a short nap before 
        //  trying again as the window is still creating.
        Sleep(100);
    }

    PostMessage(rho_wmimpl_get_mainwnd(), PB_ONTOPMOSTWINDOW,(LPARAM)0, (WPARAM)hwndHTMLMessageWindow);

    return 0;
}
