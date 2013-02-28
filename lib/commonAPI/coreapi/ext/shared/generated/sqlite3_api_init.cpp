#include "common/app_build_capabilities.h"

extern "C" void Init_RubyAPI_SQLite3();
extern "C" void Init_JSAPI_SQLite3();

extern "C" void Init_SQLite3_API()
{
#ifndef RHO_NO_RUBY_API
    Init_RubyAPI_SQLite3();
#endif

#ifndef RHO_NO_JS_API
    Init_JSAPI_SQLite3();
#endif
}
