#include "api_generator/js_helpers.h"

#include "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "NetworkAccess"


rho::String js_s_NetworkAccess_cancel(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_s_NetworkAccess_downloadFile(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_s_NetworkAccess_get(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_s_NetworkAccess_post(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_s_NetworkAccess_uploadFile(rho::json::CJSONArray& argv, const rho::String& strObjID);





extern "C" void Init_JSAPI_NetworkAccess(void)
{

    rho::apiGenerator::js_define_method("Rho:NetworkAccess:cancel", js_s_NetworkAccess_cancel);

    rho::apiGenerator::js_define_method("Rho:NetworkAccess:downloadFile", js_s_NetworkAccess_downloadFile);

    rho::apiGenerator::js_define_method("Rho:NetworkAccess:get", js_s_NetworkAccess_get);

    rho::apiGenerator::js_define_method("Rho:NetworkAccess:post", js_s_NetworkAccess_post);

    rho::apiGenerator::js_define_method("Rho:NetworkAccess:uploadFile", js_s_NetworkAccess_uploadFile);


}

