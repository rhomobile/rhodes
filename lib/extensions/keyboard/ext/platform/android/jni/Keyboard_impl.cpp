#include "rhodes.h"
#include "Keyboard.h"

#include "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "Keyboard_impl"

#define KEYBOARD_FACTORY_CLASS "com.rho.keyboard.KeyboardFactory"

extern "C" void Init_Keyboard_API(void);

extern "C" void Init_Keyboard(void)
{
    RAWTRACE(__FUNCTION__);

    JNIEnv *env = jnienv();
    if(env)
    {
        jclass cls = rho_find_class(env, KEYBOARD_FACTORY_CLASS);
        if(!cls)
        {
            RAWLOG_ERROR1("Failed to find java class: %s", KEYBOARD_FACTORY_CLASS);
            return;
        }
        jmethodID midFactory = env->GetMethodID(cls, "<init>", "()V");
        if(!midFactory)
        {
            RAWLOG_ERROR1("Failed to get constructor for java class %s", KEYBOARD_FACTORY_CLASS);
            return;
        }
        jobject jFactory = env->NewObject(cls, midFactory);
        if(env->IsSameObject(jFactory, NULL))
        {
            RAWLOG_ERROR1("Failed to create %s instance", KEYBOARD_FACTORY_CLASS);
            return;
        }
        
        RAWTRACE("Initializing Java factory");

        rho::CKeyboardBase::setJavaFactory(env, jFactory);

        RAWTRACE("Deleting JNI reference");

        env->DeleteLocalRef(jFactory);

        RAWTRACE("Initializing API");

        Init_Keyboard_API();

        RAWTRACE("Init_Keyboard succeeded");
    }
    else
    {
        RAWLOG_ERROR("Failed to initialize Keyboard API: jnienv() is failed");
    }

}

extern "C" void Init_Keyboard_extension() {
    Init_Keyboard();
}
