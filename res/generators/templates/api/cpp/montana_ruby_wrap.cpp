<% moduleNamespace = CppGen::make_namespace($cur_module.parents) %>
#include "<%= $cur_module.name %>Base.h"

#include "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "<%= $cur_module.name %>"

#include "api_generator/RubyResultConvertor.h"

#include "ruby/ext/rho/rhoruby.h"
#include "common/StringConverter.h"
#include "common/AutoPointer.h"

using namespace rho;
using namespace rho::common;
using namespace rho::apiGenerator;

extern "C"
{
void rho_os_impl_performOnUiThread(rho::common::IRhoRunnable* pTask);
VALUE getRuby_<%= $cur_module.name %>_Module();

<% if $cur_module.is_template_default_instance %>
VALUE rb_<%= $cur_module.name %>_s_default(VALUE klass)
{
    rho::String strDefaultID = <%= moduleNamespace%>C<%= $cur_module.name %>FactoryBase::get<%= $cur_module.name %>SingletonS()->getDefaultID();

    return rho_ruby_create_object_with_id( klass, strDefaultID.c_str() );
}

VALUE rb_<%= $cur_module.name %>_s_setDefault(VALUE klass, VALUE valObj)
{
    const char* szID = rho_ruby_get_object_id( valObj );
    <%= moduleNamespace%>C<%= $cur_module.name %>FactoryBase::get<%= $cur_module.name %>SingletonS()->setDefaultID(szID);

    return rho_ruby_get_NIL();
}
<% end %>

static void string_iter(const char* szVal, int valueLen, void* par)
{
    rho::Vector<rho::String>& ar = *((rho::Vector<rho::String>*)(par));
    ar.addElement( rho::String(szVal,valueLen) );
}

static void getStringArrayFromValue(VALUE val, rho::Vector<rho::String>& res)
{
    rho_ruby_enum_strary_json(val, string_iter, &res);
}

static void hash_eachstr(const char* szName, const char* szVal, int valueLen, void* par)
{
    rho::Hashtable<rho::String, rho::String>& hash = *((rho::Hashtable<rho::String, rho::String>*)(par));
    hash.put( szName, rho::String(szVal,valueLen) );
}

static void getStringHashFromValue(VALUE val, rho::Hashtable<rho::String, rho::String>& res)
{
    rho_ruby_enum_strhash_json(val, hash_eachstr, &res);
}
    
static rho::String getStringObjectFromValue(VALUE val)
{
    return rho::String(getStringFromValue(val),getStringLenFromValue(val));
}

<% $cur_module.methods.each do |module_method|
   if module_method.generateNativeAPI
   if module_method.access == ModuleMethod::ACCESS_STATIC %>
<%= api_generator_MakeRubyMethodDecl($cur_module.name, module_method, true)%><%
else %>
static VALUE _api_generator_<%= $cur_module.name %>_<%= module_method.native_name %>(int argc, VALUE *argv, <%= moduleNamespace%>I<%= $cur_module.name %>* pObj)<%
end %>
{
    rho::apiGenerator::CMethodResult oRes;
<% if module_method.access != ModuleMethod::ACCESS_STATIC %>
    if(!pObj)
    {
        oRes.setArgError("Invalid object!");
        return oRes.toRuby();
    }
<% end %>
<% if module_method.result != nil
    if module_method.result.sub_param && module_method.result.sub_param.name %>
    oRes.setParamName( "<%= module_method.result.sub_param.name %>" );<%
    end

    result_type = nil
    if RhogenCore::BASE_TYPES.include?(module_method.result.type)
      result_type = module_method.result.item_type
    else
      result_type = module_method.result.type;
    end

if api_generator_isSelfModule( $cur_module, result_type) %>
    oRes.setRubyObjectClass( getRuby_<%= $cur_module.name %>_Module() );<%
elsif result_type && result_type.length()>0 && !RhogenCore::BASE_TYPES.include?(result_type) %>
    oRes.setRubyObjectClassPath( "<%= result_type %>" );<%
    if !module_method.linked_entity.nil?%>
    oRes.setResultAsEntity();
<% end 
end; end

if module_method.linked_property && module_method.special_behaviour == ModuleMethod::SPECIAL_BEHAVIOUR_GETTER %>
    oRes.setRequestedType(<%= CppGen::native_result_type(module_method.linked_property) %>);<%
elsif module_method.result && module_method.result.type %>
    oRes.setRequestedType(<%= CppGen::native_result_type(module_method.result) %>);<%
end %>
    rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor = 0;
    bool bUseCallback = false;
    int nCallbackArg = 0;<%

functor_params = ""; first_arg = 0; 
module_method.params.each do |param| %>
    nCallbackArg = <%= first_arg + 1 %>;<%
if !param.can_be_nil %>
    if ( argc == <%= first_arg %> )
    {
        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(<%= module_method.params.size() %>) );
        return oRes.toRuby();
    }<%
end

if param.type == RhogenCore::TYPE_STRING %>
    <%= CppGen::native_type(param) %> arg<%= first_arg %> = "<%= param.default_value ? param.default_value : "" %>";
    if ( argc > <%= first_arg %> )
    {
        if ( rho_ruby_is_string(argv[<%= first_arg %>]) )
        {
            arg<%= first_arg %> = getStringObjectFromValue(argv[<%= first_arg %>]);
        }
        else if (!rho_ruby_is_NIL(argv[<%= first_arg %>]))
        {
            oRes.setArgError("Type error: argument " <%= "\"#{first_arg}\"" %> " should be " <%= "\"#{param.type.downcase}\"" %> );
            return oRes.toRuby();
        }
    }
<% end

if param.type == RhogenCore::TYPE_INT %>
    <%= CppGen::native_type(param) %> arg<%= first_arg %> = <%= param.default_value ? param.default_value : 0 %>;
    if ( argc > <%= first_arg %> )
    {
        if ( rho_ruby_is_integer(argv[<%= first_arg %>]) )
            arg<%= first_arg %> = rho_ruby_get_int(argv[<%= first_arg %>]);
        else if (!rho_ruby_is_NIL(argv[<%= first_arg %>]))
        {
            oRes.setArgError("Type error: argument " <%= "\"#{first_arg}\"" %> " should be " <%= "\"#{param.type.downcase}\"" %> );
            return oRes.toRuby();
        }
    }
<% end

if param.type == RhogenCore::TYPE_BOOL %>
    <%= CppGen::native_type(param) %> arg<%= first_arg %> = <%= param.default_value ? param.default_value : false %>;
    if ( argc > <%= first_arg %> )
    {
        if ( rho_ruby_is_boolean(argv[<%= first_arg %>]) )
            arg<%= first_arg %> = rho_ruby_get_bool(argv[<%= first_arg %>]) ? true : false;
        else if (!rho_ruby_is_NIL(argv[<%= first_arg %>]))
        {
            oRes.setArgError("Type error: argument " <%= "\"#{first_arg}\"" %> " should be " <%= "\"#{param.type.downcase}\"" %> );
            return oRes.toRuby();
        }
    }
<% end

if param.type == RhogenCore::TYPE_DOUBLE %>
    <%= CppGen::native_type(param) %> arg<%= first_arg %> = <%= param.default_value ? param.default_value : 0 %>;
    if ( argc > <%= first_arg %> )
    {
        if ( rho_ruby_is_double(argv[<%= first_arg %>]) )
            arg<%= first_arg %> = rho_ruby_get_double(argv[<%= first_arg %>]);
        else if (!rho_ruby_is_NIL(argv[<%= first_arg %>]))
        {
            oRes.setArgError("Type error: argument " <%= "\"#{first_arg}\"" %> " should be " <%= "\"#{param.type.downcase}\"" %> );
            return oRes.toRuby();
        }
    }
<% end

convert_args = ["argv[#{first_arg}]","arg#{first_arg}"].join(', ')

if param.type == RhogenCore::TYPE_ARRAY %>
    <%= CppGen::native_type(param) %> arg<%= first_arg %>;
    if ( argc > <%= first_arg %> )
    {
        if ( rho_ruby_is_array(argv[<%= first_arg %>]) )
        {
            rho::String res;
            <%= CppGen::result_converter(param) %>(<%= convert_args %>, res);
        }
        else if (!rho_ruby_is_NIL(argv[<%= first_arg %>]))
        {
            oRes.setArgError("Type error: argument " <%= "\"#{first_arg}\"" %> " should be " <%= "\"#{param.type.downcase}\"" %> );
            return oRes.toRuby();
        }
    }
<% end

if param.type == RhogenCore::TYPE_HASH %>
    <%= CppGen::native_type(param) %> arg<%= first_arg %>;
    if ( argc > <%= first_arg %> )
    {
        if ( rho_ruby_is_hash(argv[<%= first_arg %>]) )
        {
            rho::String res;
            <%= CppGen::result_converter(param) %>(<%= convert_args %>, res);
        }
        else if (!rho_ruby_is_NIL(argv[<%= first_arg %>]))
        {
            oRes.setArgError("Type error: argument " <%= "\"#{first_arg}\"" %> " should be " <%= "\"#{param.type.downcase}\"" %> );
            return oRes.toRuby();
        }
    }
<% end

if !RhogenCore::BASE_TYPES.include?(param.type) %>
    <%= CppGen::native_type(param) %> arg<%= first_arg %>;
    if ( argc > <%= first_arg %> )
    {
        <%  type_name = param.type;
            if api_generator_isSelfModule( $cur_module, param.type); type_name = api_generator_getRubyModuleFullName($cur_module); %>
            if ( rho_ruby_is_object_of_class(argv[<%= first_arg %>], getRuby_<%= $cur_module.name %>_Module()) )<% 
        else %>
            if ( rho_ruby_is_object_of_class(argv[<%= first_arg %>], rho_ruby_get_class_byname("<%=param.type%>") ) )<% 
        end %>
            arg<%= first_arg %> = rho_ruby_get_object_id( argv[<%= first_arg %>] );
        else if (!rho_ruby_is_NIL(argv[<%= first_arg %>]))
        {
            oRes.setArgError("Type error: argument " <%= "\"#{first_arg}\"" %> " should be " <%= "\"#{type_name}\"" %> );
            return oRes.toRuby();
        }
    }<%
end
functor_params += "arg#{first_arg}, "
first_arg = first_arg+1
end %>
    if ( argc > nCallbackArg )
    {
<% if module_method.has_callback == ModuleMethod::CALLBACK_NONE %>
        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(<%= module_method.params.size() %>) );
        return oRes.toRuby();<%
else %>
        if ( rho_ruby_is_proc(argv[nCallbackArg]) || rho_ruby_is_method(argv[nCallbackArg]) )
        {
            oRes.setRubyCallbackProc( argv[nCallbackArg] );
        }else if ( rho_ruby_is_string(argv[nCallbackArg]) )
        {
            oRes.setRubyCallback( getStringObjectFromValue(argv[nCallbackArg]) );
        }else
        {
            oRes.setArgError("Type error: callback should be String");
            return oRes.toRuby();
        }

        oRes.setCallInUIThread(<%= (module_method.run_in_thread == ModuleMethod::RUN_IN_THREAD_UI) ? "true" : "false" %>);
        if ( argc > nCallbackArg + 1 )
        {
            if ( !rho_ruby_is_NIL(argv[nCallbackArg + 1]) )
            {
                if ( !rho_ruby_is_string(argv[nCallbackArg + 1]) )
                {
                    oRes.setArgError("Type error: callback parameter should be String");
                    return oRes.toRuby();
                }

                oRes.setCallbackParam( getStringObjectFromValue(argv[nCallbackArg + 1]) );
            }
        }
        
        bUseCallback = true;<%
end %>
    }<%
if module_method.has_callback == ModuleMethod::CALLBACK_MANDATORY%>
    else
    {
        oRes.setArgError("Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(<%= module_method.params.size()+1 %>) + ".Mandatory Callback parameter is mised." );
        return oRes.toRuby();
    }<%
end

numParams = module_method.params.size()+1
if module_method.access != ModuleMethod::ACCESS_STATIC
    nativeInterfaceFunc = "#{moduleNamespace}I#{$cur_module.name}::#{module_method.native_name}"
    functorWar = "pFunctor = rho_makeInstanceClassFunctor#{numParams}( pObj, &#{nativeInterfaceFunc}, #{functor_params} oRes );"
else 
    nativeSingleton = "#{moduleNamespace}C#{$cur_module.name}FactoryBase::get#{$cur_module.name}SingletonS()"
    nativeSingletonMethod = "#{moduleNamespace}I#{$cur_module.name}Singleton::#{module_method.native_name}"
    functorWar = "pFunctor = rho_makeInstanceClassFunctor#{numParams}( #{nativeSingleton}, &#{nativeSingletonMethod}, #{functor_params} oRes );"
end

if module_method.run_in_thread == ModuleMethod::RUN_IN_THREAD_UI %>
    <%= functorWar %>
    rho_os_impl_performOnUiThread( pFunctor );<%
elsif (module_method.run_in_thread == ModuleMethod::RUN_IN_THREAD_MODULE) || (module_method.run_in_thread == ModuleMethod::RUN_IN_THREAD_SEPARATED) %>
    <%= functorWar %>
    <%= moduleNamespace%>C<%= $cur_module.name %>FactoryBase::get<%= $cur_module.name %>SingletonS()->addCommandToQueue( pFunctor );<%
else if module_method.run_in_thread != ModuleMethod::RUN_IN_THREAD_NONE %>
    if ( bUseCallback )
    {
        <%= functorWar %>
        <%= moduleNamespace%>C<%= $cur_module.name %>FactoryBase::get<%= $cur_module.name %>SingletonS()->addCommandToQueue( pFunctor );
    }
    else <%
end %>
    {
<% if module_method.access != ModuleMethod::ACCESS_STATIC %>
        pObj-><%= module_method.native_name %>( <%= functor_params %> oRes );<%
else %>
        <%= moduleNamespace%>C<%= $cur_module.name %>FactoryBase::get<%= $cur_module.name %>SingletonS()-><%= module_method.native_name %>( <%= functor_params %> oRes );<%
end %>
    }<% end %>
    
    return oRes.toRuby();
}
<% else 
if !module_method.generateNativeAPI && module_method.access != ModuleMethod::ACCESS_STATIC %>
VALUE rb_impl_<%= $cur_module.name %>_<%= module_method.native_name %>(int argc, VALUE *argv, <%= moduleNamespace%>I<%= $cur_module.name %>* pObj);<%
end
end %>

<% if module_method.access != ModuleMethod::ACCESS_STATIC %>
<%= api_generator_MakeRubyMethodDecl($cur_module.name, module_method, module_method.access == ModuleMethod::ACCESS_STATIC)%>
{
    const char* szID = rho_ruby_get_object_id( obj );
    if (!szID)
        rho_ruby_raise_runtime("Object was deleted.");

    VALUE res = 0;
    <%= moduleNamespace%>I<%= $cur_module.name %>* pObj =  <%= moduleNamespace%>C<%= $cur_module.name %>FactoryBase::getInstance()->getModuleByID(szID);
<% if module_method.generateNativeAPI%>
    res = _api_generator_<%= $cur_module.name %>_<%= module_method.native_name %>(argc, argv, pObj);<%
else%>
    res = rb_impl_<%= $cur_module.name %>_<%= module_method.native_name %>(argc, argv, pObj);<%
end%>
<% if module_method.is_destructor %>
    <%= moduleNamespace%>C<%= $cur_module.name %>FactoryBase::getInstance()->deleteModuleByID(szID);        
    rho_ruby_clear_object_id( obj );<%
end %>
    return res;
}
<% end %>

<% if $cur_module.is_template_default_instance && module_method.access == ModuleMethod::ACCESS_INSTANCE && !module_method.is_constructor %>
<%= api_generator_MakeRubyMethodDecl($cur_module.name + "_def", module_method, true)%>
{
    rho::String strDefaultID = <%= moduleNamespace%>C<%= $cur_module.name %>FactoryBase::get<%= $cur_module.name %>SingletonS()->getDefaultID();
    <%= moduleNamespace%>I<%= $cur_module.name %>* pObj = <%= moduleNamespace%>C<%= $cur_module.name %>FactoryBase::getInstance()->getModuleByID(strDefaultID);
<% if module_method.generateNativeAPI%>
    return _api_generator_<%= $cur_module.name %>_<%= module_method.native_name %>(argc, argv, pObj);<%
else%>
    return rb_impl_<%= $cur_module.name %>_<%= module_method.native_name %>(argc, argv, pObj);<%
end%>
}
<% end %>
<% end %>

}