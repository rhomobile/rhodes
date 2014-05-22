#include "stdafx.h"


#if defined(OS_PLATFORM_MOTCE)

#include <atlbase.h>

#include "CEBrowserEngine.h"
#include "common/RhoConf.h"
#include "MainWindow.h"
#include "EngineDefines.h"

#if defined(_WIN32_WCE)
#include <webvw.h>
#endif
//////////////////////////////////////////////////////////////////////////

extern "C" HWND rho_wmimpl_get_mainwnd();

IMPLEMENT_LOGCLASS(CEBrowserEngine,"CEBrowser");

//////////////////////////////////////////////////////////////////////////

UINT WM_BROWSER_ONDOCUMENTCOMPLETE      = ::RegisterWindowMessage(L"RHODES_WM_BROWSER_ONDOCUMENTCOMPLETE");
UINT WM_BROWSER_ONNAVIGATECOMPLETE      = ::RegisterWindowMessage(L"RHODES_WM_BROWSER_ONNAVIGATECOMPLETE");
UINT WM_BROWSER_ONTITLECHANGE           = ::RegisterWindowMessage(L"RHODES_WM_BROWSER_ONTITLECHANGE");
UINT WM_BROWSER_ONBEFORENAVIGATE        = ::RegisterWindowMessage(L"RHODES_WM_BROWSER_ONBEFORENAVIGATE");
UINT WM_BROWSER_ONNAVIGATIONTIMEOUT     = ::RegisterWindowMessage(L"RHODES_WM_BROWSER_ONNAVIGATIONTIMEOUT");
UINT WM_BROWSER_ONNAVIGATIONERROR       = ::RegisterWindowMessage(L"RHODES_WM_BROWSER_ONNAVIGATIONERROR");
UINT WM_BROWSER_ONSETSIPSTATE           = ::RegisterWindowMessage(L"RHODES_WM_BROWSER_ONSETSIPSTATE");
UINT WM_BROWSER_ONALERTPOPUP            = ::RegisterWindowMessage(L"WM_BROWSER_ONALERTPOPUP");
UINT WM_BROWSER_ONAUTHENTICATIONREQUEST = ::RegisterWindowMessage(L"WM_BROWSER_ONAUTHENTICATIONREQUEST");

//////////////////////////////////////////////////////////////////////////

CEBrowserEngine::CEBrowserEngine(HWND hwndParent, HINSTANCE hInstance)
    : m_ulRefs(0)
    , m_bInPlaceActive(true)
    , m_pBrowser(NULL)
    , m_hNavigated(NULL)
    , m_hwndTabHTML(NULL)
    , bRunningOnWM(FALSE)
    , bDeviceCausesDoubleBackspace(FALSE)
    , m_tabID(0)
    , m_dwNavigationTimeout(30*1000)
    , m_bLoadingComplete(FALSE)
{
	m_hwndParent  = hwndParent;
	m_hInstance = hInstance;

	memset(m_tcCurrentPageTitle, NULL, sizeof(TCHAR) * MAX_URL);
	memset(m_tcNavigatedURL, 0, sizeof(TCHAR) * MAX_URL);

	GetWindowRect(hwndParent, &m_rcViewSize);
    CreateEngine();
}

CEBrowserEngine::~CEBrowserEngine(void)
{
	//destroy the browser window
	DestroyWindow(m_hwndTabHTML);
	m_hwndTabHTML = NULL;
}

LRESULT CEBrowserEngine::CreateEngine()
{
	IUnknown		*pUnk = NULL,		/// pointer to an IUknown interface object
					*pUnk2 = NULL;		/// pointer to an IUknown interface object

	IOleObject		*pObject = NULL;	/// IOleObject object pointer
	DWORD			dwFlags;			/// shdocvw client flags
	IClassFactory	*ppv;				/// pointer variable to hold WebBrowser interface pointer

	//we only want to do this once, so check that the m_pBrowser is null
	if (!m_pBrowser)
	{
		// See if text selection is enabled, so we can pass the correct value through the GetHostInfo() interface
		//configFunction(m_tabID, L"HTMLStyles\\TextSelectionEnabled", tcConfigSetting);
		m_bTextSelectionEnabled = FALSE; //(tcConfigSetting [0] == L'1');

		// Create an instance of a web browser object (from Shdocvw.dll).
		
		//get the interface pointer
		ppv = NULL;
		if (S_OK != CoGetClassObject(CLSID_WebBrowser, CLSCTX_INPROC_SERVER | CLSCTX_INPROC_HANDLER, NULL,IID_IClassFactory, (LPVOID *)(&ppv))) 
			return S_FALSE;

		//create an uninitialized object of a WebBrowser 
		if (S_OK != ppv->CreateInstance(NULL, IID_IUnknown, (LPVOID*)&pUnk))
			return S_FALSE;
		
		// Retrieve an IOleObject interface to set up OLE activation.
		if (S_OK != pUnk->QueryInterface(IID_IOleObject, (LPVOID *)(&pObject))) 
			goto Cleanup;

		// Check if Shdocvw requires a client site to be created first.
		if (S_OK != pObject->GetMiscStatus(DVASPECT_CONTENT, &dwFlags)) 
			goto Cleanup;

		if (dwFlags & OLEMISC_SETCLIENTSITEFIRST)
		{
			// Get an IOleClientSite instance from the browser and pass it to the browser.
			IOleClientSite *pClientSite;
			if (S_OK != QueryInterface(IID_IOleClientSite, (LPVOID *)(&pClientSite))) 
				goto Cleanup;

			if (S_OK != pObject->SetClientSite(pClientSite)) 
				goto Cleanup;

			pClientSite->Release();
		}

		// Activate the object. 
		// Store off the IOleObject reference.
		m_pObject = pObject;
		pObject->AddRef();

		// Retrieve the IWebBrowser2 interface from the IOleObject.
		if (S_OK != pObject->QueryInterface(IID_IWebBrowser2, (void **)&m_pBrowser)) 
			goto Cleanup;
			
		if (pObject)
			pObject->Release();

		InitEvents();

		//  If this engine is being run on Windows Mobile set a flag used
		//  later to determine the accelerator key behaviour.
		OSVERSIONINFO osvi;
		memset(&osvi, 0, sizeof(OSVERSIONINFO));
		osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
		GetVersionEx(&osvi);
		bRunningOnWM = (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2) ||
		     (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1);

		//  Devices which cause a Double Backspace:
		//  1. MC3190 CE 6 build 3122
		if (osvi.dwMajorVersion == 6 && 
			osvi.dwMinorVersion == 0 &&	osvi.dwBuildNumber >= 3122)
		{
			bDeviceCausesDoubleBackspace = TRUE;
		}
	}

Cleanup:
	if (pUnk)
		pUnk->Release();
	if (pObject)
		pObject->Release();

	IOleWindow *pWnd = NULL;

	if (m_pBrowser)
	{
		if (S_OK !=  m_pBrowser->QueryInterface(IID_IOleWindow, (LPVOID *)(&pWnd)))
			return S_FALSE;
	}

	if (pWnd)
	{
		pWnd->GetWindow(&m_hwndTabHTML);
		pWnd->Release();
	}

	if (pUnk2)
		pUnk2->Release();

	LONG lStyle = GetWindowLong(m_hwndTabHTML, GWL_STYLE);

    lStyle ^= WS_POPUP;
	lStyle |= WS_CHILD;

    SetWindowLong(m_hwndTabHTML, GWL_STYLE, lStyle);
	SetParent(m_hwndTabHTML, m_hwndParent);

	//ShowWindow(m_hwndTabHTML, SW_SHOW);
	//SetForegroundWindow(m_hwndTabHTML);
	//MoveWindow(m_hwndTabHTML, m_rcViewSize.left, m_rcViewSize.top, (m_rcViewSize.right-m_rcViewSize.left), (m_rcViewSize.bottom-m_rcViewSize.top), FALSE);

	return S_OK;		
};

BOOL CEBrowserEngine::Navigate(LPCTSTR tcURL, int iTabID)
{
	LRESULT retVal = S_FALSE;

	if (!tcURL || wcslen(tcURL) == 0)
		return S_FALSE;

    LOG(INFO)  + "Navigate: " + tcURL;

    //m_bLoadingComplete = FALSE;

	if (wcslen(tcURL) >= wcslen(L"history:back") && 
        wcsicmp(tcURL, L"history:back") == 0)
	{
        m_pBrowser->GoBack();
	}
	else
	{		
		TCHAR tcDereferencedURL[MAX_URL];
		memset(tcDereferencedURL, 0, MAX_URL * sizeof(TCHAR));
        if (rho::browser::IsRelativeURL(tcURL))
		{
            if (!rho::browser::DereferenceURL(tcURL, tcDereferencedURL, m_tcNavigatedURL))
				return S_FALSE;
		}
		else
			wcscpy(tcDereferencedURL, tcURL);

		if (tcDereferencedURL[0] == L'/')
		{
			if (wcslen(tcDereferencedURL) <= (MAX_URL - wcslen(L"file://") + 1))
			{
				TCHAR tcNewURL[MAX_URL + 1];
				wsprintf(tcNewURL, L"file://\\%s", tcDereferencedURL + 1);
				if (S_OK == m_pBrowser->Navigate(BSTR(tcNewURL), NULL, NULL, NULL, NULL))
					retVal = S_OK;
				else
					retVal = S_FALSE;
			}
		}
		else
		{
			if (S_OK == m_pBrowser->Navigate(BSTR(tcDereferencedURL), NULL, NULL, NULL, NULL))
				retVal = S_OK;
			else
				retVal = S_FALSE;
		}
	}

    return SUCCEEDED(retVal);
}

BOOL CEBrowserEngine::StopOnTab(UINT iTab)
{
	if(S_OK == m_pBrowser->Stop())
	{
		SetEvent(m_hNavigated);
		CloseHandle(m_hNavigated);
		m_hNavigated = NULL;

        PostMessage(m_hwndParent, WM_BROWSER_ONNAVIGATECOMPLETE, m_tabID, (LPARAM)_tcsdup(L"NavigationStopped"));
        PostMessage(m_hwndParent, WM_BROWSER_ONDOCUMENTCOMPLETE, m_tabID, (LPARAM)_tcsdup(L"NavigationStopped"));

		return TRUE;
	}

	return FALSE;
}

DWORD WINAPI CEBrowserEngine::NavigationTimeoutThread( LPVOID lpParameter )
{
    CEBrowserEngine * pEng = (CEBrowserEngine*) lpParameter;

    if(pEng->m_hNavigated==NULL)
        pEng->m_hNavigated = CreateEvent(NULL, TRUE, FALSE, L"PB_IEENGINE_NAVIGATION_IN_PROGRESS");

    if(WaitForSingleObject(pEng->m_hNavigated, pEng->m_dwNavigationTimeout) != WAIT_OBJECT_0)
    {
        //no point in doing anything as there is no event handler
        pEng->StopOnTab(pEng->m_tabID);
        CloseHandle(pEng->m_hNavigated);
        pEng->m_hNavigated = NULL;

        SendMessage(pEng->m_hwndParent, WM_BROWSER_ONNAVIGATIONTIMEOUT, 
            (WPARAM)pEng->m_tabID, (LPARAM)pEng->m_tcNavigatedURL);
    }

    return 0;
}

HRESULT CEBrowserEngine::InitEvents()
{
	HRESULT                    hr;
	IConnectionPointContainer *pCPCont = NULL;
	DWebBrowserEvents2        *pEvents = NULL;
	DWORD                     _dwEventCookie;

	if (!m_pBrowser)
		return S_FALSE;

	hr = m_pObject->DoVerb(OLEIVERB_UIACTIVATE, NULL, this, 0, m_hwndParent, &m_rcViewSize);
	if (hr == OLE_E_NOT_INPLACEACTIVE)
		return S_FALSE;

	// Get the connection point container for the browser object.
	hr = m_pBrowser->QueryInterface(IID_IConnectionPointContainer, (LPVOID *)&pCPCont);
	if (hr)
		return S_FALSE;

	// Look for DWebBrowserEvents2 connection point.
	hr = pCPCont->FindConnectionPoint(DIID_DWebBrowserEvents2, &m_pCP);
	if (hr)
	{
		m_pCP = NULL;
		goto Cleanup;
	}

	// Get a DWebBrowserEvents2 pointer from the browser.
	hr = QueryInterface(DIID_DWebBrowserEvents2, (LPVOID *)(&pEvents));
	if (hr)
		goto Cleanup;

	// Add your event sink to the connectionpoint.
	hr = m_pCP->Advise(pEvents, &(_dwEventCookie));
	if (hr)
		goto Cleanup;

Cleanup:
	if (pCPCont)
		pCPCont->Release();
	if (pEvents)
		pEvents->Release();

	return S_OK;
}

HRESULT CEBrowserEngine::GetHostInfo( DOCHOSTUIINFO* pInfo )
{
	pInfo->cbSize   = sizeof(DOCHOSTUIINFO);
    pInfo->dwFlags = DOCHOSTUIFLAG_NO3DBORDER;

	if (!m_bTextSelectionEnabled)
		pInfo->dwFlags |= DOCHOSTUIFLAG_DIALOG;

	pInfo->dwDoubleClick = DOCHOSTUIDBLCLK_DEFAULT;

	return S_OK;
}

HRESULT CEBrowserEngine::TranslateAccelerator( 
			LPMSG lpMsg,
			const GUID __RPC_FAR *pguidCmdGroup,
			DWORD nCmdID)
{
	if (lpMsg && (lpMsg->message == WM_KEYDOWN))
	{
		if (lpMsg->wParam == VK_LEFT ||
					lpMsg->wParam == VK_RIGHT ||
					lpMsg->wParam == VK_UP ||
					lpMsg->wParam == VK_DOWN ||
					//lpMsg->wParam == VK_BACK ||
					lpMsg->wParam == VK_RETURN)
		{
			return S_OK;
		}
	}

	if (lpMsg && (lpMsg->message == WM_KEYDOWN))
	{
		if (lpMsg->wParam == VK_BACK)
		{
			return S_OK;
		}
		else if (lpMsg->wParam == VK_LEFT ||
				lpMsg->wParam == VK_RIGHT ||
				lpMsg->wParam == VK_UP ||
				lpMsg->wParam == VK_DOWN)
		{
			return S_OK;  
		}
		else
		{
			return S_FALSE;
		}
	}

	return S_FALSE;
}

HRESULT CEBrowserEngine::QueryInterface (REFIID riid, LPVOID * ppv)
{
	if ((riid == IID_IOleContainer) || (riid == IID_IUnknown))
	{
		*ppv = (IOleContainer *) this;
	}
	else if (riid == IID_IOleClientSite)
	{
		*ppv = (IOleClientSite *)this;
	}
	else if (riid == IID_IOleInPlaceSite)
	{
		*ppv = (IOleInPlaceSite *)this;
	}
	else if (riid == DIID_DWebBrowserEvents2)
	{
		*ppv = (DWebBrowserEvents2 *)this;
	}
	else if (riid == IID_IDocHostShowUI)
	{
   		*ppv = (IDocHostShowUI *)this;
	}
	else if (riid == IID_IDocHostUIHandler)
	{
   		*ppv = (IDocHostUIHandler *)this;
	}
	else
	{
		*ppv = NULL;
		return E_NOINTERFACE;
	}
	
	AddRef();

	return S_OK;
}

ULONG CEBrowserEngine::AddRef(void)
{
	//safely increment the reference counter
	InterlockedIncrement((LONG*)&m_ulRefs);
	return m_ulRefs;
}

ULONG CEBrowserEngine::Release(void)
{
	//safely decrement the reference counter
	if (InterlockedDecrement((LONG*)&m_ulRefs) == 0)
	{
		delete this;
		return 0;
	}
	return m_ulRefs;
}

HRESULT CEBrowserEngine::Scroll (SIZE scrollExtent)
{
	return S_OK;
}

HRESULT CEBrowserEngine::GetWindow(HWND *phwnd)
{
	*phwnd = m_hwndTabHTML;
	return S_OK;
}

HRESULT CEBrowserEngine::Invoke(DISPID dispidMember,
                                 REFIID riid,
                                 LCID lcid,
                                 WORD wFlags,
                                 DISPPARAMS FAR* pdparams, 
                                 VARIANT FAR* pvarResult, 
                                 EXCEPINFO FAR* pexcepinfo,
                                 UINT FAR* puArgErr)
{
	HRESULT retVal = DISP_E_MEMBERNOTFOUND;
	TCHAR *tcURL = new TCHAR[MAX_URL];
	memset(tcURL, NULL, sizeof(TCHAR) * MAX_URL);

	switch (dispidMember) 
	{
	case DISPID_NAVIGATEERROR:
		SetEvent(m_hNavigated);
		CloseHandle(m_hNavigated);
		m_hNavigated = NULL;

		//get the URL which failed
		if (pdparams && pdparams->rgvarg[0].vt == VT_BSTR)
			wcsncpy(tcURL, pdparams->rgvarg[3].pvarVal->bstrVal, MAX_URL-1);
		
        SendMessage(m_hwndParent, WM_BROWSER_ONNAVIGATIONERROR, (WPARAM)m_tabID, (LPARAM)tcURL);
		
        *(pdparams->rgvarg[0].pboolVal) = VARIANT_TRUE;
		retVal = S_OK;

		break;
	
	case DISPID_NAVIGATECOMPLETE2:
		SetEvent(m_hNavigated);
		CloseHandle(m_hNavigated);
		m_hNavigated = NULL;

		if (pdparams && pdparams->rgvarg[0].vt == VT_BSTR) 
		{
			if(pdparams->rgvarg[0].bstrVal)
				wcsncpy(tcURL, pdparams->rgvarg[0].bstrVal, MAX_URL-1);
		}
		else if (pdparams && pdparams->rgvarg[0].vt == (VT_VARIANT|VT_BYREF)) 
		{
			if(pdparams->rgvarg[0].pvarVal->vt == VT_BSTR && pdparams->rgvarg[0].pvarVal->bstrVal)
				wcsncpy(tcURL, pdparams->rgvarg[0].pvarVal->bstrVal, MAX_URL-1);
		}

        SendMessage(m_hwndParent, WM_BROWSER_ONNAVIGATECOMPLETE, (WPARAM)m_tabID, (LPARAM)tcURL);

		retVal = S_OK;
		break;
	case DISPID_TITLECHANGE:
		if (pdparams && pdparams->rgvarg[0].vt == VT_BSTR) 
			wcsncpy(tcURL, pdparams->rgvarg[0].bstrVal, MAX_URL-1);
			wcscpy(m_tcCurrentPageTitle, tcURL);

        SendMessage(m_hwndParent, WM_BROWSER_ONTITLECHANGE, (WPARAM)m_tabID, (LPARAM)tcURL);
        retVal = S_OK;

		break;

	case DISPID_DOCUMENTCOMPLETE:
		//Validate that there is an event handler

		if (pdparams && pdparams->rgvarg[0].vt == VT_BSTR) 
		{
			if(pdparams->rgvarg[0].bstrVal)
				wcsncpy(tcURL, pdparams->rgvarg[0].bstrVal, MAX_URL-1);
		}
		else if (pdparams && pdparams->rgvarg[0].vt == (VT_VARIANT|VT_BYREF)) 
		{
			if(pdparams->rgvarg[0].pvarVal->vt == VT_BSTR && pdparams->rgvarg[0].pvarVal->bstrVal)
				wcsncpy(tcURL, pdparams->rgvarg[0].pvarVal->bstrVal, MAX_URL-1);
		}

		PostMessage(m_hwndParent, WM_BROWSER_ONDOCUMENTCOMPLETE, m_tabID, (LPARAM)_tcsdup(tcURL));

        m_bLoadingComplete = true;
        InvalidateRect(GetHTMLWND(0), NULL, FALSE);
        ParseTags();

		retVal = S_OK;
        
		break;

	case DISPID_BEFORENAVIGATE2:
		if (pdparams && pdparams->rgvarg[5].pvarVal[0].vt == VT_BSTR) 
		{
			if(pdparams->rgvarg[5].pvarVal[0].bstrVal)
				wcsncpy(tcURL, pdparams->rgvarg[5].pvarVal[0].bstrVal, MAX_URL-1);
		}

		if(memcmp(tcURL, L"res://", 12) == 0)
		{
			*(pdparams->rgvarg[0].pboolVal) = VARIANT_TRUE;
			retVal = S_OK;
			break;
		}

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
            m_pBrowser->GoBack();
            break;
		}

		CloseHandle (CreateThread(NULL, 0, &CEBrowserEngine::NavigationTimeoutThread, (LPVOID)this, 0, NULL));
		wcscpy(m_tcNavigatedURL, tcURL);

#ifdef SCROLL_NOTIFY
		// Stop any checking for scroll changes during navigation
		if (pScrollNotify)
		{
			delete pScrollNotify;
			pScrollNotify = NULL;
		}
#endif
        PostMessage(m_hwndParent, WM_BROWSER_ONBEFORENAVIGATE, (WPARAM)m_tabID, (LPARAM)_tcsdup(tcURL));

		retVal = S_OK;
		break;
	}

	delete[] tcURL;
	tcURL = NULL;

	return retVal;
}

HRESULT CEBrowserEngine::OnInPlaceActivate(void)
{
    m_bInPlaceActive = TRUE;
    return S_OK;
}

HRESULT CEBrowserEngine::GetWindowContext(
    LPOLEINPLACEFRAME FAR * lplpFrame, 
    LPOLEINPLACEUIWINDOW FAR *  lplpDoc, 
    LPRECT lprcPosRect, 
    LPRECT lprcClipRect, 
    LPOLEINPLACEFRAMEINFO lpFrameInfo)
{
	int nHeight = 0;
	GetClientRect(m_hwndTabHTML, lprcPosRect);
	GetClientRect(m_hwndTabHTML, lprcClipRect);

	RECT rc = {0,0,0,0};

	return S_OK;
}

HRESULT CEBrowserEngine::OnInPlaceDeactivate(void)
{
	m_bInPlaceActive = FALSE;
	return S_OK;
}

BOOL CEBrowserEngine::ResizeOnTab(int iInstID,RECT rcNewSize) 
{
    m_rcViewSize = rcNewSize;
    if(!m_bLoadingComplete)
        return TRUE;

    if (MoveWindow(m_hwndTabHTML, m_rcViewSize.left, 
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

BOOL CEBrowserEngine::BackOnTab(int iInstID,int iPagesBack) 
{
    if (m_pBrowser)
    {
        HRESULT hr = m_pBrowser->GoBack();
        return SUCCEEDED(hr);
    }

    return FALSE;
}

BOOL CEBrowserEngine::ForwardOnTab(int iInstID) 
{
    if (m_pBrowser)
    {
        return SUCCEEDED(m_pBrowser->GoForward());
    }

    return FALSE;
}

BOOL CEBrowserEngine::ReloadOnTab(bool bFromCache, UINT iTab) 
{
    VARIANT var;
    if(bFromCache)
        var.intVal = REFRESH_NORMAL;
    else 
        var.intVal = REFRESH_COMPLETELY;

    return SUCCEEDED(m_pBrowser->Refresh2(&var));
}

LRESULT CEBrowserEngine::OnWebKitMessages(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) 
{
    bHandled = TRUE;

    switch (uMsg) 
    {
    case PB_ONMETA:
        {
            EngineMETATag* metaTag2 = (EngineMETATag*)lParam;
            rho::browser::MetaHandler(m_tabID, metaTag2);
        }
        break;
    }

    return 0;
}

void CEBrowserEngine::RunMessageLoop(CMainWindow& mainWnd) 
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

bool CEBrowserEngine::isExistJavascript(const wchar_t* szJSFunction, int index) 
{
    TCHAR functionName[MAX_URL];
    memset(functionName, 0, sizeof(functionName));

    TCHAR* functionNamePtr = functionName;
    TCHAR* pParen = wcschr(szJSFunction, '(');
    int posOfFirstParen = pParen-szJSFunction;
    int startPos = 0;

    if (!_memicmp(szJSFunction, L"JavaScript:", 22))
        startPos = 11;

    for (int i=startPos; i < posOfFirstParen; i++)
    {
        //  Populate the function Name
        if (szJSFunction[i] != ' ')
        {
            *functionNamePtr = szJSFunction[i];
            functionNamePtr++;
        }
    }

    OLECHAR FAR* szJSMethodName = (OLECHAR *)szJSFunction;
    LPDISPATCH   pDisp          = (IDispatch FAR*)NULL;
    bool         retVal         = false;

    // Set the IDispatch object to the document loaded in the browser
    m_pBrowser->get_Document(&pDisp);

    if (pDisp != NULL)
    {
        IHTMLDocument2* pHTMLDocument2;
        HRESULT hr = pDisp->QueryInterface( IID_IHTMLDocument2, (void**)&pHTMLDocument2 );

        //  Set the IDispatch pointer to the scripts in the document
        hr = pHTMLDocument2->get_Script(&pDisp);
        //  dispid will hold a reference to the JavaScript function we want to call
        DISPID dispid;

        //  Obtain the id of the JavaScript method with the name 'szJSMethodName'
        hr = pDisp->GetIDsOfNames(IID_NULL, &szJSMethodName, 1, LOCALE_SYSTEM_DEFAULT, &dispid);

        if (hr != DISP_E_UNKNOWNNAME)
        {
            retVal = true;
        }
    }

    return retVal;
}

void CEBrowserEngine::executeJavascript(const wchar_t* szJSFunction, int index) 
{
    if (!isExistJavascript(szJSFunction, index))
    {
        return;
    }
    else
    {
        if (_memicmp(szJSFunction, L"JavaScript:", 22))
        {
            //  Function does not start with JavaScript:
            wchar_t* tcURI = new wchar_t[MAX_URL];
            wsprintf(tcURI, L"JavaScript:%s", szJSFunction);
            Navigate(tcURI, index);
            delete[] tcURI;
        }
        else
        {
            Navigate(szJSFunction, index);
        }
    }
}

void CEBrowserEngine::OnDocumentComplete(LPCTSTR url) 
{
    if(!m_bLoadingComplete && wcscmp(url,_T("about:blank")) != 0)
    {
        m_bLoadingComplete = true;

        ResizeOnTab(0, m_rcViewSize);
    }
}

bool CEBrowserEngine::RegisterForMessage(unsigned int iMsgId)
{
    return true;
}

bool CEBrowserEngine::DeRegisterForMessage(unsigned int iMsgId)
{
    return true;
}

bool CEBrowserEngine::RegisterForPrimaryMessage(unsigned int iMsgId)
{
    return true;
}

bool CEBrowserEngine::DeRegisterForPrimaryMessage(unsigned int iMsgId)
{
    return true;
}

int CEBrowserEngine::NewTab() { return 0; }
int CEBrowserEngine::SwitchTab(int iTabID){ return 0;}
BOOL CEBrowserEngine::CloseTab(int iTabID) { return false; }

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		October 2009
*/
HRESULT CEBrowserEngine::ParseTags()
{
	IDispatch* pDisp;
	m_pBrowser->get_Document(&pDisp);
		
	if (pDisp != NULL )
	{
		IHTMLDocument2* pHTMLDocument2;
		HRESULT hr;
		
		hr = pDisp->QueryInterface( IID_IHTMLDocument2, (void**)&pHTMLDocument2 );
		// Finished with pDisp so release
		pDisp->Release();

		if (hr == S_OK)
		{
			IHTMLElementCollection* pColl;
			hr = pHTMLDocument2->get_all( &pColl );
			
			// Finished so release
			pHTMLDocument2->Release();

			if (hr == S_OK)
			{
				LONG celem;
				hr = pColl->get_length( &celem );

				if ( hr == S_OK )
				{
					for ( int i=0; i< celem; i++ )
					{
						VARIANT varIndex;
						varIndex.vt = VT_UINT;
						varIndex.lVal = i;
						VARIANT var2;
						VariantInit( &var2 );
						IDispatch* pDisp2; 

						hr = pColl->item( varIndex, var2, &pDisp2 );

						if ( hr == S_OK )
						{
							BSTR bstr;
							memset(&bstr, 0, sizeof(BSTR));
							IHTMLMetaElement* pMetaElem;
							hr = pDisp2->QueryInterface( IID_IHTMLMetaElement, 
								(void **)&pMetaElem );
							if ( hr == S_OK )
							{
								//  The engine uses its own definition of 
								//  meta tags, must HTTP Equiv and Contents
								EngineMETATag metaTag;
								memset(&metaTag, 0, sizeof(metaTag));
								TCHAR tcHttpEquiv[MAX_URL], tcContents[MAX_URL];
								memset(tcHttpEquiv, 0, MAX_URL);
								memset(tcContents, 0, MAX_URL);
								//  Obtain the HTTP Equiv from the IE 
								//  component, stored in bstr
								if (S_OK == pMetaElem->get_httpEquiv(&bstr)) 
								{
									if (bstr != 0) 
									{
										//  Copy the HTTP Equiv returned
										//  from the IE Component into our 
										//  Meta Tag Structure
										wcsncpy(tcHttpEquiv, bstr, MAX_URL);
										metaTag.tcHTTPEquiv = tcHttpEquiv;
										::SysFreeString(bstr);
									}
									else if	(S_OK == pMetaElem->get_name(&bstr))
									{
										//  Failed to get the HTTP Equiv, try and get the <META Name...>
										if (bstr != 0)
										{
											//  Copy the HTTP Equiv returned
											//  from the IE Component into our 
											//  Meta Tag Structure
											wcsncpy(tcHttpEquiv, bstr, MAX_URL);
											metaTag.tcHTTPEquiv = tcHttpEquiv;
											::SysFreeString(bstr);
										}
									}
								}
								if (metaTag.tcHTTPEquiv && S_OK == pMetaElem->get_content(&bstr)) 
								{
									if (bstr != 0) 
									{
										//  Copy the Contents returned
										//  from the IE component into our
										//  Meta Tag Structure
										wcsncpy(tcContents, bstr, MAX_URL);
										metaTag.tcContents = (TCHAR*) tcContents;
										::SysFreeString(bstr);
										//  Invoke the Meta Tag Callback.
										//  This blocks whilst the callback
										//  code is handled so metaTag does not
										//  go out of scope.
                                        PostMessage(rho_wmimpl_get_mainwnd(), PB_ONMETA, (WPARAM)m_tabID, (LPARAM)&metaTag);
									}
								}
								pMetaElem->Release();
							}
							pDisp2->Release();
						}
					}
				}
				pColl->Release();
			}
		}
	}
	return S_OK;
}


#endif //!defined( OS_WINCE )
