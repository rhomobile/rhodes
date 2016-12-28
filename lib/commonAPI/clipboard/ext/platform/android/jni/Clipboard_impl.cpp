#include "rhodes.h"
#include "Clipboard.h"

#include "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "Clipboard_impl"

#define CLIPBOARD_FACTORY_CLASS "com.rho.clipboard.ClipboardFactory"

extern "C" void Init_Clipboard_API(void);

extern "C" void Init_Clipboard(void)
{
    RAWTRACE(__FUNCTION__);

    JNIEnv *env = jnienv();
    if(env)
    {
        jclass cls = rho_find_class(env, CLIPBOARD_FACTORY_CLASS);
        if(!cls)
        {
            RAWLOG_ERROR1("Failed to find java class: %s", CLIPBOARD_FACTORY_CLASS);
            return;
        }
        jmethodID midFactory = env->GetMethodID(cls, "<init>", "()V");
        if(!midFactory)
        {
            RAWLOG_ERROR1("Failed to get constructor for java class %s", CLIPBOARD_FACTORY_CLASS);
            return;
        }
        jobject jFactory = env->NewObject(cls, midFactory);
        if(env->IsSameObject(jFactory, NULL))
        {
            RAWLOG_ERROR1("Failed to create %s instance", CLIPBOARD_FACTORY_CLASS);
            return;
        }
        
        RAWTRACE("Initializing Java factory");

        rho::CClipboardBase::setJavaFactory(env, jFactory);

        RAWTRACE("Deleting JNI reference");

        env->DeleteLocalRef(jFactory);

        RAWTRACE("Initializing API");

        Init_Clipboard_API();

        RAWTRACE("Init_Clipboard succeeded");
    }
    else
    {
        RAWLOG_ERROR("Failed to initialize Clipboard API: jnienv() is failed");
    }

}

extern "C" void Init_Clipboard_extension() {
    Init_Clipboard();
}
