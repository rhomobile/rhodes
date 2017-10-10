#include "stdafx.h"


#if defined(OS_PLATFORM_MOTCE)

#include <atlbase.h>

#include "CEBrowserEngine.h"
#include "common/RhoConf.h"
#include "MainWindow.h"
#include "EngineDefines.h"
#include "EditSip.h"

#if defined(_WIN32_WCE)
#include <webvw.h>
#endif
//////////////////////////////////////////////////////////////////////////
extern "C" const wchar_t* rho_wmimpl_getNavTimeOutVal();
extern "C" HWND rho_wmimpl_get_mainwnd();
extern "C" LRESULT rho_wm_appmanager_ProcessOnTopMostWnd(WPARAM wParam, LPARAM lParam);
extern "C" bool rho_wmimpl_get_textselectionenabled();

LRESULT CALLBACK EditWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

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
WNDPROC OldWndProc;
BOOL isCancelButtonPressed=false;
//////////////////////////////////////////////////////////////////////////
CEBrowserEngine::CEBrowserEngine(HWND hwndParent, HINSTANCE hInstance)
    : m_ulRefs(0)
    , m_bInPlaceActive(true)
    , m_pBrowser(NULL)
    , m_hNavigated(NULL)
    , m_hDocComp(NULL)
    , m_hwndTabHTML(NULL)
    , bRunningOnWM(FALSE)
    , bDeviceCausesDoubleBackspace(FALSE)
    , m_tabID(0)
    , m_dwNavigationTimeout(45000)
    , m_bLoadingComplete(FALSE)
    , m_bNavigationError(FALSE)
	, m_bInitialised(FALSE)
	,m_bNavigationComplete(FALSE)
{
	m_hwndParent  = hwndParent;
	m_hInstance = hInstance;

	memset(m_tcCurrentPageTitle, NULL, sizeof(TCHAR) * MAX_URL);
	memset(m_tcNavigatedURL, 0, sizeof(TCHAR) * MAX_URL);
	//TODO TAU
	//convertFromStringW(rho_wmimpl_getNavTimeOutVal(),m_dwNavigationTimeout);
	if(m_dwNavigationTimeout<=0)
	{
		LOG(WARNING)+" NavigationTimeout  value  from config.xml not correct "+m_dwNavigationTimeout;
		m_dwNavigationTimeout=45000;
	}
    CoInitializeEx(NULL, COINIT_MULTITHREADED);

	GetWindowRect(hwndParent, &m_rcViewSize);
    CreateEngine();
	RHODESAPP().getExtManager().getEngineEventMngr().setEngineInterface(this);
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


	TCHAR szPlatform[128];
	memset(szPlatform, 0, 127 * sizeof(TCHAR));
	SystemParametersInfo(SPI_GETOEMINFO, 127, szPlatform, 0);


		if((_tcsstr(szPlatform,TEXT("MK4000")) != NULL) || (_tcsstr(szPlatform,TEXT("MK3100")) != NULL) || (_tcsstr(szPlatform,TEXT("MC18")) != NULL)|| (_tcsstr(szPlatform,TEXT("MK3000")) != NULL))
		{
			HKEY hRegKey= NULL;
			DWORD Type;
			DWORD RetSize = 999;
			DWORD dwManSIP = 0;
			if (ERROR_SUCCESS == RegOpenKeyEx(HKEY_CURRENT_USER, _T("Software\\Microsoft\\Internet Explorer\\Main"), 0, 0, &hRegKey)) 
			{

				RetSize = 999;
				RegQueryValueEx(hRegKey, _T("Disable Auto SIP"), NULL, &Type, (BYTE *) &dwManSIP, &RetSize);
				if(dwManSIP==1)
				{
					DWORD dwAutoSIP =0;
					LONG lResult = RegSetValueEx(hRegKey,_T("Disable Auto SIP"),0,Type,(BYTE *)&dwAutoSIP,sizeof(DWORD));
				}

				RegCloseKey(hRegKey);
			}
		}

	/////////////
	//we only want to do this once, so check that the m_pBrowser is null
	if (!m_pBrowser)
	{
    
		// See if text selection is enabled, so we can pass the correct value through the GetHostInfo() interface
		//configFunction(m_tabID, L"HTMLStyles\\TextSelectionEnabled", tcConfigSetting);
		//TODO TAU
		m_bTextSelectionEnabled = FALSE; //(tcConfigSetting [0] == L'1');
		//m_bTextSelectionEnabled = rho_wmimpl_get_textselectionenabled();

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
//		CloseHandle (CreateThread(NULL, 0, 
//			&CEBrowserEngine::RegisterWndProcThread, (LPVOID)this, 0, NULL));
		


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

	ShowWindow(m_hwndTabHTML, SW_SHOW);
	SetForegroundWindow(m_hwndTabHTML);
	MoveWindow(m_hwndTabHTML, m_rcViewSize.left, m_rcViewSize.top, (m_rcViewSize.right-m_rcViewSize.left), (m_rcViewSize.bottom-m_rcViewSize.top), FALSE);

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

        SetEvent(m_hDocComp);
        CloseHandle(m_hDocComp);
        m_hDocComp = NULL;
            
        PostMessage(m_hwndParent, WM_BROWSER_ONNAVIGATECOMPLETE, m_tabID, (LPARAM)_tcsdup(L"NavigationStopped"));
        PostMessage(m_hwndParent, WM_BROWSER_ONDOCUMENTCOMPLETE, m_tabID, (LPARAM)_tcsdup(L"NavigationStopped"));

		return TRUE;
	}

	return FALSE;
}

DWORD WINAPI CEBrowserEngine::DocumentTimeoutThread( LPVOID lpParameter )
{
    LOG(INFO) + (L"DocThread Started\n");

    CEBrowserEngine * pEng = reinterpret_cast<CEBrowserEngine*>(lpParameter);

    if(pEng->m_hDocComp == NULL)
    {
        pEng->m_hDocComp = CreateEvent(NULL, TRUE, FALSE, L"PB_IEENGINE_DOCCOMPLETE_IN_PROGRESS"/*NULL*/);

        if(WaitForSingleObject(pEng->m_hDocComp, pEng->m_dwNavigationTimeout) != WAIT_OBJECT_0)
        {
            //no point in doing anything as there is no event handler
            pEng->StopOnTab(pEng->m_tabID);
            CloseHandle(pEng->m_hDocComp);
            pEng->m_hDocComp = NULL;

            //send fake document complete event to plug-in modules
            SendMessage(pEng->m_hwndParent, WM_BROWSER_ONDOCUMENTCOMPLETE, 
                (WPARAM)pEng->m_tabID, (LPARAM)pEng->m_tcNavigatedURL);

            //send navigation timeout event to hit the local bad link page for recovery from missing document complete event
            SendMessage(pEng->m_hwndParent, WM_BROWSER_ONNAVIGATIONTIMEOUT, 
                (WPARAM)pEng->m_tabID, (LPARAM)pEng->m_tcNavigatedURL);
        }
    }

    LOG(INFO) + (L"DocThread Ended\n");

    return 0;
}

DWORD WINAPI CEBrowserEngine::NavigationTimeoutThread( LPVOID lpParameter )
{
    CEBrowserEngine * pEng = (CEBrowserEngine*) lpParameter;
	DWORD dwWaitResult;
	bool flag=false;
	HWND authwindowhandle;

    if (pEng->m_dwNavigationTimeout)
    {		
    	do
    	{
		dwWaitResult = WaitForSingleObject(pEng->m_hNavigated, pEng->m_dwNavigationTimeout);

		switch (dwWaitResult) 
		{
			// Event object was signaled
			case WAIT_OBJECT_0: 
				//
				// TODO: Read from the shared buffer
				//
				LOG(INFO) + "NavigationTimeoutThread:Event object was signaled\n";
								
				CloseHandle(pEng->m_hNavigated);
				pEng->m_hNavigated = NULL;
				flag=false;
				if(pEng->m_bNavigationError)
				{
					LOG(INFO) + "NavigationTimeoutThread:m_bNavigationError\n";
					pEng->StopOnTab(0);
					Sleep(400);
					SendMessage(pEng->m_hwndParent, WM_BROWSER_ONNAVIGATIONERROR, 
					(WPARAM)pEng->m_tabID, (LPARAM)pEng->m_tcNavigatedURL);
					pEng->m_bNavigationError=FALSE;

				}
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
					
				pEng->StopOnTab(0);
						
				CloseHandle(pEng->m_hNavigated);
				pEng->m_hNavigated = NULL;
				SendMessage(pEng->m_hwndParent, WM_BROWSER_ONNAVIGATIONTIMEOUT, 
                (WPARAM)pEng->m_tabID, (LPARAM)pEng->m_tcNavigatedURL);

				break; 

			// An error occurred
			default: 
				LOG(INFO) + "Wait error  GetLastError()=\n"+ GetLastError();
				flag=false;
				return 0; 
		}		
		
    	}while(flag);	
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
	{
		//Setting of pInfo->dwFlags to DOCHOSTUIFLAG_DIALOG
		//has been commented as this restricts zoom button to work
		//pInfo->dwFlags |= DOCHOSTUIFLAG_DIALOG;
	}

	pInfo->dwDoubleClick = DOCHOSTUIDBLCLK_DEFAULT;

	return S_OK;
}

HRESULT CEBrowserEngine::TranslateAccelerator( 
			LPMSG lpMsg,
			const GUID __RPC_FAR *pguidCmdGroup,
			DWORD nCmdID)
{
	/*
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
*/
/*	if (lpMsg && (lpMsg->message == WM_KEYDOWN))
	{
		if (lpMsg->wParam == VK_LEFT ||	lpMsg->wParam == VK_RIGHT || lpMsg->wParam == VK_UP || lpMsg->wParam == VK_DOWN || lpMsg->wParam == VK_RETURN)
		{
			//EMBPD00174595 - Prevent duplicate left, right, up, down & enter keys
			return S_OK;
		}
	}
*/
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
        	LOG(INFO) + "DISPID_NAVIGATEERROR";
        	if(!isCancelButtonPressed)
		{
			m_bNavigationError=TRUE;
			SetEvent(m_hNavigated);
			CloseHandle(m_hNavigated);
			m_hNavigated = NULL;
		}
		else 
			isCancelButtonPressed=false;
		//get the URL which failed
		if (pdparams && pdparams->rgvarg[0].vt == VT_BSTR)
			wcsncpy(tcURL, pdparams->rgvarg[3].pvarVal->bstrVal, MAX_URL-1);
		
  //      SendMessage(m_hwndParent, WM_BROWSER_ONNAVIGATIONERROR, (WPARAM)m_tabID, (LPARAM)tcURL);
		
        *(pdparams->rgvarg[0].pboolVal) = VARIANT_TRUE;
		retVal = S_OK;

		break;
	
	case DISPID_NAVIGATECOMPLETE2:
        LOG(INFO) + "DISPID_NAVIGATECOMPLETE2";
		if (!m_bInitialised)
			RegisterWndProcThread(this);

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

        SetEvent(m_hDocComp);
        CloseHandle(m_hDocComp);
        m_hDocComp = NULL;

        CloseHandle (CreateThread(NULL, 0, &CEBrowserEngine::DocumentTimeoutThread, (LPVOID)this, 0, NULL));

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
    case DISPID_PROGRESSCHANGE:
        {
            LOG(INFO) + "progress change";	
            if (pdparams && pdparams->rgvarg[1].vt == VT_I4) 
            { 
                if(0 == pdparams->rgvarg[1].iVal)//if progress completed
                {
					ParseTags();//moved from doc complete as we won't get doc complete notification on page refresh.
                    LOG(INFO) + "before calling dominjector ";
                    RHODESAPP().getExtManager().getEngineEventMngr().injectDOMElements();
                }
            }
            break;
        }

	case DISPID_DOCUMENTCOMPLETE:
		//Validate that there is an event handler
		LOG(INFO) + "DISPID_DOCUMENTCOMPLETE";		
        SetEvent(m_hDocComp);
        CloseHandle(m_hDocComp);
        m_hDocComp = NULL;

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

		retVal = S_OK;
        m_bNavigationComplete = TRUE;
		break;

	case DISPID_BEFORENAVIGATE2:
        LOG(INFO) + "DISPID_BEFORENAVIGATE2";

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
		
		//EMBPD00163219- To Append "file://" in  URL
		TCHAR strFile[MAX_URL];
		memset(strFile, NULL, sizeof(TCHAR) * MAX_URL);
		if(memcmp(tcURL, L"\\", 1) == 0)
		{
			wcscpy(strFile,L"file://");
			wcscat(strFile,tcURL);
			LOG(INFO) + "CEBrowserEngine tcURL "+tcURL;
			LOG(INFO) + "CEBrowserEngine strFile "+strFile;

		}
		else
		{
			wcscpy(strFile,tcURL);
		}

		//  Test if the user has attempted to navigate back in the history
		if (wcsicmp(strFile, L"history:back") == 0)
		{
            		m_pBrowser->GoBack();
        		 *(pdparams->rgvarg[0].pboolVal) = VARIANT_TRUE;
			retVal = S_OK;
            		break;
		}
        if(m_hNavigated==NULL)
            m_hNavigated = CreateEvent(NULL, TRUE, FALSE, L"PB_IEENGINE_NAVIGATION_IN_PROGRESS");

		CloseHandle (CreateThread(NULL, 0, &CEBrowserEngine::NavigationTimeoutThread, (LPVOID)this, 0, NULL));
		// EMBPD00158491
		m_bNavigationComplete = FALSE;
		CloseHandle (CreateThread(NULL, 0, &CEBrowserEngine::NetworkWindowThread, (LPVOID)this, 0, NULL));
		wcscpy(m_tcNavigatedURL, strFile);

#ifdef SCROLL_NOTIFY
		// Stop any checking for scroll changes during navigation
		if (pScrollNotify)
		{
			delete pScrollNotify;
			pScrollNotify = NULL;
		}
#endif
        PostMessage(m_hwndParent, WM_BROWSER_ONBEFORENAVIGATE, (WPARAM)m_tabID, (LPARAM)_tcsdup(strFile));

		retVal = S_OK;
		break;
	}

	delete[] tcURL;
	//tcURL = NULL;

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

void CEBrowserEngine::RunMessageLoop(CMainWindow& mainWnd) 
{
	MSG msg;
	//This is used for checking the editable window focus. If the focus is lost on key down then we set the focus back to the screen
	TCHAR getEditableWndClassName[MAX_PATH];
	HWND getEditableFocusWnd = NULL;
	
    while (GetMessage(&msg, NULL, 0, 0))
    {
    	        HRESULT handleKey = S_FALSE;

        if (RHODESAPP().getExtManager().onWndMsg(msg) )
            continue;

		if (msg.message == WM_MOUSEFIRST)
		{
			//Get the details of the editable window, this will be used if the focus is lost on key down
			getEditableFocusWnd = NULL;
			ZeroMemory(getEditableWndClassName,MAX_PATH);
			getEditableFocusWnd = GetFocus();
			if(getEditableFocusWnd != NULL)
			{
				GetClassName(getEditableFocusWnd,getEditableWndClassName,MAX_PATH);
			}
			
			bool m_bFullScreen = RHOCONF().getBool("full_screen");

			if (m_bFullScreen)
			{
				HWND taskbarWnd = FindWindow(L"HHTaskBar", NULL);
				ShowWindow(taskbarWnd, SW_HIDE);
			}
		}
		
		if ((msg.message == WM_KEYDOWN || msg.message == WM_KEYUP) && msg.wParam != VK_BACK)	//  Run Browser TranslateAccelerator
		{
			//Setting foucs back to the editable window if the focus is lost
			if( _tcsicmp(getEditableWndClassName,TEXT("Internet Explorer_Server")) == 0 )
			{
				HWND checkFocusWnd = GetFocus();
				if((getEditableFocusWnd != NULL) && (checkFocusWnd != NULL))
				{
					if((checkFocusWnd != getEditableFocusWnd) && (msg.message == WM_KEYDOWN))
					{
						SetFocus(getEditableFocusWnd);
					}
				}
			}
			
			IDispatch* pDisp;
			m_pBrowser->get_Document(&pDisp);
			if (pDisp != NULL)
			{
				IOleInPlaceActiveObject* pInPlaceObject;
				pDisp->QueryInterface( IID_IOleInPlaceActiveObject, (void**)&pInPlaceObject);
			        handleKey = pInPlaceObject->TranslateAccelerator(&msg);
			}
		}

		
		if(handleKey != S_OK)
		{
		
			if (!mainWnd.TranslateAccelerator(&msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			LOG(INFO)+"HandleKey S_OK";
			
		}

		if(msg.message == WM_PAINT)
			RHODESAPP().getExtManager().onHTMLWndMsg(msg);	
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

    OLECHAR FAR* szJSMethodName = (OLECHAR *)functionName;
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
		         InvokeJs(tcURI, index);
            delete[] tcURI;
        }
        else
        {
		         InvokeJs(szJSFunction, index);
        }
    }
}

void CEBrowserEngine::InvokeJs(const wchar_t* szJSFunction, int index)
{
	LPDISPATCH   pDoc          = (IDispatch FAR*)NULL;
	CComPtr<IHTMLWindow2> pIHTMLWindow2 = NULL;
	CComVariant retval;
	// get the active doc interface
	m_pBrowser->get_Document(&pDoc);
	if (pDoc != NULL)
	{
		IHTMLDocument2* pHTMLDocument2;
		HRESULT hr = pDoc->QueryInterface( IID_IHTMLDocument2, (void**)&pHTMLDocument2 );		
		//get the associated window; It provides access to the window object, which represents an open window in the browser.
		pHTMLDocument2->get_parentWindow( &pIHTMLWindow2);
		//Executes the specified script in the provided language.		
		hr = pIHTMLWindow2->execScript( CComBSTR(szJSFunction) , NULL , &retval );
		if(FAILED(hr))
		{
			LOG(ERROR) + L"execute script failed";
		}
	
	}
	else
	{
		LOG(ERROR) + L"Unable to get Document interface";
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
							
                            hr = pDisp2->QueryInterface( IID_IHTMLMetaElement, (void **)&pMetaElem );

							if ( hr == S_OK )
							{
								//  The engine uses its own definition of 
								//  meta tags, must HTTP Equiv and Contents
								EngineMETATag *metaTag = new EngineMETATag;
								//memset(&metaTag, 0, sizeof(metaTag));
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
                                        int httpEquivLenght = wcslen(tcHttpEquiv)+1;
                                        metaTag->tcHTTPEquiv = new WCHAR[httpEquivLenght];
                                        wcsncpy(metaTag->tcHTTPEquiv, tcHttpEquiv, httpEquivLenght);
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
                                            int httpEquivLenght = wcslen(tcHttpEquiv)+1;
                                            metaTag->tcHTTPEquiv = new WCHAR[httpEquivLenght];											
                                            wcsncpy(metaTag->tcHTTPEquiv, tcHttpEquiv, httpEquivLenght);
											::SysFreeString(bstr);
										}
									}
								}

								if (metaTag->tcHTTPEquiv && S_OK == pMetaElem->get_content(&bstr)) 
								{
									if (bstr != 0) 
									{
										//  Copy the Contents returned
										//  from the IE component into our
										//  Meta Tag Structure
										wcsncpy(tcContents, bstr, MAX_URL);
                                        int contentsLenght = wcslen(tcContents)+1;
                                        metaTag->tcContents = new WCHAR[contentsLenght];
                                        wcsncpy(metaTag->tcContents, tcContents, contentsLenght);
										::SysFreeString(bstr);
										//  Invoke the Meta Tag Callback.
										//  This blocks whilst the callback
										//  code is handled so metaTag does not
										//  go out of scope.
                                        SendMessage(rho_wmimpl_get_mainwnd(), PB_ONMETA, (WPARAM)m_tabID, (LPARAM)metaTag);
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


/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		November 2009 (DCC: Initial Creation)
*/
DWORD WINAPI CEBrowserEngine::RegisterWndProcThread(LPVOID lpParameter)
{
	//  We are passed a pointer to the engine we are interested in.
	CEBrowserEngine* pEngine = reinterpret_cast<CEBrowserEngine*>(lpParameter);
	pEngine->m_bInitialised = true;

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
	HWND hwndHTMLChild1 = NULL;
	HWND hwndHTMLMessageWindow = NULL;

    while (hwndHTMLMessageWindow == NULL)
	{
        hwndHTML = pEngine->GetHTMLWND(0);

        if (hwndHTML != NULL)
		{
            hwndHTMLChild1 = ::GetWindow(hwndHTML, GW_CHILD);

            if (hwndHTMLChild1 != NULL)
			{
                hwndHTMLMessageWindow = ::GetWindow(hwndHTMLChild1, GW_CHILD);
			}
		}

        Sleep(100);
	}

	//  Invoke directly as this must be done before all meta tag parsing.  On some devices meta tags were 
	//  being partially processed causing subsequent meta tags to not be read correctly.
	//rho_wm_appmanager_ProcessOnTopMostWnd((LPARAM)0, (WPARAM)hwndHTMLMessageWindow);
	SendMessage(rho_wmimpl_get_mainwnd(), PB_ONTOPMOSTWINDOW,(LPARAM)0, (WPARAM)hwndHTMLMessageWindow);

    return 0;
}

int CEBrowserEngine::GetTextZoomOnTab(UINT iTab)
{
	int nZoom = TEXT_ZOOM_NORMAL;//Setting initial value as Normal
	LPDISPATCH pDisp = NULL;
	LPOLECOMMANDTARGET pCmdTarg = NULL;
	if (S_OK != m_pBrowser->get_Document(&pDisp)) 
		return nZoom;
	if (pDisp == NULL)
		return nZoom;
	pDisp->QueryInterface(IID_IOleCommandTarget, (LPVOID*)&pCmdTarg);
	if (pCmdTarg == NULL)
		return nZoom;
	VARIANT vaZoomFactor;   // input arguments
	VariantInit(&vaZoomFactor);
	pCmdTarg->Exec(NULL,
				OLECMDID_ZOOM,
				OLECMDEXECOPT_DONTPROMPTUSER,
				NULL,
				&vaZoomFactor);
	//  vaZoomFactor contains the current zoom level of the page
	if (V_I4(&vaZoomFactor) == (DWORD)TEXT_ZOOM_SMALLEST)
		nZoom = TEXT_ZOOM_SMALLEST;
	else if (V_I4(&vaZoomFactor) == (DWORD)TEXT_ZOOM_SMALLER)
		nZoom = TEXT_ZOOM_SMALLER;
	else if (V_I4(&vaZoomFactor) == (DWORD)TEXT_ZOOM_NORMAL)
		nZoom = TEXT_ZOOM_NORMAL;
	else if (V_I4(&vaZoomFactor) == (DWORD)TEXT_ZOOM_BIGGER)
		nZoom = TEXT_ZOOM_BIGGER;
	else if (V_I4(&vaZoomFactor) == (DWORD)TEXT_ZOOM_BIGGEST)
		nZoom = TEXT_ZOOM_BIGGEST;
	VariantClear(&vaZoomFactor);
	if (pCmdTarg)
	   pCmdTarg->Release(); // release document's command target
	if (pDisp)
	   pDisp->Release();// release document's dispatch interface
	return nZoom;
}

BOOL CEBrowserEngine::ZoomTextOnTab(int nZoom, UINT iTab) 
{ 
	LPDISPATCH pDisp = NULL;
	LPOLECOMMANDTARGET pCmdTarg = NULL;
	if (S_OK != m_pBrowser->get_Document(&pDisp)) 
		return S_FALSE;
	if (pDisp == NULL)
		return S_FALSE;
	pDisp->QueryInterface(IID_IOleCommandTarget, (LPVOID*)&pCmdTarg);
	if (pCmdTarg == NULL)
		return S_FALSE;
	VARIANT vaZoomFactor;   // input arguments
	VariantInit(&vaZoomFactor);
	V_VT(&vaZoomFactor) = VT_I4;
	V_I4(&vaZoomFactor) = nZoom;
	pCmdTarg->Exec(NULL,
				OLECMDID_ZOOM,
				OLECMDEXECOPT_DONTPROMPTUSER,
				&vaZoomFactor,
				NULL);
	VariantClear(&vaZoomFactor);
	if (pCmdTarg)
	   pCmdTarg->Release(); // release document's command target
	if (pDisp)
	   pDisp->Release();    // release document's dispatch interface
	return S_OK;
}

LRESULT CALLBACK NewWndProc (HWND hWnd, UINT msg,WPARAM wParam, LPARAM lParam)
{

        if(msg == WM_COMMAND)
        {

                if((LOWORD(wParam) == IDCANCEL) && (HIWORD(wParam) == BN_CLICKED) )
                {
			isCancelButtonPressed=true;
                }
        }
        return CallWindowProc(OldWndProc, hWnd, msg, wParam, lParam); 
}
DWORD WINAPI CEBrowserEngine::NetworkWindowThread( LPVOID lpParameter )
{

	CEBrowserEngine * pEng = (CEBrowserEngine*) lpParameter;

	TCHAR szWindowText[MAX_PATH];
	TCHAR szWindowClass[MAX_PATH];
	HWND hCurrentWindow = NULL;

	ZeroMemory(szWindowText,MAX_PATH);
	ZeroMemory(szWindowClass,MAX_PATH);

	while(!pEng->m_bNavigationComplete)
	{
		HWND hCurrentWindow=FindWindow(TEXT("Dialog"),TEXT("Enter Network Password"));
		if(hCurrentWindow != NULL)
		{
			OldWndProc = (WNDPROC)SetWindowLongPtr (hCurrentWindow,GWLP_WNDPROC, (LONG_PTR)NewWndProc);
			GetWindowText(hCurrentWindow,szWindowText,MAX_PATH);
			ZeroMemory(szWindowClass,MAX_PATH);
			GetClassName(hCurrentWindow,szWindowClass,MAX_PATH);
			LONG lIndex = GetWindowLong(hCurrentWindow,GWL_ID);
			
			::EnumChildWindows(hCurrentWindow,(WNDENUMPROC)NetworkWndProc,0);
			pEng->m_bNavigationComplete = TRUE;
			return 0;	
		}
	}
	return 0;
}
bool CEBrowserEngine::executAnonymousJs(wchar_t* szFunctionText, int nTabID)
{
	bool bRetStatus = true;
	InvokeJs(szFunctionText, nTabID);
	//LOG(INFO) + szFunctionText;
	return bRetStatus;
}

// EMBPD00158491 - [SAP-ITS][CE5/MK4000]-Sip is not shown while trying to enter text in fields of Authentication screen
// SIP not appearing on authentication window on IE engine has been fixed
LRESULT CALLBACK NetworkWndProc(HWND hWnd,LPARAM lParam)
{
	if(hWnd)
	{

		TCHAR szChildWndText[MAX_PATH];
		TCHAR szChildWndClass[MAX_PATH];

		ZeroMemory(szChildWndText,MAX_PATH);
		ZeroMemory(szChildWndClass,MAX_PATH);

		GetWindowText(hWnd,szChildWndText,MAX_PATH);
		GetClassName(hWnd,szChildWndClass,MAX_PATH);
		
		if(_tcsicmp(szChildWndClass,TEXT("Edit")) ==0)
		{
			g_wpEditWndProc= (WNDPROC)SetWindowLong(hWnd,GWL_WNDPROC,(LONG)EditWndProc);
			if(g_wpEditWndProc== NULL)
			{
				LOG(ERROR) + "[NetworkWndProc] Setwindowlong returns null";
				LOG(ERROR) + GetLastError();
			}
		}
		return TRUE;
	}
	else
		return FALSE;
	
}



LRESULT CALLBACK EditWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{

			switch(message)
			{
			case WM_SETFOCUS:
				{
					ShowSIPWnd(TRUE);
				}
				break;
			case WM_KILLFOCUS:
				{
					ShowSIPWnd(FALSE);
				}
				break;
			}
			return CallWindowProc(g_wpEditWndProc, hwnd, message, wParam, lParam); 
}
#endif //!defined( OS_WINCE )

