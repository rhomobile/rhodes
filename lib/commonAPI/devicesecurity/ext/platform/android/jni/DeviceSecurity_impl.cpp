#include "rhodes.h"
#include "DeviceSecurity.h"

#include "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "DeviceSecurity_impl"

#define DEVICESECURITY_FACTORY_CLASS "com.rho.devicesecurity.DeviceSecurityFactory"

extern "C" void Init_DeviceSecurity_API(void);

extern "C" void Init_DeviceSecurity(void)
{
    RAWTRACE(__FUNCTION__);

    JNIEnv *env = jnienv();
    if(env)
    {
        jclass cls = rho_find_class(env, DEVICESECURITY_FACTORY_CLASS);
        if(!cls)
        {
            RAWLOG_ERROR1("Failed to find java class: %s", DEVICESECURITY_FACTORY_CLASS);
            return;
        }
        jmethodID midFactory = env->GetMethodID(cls, "<init>", "()V");
        if(!midFactory)
        {
            RAWLOG_ERROR1("Failed to get constructor for java class %s", DEVICESECURITY_FACTORY_CLASS);
            return;
        }
        jobject jFactory = env->NewObject(cls, midFactory);
        if(env->IsSameObject(jFactory, NULL))
        {
            RAWLOG_ERROR1("Failed to create %s instance", DEVICESECURITY_FACTORY_CLASS);
            return;
        }
        
        RAWTRACE("Initializing Java factory");

        rho::CDeviceSecurityBase::setJavaFactory(env, jFactory);

        RAWTRACE("Deleting JNI reference");

        env->DeleteLocalRef(jFactory);

        RAWTRACE("Initializing API");

        Init_DeviceSecurity_API();

        RAWTRACE("Init_DeviceSecurity succeeded");
    }
    else
    {
        RAWLOG_ERROR("Failed to initialize DeviceSecurity API: jnienv() is failed");
    }

}

extern "C" void Init_DeviceSecurity_extension() {
    Init_DeviceSecurity();
}
