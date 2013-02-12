#include "api_generator/js_helpers.h"

#include "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "Process"


rho::String js_s_Process_runApplication(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_Process_waitForApplication(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_Process_closeHandle(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_Process_getProcessExitCode(rho::json::CJSONArray& argv, const rho::String& strObjID);





extern "C" void Init_JSAPI_Process(void)
{

    rho::apiGenerator::js_define_method("Rho:System:Process:runApplication", js_s_Process_runApplication);

    rho::apiGenerator::js_define_method("Rho:System:Process:waitForApplication", js_Process_waitForApplication);

    rho::apiGenerator::js_define_method("Rho:System:Process:closeHandle", js_Process_closeHandle);

    rho::apiGenerator::js_define_method("Rho:System:Process:getProcessExitCode", js_Process_getProcessExitCode);


}

