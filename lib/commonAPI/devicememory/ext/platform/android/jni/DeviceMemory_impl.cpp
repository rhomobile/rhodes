#include "rhodes.h"
#include "DeviceMemory.h"

#include "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "DeviceMemory_impl"

#define DEVICEMEMORY_FACTORY_CLASS "com.rho.devicememory.DeviceMemoryFactory"

extern "C" void Init_DeviceMemory_API(void);

extern "C" void Init_DeviceMemory(void)
{
    RAWTRACE(__FUNCTION__);

    JNIEnv *env = jnienv();
    if(env)
    {
        jclass cls = rho_find_class(env, DEVICEMEMORY_FACTORY_CLASS);
        if(!cls)
        {
            RAWLOG_ERROR1("Failed to find java class: %s", DEVICEMEMORY_FACTORY_CLASS);
            return;
        }
        jmethodID midFactory = env->GetMethodID(cls, "<init>", "()V");
        if(!midFactory)
        {
            RAWLOG_ERROR1("Failed to get constructor for java class %s", DEVICEMEMORY_FACTORY_CLASS);
            return;
        }
        jobject jFactory = env->NewObject(cls, midFactory);
        if(env->IsSameObject(jFactory, NULL))
        {
            RAWLOG_ERROR1("Failed to create %s instance", DEVICEMEMORY_FACTORY_CLASS);
            return;
        }
        
        RAWTRACE("Initializing Java factory");

        rho::CDeviceMemoryBase::setJavaFactory(env, jFactory);

        RAWTRACE("Deleting JNI reference");

        env->DeleteLocalRef(jFactory);

        RAWTRACE("Initializing API");

        Init_DeviceMemory_API();

        RAWTRACE("Init_DeviceMemory succeeded");
    }
    else
    {
        RAWLOG_ERROR("Failed to initialize DeviceMemory API: jnienv() is failed");
    }

}

extern "C" void Init_DeviceMemory_extension() {
    Init_DeviceMemory();
}
