#include "rhodes.h"
#include "GenPropBag.h"

#include "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "GenPropBag_impl"

#define GENPROPBAG_FACTORY_CLASS "com.rho.genpropbag.GenPropBagFactory"

extern "C" void Init_GenPropBag_API(void);

extern "C" void Init_GenPropBag(void)
{
    RAWTRACE(__FUNCTION__);

    JNIEnv *env = jnienv();
    if(env)
    {
        jclass cls = rho_find_class(env, GENPROPBAG_FACTORY_CLASS);
        if(!cls)
        {
            RAWLOG_ERROR1("Failed to find java class: %s", GENPROPBAG_FACTORY_CLASS);
            return;
        }
        jmethodID midFactory = env->GetMethodID(cls, "<init>", "()V");
        if(!midFactory)
        {
            RAWLOG_ERROR1("Failed to get constructor for java class %s", GENPROPBAG_FACTORY_CLASS);
            return;
        }
        jobject jFactory = env->NewObject(cls, midFactory);
        if(env->IsSameObject(jFactory, NULL))
        {
            RAWLOG_ERROR1("Failed to create %s instance", GENPROPBAG_FACTORY_CLASS);
            return;
        }
        
        RAWTRACE("Initializing Java factory");

        rho::CGenPropBagBase::setJavaFactory(env, jFactory);

        RAWTRACE("Deleting JNI reference");

        env->DeleteLocalRef(jFactory);

        RAWTRACE("Initializing API");

        Init_GenPropBag_API();

        RAWTRACE("Init_GenPropBag succeeded");
    }
    else
    {
        RAWLOG_ERROR("Failed to initialize GenPropBag API: jnienv() is failed");
    }

}

extern "C" void Init_GenPropBag_extension() {
    Init_GenPropBag();
}
