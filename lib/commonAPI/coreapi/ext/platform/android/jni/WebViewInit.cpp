#include "rhodes.h"
#include "WebView.h"

#include "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "WebView"

#define FACTORY_CLASS "com.rho.webview.WebViewFactory"

extern "C" void Init_WebView_API(void);

extern "C" void Init_WebView(void)
{
    RAWTRACE(__FUNCTION__);

    RAWTRACE("Initializing API");

    Init_WebView_API();
    RHODESAPP().getExtManager().requireRubyFile("RhoWebViewApi");

    RAWTRACE1("%s succeeded", __FUNCTION__);
}
