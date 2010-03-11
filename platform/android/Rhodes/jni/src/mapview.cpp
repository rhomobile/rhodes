#include "JNIRhodes.h"

#include "gapikey.h"

#include <common/rhoparams.h>

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "MapView"

extern "C" void alert_show_popup(char *);

RHO_GLOBAL void mapview_create(rho_param *p)
{
#ifdef GOOGLE_API_KEY
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
    env->CallStaticVoidMethod(clsMapView, midCreate, rho_cast<jstring>(GOOGLE_API_KEY), paramsObj);
#else
    const char *message = "MapView disabled";
    alert_show_popup((char*)message);
#endif
}

