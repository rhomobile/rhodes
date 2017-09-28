#pragma once

#include "../../../../shared/generated/cpp/I<%= $cur_module.name %>.h"
#include "api_generator/uwp/IMethodResult.h"
<%
  dynamic_methods = ''
  static_methods = ''

  $cur_module.methods.each do |module_method|
    next if !module_method.generateNativeAPI

    params = ''
    call_params = ''
    module_method.params.each do |param|
        params += "#{api_generator_cli_makeNativeTypeArg(param.type)} #{param.name}, "
        call_params += "#{api_generator_cli_call_conversion(param.type, param.name)}, "
    end

    params += "IMethodResult^ oResult"
    call_params += '*(CMethodResult*)(oResult->getNative())'
    module_method.cached_data["cli_params"] = params
    module_method.cached_data["cli_call_params"] = call_params

    method_def = "        void #{module_method.native_name}(#{params});\n"
    if module_method.access == ModuleMethod::ACCESS_STATIC
      static_methods += method_def
    else
      dynamic_methods += method_def
    end
  end
  dynamic_methods = "\n\n" + dynamic_methods if dynamic_methods.length() > 0
  static_methods = "    public:\n" + static_methods if static_methods.length() > 0
%>
namespace rhoruntime
{
    public interface class I<%= $cur_module.name %>Impl
    {
    public:
        int64 getNativeImpl();
        void setNativeImpl(Platform::String^ strID, int64 native);
<%= dynamic_methods%>    };

    public interface class I<%= $cur_module.name %>SingletonImpl
    {
<%= static_methods%>    };
    public interface class I<%= $cur_module.name %>FactoryImpl
    {
    public:
        I<%= $cur_module.name %>Impl^ getImpl(Platform::String^ id);
        I<%= $cur_module.name %>SingletonImpl^ getSingletonImpl();
    };

    public ref class <%= $cur_module.name %>RuntimeComponent sealed
    {
    public:
        <%= $cur_module.name %>RuntimeComponent(I<%= $cur_module.name %>Impl^ impl);
<% $cur_module.methods.each do |module_method|
    next if module_method.access == ModuleMethod::ACCESS_STATIC
    next if !module_method.generateNativeAPI
    next if /^(getProperty|getProperties|getAllProperties|setProperty|setProperties)$/ !~ module_method.native_name
%>        void <%= module_method.native_name%>(<%= module_method.cached_data["cli_params"]%>);
<% end %>    private:
        I<%= $cur_module.name %>Impl^ _impl;
        int64 _cppImpl;
        int64 getCppImpl();
    };

    public ref class <%= $cur_module.name %>SingletonComponent sealed: public I<%= $cur_module.name %>SingletonImpl
    {
    public:
        <%= $cur_module.name %>SingletonComponent(I<%= $cur_module.name %>SingletonImpl^ impl);
<% $cur_module.methods.each do |module_method|
    next if module_method.access != ModuleMethod::ACCESS_STATIC
%>        virtual void <%= module_method.native_name%>(<%= module_method.cached_data["cli_params"]%>);
<% end %>    private:
        I<%= $cur_module.name %>SingletonImpl^ _impl;
    };

    public ref class <%= $cur_module.name %>FactoryComponent sealed
    {
    public:
        static void setImpl(I<%= $cur_module.name %>FactoryImpl^ impl);
    };
}
