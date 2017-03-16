#include "rhodes.h"
#include "Jxcore.h"

#include "logging/RhoLog.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "Jxcore_impl"

#define JXCORE_FACTORY_CLASS "com.rho.jxcore.JxcoreFactory"

extern "C" void Init_Jxcore_API(void);

extern "C" void Init_Jxcore(void)
{
    RAWTRACE(__FUNCTION__);

    JNIEnv *env = jnienv();
    if(env)
    {
        jclass cls = rho_find_class(env, JXCORE_FACTORY_CLASS);
        if(!cls)
        {
            RAWLOG_ERROR1("Failed to find java class: %s", JXCORE_FACTORY_CLASS);
            return;
        }
        jmethodID midFactory = env->GetMethodID(cls, "<init>", "()V");
        if(!midFactory)
        {
            RAWLOG_ERROR1("Failed to get constructor for java class %s", JXCORE_FACTORY_CLASS);
            return;
        }
        jobject jFactory = env->NewObject(cls, midFactory);
        if(env->IsSameObject(jFactory, NULL))
        {
            RAWLOG_ERROR1("Failed to create %s instance", JXCORE_FACTORY_CLASS);
            return;
        }
        
        RAWTRACE("Initializing Java factory");

        rho::CJxcoreBase::setJavaFactory(env, jFactory);

        RAWTRACE("Deleting JNI reference");

        env->DeleteLocalRef(jFactory);

        RAWTRACE("Initializing API");

        Init_Jxcore_API();

        RAWTRACE("Init_Jxcore succeeded");
    }
    else
    {
        RAWLOG_ERROR("Failed to initialize Jxcore API: jnienv() is failed");
    }

}

namespace jxcore {
    extern int Initialize(JavaVM *vm);
}

extern "C" void Init_Jxcore_extension() {

    RAWTRACE1( ">>>>>>> Init_Jxcore_extension: before init JX, jvm: 0x%08x", jvm() );

    jxcore::Initialize(jvm());


    RAWTRACE( ">>>>>>> Init_Jxcore_extension: after init JX");

    Init_Jxcore();

}
