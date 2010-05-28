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

