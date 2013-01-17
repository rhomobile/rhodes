#include "rhoruntime.h"
#include "common/RhoStd.h"
#include "common/StringConverter.h"
#include "common/RhodesApp.h"
#include "ruby/ext/rho/rhoruby.h"

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
	rho::StringW jsW;
	rho::common::convertToStringW(js, jsW);
	CRhoRuntime::getInstance()->getMainPage()->executeScript(ref new String(jsW.c_str()), index);
	// TODO: implement return value of rho_webview_execute_js
    return "";
}

extern "C" const char* rho_webview_current_location(int index)
{
	rho::String urlA = rho::common::convertToStringA(CRhoRuntime::getInstance()->getMainPage()->getCurrentURL(index)->Data());
	return urlA.c_str();
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
	rho::StringW urlW;
	rho::StringW cookieW;

	rho::String fullUrl = RHODESAPPBASE().canonicalizeRhoUrl(url);
	rho::common::convertToStringW(fullUrl.c_str(), urlW);
	rho::common::convertToStringW(cookie, cookieW);

	CRhoRuntime::getInstance()->getMainPage()->setCookie(ref new String(urlW.c_str()), ref new String(cookieW.c_str()));
}

extern "C" void rho_webview_navigate(const char* url, int index)
{ 
	rho::StringW urlW;
	
	rho::String fullUrl = RHODESAPPBASE().canonicalizeRhoUrl(url);
	rho::common::convertToStringW(fullUrl.c_str(), urlW);

	CRhoRuntime::getInstance()->getMainPage()->navigate(ref new String(urlW.c_str()), index);
}

extern "C" void rho_webview_refresh(int index)
{
	CRhoRuntime::getInstance()->getMainPage()->Refresh(index);
}

extern "C" HINSTANCE rho_wmimpl_get_appinstance()
{
    return 0;//_AtlModule.GetModuleInstance();
}

extern "C" void rho_webview_set_menu_items(VALUE valMenu) 
{
    rho_rhodesapp_setViewMenu(valMenu);
	CRhoRuntime^ runtime = CRhoRuntime::getInstance();
	if (runtime != nullptr)
		runtime->createCustomMenu();
}

extern "C" void rho_webview_save(const char* format, const char* path, int tab_index)
{
}
