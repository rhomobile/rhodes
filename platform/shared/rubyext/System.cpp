#include "common/RhoPort.h"
#include "ruby/ext/rho/rhoruby.h"
#include "sync/ClientRegister.h"
#include "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "RhoSystem"

extern "C"
{
extern VALUE rho_sysimpl_get_property(char* szPropName);
extern VALUE rho_sys_has_network();
extern VALUE rho_sys_get_locale();
extern int rho_sys_get_screen_width();
extern int rho_sys_get_screen_height();

static const char* getPlatformProp()
{
#if defined(OS_MACOSX)
	return "APPLE";
#elif defined(OS_WINDOWS) || defined (OS_WINCE)
	return "WINDOWS";
#elif defined(OS_SYMBIAN)
	return "SYMBIAN";
#elif defined(OS_ANDROID)
    return "ANDROID";
#elif defined(OS_LINUX)
    return "LINUX";
#else
	return "UNKNOWN";
#endif			
}

VALUE rho_sys_get_property(char* szPropName) 
{
	if (!szPropName || !*szPropName) 
        return rho_ruby_get_NIL();
    
    VALUE res = rho_sysimpl_get_property(szPropName);
    if (res)
        return res;

	if (strcasecmp("platform",szPropName) == 0) 
        return rho_ruby_create_string(getPlatformProp());

	if (strcasecmp("has_network",szPropName) == 0) 
        return rho_sys_has_network();

	if (strcasecmp("locale",szPropName) == 0) 
        return rho_sys_get_locale();

	if (strcasecmp("screen_width",szPropName) == 0) 
        return rho_ruby_create_integer(rho_sys_get_screen_width());

	if (strcasecmp("screen_height",szPropName) == 0) 
        return rho_ruby_create_integer(rho_sys_get_screen_height());

	if (strcasecmp("device_id",szPropName) == 0) 
    {
        rho::String strDeviceID = "";
        if ( rho::sync::CClientRegister::getInstance() )
            strDeviceID = rho::sync::CClientRegister::getInstance()->getDevicePin();

        return rho_ruby_create_string(strDeviceID.c_str());
    }

	if (strcasecmp("full_browser",szPropName) == 0) 
        return rho_ruby_create_boolean(1);

    RAWLOG_ERROR1("Unknown Rho::System property : %s", szPropName);

    return rho_ruby_get_NIL();
}

}
