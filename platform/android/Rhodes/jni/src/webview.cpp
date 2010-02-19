#include "JNIRhodes.h"

#include <common/RhodesApp.h>

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "WebView"

RHO_GLOBAL void webview_navigate(char* url, int index)
{
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_WEB_VIEW);
    if (!cls) return;
    jmethodID mid = getJNIClassStaticMethod(cls, "navigate", "(Ljava/lang/String;I)V");
    if (!mid) return;

    char *normUrl = rho_http_normalizeurl(url);
    JNIEnv *env = jnienv();
    jstring objNormUrl = env->NewStringUTF(normUrl);
    env->CallStaticVoidMethod(cls, mid, objNormUrl, index);
    env->DeleteLocalRef(objNormUrl);
}

RHO_GLOBAL void webview_refresh(int index)
{
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_WEB_VIEW);
    if (!cls) return;
    jmethodID mid = getJNIClassStaticMethod(cls, "refresh", "(I)V");
    if (!mid) return;
    jnienv()->CallStaticVoidMethod(cls, mid, index);
}

RHO_GLOBAL char* webview_current_location(int index)
{
    static rho::String curLoc;

    jclass cls = getJNIClass(RHODES_JAVA_CLASS_WEB_VIEW);
    if (!cls) return NULL;
    jmethodID mid = getJNIClassStaticMethod(cls, "currentLocation", "(I)Ljava/lang/String;");
    if (!mid) return NULL;

    JNIEnv *env = jnienv();
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
	jclass cls = getJNIClass(RHODES_JAVA_CLASS_WEB_VIEW);
	if (!cls) return 0;
	jmethodID mid = getJNIClassStaticMethod(cls, "activeTab", "()I");
	if (!mid) return 0;

	JNIEnv *env = jnienv();
	return env->CallStaticIntMethod(cls, mid);
}

RHO_GLOBAL char* webview_execute_js(char* js, int index)
{
    static rho::String result;

    jclass cls = getJNIClass(RHODES_JAVA_CLASS_WEB_VIEW);
    if (!cls) return NULL;
    jmethodID mid = getJNIClassStaticMethod(cls, "executeJs", "(Ljava/lang/String;I)Ljava/lang/String;");
    if (!mid) return NULL;

    JNIEnv *env = jnienv();
    jstring objJs = env->NewStringUTF(js);
    jstring str = (jstring)env->CallStaticObjectMethod(cls, mid, objJs, index);
    env->DeleteLocalRef(objJs);
    const char *s = env->GetStringUTFChars(str, JNI_FALSE);
    result = s;
    env->ReleaseStringUTFChars(str, s);
    return (char*)result.c_str();
}

