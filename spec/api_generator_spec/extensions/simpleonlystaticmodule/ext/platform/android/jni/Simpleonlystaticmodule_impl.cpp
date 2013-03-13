#include "rhodes.h"
#include "Simpleonlystaticmodule.h"

#include "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "Simpleonlystaticmodule_impl"

#define SIMPLEONLYSTATICMODULE_FACTORY_CLASS "com.rho.examples.simpleonlystaticmodule.SimpleonlystaticmoduleFactory"

extern "C" void Init_SimpleOnlyStaticModule_API(void);

extern "C" void Init_SimpleOnlyStaticModule(void)
{
    RAWTRACE(__FUNCTION__);

    JNIEnv *env = jnienv();
    if(env)
    {
        RAWTRACE1("Finding java factory class: %s", SIMPLEONLYSTATICMODULE_FACTORY_CLASS);
        jclass cls = rho_find_class(env, SIMPLEONLYSTATICMODULE_FACTORY_CLASS);
        if(env->IsSameObject(cls, NULL))
        {
            RAWLOG_ERROR1("Failed to find java class: %s", SIMPLEONLYSTATICMODULE_FACTORY_CLASS);
            return;
        }
        RAWTRACE("Finding the java class constructor");
        jmethodID midFactory = env->GetMethodID(cls, "<init>", "()V");
        if(!midFactory)
        {
            RAWLOG_ERROR1("Failed to get constructor for java class %s", SIMPLEONLYSTATICMODULE_FACTORY_CLASS);
            return;
        }
        RAWTRACE("Creating java factory object");
        jobject jFactory = env->NewObject(cls, midFactory);
        RAWTRACE("Checking the java factory object");
        if(env->IsSameObject(jFactory, NULL))
        {
            RAWLOG_ERROR1("Failed to create %s instance", SIMPLEONLYSTATICMODULE_FACTORY_CLASS);
            return;
        }
        
        RAWTRACE("Initializing Java factory");

        rho::examples::CSimpleOnlyStaticModuleBase::setJavaFactory(env, jFactory);

        RAWTRACE("Deleting JNI reference");

        env->DeleteLocalRef(jFactory);

        RAWTRACE("Initializing API");

        Init_SimpleOnlyStaticModule_API();

        RAWTRACE("Init_Simpleonlystaticmodule succeeded");
    }
    else
    {
        RAWLOG_ERROR("Failed to initialize Simpleonlystaticmodule API: jnienv() is failed");
    }

}
