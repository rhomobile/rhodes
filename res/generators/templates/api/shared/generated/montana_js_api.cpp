#include "api_generator/js_helpers.h"
#include "api_generator/Api.h"

#include "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "<%= $cur_module.name %>"

<% 
parent = ''
$cur_module.parents.each do |p|
    parent = parent + p + ":"
end

def api_generator_MakeJSMethodDef(module_method_name, module_method_native_name, is_static)
    method_name = 'js_'
    method_name += 's_' if is_static
    method_name += $cur_module.name + "_" + module_method_native_name

    method_type = is_static ? "Static" : "Instance"

    "    define#{method_type}Method(\"#{module_method_name}\", #{method_name});"
end

%>

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


namespace {
  using namespace rho::apiGenerator;
  class <%= $cur_module.name %>Dispatcher : public ApiHandler<Func_JS>
  {
  public:
    <%= $cur_module.name %>Dispatcher() : ApiHandler("<%= parent %><%= $cur_module.name %>") {}
    virtual ~<%= $cur_module.name %>Dispatcher() {}
    virtual void initialize();
  };
  
  void <%= $cur_module.name %>Dispatcher::initialize()
  {
    ApiHandler<Func_JS>::initialize();
    
    RAWTRACE("Initializing <%= parent %><%= $cur_module.name %> API...");
<% $cur_module.methods.each do |module_method|%>

<%= api_generator_MakeJSMethodDef(module_method.binding_name, module_method.native_name, module_method.access == ModuleMethod::ACCESS_STATIC) %>
<% if $cur_module.is_template_default_instance && (module_method.access == ModuleMethod::ACCESS_INSTANCE)
%><%= "    //  should define static method ! "+api_generator_MakeJSMethodDef("def_"+module_method.binding_name, "def_"+module_method.native_name, true) %>;
<% end
end %>
<% if $cur_module.is_template_default_instance %>
<%= api_generator_MakeJSMethodDef("getDefaultID", "getDefaultID", true) %>
<%= api_generator_MakeJSMethodDef("getDefault", "getDefault", true) %>
<%= api_generator_MakeJSMethodDef("setDefaultID", "setDefaultID", true) %>
<% end %>
    RAWTRACE("<%= parent %><%= $cur_module.name %> API - done");
  }
}

extern "C" void Init_JSAPI_<%= $cur_module.name %>(void)
{
  rho::apiGenerator::defineJSApiModule(new <%= $cur_module.name %>Dispatcher);
}
