#include "JNIRhodes.h"

#include "RhoClassFactory.h"
#include "ruby/ext/rho/rhoruby.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "Callbacks"

extern "C" void webview_navigate(char* url, int index);

namespace rho
{
namespace common
{

IRhoClassFactory* createClassFactory()
{
    return new CRhoClassFactory();
}

} // namespace common
} // namespace rho

RHO_GLOBAL void rho_map_location(char* query)
{
    rho::String url = "http://maps.google.com/?";
    url += query;
    webview_navigate((char*)url.c_str(), 0);
}

RHO_GLOBAL void rho_appmanager_load( void* /*httpContext*/, const char* /*szQuery*/)
{
    // Nothing
}

RHO_GLOBAL VALUE rho_sys_has_network()
{
    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_RHODES);
    if (!cls) return rho_ruby_create_boolean(0);
    jmethodID mid = getJNIClassStaticMethod(env, cls, "hasNetwork", "()Z");
    if (!mid) return rho_ruby_create_boolean(0);
    return rho_ruby_create_boolean(env->CallStaticBooleanMethod(cls, mid));
}

RHO_GLOBAL void delete_files_in_folder(const char *szFolderPath)
{
    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_RHODES);
    if (!cls) return;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "deleteFilesInFolder", "(Ljava/lang/String;)V");
    if (!mid) return;
    jstring objFolderPath = rho_cast<jstring>(szFolderPath);
    env->CallStaticVoidMethod(cls, mid, objFolderPath);
    env->DeleteLocalRef(objFolderPath);
}

RHO_GLOBAL VALUE rho_sys_makephonecall(const char* callname, int nparams, char** param_names, char** param_values)
{
    // TODO:
    RHO_NOT_IMPLEMENTED;
    return 0;
}

RHO_GLOBAL void rho_net_impl_network_indicator(int enable)
{
    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_RHODES);
    if (!cls) return;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "showNetworkIndicator", "(Z)V");
    if (!mid) return;
    env->CallStaticVoidMethod(cls, mid, enable);
}

RHO_GLOBAL void *rho_nativethread_start()
{
    JNIEnv *env;
    jvm()->AttachCurrentThread(&env, NULL);
    store_thr_jnienv(env);
    return NULL;
}

RHO_GLOBAL void rho_nativethread_end(void *)
{
    jvm()->DetachCurrentThread();
}

RHO_GLOBAL VALUE rho_sysimpl_get_property(char* szPropName)
{
    VALUE nil = rho_ruby_get_NIL();

    JNIEnv *env = jnienv();

    jclass cls = getJNIClass(RHODES_JAVA_CLASS_RHODES);
    if (!cls) return nil;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "getProperty", "(Ljava/lang/String;)Ljava/lang/Object;");
    if (!mid) return nil;

    jobject result = env->CallStaticObjectMethod(cls, mid, rho_cast<jstring>(szPropName));
    if (!result) return nil;

    jclass clsBoolean = getJNIClass(RHODES_JAVA_CLASS_BOOLEAN);
    jclass clsInteger = getJNIClass(RHODES_JAVA_CLASS_INTEGER);
    jclass clsString = getJNIClass(RHODES_JAVA_CLASS_STRING);

    if (env->IsInstanceOf(result, clsBoolean)) {
        jmethodID midValue = getJNIClassMethod(env, clsBoolean, "booleanValue", "()Z");
        return rho_ruby_create_boolean((int)env->CallBooleanMethod(result, midValue));
    }
    else if (env->IsInstanceOf(result, clsInteger)) {
        jmethodID midValue = getJNIClassMethod(env, clsInteger, "intValue", "()I");
        return rho_ruby_create_integer((int)env->CallIntMethod(result, midValue));
    }
    else if (env->IsInstanceOf(result, clsString)) {
        jstring resStrObj = (jstring)result;
        return rho_ruby_create_string(rho_cast<std::string>(resStrObj).c_str());
    }

    return nil;
}

RHO_GLOBAL VALUE rho_sys_get_locale()
{
    return rho_sysimpl_get_property((char*)"locale");
}

RHO_GLOBAL int rho_sys_get_screen_width()
{
    return NUM2INT(rho_sysimpl_get_property((char*)"screen_width"));
}

RHO_GLOBAL int rho_sys_get_screen_height()
{
    return NUM2INT(rho_sysimpl_get_property((char*)"screen_height"));
}

