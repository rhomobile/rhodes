<% moduleNamespace = CppGen::make_namespace($cur_module.parents) %>

#include "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "<%= $cur_module.name %>"

#include "<%= $cur_module.name %>Base.h"
#include "api_generator/js_helpers.h"
#include "api_generator/JSONResultConvertor.h"

#include "common/StringConverter.h"

extern "C" void rho_os_impl_performOnUiThread(rho::common::IRhoRunnable* pTask);

#ifdef OS_ANDROID
#include <jni.h>
extern "C" void* rho_nativethread_start();
JNIEnv* jnienv();
#endif //OS_ANDROID

using namespace rho;
using namespace rho::json;
using namespace rho::common;
using namespace rho::apiGenerator;

<% $cur_module.methods.each do |module_method| 
   if module_method.generateNativeAPI %>
<%= api_generator_MakeJSMethodDecl($cur_module.name, module_method.native_name, module_method.access == ModuleMethod::ACCESS_STATIC)%>
{
    RAWTRACE4("<%=module_method.native_name%>(strObjID = %s, argc = %d, strCallbackID = %s, strJsVmID = %s)", strObjID.c_str(), argv.getSize(), strCallbackID.c_str(), strJsVmID.c_str());

#ifdef OS_ANDROID
    if ( jnienv() == 0 )
    {
        rho_nativethread_start();
    }
#endif

    rho::apiGenerator::CMethodResult oRes;

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
    oRes.setJSObjectClassPath( "<%= api_generator_getJSModuleName(api_generator_getRubyModuleFullName($cur_module))%>" );<%
elsif result_type && result_type.length()>0 && !RhogenCore::BASE_TYPES.include?(result_type) %>
    oRes.setJSObjectClassPath( "<%= api_generator_getJSModuleName(result_type) %>" );<%
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
    int argc = argv.getSize();
<% if module_method.access != ModuleMethod::ACCESS_STATIC %>
    <%= moduleNamespace%>I<%= $cur_module.name %>* pObj = <%= moduleNamespace%>C<%= $cur_module.name %>FactoryBase::getInstance()->getModuleByID(strObjID);
<%end
functor_params = ""; first_arg = 0;
module_method.params.each do |param| 
  if !param.can_be_nil %>
    if ( argc == <%= first_arg %> )
    {
        oRes.setArgError( "Wrong number of arguments: " + convertToStringA(argc) + " instead of " + convertToStringA(<%= module_method.params.size() %>) );
        return oRes.toJSON();
    }
    <% end 
     if param.type == RhogenCore::TYPE_STRING %>
    <%= CppGen::native_type(param) %> arg<%= first_arg %> = "<%= param.default_value ? param.default_value : "" %>";
    if ( argc > <%= first_arg %> )
    {
        if ( argv[<%= first_arg %>].isString() )
        {
            arg<%= first_arg %> = argv[<%= first_arg %>].getStringObject();
        }
        else if (!argv[<%= first_arg %>].isNull())
        {
            oRes.setArgError( "Type error: argument " <%= "\"#{first_arg}\"" %> " should be " <%= "\"#{param.type.downcase}\"" %> );
            return oRes.toJSON();
        }
    }
<%
  end 
  if param.type == RhogenCore::TYPE_INT %>
    <%= CppGen::native_type(param) %> arg<%= first_arg %> = <%= param.default_value ? param.default_value : 0 %>;
    if ( argc > <%= first_arg %> )
    {
        if ( argv[<%= first_arg %>].isInteger() )
            arg<%= first_arg %> = argv[<%= first_arg %>].getInt();
        else if (!argv[<%= first_arg %>].isNull())
        {
            oRes.setArgError("Type error: argument " <%= "\"#{first_arg}\"" %> " should be " <%= "\"#{param.type.downcase}\"" %> );
            return oRes.toJSON();
        }
    }
<%
  end 
  
  if param.type == RhogenCore::TYPE_BOOL %>
    <%= CppGen::native_type(param) %> arg<%= first_arg %> = <%= param.default_value ? param.default_value : false %>;
    if ( argc > <%= first_arg %> )
    {
        if ( argv[<%= first_arg %>].isBoolean() )
            arg<%= first_arg %> = argv[<%= first_arg %>].getBoolean();
        else if (!argv[<%= first_arg %>].isNull())
        {
            oRes.setArgError("Type error: argument " <%= "\"#{first_arg}\"" %> " should be " <%= "\"#{param.type.downcase}\"" %> );
            return oRes.toJSON();
        }
    }
<%
  end

  if param.type == RhogenCore::TYPE_DOUBLE %>
    <%= CppGen::native_type(param) %> arg<%= first_arg %> = <%= param.default_value ? param.default_value : 0 %>;
    if ( argc > <%= first_arg %> )
    {
        if ( argv[<%= first_arg %>].isFloat() )
            arg<%= first_arg %> = argv[<%= first_arg %>].getDouble();
        else if ( argv[<%= first_arg %>].isInteger() )
            arg<%= first_arg %> = (int64)argv[<%= first_arg %>].getUInt64();
        else if (!argv[<%= first_arg %>].isNull())
        {
            oRes.setArgError("Type error: argument " <%= "\"#{first_arg}\"" %> " should be " <%= "\"#{param.type.downcase}\"" %> );
            return oRes.toJSON();
        }
    }
<% 
  end 

  convert_args = ["argv[#{first_arg}]","arg#{first_arg}"].join(', ')

  if param.type == RhogenCore::TYPE_ARRAY %>
    <%= CppGen::native_type(param) %> arg<%= first_arg %>;
    if ( argc > <%= first_arg %> )
    {
        CJSONEntry value(argv[<%= first_arg %>]);
        if ( value.isArray() )
        {
            <% if param.api_style == RhogenCore::API_STYLE_LEGACY %>
            CJSONArray arParam(value);
            arg<%= first_arg %>.reserve(arParam.getSize());
            for( int i = 0; i < arParam.getSize(); i++ )
            {
                arg<%= first_arg %>.addElement( arParam[i].getStringObject() );
            }
            <% else %>
            rho::String res;
            <%= CppGen::result_converter(param) %>(value, arg<%= first_arg %>, res);
            <% end %>
        }
        else if (!value.isNull())
        {
            oRes.setArgError("Type error: argument " <%= "\"#{first_arg}\"" %> " should be " <%= "\"#{param.type.downcase}\"" %> );
            return oRes.toJSON();
        }
    }
<% 
  end 

  if param.type == RhogenCore::TYPE_HASH %>
    <%= CppGen::native_type(param) %> arg<%= first_arg %>;
    if ( argc > <%= first_arg %> )
    {
        CJSONEntry value(argv[<%= first_arg %>]);
        if ( value.isObject() )
        {
            <% if param.api_style == RhogenCore::API_STYLE_LEGACY %>
            CJSONStructIterator objIter(value);

            for( ; !objIter.isEnd(); objIter.next() )
            {
                arg<%= first_arg %>[objIter.getCurKey()] = objIter.getCurString();
            }
            <% else %>
            rho::String res;
            <%= CppGen::result_converter(param) %>(value, arg<%= first_arg %>, res);
            <% end %>
        }
        else if (!value.isNull())
        {
            oRes.setArgError("Type error: argument " <%= "\"#{first_arg}\"" %> " should be " <%= "\"#{param.type.downcase}\"" %> );
            return oRes.toJSON();
        }
    }
<% 
  end 

  if !RhogenCore::BASE_TYPES.include?(param.type) %>
    <%= CppGen::native_type(param) %> arg<%= first_arg %>;
<%
  end 

  functor_params += "arg#{first_arg}, " 
  first_arg = first_arg+1 
end 

if module_method.has_callback != ModuleMethod::CALLBACK_NONE %>
    oRes.setCallInUIThread(<%= (module_method.run_in_thread == ModuleMethod::RUN_IN_THREAD_UI) ? "true" : "false" %>);
    oRes.setJSCallback( strCallbackID, strJsVmID);
    oRes.setCallbackParam( strCallbackParam );
<% 
end 

if module_method.has_callback  == ModuleMethod::CALLBACK_MANDATORY %>
    if ( !oRes.hasCallback() )
    {
        oRes.setArgError("Callback argument is missed");
        return oRes.toJSON();
    }
<%
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
    if ( oRes.hasCallback() )
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
    
    return oRes.toJSON();

}
<% end 
   end %>

<% if $cur_module.is_template_default_instance %>
<%= api_generator_MakeJSMethodDecl($cur_module.name, "getDefaultID", true)%>
{
    rho::apiGenerator::CMethodResult oRes;
    rho::String strDefaultID = <%= moduleNamespace%>C<%= $cur_module.name %>FactoryBase::get<%= $cur_module.name %>SingletonS()->getDefaultID();
    oRes.set(strDefaultID);

    return oRes.toJSON();
}

<%= api_generator_MakeJSMethodDecl($cur_module.name, "getDefault", true)%>
{
    rho::apiGenerator::CMethodResult oRes;
    rho::String strDefaultID = <%= moduleNamespace%>C<%= $cur_module.name %>FactoryBase::get<%= $cur_module.name %>SingletonS()->getDefaultID();
    oRes.set(strDefaultID);
    oRes.setJSObjectClassPath("<%= api_generator_getJSModuleName(api_generator_getRubyModuleFullName($cur_module))%>");

    return oRes.toJSON();
}

<%= api_generator_MakeJSMethodDecl($cur_module.name, "setDefaultID", true)%>
{
    rho::apiGenerator::CMethodResult oRes;
    rho::json::CJSONEntry el = argv[0];

    if (el.isString()) {
        <%= moduleNamespace%>C<%= $cur_module.name %>FactoryBase::get<%= $cur_module.name %>SingletonS()->setDefaultID(el.getString());
    } else {
        oRes.setError("Method parameter should be defined as string!");
    }

    return oRes.toJSON();
}

<% end %>
