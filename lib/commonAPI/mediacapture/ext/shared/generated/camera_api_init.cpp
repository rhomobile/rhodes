#include "common/app_build_capabilities.h"

extern "C" void Init_RubyAPI_Camera();
extern "C" void Init_JSAPI_Camera();

extern "C" void Init_Camera_API()
{
#ifndef RHO_NO_RUBY_API
    Init_RubyAPI_Camera();
#endif

#ifndef RHO_NO_JS_API
    Init_JSAPI_Camera();
#endif
}
