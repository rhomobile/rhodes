#include "rhodes.h"
#include "NativeTabbar.h"

#include "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "NativeTabbar_impl"

#define NATIVETABBAR_FACTORY_CLASS "com.rho.nativetabbar.NativeTabbarFactory"

extern "C" void Init_NativeTabbar_API(void);

extern "C" void Init_NativeTabbar(void)
{
    RAWTRACE(__FUNCTION__);

    JNIEnv *env = jnienv();
    if(env)
    {
        jclass cls = rho_find_class(env, NATIVETABBAR_FACTORY_CLASS);
        if(!cls)
        {
            RAWLOG_ERROR1("Failed to find java class: %s", NATIVETABBAR_FACTORY_CLASS);
            return;
        }
        jmethodID midFactory = env->GetMethodID(cls, "<init>", "()V");
        if(!midFactory)
        {
            RAWLOG_ERROR1("Failed to get constructor for java class %s", NATIVETABBAR_FACTORY_CLASS);
            return;
        }
        jobject jFactory = env->NewObject(cls, midFactory);
        if(env->IsSameObject(jFactory, NULL))
        {
            RAWLOG_ERROR1("Failed to create %s instance", NATIVETABBAR_FACTORY_CLASS);
            return;
        }
        
        RAWTRACE("Initializing Java factory");

        rho::CNativeTabbarBase::setJavaFactory(env, jFactory);

        RAWTRACE("Deleting JNI reference");

        env->DeleteLocalRef(jFactory);

        RAWTRACE("Initializing API");

        Init_NativeTabbar_API();

        RAWTRACE("Init_NativeTabbar succeeded");
    }
    else
    {
        RAWLOG_ERROR("Failed to initialize NativeTabbar API: jnienv() is failed");
    }
    RHODESAPP().getExtManager().requireRubyFile("RhoNativeTabbarApi");
}
