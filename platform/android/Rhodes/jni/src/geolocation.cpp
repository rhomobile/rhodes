#include "JNIRhodes.h"

#include <com_rhomobile_rhodes_geolocation_GeoLocationImpl.h>

#include "rubyext/GeoLocation.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "GeoLocation"

JNIEXPORT void JNICALL Java_com_rhomobile_rhodes_geolocation_GeoLocationImpl_geoCallback
  (JNIEnv *, jobject)
{
    RAWTRACE("Call geo callback");
    rho_geo_callcallback();
}

RHO_GLOBAL double rho_geo_latitude()
{
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_GEO_LOCATION);
    if (!cls) return 0;
    jmethodID mid = getJNIClassStaticMethod(cls, "getLatitude", "()D");
    if (!mid) return 0;
    return jnienv()->CallStaticDoubleMethod(cls, mid);
}

RHO_GLOBAL double rho_geo_longitude()
{
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_GEO_LOCATION);
    if (!cls) return 0;
    jmethodID mid = getJNIClassStaticMethod(cls, "getLongitude", "()D");
    if (!mid) return 0;
    return jnienv()->CallStaticDoubleMethod(cls, mid);
}

RHO_GLOBAL int rho_geo_known_position()
{
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_GEO_LOCATION);
    if (!cls) return 0;
    jmethodID mid = getJNIClassStaticMethod(cls, "isKnownPosition", "()Z");
    if (!mid) return 0;
    return jnienv()->CallStaticBooleanMethod(cls, mid);
}

RHO_GLOBAL void rho_geoimpl_settimeout(int nTimeoutSec)
{
}

RHO_GLOBAL int rho_geo_is_available()
{
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_GEO_LOCATION);
    if (!cls) return 0;
    jmethodID mid = getJNIClassStaticMethod(cls, "isAvailable", "()Z");
    if (!mid) return 0;

    return jnienv()->CallStaticBooleanMethod(cls, mid);
}
