#include "common/app_build_capabilities.h"

extern "C" void Init_RubyAPI_Megamodule();
extern "C" void Init_JSAPI_Megamodule();

extern "C" void Init_Megamodule_API()
{
#ifndef RHO_NO_RUBY_API
    Init_RubyAPI_Megamodule();
#endif

#ifndef RHO_NO_JS_API
    Init_JSAPI_Megamodule();
#endif
}
