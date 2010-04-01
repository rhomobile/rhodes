#include "stdafx.h"

#include "common/RhodesApp.h"
#include "MainWindow.h"

#include "WebView.h"

extern "C" HWND getMainWnd();

IMPLEMENT_LOGCLASS(CWebView, "RingtoneManager");

CWebView *CWebView::m_pInstance = NULL;
CMutex CWebView::m_mxLocker;

void CWebView::createCWebView()
{
    static CWebView instance;
    m_pInstance = &instance;
}

CWebView &CWebView::getCWebView()
{
    m_mxLocker.Lock();

    if (!m_pInstance)
        createCWebView();
    
    m_mxLocker.Unlock();
    
    return *m_pInstance;
}


//C interface

//Sync hook to refresh the web view
void perform_webview_refresh() {
	webview_refresh(0);
}


void webview_set_menu_items(VALUE valMenu) 
{
	/*
    RHODESAPP().setViewMenu(valMenu);
	HWND main_wnd = getMainWnd();
	::PostMessage(main_wnd, WM_SET_CUSTOM_MENU, 0, (LPARAM )new Hashtable <String, String> (RHODESAPP().getViewMenu()));
	*/
}

 int webview_active_tab() {
	return 0;
}

 char* webview_current_location(int index) {
    return const_cast<char*>(RHODESAPP().getCurrentUrl(index).c_str());
}
