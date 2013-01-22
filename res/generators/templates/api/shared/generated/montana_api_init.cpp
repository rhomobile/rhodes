#include "common/app_build_capabilities.h"

extern "C" void Init_RubyAPI_<%= $name %>();
extern "C" void Init_JSAPI_<%= $name %>();

void Init_<%= $name %>_API()
{
#ifndef RHO_NO_RUBY_API
    Init_RubyAPI_<%= $name %>();
#endif

#ifndef RHO_NO_JS_API
    Init_JSAPI_<%= $name %>();
#endif
}
