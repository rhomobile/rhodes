#include "JNIRhodes.h"

#include "RhoClassFactory.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "Callbacks"


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
    // TODO:
    RHO_NOT_IMPLEMENTED;
}

RHO_GLOBAL void rho_appmanager_load( void* httpContext, const char* szQuery)
{
    // TODO:
    RHO_NOT_IMPLEMENTED;
}

RHO_GLOBAL int rho_net_has_network()
{
    RHO_LOG_CALLBACK;
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_RHODES);
    if (!cls) return 0;
    jmethodID mid = getJNIClassStaticMethod(cls, "hasNetwork", "()Z");
    if (!mid) return 0;
    return jnienv()->CallStaticBooleanMethod(cls, mid);
}

RHO_GLOBAL void delete_files_in_folder(const char *szFolderPath)
{
    RHO_LOG_CALLBACK;
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_RHODES);
    if (!cls) return;
    jmethodID mid = getJNIClassStaticMethod(cls, "deleteFilesInFolder", "(Ljava/lang/String;)V");
    if (!mid) return;
    JNIEnv *env = jnienv();
    env->CallStaticVoidMethod(cls, mid, env->NewStringUTF(szFolderPath));
}

RHO_GLOBAL VALUE rho_syscall(const char* callname, int nparams, char** param_names, char** param_values)
{
    // TODO:
    RHO_NOT_IMPLEMENTED;
    return 0;
}

RHO_GLOBAL void rho_net_impl_network_indicator(int enable)
{
    RHO_LOG_CALLBACK;
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_RHODES);
    if (!cls) return;
    jmethodID mid = getJNIClassStaticMethod(cls, "showNetworkIndicator", "(Z)V");
    if (!mid) return;
    jnienv()->CallStaticVoidMethod(cls, mid, enable);
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
