#include "rhodes.h"
#include "PrinterZebra.h"

#include "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "PrintingZebra_impl"

#define PRINTINGZEBRA_FACTORY_CLASS "com.rho.printerzebra.PrinterZebraFactory"

extern "C" void Init_PrinterZebra_API(void);

extern "C" void Init_PrintingZebra(void)
{
    RAWTRACE(__FUNCTION__);

    JNIEnv *env = jnienv();
    if(env)
    {
        jclass cls = rho_find_class(env, PRINTINGZEBRA_FACTORY_CLASS);
        if(!cls)
        {
            RAWLOG_ERROR1("Failed to find java class: %s", PRINTINGZEBRA_FACTORY_CLASS);
            return;
        }
        jmethodID midFactory = env->GetMethodID(cls, "<init>", "()V");
        if(!midFactory)
        {
            RAWLOG_ERROR1("Failed to get constructor for java class %s", PRINTINGZEBRA_FACTORY_CLASS);
            return;
        }
        jobject jFactory = env->NewObject(cls, midFactory);
        if(env->IsSameObject(jFactory, NULL))
        {
            RAWLOG_ERROR1("Failed to create %s instance", PRINTINGZEBRA_FACTORY_CLASS);
            return;
        }
        
        RAWTRACE("Initializing Java factory");

        rho::CPrinterZebraBase::setJavaFactory(env, jFactory);

        RAWTRACE("Deleting JNI reference");

        env->DeleteLocalRef(jFactory);

        RAWTRACE("Initializing API");

        Init_PrinterZebra_API();

        RAWTRACE("Init_PrintingZebra succeeded");
    }
    else
    {
        RAWLOG_ERROR("Failed to initialize PrintingZebra API: jnienv() is failed");
    }

}

extern "C" void Init_PrintingZebra_extension() {
    Init_PrintingZebra();
}
