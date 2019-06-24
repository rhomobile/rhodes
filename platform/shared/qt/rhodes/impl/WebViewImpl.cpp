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

#include "common/RhoPort.h"
#include "ruby/ext/rho/rhoruby.h"
#include "rubyext/WebView.h"
#include "common/RhodesApp.h"
#include "common/StringConverter.h"

#ifndef RHODES_VERSION_LIBRARY
#include "MainWindowImpl.h"
#endif

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "WebView"

using namespace rho;
using namespace rho::common;

static wchar_t *dup_wcs(const wchar_t *s)
{
    size_t len = wcslen(s);
    wchar_t *ret = (wchar_t*)malloc((len+1)*sizeof(wchar_t));
    wcscpy(ret, s);
    return ret;
}

extern "C" {

void rho_webview_refresh(int index) 
{
#ifndef RHODES_VERSION_LIBRARY
    CMainWindow::getInstance()->refreshCommand(index);
#endif
}

void rho_webview_navigate(const char* url, int index) 
{
#ifndef RHODES_VERSION_LIBRARY
    if ( !url )
    {
        RAWLOG_ERROR("WebView.navigate failed: url is nil");
        return;
    }

    String strUrl = RHODESAPP().canonicalizeRhoUrl(url);

    TNavigateData* nd = (TNavigateData*)malloc(sizeof(TNavigateData));
    nd->index = index;
    nd->url = dup_wcs(convertToStringW(strUrl).c_str());
    CMainWindow::getInstance()->navigateCommand(nd);
#endif
}

void rho_webview_navigate_back()
{
#ifndef RHODES_VERSION_LIBRARY
    CMainWindow::getInstance()->webviewNavigateBackCommand(-1);
#endif
}

void rho_webview_navigate_back_with_tab(int index)
{
#ifndef RHODES_VERSION_LIBRARY
    CMainWindow::getInstance()->webviewNavigateBackCommand(index);
#endif
}

void rho_webview_navigate_forward()
{
#ifndef RHODES_VERSION_LIBRARY
    CMainWindow::getInstance()->navigateForwardCommand();
#endif
}

const char* rho_webview_execute_js(const char* js, int index) 
{
#ifndef RHODES_VERSION_LIBRARY
    String strJS = "javascript:";
    strJS += js;
    rho_webview_navigate(strJS.c_str(), index);
#endif
    return "";
}

const char* rho_webview_execute_js_sync(const char* js, int index) 
{
    // TODO: implement sync js callback
    rho_webview_execute_js(js, index);
    return "";
}

const char* rho_webview_current_location(int index) 
{
    return RHODESAPP().getCurrentUrl(index).c_str();
}

int rho_webview_active_tab() 
{
 #ifndef RHODES_VERSION_LIBRARY
    return CMainWindow::getInstance()->tabbarGetCurrent();
#else
    return -1;
#endif
}

void rho_webview_full_screen_mode(int enable)
{
#ifndef RHODES_VERSION_LIBRARY
    CMainWindow::getInstance()->fullscreenCommand(enable);
#endif
}

int rho_webview_get_full_screen()
{
#ifndef RHODES_VERSION_LIBRARY
    return CMainWindow::getInstance()->getFullScreen() ? 1 : 0;
#else
    return -1;
#endif
}

void rho_webview_set_cookie(const char *url, const char *cookie)
{
#ifndef RHODES_VERSION_LIBRARY
    CMainWindow::getInstance()->setCookie(url, cookie);
#endif
}

void rho_webview_save(const char* format, const char* path, int tab_index)
{
    RAWLOG_ERROR("rho_webview_save is not implemented");
}

VALUE rho_webview_get_current_url(int tab_index)
{
    RAWLOG_ERROR("rho_webview_get_current_url is not implemented");
    return rho_ruby_get_NIL();
}

} //extern "C"
