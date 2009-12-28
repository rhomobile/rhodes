#include "JNIRhodes.h"

#include "RhoClassFactory.h"

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

RHO_GLOBAL int rho_net_has_network()
{
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_RHODES);
    if (!cls) return 0;
    jmethodID mid = getJNIClassStaticMethod(cls, "hasNetwork", "()Z");
    if (!mid) return 0;
    return jnienv()->CallStaticBooleanMethod(cls, mid);
}

RHO_GLOBAL void delete_files_in_folder(const char *szFolderPath)
{
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_RHODES);
    if (!cls) return;
    jmethodID mid = getJNIClassStaticMethod(cls, "deleteFilesInFolder", "(Ljava/lang/String;)V");
    if (!mid) return;
    JNIEnv *env = jnienv();
    jstring objFolderPath = env->NewStringUTF(szFolderPath);
    env->CallStaticVoidMethod(cls, mid, objFolderPath);
    env->DeleteLocalRef(objFolderPath);
}

RHO_GLOBAL VALUE rho_syscall(const char* callname, int nparams, char** param_names, char** param_values)
{
    // TODO:
    RHO_NOT_IMPLEMENTED;
    return 0;
}

RHO_GLOBAL void rho_net_impl_network_indicator(int enable)
{
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

RHO_GLOBAL const char *rho_sys_get_locale()
{
	if (g_currentLocale.empty())
	{
		jclass cls = getJNIClass(RHODES_JAVA_CLASS_RHODES);
		if (!cls) return NULL;
		jmethodID mid = getJNIClassStaticMethod(cls, "getCurrentLocale", "()Ljava/lang/String;");
		if (!mid) return NULL;
		JNIEnv *env = jnienv();
		jstring objLocale = (jstring)env->CallStaticObjectMethod(cls, mid);
		if (!objLocale) return NULL;
		const char *s = env->GetStringUTFChars(objLocale, JNI_FALSE);
		g_currentLocale = s;
		env->ReleaseStringUTFChars(objLocale, s);
	}
	return g_currentLocale.c_str();
}

RHO_GLOBAL int rho_sys_get_screen_width()
{
	if (g_screenWidth == 0)
	{
		jclass cls = getJNIClass(RHODES_JAVA_CLASS_RHODES);
		if (!cls) return 0;
		jmethodID mid = getJNIClassStaticMethod(cls, "getScreenWidth", "()I");
		if (!mid) return 0;
		g_screenWidth = jnienv()->CallStaticIntMethod(cls, mid);
	}
	return g_screenWidth;
}

RHO_GLOBAL int rho_sys_get_screen_height()
{
	if (g_screenHeight == 0)
	{
		jclass cls = getJNIClass(RHODES_JAVA_CLASS_RHODES);
		if (!cls) return 0;
		jmethodID mid = getJNIClassStaticMethod(cls, "getScreenHeight", "()I");
		if (!mid) return 0;
		g_screenHeight = jnienv()->CallStaticIntMethod(cls, mid);
	}
	return g_screenHeight;
}
