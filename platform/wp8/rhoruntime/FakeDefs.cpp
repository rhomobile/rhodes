
#include <windows.h>
#include <string>
#include "../../shared/ruby/ext/rho/rhoruby.h"
#include "../../shared/common/RhoDefs.h"
#include <logging/RhoLog.h>


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

RHO_GLOBAL const char* rho_sys_get_http_proxy_url()
{
	return "";
}

RHO_GLOBAL void rho_sys_set_http_proxy_url(const char* url)
{
}

RHO_GLOBAL void rho_sys_unset_http_proxy()
{
}

RHO_GLOBAL HWND getMainWnd()
{
	return 0;
}


RHO_GLOBAL void rho_platform_restart_application()
{
}