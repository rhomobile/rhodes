//
//  <%= $cur_module.name %>Impl.cpp
#include "generated/cpp/<%= $cur_module.name %>Base.h"
#include "common/RhoStd.h"
#include "common/AutoPointer.h"
#include "common/RhodesApp.h"
#include "common/RhoConf.h"
#include "logging/RhoLog.h"

namespace rho {
    
    using namespace apiGenerator;
    using namespace common;
    
    class C<%= $cur_module.name %>SingletonImpl: public C<%= $cur_module.name %>SingletonBase
    {
    public:
        
        C<%= $cur_module.name %>SingletonImpl(): C<%= $cur_module.name %>SingletonBase(){}
        
        //methods
<%      last_entity = nil

        $cur_module.methods.each do |module_method|
        next if module_method.access != ModuleMethod::ACCESS_STATIC

        params = ''
        module_method.params.each do |param|
            params += " #{api_generator_cpp_makeNativeTypeArg(param.type)} #{param.name}, "
        end

        params += 'rho::apiGenerator::CMethodResult& oResult'

        ett = module_method.linked_entity;
        if last_entity != ett
            if !ett.nil? %>
        /* Enity <%= ett.name %> fileds 
        <% ett.fields.each do |fld|
        %>  <%= fld.const ? "const " : ""%><%= fld.cpptype %> <%= fld.name %>;
        <% end %>*/ 
<%

            end
            last_entity = ett
        end

    %><% if !module_method.desc.nil? 
%>        // <%= module_method.name%> <%= module_method.desc %> <% end %>
        virtual void <%= module_method.native_name%>(<%= params%>) {
            // RAWLOGC_INFO("<%= module_method.native_name%>","<%= $cur_module.name %>");
            <% 

            if !ett.nil? && ett.init_method == module_method && ett.fields.size > 0 %>
            <% 
            %>
            /* Enity <%= ett.name %> fileds initialization */
            rho::Hashtable<rho::String, rho::String> result;

            <% ett.fields.each do |fld|
            %> // <%= fld.type %> <%= "const" if fld.const %> <%= "binding" if fld.binding %>
            result["<%= fld.name %>"] =  <%= fld.const ? "" : ( fld.type == MethodParam::TYPE_STRING ? fld.default_value : "#{fld.default_value}" )%>;
            <% end %> 
            oResult.set(result);
            <% end %>
        } 
<% end %>
    };
    
    class C<%= $cur_module.name %>Impl : public C<%= $cur_module.name %>Base
    {
    public:
        virtual ~C<%= $cur_module.name %>Impl() {}

        //methods
<%      $cur_module.methods.each do |module_method|
        next if module_method.access == ModuleMethod::ACCESS_STATIC
        next if !module_method.generateNativeAPI

        params = ''
        call_params = ''
        module_method.params.each do |param|
            params += " #{api_generator_cpp_makeNativeTypeArg(param.type)} #{param.name}, "

            call_params += " ," if call_params.length() > 0
            call_params += "#{param.name}"
        end

        params += 'rho::apiGenerator::CMethodResult& oResult'
        module_method.cached_data["cpp_params"] = params
        module_method.cached_data["cpp_call_params"] = call_params
    %>
        virtual void <%= module_method.native_name%>(<%= params%>) {

        } 
<% end %>
    };
    
    ////////////////////////////////////////////////////////////////////////
    
    class C<%= $cur_module.name %>Factory: public C<%= $cur_module.name %>FactoryBase    {
    public:
        C<%= $cur_module.name %>Factory(){}
        
        I<%= $cur_module.name %>Singleton* createModuleSingleton()
        { 
            return new C<%= $cur_module.name %>SingletonImpl();
        }
        
        virtual I<%= $cur_module.name %>* createModuleByID(const rho::String& strID){ return new C<%= $cur_module.name %>Impl(); };
        
    };
    
}

extern "C" void Init_<%= $cur_module.name %>_extension()
{
    rho::C<%= $cur_module.name %>Factory::setInstance( new rho::C<%= $cur_module.name %>Factory() );
    rho::Init_<%= $cur_module.name %>_API();
    <% if $cur_module.entities.size > 0 
    module_name = $cur_module.parents.clone().push($cur_module.name).join('')
%>RHODESAPP().getExtManager().requireRubyFile("<%=module_name%>Entities"); 
<% end %>
}