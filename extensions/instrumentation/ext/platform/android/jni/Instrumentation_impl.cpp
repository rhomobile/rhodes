#include "rhodes.h"
#include "Instrumentation.h"

#include "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "Instrumentation_impl"

#define INSTRUMENTATION_FACTORY_CLASS "com.rho.instrumentation.InstrumentationFactory"

extern "C" void Init_Instrumentation_API(void);

extern "C" void Init_Instrumentation(void)
{
    RAWTRACE(__FUNCTION__);

    JNIEnv *env = jnienv();
    if(env)
    {
        jclass cls = rho_find_class(env, INSTRUMENTATION_FACTORY_CLASS);
        if(!cls)
        {
            RAWLOG_ERROR1("Failed to find java class: %s", INSTRUMENTATION_FACTORY_CLASS);
            return;
        }
        jmethodID midFactory = env->GetMethodID(cls, "<init>", "()V");
        if(!midFactory)
        {
            RAWLOG_ERROR1("Failed to get constructor for java class %s", INSTRUMENTATION_FACTORY_CLASS);
            return;
        }
        jobject jFactory = env->NewObject(cls, midFactory);
        if(env->IsSameObject(jFactory, NULL))
        {
            RAWLOG_ERROR1("Failed to create %s instance", INSTRUMENTATION_FACTORY_CLASS);
            return;
        }
        
        RAWTRACE("Initializing Java factory");

        rho::CInstrumentationBase::setJavaFactory(env, jFactory);

        RAWTRACE("Deleting JNI reference");

        env->DeleteLocalRef(jFactory);

        RAWTRACE("Initializing API");

        Init_Instrumentation_API();

        RAWTRACE("Init_Instrumentation succeeded");
    }
    else
    {
        RAWLOG_ERROR("Failed to initialize Instrumentation API: jnienv() is failed");
    }

}

extern "C" void Init_Instrumentation_extension() {
    Init_Instrumentation();
}
