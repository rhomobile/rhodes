using System;
using System.Collections.Generic;
using System.Net;
using Windows.UI.Core;
using System.Threading.Tasks;
using rhoruntime;
using rhodes;
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
        protected CoreDispatcher dispatcher = null;
        protected <%= $cur_module.name %>RuntimeComponent _runtime;
<%= dynamic_constants %>
        public <%= $cur_module.name %>Base(string id)
        {
            _strID = id;
            _runtime = new <%= $cur_module.name %>RuntimeComponent(this);
            try{dispatcher = MainPage.getDispatcher();
            }catch(Exception e){deb("Can't get access to dispatcher");}
        }

        public static void deb(String s, [System.Runtime.CompilerServices.CallerMemberName] string memberName = "")
        {
            if (memberName.Length != 0) {memberName = memberName + " : ";}
            System.Diagnostics.Debug.WriteLine(memberName + s);
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
            if (dispatcher != null) {
              var ignore = dispatcher.RunAsync(CoreDispatcherPriority.Normal, () =>
              {try{a();} catch (Exception ex) {System.Diagnostics.Debug.WriteLine("Invoke in UI Thread exception");} });
            }else{a();}
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

        protected SortedDictionary<string, <%= $cur_module.name %>Base> keeper = new SortedDictionary<string, <%= $cur_module.name %>Base>();

        public I<%= $cur_module.name %>Impl get<%= $cur_module.name %>ByID(string id)
        {
            if (keeper.ContainsKey(id))
            {
                return keeper[id];
            }
            else
            {
                <%= $cur_module.name %>Base impl = new <%= $cur_module.name %>(id);
                keeper.Add(id, impl);
                return impl;
            }
        }

        protected <%= $cur_module.name %>SingletonComponent _runtime;
<%= static_constants %>
        public <%= $cur_module.name %>SingletonBase()
        {
              try{dispatcher = MainPage.getDispatcher();
              }catch(Exception e){deb("Can't get access to dispatcher");}
              _runtime = new <%= $cur_module.name %>SingletonComponent(this);
        }

        public static void deb(String s, [System.Runtime.CompilerServices.CallerMemberName] string memberName = "")
        {
            if (memberName.Length != 0) {memberName = memberName + " : ";}
            System.Diagnostics.Debug.WriteLine(memberName + s);
        }

        public void dispatchInvoke(Action a)
        {
            if (dispatcher != null) {
              var ignore = dispatcher.RunAsync(CoreDispatcherPriority.Normal, () =>
              {try{a();} catch (Exception ex) {System.Diagnostics.Debug.WriteLine("Invoke in UI Thread exception");} });
            }else{a();}
        }
        protected CoreDispatcher dispatcher = null;

<%= static_methods%>
    }

    public class <%= $cur_module.name %>FactoryBase : I<%= $cur_module.name %>FactoryImpl
    {
        protected static <%= $cur_module.name %>Singleton instance = null;
        public virtual I<%= $cur_module.name %>Impl getImpl(string id) {
            getSingletonImpl();
            return instance.get<%= $cur_module.name %>ByID(id);
        }
        public I<%= $cur_module.name %>SingletonImpl getSingletonImpl() {
            if (instance == null){instance = new <%= $cur_module.name %>Singleton();}
            return instance;
          
        }
    }
}
<% $cur_module.parents.each do |parent| %>
}<%
end %>
