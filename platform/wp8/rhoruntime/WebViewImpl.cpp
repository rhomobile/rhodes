#include "rhoruntime.h"
#include "common/RhoStd.h"
#include "common/StringConverter.h"

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
	// TODO: implement return value
    return "";
}

extern "C" const char* rho_webview_get_current_url(int index)
{
	rho::String urlA = rho::common::convertToStringA(CRhoRuntime::getInstance()->getMainPage()->getCurrentURL(index)->Data());
	return urlA.c_str();
}

extern "C" const char* rho_webview_current_location(int index)
{
	return rho_webview_get_current_url(index);
}

extern "C" void rho_webview_full_screen_mode(int enable)
{
	CRhoRuntime::getInstance()->getMainPage()->fullscreenCommand(enable);
}

extern "C" void rho_webview_set_cookie(const char *url, const char *cookie)
{
	rho::StringW urlW;
	rho::StringW cookieW;
	rho::common::convertToStringW(url, urlW);
	rho::common::convertToStringW(cookie, cookieW);
	CRhoRuntime::getInstance()->getMainPage()->setCookie(ref new String(urlW.c_str()), ref new String(cookieW.c_str()));
}

extern "C" void rho_webview_navigate(const char* url, int index)
{ 
	rho::StringW urlW;
	rho::common::convertToStringW(url, urlW);
	CRhoRuntime::getInstance()->getMainPage()->navigate(ref new String(urlW.c_str()), index);
}

extern "C" void rho_webview_refresh(int index)
{
	CRhoRuntime::getInstance()->getMainPage()->Refresh(index);
}
