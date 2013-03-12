#include "rhodes.h"
#include "Megamodule.h"

#include "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "Megamodule_impl"

#define MEGAMODULE_FACTORY_CLASS "com.rho.megamodule.MegamoduleFactory"

extern "C" void Init_Megamodule_API(void);

extern "C" void Init_Megamodule(void)
{
    RAWTRACE(__FUNCTION__);

    JNIEnv *env = jnienv();
    if(env)
    {
        jclass cls = rho_find_class(env, MEGAMODULE_FACTORY_CLASS);
        if(!cls)
        {
            RAWLOG_ERROR1("Failed to find java class: %s", MEGAMODULE_FACTORY_CLASS);
            return;
        }
        jmethodID midFactory = env->GetMethodID(cls, "<init>", "()V");
        if(!midFactory)
        {
            RAWLOG_ERROR1("Failed to get constructor for java class %s", MEGAMODULE_FACTORY_CLASS);
            return;
        }
        jobject jFactory = env->NewObject(cls, midFactory);
        if(env->IsSameObject(jFactory, NULL))
        {
            RAWLOG_ERROR1("Failed to create %s instance", MEGAMODULE_FACTORY_CLASS);
            return;
        }
        
        RAWTRACE("Initializing Java factory");

        rho::examples::CMegamoduleBase::setJavaFactory(env, jFactory);

        RAWTRACE("Deleting JNI reference");

        env->DeleteLocalRef(jFactory);

        RAWTRACE("Initializing API");

        Init_Megamodule_API();

        RAWTRACE("Init_Megamodule succeeded");
    }
    else
    {
        RAWLOG_ERROR("Failed to initialize Megamodule API: jnienv() is failed");
    }

}

extern "C" void Init_Megamodule_extension() {
    Init_Megamodule();
}
