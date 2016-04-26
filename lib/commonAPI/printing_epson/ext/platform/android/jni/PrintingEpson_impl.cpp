#include "rhodes.h"
#include "PrintingEpson.h"

#include "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "PrintingEpson_impl"

#define PRINTINGEPSON_FACTORY_CLASS "com.rho.printingepson.PrintingEpsonFactory"

extern "C" void Init_PrintingEpson_API(void);

extern "C" void Init_PrintingEpson(void)
{
    RAWTRACE(__FUNCTION__);

    JNIEnv *env = jnienv();
    if(env)
    {
        jclass cls = rho_find_class(env, PRINTINGEPSON_FACTORY_CLASS);
        if(!cls)
        {
            RAWLOG_ERROR1("Failed to find java class: %s", PRINTINGEPSON_FACTORY_CLASS);
            return;
        }
        jmethodID midFactory = env->GetMethodID(cls, "<init>", "()V");
        if(!midFactory)
        {
            RAWLOG_ERROR1("Failed to get constructor for java class %s", PRINTINGEPSON_FACTORY_CLASS);
            return;
        }
        jobject jFactory = env->NewObject(cls, midFactory);
        if(env->IsSameObject(jFactory, NULL))
        {
            RAWLOG_ERROR1("Failed to create %s instance", PRINTINGEPSON_FACTORY_CLASS);
            return;
        }
        
        RAWTRACE("Initializing Java factory");

        rho::CPrintingEpsonBase::setJavaFactory(env, jFactory);

        RAWTRACE("Deleting JNI reference");

        env->DeleteLocalRef(jFactory);

        RAWTRACE("Initializing API");

        Init_PrintingEpson_API();

        RAWTRACE("Init_PrintingEpson succeeded");
    }
    else
    {
        RAWLOG_ERROR("Failed to initialize PrintingEpson API: jnienv() is failed");
    }

}

extern "C" void Init_PrintingEpson_extension() {
    Init_PrintingEpson();
}
