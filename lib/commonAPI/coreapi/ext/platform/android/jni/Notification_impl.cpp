#include "rhodes.h"
#include "Notification.h"

#include "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "Notification_impl"

#define NOTIFICATION_FACTORY_CLASS "com.motorolasolutions.rho.notification.NotificationFactory"

extern "C" void Init_Notification_API(void);

extern "C" void Init_Notification(void)
{
    RAWTRACE(__FUNCTION__);

    JNIEnv *env = jnienv();
    if(env)
    {
        jclass cls = rho_find_class(env, NOTIFICATION_FACTORY_CLASS);
        if(!cls)
        {
            RAWLOG_ERROR1("Failed to find java class: %s", NOTIFICATION_FACTORY_CLASS);
            return;
        }
        jmethodID midFactory = env->GetMethodID(cls, "<init>", "()V");
        if(!midFactory)
        {
            RAWLOG_ERROR1("Failed to get constructor for java class %s", NOTIFICATION_FACTORY_CLASS);
            return;
        }
        jobject jFactory = env->NewObject(cls, midFactory);
        if(env->IsSameObject(jFactory, NULL))
        {
            RAWLOG_ERROR1("Failed to create %s instance", NOTIFICATION_FACTORY_CLASS);
            return;
        }
        
        RAWTRACE("Initializing Java factory");

        rho::CNotificationBase::setJavaFactory(env, jFactory);

        RAWTRACE("Deleting JNI reference");

        env->DeleteLocalRef(jFactory);

        RAWTRACE("Initializing API");

        Init_Notification_API();

        RAWTRACE("Init_Notification succeeded");
    }
    else
    {
        RAWLOG_ERROR("Failed to initialize Notification API: jnienv() is failed");
    }
    RHODESAPP().getExtManager().requireRubyFile("RhoNotificationApi");
}

extern "C" void Init_Notification_extension() {
    Init_Notification();
    RHODESAPP().getExtManager().requireRubyFile("RhoNotificationApi");
}
