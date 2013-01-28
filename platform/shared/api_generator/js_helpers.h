#pragma once

#include "json/JSONIterator.h"

typedef rho::String (*Func_JS)(rho::json::CJSONArray& argv, const rho::String& strObjID );

void js_define_method(const char* szMethodPath, Func_JS pFunc );
rho::String js_entry_point(const char* szJSON);
void js_register_http_entry_point();

