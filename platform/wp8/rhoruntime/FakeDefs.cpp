
#include <windows.h>
#include <string>
#include "../../shared/ruby/ext/rho/rhoruby.h"
#include "../../shared/common/RhoDefs.h"
#include <logging/RhoLog.h>

//NavBar - iphone only
RHO_GLOBAL void create_navbar(rho_param *p)
{
}

RHO_GLOBAL void remove_navbar()
{
}

RHO_GLOBAL VALUE navbar_started()
{
    return rho_ruby_create_boolean(0);
}


RHO_GLOBAL void rho_map_location(char* query)
{
}

RHO_GLOBAL void rho_appmanager_load( void* /*httpContext*/, const char* /*szQuery*/)
{
    // Nothing
}

RHO_GLOBAL void rho_sys_report_app_started()
{
}

RHO_GLOBAL void rho_conf_show_log()
{
}

/*RHO_GLOBAL const char* rho_sys_get_http_proxy_url()
{
	return "";
}

RHO_GLOBAL void rho_sys_set_http_proxy_url(const char* url)
{
}

RHO_GLOBAL void rho_sys_unset_http_proxy()
{
}*/

RHO_GLOBAL HWND getMainWnd()
{
	return 0;
}


RHO_GLOBAL void rho_platform_restart_application()
{
}

RHO_GLOBAL double rho_geo_speed() {
    return 0.0;
}

RHO_GLOBAL int rho_geo_satellites() {
	return 0;    
}

RHO_GLOBAL void rho_geo_set_notification_ex(const char *url, rho_param* p, char* params) {
   
}
