#include "rhodes.h"
#include "Sensor.h"

#include "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "Sensor_impl"

#define SENSOR_FACTORY_CLASS "com.motorolasolutions.rho.sensor.SensorFactory"

extern "C" void Init_Sensor_API(void);

extern "C" void Init_Sensor(void)
{
    RAWTRACE(__FUNCTION__);

    JNIEnv *env = jnienv();
    if(env)
    {
        jclass cls = rho_find_class(env, SENSOR_FACTORY_CLASS);
        if(!cls)
        {
            RAWLOG_ERROR1("Failed to find java class: %s", SENSOR_FACTORY_CLASS);
            return;
        }
        jmethodID midFactory = env->GetMethodID(cls, "<init>", "()V");
        if(!midFactory)
        {
            RAWLOG_ERROR1("Failed to get constructor for java class %s", SENSOR_FACTORY_CLASS);
            return;
        }
        jobject jFactory = env->NewObject(cls, midFactory);
        if(env->IsSameObject(jFactory, NULL))
        {
            RAWLOG_ERROR1("Failed to create %s instance", SENSOR_FACTORY_CLASS);
            return;
        }
        
        RAWTRACE("Initializing Java factory");

        rho::CSensorBase::setJavaFactory(env, jFactory);

        RAWTRACE("Deleting JNI reference");

        env->DeleteLocalRef(jFactory);

        RAWTRACE("Initializing API");

        Init_Sensor_API();

        RAWTRACE("Init_Sensor succeeded");
    }
    else
    {
        RAWLOG_ERROR("Failed to initialize Sensor API: jnienv() is failed");
    }

}

extern "C" void Init_Sensor_extension() {
    Init_Sensor();
}
