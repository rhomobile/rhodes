#include "api_generator/js_helpers.h"

#include "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "SimpleOnlyStaticModule"


rho::String js_s_SimpleOnlyStaticModule_calcSumm(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_s_SimpleOnlyStaticModule_joinStrings(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_s_SimpleOnlyStaticModule_getPlatform(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_s_SimpleOnlyStaticModule_showAlertFromUIThread(rho::json::CJSONArray& argv, const rho::String& strObjID);





extern "C" void Init_JSAPI_SimpleOnlyStaticModule(void)
{

    rho::apiGenerator::js_define_method("Rho:Examples:SimpleOnlyStaticModule:calcSumm", js_s_SimpleOnlyStaticModule_calcSumm);

    rho::apiGenerator::js_define_method("Rho:Examples:SimpleOnlyStaticModule:joinStrings", js_s_SimpleOnlyStaticModule_joinStrings);

    rho::apiGenerator::js_define_method("Rho:Examples:SimpleOnlyStaticModule:getPlatform", js_s_SimpleOnlyStaticModule_getPlatform);

    rho::apiGenerator::js_define_method("Rho:Examples:SimpleOnlyStaticModule:showAlertFromUIThread", js_s_SimpleOnlyStaticModule_showAlertFromUIThread);


}

