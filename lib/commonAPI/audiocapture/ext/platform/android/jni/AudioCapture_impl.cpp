#include "rhodes.h"
#include "AudioCapture.h"

#include "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "AudioCapture_impl"

#define AUDIOCAPTURE_FACTORY_CLASS "com.rho.audiocapture.AudioCaptureFactory"

extern "C" void Init_AudioCapture_API(void);

extern "C" void Init_AudioCapture(void)
{
    RAWTRACE(__FUNCTION__);

    JNIEnv *env = jnienv();
    if(env)
    {
        jclass cls = rho_find_class(env, AUDIOCAPTURE_FACTORY_CLASS);
        if(!cls)
        {
            RAWLOG_ERROR1("Failed to find java class: %s", AUDIOCAPTURE_FACTORY_CLASS);
            return;
        }
        jmethodID midFactory = env->GetMethodID(cls, "<init>", "()V");
        if(!midFactory)
        {
            RAWLOG_ERROR1("Failed to get constructor for java class %s", AUDIOCAPTURE_FACTORY_CLASS);
            return;
        }
        jobject jFactory = env->NewObject(cls, midFactory);
        if(env->IsSameObject(jFactory, NULL))
        {
            RAWLOG_ERROR1("Failed to create %s instance", AUDIOCAPTURE_FACTORY_CLASS);
            return;
        }
        
        RAWTRACE("Initializing Java factory");

        rho::CAudioCaptureBase::setJavaFactory(env, jFactory);

        RAWTRACE("Deleting JNI reference");

        env->DeleteLocalRef(jFactory);

        RAWTRACE("Initializing API");

        Init_AudioCapture_API();

        RAWTRACE("Init_AudioCapture succeeded");
    }
    else
    {
        RAWLOG_ERROR("Failed to initialize AudioCapture API: jnienv() is failed");
    }

}

extern "C" void Init_AudioCapture_extension() {
    Init_AudioCapture();
}
