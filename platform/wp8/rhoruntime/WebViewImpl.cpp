#include "rhoruntime.h"
#include "common/RhoStd.h"
#include "common/StringConverter.h"
#include "common/RhodesApp.h"
#include "ruby/ext/rho/rhoruby.h"
#include "common/RhoConvertWP8.h"

using namespace rhoruntime;
using namespace Platform;

extern "C" void rho_webview_navigate_back()
{
	CRhoRuntime::getInstance()->getMainPage()->GoBack();
}

extern "C" int rho_webview_active_tab() 
{
	return CRhoRuntime::getInstance()->getMainPage()->tabbarGetCurrent();
}

extern "C" void rho_webview_navigate_forward()
{
    CRhoRuntime::getInstance()->getMainPage()->GoForward();
}

extern "C" const char* rho_webview_execute_js(const char* js, int index) 
{
    rho::common::convertStringAFromWP8(CRhoRuntime::getInstance()->getMainPage()->executeScriptAsync(rho::common::convertStringCToWP8(js), index));
    return "";
}

extern "C" const char* rho_webview_execute_js_sync(const char* js, int index) 
{
    rho::String urlA = rho::common::convertStringAFromWP8(CRhoRuntime::getInstance()->getMainPage()->executeScript(rho::common::convertStringCToWP8(js), index));
    return strdup(urlA.c_str());
}

extern "C" const char* rho_webview_current_location(int index)
{
	rho::String urlA = rho::common::convertStringAFromWP8(CRhoRuntime::getInstance()->getMainPage()->getCurrentURL(index));
	return strdup(urlA.c_str());
}

extern "C" unsigned long rho_webview_get_current_url(int index)
{
	return rho_ruby_create_string(rho_webview_current_location(index));
}

extern "C" void rho_webview_full_screen_mode(int enable)
{
	CRhoRuntime::getInstance()->getMainPage()->fullscreenCommand(enable);
}

extern "C" void rho_webview_set_cookie(const char *url, const char *cookie)
{
	rho::String fullUrl = RHODESAPPBASE().canonicalizeRhoUrl(url);
	CRhoRuntime::getInstance()->getMainPage()->setCookie(rho::common::convertStringToWP8(fullUrl), rho::common::convertStringToWP8(cookie));
}

extern "C" void waitForBrowserInitialized(int index)
{
	while(true)
	{
		
		if(CRhoRuntime::getInstance()->getMainPage()->isBrowserInitialized(index))
			return;
		else
			Sleep(100);
	}
}

extern "C" void rho_webview_navigate(const char* url, int index)
{ 
	//waitForBrowserInitialized(index);
	rho::String fullUrl = RHODESAPPBASE().canonicalizeRhoUrl(url);
	LOG(INFO) + fullUrl.c_str();
	CRhoRuntime::getInstance()->getMainPage()->navigate(rho::common::convertStringToWP8(fullUrl), index);
}

extern "C" void rho_webview_refresh(int index)
{
	CRhoRuntime::getInstance()->getMainPage()->Refresh(index);
}

extern "C" HINSTANCE rho_wmimpl_get_appinstance()
{
    return 0;//_AtlModule.GetModuleInstance();
}

extern "C" void rho_webview_save(const char* format, const char* path, int tab_index)
{
}
