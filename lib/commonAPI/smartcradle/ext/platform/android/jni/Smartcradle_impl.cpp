#include "rhodes.h"
#include "Smartcradle.h"

#include "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "Smartcradle_impl"

#define SMARTCRADLE_FACTORY_CLASS "com.rho.smartcradle.SmartCradleFactory"

extern "C" void Init_SmartCradle_API(void);

extern "C" void Init_Smartcradle(void)
{
    RAWTRACE(__FUNCTION__);

    JNIEnv *env = jnienv();
    if(env)
    {
        jclass cls = rho_find_class(env, SMARTCRADLE_FACTORY_CLASS);
        if(!cls)
        {
            RAWLOG_ERROR1("Failed to find java class: %s", SMARTCRADLE_FACTORY_CLASS);
            return;
        }
        jmethodID midFactory = env->GetMethodID(cls, "<init>", "()V");
        if(!midFactory)
        {
            RAWLOG_ERROR1("Failed to get constructor for java class %s", SMARTCRADLE_FACTORY_CLASS);
            return;
        }
        jobject jFactory = env->NewObject(cls, midFactory);
        if(env->IsSameObject(jFactory, NULL))
        {
            RAWLOG_ERROR1("Failed to create %s instance", SMARTCRADLE_FACTORY_CLASS);
            return;
        }
        
        RAWTRACE("Initializing Java factory");

        rho::CSmartCradleBase::setJavaFactory(env, jFactory);

        RAWTRACE("Deleting JNI reference");

        env->DeleteLocalRef(jFactory);

        RAWTRACE("Initializing API");
		LOG(ERROR) + "Init_SmartCradle_API called";

        Init_SmartCradle_API();

        RAWTRACE("Init_Smartcradle succeeded");
    }
    else
    {
        RAWLOG_ERROR("Failed to initialize Smartcradle API: jnienv() is failed");
    }

}

extern "C" void Init_Smartcradle_extension() {
	LOG(ERROR) + "Init_Smartcradle_extension called";
    Init_Smartcradle();
}
