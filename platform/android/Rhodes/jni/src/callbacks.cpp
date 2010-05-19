#include "rhodes/JNIRhodes.h"

#include "rhodes/RhoClassFactory.h"

#include <ruby/ext/rho/rhoruby.h>

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "Callbacks"

extern "C" void rho_webview_navigate(const char* url, int index);

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
    rho_webview_navigate(url.c_str(), 0);
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
    // No GUI indicator on Android
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

RHO_GLOBAL int rho_sysimpl_get_property(char* szPropName, VALUE* resValue)
{
    JNIEnv *env = jnienv();

    jclass cls = getJNIClass(RHODES_JAVA_CLASS_RHODES);
    if (!cls) return 0;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "getProperty", "(Ljava/lang/String;)Ljava/lang/Object;");
    if (!mid) return 0;

    jobject result = env->CallStaticObjectMethod(cls, mid, rho_cast<jstring>(szPropName));
    if (!result) return 0;

    jclass clsBoolean = getJNIClass(RHODES_JAVA_CLASS_BOOLEAN);
    jclass clsInteger = getJNIClass(RHODES_JAVA_CLASS_INTEGER);
    jclass clsFloat = getJNIClass(RHODES_JAVA_CLASS_FLOAT);
    jclass clsDouble = getJNIClass(RHODES_JAVA_CLASS_DOUBLE);
    jclass clsString = getJNIClass(RHODES_JAVA_CLASS_STRING);

    if (env->IsInstanceOf(result, clsBoolean)) {
        jmethodID midValue = getJNIClassMethod(env, clsBoolean, "booleanValue", "()Z");
        *resValue = rho_ruby_create_boolean((int)env->CallBooleanMethod(result, midValue));
        return 1;
    }
    else if (env->IsInstanceOf(result, clsInteger)) {
        jmethodID midValue = getJNIClassMethod(env, clsInteger, "intValue", "()I");
        *resValue = rho_ruby_create_integer((int)env->CallIntMethod(result, midValue));
        return 1;
    }
    else if (env->IsInstanceOf(result, clsFloat)) {
        jmethodID midValue = getJNIClassMethod(env, clsFloat, "floatValue", "()F");
        *resValue = rho_ruby_create_double((double)env->CallFloatMethod(result, midValue));
        return 1;
    }
    else if (env->IsInstanceOf(result, clsDouble)) {
        jmethodID midValue = getJNIClassMethod(env, clsDouble, "doubleValue", "()D");
        *resValue = rho_ruby_create_double((double)env->CallDoubleMethod(result, midValue));
        return 1;
    }
    else if (env->IsInstanceOf(result, clsString)) {
        jstring resStrObj = (jstring)result;
        *resValue = rho_ruby_create_string(rho_cast<std::string>(resStrObj).c_str());
        return 1;
    }

    return 0;
}

RHO_GLOBAL VALUE rho_sys_get_locale()
{
    VALUE res;
    if ( rho_sysimpl_get_property((char*)"locale", &res) )
        return res;

    return rho_ruby_get_NIL();
}

RHO_GLOBAL int rho_sys_get_screen_width()
{
    VALUE res;
    if ( rho_sysimpl_get_property((char*)"screen_width", &res) )
        return NUM2INT(res);

    return 0;
}

RHO_GLOBAL int rho_sys_get_screen_height()
{
    VALUE res;
    if ( rho_sysimpl_get_property((char*)"screen_height", &res) )
        return NUM2INT(res);

    return 0;
}

RHO_GLOBAL void rho_sys_app_exit()
{
    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_RHODES);
    if (!cls) return;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "exit", "()V");
    if (!mid) return;
    env->CallStaticVoidMethod(cls, mid);
}

