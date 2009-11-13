#include "JNIRhodes.h"

#include <common/RhodesApp.h>

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "WebView"

RHO_GLOBAL void webview_navigate(char* url, int index)
{
    RHO_LOG_CALLBACK;
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_WEB_VIEW);
    if (!cls) return;
    jmethodID mid = getJNIClassStaticMethod(cls, "navigate", "(Ljava/lang/String;)V");
    if (!mid) return;

    JNIEnv *env = jnienv();
    env->CallStaticVoidMethod(cls, mid, env->NewStringUTF(url));
}

RHO_GLOBAL void webview_refresh()
{
    RHO_LOG_CALLBACK;
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_WEB_VIEW);
    if (!cls) return;
    jmethodID mid = getJNIClassStaticMethod(cls, "refresh", "()V");
    if (!mid) return;
    jnienv()->CallStaticVoidMethod(cls, mid);
}

RHO_GLOBAL char* webview_current_location()
{
    RHO_LOG_CALLBACK;
    static rho::String curLoc;

    jclass cls = getJNIClass(RHODES_JAVA_CLASS_WEB_VIEW);
    if (!cls) return NULL;
    jmethodID mid = getJNIClassStaticMethod(cls, "currentLocation", "()Ljava/lang/String;");
    if (!mid) return NULL;

    JNIEnv *env = jnienv();
    jstring str = (jstring)env->CallStaticObjectMethod(cls, mid);
    const char *s = env->GetStringUTFChars(str, JNI_FALSE);
    curLoc = s;
    env->ReleaseStringUTFChars(str, s);
    return (char*)curLoc.c_str();
}

RHO_GLOBAL void webview_set_menu_items(VALUE valMenu)
{
    RHO_LOG_CALLBACK;
    rho_rhodesapp_setViewMenu(valMenu);
}

RHO_GLOBAL int webview_active_tab()
{
    RHO_LOG_CALLBACK;
    return 0;
}

RHO_GLOBAL char* webview_execute_js(char* js)
{
    RHO_LOG_CALLBACK;
    static rho::String result;

    jclass cls = getJNIClass(RHODES_JAVA_CLASS_WEB_VIEW);
    if (!cls) return NULL;
    jmethodID mid = getJNIClassStaticMethod(cls, "executeJs", "(Ljava/lang/String;)Ljava/lang/String;");
    if (!mid) return NULL;

    JNIEnv *env = jnienv();
    jstring str = (jstring)env->CallStaticObjectMethod(cls, mid, env->NewStringUTF(js));
    const char *s = env->GetStringUTFChars(str, JNI_FALSE);
    result = s;
    env->ReleaseStringUTFChars(str, s);
    return (char*)result.c_str();
}

