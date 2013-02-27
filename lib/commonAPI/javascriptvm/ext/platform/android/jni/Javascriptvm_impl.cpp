#include "rhodes.h"
#include "JavascriptVM.h"

#include "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "JavascriptVM_impl"

#define JAVASCRIPTVM_FACTORY_CLASS "com.rho.javascriptvm.JavascriptVMFactory"

extern "C" void Init_JavascriptVM_API(void);

extern "C" void Init_JavascriptVM(void)
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

        rho::CJavascriptVMBase::setJavaFactory(env, jFactory);

        RAWTRACE("Deleting JNI reference");

        env->DeleteLocalRef(jFactory);

        RAWTRACE("Initializing API");

        Init_JavascriptVM_API();

        RAWTRACE("Init_JavascriptVM succeeded");
    }
    else
    {
        RAWLOG_ERROR("Failed to initialize JavascriptVM API: jnienv() is failed");
    }

}

extern "C" void Init_JavascriptVM_extension() {
    Init_JavascriptVM();
}
