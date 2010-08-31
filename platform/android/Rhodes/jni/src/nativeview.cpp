#include <rhodes.h>
#include <android/log.h>
#include <common/RhodesApp.h>
#include <logging/RhoLogConf.h>
#include "rhodes/JNIRhodes.h"


#include <stdlib.h>


#import <RhoNativeViewManager.h> 



void RhoNativeViewManager::registerViewType(const char* viewType, NativeViewFactory* factory) {
}

void RhoNativeViewManager::unregisterViewType(const char* viewType) {
}

// that function return native object used for display Web content :
// UIWebView* for iPhone
// jobject for Android - jobect is android.webkit.WebView class type
// HWND for Windows Mobile 
void* RhoNativeViewManager::getWebViewObject(int tab_index) {
    JNIEnv *env = jnienv();
    jclass cls = rho_find_class(env, "com/rhomobile/rhodes/nativeview/RhoNativeViewManager");
    if (!cls) return null;
    jmethodID mid = env->GetStaticMethodID( cls, "getWebViewObject", "(I)Landroid/webkit/WebView;");
    if (!mid) return null;
    return env->CallStaticObjectMethod(cls, mid, tab_index);
}



