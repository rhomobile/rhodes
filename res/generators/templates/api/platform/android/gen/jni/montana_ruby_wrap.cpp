#include "<%= $cur_module.name %>.h"

#include "MethodResultJni.h"

#include "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "<%= $cur_module.name %>RUBY"

#include "ext/rho/rhoruby.h"
#include "rhodes/JNIRhoRuby.h"

typedef <%= api_generator_cpp_MakeNamespace($cur_module.parents)%>C<%= $cur_module.name %>Proxy<ArgumentsAdapter<std::vector<VALUE> > > ObjectProxy;

using namespace rhoelements;
extern "C"
{

VALUE getRuby_<%= $cur_module.name %>_Module();

<% if $cur_module.is_template_default_instance %>
VALUE rb_<%= $cur_module.name %>_s_default(VALUE klass)
{
    RAWTRACE("rb_barcode1_s_default");
    return rho_ruby_create_object_with_id(klass, <%= api_generator_cpp_MakeNamespace($cur_module.parents)%>C<%= $cur_module.name %>Base::getDefaultID().c_str());
}

VALUE rb_<%= $cur_module.name %>_s_setDefault(VALUE klass, VALUE valObj)
{
    RAWTRACE("rb_barcode1_s_set_default");

    const char* szID = rho_ruby_get_object_id(valObj);
    <%= api_generator_cpp_MakeNamespace($cur_module.parents)%>C<%= $cur_module.name %>Base::setDefaultID(szID);

    return rho_ruby_get_NIL();
} <%
end %>

<%
$cur_module.methods.each do |method|
if method.access == ModuleMethod::ACCESS_STATIC %>
<%= api_generator_MakeRubyMethodDecl($cur_module.name, method, true)%>
{
    RAWTRACE(__FUNCTION__);<%
else %>
static VALUE <%= $cur_module.name %>_<%= method.native_name %>(int argc, VALUE *argv, const rho::String& id)
{
    RAWTRACE2("%s(id=%s)", __FUNCTION__, id.c_str());<%
end %>
    MethodResultJni result;
    if(!result)
    {
        result.setError("JNI error: failed to initialize MethodResult java object");
        RAWLOG_ERROR("JNI error: failed to initialize MethodResult java object ^^^");
        return result.toRuby();
    }
<%
unless method.access == ModuleMethod::ACCESS_STATIC %>
    ObjectProxy <%= $cur_module.name.downcase %>(id);<%
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

    if((argc < <%= min_params %>) || (argc > <%= max_params %>))
    {
        result.setArgError("Wrong number of arguments");
        RAWLOG_ERROR1("Wrong number of arguments: %d ^^^", argc);
        return result.toRuby();
    }
    
    unsigned realParamCount = (argc < <%= method.params.size %>) ? argc : <%= method.params.size %>;
    std::vector<VALUE> arguments(argv, argv + realParamCount);
    if(argc > <%= method.params.size %>)
    {
        if (rho_ruby_is_proc(argv[<%= method.params.size %>]) || rho_ruby_is_method(argv[<%= method.params.size %>]))
        {
            result.setRubyProcCallBack(argv[<%= method.params.size %>]);
        } else
        {
            if(argc > <%= method.params.size+1 %>)
                result.setCallBack(argv[<%= method.params.size %>], argv[<%= method.params.size+1 %>]);
            else
                result.setCallBack(argv[<%= method.params.size %>]);
    
        }
        if(!result.hasCallBackUrl())
        {
            RAWLOG_ERROR("Error setting callback ^^^");
            return result.toRuby();
        }
    }
<%
if method.access == ModuleMethod::ACCESS_STATIC %>
    ObjectProxy::<%= method.native_name %>(argumentsAdapter(arguments), result); <%
else %>
    <%= $cur_module.name.downcase %>.<%= method.native_name %>(argumentsAdapter(arguments), result); <%
end %>
    VALUE res = <%
if api_generator_isApiObjectParam(method.result) %>
        result.enumerateRubyObjects(rho_ruby_get_class_byname("<%=api_generator_ruby_makeApiObjectTypeName(method.result, $cur_module)%>"));
<%
else %>
        result.toRuby();
<%
end %>
    RAWTRACE(__FUNCTION__);
    return res;
}<%
if method.access != ModuleMethod::ACCESS_STATIC %>
<%= api_generator_MakeRubyMethodDecl($cur_module.name, method, false) %>
{
    rho::String id = rho_ruby_get_object_id(obj);
    return <%= $cur_module.name %>_<%= method.native_name %>(argc, argv, id);
}<%
end %>
<%
if $cur_module.is_template_default_instance && method.access == ModuleMethod::ACCESS_INSTANCE %>
<%= api_generator_MakeRubyMethodDecl($cur_module.name + "_def", method, true)%>
{
    rho::String id = ObjectProxy::getDefaultID();
    return <%= $cur_module.name %>_<%= method.native_name %>(argc, argv, id);
}<%
end
end %>
}
