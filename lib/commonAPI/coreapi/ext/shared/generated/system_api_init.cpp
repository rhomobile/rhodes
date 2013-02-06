#include "common/app_build_capabilities.h"

extern "C" void Init_RubyAPI_System();
extern "C" void Init_JSAPI_System();

extern "C" void Init_System_API()
{
#ifndef RHO_NO_RUBY_API
    Init_RubyAPI_System();
#endif

#ifndef RHO_NO_JS_API
    Init_JSAPI_System();
#endif
}
