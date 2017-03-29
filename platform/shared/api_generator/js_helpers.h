#pragma once


#ifdef __cplusplus

#include "json/JSONIterator.h"

namespace rho
{
namespace apiGenerator
{

template <typename T> class ApiHandler;

typedef rho::String (*Func_JS)(const String& strObjID, rho::json::CJSONArray& argv, const String& strCallbackID, const String& strJsVmID, const String& strCallbackParam);

void defineJSApiModule(ApiHandler<Func_JS>*);

rho::String js_entry_point(const char* szJSON);
void rho_http_js_entry_point(void *arg, rho::String const &query );

}
}

#endif
//__cplusplus


#ifdef __cplusplus
extern "C"{
#endif //__cplusplus
    
typedef void (*JVM_Callback_Provider)(const char* js_code);
    
void setCustomJVMCallbackProvider(const char* jvm_id, JVM_Callback_Provider provider);

//return 1 if callback called (has JVM with defined ID
// if not found return zero
int callCustomJVMCallbackProvider(const char* jvm_id, const char* js_code);
    
#ifdef __cplusplus
}
#endif //__cplusplus
