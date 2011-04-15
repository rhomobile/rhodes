#include "rhodes/JNIRhodes.h"

#include <common/rhoparams.h>
#include "common/RhodesApp.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "NativeBar"

RHO_GLOBAL void create_nativebar_inner(int bar_type, rho_param *p)
{
    JNIEnv *env = jnienv();
    jclass clsNativeBar = getJNIClass(RHODES_JAVA_CLASS_NATIVEBAR);
    if (!clsNativeBar) return;
    jmethodID midCreate = getJNIClassStaticMethod(env, clsNativeBar, "create", "(ILjava/lang/Object;)V");
    if (!midCreate) return;

    if (p->type != RHO_PARAM_ARRAY && p->type != RHO_PARAM_HASH) {
        RAWLOG_ERROR("Unexpected parameter type, should be Array or Hash");
        return;
    }
    // check for iPad SplitTabBar type -> redirect to TabBar
    if (bar_type == 3) {
        bar_type = 1;
    }

    jobject paramsObj = RhoValueConverter(env).createObject(p);
    env->CallStaticVoidMethod(clsNativeBar, midCreate, bar_type, paramsObj);
    env->DeleteLocalRef(paramsObj);
}

RHO_GLOBAL void remove_nativebar_inner()
{
    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_NATIVEBAR);
    if (!cls) return;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "remove", "()V");
    if (!mid) return;

    env->CallStaticVoidMethod(cls, mid);
}

RHO_GLOBAL void nativebar_switch_tab_inner(int index)
{
    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_NATIVEBAR);
    if (!cls) return;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "switchTab", "(I)V");
    if (!mid) return;

    env->CallStaticVoidMethod(cls, mid, index);
}

RHO_GLOBAL VALUE nativebar_started()
{
    JNIEnv *env = jnienv();
    VALUE nil = rho_ruby_get_NIL();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_NATIVEBAR);
    if (!cls) return nil;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "isStarted", "()Z");
    if (!mid) return nil;

    return rho_ruby_create_boolean(env->CallStaticBooleanMethod(cls, mid));
}

RHO_GLOBAL void create_nativebar(int bar_type, rho_param *p)
{
	RAWLOG_INFO("NativeBar.create() is DEPRECATED API ! Please use Rho::NativeToolbar.create() or Rho::NativeTabbar.create().");
	create_nativebar_inner(bar_type, p);
}

RHO_GLOBAL void remove_nativebar()
{
	RAWLOG_INFO("NativeBar.remove() is DEPRECATED API ! Please use Rho::NativeToolbar.remove() or Rho::NativeTabbar.remove().");
	remove_nativebar_inner();
}

RHO_GLOBAL void nativebar_switch_tab(int index)
{
	RAWLOG_INFO("NativeBar.switch_tab() is DEPRECATED API ! Please use Rho::NativeTabbar.switch_tab().");
	nativebar_switch_tab_inner(index);
}




RHO_GLOBAL void nativebar_set_tab_badge(int index,char *val) {
	RAWLOG_INFO("NativeBar.set_tab_badge() is DEPRECATED API ! Please use Rho::NativeTabbar.set_tab_badge().");
}

RHO_GLOBAL void create_native_toolbar(int bar_type, rho_param *p) {
	create_nativebar_inner( bar_type, p);
}

RHO_GLOBAL void remove_native_toolbar() {
	remove_nativebar_inner();
}

RHO_GLOBAL void create_native_tabbar(int bar_type, rho_param *p) {
	create_nativebar_inner( bar_type, p);
}

RHO_GLOBAL void remove_native_tabbar() {
	remove_nativebar_inner();
}

RHO_GLOBAL void native_tabbar_switch_tab(int index) {
	nativebar_switch_tab_inner(index);
}

RHO_GLOBAL void native_tabbar_set_tab_badge(int index,char *val) {
	RAWLOG_INFO("Rho::NativeTabbar.set_tab_badge() unsupported on Android !");
}

RHO_GLOBAL int native_tabbar_get_current_tab() {
    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_NATIVEBAR);
    if (!cls) return 0;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "activeTab", "()I");
    if (!mid) return 0;
    return env->CallStaticIntMethod(cls, mid);
}


//private static native void onTabBarChangeTabCallback(String callback_url, String tab_index);
RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_mainview_TabbedMainView_onTabBarChangeTabCallback
  (JNIEnv *env, jclass, jstring callback, jstring body)
{
        rho_net_request_with_data(rho_http_normalizeurl(rho_cast<std::string>(callback).c_str()), rho_cast<std::string>(body).c_str());

}
