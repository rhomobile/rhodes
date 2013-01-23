#include "rhodes/JNIRhodes.h"

#define BARCODE1_SINGLETON_CLASS "com.motorolasolutions.rhoelements.barcode1.Barcode1Singleton"

extern "C" void Init_RubyAPI_Barcode1(void);

extern "C" void Init_Barcode1(void)
{
    JNIEnv *env = jnienv();
    if(env)
    {
        jclass barcode1SingletonClass = rho_find_class(env, BARCODE1_SINGLETON_CLASS);
        if(!barcode1SingletonClass)
        {
            RAWLOG_ERROR1("Failed to find java class: %s", BARCODE1_SINGLETON_CLASS);
            return;
        }
        jmethodID midInit = env->GetStaticMethodID(barcode1SingletonClass, "init", "()V");
        if(!midInit)
        {
            RAWLOG_ERROR1("Failed to get method 'init' for java class %s", BARCODE1_SINGLETON_CLASS);
            return;
        }
        env->CallStaticVoidMethod(barcode1SingletonClass, midInit);

        Init_RubyAPI_Barcode1();
        //Init_JSAPI_Barcode1();
    }
    else
    {
        RAWLOG_ERROR("Failed to initialize Barcode1 API: jnienv() is failed");
    }

}
