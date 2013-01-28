#include "api_generator/js_helpers.h"

#include "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "<%= $cur_module.name %>"

<% $cur_module.methods.each do |module_method|%>
<%= api_generator_MakeJSMethodDecl($cur_module.name, module_method.name)
%>;<% end %>

<% if $cur_module.is_template_default_instance %>
<%= api_generator_MakeJSMethodDecl($cur_module.name, "getDefaultID") %>;
<%= api_generator_MakeJSMethodDecl($cur_module.name, "setDefaultID") %>;
<% end %>

<% 
def api_generator_MakeJSMethodDef(module_method_name)
    method_name = 'js_' + $cur_module.name + "_" + module_method_name

    "    js_define_method(\"#{$cur_module.parent}:#{$cur_module.name}:#{module_method_name}\", #{method_name});"
end

%>
extern "C" void Init_JSAPI_<%= $cur_module.name %>(void)
{
<% $cur_module.methods.each do |module_method|%>
<%= api_generator_MakeJSMethodDef(module_method.name)
 %><% end %>

<% if $cur_module.is_template_default_instance %>
<%= api_generator_MakeJSMethodDef("getDefaultID") %>
<%= api_generator_MakeJSMethodDef("setDefaultID") %>
<% end %>
}

