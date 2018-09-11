#include "rhodes.h"

#include "BluetoothConnection.h"

#include "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "BluetoothConnection_impl"

#define BLUETOOTHCONNECTION_FACTORY_CLASS "com.rho.bluetoothconnection.BluetoothConnectionFactory"

extern "C" void Init_BluetoothConnection_API(void);

extern "C" void Init_BluetoothConnection(void)
{
    RAWTRACE(__FUNCTION__);

    JNIEnv *env = jnienv();
    if(env)
    {
        jclass cls = rho_find_class(env, BLUETOOTHCONNECTION_FACTORY_CLASS);
        if(!cls)
        {
            RAWLOG_ERROR1("Failed to find java class: %s", BLUETOOTHCONNECTION_FACTORY_CLASS);
            return;
        }
        jmethodID midFactory = env->GetMethodID(cls, "<init>", "()V");
        if(!midFactory)
        {
            RAWLOG_ERROR1("Failed to get constructor for java class %s", BLUETOOTHCONNECTION_FACTORY_CLASS);
            return;
        }
        jobject jFactory = env->NewObject(cls, midFactory);
        if(env->IsSameObject(jFactory, NULL))
        {
            RAWLOG_ERROR1("Failed to create %s instance", BLUETOOTHCONNECTION_FACTORY_CLASS);
            return;
        }
        
        RAWTRACE("Initializing Java factory");

        rho::CBluetoothConnectionBase::setJavaFactory(env, jFactory);

        RAWTRACE("Deleting JNI reference");

        env->DeleteLocalRef(jFactory);

        RAWTRACE("Initializing API");

        Init_BluetoothConnection_API();

        RAWTRACE("Init_BluetoothConnection succeeded");
    }
    else
    {
        RAWLOG_ERROR("Failed to initialize BluetoothConnection API: jnienv() is failed");
    }

}

extern "C" void Init_Bluetooth_extension() {
    Init_BluetoothConnection();
}
