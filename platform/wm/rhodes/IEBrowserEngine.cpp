#include "stdafx.h"
#include <wininet.h>
#include "IEBrowserEngine.h"
#include "common/RhoConf.h"
#include "MainWindow.h"

#if defined(_WIN32_WCE)
#include <webvw.h>
#endif

CIEBrowserEngine::CIEBrowserEngine(HWND hParentWnd, HINSTANCE hInstance) :
    m_spIWebBrowser2(NULL)
{
#if defined(_WIN32_WCE)
    m_browser.Create(hParentWnd,
                     CWindow::rcDefault, // proper sizing is done in CMainWindow::OnSize
		    		 TEXT("Microsoft.PIEDocView"), // ProgID of the control
                     WS_CHILD, 0,
                     ID_BROWSER);

#else
	m_browser.Create(hParentWnd,
                     CWindow::rcDefault, // proper sizing is done in CMainWindow::OnSize
					 TEXT("Shell.Explorer"), // ProgID of the control
                     WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0,
                     ID_BROWSER);
#endif

    // cache IWebBrowser2 interface pointer
    HRESULT hr = m_browser.QueryControl(&m_spIWebBrowser2);
    m_spIWebBrowser2->put_AddressBar(VARIANT_FALSE);

    if ( !RHOCONF().getBool("wm_show_statusbar") )
        m_spIWebBrowser2->put_StatusBar(VARIANT_FALSE);

}

CIEBrowserEngine::~CIEBrowserEngine(void)
{
    //TODO: destroy browser
}

BOOL CIEBrowserEngine::Navigate(LPCTSTR szURL)
{
    BSTR bstrUrl = SysAllocString(szURL);
    BOOL bRes = m_spIWebBrowser2->Navigate(bstrUrl, NULL, &CComVariant(L"_self"), NULL, NULL) == S_OK;

    SysFreeString(bstrUrl);    

    return bRes;
}

BOOL CIEBrowserEngine::ResizeOnTab(int iInstID,RECT rcNewSize)
{
    return m_browser.MoveWindow(&rcNewSize);
}

BOOL CIEBrowserEngine::BackOnTab(int iInstID,int iPagesBack /*= 1*/)
{
    return m_spIWebBrowser2->GoBack() == S_OK;
}

BOOL CIEBrowserEngine::ForwardOnTab(int iInstID)
{
    return m_spIWebBrowser2->GoForward() == S_OK;
}

BOOL CIEBrowserEngine::ReloadOnTab(bool bFromCache, UINT iTab)
{
    return m_spIWebBrowser2->Refresh() == S_OK;
}

static void writeHtmlToTheDoc (
#if defined(_WIN32_WCE) && !defined( OS_PLATFORM_MOTCE )
					IPIEHTMLDocument2 *document
#else
					IHTMLDocument2 *document
#endif
					, LPCTSTR szHtml)
{
	HRESULT hresult = S_OK;
	VARIANT *param;
	SAFEARRAY *sfArray;
	//std::wstring html;
	BSTR bstr = SysAllocString(szHtml);

	// Creates a new one-dimensional array
	sfArray = SafeArrayCreateVector(VT_VARIANT, 0, 1);

	if (sfArray && document) 
    {
	    hresult = SafeArrayAccessData(sfArray,(LPVOID*) & param);
	    param->vt = VT_BSTR;
	    param->bstrVal = bstr;
	    hresult = SafeArrayUnaccessData(sfArray);
	    hresult = document->write(sfArray);
    }

	SysFreeString(bstr);
	if (sfArray != NULL) {
		SafeArrayDestroy(sfArray);
	}
}

BOOL CIEBrowserEngine::NavigateToHtml(LPCTSTR szHtml)
{
	HRESULT hr;
	IDispatch* pHtmlDoc = NULL;

    // Retrieve the document object.
    hr = m_spIWebBrowser2->get_Document( &pHtmlDoc );
    if ( SUCCEEDED(hr) )
    {
#if defined(_WIN32_WCE)&& !defined( OS_PLATFORM_MOTCE )
		IPIEHTMLDocument2* pDoc;
		hr = pHtmlDoc->QueryInterface(__uuidof(IPIEHTMLDocument2),  (void**)&pDoc );
#else
		IHTMLDocument2* pDoc;
		hr = pHtmlDoc->QueryInterface(__uuidof(IHTMLDocument2),  (void**)&pDoc );
#endif
        if ( SUCCEEDED(hr) )
        {
			// Write to the document
			writeHtmlToTheDoc(pDoc, szHtml);
			pDoc->Release();
        }
    }

    return TRUE;
}

LRESULT CIEBrowserEngine::OnWebKitMessages(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    bHandled = FALSE;
    return 0;
}

void CIEBrowserEngine::RunMessageLoop(CMainWindow& mainWnd)
{
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        if (!mainWnd.TranslateAccelerator(&msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
}

void CIEBrowserEngine::SetCookie(char* url, char* cookie)
{
	URL_COMPONENTSA uri;
	::memset(&uri, 0, sizeof(uri));
	uri.dwStructSize = sizeof(uri);
	uri.dwSchemeLength = 1;
	uri.dwHostNameLength = 1;
	uri.dwUrlPathLength = 1;
	if (!::InternetCrackUrlA(url, ::strlen(url), 0, &uri)) {
		RAWLOG_ERROR1("WebView.set_cookie: can not parse url: %s", url);
		return;
	}

	std::string nurl(uri.lpszScheme, uri.dwSchemeLength);
	nurl += "://";
	nurl += std::string(uri.lpszHostName, uri.dwHostNameLength);
	nurl += std::string(uri.lpszUrlPath, uri.dwUrlPathLength);

	for (const char *c = cookie;;) {
		const char *s = c;
		for (; *s != ';' && *s != '\0'; ++s);
		std::string c1(c, s - c);
		if (!::InternetSetCookieA(nurl.c_str(), NULL, c1.c_str()))
			RAWLOG_ERROR1("WebView.set_cookie: can not set cookie for url %s", nurl.c_str());
		if (*s == '\0')
			break;
		for (c = s + 1; *c == ' '; ++c);
	}
}
