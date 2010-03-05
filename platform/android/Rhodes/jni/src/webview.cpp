#include "JNIRhodes.h"

#include <common/RhodesApp.h>

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "WebView"

RHO_GLOBAL void webview_navigate(char* url, int index)
{
    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_WEB_VIEW);
    if (!cls) return;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "navigate", "(Ljava/lang/String;I)V");
    if (!mid) return;

    char *normUrl = rho_http_normalizeurl(url);
    jstring objNormUrl = env->NewStringUTF(normUrl);
    env->CallStaticVoidMethod(cls, mid, objNormUrl, index);
    env->DeleteLocalRef(objNormUrl);
}

RHO_GLOBAL void webview_refresh(int index)
{
    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_WEB_VIEW);
    if (!cls) return;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "refresh", "(I)V");
    if (!mid) return;
    env->CallStaticVoidMethod(cls, mid, index);
}

RHO_GLOBAL char* webview_current_location(int index)
{
    static rho::String curLoc;

    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_WEB_VIEW);
    if (!cls) return NULL;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "currentLocation", "(I)Ljava/lang/String;");
    if (!mid) return NULL;

    jstring str = (jstring)env->CallStaticObjectMethod(cls, mid, index);
    const char *s = env->GetStringUTFChars(str, JNI_FALSE);
    curLoc = s;
    env->ReleaseStringUTFChars(str, s);
    return (char*)curLoc.c_str();
}

RHO_GLOBAL void webview_set_menu_items(VALUE valMenu)
{
    rho_rhodesapp_setViewMenu(valMenu);
}

RHO_GLOBAL int webview_active_tab()
{
    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_WEB_VIEW);
    if (!cls) return 0;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "activeTab", "()I");
    if (!mid) return 0;

    return env->CallStaticIntMethod(cls, mid);
}

RHO_GLOBAL char* webview_execute_js(char* js, int index)
{
    static rho::String result;

    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_WEB_VIEW);
    if (!cls) return NULL;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "executeJs", "(Ljava/lang/String;I)Ljava/lang/String;");
    if (!mid) return NULL;

    jstring objJs = env->NewStringUTF(js);
    jstring str = (jstring)env->CallStaticObjectMethod(cls, mid, objJs, index);
    env->DeleteLocalRef(objJs);
    const char *s = env->GetStringUTFChars(str, JNI_FALSE);
    result = s;
    env->ReleaseStringUTFChars(str, s);
    return (char*)result.c_str();
}

