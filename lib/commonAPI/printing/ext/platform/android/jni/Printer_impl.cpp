#include "rhodes.h"
#include "Printer.h"

#include "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "Printer_impl"

#define PRINTING_FACTORY_CLASS "com.rho.printer.PrinterFactory"

extern "C" void Init_Printer_API(void);

extern "C" void Init_Printing(void)
{
    RAWTRACE(__FUNCTION__);

    JNIEnv *env = jnienv();
    if(env)
    {
        jclass cls = rho_find_class(env, PRINTING_FACTORY_CLASS);
        if(!cls)
        {
            RAWLOG_ERROR1("Failed to find java class: %s", PRINTING_FACTORY_CLASS);
            return;
        }
        jmethodID midFactory = env->GetMethodID(cls, "<init>", "()V");
        if(!midFactory)
        {
            RAWLOG_ERROR1("Failed to get constructor for java class %s", PRINTING_FACTORY_CLASS);
            return;
        }
        jobject jFactory = env->NewObject(cls, midFactory);
        if(env->IsSameObject(jFactory, NULL))
        {
            RAWLOG_ERROR1("Failed to create %s instance", PRINTING_FACTORY_CLASS);
            return;
        }
        
        RAWTRACE("Initializing Java factory");

        rho::CPrinterBase::setJavaFactory(env, jFactory);

        RAWTRACE("Deleting JNI reference");

        env->DeleteLocalRef(jFactory);

        RAWTRACE("Initializing API");

        Init_Printer_API();

        RAWTRACE("Init_Printing succeeded");
    }
    else
    {
        RAWLOG_ERROR("Failed to initialize Printing API: jnienv() is failed");
    }

}

extern "C" void Init_Printing_extension() {
    Init_Printing();
}
