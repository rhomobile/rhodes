#pragma once

#include "common/RhoStd.h"
#include "logging/RhoLog.h"
#include "rhodes/JNIRhodes.h"
#include "MethodExecutorJni.h"
#include "MethodResultJni.h"

<% $cur_module.parents.each do |parent| %>
namespace <%= parent.downcase() %> {<%
end %>

using rho::apiGenerator::MethodResultJni;
using rho::apiGenerator::MethodExecutorJni;

class C<%= $cur_module.name %>Base : public MethodExecutorJni
{
protected:
    DEFINE_LOGCLASS;

    static const char* const FACTORY_SINGLETON_CLASS;
    static jclass s_clsFactorySingleton;
    static jmethodID s_midFactorySetInstance;
    static jmethodID s_midFactoryGetInstance;

    //I<%= $cur_module.name %>Factory staff
    static const char* const IFACTORY_CLASS;
    static jclass s_clsIFactory;
    static jmethodID s_midGetApiSingleton;
    static jmethodID s_midGetApiObject;

    //<%= $cur_module.name %>SingletonBase staff
    static const char* const SINGLETON_BASE_CLASS;
    static jclass s_clsSingletonBase;

    //<%= $cur_module.name %>Base staff
    static const char* const OBJECT_BASE_CLASS;
    static jclass s_clsObjectBase;

    //Method tasks
<% $cur_module.methods.each do |method| %>
    static const char* const <%= method.native_name.upcase %>_TASK_CLASS;
    static jclass s_cls<%= method.native_name %>Task;
    static jmethodID s_mid<%= method.native_name %>Task;<%
end %>
<%
if $cur_module.is_template_default_instance %>
    //IRhoApiDefaultId staff
    static const char* const IDEFAULTID_CLASS;
    static jclass s_clsIDefaultId;
    static jmethodID s_midGetDefaultID;
    static jmethodID s_midSetDefaultID;<%
end %>

    static JNIEnv* jniInit(JNIEnv* env);
    static JNIEnv* jniInit();

    static jobject getFactory(JNIEnv* env);
    static jobject getSingleton(JNIEnv* env);

    rho::String m_id;
    jobject getObject(JNIEnv* env);
public:
    static void setJavaFactory(JNIEnv* env, jobject jFactory);
<%
if $cur_module.is_template_default_instance %>
    static rho::String getDefaultID();
    static void setDefaultID(const rho::String& id); <%
end %>

    C<%= $cur_module.name %>Base(const rho::String& id)
        : MethodExecutorJni(), m_id(id)
        {}
    virtual ~C<%= $cur_module.name %>Base() {}
};

template <typename T>
class C<%= $cur_module.name %>Proxy : public C<%= $cur_module.name %>Base
{
public:
    C<%= $cur_module.name %>Proxy(const rho::String& id)
        : C<%= $cur_module.name %>Base(id)
        {}
    virtual ~C<%= $cur_module.name %>Proxy() {}
<%
$cur_module.methods.each do |method| 
if method.access == ModuleMethod::ACCESS_STATIC %>
    static<%
end %>
    void <%= method.native_name %>(const T& argsAdapter, MethodResultJni& result)
    {
        LOG(TRACE) + "<%= method.native_name %>";

        JNIEnv *env = jniInit();
        if (!env) {
            LOG(FATAL) + "JNI initialization failed";
            return;
        }

        jhobject jhObject = <%
if method.access == ModuleMethod::ACCESS_STATIC %>
            getSingleton(env); <%
else %>
            getObject(env); <%
end

method.params.each_index do |index| 
param = method.params[index]
  if param.default_value || param.can_be_nil
    def_val = '0';
    if(param.default_value)
      if(param.type == MethodParam::TYPE_STRING)
        def_val = "rho_cast<#{api_generator_jni_makeJNIType(param.type)}>(env, \"#{param.default_value}\")"
      else
        def_val = "rho_cast<#{api_generator_jni_makeJNIType(param.type)}>(env, static_cast<#{api_generator_cpp_makeNativeTypeArg(param.type)}>(#{param.default_value}))"
      end
    end %>

        jholder< <%=api_generator_jni_makeJNIType(param.type) %> > jh<%= param.name %> = (argsAdapter.size() <= <%= index %>) ?
            <%= def_val %> : <%
    if param.is_property_hash %>
                convertToPropertyMap(env, argsAdapter[<%= index %>]);<%
    else %>
                rho_cast< <%=api_generator_jni_makeJNIType(param.type) %> >(env, argsAdapter[<%= index %>]);<%
    end
  else %>

        if(argsAdapter.size() <= <%= index %>)
        {
            LOG(ERROR) + "Wrong number of arguments: '<%= param.name %>' must be set ^^^";
            result.setArgError("Wrong number of arguments: '<%= param.name %>' must be set");
            return;
        }
        jholder< <%=api_generator_jni_makeJNIType(param.type) %> > jh<%= param.name %> = <%
    if param.is_property_hash %>
            convertToPropertyMap(env, argsAdapter[<%= index %>]);<%
    else %>
            rho_cast< <%=api_generator_jni_makeJNIType(param.type) %> >(env, argsAdapter[<%= index %>]);<%
    end
  end
end %>

        jhobject jhTask = env->NewObject(s_cls<%= method.native_name%>Task, s_mid<%= method.native_name%>Task,
                    jhObject.get(), <%
method.params.each do |param| %>
                    jh<%= param.name %>.get(),<%
end %>
                    static_cast<jobject>(result));

        run(env, jhTask.get(), result, <%=
if method.run_in_thread == ModuleMethod::RUN_IN_THREAD_UNDEFINED
    "rho::apiGenerator::NOT_FORCE_THREAD"
elsif method.run_in_thread == ModuleMethod::RUN_IN_THREAD_NONE
    "rho::apiGenerator::FORCE_CURRENT_THREAD"
elsif method.run_in_thread == ModuleMethod::RUN_IN_THREAD_UI
    "rho::apiGenerator::FORCE_UI_THREAD"
elsif method.run_in_thread == ModuleMethod::RUN_IN_THREAD_MODULE
    "rho::apiGenerator::FORCE_MODULE_THREAD"
elsif method.run_in_thread == ModuleMethod::RUN_IN_THREAD_SEPARATED
    "rho::apiGenerator::FORCE_NEW_THREAD"
end %>);
        if(env->ExceptionCheck() == JNI_TRUE)
        {
            rho::String message = rho::common::clearException(env);
            LOG(ERROR) + message;
            result.setError(message);
        }
    }
<%
end %>

};

<% $cur_module.parents.each do |parent| %>
}<%
end %>
