#include "rhodes.h"
#include "Websocket.h"

#include "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "Websocket_impl"

#define WEBSOCKET_FACTORY_CLASS "com.websocket.WebsocketFactory"

extern "C" void Init_Websocket_API(void);

extern "C" void Init_Websocket(void)
{
    RAWTRACE(__FUNCTION__);

    JNIEnv *env = jnienv();
    if(env)
    {
        jclass cls = rho_find_class(env, WEBSOCKET_FACTORY_CLASS);
        if(!cls)
        {
            RAWLOG_ERROR1("Failed to find java class: %s", WEBSOCKET_FACTORY_CLASS);
            return;
        }
        jmethodID midFactory = env->GetMethodID(cls, "<init>", "()V");
        if(!midFactory)
        {
            RAWLOG_ERROR1("Failed to get constructor for java class %s", WEBSOCKET_FACTORY_CLASS);
            return;
        }
        jobject jFactory = env->NewObject(cls, midFactory);
        if(env->IsSameObject(jFactory, NULL))
        {
            RAWLOG_ERROR1("Failed to create %s instance", WEBSOCKET_FACTORY_CLASS);
            return;
        }
        
        RAWTRACE("Initializing Java factory");

        CWebsocketBase::setJavaFactory(env, jFactory);

        RAWTRACE("Deleting JNI reference");

        env->DeleteLocalRef(jFactory);

        RAWTRACE("Initializing API");

        Init_Websocket_API();

        RAWTRACE("Init_Websocket succeeded");
    }
    else
    {
        RAWLOG_ERROR("Failed to initialize Websocket API: jnienv() is failed");
    }

}

extern "C" void Init_Websocket_extension() {
    Init_Websocket();
}
