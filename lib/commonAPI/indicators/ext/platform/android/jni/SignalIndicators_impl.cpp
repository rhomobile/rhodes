#include "rhodes.h"
#include "SignalIndicators.h"

#include "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "SignalIndicators_impl"

#define SIGNALINDICATORS_FACTORY_CLASS "com.rho.signalindicators.SignalIndicatorsFactory"

extern "C" void Init_SignalIndicators_API(void);

extern "C" void Init_SignalIndicators(void)
{
    RAWTRACE(__FUNCTION__);

    JNIEnv *env = jnienv();
    if(env)
    {
        jclass cls = rho_find_class(env, SIGNALINDICATORS_FACTORY_CLASS);
        if(!cls)
        {
            RAWLOG_ERROR1("Failed to find java class: %s", SIGNALINDICATORS_FACTORY_CLASS);
            return;
        }
        jmethodID midFactory = env->GetMethodID(cls, "<init>", "()V");
        if(!midFactory)
        {
            RAWLOG_ERROR1("Failed to get constructor for java class %s", SIGNALINDICATORS_FACTORY_CLASS);
            return;
        }
        jobject jFactory = env->NewObject(cls, midFactory);
        if(env->IsSameObject(jFactory, NULL))
        {
            RAWLOG_ERROR1("Failed to create %s instance", SIGNALINDICATORS_FACTORY_CLASS);
            return;
        }
        
        RAWTRACE("Initializing Java factory");

        rho::CSignalIndicatorsBase::setJavaFactory(env, jFactory);

        RAWTRACE("Deleting JNI reference");

        env->DeleteLocalRef(jFactory);

        RAWTRACE("Initializing API");

        Init_SignalIndicators_API();

        RAWTRACE("Init_SignalIndicators succeeded");
    }
    else
    {
        RAWLOG_ERROR("Failed to initialize SignalIndicators API: jnienv() is failed");
    }

}

extern "C" void Init_SignalIndicators_extension() {
    Init_SignalIndicators();
}
