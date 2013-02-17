#include "<%= $cur_module.name %>.h"

#include "MethodResultJni.h"
#include "api_generator/MethodResult.h"

#include "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "<%= $cur_module.name %>JS"

#include "rhodes/JNIRhoJS.h"

typedef <%= api_generator_cpp_MakeNamespace($cur_module.parents)%>C<%= $cur_module.name %>Proxy<ArgumentsAdapter<rho::json::CJSONArray> > ObjectProxy;

using namespace rhoelements;
<%
if $cur_module.is_template_default_instance %>
<%= api_generator_MakeJSMethodDecl($cur_module.name, "getDefaultID", true) %>
{
    RAWTRACE(__FUNCTION__);
    
    rho::apiGenerator::CMethodResult result;
    result.set(ObjectProxy::getDefaultID());
    return result.toJSON();
}

<%= api_generator_MakeJSMethodDecl($cur_module.name, "setDefaultID", true) %>
{
    RAWTRACE(__FUNCTION__);

    rho::apiGenerator::CMethodResult result;
    ObjectProxy::setDefaultID(strObjID);
    return result.toJSON();
} <%
end %>

<%
$cur_module.methods.each do |method| %>
<%= api_generator_MakeJSMethodDecl($cur_module.name, method.native_name, method.access == ModuleMethod::ACCESS_STATIC) %>
{
    RAWTRACE2("%s(id=%s)", __FUNCTION__, strObjID.c_str());

    MethodResultJni result;
    if(!result)
    {
        result.setError("JNI error: failed to initialize MethodResult java object");
        RAWLOG_ERROR("JNI error: failed to initialize MethodResult java object ^^^");
        return result.toJson();
    }
<%
unless method.access == ModuleMethod::ACCESS_STATIC %>
    ObjectProxy <%= $cur_module.name.downcase %>(strObjID);<%
end
min_params = 0
max_params = method.params.size + 2
method.params.each_index do |idx|
  param = method.params[idx]
  next if param.can_be_nil
  min_params = idx + 1
end
if method.has_callback == ModuleMethod::CALLBACK_MANDATORY
  min_params = method.params.size + 1
elsif method.has_callback == ModuleMethod::CALLBACK_NONE
  max_params = method.params.size
end %>

    int argc = argv.getSize();
    if((argc < <%= min_params %>) || (argc > <%= max_params %>))
    {
        result.setArgError("Wrong number of arguments");
        RAWLOG_ERROR1("Wrong number of arguments: %d ^^^", argc);
        return result.toJson();
    }
    
    if(argc > <%= method.params.size %>)
    {
        if(argc > <%= method.params.size+1 %>)
            result.setCallBack(argv[<%= method.params.size %>], argv[<%= method.params.size+1 %>]);
        else
            result.setCallBack(argv[<%= method.params.size %>]);
    
        if(!result.hasCallBackUrl())
        {
            RAWLOG_ERROR("Error setting callback ^^^");
            return result.toJson();
        }
    }
<%
if method.access == ModuleMethod::ACCESS_STATIC %>
    ObjectProxy::<%= method.native_name %>(argumentsAdapter(argv), result); <%
else %>
    <%= $cur_module.name.downcase %>.<%= method.native_name %>(argumentsAdapter(argv), result); <%
end %>
    rho::String res = <%
if method.is_factory_method %>
        result.enumerateJSObjects();
<%
else %>
        result.toJson();
<%
end %>
    RAWTRACE2("%s(id=%s) end ^^^", __FUNCTION__, strObjID.c_str());
    return res;
}<%
end %>
