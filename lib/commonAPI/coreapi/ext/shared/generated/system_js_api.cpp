#include "api_generator/js_helpers.h"

#include "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "System"


rho::String js_System_getplatform(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_System_def_getplatform(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_System_setplatform(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_System_def_setplatform(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_System_applicationInstall(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_System_def_applicationInstall(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_System_isApplicationInstalled(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_System_def_isApplicationInstalled(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_System_applicationUninstall(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_System_def_applicationUninstall(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_System_getStartParams(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_System_def_getStartParams(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_System_openUrl(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_System_def_openUrl(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_System_getProperty(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_System_def_getProperty(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_System_getProperties(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_System_def_getProperties(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_System_getAllProperties(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_System_def_getAllProperties(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_System_setProperty(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_System_def_setProperty(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_System_setProperties(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_System_def_setProperties(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_System_clearAllProperties(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_System_def_clearAllProperties(rho::json::CJSONArray& argv, const rho::String& strObjID);



rho::String js_s_System_getDefaultID(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_System_setDefaultID(rho::json::CJSONArray& argv, const rho::String& strObjID);



extern "C" void Init_JSAPI_System(void)
{

    js_define_method("Rho:System:getplatform", js_System_getplatform);
    //  should define static method !     js_define_method("Rho:System:def_getplatform", js_s_System_def_getplatform);;

    js_define_method("Rho:System:setplatform", js_System_setplatform);
    //  should define static method !     js_define_method("Rho:System:def_setplatform", js_s_System_def_setplatform);;

    js_define_method("Rho:System:applicationInstall", js_System_applicationInstall);
    //  should define static method !     js_define_method("Rho:System:def_applicationInstall", js_s_System_def_applicationInstall);;

    js_define_method("Rho:System:isApplicationInstalled", js_System_isApplicationInstalled);
    //  should define static method !     js_define_method("Rho:System:def_isApplicationInstalled", js_s_System_def_isApplicationInstalled);;

    js_define_method("Rho:System:applicationUninstall", js_System_applicationUninstall);
    //  should define static method !     js_define_method("Rho:System:def_applicationUninstall", js_s_System_def_applicationUninstall);;

    js_define_method("Rho:System:getStartParams", js_System_getStartParams);
    //  should define static method !     js_define_method("Rho:System:def_getStartParams", js_s_System_def_getStartParams);;

    js_define_method("Rho:System:openUrl", js_System_openUrl);
    //  should define static method !     js_define_method("Rho:System:def_openUrl", js_s_System_def_openUrl);;

    js_define_method("Rho:System:getProperty", js_System_getProperty);
    //  should define static method !     js_define_method("Rho:System:def_getProperty", js_s_System_def_getProperty);;

    js_define_method("Rho:System:getProperties", js_System_getProperties);
    //  should define static method !     js_define_method("Rho:System:def_getProperties", js_s_System_def_getProperties);;

    js_define_method("Rho:System:getAllProperties", js_System_getAllProperties);
    //  should define static method !     js_define_method("Rho:System:def_getAllProperties", js_s_System_def_getAllProperties);;

    js_define_method("Rho:System:setProperty", js_System_setProperty);
    //  should define static method !     js_define_method("Rho:System:def_setProperty", js_s_System_def_setProperty);;

    js_define_method("Rho:System:setProperties", js_System_setProperties);
    //  should define static method !     js_define_method("Rho:System:def_setProperties", js_s_System_def_setProperties);;

    js_define_method("Rho:System:clearAllProperties", js_System_clearAllProperties);
    //  should define static method !     js_define_method("Rho:System:def_clearAllProperties", js_s_System_def_clearAllProperties);;


    js_define_method("Rho:System:getDefaultID", js_s_System_getDefaultID);
    js_define_method("Rho:System:setDefaultID", js_s_System_setDefaultID);

}

