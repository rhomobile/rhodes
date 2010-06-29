#include "rhodes/JNIRhodes.h"

#include <common/RhodesApp.h>

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "WebView"

RHO_GLOBAL void rho_webview_navigate(const char* url, int index)
{
    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_WEB_VIEW);
    if (!cls) return;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "navigate", "(Ljava/lang/String;I)V");
    if (!mid) return;

    if ( !url )
    {
        RAWLOG_ERROR("WebView.navigate failed: url is nil");
        return;
    }

    char *normUrl = rho_http_normalizeurl(url);
    jstring objNormUrl = rho_cast<jstring>(normUrl);
    env->CallStaticVoidMethod(cls, mid, objNormUrl, index);
    env->DeleteLocalRef(objNormUrl);
}

RHO_GLOBAL void rho_webview_refresh(int index)
{
    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_WEB_VIEW);
    if (!cls) return;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "refresh", "(I)V");
    if (!mid) return;
    env->CallStaticVoidMethod(cls, mid, index);
}

RHO_GLOBAL void rho_webview_navigate_back()
{
    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_WEB_VIEW);
    if (!cls) return;
    jmethodID mid =getJNIClassStaticMethod(env, cls, "navigateBack", "()V");
    if (!mid) return;
    env->CallStaticVoidMethod(cls, mid);
}

RHO_GLOBAL const char* rho_webview_current_location(int index)
{
    return rho_rhodesapp_getcurrenturl(index);
}

RHO_GLOBAL void rho_webview_set_menu_items(VALUE valMenu)
{
    rho_rhodesapp_setViewMenu(valMenu);
}

RHO_GLOBAL int rho_webview_active_tab()
{
    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_WEB_VIEW);
    if (!cls) return 0;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "activeTab", "()I");
    if (!mid) return 0;

    return env->CallStaticIntMethod(cls, mid);
}

RHO_GLOBAL const char* rho_webview_execute_js(const char* js, int index)
{
    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_WEB_VIEW);
    if (!cls) return NULL;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "executeJs", "(Ljava/lang/String;I)V");
    if (!mid) return NULL;

    jstring objJs = rho_cast<jstring>(env, js);
    env->CallStaticVoidMethod(cls, mid, objJs, index);
    env->DeleteLocalRef(objJs);
    return "";
}

RHO_GLOBAL void rho_webview_full_screen_mode(int enable)
{
    //TODO: rho_webview_full_screen_mode
}

RHO_GLOBAL void rho_webview_set_cookie(const char *url, const char *cookie)
{
    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_WEB_VIEW);
    if (!cls) return;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "setCookie", "(Ljava/lang/String;Ljava/lang/String;)V");
    if (!mid) return;

    jstring urlObj = rho_cast<jstring>(url);
    jstring cookieObj = rho_cast<jstring>(cookie);
    env->CallStaticVoidMethod(cls, mid, urlObj, cookieObj);
    env->DeleteLocalRef(urlObj);
    env->DeleteLocalRef(cookieObj);
}
