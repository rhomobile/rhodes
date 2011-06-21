#include <time.h>
#include <wchar.h>
#include "ruby/ext/rho/rhoruby.h"
#include "rubyext/WebView.h"
#include "common/RhodesApp.h"
#include "common/StringConverter.h"
#include "MainWindowImpl.h"

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
    CMainWindow::getInstance()->refreshCommand(index);
}

void rho_webview_navigate(const char* url, int index) 
{
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
}

void rho_webview_navigate_back()
{
    CMainWindow::getInstance()->navigateBackCommand();
}

void rho_webview_navigate_forward()
{
    CMainWindow::getInstance()->navigateForwardCommand();
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
    //TODO: return getAppWindow().tabbarGetCurrent();
    return 0;
}

void rho_webview_set_menu_items(VALUE valMenu) 
{
    rho_rhodesapp_setViewMenu(valMenu); 
}

void rho_webview_full_screen_mode(int enable)
{
    CMainWindow::getInstance()->fullscreenCommand(enable);
}

void rho_webview_set_cookie(const char *url, const char *cookie)
{
    //TODO: rho_webview_set_cookie
}

} //extern "C"
