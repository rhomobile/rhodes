#include "rhodes.h"
#include "Eventsource.h"

#include "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "Eventsource_impl"

#define EVENTSOURCE_FACTORY_CLASS "com.rho.eventsource.EventsourceFactory"

extern "C" void Init_Eventsource_API(void);

extern "C" void Init_Eventsource(void)
{
    RAWTRACE(__FUNCTION__);

    JNIEnv *env = jnienv();
    if(env)
    {
        jclass cls = rho_find_class(env, EVENTSOURCE_FACTORY_CLASS);
        if(!cls)
        {
            RAWLOG_ERROR1("Failed to find java class: %s", EVENTSOURCE_FACTORY_CLASS);
            return;
        }
        jmethodID midFactory = env->GetMethodID(cls, "<init>", "()V");
        if(!midFactory)
        {
            RAWLOG_ERROR1("Failed to get constructor for java class %s", EVENTSOURCE_FACTORY_CLASS);
            return;
        }
        jobject jFactory = env->NewObject(cls, midFactory);
        if(env->IsSameObject(jFactory, NULL))
        {
            RAWLOG_ERROR1("Failed to create %s instance", EVENTSOURCE_FACTORY_CLASS);
            return;
        }
        
        RAWTRACE("Initializing Java factory");

        rho::CEventsourceBase::setJavaFactory(env, jFactory);

        RAWTRACE("Deleting JNI reference");

        env->DeleteLocalRef(jFactory);

        RAWTRACE("Initializing API");

        Init_Eventsource_API();

        RAWTRACE("Init_Eventsource succeeded");
    }
    else
    {
        RAWLOG_ERROR("Failed to initialize Eventsource API: jnienv() is failed");
    }

}

extern "C" void Init_Eventsource_extension() {
    Init_Eventsource();
}
