#include "JNIRhodes.h"

const char *javaWebViewClass = "com/rhomobile/rhodes/WebView";

RHO_GLOBAL void webview_navigate(char* url, int index)
{
    jclass cls = getJNIClass(javaWebViewClass);
    if (cls == NULL) return;
    jmethodID mid = getJNIClassMethod(cls, "navigate", "(Ljava/lang/String;)V");
    if (mid == NULL) return;
    jnienv()->CallStaticVoidMethod(cls, mid, jnienv()->NewStringUTF(url));
}

RHO_GLOBAL void webview_refresh()
{
    jclass cls = getJNIClass(javaWebViewClass);
    if (cls == NULL) return;
    jmethodID mid = getJNIClassMethod(cls, "refresh", "()V");
    if (mid == NULL) return;
    jnienv()->CallStaticVoidMethod(cls, mid);
}

RHO_GLOBAL char* webview_current_location()
{
    static rho::String curLoc;

    jclass cls = getJNIClass(javaWebViewClass);
    if (cls == NULL) return NULL;
    jmethodID mid = getJNIClassMethod(cls, "currentLocation", "()Ljava/lang/String;");
    if (mid == NULL) return NULL;
    jstring str = (jstring)jnienv()->CallStaticObjectMethod(cls, mid);
    const char *s = jnienv()->GetStringUTFChars(str, JNI_FALSE);
    curLoc = s;
    jnienv()->ReleaseStringUTFChars(str, s);
    return (char*)curLoc.c_str();
}

RHO_GLOBAL void webview_set_menu_items(VALUE argv)
{
    // TODO:
}

RHO_GLOBAL int webview_active_tab()
{
    // TODO:
    return 0;
}

RHO_GLOBAL char* webview_execute_js(char* js)
{
    // TODO:
    return NULL;
}

