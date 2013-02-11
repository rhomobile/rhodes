#include "common/app_build_capabilities.h"

extern "C" void Init_RubyAPI_JavascriptVM();
extern "C" void Init_JSAPI_JavascriptVM();

extern "C" void Init_JavascriptVM_API()
{
#ifndef RHO_NO_RUBY_API
    Init_RubyAPI_JavascriptVM();
#endif

#ifndef RHO_NO_JS_API
    Init_JSAPI_JavascriptVM();
#endif
}
