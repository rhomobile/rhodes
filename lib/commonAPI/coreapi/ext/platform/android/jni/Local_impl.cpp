#include "rhodes.h"
#include "Local.h"

#include "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "Local_impl"

#define LOCAL_FACTORY_CLASS "com.motorolasolutions.rho.push.local.LocalFactory"

extern "C" void Init_Local_API(void);

extern "C" void Init_Local(void)
{
    RAWTRACE(__FUNCTION__);

    JNIEnv *env = jnienv();
    if(env)
    {
        jclass cls = rho_find_class(env, LOCAL_FACTORY_CLASS);
        if(!cls)
        {
            RAWLOG_ERROR1("Failed to find java class: %s", LOCAL_FACTORY_CLASS);
            return;
        }
        jmethodID midFactory = env->GetMethodID(cls, "<init>", "()V");
        if(!midFactory)
        {
            RAWLOG_ERROR1("Failed to get constructor for java class %s", LOCAL_FACTORY_CLASS);
            return;
        }
        jobject jFactory = env->NewObject(cls, midFactory);
        if(env->IsSameObject(jFactory, NULL))
        {
            RAWLOG_ERROR1("Failed to create %s instance", LOCAL_FACTORY_CLASS);
            return;
        }
        
        RAWTRACE("Initializing Java factory");

        rho::push::CLocalBase::setJavaFactory(env, jFactory);

        RAWTRACE("Deleting JNI reference");

        env->DeleteLocalRef(jFactory);

        RAWTRACE("Initializing API");

        Init_Local_API();

        RAWTRACE("Init_Local succeeded");
    }
    else
    {
        RAWLOG_ERROR("Failed to initialize Local API: jnienv() is failed");
    }

}

extern "C" void Init_Local_extension() {
    Init_Local();
}
