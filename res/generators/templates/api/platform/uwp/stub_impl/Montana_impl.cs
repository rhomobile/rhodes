using System;
using System.Collections.Generic;
using System.Net;
using Windows.UI.Core;
using System.Threading.Tasks;
using rhoruntime;
<% $cur_module.parents.each do |parent| %>
namespace <%= parent.downcase() %> {<%
end %>
<%
  dynamic_methods = ''
  static_methods = ''

  $cur_module.methods.each do |module_method|
    next if !module_method.generateNativeAPI

    args = ''
    params = ''
    module_method.params.each do |param|
        params += "#{api_generator_cs_makeNativeTypeArg(param.type)} #{param.name}, "
        args += "#{param.name}, "
    end

    params += "IMethodResult oResult"
    args += "oResult"
    module_method.cached_data["cs_params"] = params

    method_def = "\n        public override void #{module_method.native_name}(#{params})\n        {\n            // implement this method in C# here\n        }\n"

    if module_method.access == ModuleMethod::ACCESS_STATIC
      static_methods += method_def
    else
      dynamic_methods += method_def if /^(getProperty|getProperties|getAllProperties|setProperty|setProperties)$/ !~ module_method.native_name
    end
  end
%>
namespace <%= $cur_module.name %>Impl
{
    public class <%= $cur_module.name %> : <%= $cur_module.name %>Base
    {
        public <%= $cur_module.name %>(string id) : base(id)
        {
            // initialize class instance in C# here
        }
<%= dynamic_methods%>    }

    public class <%= $cur_module.name %>Singleton : <%= $cur_module.name %>SingletonBase
    {
        public <%= $cur_module.name %>Singleton()
        {
            // initialize singleton instance in C# here
        }
<%= static_methods%>    }

    public class <%= $cur_module.name %>Factory : <%= $cur_module.name %>FactoryBase
    {
    }
}
<% $cur_module.parents.each do |parent| %>
}<%
end %>
