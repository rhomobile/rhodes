#include "JNIRhodes.h"

#include "RhoClassFactory.h"
#include "ruby/ext/rho/rhoruby.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "Callbacks"

extern "C" void webview_navigate(char* url, int index);

static rho::String g_currentLocale;
static int g_screenWidth;
static int g_screenHeight;

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

RHO_GLOBAL VALUE rho_sys_get_locale()
{
    if (g_currentLocale.empty())
    {
        JNIEnv *env = jnienv();
        jclass cls = getJNIClass(RHODES_JAVA_CLASS_RHODES);
        if (!cls) return rho_ruby_create_string("");
        jmethodID mid = getJNIClassStaticMethod(env, cls, "getCurrentLocale", "()Ljava/lang/String;");
        if (!mid) return rho_ruby_create_string("");
        jstring objLocale = (jstring)env->CallStaticObjectMethod(cls, mid);
        if (!objLocale) return rho_ruby_create_string("");
        const char *s = env->GetStringUTFChars(objLocale, JNI_FALSE);
        g_currentLocale = s;
        env->ReleaseStringUTFChars(objLocale, s);
    }
    return rho_ruby_create_string(g_currentLocale.c_str());
}

RHO_GLOBAL int rho_sys_get_screen_width()
{
    if (g_screenWidth == 0)
    {
        JNIEnv *env = jnienv();
        jclass cls = getJNIClass(RHODES_JAVA_CLASS_RHODES);
        if (!cls) return 0;
        jmethodID mid = getJNIClassStaticMethod(env, cls, "getScreenWidth", "()I");
        if (!mid) return 0;
        g_screenWidth = env->CallStaticIntMethod(cls, mid);
    }
    return g_screenWidth;
}

RHO_GLOBAL int rho_sys_get_screen_height()
{
    if (g_screenHeight == 0)
    {
        JNIEnv *env = jnienv();
        jclass cls = getJNIClass(RHODES_JAVA_CLASS_RHODES);
        if (!cls) return 0;
        jmethodID mid = getJNIClassStaticMethod(env, cls, "getScreenHeight", "()I");
        if (!mid) return 0;
        g_screenHeight = env->CallStaticIntMethod(cls, mid);
    }
    return g_screenHeight;
}

RHO_GLOBAL VALUE rho_sysimpl_get_property(char* szPropName)
{
    VALUE nil = rho_ruby_get_NIL();

    JNIEnv *env = jnienv();

    jclass cls = getJNIClass(RHODES_JAVA_CLASS_RHODES);
    if (!cls) return nil;
    if (strcasecmp("has_camera", szPropName) == 0) {
        jmethodID mid = getJNIClassStaticMethod(env, cls, "hasCamera", "()Z");
        if (!mid) return nil;
        jboolean result = env->CallStaticBooleanMethod(cls, mid);
        return rho_ruby_create_boolean((int)result);
    }

    return nil;
}
