#include "StdAfx.h"

#include <algorithm>

#include <common/rhoconf.h>

#include "BrowserFactory.h"
#include "CEBrowserEngine.h"
#include "IEBrowserEngine.h"
#include "EngineEventListner.h"
#include <common/RhodesApp.h>

//////////////////////////////////////////////////////////////////////////

extern "C" HINSTANCE rho_wmimpl_get_appinstance();
extern "C" const char* rho_wmimpl_get_webengine();
extern "C" const char* get_app_build_config_item(const char* key);
extern rho::IBrowserEngine* rho_wmimpl_get_webkitBrowserEngine(HWND hwndParent, HINSTANCE rhoAppInstance);

//////////////////////////////////////////////////////////////////////////

const char* rho::BrowserFactory::IETag     = "ie";
const char* rho::BrowserFactory::webkitTag = "webkit";

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
	RHODESAPP().getExtManager().getEngineEventMngr().setEngineType(rho::engineeventlistner::eWebkit);
	return rho_wmimpl_get_webkitBrowserEngine(hwndParent, rho_wmimpl_get_appinstance());
}

IBrowserEngine* BrowserFactory::createIE(HWND hwndParent)
{
    if (RHO_IS_WMDEVICE)
    {
		RHODESAPP().getExtManager().getEngineEventMngr().setEngineType(rho::engineeventlistner::eWmIe);
		return CIEBrowserEngine::getInstance(hwndParent, rho_wmimpl_get_appinstance());
    }
    else
    {
		RHODESAPP().getExtManager().getEngineEventMngr().setEngineType(rho::engineeventlistner::eCeIe);
        return new CEBrowserEngine(hwndParent, rho_wmimpl_get_appinstance());
    }
}

EBrowserEngineType BrowserFactory::convertBrowserType(rho::String browserType)
{
    rho::String browserTypeTag;
    
    std::transform(browserType.begin(), browserType.end(), 
        std::back_inserter(browserTypeTag), ::tolower);

    if (browserTypeTag == String(IETag))
    {
        return eIE;
    }
    else if (browserTypeTag == String(webkitTag))
    {
        return eWebkit;
    }

    return eNone;
}

IBrowserEngine* BrowserFactory::create(HWND hwndParent)
{    
    EBrowserEngineType selBrowserType  = eNone;
    String             buildConfigType = ""; 
    //TODO TAU
	//String             xmlConfigType   = rho_wmimpl_get_webengine();
	String             xmlConfigType;
    String             rhoConfigType   = RHOCONF().getString("webengine");
    
    if (get_app_build_config_item("webengine"))
    {
        buildConfigType = get_app_build_config_item("webengine");
    }

    if (buildConfigType.empty())
    {
        if (xmlConfigType.empty())
        {
            selBrowserType = convertBrowserType(rhoConfigType);
        }
        else
        {
            selBrowserType = convertBrowserType(xmlConfigType);
        }
    }
    else
    {
        selBrowserType = convertBrowserType(buildConfigType);
    }

    if (selBrowserType == eNone)
    {
        selBrowserType = eIE;
        LOG(INFO) + "WebKit extension missing. Selected Internet Explorer engine automatically.";
    } 

    m_selBrowserType = selBrowserType;

    switch (selBrowserType)
    {
    case eWebkit:
        LOG(INFO) + "Webkit browser engine was created.";
        return createWebkit(hwndParent);
        break;    
    case eIE:
        LOG(INFO) + "IE browser engine was created.";
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
