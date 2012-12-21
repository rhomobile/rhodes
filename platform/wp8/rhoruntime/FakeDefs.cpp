
#include <windows.h>
#include <string>
//#include "../../shared/ruby/include/ruby.h"
#include "../../shared/common/RhoDefs.h"
#include <logging/RhoLog.h>

extern "C" HINSTANCE rho_wmimpl_get_appinstance()
{
    return 0;//_AtlModule.GetModuleInstance();
}

//void rho_webview_set_menu_items(VALUE valMenu) 
//{
//    //rho_rhodesapp_setViewMenu(valMenu); 
//}

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

extern "C" const char* rho_native_rhopath()
{
    return "rho/";
}

extern "C" const char* rho_native_get_appname()
{
    return "Rhodes";
}

RHO_GLOBAL const char* rho_native_reruntimepath()
{
    return "rho/";
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