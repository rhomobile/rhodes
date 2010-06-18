#include "rhodes/JNIRhodes.h"

#include <common/rhoparams.h>

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "NavBar"

RHO_GLOBAL void create_navbar(rho_param *p)
{
    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_NAVBAR);
    if (!cls) return;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "create", "(Ljava/util/Map;)V");
    if (!mid) return;

    jobject paramsObj = RhoValueConverter(env).createObject(p);
    env->CallStaticVoidMethod(cls, mid, paramsObj);
    env->DeleteLocalRef(paramsObj);
}

RHO_GLOBAL void remove_navbar()
{
    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_NAVBAR);
    if (!cls) return;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "remove", "()V");
    if (!mid) return;

    env->CallStaticVoidMethod(cls, mid);
}

RHO_GLOBAL VALUE navbar_started()
{
    JNIEnv *env = jnienv();
    VALUE nil = rho_ruby_get_NIL();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_NAVBAR);
    if (!cls) return nil;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "isStarted", "()Z");
    if (!mid) return nil;

    return rho_ruby_create_boolean(env->CallStaticBooleanMethod(cls, mid));
}
