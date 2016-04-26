#include "rhodes.h"
#include "Battery.h"

#include "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "Battery_impl"

#define BATTERY_FACTORY_CLASS "com.rho.battery.BatteryFactory"

extern "C" void Init_Battery_API(void);

extern "C" void Init_Battery(void)
{
    RAWTRACE(__FUNCTION__);

    JNIEnv *env = jnienv();
    if(env)
    {
        jclass cls = rho_find_class(env, BATTERY_FACTORY_CLASS);
        if(!cls)
        {
            RAWLOG_ERROR1("Failed to find java class: %s", BATTERY_FACTORY_CLASS);
            return;
        }
        jmethodID midFactory = env->GetMethodID(cls, "<init>", "()V");
        if(!midFactory)
        {
            RAWLOG_ERROR1("Failed to get constructor for java class %s", BATTERY_FACTORY_CLASS);
            return;
        }
        jobject jFactory = env->NewObject(cls, midFactory);
        if(env->IsSameObject(jFactory, NULL))
        {
            RAWLOG_ERROR1("Failed to create %s instance", BATTERY_FACTORY_CLASS);
            return;
        }
        
        RAWTRACE("Initializing Java factory");

        rho::CBatteryBase::setJavaFactory(env, jFactory);

        RAWTRACE("Deleting JNI reference");

        env->DeleteLocalRef(jFactory);

        RAWTRACE("Initializing API");

        Init_Battery_API();

        RAWTRACE("Init_Battery succeeded");
    }
    else
    {
        RAWLOG_ERROR("Failed to initialize Battery API: jnienv() is failed");
    }

}

extern "C" void Init_Battery_extension() {
    Init_Battery();
}
