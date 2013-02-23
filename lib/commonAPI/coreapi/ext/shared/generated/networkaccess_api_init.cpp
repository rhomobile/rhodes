#include "common/app_build_capabilities.h"

extern "C" void Init_RubyAPI_NetworkAccess();
extern "C" void Init_JSAPI_NetworkAccess();

extern "C" void Init_NetworkAccess_API()
{
#ifndef RHO_NO_RUBY_API
    Init_RubyAPI_NetworkAccess();
#endif

#ifndef RHO_NO_JS_API
    Init_JSAPI_NetworkAccess();
#endif
}
