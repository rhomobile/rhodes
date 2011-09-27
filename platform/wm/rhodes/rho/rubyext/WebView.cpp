/*------------------------------------------------------------------------
* (The MIT License)
* 
* Copyright (c) 2008-2011 Rhomobile, Inc.
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
* 
* http://rhomobile.com
*------------------------------------------------------------------------*/

#include "stdafx.h"
#include <wininet.h>

#include "ruby/ext/rho/rhoruby.h"
#include "rubyext/WebView.h"
#include "common/RhodesApp.h"
#include "common/StringConverter.h"
#include "MainWindow.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "WebView"

using namespace rho;
using namespace rho::common;

extern CMainWindow& getAppWindow();

extern "C" {
HWND getMainWnd();

void rho_webview_refresh(int index) 
{
    //rho_webview_navigate(RHODESAPP().getCurrentUrl().c_str(),0);
    ::PostMessage( getMainWnd(), WM_COMMAND, IDM_REFRESH, (LPARAM)index );
}

void rho_webview_navigate(const char* url, int index) 
{
    if ( !url )
    {
        RAWLOG_ERROR("WebView.navigate failed: url is nil");
        return;
    }

    String strUrl = RHODESAPP().canonicalizeRhoUrl(url);
#ifdef RHODES_EMULATOR
    TNavigateData* nd = (TNavigateData*)malloc(sizeof(TNavigateData));
    nd->index = index;
    nd->url = _tcsdup(convertToStringW(strUrl).c_str());
    ::PostMessage( getMainWnd(), WM_COMMAND, IDM_NAVIGATE, (LPARAM)nd );
#else
    ::PostMessage( getMainWnd(), WM_COMMAND, IDM_NAVIGATE, (LPARAM)_tcsdup(convertToStringW(strUrl).c_str()) );
#endif
}

void rho_webview_navigate_back()
{
    ::PostMessage( getMainWnd(), WM_COMMAND, IDM_NAVIGATE_BACK, (LPARAM)0 );
}

void rho_webview_navigate_forward()
{
    ::PostMessage( getMainWnd(), WM_COMMAND, IDM_NAVIGATE_FORWARD, (LPARAM)0 );
}

const char* rho_webview_execute_js(const char* js, int index) 
{
    String strJS = "javascript:";
    strJS += js;

	//RAWTRACEC1("Execute JS: %s", js);

    rho_webview_navigate(strJS.c_str(), index);
	return "";
}

const char* rho_webview_current_location(int index) 
{
    return RHODESAPP().getCurrentUrl(index).c_str();
}

int rho_webview_active_tab() 
{
#ifdef RHODES_EMULATOR
    return getAppWindow().tabbarGetCurrent();
#else
	return 0;
#endif
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
    TCookieData* cd = (TCookieData*)malloc(sizeof(TCookieData));
    cd->url = strdup(url);
    cd->cookie = strdup(cookie);
    ::PostMessage( getMainWnd(), WM_COMMAND, ID_SETCOOKIE, (LPARAM)cd );
}

}
