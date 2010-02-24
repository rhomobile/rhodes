#include "JNIRhodes.h"

#include "gapikey.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "MapView"

RHO_GLOBAL void mapview_create(int nparams, char** params, int nannotations, char** annotation)
{
#ifdef GOOGLE_API_KEY
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_MAPVIEW);
    if (!cls) return;
    jmethodID mid = getJNIClassStaticMethod(cls, "create", "(Ljava/lang/String;)V");
    if (!mid) return;

    JNIEnv *env = jnienv();
    env->CallStaticVoidMethod(cls, mid, env->NewStringUTF(GOOGLE_API_KEY));
#else
    RHO_NOT_IMPLEMENTED;
#endif
}

