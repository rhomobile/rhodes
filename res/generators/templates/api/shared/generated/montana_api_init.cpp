#include "common/app_build_capabilities.h"


extern "C" void Init_JSAPI_<%= $cur_module.name %>();
#ifndef RHO_NO_RUBY_API
extern "C" void Init_RubyAPI_<%= $cur_module.name %>();
extern "C" int  rho_ruby_is_started();
#endif

extern "C" void Init_<%= $cur_module.name %>_API()
{
#ifndef RHO_NO_RUBY_API
    if (rho_ruby_is_started())
    {
    	Init_RubyAPI_<%= $cur_module.name %>();
    }
#endif

#ifndef RHO_NO_JS_API
    Init_JSAPI_<%= $cur_module.name %>();
#endif
}
