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
    jstring objFolderPath = env->NewStringUTF(szFolderPath);
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

    RAWLOG_INFO("get_property (1)");
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_RHODES);
    if (!cls) return nil;
    RAWLOG_INFO("get_property (2)");
    jmethodID mid = getJNIClassStaticMethod(env, cls, "getProperty", "(Ljava/lang/String;)Ljava/lang/Object;");
    if (!mid) return nil;

    RAWLOG_INFO("get_property (3)");
    jobject result = env->CallStaticObjectMethod(cls, mid, env->NewStringUTF(szPropName));
    if (!result) return nil;

    RAWLOG_INFO("get_property (4)");
    jclass clsBoolean = getJNIClass(RHODES_JAVA_CLASS_BOOLEAN);
    jclass clsInteger = getJNIClass(RHODES_JAVA_CLASS_INTEGER);
    jclass clsString = getJNIClass(RHODES_JAVA_CLASS_STRING);

    RAWLOG_INFO("get_property (5)");
    if (env->IsInstanceOf(result, clsBoolean)) {
        RAWLOG_INFO("get_property (6.1)");
        jmethodID midValue = getJNIClassMethod(env, clsBoolean, "booleanValue", "()Z");
        RAWLOG_INFO("get_property (6.2)");
        return rho_ruby_create_boolean((int)env->CallBooleanMethod(result, midValue));
    }
    else if (env->IsInstanceOf(result, clsInteger)) {
        RAWLOG_INFO("get_property (7.1)");
        jmethodID midValue = getJNIClassMethod(env, clsInteger, "intValue", "()I");
        RAWLOG_INFO("get_property (7.2)");
        return rho_ruby_create_integer((int)env->CallIntMethod(result, midValue));
    }
    else if (env->IsInstanceOf(result, clsString)) {
        RAWLOG_INFO("get_property (8.1)");
        jstring resStrObj = (jstring)result;
        RAWLOG_INFO("get_property (8.2)");
        const char *s = env->GetStringUTFChars(resStrObj, JNI_FALSE);
        RAWLOG_INFO("get_property (8.3)");
        VALUE ret = rho_ruby_create_string(s);
        RAWLOG_INFO("get_property (8.4)");
        env->ReleaseStringUTFChars(resStrObj, s);
        RAWLOG_INFO("get_property (8.5)");
        return ret;
    }

    RAWLOG_INFO("get_property (exit");
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

