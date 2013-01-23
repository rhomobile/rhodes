#include "common/app_build_capabilities.h"

extern "C" void Init_RubyAPI_<%= $cur_module.name %>();
extern "C" void Init_JSAPI_<%= $cur_module.name %>();

void Init_<%= $cur_module.name %>_API()
{
#ifndef RHO_NO_RUBY_API
    Init_RubyAPI_<%= $cur_module.name %>();
#endif

#ifndef RHO_NO_JS_API
    Init_JSAPI_<%= $cur_module.name %>();
#endif
}
