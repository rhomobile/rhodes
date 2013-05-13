#include "rhodes.h"
#include "NativeToolbar.h"

#include "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "NativeToolbar_impl"

#define NATIVETOOLBAR_FACTORY_CLASS "com.rho.nativetoolbar.NativeToolbarFactory"

extern "C" void Init_NativeToolbar_API(void);

extern "C" void Init_NativeToolbar(void)
{
    RAWTRACE(__FUNCTION__);

    JNIEnv *env = jnienv();
    if(env)
    {
        jclass cls = rho_find_class(env, NATIVETOOLBAR_FACTORY_CLASS);
        if(!cls)
        {
            RAWLOG_ERROR1("Failed to find java class: %s", NATIVETOOLBAR_FACTORY_CLASS);
            return;
        }
        jmethodID midFactory = env->GetMethodID(cls, "<init>", "()V");
        if(!midFactory)
        {
            RAWLOG_ERROR1("Failed to get constructor for java class %s", NATIVETOOLBAR_FACTORY_CLASS);
            return;
        }
        jobject jFactory = env->NewObject(cls, midFactory);
        if(env->IsSameObject(jFactory, NULL))
        {
            RAWLOG_ERROR1("Failed to create %s instance", NATIVETOOLBAR_FACTORY_CLASS);
            return;
        }
        
        RAWTRACE("Initializing Java factory");

        rho::CNativeToolbarBase::setJavaFactory(env, jFactory);

        RAWTRACE("Deleting JNI reference");

        env->DeleteLocalRef(jFactory);

        RAWTRACE("Initializing API");

        Init_NativeToolbar_API();

        RAWTRACE("Init_NativeToolbar succeeded");
    }
    else
    {
        RAWLOG_ERROR("Failed to initialize NativeToolbar API: jnienv() is failed");
    }
    RHODESAPP().getExtManager().requireRubyFile("RhoNativeToolbarApi");
}

