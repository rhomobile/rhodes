#include "rhodes.h"
#include "Mediaplayer.h"

#include "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "Mediaplayer_impl"

#define MEDIAPLAYER_FACTORY_CLASS "com.rho.mediaplayer.MediaplayerFactory"

extern "C" void Init_Mediaplayer_API(void);

extern "C" void Init_Mediaplayer(void)
{
    RAWTRACE(__FUNCTION__);

    JNIEnv *env = jnienv();
    if(env)
    {
        jclass cls = rho_find_class(env, MEDIAPLAYER_FACTORY_CLASS);
        if(!cls)
        {
            RAWLOG_ERROR1("Failed to find java class: %s", MEDIAPLAYER_FACTORY_CLASS);
            return;
        }
        jmethodID midFactory = env->GetMethodID(cls, "<init>", "()V");
        if(!midFactory)
        {
            RAWLOG_ERROR1("Failed to get constructor for java class %s", MEDIAPLAYER_FACTORY_CLASS);
            return;
        }
        jobject jFactory = env->NewObject(cls, midFactory);
        if(env->IsSameObject(jFactory, NULL))
        {
            RAWLOG_ERROR1("Failed to create %s instance", MEDIAPLAYER_FACTORY_CLASS);
            return;
        }
        
        RAWTRACE("Initializing Java factory");

        rho::CMediaplayerBase::setJavaFactory(env, jFactory);

        RAWTRACE("Deleting JNI reference");

        env->DeleteLocalRef(jFactory);

        RAWTRACE("Initializing API");

        Init_Mediaplayer_API();

        RAWTRACE("Init_Mediaplayer succeeded");
    }
    else
    {
        RAWLOG_ERROR("Failed to initialize Mediaplayer API: jnienv() is failed");
    }

}

extern "C" void Init_Mediaplayer_extension() {
    Init_Mediaplayer();
}
