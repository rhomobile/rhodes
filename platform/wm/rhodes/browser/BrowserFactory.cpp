#include "StdAfx.h"
#include "BrowserFactory.h"

#include "CEBrowserEngine.h"
#include "IEBrowserEngine.h"

extern "C" HINSTANCE rho_wmimpl_get_appinstance();
extern rho::IBrowserEngine* rho_wmimpl_get_webkitBrowserEngine(HWND hwndParent, HINSTANCE rhoAppInstance);

namespace rho
{

BrowserFactory* BrowserFactory::g_browserFactory = 0;

IBrowserFactory* BrowserFactory::getInstance()
{
    if (g_browserFactory == 0)
    {
        g_browserFactory = new BrowserFactory();
    }

    return g_browserFactory;
}

IBrowserEngine* BrowserFactory::create(HWND hwndParent)
{
//#if defined(APP_BUILD_CAPABILITY_WEBKIT_BROWSER)
//    return rho_wmimpl_get_webkitBrowserEngine(hwndParent, rho_wmimpl_get_appinstance());
//#elif defined(OS_PLATFORM_MOTCE)
//    return new CEBrowserEngine(hwndParent, rho_wmimpl_get_appinstance());
//#else
    return CIEBrowserEngine::getInstance(hwndParent, rho_wmimpl_get_appinstance());
//#endif //APP_BUILD_CAPABILITY_WEBKIT_BROWSER
}

//
EBrowserEngine BrowserFactory::getCurrentBrowserType()
{
    //#if defined(APP_BUILD_CAPABILITY_WEBKIT_BROWSER)
    //    return eWebkit;
    //#elif defined(OS_PLATFORM_MOTCE)
    //    return eIE_CE;
    //#else
        return eIE_WM;
    //#endif 

    return eNone;
}

}

extern "C" bool rho_wmimpl_is_browser_ieforwm()
{
    return (bool)(rho::eIE_WM == rho::BrowserFactory::getCurrentBrowserType());
}