#include "stdafx.h"

#include "ruby/ext/rho/rhoruby.h"
#include "rubyext/WebView.h"
#include "common/RhodesApp.h"
#include "common/StringConverter.h"
#include "MainWindow.h"

using namespace rho;
using namespace rho::common;
extern "C" {
HWND getMainWnd();

void rho_webview_refresh(int index) 
{
    rho_webview_navigate(RHODESAPP().getCurrentUrl().c_str(),0);
}

void rho_webview_navigate(const char* url, int index) 
{
    String strUrl = RHODESAPP().canonicalizeRhoUrl(url);
    ::PostMessage( getMainWnd(), WM_COMMAND, IDM_NAVIGATE, (LPARAM)_tcsdup(convertToStringW(strUrl).c_str()) );
}

void rho_webview_navigate_back()
{
    ::PostMessage( getMainWnd(), WM_COMMAND, IDM_NAVIGATE_BACK, (LPARAM)0 );
}

const char* rho_webview_execute_js(const char* js, int index) 
{
    String strJS = "javascript:";
    strJS += js;

    rho_webview_navigate(strJS.c_str(), index);
	return "";
}

const char* rho_webview_current_location(int index) 
{
    return RHODESAPP().getCurrentUrl(index).c_str();
}

int rho_webview_active_tab() 
{
	return 0;
}

void rho_webview_set_menu_items(VALUE valMenu) 
{
    rho_rhodesapp_setViewMenu(valMenu); 
}


void rho_webview_full_screen_mode(int enable)
{
    ::PostMessage( getMainWnd(), WM_COMMAND, ID_FULLSCREEN, (LPARAM)enable );
}

void rho_webview_set_cookie(const char *url, const char *cookie)
{
    // TODO: implement
}

}



