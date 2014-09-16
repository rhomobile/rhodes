#include "<%= $cur_module.name %>.h"

<% $cur_module.parents.each do |parent| %>
namespace <%= parent.downcase %> {<%
end %>

IMPLEMENT_LOGCLASS(C<%= $cur_module.name %>Base, "<%= $cur_module.name %>JNI");

//<%= $cur_module.name %>FactorySingleton staff
const char* const C<%= $cur_module.name %>Base::FACTORY_SINGLETON_CLASS = "<%= api_generator_java_makePackageName($cur_module) %>.<%= $cur_module.name %>FactorySingleton";
jclass C<%= $cur_module.name %>Base::s_clsFactorySingleton = 0;
jmethodID C<%= $cur_module.name %>Base::s_midFactorySetInstance;
jmethodID C<%= $cur_module.name %>Base::s_midFactoryGetInstance;

//<%= $cur_module.name %>Factory staff
const char* const C<%= $cur_module.name %>Base::IFACTORY_CLASS = "<%= api_generator_java_makePackageName($cur_module) %>.I<%= $cur_module.name %>Factory";
jclass C<%= $cur_module.name %>Base::s_clsIFactory = 0;
jmethodID C<%= $cur_module.name %>Base::s_midGetApiSingleton;
jmethodID C<%= $cur_module.name %>Base::s_midGetApiObject;

//<%= $cur_module.name %>SingletonBase staff
const char* const C<%= $cur_module.name %>Base::SINGLETON_BASE_CLASS = "<%= api_generator_java_makePackageName($cur_module) %>.<%= $cur_module.name %>SingletonBase";
jclass C<%= $cur_module.name %>Base::s_clsSingletonBase = 0;

//<%= $cur_module.name %>Base staff
const char* const C<%= $cur_module.name %>Base::OBJECT_BASE_CLASS = "<%= api_generator_java_makePackageName($cur_module) %>.<%= $cur_module.name %>Base";
jclass C<%= $cur_module.name %>Base::s_clsObjectBase = 0;

<% if $cur_module.is_template_default_instance %>
//IRhoApiDefaultId staff
const char* const C<%= $cur_module.name %>Base::IDEFAULTID_CLASS = "com.rhomobile.rhodes.api.IRhoApiDefaultId";
jclass C<%= $cur_module.name %>Base::s_clsIDefaultId = 0;
jmethodID C<%= $cur_module.name %>Base::s_midGetDefaultID;
jmethodID C<%= $cur_module.name %>Base::s_midSetDefaultID;
<% end %>

//Method tasks
<% $cur_module.methods.each do |method| %>
const char* const C<%= $cur_module.name %>Base::<%= method.native_name.upcase %>_TASK_CLASS = <%
if method.access == ModuleMethod::ACCESS_STATIC %>
        "<%= api_generator_java_makePackageName($cur_module) %>.<%= $cur_module.name %>SingletonBase$<%= method.native_name %>Task";
<% else %>
        "<%= api_generator_java_makePackageName($cur_module) %>.<%= $cur_module.name %>Base$<%= method.native_name %>Task";
<% end %>
jclass C<%= $cur_module.name %>Base::s_cls<%= method.native_name %>Task = 0;
jmethodID C<%= $cur_module.name %>Base::s_mid<%= method.native_name %>Task;
<% end %>

//----------------------------------------------------------------------------------------------------------------------

JNIEnv* C<%= $cur_module.name %>Base::jniInit()
{
    JNIEnv *env = jnienv();
    if(!env)
    {
        LOG(FATAL) + "JNI init failed: JNIEnv is null";
        return 0;
    }
    return jniInit(env);
}
//----------------------------------------------------------------------------------------------------------------------

JNIEnv* C<%= $cur_module.name %>Base::jniInit(JNIEnv* env)
{
    static bool initialized = false;
    env = MethodExecutorJni::jniInit(env);
    if (!env) {
        LOG(FATAL) + "JNI init failed";
        return 0;
    }

    if(!initialized)
    {
        //init <%= $cur_module.name %>FactorySingleton JNI
        s_clsFactorySingleton = loadClass(env, FACTORY_SINGLETON_CLASS);
        if (!s_clsFactorySingleton) return 0;

        s_midFactorySetInstance = env->GetStaticMethodID(s_clsFactorySingleton, "setInstance", "(L<%= api_generator_java_makePackagePath($cur_module) %>/I<%= $cur_module.name %>Factory;)V");
        if(!s_midFactorySetInstance)
        {
            LOG(FATAL) + "Failed to get method 'setInstance' for java class " + FACTORY_SINGLETON_CLASS;
            return NULL;
        }
        s_midFactoryGetInstance = env->GetStaticMethodID(s_clsFactorySingleton, "getInstance", "()L<%= api_generator_java_makePackagePath($cur_module) %>/I<%= $cur_module.name %>Factory;");
        if(!s_midFactoryGetInstance)
        {
            LOG(FATAL) + "Failed to get method 'getInstance' for java class " + FACTORY_SINGLETON_CLASS;
            return NULL;
        }

        //init I<%= $cur_module.name %>Factory JNI
        s_clsIFactory = loadClass(env, IFACTORY_CLASS);
        if (!s_clsIFactory) return 0;
        s_midGetApiSingleton = env->GetMethodID(s_clsIFactory, "getApiSingleton", "()L<%= api_generator_java_makePackagePath($cur_module) %>/I<%= $cur_module.name %>Singleton;");
        if(!s_midGetApiSingleton)
        {
            LOG(FATAL) + "Failed to get method 'getApiSingleton' for java class " + IFACTORY_CLASS;
            return NULL;
        }
        s_midGetApiObject = env->GetMethodID(s_clsIFactory, "getApiObject", "(Ljava/lang/String;)L<%= api_generator_java_makePackagePath($cur_module) %>/I<%= $cur_module.name %>;");
        if(!s_midGetApiObject)
        {
            LOG(FATAL) + "Failed to get method 'getApiObject' for java class " + IFACTORY_CLASS;
            return NULL;
        }

        s_clsSingletonBase = loadClass(env, SINGLETON_BASE_CLASS);
        if (!s_clsSingletonBase) return 0;
        s_clsObjectBase = loadClass(env, OBJECT_BASE_CLASS);
        if (!s_clsObjectBase) return 0;


<% $cur_module.methods.each do |method|
    java_class = "I#{$cur_module.name}"
    java_class += 'Singleton' if method.access == ModuleMethod::ACCESS_STATIC
    params_signature = "(L#{api_generator_java_makePackagePath($cur_module)}/#{java_class};"
    method.params.each do |param|
        params_signature += api_generator_java_makeNativeTypeSignature(param.type)
    end
    params_signature += "Lcom/rhomobile/rhodes/api/IMethodResult;)V"
%>
        s_cls<%= method.native_name %>Task = loadClass(env, <%= method.native_name.upcase %>_TASK_CLASS);
        if (!s_cls<%= method.native_name %>Task) return 0;
        s_mid<%= method.native_name %>Task = env->GetMethodID(s_cls<%= method.native_name %>Task, "<init>",
                        "<%= params_signature %>");
        if(!s_mid<%= method.native_name %>Task)
        {
            LOG(FATAL) + "Failed to get constructor for java class " + <%= method.native_name.upcase %>_TASK_CLASS;
            return NULL;
        }
<% end %>
<% if $cur_module.is_template_default_instance %>
        s_clsIDefaultId = loadClass(env, IDEFAULTID_CLASS);
        if (!s_clsIDefaultId) return 0;
        s_midGetDefaultID = env->GetMethodID(s_clsIDefaultId, "getDefaultID", "()Ljava/lang/String;");
        if(!s_midGetDefaultID)
        {
            LOG(FATAL) + "Failed to get method 'getDefaultID' for java class " + IDEFAULTID_CLASS;
            return NULL;
        }
        s_midSetDefaultID = env->GetMethodID(s_clsIDefaultId, "setDefaultID", "(Ljava/lang/String;)V");
        if(!s_midSetDefaultID)
        {
            LOG(FATAL) + "Failed to get method 'setDefaultID' for java class " + IDEFAULTID_CLASS;
            return NULL;
        }
<% end %>

        initialized = true;
        LOG(TRACE) + "C<%= $cur_module.name %> JNI init succeeded";
    }
    return env;
}
//----------------------------------------------------------------------------------------------------------------------

void C<%= $cur_module.name %>Base::setJavaFactory(JNIEnv* env, jobject jFactory)
{
    LOG(TRACE) + "setJavaFactory";

    env = jniInit(env);
    if (!env) {
        LOG(FATAL) + "JNI initialization failed";
        return;
    }

    env->CallStaticVoidMethod(s_clsFactorySingleton, s_midFactorySetInstance, jFactory);

    LOG(TRACE) + "setJavaFactory succeeded";
}
//----------------------------------------------------------------------------------------------------------------------
<% if $cur_module.is_template_default_instance %>
rho::String C<%= $cur_module.name %>Base::getDefaultID()
{
    LOG(TRACE) + "getDefaultID";

    JNIEnv *env = jniInit();
    if (!env) {
        LOG(FATAL) + "JNI initialization failed";
        return rho::String();
    }

    jhobject jhSingleton = getSingleton(env);
    jhstring res = static_cast<jstring>(env->CallObjectMethod(jhSingleton.get(), s_midGetDefaultID));
    return rho_cast<rho::String>(env, res);
}
//----------------------------------------------------------------------------------------------------------------------

void C<%= $cur_module.name %>Base::setDefaultID(const rho::String& id)
{
    LOG(TRACE) + "setDefaultID: " + id;

    JNIEnv *env = jniInit();
    if (!env) {
        LOG(FATAL) + "JNI initialization failed";
        return;
    }

    jhobject instance = getSingleton(env);
    jhstring jhId = rho_cast<jstring>(env, id);
    env->CallVoidMethod(instance.get(), s_midSetDefaultID, jhId.get());
}
//----------------------------------------------------------------------------------------------------------------------
<% end %>
jobject C<%= $cur_module.name %>Base::getFactory(JNIEnv* env)
{
    jobject res = env->CallStaticObjectMethod(s_clsFactorySingleton, s_midFactoryGetInstance);
    return res;
}
//----------------------------------------------------------------------------------------------------------------------

jobject C<%= $cur_module.name %>Base::getSingleton(JNIEnv* env)
{
    jhobject jhFactory = getFactory(env);
    jobject res = env->CallObjectMethod(jhFactory.get(), s_midGetApiSingleton);
    return res;
}
//----------------------------------------------------------------------------------------------------------------------

jobject C<%= $cur_module.name %>Base::getObject(JNIEnv* env)
{
    jhstring jhId = rho_cast<jstring>(env, m_id);
    jhobject jhFactory = getFactory(env);
    jobject res = env->CallObjectMethod(jhFactory.get(), s_midGetApiObject, jhId.get());
    return res;
}
//----------------------------------------------------------------------------------------------------------------------

<% $cur_module.parents.each do |parent| %>
}<%
end %>
