#pragma once

#include "common/RhoStd.h"
#include "logging/RhoLog.h"
#include "rhodes/JNIRhodes.h"
#include "MethodExecutorJni.h"

namespace rhoelements {
  class MethodResultJni;
}

<% $cur_module.parents.each do |parent| %>
namespace <%= parent.downcase() %> {<%
end %>

using rhoelements::MethodResultJni;
using rhoelements::MethodExecutorJni;

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

        RHO_ASSERT(argsAdapter.size() >= <%= method.params.size %>);
        RHO_ASSERT(argsAdapter.size() <= (<%= method.params.size %> + 2));

        jhobject jhObject = <%
if method.access == ModuleMethod::ACCESS_STATIC %>
            getSingleton(env); <%
else %>
            getObject(env); <%
end

method.params.each_index do |index| 
param = method.params[index] %>

        jholder< <%=api_generator_jni_makeJNIType(param.type) %> > jh<%= param.name %>
            = rho_cast< <%=api_generator_jni_makeJNIType(param.type) %> >(env, argsAdapter[<%= index %>]);<%
end %>
        jhobject jhTask = env->NewObject(s_cls<%= method.native_name%>Task, s_mid<%= method.native_name%>Task,
                    jhObject.get(), <%
method.params.each do |param| %>
                    jh<%= param.name %>.get(),<%
end %>
                    static_cast<jobject>(result));

        run(env, jhTask.get(), result, <%=
if method.run_in_thread == ModuleMethod::RUN_IN_THREAD_NONE
    "false, false"
elsif method.run_in_thread == ModuleMethod::RUN_IN_THREAD_UI
    "true, false"
else
    "true, true"
end%>);
    }
<%
end %>

};

<% $cur_module.parents.each do |parent| %>
}<%
end %>
