#include "stdafx.h"
#include <wininet.h>

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

	RAWTRACEC1("Execute JS: %s", js);

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

}



