#include "<%= $cur_module.name %>.h"

#include "MethodResultJni.h"
<%if $cur_module.is_template_default_instance %>
#include "api_generator/MethodResult.h"
#include "api_generator/MethodResultConvertor.h"
#include "api_generator/JSONResultConvertor.h"
<% end %>
#include "rhodes/JNIRhoJS.h"

#include "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "<%= $cur_module.name %>JS"

typedef <%= CppGen::make_namespace($cur_module.parents)%>C<%= $cur_module.name %>Proxy<ArgumentsAdapter<rho::json::CJSONArray> > ObjectProxy;

using namespace rho::apiGenerator;
<%
if $cur_module.is_template_default_instance %>
<%= api_generator_MakeJSMethodDecl($cur_module.name, "getDefaultID", true) %>
{
    RAWTRACE(__FUNCTION__);
    
    rho::apiGenerator::CMethodResult result(false);
    result.set(ObjectProxy::getDefaultID());
    return CMethodResultConvertor().toJSON(result);
}

<%= api_generator_MakeJSMethodDecl($cur_module.name, "getDefault", true) %>
{
    RAWTRACE(__FUNCTION__);
    
    rho::apiGenerator::CMethodResult result(false);
    result.setJSObjectClassPath("<%= api_generator_getJSModuleName(api_generator_getRubyModuleFullName($cur_module))%>");
    result.set(ObjectProxy::getDefaultID());
    return CMethodResultConvertor().toJSON(result);
}

<%= api_generator_MakeJSMethodDecl($cur_module.name, "setDefaultID", true) %>
{
    RAWTRACE(__FUNCTION__);

    rho::apiGenerator::CMethodResult result(false);
    ObjectProxy::setDefaultID(argv.getItem(0).getString());
    return CMethodResultConvertor().toJSON(result);
} <%
end %>

<%
$cur_module.methods.each do |method| %>
<%= api_generator_MakeJSMethodDecl($cur_module.name, method.native_name, method.access == ModuleMethod::ACCESS_STATIC) %>
{
    RAWTRACE2("%s(id=%s)", __FUNCTION__, strObjID.c_str());

    MethodResultJni result(false);
    if(!result)
    {
        result.setError("JNI error: failed to initialize MethodResult java object");
        RAWLOG_ERROR("JNI error: failed to initialize MethodResult java object ^^^");
        return CMethodResultConvertor().toJSON(result);
    }
<%
unless method.access == ModuleMethod::ACCESS_STATIC %>
    ObjectProxy <%= $cur_module.name.downcase %>(strObjID);<%
end
min_params = 0
max_params = method.params.size
method.params.each_index do |idx|
  param = method.params[idx]
  next if param.can_be_nil
  min_params = idx + 1
end
#if method.has_callback == ModuleMethod::CALLBACK_MANDATORY
#  min_params = method.params.size + 1
#elsif method.has_callback == ModuleMethod::CALLBACK_NONE
#  max_params = method.params.size
#end
%>

    int argc = argv.getSize();
    if((argc < <%= min_params %>) || (argc > <%= max_params %>))
    {
        result.setArgError("Wrong number of arguments");
        RAWLOG_ERROR1("Wrong number of arguments: %d ^^^", argc);
        return CMethodResultConvertor().toJSON(result);
    }
    
    if(strCallbackID.length() != 0)
    {
        result.setCallBack(strCallbackID, strCallbackParam);
    }
<%
if method.has_callback == ModuleMethod::CALLBACK_MANDATORY %>
    if(!result.hasCallback())
    {
        if(!result.isError())
        {
            result.setArgError("No callback handler provided");
        }
        RAWLOG_ERROR1("Error setting callback: %s", result.getErrorMessage().c_str());
        return CMethodResultConvertor().toJSON(result);
    }
<% end

if api_generator_isApiObjectParam(method.result)
  if method.result.type == RhogenCore::TYPE_CLASS %>
    result.setObjectClassPath("<%= api_generator_getJSModuleName(api_generator_getRubyModuleFullName($cur_module))%>");
    RAWTRACE("Object class path is set");<%
  else %>
    result.setObjectClassPath("<%= api_generator_getJSModuleName(api_generator_ruby_makeApiObjectTypeName(method.result, $cur_module)) %>");
    RAWTRACE("Object class path is set");<%
  end
end
if method.access == ModuleMethod::ACCESS_STATIC %>
    ObjectProxy::<%= method.native_name %>(argumentsAdapter(argv), result); <%
else %>
    <%= $cur_module.name.downcase %>.<%= method.native_name %>(argumentsAdapter(argv), result); <%
end %>
    rho::String res = CMethodResultConvertor().toJSON(result);
    RAWTRACE(res.c_str());
    RAWTRACE2("%s(id=%s) end ^^^", __FUNCTION__, strObjID.c_str());
    return res;
}<%
end %>
