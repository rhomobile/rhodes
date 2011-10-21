/*------------------------------------------------------------------------
* (The MIT License)
*
* Copyright (c) 2008-2011 Rhomobile, Inc.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*
* http://rhomobile.com
*------------------------------------------------------------------------*/

#include "rhodes/JNIRhodes.h"
#include "rhodes/JNIRhoRuby.h"

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

    std::string normUrl = RHODESAPP().canonicalizeRhoUrl(url);
    jhstring objNormUrl = rho_cast<jhstring>(env, normUrl);
    env->CallStaticVoidMethod(cls, mid, objNormUrl.get(), index);
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

    jhstring objJs = rho_cast<jhstring>(env, js);
    env->CallStaticVoidMethod(cls, mid, objJs.get(), index);
    return "";
}

RHO_GLOBAL void rho_webview_full_screen_mode(int enable)
{
    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_RHODES_ACTIVITY);
    if (!cls) return;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "setFullscreen", "(I)V");
    if (!mid) return;
    env->CallStaticVoidMethod(cls, mid, enable);
}

RHO_GLOBAL void rho_webview_set_cookie(const char *url, const char *cookie)
{
    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_WEB_VIEW);
    if (!cls) return;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "setCookie", "(Ljava/lang/String;Ljava/lang/String;)V");
    if (!mid) return;

    jhstring urlObj = rho_cast<jhstring>(url);
    jhstring cookieObj = rho_cast<jhstring>(cookie);
    env->CallStaticVoidMethod(cls, mid, urlObj.get(), cookieObj.get());
}
