#include "common/app_build_capabilities.h"

extern "C" void Init_RubyAPI_Process();
extern "C" void Init_JSAPI_Process();

extern "C" void Init_Process_API()
{
#ifndef RHO_NO_RUBY_API
    Init_RubyAPI_Process();
#endif

#ifndef RHO_NO_JS_API
    Init_JSAPI_Process();
#endif
}
