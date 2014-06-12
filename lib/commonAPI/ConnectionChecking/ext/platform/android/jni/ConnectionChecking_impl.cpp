#include "rhodes.h"
#include "ConnectionChecking.h"

#include "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "ConnectionChecking_impl"

#define CONNECTIONCHECKING_FACTORY_CLASS "com.rho.connectionchecking.ConnectionCheckingFactory"

extern "C" void Init_ConnectionChecking_API(void);

extern "C" void Init_ConnectionChecking(void)
{
    RAWTRACE(__FUNCTION__);

    JNIEnv *env = jnienv();
    if(env)
    {
        jclass cls = rho_find_class(env, CONNECTIONCHECKING_FACTORY_CLASS);
        if(!cls)
        {
            RAWLOG_ERROR1("Failed to find java class: %s", CONNECTIONCHECKING_FACTORY_CLASS);
            return;
        }
        jmethodID midFactory = env->GetMethodID(cls, "<init>", "()V");
        if(!midFactory)
        {
            RAWLOG_ERROR1("Failed to get constructor for java class %s", CONNECTIONCHECKING_FACTORY_CLASS);
            return;
        }
        jobject jFactory = env->NewObject(cls, midFactory);
        if(env->IsSameObject(jFactory, NULL))
        {
            RAWLOG_ERROR1("Failed to create %s instance", CONNECTIONCHECKING_FACTORY_CLASS);
            return;
        }
        
        RAWTRACE("Initializing Java factory");

        rho::CConnectionCheckingBase::setJavaFactory(env, jFactory);

        RAWTRACE("Deleting JNI reference");

        env->DeleteLocalRef(jFactory);

        RAWTRACE("Initializing API");

        Init_ConnectionChecking_API();

        RAWTRACE("Init_ConnectionChecking succeeded");
    }
    else
    {
        RAWLOG_ERROR("Failed to initialize ConnectionChecking API: jnienv() is failed");
    }

}

extern "C" void Init_ConnectionChecking_extension() {
    Init_ConnectionChecking();
}
