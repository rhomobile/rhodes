#pragma once

#include "common/RhoStd.h"
#include "api_generator/MethodResult.h"
#include "api_generator/BaseClasses.h"

<% $cur_module.parents.each do |parent| %>
namespace <%= parent.downcase() %> {<%
end %>
///////////////////////////////////////////////////////////
<% $cur_module.entities.each do |ett| 
%>struct <%= ett.native_name %>;
<% end %>

<% $cur_module.entities.each do |ett| %>
struct <%= ett.native_name %>
{
    <%= ett.native_name %>()<% if ett.fields.size > 0 %> : <% end %><%= ett.fields.map{ |f| f.name + '(' + f.default_value + ')' }.join(', ') %> {} <% 
    if ett.fields.size > 0%>
    <%= ett.native_name %>( <%= ett.fields.map{ |f| f.cpptype + ' _' + f.name }.join(', ') %> ) : <%= ett.fields.map{ |f| f.name + '(_' + f.name + ')' }.join(', ') %> {} <% end %>

    virtual ~<%= ett.native_name %>(){}

<% ett.fields.each do |fld|
%>    <%= fld.cpptype %> <%= fld.name %>;
<% end %>};
<% end %>
struct I<%= $cur_module.name %>
{
//constants
<% if $cur_module.properties_access == ModuleMethod::ACCESS_INSTANCE
   $cur_module.constants.each do |module_constant|
    if  module_constant.name && module_constant.name.length() > 0 
    if module_constant.type == RhogenCore::TYPE_STRING %>
    static const char <%= module_constant.name %>[];// "<%= module_constant.value %>" <%
else %>
    static const <%= CppGen::native_type(module_constant) %> <%= module_constant.name %> = <%= module_constant.value %>; <%
end; end; end; end %>

//methods
    virtual ~I<%= $cur_module.name %>(){}

<% $cur_module.methods.each do |module_method|
    next if module_method.access == ModuleMethod::ACCESS_STATIC
    next if !module_method.generateNativeAPI

    params = ''
    call_params = ''
    module_method.params.each do |param|
        params += " #{CppGen::native_type_arg(param)} #{param.name}, "

        call_params += " ," if call_params.length() > 0
        call_params += "#{param.name}"
    end

    params += 'rho::apiGenerator::CMethodResult& oResult'
    module_method.cached_data["cpp_params"] = params
    module_method.cached_data["cpp_call_params"] = call_params
%>    virtual void <%= module_method.native_name%>(<%= params%>) = 0;
<% end %>
};

struct I<%= $cur_module.name %>Singleton
{
//constants
<% if $cur_module.properties_access == ModuleMethod::ACCESS_STATIC
   $cur_module.constants.each do |module_constant|
    if  module_constant.name && module_constant.name.length() > 0 
    if module_constant.type == RhogenCore::TYPE_STRING %>
    static const char <%= module_constant.name %>[];// "<%= module_constant.value %>" <%
else %>
    static const <%= CppGen::native_type(module_constant) %> <%= module_constant.name %> = <%= module_constant.value %>; <%
end; end; end; end %>

    virtual ~I<%= $cur_module.name %>Singleton(){}

//methods
<% $cur_module.methods.each do |module_method|
    next if module_method.access != ModuleMethod::ACCESS_STATIC

    params = ''
    module_method.params.each do |param|
        params += " #{CppGen::native_type_arg(param)} #{param.name}, "
    end

    params += 'rho::apiGenerator::CMethodResult& oResult'

%>    virtual void <%= module_method.native_name%>(<%= params%>) = 0;
<% end %>
<% if $cur_module.is_template_default_instance %>
    virtual rho::String getDefaultID() = 0;
    virtual rho::String getInitialDefaultID() = 0;
    virtual void setDefaultID(const rho::String& strID) = 0;
<% end %>
    virtual void addCommandToQueue(rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor) = 0;
    virtual void callCommandInThread(rho::common::IRhoRunnable* pFunctor) = 0;
};

struct I<%= $cur_module.name %>Factory
{
    virtual ~I<%= $cur_module.name %>Factory(){}

    virtual I<%= $cur_module.name %>Singleton* getModuleSingleton() = 0;

<% if $cur_module.is_template_default_instance %>
    virtual I<%= $cur_module.name %>* getModuleByID(const rho::String& strID) = 0;
<% end %>
};

<% $cur_module.parents.each do |parent| %>
}<%
end %>
