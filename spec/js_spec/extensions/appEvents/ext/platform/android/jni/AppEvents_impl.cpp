#include "rhodes.h"
#include "AppEvents.h"

#include "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "AppEvents_impl"

#define APPEVENTS_FACTORY_CLASS "com.rho.appevents.AppEventsFactory"

extern "C" void Init_AppEvents_API(void);

extern "C" void Init_AppEvents(void)
{
    RAWTRACE(__FUNCTION__);

    JNIEnv *env = jnienv();
    if(env)
    {
        jclass cls = rho_find_class(env, APPEVENTS_FACTORY_CLASS);
        if(!cls)
        {
            RAWLOG_ERROR1("Failed to find java class: %s", APPEVENTS_FACTORY_CLASS);
            return;
        }
        jmethodID midFactory = env->GetMethodID(cls, "<init>", "()V");
        if(!midFactory)
        {
            RAWLOG_ERROR1("Failed to get constructor for java class %s", APPEVENTS_FACTORY_CLASS);
            return;
        }
        jobject jFactory = env->NewObject(cls, midFactory);
        if(env->IsSameObject(jFactory, NULL))
        {
            RAWLOG_ERROR1("Failed to create %s instance", APPEVENTS_FACTORY_CLASS);
            return;
        }
        
        RAWTRACE("Initializing Java factory");

        rho::CAppEventsBase::setJavaFactory(env, jFactory);

        RAWTRACE("Deleting JNI reference");

        env->DeleteLocalRef(jFactory);

        RAWTRACE("Initializing API");

        Init_AppEvents_API();

        RAWTRACE("Init_AppEvents succeeded");
    }
    else
    {
        RAWLOG_ERROR("Failed to initialize AppEvents API: jnienv() is failed");
    }

}

extern "C" void Init_AppEvents_extension() {
    Init_AppEvents();
}
