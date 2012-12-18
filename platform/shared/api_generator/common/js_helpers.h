#pragma once

#include "json/JSONIterator.h"

typedef rho::String (*Func_JS)(const rho::String& strID, rho::json::CJSONEntry& oParams );

void js_define_method(const char* szMethodPath, Func_JS pFunc );
rho::String js_entry_point(const char* szJSON);


