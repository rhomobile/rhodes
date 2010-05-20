#include "rhodes/JNIRhodes.h"

#include <common/rhoparams.h>

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "NavBar"

RHO_GLOBAL void create_navbar(rho_param *p)
{
    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_NAVBAR);
    if (!cls) return;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "create", "(Ljava/util/Vector;)V");
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
