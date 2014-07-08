#include "rhodes.h"
#include "Videocapture.h"

#include "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "Videocapture_impl"

#define VIDEOCAPTURE_FACTORY_CLASS "com.motorolasolutions.rho.videocapture.VideocaptureFactory"

extern "C" void Init_Videocapture_API(void);

extern "C" void Init_Videocapture(void)
{
    RAWTRACE(__FUNCTION__);

    JNIEnv *env = jnienv();
    if(env)
    {
        jclass cls = rho_find_class(env, VIDEOCAPTURE_FACTORY_CLASS);
        if(!cls)
        {
            RAWLOG_ERROR1("Failed to find java class: %s", VIDEOCAPTURE_FACTORY_CLASS);
            return;
        }
        jmethodID midFactory = env->GetMethodID(cls, "<init>", "()V");
        if(!midFactory)
        {
            RAWLOG_ERROR1("Failed to get constructor for java class %s", VIDEOCAPTURE_FACTORY_CLASS);
            return;
        }
        jobject jFactory = env->NewObject(cls, midFactory);
        if(env->IsSameObject(jFactory, NULL))
        {
            RAWLOG_ERROR1("Failed to create %s instance", VIDEOCAPTURE_FACTORY_CLASS);
            return;
        }
        
        RAWTRACE("Initializing Java factory");

        rho::CVideocaptureBase::setJavaFactory(env, jFactory);

        RAWTRACE("Deleting JNI reference");

        env->DeleteLocalRef(jFactory);

        RAWTRACE("Initializing API");

        Init_Videocapture_API();

        RAWTRACE("Init_Videocapture succeeded");
    }
    else
    {
        RAWLOG_ERROR("Failed to initialize Videocapture API: jnienv() is failed");
    }

}
