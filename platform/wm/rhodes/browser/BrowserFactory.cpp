#include "StdAfx.h"
#include "BrowserFactory.h"

#include "CEBrowserEngine.h"
#include "IEBrowserEngine.h"

extern "C" HINSTANCE rho_wmimpl_get_appinstance();
extern rho::IBrowserEngine* rho_wmimpl_get_webkitBrowserEngine(HWND hwndParent, HINSTANCE rhoAppInstance);

namespace rho
{

BrowserFactory* BrowserFactory::g_BrowserFactory = 0;

IBrowserFactory* BrowserFactory::getInstance()
{
    if (g_BrowserFactory == 0)
    {
        g_BrowserFactory = new BrowserFactory();
    }

    return g_BrowserFactory;
}

IBrowserEngine* BrowserFactory::create(HWND hwndParent)
{
#if defined(APP_BUILD_CAPABILITY_WEBKIT_BROWSER)
    return rho_wmimpl_get_webkitBrowserEngine(hwndParent, rho_wmimpl_get_appinstance());
#elif defined(OS_PLATFORM_MOTCE)
    return new CEBrowserEngine(hwndParent, rho_wmimpl_get_appinstance());
#else
    return CIEBrowserEngine::getInstance(hwndParent, rho_wmimpl_get_appinstance());
#endif //APP_BUILD_CAPABILITY_WEBKIT_BROWSER
}

}