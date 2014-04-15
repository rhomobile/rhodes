#pragma once

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
