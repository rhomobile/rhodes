#include "rhodes/JNIRhodes.h"

#include <common/rhoparams.h>

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "MapView"

RHO_GLOBAL void mapview_create(rho_param *p)
{
#ifdef RHO_GOOGLE_API_KEY
    JNIEnv *env = jnienv();
    jclass clsMapView = getJNIClass(RHODES_JAVA_CLASS_MAPVIEW);
    if (!clsMapView) return;
    jmethodID midCreate = getJNIClassStaticMethod(env, clsMapView, "create", "(Ljava/lang/String;Ljava/util/Map;)V");
    if (!midCreate) return;

    if (p->type != RHO_PARAM_HASH) {
        RAWLOG_ERROR("create: wrong input parameter (expect Hash)");
        return;
    }

    jobject paramsObj = RhoValueConverter(env).createObject(p);
    jstring keyObj = rho_cast<jstring>(RHO_GOOGLE_API_KEY);
    env->CallStaticVoidMethod(clsMapView, midCreate, keyObj, paramsObj);
    env->DeleteLocalRef(keyObj);
    env->DeleteLocalRef(paramsObj);
#else
    RAWLOG_ERROR("MapView disabled at build time");
#endif
}

RHO_GLOBAL void mapview_close()
{
#ifdef RHO_GOOGLE_API_KEY
    JNIEnv *env = jnienv();
    jclass clsMapView = getJNIClass(RHODES_JAVA_CLASS_MAPVIEW);
    if (!clsMapView) return;
    jmethodID midClose = getJNIClassStaticMethod(env, clsMapView, "close", "()V");
    if (!midClose) return;

    env->CallStaticVoidMethod(clsMapView, midClose);
#endif
}

RHO_GLOBAL VALUE mapview_state_started()
{
#ifdef RHO_GOOGLE_API_KEY
    VALUE nil = rho_ruby_get_NIL();
    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_MAPVIEW);
    if (!cls) return nil;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "isStarted", "()Z");
    if (!mid) return nil;

    return rho_ruby_create_boolean(env->CallStaticBooleanMethod(cls, mid));
#else
    return rho_ruby_create_boolean(0);
#endif
}

RHO_GLOBAL double mapview_state_center_lat()
{
#ifdef RHO_GOOGLE_API_KEY
    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_MAPVIEW);
    if (!cls) return 0;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "getCenterLatitude", "()D");
    if (!mid) return 0;

    return env->CallStaticDoubleMethod(cls, mid);
#else
    return 0;
#endif
}

RHO_GLOBAL double mapview_state_center_lon()
{
#ifdef RHO_GOOGLE_API_KEY
    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_MAPVIEW);
    if (!cls) return 0;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "getCenterLongitude", "()D");
    if (!mid) return 0;

    return env->CallStaticDoubleMethod(cls, mid);
#else
    return 0;
#endif
}

