
#include <windows.h>
//#include "../../shared/ruby/include/ruby.h"
#include "../../shared/common/RhoDefs.h"



extern "C" HINSTANCE rho_wmimpl_get_appinstance()
{
    return 0;//_AtlModule.GetModuleInstance();
}

extern "C" int rho_webview_active_tab() 
{
    return 0;// CMainWindow::getInstance()->tabbarGetCurrent();
}

extern "C" void rho_webview_navigate_back()
{
    //CMainWindow::getInstance()->navigateBackCommand();
}

extern "C" void rho_webview_navigate_forward()
{
    //CMainWindow::getInstance()->navigateForwardCommand();
}

extern "C" const char* rho_webview_execute_js(const char* js, int index) 
{
    return "";
}

extern "C" const char* rho_webview_current_location(int index) 
{
    return "";//RHODESAPP().getCurrentUrl(index).c_str();
}

//void rho_webview_set_menu_items(VALUE valMenu) 
//{
//    //rho_rhodesapp_setViewMenu(valMenu); 
//}

extern "C" void rho_webview_full_screen_mode(int enable)
{
    //CMainWindow::getInstance()->fullscreenCommand(enable);
}

extern "C" void rho_webview_set_cookie(const char *url, const char *cookie)
{
    //CMainWindow::getInstance()->setCookie(url, cookie);
}

RHO_GLOBAL void rho_map_location(char* query)
{
}

RHO_GLOBAL void rho_appmanager_load( void* /*httpContext*/, const char* /*szQuery*/)
{
    // Nothing
}

RHO_GLOBAL void rho_sys_app_exit()
{
}

RHO_GLOBAL void rho_sys_report_app_started()
{
}

RHO_GLOBAL void rho_conf_show_log()
{
}

RHO_GLOBAL const char* rho_native_rhopath()
{
    return "";//rho_root_path().c_str();
}

extern "C" void rho_webview_navigate(const char* url, int index)
{ 
}

RHO_GLOBAL void rho_webview_refresh(int index)
{
}

RHO_GLOBAL const char* rho_native_reruntimepath()
{
    return "";//rho_root_path().c_str();
}

RHO_GLOBAL HWND getMainWnd()
{
	return 0;
}

RHO_GLOBAL double rho_geo_latitude()
{
	return 0;
}

RHO_GLOBAL double rho_geo_longitude()
{
	return 0;
}

RHO_GLOBAL float rho_geo_accuracy()
{
	return 0;
}

RHO_GLOBAL int rho_geo_known_position()
{
	return 0;
}

RHO_GLOBAL void rho_geoimpl_settimeout(int nTimeoutSec)
{
	
}

RHO_GLOBAL int rho_geo_is_available()
{
	return 0;
}

RHO_GLOBAL void alert_show_status(const char* szTitle, const char* szMessage, const char* szHide)
{
}