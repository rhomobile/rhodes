#include "JNIRhodes.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "GeoLocation"

RHO_GLOBAL double geo_latitude()
{
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_GEO_LOCATION);
    if (!cls) return 0;
    jmethodID mid = getJNIClassStaticMethod(cls, "getLatitude", "()D");
    if (!mid) return 0;
    return jnienv()->CallStaticDoubleMethod(cls, mid);
}

RHO_GLOBAL double geo_longitude()
{
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_GEO_LOCATION);
    if (!cls) return 0;
    jmethodID mid = getJNIClassStaticMethod(cls, "getLongitude", "()D");
    if (!mid) return 0;
    return jnienv()->CallStaticDoubleMethod(cls, mid);
}

RHO_GLOBAL int geo_known_position()
{
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_GEO_LOCATION);
    if (!cls) return 0;
    jmethodID mid = getJNIClassStaticMethod(cls, "isKnownPosition", "()Z");
    if (!mid) return 0;
    return jnienv()->CallStaticBooleanMethod(cls, mid);
}

