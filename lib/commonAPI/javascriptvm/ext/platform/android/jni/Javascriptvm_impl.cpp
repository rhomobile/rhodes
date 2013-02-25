#include "rhodes.h"
#include "Javascriptvm.h"

#include "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "Javascriptvm_impl"

#define JAVASCRIPTVM_FACTORY_CLASS "com.rho.javascriptvm.JavascriptvmFactory"

extern "C" void Init_Javascriptvm_API(void);

extern "C" void Init_Javascriptvm(void)
{
    RAWTRACE(__FUNCTION__);

    JNIEnv *env = jnienv();
    if(env)
    {
        jclass cls = rho_find_class(env, JAVASCRIPTVM_FACTORY_CLASS);
        if(!cls)
        {
            RAWLOG_ERROR1("Failed to find java class: %s", JAVASCRIPTVM_FACTORY_CLASS);
            return;
        }
        jmethodID midFactory = env->GetMethodID(cls, "<init>", "()V");
        if(!midFactory)
        {
            RAWLOG_ERROR1("Failed to get constructor for java class %s", JAVASCRIPTVM_FACTORY_CLASS);
            return;
        }
        jobject jFactory = env->NewObject(cls, midFactory);
        if(env->IsSameObject(jFactory, NULL))
        {
            RAWLOG_ERROR1("Failed to create %s instance", JAVASCRIPTVM_FACTORY_CLASS);
            return;
        }
        
        RAWTRACE("Initializing Java factory");

        rho::CJavascriptvmBase::setJavaFactory(env, jFactory);

        RAWTRACE("Deleting JNI reference");

        env->DeleteLocalRef(jFactory);

        RAWTRACE("Initializing API");

        Init_Javascriptvm_API();

        RAWTRACE("Init_Javascriptvm succeeded");
    }
    else
    {
        RAWLOG_ERROR("Failed to initialize Javascriptvm API: jnienv() is failed");
    }

}

extern "C" void Init_Javascriptvm_extension() {
    Init_Javascriptvm();
}
