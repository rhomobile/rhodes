#include "rhodes.h"
#include "Sensors.h"

#include "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "Sensors_impl"

#define SENSORS_FACTORY_CLASS "com.motorolasolutions.rho.sensors.SensorsFactory"

extern "C" void Init_Sensors_API(void);

extern "C" void Init_Sensors(void)
{
    RAWTRACE(__FUNCTION__);

    JNIEnv *env = jnienv();
    if(env)
    {
        jclass cls = rho_find_class(env, SENSORS_FACTORY_CLASS);
        if(!cls)
        {
            RAWLOG_ERROR1("Failed to find java class: %s", SENSORS_FACTORY_CLASS);
            return;
        }
        jmethodID midFactory = env->GetMethodID(cls, "<init>", "()V");
        if(!midFactory)
        {
            RAWLOG_ERROR1("Failed to get constructor for java class %s", SENSORS_FACTORY_CLASS);
            return;
        }
        jobject jFactory = env->NewObject(cls, midFactory);
        if(env->IsSameObject(jFactory, NULL))
        {
            RAWLOG_ERROR1("Failed to create %s instance", SENSORS_FACTORY_CLASS);
            return;
        }
        
        RAWTRACE("Initializing Java factory");

        rho::CSensorsBase::setJavaFactory(env, jFactory);

        RAWTRACE("Deleting JNI reference");

        env->DeleteLocalRef(jFactory);

        RAWTRACE("Initializing API");

        Init_Sensors_API();

        RAWTRACE("Init_Sensors succeeded");
    }
    else
    {
        RAWLOG_ERROR("Failed to initialize Sensors API: jnienv() is failed");
    }

}

extern "C" void Init_Sensors_extension() {
    Init_Sensors();
}
