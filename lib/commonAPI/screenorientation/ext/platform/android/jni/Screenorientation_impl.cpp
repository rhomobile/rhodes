#include "rhodes.h"
#include "ScreenOrientation.h"

#include "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "Screenorientation_impl"

#define SCREENORIENTATION_FACTORY_CLASS "com.motorolasolutions.rho.screenorientation.ScreenOrientationFactory"

extern "C" void Init_ScreenOrientation_API(void);

extern "C" void Init_ScreenOrientation(void)
{
    RAWTRACE(__FUNCTION__);

    JNIEnv *env = jnienv();
    if(env)
    {
        jclass cls = rho_find_class(env, SCREENORIENTATION_FACTORY_CLASS);
        if(!cls)
        {
            RAWLOG_ERROR1("Failed to find java class: %s", SCREENORIENTATION_FACTORY_CLASS);
            return;
        }
        jmethodID midFactory = env->GetMethodID(cls, "<init>", "()V");
        if(!midFactory)
        {
            RAWLOG_ERROR1("Failed to get constructor for java class %s", SCREENORIENTATION_FACTORY_CLASS);
            return;
        }
        jobject jFactory = env->NewObject(cls, midFactory);
        if(env->IsSameObject(jFactory, NULL))
        {
            RAWLOG_ERROR1("Failed to create %s instance", SCREENORIENTATION_FACTORY_CLASS);
            return;
        }
        
        RAWTRACE("Initializing Java factory");

        rho::CScreenOrientationBase::setJavaFactory(env, jFactory);

        RAWTRACE("Deleting JNI reference");

        env->DeleteLocalRef(jFactory);

        RAWTRACE("Initializing API");

        Init_ScreenOrientation_API();

        RAWTRACE("Init_Screenorientation succeeded");
    }
    else
    {
        RAWLOG_ERROR("Failed to initialize Screenorientation API: jnienv() is failed");
    }

}

extern "C" void Init_ScreenOrientation_extension() {
    Init_ScreenOrientation();
}
