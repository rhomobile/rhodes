#include "rhodes.h"
#include "Coreapi.h"

#include "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "Coreapi_impl"

#define COREAPI_FACTORY_CLASS "com.rho.coreapi.CoreapiFactory"

extern "C" void Init_Coreapi_API(void);

extern "C" void Init_Coreapi(void)
{
    RAWTRACE(__FUNCTION__);

    JNIEnv *env = jnienv();
    if(env)
    {
        jclass cls = rho_find_class(env, COREAPI_FACTORY_CLASS);
        if(!cls)
        {
            RAWLOG_ERROR1("Failed to find java class: %s", COREAPI_FACTORY_CLASS);
            return;
        }
        jmethodID midFactory = env->GetMethodID(cls, "<init>", "()V");
        if(!midFactory)
        {
            RAWLOG_ERROR1("Failed to get constructor for java class %s", COREAPI_FACTORY_CLASS);
            return;
        }
        jobject jFactory = env->NewObject(cls, midFactory);
        if(env->IsSameObject(jFactory, NULL))
        {
            RAWLOG_ERROR1("Failed to create %s instance", COREAPI_FACTORY_CLASS);
            return;
        }
        
        RAWTRACE("Initializing Java factory");

        rho::CCoreapiBase::setJavaFactory(env, jFactory);

        RAWTRACE("Deleting JNI reference");

        env->DeleteLocalRef(jFactory);

        RAWTRACE("Initializing API");

        Init_Coreapi_API();

        RAWTRACE("Init_Coreapi succeeded");
    }
    else
    {
        RAWLOG_ERROR("Failed to initialize Coreapi API: jnienv() is failed");
    }

}

extern "C" void Init_Coreapi_extension() {
    Init_Coreapi();
}
