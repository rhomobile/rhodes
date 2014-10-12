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

    args = ''
    params = ''
    module_method.params.each do |param|
        params += "#{api_generator_cs_makeNativeTypeArg(param.type)} #{param.name}, "
        args += "#{param.name}, "
    end

    params += "IMethodResult oResult"
    args += "oResult"
    module_method.cached_data["cs_params"] = params

    method_body = (/^(getProperty|getProperties|getAllProperties|setProperty|setProperties)$/ !~ module_method.native_name) ? "// implement this method in C# here" : "_runtime.#{module_method.native_name}(#{args});"
    method_def = "\n        public void #{module_method.native_name}(#{params})\n        {\n            #{method_body}\n        }\n"

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
        private long _nativeImpl = 0;
        <%= $cur_module.name %>RuntimeComponent _runtime;

        public <%= $cur_module.name %>()
        {
            _runtime = new <%= $cur_module.name %>RuntimeComponent(this);
        }

        public long getNativeImpl()
        {
            return _nativeImpl;
        }

        public void setNativeImpl(long native)
        {
            _nativeImpl = native;
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
