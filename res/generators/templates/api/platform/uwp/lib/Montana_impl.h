#include "common/RhodesApp.h"
#include "api_generator/uwp/MethodResultImpl.h"
#include "../../../../shared/generated/cpp/<%= $cur_module.name %>Base.h"
#include "<%= $cur_module.name %>Runtime.h"
#include "../../uwp/rhoruntime/common/RhoConvertWP8.h"

<% $cur_module.parents.each do |parent| %>
namespace <%= parent.downcase() %> {<%
end%>
<%
  dynamic_methods = ''

  $cur_module.methods.each do |module_method|
    next if !module_method.generateNativeAPI || (module_method.access == ModuleMethod::ACCESS_STATIC)

    params = ''
    module_method.params.each do |param|
        params += "#{CppGen::native_type_arg(param)} #{param.name}, "
        param_name = param.name
        param_conv = api_generator_cli_conversion(param.type, param.name)
        if param_conv.length() > 0
          param_name = '_' + param_name
        end
    end

    params += 'rho::apiGenerator::CMethodResult& oResult'

    method_def = "\n    virtual void #{module_method.native_name}(#{params});"
    if /^(getProperty|getProperties|getAllProperties|setProperty|setProperties)$/ !~ module_method.native_name
      dynamic_methods += method_def
    end
  end
%>
class C<%= $cur_module.name %>Impl: public C<%= $cur_module.name %>Base
{
private:
    rhoruntime::I<%= $cur_module.name %>Impl^ _runtime;
public:
    C<%= $cur_module.name %>Impl(const rho::String& strID, rhoruntime::I<%= $cur_module.name %>Impl^ runtime): C<%= $cur_module.name %>Base(), _runtime(runtime)
    {
        _runtime->setNativeImpl(rho::common::convertStringToWP8(strID), (int64)this);
    }
<%= dynamic_methods%>
};
<% $cur_module.parents.each do |parent| %>
}<%
end %>
