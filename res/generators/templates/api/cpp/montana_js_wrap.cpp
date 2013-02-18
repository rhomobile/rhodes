#include "<%= $cur_module.name %>Base.h"
#include "api_generator\js_helpers.h"

#include "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "<%= $cur_module.name %>"

#include "common/StringConverter.h"

extern "C" void rho_wm_impl_performOnUiThread(rho::common::IRhoRunnable* pTask);

using namespace rho;
using namespace rho::json;
using namespace rho::common;

<% $cur_module.methods.each do |module_method| %>
<%= api_generator_MakeJSMethodDecl($cur_module.name, module_method.native_name, module_method.access == ModuleMethod::ACCESS_STATIC)%>
{
    rho::apiGenerator::CMethodResult oRes;

    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int argc = argv.getSize();
    int nCallbackArg = 0;
<% if module_method.access != ModuleMethod::ACCESS_STATIC %>
    <%= api_generator_cpp_MakeNamespace($cur_module.parents)%>I<%= $cur_module.name %>* pObj = <%= api_generator_cpp_MakeNamespace($cur_module.parents)%>C<%= $cur_module.name %>FactoryBase::getInstance()->getModuleByID(strObjID);
<%end%>

<% functor_params = ""; first_arg = 0; 
   module_method.params.each do |param| %>
    nCallbackArg = <%= first_arg + 1 %>;

    <% if !param.can_be_nil %>
    if ( argc == <%= first_arg %> )
    {
        oRes.setArgError( "Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(<%= module_method.params.size() %>) );
        return oRes.toJSON();
    }
    <% end %>

<% if param.type == MethodParam::TYPE_STRING %>
    <%= api_generator_cpp_makeNativeType(param.type) %> arg<%= first_arg %>;
    if ( argc > <%= first_arg %> )
    {
        if ( argv[<%= first_arg %>].isString() )
        {
            arg<%= first_arg %> = argv[<%= first_arg %>].getString();
<% if first_arg == 0 %>
            oRes.setStringParam(argv[<%= first_arg %>].getString());
<% end %>
        }
        else if (!argv[<%= first_arg %>].isNull())
        {
            oRes.setArgError( "Type error: argument " <%= "\"#{first_arg}\"" %> " should be " <%= "\"#{param.type.downcase}\"" %> );
            return oRes.toJSON();
        }
    }
<% end %>

<% if param.type == MethodParam::TYPE_INT %>
    <%= api_generator_cpp_makeNativeType(param.type) %> arg<%= first_arg %>;
    if ( argc > <%= first_arg %> )
    {
        if ( argv[<%= first_arg %>].isString() )
            arg<%= first_arg %> = argv[<%= first_arg %>].getInt();
        else if (!argv[<%= first_arg %>].isNull())
        {
            oRes.setArgError("Type error: argument " <%= "\"#{first_arg}\"" %> " should be " <%= "\"#{param.type.downcase}\"" %> );
            return oRes.toJSON();
        }
    }
<% end %>

<% if param.type == MethodParam::TYPE_BOOL %>
    <%= api_generator_cpp_makeNativeType(param.type) %> arg<%= first_arg %>;
    if ( argc > <%= first_arg %> )
    {
        if ( argv[<%= first_arg %>].isString() )
            arg<%= first_arg %> = argv[<%= first_arg %>].getInt() ? true : false;
        else if (!argv[<%= first_arg %>].isNull())
        {
            oRes.setArgError("Type error: argument " <%= "\"#{first_arg}\"" %> " should be " <%= "\"#{param.type.downcase}\"" %> );
            return oRes.toJSON();
        }
    }
<% end %>

<% if param.type == MethodParam::TYPE_DOUBLE %>
    <%= api_generator_cpp_makeNativeType(param.type) %> arg<%= first_arg %>;
    if ( argc > <%= first_arg %> )
    {
        if ( argv[<%= first_arg %>].isString() )
            arg<%= first_arg %> = argv[<%= first_arg %>].getDouble();
        else if (!argv[<%= first_arg %>].isNull())
        {
            oRes.setArgError("Type error: argument " <%= "\"#{first_arg}\"" %> " should be " <%= "\"#{param.type.downcase}\"" %> );
            return oRes.toJSON();
        }
    }
<% end %>

<% if param.type == MethodParam::TYPE_ARRAY %>
    <%= api_generator_cpp_makeNativeType(param.type) %> arg<%= first_arg %>;
    if ( argc > <%= first_arg %> )
    {
        if ( argv[<%= first_arg %>].isArray() )
        {
            CJSONArray arParam(argv[<%= first_arg %>]);
            for( int i = 0; i < arParam.getSize(); i++ )
            {
                arg<%= first_arg %>.addElement( arParam[i].getString() );
            }
        }
        else if (!argv[<%= first_arg %>].isNull())
        {
            oRes.setArgError("Type error: argument " <%= "\"#{first_arg}\"" %> " should be " <%= "\"#{param.type.downcase}\"" %> );
            return oRes.toJSON();
        }
    }
<% end %>

<% if param.type == MethodParam::TYPE_HASH %>
    <%= api_generator_cpp_makeNativeType(param.type) %> arg<%= first_arg %>;
    if ( argc > <%= first_arg %> )
    {
        if ( argv[<%= first_arg %>].isObject() )
        {
            CJSONStructIterator objIter(argv[<%= first_arg %>]);

            for( ; !objIter.isEnd(); objIter.next() )
            {
                arg<%= first_arg %>[objIter.getCurKey()] = objIter.getCurString();
            }
        }
        else if (!argv[<%= first_arg %>].isNull())
        {
            oRes.setArgError("Type error: argument " <%= "\"#{first_arg}\"" %> " should be " <%= "\"#{param.type.downcase}\"" %> );
            return oRes.toJSON();
        }
    }
<% end %>

<% if !MethodParam::BASE_TYPES.include?(param.type) %>
    <%= api_generator_cpp_makeNativeType(param.type) %> arg<%= first_arg %>;
<% end %>

<% functor_params += "arg#{first_arg}, " %>
<% first_arg = first_arg+1 %>
<% end %>

    if ( argc > nCallbackArg )
    {
<% if module_method.has_callback == ModuleMethod::CALLBACK_NONE %>
        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(<%= module_method.params.size() %>) );
        return oRes.toJSON();
<% end %>
        
        if ( !argv[nCallbackArg].isString() )
        {
            oRes.setArgError("Type error: callback should be String");
            return oRes.toJSON();
        }

        oRes.setCallInUIThread(<%= (module_method.run_in_thread == ModuleMethod::RUN_IN_THREAD_UI) ? "true" : "false" %>);
        oRes.setRubyCallback( argv[nCallbackArg].getString() );
        if ( argc > nCallbackArg + 1 )
        {
            if ( !argv[nCallbackArg + 1].isString() )
            {
                oRes.setArgError("Type error: callback parameter should be String");
                return oRes.toJSON();
            }

            oRes.setCallbackParam( argv[nCallbackArg + 1].getString() );
        }
        
    }

<% if module_method.access != ModuleMethod::ACCESS_STATIC %>
    pFunctor = rho_makeInstanceClassFunctor<%= module_method.params.size()+1%>( pObj, &<%= api_generator_cpp_MakeNamespace($cur_module.parents)%>I<%= $cur_module.name %>::<%= module_method.native_name %>, <%= functor_params %> oRes );
<% else %>
    pFunctor = rho_makeInstanceClassFunctor<%= module_method.params.size()+1%>( <%= api_generator_cpp_MakeNamespace($cur_module.parents)%>C<%= $cur_module.name %>FactoryBase::get<%= $cur_module.name %>SingletonS(), &<%= api_generator_cpp_MakeNamespace($cur_module.parents)%>I<%= $cur_module.name %>Singleton::<%= module_method.native_name %>, <%= functor_params %> oRes );
<% end %>

<% if (module_method.run_in_thread == ModuleMethod::RUN_IN_THREAD_UI) %>
    rho_wm_impl_performOnUiThread( pFunctor );
<% elsif (module_method.run_in_thread == ModuleMethod::RUN_IN_THREAD_MODULE) || (module_method.run_in_thread == ModuleMethod::RUN_IN_THREAD_SEPARATED) %>
    <%= api_generator_cpp_MakeNamespace($cur_module.parents)%>C<%= $cur_module.name %>FactoryBase::get<%= $cur_module.name %>SingletonS()->addCommandToQueue( pFunctor );
<% else if module_method.run_in_thread != ModuleMethod::RUN_IN_THREAD_NONE %>

    if ( bUseCallback )
        <%= api_generator_cpp_MakeNamespace($cur_module.parents)%>C<%= $cur_module.name %>FactoryBase::get<%= $cur_module.name %>SingletonS()->addCommandToQueue( pFunctor );
    else <%
end %>
    {
        delete pFunctor;

<% if module_method.access != ModuleMethod::ACCESS_STATIC %>
        pObj-><%= module_method.native_name %>( <%= functor_params %> oRes );
<% else %>
        <%= api_generator_cpp_MakeNamespace($cur_module.parents)%>C<%= $cur_module.name %>FactoryBase::get<%= $cur_module.name %>SingletonS()-><%= module_method.native_name %>( <%= functor_params %> oRes );
<% end %>

    }
<% end %>

    return oRes.toJSON();

}

<% end %>

<% if $cur_module.is_template_default_instance %>
<%= api_generator_MakeJSMethodDecl($cur_module.name, "getDefaultID", true)%>
{
    rho::apiGenerator::CMethodResult oRes;
    rho::String strDefaultID = <%= api_generator_cpp_MakeNamespace($cur_module.parents)%>C<%= $cur_module.name %>FactoryBase::get<%= $cur_module.name %>SingletonS()->getDefaultID();
    oRes.set(strDefaultID);

    return oRes.toJSON();
}

<%= api_generator_MakeJSMethodDecl($cur_module.name, "setDefaultID", true)%>
{
    rho::apiGenerator::CMethodResult oRes;
    <%= api_generator_cpp_MakeNamespace($cur_module.parents)%>C<%= $cur_module.name %>FactoryBase::get<%= $cur_module.name %>SingletonS()->setDefaultID(strObjID);

    return oRes.toJSON();
}

<% end %>
