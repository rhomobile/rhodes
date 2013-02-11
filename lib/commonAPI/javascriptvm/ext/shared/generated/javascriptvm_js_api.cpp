#include "api_generator/js_helpers.h"

#include "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "JavascriptVM"


rho::String js_s_JavascriptVM_executeScript(rho::json::CJSONArray& argv, const rho::String& strObjID);





extern "C" void Init_JSAPI_JavascriptVM(void)
{

    rho::apiGenerator::js_define_method("Rho:JavascriptVM:executeScript", js_s_JavascriptVM_executeScript);


}

