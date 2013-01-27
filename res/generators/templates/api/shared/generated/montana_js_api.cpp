#include "api_generator/js_helpers.h"

#include "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "<%= $cur_module.name %>"

<% $cur_module.methods.each do |module_method|%>
<%= api_generator_MakeJSMethodDecl($cur_module.name, module_method)
%>;<% end %>
<% 
def api_generator_MakeJSMethodDef(module_method)
    method_name = 'js_' + $cur_module.name + "_" + module_method.name

    "    js_define_method(\"#{$cur_module.parent}:#{$cur_module.name}:#{module_method.name}\", #{method_name});"
end

%>
extern "C" void Init_JSAPI_<%= $cur_module.name %>(void)
{
<% $cur_module.methods.each do |module_method|%>
<%= api_generator_MakeJSMethodDef(module_method)
 %><% end %>
}

