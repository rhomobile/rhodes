#include "StdAfx.h"

#include <common/rhoconf.h>

#include "BrowserFactory.h"
#include "CEBrowserEngine.h"
#include "IEBrowserEngine.h"

//////////////////////////////////////////////////////////////////////////

extern "C" HINSTANCE rho_wmimpl_get_appinstance();
extern "C" const char* rho_wmimpl_get_webengine();

extern rho::IBrowserEngine* rho_wmimpl_get_webkitBrowserEngine(HWND hwndParent, HINSTANCE rhoAppInstance);

//////////////////////////////////////////////////////////////////////////

const char* rho::BrowserFactory::IETag = "IE";
const char* rho::BrowserFactory::webkitTag = "WebKit";

//////////////////////////////////////////////////////////////////////////

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

IBrowserEngine* BrowserFactory::createWebkit(HWND hwndParent)
{
    return rho_wmimpl_get_webkitBrowserEngine(hwndParent, rho_wmimpl_get_appinstance());
}

IBrowserEngine* BrowserFactory::createIE(HWND hwndParent)
{
    if (RHO_IS_WMDEVICE)
    {
        return CIEBrowserEngine::getInstance(hwndParent, rho_wmimpl_get_appinstance());
    }
    else
    {
        return new CEBrowserEngine(hwndParent, rho_wmimpl_get_appinstance());
    }
}

EBrowserEngineType BrowserFactory::convertBrowserType(rho::String browserType)
{
    rho::String browserTypeTag = browserType; //TODO - convert to uppercase
    
    if (browserTypeTag == String(IETag))
    {
        return eIE;
    }
    else if (browserTypeTag == webkitTag)
    {
        return eWebkit;
    }

    return eNone;
}

IBrowserEngine* BrowserFactory::create(HWND hwndParent)
{
    EBrowserEngineType selBrowserType = eNone;
    String             xmlConfigType  = rho_wmimpl_get_webengine();
    String             rhoConfigType  = RHOCONF().getString("webengine");

    if (xmlConfigType.empty())
    {
        selBrowserType = convertBrowserType(rhoConfigType);
    }
    else
    {
        selBrowserType = convertBrowserType(xmlConfigType);
    }

    if (selBrowserType == eNone)
    {
        selBrowserType = eWebkit;
        LOG(INFO) + "Browser engine was not set in config`s. Selected Webkit engine automatically.";
    } 

    m_selBrowserType = selBrowserType;

    switch (selBrowserType)
    {
    case eWebkit:
        return createWebkit(hwndParent);
        break;    
    case eIE:
        return createIE(hwndParent);
        break;
    case eNone:
        LOG(ERROR) + "Browser engine was not selected.";
        break;
    }

    return 0;
}

EBrowserEngineType BrowserFactory::getBrowserType() const
{
    return m_selBrowserType;
}

EBrowserEngineType BrowserFactory::getCurrentBrowserType()
{
    if (getInstance())
    {
        return BrowserFactory::g_browserFactory->getBrowserType();
    }

    return eNone;
}

}//end of rho

//////////////////////////////////////////////////////////////////////////

extern "C" bool rho_wmimpl_is_browser_ieforwm()
{
    return (bool)(rho::eIE == rho::BrowserFactory::getCurrentBrowserType());
}