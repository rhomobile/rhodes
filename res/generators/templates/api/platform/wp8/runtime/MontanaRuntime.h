#pragma once

#include "../../../shared/generated/cpp/I<%= $cur_module.name %>.h"

namespace <%= $cur_module.name %>Runtime
{
    public interface class IMethodResult
    {
    public:
        [Windows::Foundation::Metadata::DefaultOverloadAttribute]
        void set(Platform::String^ res);
        void set(Windows::Foundation::Collections::IVectorView<Platform::String^>^ res);
        void set(Windows::Foundation::Collections::IMapView<Platform::String^, Platform::String^>^ res);
    };

    public interface class I<%= $cur_module.name %>Impl
    {
    public:
<% $cur_module.methods.each do |module_method|
    next if module_method.access == ModuleMethod::ACCESS_STATIC
    next if !module_method.generateNativeAPI

    params = ''
    call_params = ''
    module_method.params.each do |param|
        params += "#{api_generator_cli_makeNativeTypeArg(param.type)} #{param.name}, "

        call_params += ", " if call_params.length() > 0
        call_params += "#{param.name}"
    end

    params += 'IMethodResult^ oResult'
    module_method.cached_data["cli_params"] = params
    module_method.cached_data["cli_call_params"] = call_params
%>        void <%= module_method.native_name%>(<%= params%>);
<% end %>    };

    public interface class I<%= $cur_module.name %>Singleton
    {
    public:
<% $cur_module.methods.each do |module_method|
    next if module_method.access != ModuleMethod::ACCESS_STATIC

    params = ''
    call_params = ''
    module_method.params.each do |param|
        params += "#{api_generator_cli_makeNativeTypeArg(param.type)} #{param.name}, "

        call_params += ", " if call_params.length() > 0
        call_params += "#{param.name}"
    end

    params += 'IMethodResult^ oResult'
    module_method.cached_data["cli_params"] = params
    module_method.cached_data["cli_call_params"] = call_params
%>        void <%= module_method.native_name%>(<%= params%>);
<% end %>    };

    public ref class <%= $cur_module.name %>RuntimeComponent sealed: public I<%= $cur_module.name %>Impl
    {
    public:
        <%= $cur_module.name %>RuntimeComponent(I<%= $cur_module.name %>Impl^ impl);

<% $cur_module.methods.each do |module_method|
    next if module_method.access == ModuleMethod::ACCESS_STATIC
    next if !module_method.generateNativeAPI
%>        virtual void <%= module_method.native_name%>(<%= module_method.cached_data["cli_params"]%>);
<% end %>
    private:
        I<%= $cur_module.name %>Impl^ _impl;
    };

    public ref class <%= $cur_module.name %>SingletonComponent sealed: public I<%= $cur_module.name %>Singleton
    {
    public:
        <%= $cur_module.name %>SingletonComponent(I<%= $cur_module.name %>Singleton^ impl);

<% $cur_module.methods.each do |module_method|
    next if module_method.access != ModuleMethod::ACCESS_STATIC
%>        virtual void <%= module_method.native_name%>(<%= module_method.cached_data["cli_params"]%>);
<% end %>
    private:
        I<%= $cur_module.name %>Singleton^ _impl;
    };

}