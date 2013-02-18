#include "common/app_build_capabilities.h"

extern "C" void Init_RubyAPI_SimpleOnlyStaticModule();
extern "C" void Init_JSAPI_SimpleOnlyStaticModule();

extern "C" void Init_SimpleOnlyStaticModule_API()
{
#ifndef RHO_NO_RUBY_API
    Init_RubyAPI_SimpleOnlyStaticModule();
#endif

#ifndef RHO_NO_JS_API
    Init_JSAPI_SimpleOnlyStaticModule();
#endif
}
