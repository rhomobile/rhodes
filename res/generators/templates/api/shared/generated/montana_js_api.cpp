#include "api_generator/js_helpers.h"

#include "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "<%= $cur_module.name %>"

<% $cur_module.methods.each do |module_method| %>
<%= api_generator_MakeJSMethodDecl($cur_module.name, module_method.native_name, module_method.access == ModuleMethod::ACCESS_STATIC)%>;
<% if $cur_module.is_template_default_instance && (module_method.access == ModuleMethod::ACCESS_INSTANCE)
%><%= api_generator_MakeJSMethodDecl($cur_module.name, "def_"+module_method.native_name, true)+";" %>
<% end
end %>

<% if $cur_module.is_template_default_instance %>
<%= api_generator_MakeJSMethodDecl($cur_module.name, "getDefaultID", true) %>;
<%= api_generator_MakeJSMethodDecl($cur_module.name, "getDefault", true) %>;
<%= api_generator_MakeJSMethodDecl($cur_module.name, "setDefaultID", true) %>;
<% end %>

<% 
def api_generator_MakeJSMethodDef(module_method_name, module_method_native_name, is_static)
    method_name = 'js_'
    method_name += 's_' if is_static
    method_name += $cur_module.name + "_" + module_method_native_name

    api_method_name = module_method_name
    api_method_name += ':inst' if !is_static

    parent = ''
    $cur_module.parents.each do |p|
        parent = parent + p + ":"
    end
    "    rho::apiGenerator::js_define_method(\"#{parent}#{$cur_module.name}:#{api_method_name}\", #{method_name});"
end

%>
extern "C" void Init_JSAPI_<%= $cur_module.name %>(void)
{
<% $cur_module.methods.each do |module_method|%>
<%= api_generator_MakeJSMethodDef(module_method.name, module_method.native_name, module_method.access == ModuleMethod::ACCESS_STATIC) %>
<% if $cur_module.is_template_default_instance && (module_method.access == ModuleMethod::ACCESS_INSTANCE)
%><%= "    //  should define static method ! "+api_generator_MakeJSMethodDef("def_"+module_method.name, "def_"+module_method.native_name, true) %>;
<% end
end %>
<% if $cur_module.is_template_default_instance %>
<%= api_generator_MakeJSMethodDef("getDefaultID", "getDefaultID", true) %>
<%= api_generator_MakeJSMethodDef("getDefault", "getDefault", true) %>
<%= api_generator_MakeJSMethodDef("setDefaultID", "setDefaultID", true) %>
<% end %>
}

