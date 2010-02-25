#include "JNIRhodes.h"

#include "gapikey.h"

#include <common/rhoparams.h>

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "MapView"

RHO_GLOBAL void mapview_create(rho_param *p)
{
#ifdef GOOGLE_API_KEY
    jclass clsMapView = getJNIClass(RHODES_JAVA_CLASS_MAPVIEW);
    if (!clsMapView) return;
    jmethodID midCreate = getJNIClassStaticMethod(clsMapView, "create", "(Ljava/lang/String;Ljava/util/Map;)V");
    if (!midCreate) return;

    if (p->type != RHO_PARAM_HASH) {
        RAWLOG_ERROR("create: wrong input parameter (expect Hash)");
        return;
    }

    JNIEnv *env = jnienv();
    jobject paramsObj = RhoValueConverter(env).createObject(p);
    env->CallStaticVoidMethod(clsMapView, midCreate, env->NewStringUTF(GOOGLE_API_KEY), paramsObj);
#else
    RHO_NOT_IMPLEMENTED;
#endif
}

