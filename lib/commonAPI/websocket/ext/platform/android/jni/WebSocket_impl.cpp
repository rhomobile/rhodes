#include "rhodes.h"
#include "WebSocket.h"

#include "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "WebSocket_impl"

#define WEBSOCKET_FACTORY_CLASS "com.websocket.WebSocketFactory"

extern "C" void Init_WebSocket_API(void);

extern "C" void Init_WebSocket(void)
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

        CWebSocketBase::setJavaFactory(env, jFactory);

        RAWTRACE("Deleting JNI reference");

        env->DeleteLocalRef(jFactory);

        RAWTRACE("Initializing API");

        Init_WebSocket_API();

        RAWTRACE("Init_WebSocket succeeded");
    }
    else
    {
        RAWLOG_ERROR("Failed to initialize WebSocket API: jnienv() is failed");
    }

}

extern "C" void Init_WebSocket_extension() {
    Init_WebSocket();
}
