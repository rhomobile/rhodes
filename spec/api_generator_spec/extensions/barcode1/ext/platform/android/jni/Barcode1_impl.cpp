#include "rhodes.h"
#include "Barcode1.h"

#include "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "Barcode1_impl"

#define BARCODE1_FACTORY_CLASS "com.rho.rhoelements.barcode1.Barcode1Factory"

extern "C" void Init_Barcode1_API(void);

extern "C" void Init_Barcode1(void)
{
    RAWTRACE("Init_Barcode1");

    JNIEnv *env = jnienv();
    if(env)
    {
        jclass cls = rho_find_class(env, BARCODE1_FACTORY_CLASS);
        if(!cls)
        {
            RAWLOG_ERROR1("Failed to find java class: %s", BARCODE1_FACTORY_CLASS);
            return;
        }
        jmethodID midFactory = env->GetMethodID(cls, "<init>", "()V");
        if(!midFactory)
        {
            RAWLOG_ERROR1("Failed to get constructor for java class %s", BARCODE1_FACTORY_CLASS);
            return;
        }
        jobject jFactory = env->NewObject(cls, midFactory);
        if(env->IsSameObject(jFactory, NULL))
        {
            RAWLOG_ERROR1("Failed to create %s instance", BARCODE1_FACTORY_CLASS);
            return;
        }

        rhoelements::CBarcode1Base::setJavaFactory(env, jFactory);

        env->DeleteLocalRef(jFactory);

        Init_Barcode1_API();

        RAWTRACE("Init_Barcode1 succeeded");
    }
    else
    {
        RAWLOG_ERROR("Failed to initialize Barcode1 API: jnienv() is failed");
    }

}
