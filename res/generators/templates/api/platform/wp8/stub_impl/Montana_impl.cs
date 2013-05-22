using System;
using System.Collections.Generic;
using System.Net;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Documents;
using System.Windows.Ink;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Shapes;
using rhoruntime;
<% $cur_module.parents.each do |parent| %>
namespace <%= parent.downcase() %> {<%
end %>
<%
  dynamic_methods = ''
  static_methods = ''

  $cur_module.methods.each do |module_method|
    next if !module_method.generateNativeAPI

    params = ''
    module_method.params.each do |param|
        params += "#{api_generator_cs_makeNativeTypeArg(param.type)} #{param.name}, "
    end

    params += "IMethodResult oResult"
    module_method.cached_data["cs_params"] = params

    method_def = "\n        public void #{module_method.native_name}(#{params})\n        {\n            // implement this method in C# here\n        }\n"

    if module_method.access == ModuleMethod::ACCESS_STATIC
      static_methods += method_def
    else
      dynamic_methods += method_def
    end
  end
%>
namespace <%= $cur_module.name %>Impl
{
    public class <%= $cur_module.name %> : I<%= $cur_module.name %>Impl
    {
        public <%= $cur_module.name %>()
        {
            <%= $cur_module.name %>RuntimeComponent _runtime = new <%= $cur_module.name %>RuntimeComponent(this);
        }
<%= dynamic_methods%>    }

    public class <%= $cur_module.name %>Singleton : I<%= $cur_module.name %>SingletonImpl
    {
        public <%= $cur_module.name %>Singleton()
        {
            <%= $cur_module.name %>SingletonComponent _runtime = new <%= $cur_module.name %>SingletonComponent(this);
        }
<%= static_methods%>    }

    public class <%= $cur_module.name %>Factory : I<%= $cur_module.name %>FactoryImpl
    {
        public I<%= $cur_module.name %>Impl getImpl() {
            return new <%= $cur_module.name %>();
        }
        public I<%= $cur_module.name %>SingletonImpl getSingletonImpl() {
            return new <%= $cur_module.name %>Singleton();
        }
    }
}
<% $cur_module.parents.each do |parent| %>
}<%
end %>
