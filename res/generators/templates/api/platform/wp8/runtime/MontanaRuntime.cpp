// <%= $cur_module.name %>Runtime.cpp
#include "<%= $cur_module.name %>Runtime.h"

using namespace <%= $cur_module.name %>Runtime;
using namespace Platform;
using namespace rho::apiGenerator;

<%= $cur_module.name %>RuntimeComponent::<%= $cur_module.name %>RuntimeComponent(I<%= $cur_module.name %>Impl^ impl):
    _impl(impl)
{
    // TODO: it's temporary code for testing only => commented out
    //C<%= $cur_module.name %>FactoryBase::getInstance()->createModuleByID(rho::common::convertToString(L"test"))->registerRuntime(this);
}

<%= $cur_module.name %>SingletonComponent::<%= $cur_module.name %>SingletonComponent(I<%= $cur_module.name %>Singleton^ impl):
    _impl(impl)
{
    // TODO: implement
}<% $cur_module.methods.each do |module_method|
    component_name = module_method.access == ModuleMethod::ACCESS_STATIC ? 'Singleton' : 'Runtime';
    next if (!module_method.generateNativeAPI) && (module_method.access != ModuleMethod::ACCESS_STATIC)
%>

void <%= $cur_module.name %><%= component_name%>Component::<%= module_method.native_name%>(<%= module_method.cached_data["cli_params"]%>)
{
    _impl-><%= module_method.native_name%>(<%= module_method.cached_data["cli_call_params"]%>);
}<% end %>
