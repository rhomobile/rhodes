#include "common/RhoPort.h"
#include "ruby/ext/rho/rhoruby.h"
#include "sync/ClientRegister.h"
#include "common/RhodesApp.h"
#include "common/RhoConf.h"
#include "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "RhoSystem"

extern "C"
{
extern int rho_sysimpl_get_property(char* szPropName, VALUE* resValue);
extern VALUE rho_sys_has_network();
extern VALUE rho_sys_get_locale();
extern int rho_sys_get_screen_width();
extern int rho_sys_get_screen_height();
#ifdef RHODES_EMULATOR
int rho_simimpl_get_property(char* szPropName, VALUE* resValue)
{
	if (strcasecmp("os_version",szPropName) == 0)
	{
        *resValue = rho_ruby_create_string( RHOSIMCONF().getString("os_version").c_str());
		return 1;
	}

	if (strcasecmp("is_emulator",szPropName) == 0)
    {
        *resValue = rho_ruby_create_boolean(1);
        return 1;
    }

    return 0;
}
#endif

VALUE rho_sys_get_property(char* szPropName) 
{
	if (!szPropName || !*szPropName) 
        return rho_ruby_get_NIL();
    
    VALUE res;
#ifdef RHODES_EMULATOR
    if (rho_simimpl_get_property(szPropName, &res))
        return res;
#endif

    if (rho_sysimpl_get_property(szPropName, &res))
        return res;

	if (strcasecmp("platform",szPropName) == 0) 
        return rho_ruby_create_string(rho_rhodesapp_getplatform());

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

    if (strcasecmp("phone_id", szPropName) == 0)
        return rho_ruby_create_string(""); 

	if (strcasecmp("full_browser",szPropName) == 0) 
        return rho_ruby_create_boolean(1);

	if (strcasecmp("rhodes_port",szPropName) == 0) 
        return rho_ruby_create_integer(atoi(RHODESAPP().getFreeListeningPort()));

	if (strcasecmp("is_emulator",szPropName) == 0) 
        return rho_ruby_create_boolean(0);

	if (strcasecmp("has_touchscreen",szPropName) == 0)
        return rho_ruby_create_boolean(1);

	if (strcasecmp("has_sqlite",szPropName) == 0)
        return rho_ruby_create_boolean(1);

    RAWLOG_ERROR1("Unknown Rho::System property : %s", szPropName);

    return rho_ruby_get_NIL();
}

void rho_sys_set_push_notification(const char *url, const char* params)
{
    RHODESAPP().setPushNotification(url, params);
}

void rho_sys_set_screen_rotation_notification(const char *url, const char* params)
{
    rho::String s_url = "";
    rho::String s_params = "";
    if (url != NULL) {
        s_url = url;
    }
    if (params != NULL) {
        s_params = params;
    }
	RHODESAPP().setScreenRotationNotification(s_url, s_params);
}

void rho_sys_unzip_file(const char *url)
{
    rho_unzip_file(url);
}

#if defined(OS_MACOSX) || defined(OS_ANDROID)
  // implemented in platform code
#else
int rho_sys_set_sleeping(int sleeping)
{
    return 1;
}
#endif //defined(OS_MACOSX) || defined(OS_ANDROID)

const char* rho_sys_get_start_params() 
{
    return rho::common::CRhodesApp::getStartParameters().c_str();
}

void rho_sys_start_timer( int interval, const char *url, const char* params)
{
    RHODESAPP().getTimer().addTimer(interval, url, params);
}


void rho_sys_stop_timer( const char *url )
{
    RHODESAPP().getTimer().stopTimer(url);
}

} //extern "C"
