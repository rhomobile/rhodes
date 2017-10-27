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
  dynamic_constants = ''
  static_constants = ''
  dynamic_methods = ''
  static_methods = ''
  has_getProperty = false
  has_getProperties = false
  has_getAllProperties = false
  has_setProperty = false
  has_setProperties = false

  $cur_module.constants.each do |module_constant|
    if module_constant.name && module_constant.name.length() > 0 
      if module_constant.type == RhogenCore::TYPE_STRING
        constant = "\n        public const string #{module_constant.name} = \"#{module_constant.value}\";"
      else
        constant = "\n        public const #{api_generator_cs_makeNativeTypeArg(module_constant.type)} #{module_constant.name} = #{module_constant.value};"
      end
      if $cur_module.properties_access == ModuleMethod::ACCESS_INSTANCE
        dynamic_constants += constant
      else # if $cur_module.properties_access == ModuleMethod::ACCESS_STATIC
        static_constants += constant
      end
    end
  end
  dynamic_constants += "\n" if dynamic_constants.length() > 0
  static_constants += "\n" if static_constants.length() > 0

  $cur_module.methods.each do |module_method|
    next if !module_method.generateNativeAPI

    params = module_method.cached_data["cs_params"]
    method_def = "\n        abstract public void #{module_method.native_name}(#{params});"

    if module_method.access == ModuleMethod::ACCESS_STATIC
      static_methods += method_def
    else
      if module_method.native_name == 'getProperty'
        has_getProperty = true
      elsif module_method.native_name == 'getProperties'
        has_getProperties = true
      elsif module_method.native_name == 'getAllProperties'
        has_getAllProperties = true
      elsif module_method.native_name == 'setProperty'
        has_setProperty = true
      elsif module_method.native_name == 'setProperties'
        has_setProperties = true
      else
        dynamic_methods += method_def
      end
    end
  end
%>
namespace <%= $cur_module.name %>Impl
{
    abstract public class <%= $cur_module.name %>Base : I<%= $cur_module.name %>Impl
    {
        protected string _strID = "1";
        protected long _nativeImpl = 0;
        protected <%= $cur_module.name %>RuntimeComponent _runtime;
<%= dynamic_constants %>
        public <%= $cur_module.name %>Base()
        {
            _runtime = new <%= $cur_module.name %>RuntimeComponent(this);
        }

        public long getNativeImpl()
        {
            return _nativeImpl;
        }

        public virtual void setNativeImpl(string strID, long native)
        {
            _strID = strID;
            _nativeImpl = native;
        }

        public void dispatchInvoke(Action a)
        {
            if (Deployment.Current.Dispatcher != null)
                Deployment.Current.Dispatcher.BeginInvoke(a);
            else
                a();
        }
<% if has_getProperty
%>
        public void getProperty(string propertyName, IMethodResult oResult)
        {
            _runtime.getProperty(propertyName, oResult);
        }
<% end
if has_getProperties
%>
        public void getProperties(IReadOnlyList<string> arrayofNames, IMethodResult oResult)
        {
            _runtime.getProperties(arrayofNames, oResult);
        }
<% end
if has_getAllProperties
%>
        public void getAllProperties(IMethodResult oResult)
        {
            _runtime.getAllProperties(oResult);
        }
<% end
if has_setProperty
%>
        public void setProperty(string propertyName, string propertyValue, IMethodResult oResult)
        {
            _runtime.setProperty(propertyName, propertyValue, oResult);
        }
<% end
if has_setProperties
%>
        public void setProperties(IReadOnlyDictionary<string, string> propertyMap, IMethodResult oResult)
        {
            _runtime.setProperties(propertyMap, oResult);
        }
<% end
%><%= dynamic_methods%>
    }

    abstract public class <%= $cur_module.name %>SingletonBase : I<%= $cur_module.name %>SingletonImpl
    {
        protected <%= $cur_module.name %>SingletonComponent _runtime;
<%= static_constants %>
        public <%= $cur_module.name %>SingletonBase()
        {
            _runtime = new <%= $cur_module.name %>SingletonComponent(this);
        }
<%= static_methods%>
    }

    public class <%= $cur_module.name %>FactoryBase : I<%= $cur_module.name %>FactoryImpl
    {
        public virtual I<%= $cur_module.name %>Impl getImpl() {
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
