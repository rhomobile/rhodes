#include "<%= $cur_module.name %>Base.h"

#include "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "<%= $cur_module.name %>"

#include "ruby/ext/rho/rhoruby.h"
#include "common/StringConverter.h"
#include "common/AutoPointer.h"

using namespace rho;
using namespace rho::common;

extern "C"
{

void rho_wm_impl_performOnUiThread(rho::common::IRhoRunnable* pTask);
VALUE getRuby_<%= $cur_module.name %>_Module();

<% if $cur_module.is_template_default_instance %>
VALUE rb_<%= $cur_module.name %>_s_default(VALUE klass)
{
    rho::StringW strDefaultID = <%= api_generator_cpp_MakeNamespace($cur_module.parents)%>C<%= $cur_module.name %>FactoryBase::get<%= $cur_module.name %>SingletonS()->getDefaultID();

    return rho_ruby_create_object_with_id( klass, convertToStringA(strDefaultID).c_str() );
}

VALUE rb_<%= $cur_module.name %>_s_setDefault(VALUE klass, VALUE valObj)
{
    const char* szID = rho_ruby_get_object_id( valObj );
    <%= api_generator_cpp_MakeNamespace($cur_module.parents)%>C<%= $cur_module.name %>FactoryBase::get<%= $cur_module.name %>SingletonS()->setDefaultID(convertToStringW(szID));

    return rho_ruby_get_NIL();
}
<% end %>

extern "C" static void
string_iter(const char* szVal, void* par)
{
    rho::Vector<rho::StringW>& ar = *((rho::Vector<rho::StringW>*)(par));
    ar.addElement( convertToStringW(szVal) );
}

static void getStringArrayFromValue(VALUE val, rho::Vector<rho::StringW>& res)
{
    rho_ruby_enum_strary(val, string_iter, &res);
}

extern "C" static void hash_eachstr(const char* szName, const char* szVal, void* par)
{
    rho::Hashtable<rho::StringW, rho::StringW>& hash = *((rho::Hashtable<rho::StringW, rho::StringW>*)(par));
    hash.put( convertToStringW(szName), convertToStringW(szVal) );
}

static void getStringHashFromValue(VALUE val, rho::Hashtable<rho::StringW, rho::StringW>& res)
{
    rho_ruby_enum_strhash(val, hash_eachstr, &res);
}

<% $cur_module.methods.each do |module_method| %>
<% if module_method.access == ModuleMethod::ACCESS_STATIC %>
<%= api_generator_MakeRubyMethodDecl($cur_module.name, module_method, true)%>
<% else %>
static VALUE _api_generator_<%= $cur_module.name %>_<%= module_method.native_name %>(int argc, VALUE *argv, <%= api_generator_cpp_MakeNamespace($cur_module.parents)%>I<%= $cur_module.name %>* pObj)
<% end %>
{
    rho::apiGenerator::CMethodResult oRes;

<% if module_method.is_factory_method %>
    oRes.setRubyObjectClass(getRuby_<%= $cur_module.name %>_Module());
<% end %>

    rho::common::IRhoRunnable* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;

<% functor_params = ""; first_arg = 0; %>
<% module_method.params.each do |param| %>

    nCallbackArg = <%= first_arg + 1 %>;

    <% if !param.can_be_nil %>
    if ( argc == <%= first_arg %> )
    {
        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(<%= module_method.params.size() %>) );
        return oRes.toRuby();
    }
    <% end %>

<% if param.type == MethodParam::TYPE_STRING %>
    <%= api_generator_cpp_makeNativeType(param.type) %> arg<%= first_arg %>;
    if ( argc > <%= first_arg %> )
    {
        if ( rho_ruby_is_string(argv[<%= first_arg %>]) )
        {
            arg<%= first_arg %> = convertToStringW(getStringFromValue(argv[<%= first_arg %>]));
<% if first_arg == 0 %>
            oRes.setStringParam(getStringFromValue(argv[<%= first_arg %>]));
<% end %>
        }
        else if (!rho_ruby_is_NIL(argv[<%= first_arg %>]))
        {
            oRes.setArgError(L"Type error: argument " L<%= "\"#{first_arg}\"" %> L" should be " L<%= "\"#{param.type.downcase}\"" %> );
            return oRes.toRuby();
        }
    }
<% end %>

<% if param.type == MethodParam::TYPE_INT %>
    <%= api_generator_cpp_makeNativeType(param.type) %> arg<%= first_arg %>;
    if ( argc > <%= first_arg %> )
    {
        if ( rho_ruby_is_integer(argv[<%= first_arg %>]) )
            arg<%= first_arg %> = rho_ruby_get_int(argv[<%= first_arg %>]);
        else if (!rho_ruby_is_NIL(argv[<%= first_arg %>]))
        {
            oRes.setArgError(L"Type error: argument " L<%= "\"#{first_arg}\"" %> L" should be " L<%= "\"#{param.type.downcase}\"" %> );
            return oRes.toRuby();
        }
    }
<% end %>

<% if param.type == MethodParam::TYPE_BOOL %>
    <%= api_generator_cpp_makeNativeType(param.type) %> arg<%= first_arg %>;
    if ( argc > <%= first_arg %> )
    {
        if ( rho_ruby_is_boolean(argv[<%= first_arg %>]) )
            arg<%= first_arg %> = rho_ruby_get_bool(argv[<%= first_arg %>]) ? true : false;
        else if (!rho_ruby_is_NIL(argv[<%= first_arg %>]))
        {
            oRes.setArgError(L"Type error: argument " L<%= "\"#{first_arg}\"" %> L" should be " L<%= "\"#{param.type.downcase}\"" %> );
            return oRes.toRuby();
        }
    }
<% end %>

<% if param.type == MethodParam::TYPE_DOUBLE %>
    <%= api_generator_cpp_makeNativeType(param.type) %> arg<%= first_arg %>;
    if ( argc > <%= first_arg %> )
    {
        if ( rho_ruby_is_double(argv[<%= first_arg %>]) )
            arg<%= first_arg %> = rho_ruby_get_double(argv[<%= first_arg %>]);
        else if (!rho_ruby_is_NIL(argv[<%= first_arg %>]))
        {
            oRes.setArgError(L"Type error: argument " L<%= "\"#{first_arg}\"" %> L" should be " L<%= "\"#{param.type.downcase}\"" %> );
            return oRes.toRuby();
        }
    }
<% end %>

<% if param.type == MethodParam::TYPE_ARRAY %>
    <%= api_generator_cpp_makeNativeType(param.type) %> arg<%= first_arg %>;
    if ( argc > <%= first_arg %> )
    {
        if ( rho_ruby_is_array(argv[<%= first_arg %>]) )
            getStringArrayFromValue(argv[<%= first_arg %>], arg<%= first_arg %>);
        else if (!rho_ruby_is_NIL(argv[<%= first_arg %>]))
        {
            oRes.setArgError(L"Type error: argument " L<%= "\"#{first_arg}\"" %> L" should be " L<%= "\"#{param.type.downcase}\"" %> );
            return oRes.toRuby();
        }
    }
<% end %>

<% if param.type == MethodParam::TYPE_HASH %>
    <%= api_generator_cpp_makeNativeType(param.type) %> arg<%= first_arg %>;
    if ( argc > <%= first_arg %> )
    {
        if ( rho_ruby_is_hash(argv[<%= first_arg %>]) )
            getStringHashFromValue(argv[<%= first_arg %>], arg<%= first_arg %>);
        else if (!rho_ruby_is_NIL(argv[<%= first_arg %>]))
        {
            oRes.setArgError(L"Type error: argument " L<%= "\"#{first_arg}\"" %> L" should be " L<%= "\"#{param.type.downcase}\"" %> );
            return oRes.toRuby();
        }
    }
<% end %>
        
<% functor_params += "arg#{first_arg}, " %>
<% first_arg = first_arg+1 %>
<% end %>

    if ( argc > nCallbackArg )
    {
<% if module_method.has_callback == ModuleMethod::CALLBACK_NONE %>
        oRes.setArgError(L"Wrong number of arguments: " + convertToStringW(argc) + L" instead of " + convertToStringW(<%= module_method.params.size() %>) );
        return oRes.toRuby();
<% else %>

        if ( rho_ruby_is_proc(argv[nCallbackArg]) || rho_ruby_is_method(argv[nCallbackArg]) )
        {
            oRes.setRubyCallbackProc( argv[nCallbackArg] );
        }else if ( rho_ruby_is_string(argv[nCallbackArg]) )
        {
            oRes.setRubyCallback( getStringFromValue(argv[nCallbackArg]) );
        }else
        {
            oRes.setArgError(L"Type error: callback should be String");
            return oRes.toRuby();
        }

        oRes.setCallInUIThread(<%= (module_method.run_in_thread == ModuleMethod::RUN_IN_THREAD_UI) ? "true" : "false" %>);
        if ( argc > nCallbackArg + 1 )
        {
            if ( !rho_ruby_is_string(argv[nCallbackArg + 1]) )
            {
                oRes.setArgError(L"Type error: callback parameter should be String");
                return oRes.toRuby();
            }

            oRes.setCallbackParam( getStringFromValue(argv[nCallbackArg + 1]) );
        }
        
        bUseCallback = true;
<% end %>
    }

<% if module_method.access != ModuleMethod::ACCESS_STATIC %>
    pFunctor = rho_makeInstanceClassFunctor<%= module_method.params.size()+1%>( pObj, &<%= api_generator_cpp_MakeNamespace($cur_module.parents)%>I<%= $cur_module.name %>::<%= module_method.native_name %>, <%= functor_params %> oRes );
<% else %>
    pFunctor = rho_makeInstanceClassFunctor<%= module_method.params.size()+1%>( <%= api_generator_cpp_MakeNamespace($cur_module.parents)%>C<%= $cur_module.name %>FactoryBase::get<%= $cur_module.name %>SingletonS(), &<%= api_generator_cpp_MakeNamespace($cur_module.parents)%>I<%= $cur_module.name %>Singleton::<%= module_method.native_name %>, <%= functor_params %> oRes );
<% end %>

<% if module_method.run_in_thread == ModuleMethod::RUN_IN_THREAD_UI %>
    rho_wm_impl_performOnUiThread( pFunctor );
<% elsif (module_method.run_in_thread == ModuleMethod::RUN_IN_THREAD_MODULE) || (module_method.run_in_thread == ModuleMethod::RUN_IN_THREAD_SEPARATED) %>
    <%= api_generator_cpp_MakeNamespace($cur_module.parents)%>C<%= $cur_module.name %>FactoryBase::get<%= $cur_module.name %>SingletonS()->addCommandToQueue( pFunctor );
<% else %>

    if ( bUseCallback )
        <%= api_generator_cpp_MakeNamespace($cur_module.parents)%>C<%= $cur_module.name %>FactoryBase::get<%= $cur_module.name %>SingletonS()->addCommandToQueue( pFunctor );
    else
    {
        delete pFunctor;

<% if module_method.access != ModuleMethod::ACCESS_STATIC %>
        pObj-><%= module_method.native_name %>( <%= functor_params %> oRes );
<% else %>
        <%= api_generator_cpp_MakeNamespace($cur_module.parents)%>C<%= $cur_module.name %>FactoryBase::get<%= $cur_module.name %>SingletonS()-><%= module_method.native_name %>( <%= functor_params %> oRes );
<% end %>

    }
<% end %>

    return oRes.toRuby();
}

<% if module_method.access != ModuleMethod::ACCESS_STATIC %>
<%= api_generator_MakeRubyMethodDecl($cur_module.name, module_method, module_method.access == ModuleMethod::ACCESS_STATIC)%>
{
    const char* szID = rho_ruby_get_object_id( obj );
    <%= api_generator_cpp_MakeNamespace($cur_module.parents)%>I<%= $cur_module.name %>* pObj =  <%= api_generator_cpp_MakeNamespace($cur_module.parents)%>C<%= $cur_module.name %>FactoryBase::getInstance()->getModuleByID(convertToStringW(szID));

    return _api_generator_<%= $cur_module.name %>_<%= module_method.native_name %>(argc, argv, pObj);
}
<% end %>

<% if $cur_module.is_template_default_instance && module_method.access == ModuleMethod::ACCESS_INSTANCE%>
<%= api_generator_MakeRubyMethodDecl($cur_module.name + "_def", module_method, true)%>
{
    rho::StringW strDefaultID = <%= api_generator_cpp_MakeNamespace($cur_module.parents)%>C<%= $cur_module.name %>FactoryBase::get<%= $cur_module.name %>SingletonS()->getDefaultID();
    <%= api_generator_cpp_MakeNamespace($cur_module.parents)%>I<%= $cur_module.name %>* pObj = <%= api_generator_cpp_MakeNamespace($cur_module.parents)%>C<%= $cur_module.name %>FactoryBase::getInstance()->getModuleByID(strDefaultID);

    return _api_generator_<%= $cur_module.name %>_<%= module_method.native_name %>(argc, argv, pObj);
}
<% end %>
<% end %>

}