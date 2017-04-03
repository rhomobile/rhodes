// <%= $cur_module.name %>Runtime.cpp
#include "<%= $cur_module.name %>Runtime.h"
#include "<%= $cur_module.name %>Factory.h"
#include "<%= $cur_module.name %>_impl.h"
#include "../../uwp/rhoruntime/common/RhoConvertWP8.h"

using namespace Platform;
using namespace rho::apiGenerator;
<% $cur_module.parents.each do |parent| %>
using namespace <%= parent.downcase() %>;<%
end%>

namespace rhoruntime
{

<%= $cur_module.name %>RuntimeComponent::<%= $cur_module.name %>RuntimeComponent(I<%= $cur_module.name %>Impl^ impl):
    _impl(impl), _cppImpl(0)
{
    // TODO: implement runtime component constructor
}

int64 <%= $cur_module.name %>RuntimeComponent::getCppImpl()
{
    if (_cppImpl == 0)
    {
        _cppImpl = _impl->getNativeImpl();
    }
    return _cppImpl;
}

<%= $cur_module.name %>SingletonComponent::<%= $cur_module.name %>SingletonComponent(I<%= $cur_module.name %>SingletonImpl^ impl):
    _impl(impl)
{
    // TODO: implement singleton component constructor
}<% $cur_module.methods.each do |module_method|
    next if ((!module_method.generateNativeAPI || (/^(getProperty|getProperties|getAllProperties|setProperty|setProperties)$/ !~ module_method.native_name)) && (module_method.access != ModuleMethod::ACCESS_STATIC))
    component_name = module_method.access == ModuleMethod::ACCESS_STATIC ? 'Singleton' : 'Runtime';
    native_prefix = module_method.access == ModuleMethod::ACCESS_STATIC ? '//' : '';
%>

void <%= $cur_module.name %><%= component_name%>Component::<%= module_method.native_name%>(<%= module_method.cached_data["cli_params"]%>)
{
    <%= native_prefix %>((C<%= $cur_module.name %>Impl*)getCppImpl())-><%= module_method.native_name%>(<%= module_method.cached_data["cli_call_params"]%>);
}<% end %>

void <%= $cur_module.name %>FactoryComponent::setImpl(I<%= $cur_module.name %>FactoryImpl^ impl)
{
    C<%= $cur_module.name %>Factory::setImpl(impl);
}

}