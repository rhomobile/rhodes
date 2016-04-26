#include "rhodes.h"
#include "KeyCapture.h"

#include "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "KeyCapture_impl"

#define KEYCAPTURE_FACTORY_CLASS "com.rho.keycapture.KeyCaptureFactory"

extern "C" void Init_KeyCapture_API(void);

extern "C" void Init_KeyCapture(void)
{
    RAWTRACE(__FUNCTION__);

    JNIEnv *env = jnienv();
    if(env)
    {
        jclass cls = rho_find_class(env, KEYCAPTURE_FACTORY_CLASS);
        if(!cls)
        {
            RAWLOG_ERROR1("Failed to find java class: %s", KEYCAPTURE_FACTORY_CLASS);
            return;
        }
        jmethodID midFactory = env->GetMethodID(cls, "<init>", "()V");
        if(!midFactory)
        {
            RAWLOG_ERROR1("Failed to get constructor for java class %s", KEYCAPTURE_FACTORY_CLASS);
            return;
        }
        jobject jFactory = env->NewObject(cls, midFactory);
        if(env->IsSameObject(jFactory, NULL))
        {
            RAWLOG_ERROR1("Failed to create %s instance", KEYCAPTURE_FACTORY_CLASS);
            return;
        }
        
        RAWTRACE("Initializing Java factory");

        rho::CKeyCaptureBase::setJavaFactory(env, jFactory);

        RAWTRACE("Deleting JNI reference");

        env->DeleteLocalRef(jFactory);

        RAWTRACE("Initializing API");

        Init_KeyCapture_API();

        RAWTRACE("Init_KeyCapture succeeded");
    }
    else
    {
        RAWLOG_ERROR("Failed to initialize KeyCapture API: jnienv() is failed");
    }

}

extern "C" void Init_HardwareKeys_extension() {
    Init_KeyCapture();
}
