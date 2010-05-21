#include "rhodes/JNIRhodes.h"

#include "rhodes/jni/com_rhomobile_rhodes_geolocation_GeoLocationImpl.h"

#include "rubyext/GeoLocation.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "GeoLocation"

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_geolocation_GeoLocationImpl_geoCallback
  (JNIEnv *, jobject)
{
    RAWTRACE("Call geo callback");
    rho_geo_callcallback();
}

RHO_GLOBAL double rho_geo_latitude()
{
    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_GEO_LOCATION);
    if (!cls) return 0;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "getLatitude", "()D");
    if (!mid) return 0;
    return env->CallStaticDoubleMethod(cls, mid);
}

RHO_GLOBAL double rho_geo_longitude()
{
    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_GEO_LOCATION);
    if (!cls) return 0;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "getLongitude", "()D");
    if (!mid) return 0;
    return env->CallStaticDoubleMethod(cls, mid);
}

RHO_GLOBAL int rho_geo_known_position()
{
    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_GEO_LOCATION);
    if (!cls) return 0;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "isKnownPosition", "()Z");
    if (!mid) return 0;
    return env->CallStaticBooleanMethod(cls, mid);
}

RHO_GLOBAL void rho_geoimpl_settimeout(int nTimeoutSec)
{
}

RHO_GLOBAL int rho_geo_is_available()
{
    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_GEO_LOCATION);
    if (!cls) return 0;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "isAvailable", "()Z");
    if (!mid) return 0;

    return env->CallStaticBooleanMethod(cls, mid);
}
