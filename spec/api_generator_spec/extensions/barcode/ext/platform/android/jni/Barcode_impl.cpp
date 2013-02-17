#include "rhodes.h"
#include "Barcode.h"

#include "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "Barcode_impl"

#define BARCODE_FACTORY_CLASS "com.motorolasolutions.rho.barcode.BarcodeFactory"

extern "C" void Init_Barcode_API(void);

extern "C" void Init_Barcode(void)
{
    RAWTRACE("Init_Barcode");

    JNIEnv *env = jnienv();
    if(env)
    {
        jclass cls = rho_find_class(env, BARCODE_FACTORY_CLASS);
        if(!cls)
        {
            RAWLOG_ERROR1("Failed to find java class: %s", BARCODE_FACTORY_CLASS);
            return;
        }
        jmethodID midFactory = env->GetMethodID(cls, "<init>", "()V");
        if(!midFactory)
        {
            RAWLOG_ERROR1("Failed to get constructor for java class %s", BARCODE_FACTORY_CLASS);
            return;
        }
        jobject jFactory = env->NewObject(cls, midFactory);
        if(env->IsSameObject(jFactory, NULL))
        {
            RAWLOG_ERROR1("Failed to create %s instance", BARCODE_FACTORY_CLASS);
            return;
        }

        rho::CBarcodeBase::setJavaFactory(env, jFactory);

        env->DeleteLocalRef(jFactory);

        Init_Barcode_API();

        RAWTRACE("Init_Barcode1 succeeded");
    }
    else
    {
        RAWLOG_ERROR("Failed to initialize Barcode1 API: jnienv() is failed");
    }

}

extern "C" void Init_Barcode_extension() {}
