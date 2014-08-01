#include "rhodes.h"
#include "WebViewClientSSLAuth.h"

#include "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "WebViewClientSSLAuth_impl"

#define WEBVIEWCLIENTSSLAUTH_FACTORY_CLASS "com.rho.webviewclientsslauth.WebViewClientSSLAuthFactory"

extern "C" void Init_WebViewClientSSLAuth_API(void);

extern "C" void Init_WebViewClientSSLAuth(void)
{
    RAWTRACE(__FUNCTION__);

    JNIEnv *env = jnienv();
    if(env)
    {
        jclass cls = rho_find_class(env, WEBVIEWCLIENTSSLAUTH_FACTORY_CLASS);
        if(!cls)
        {
            RAWLOG_ERROR1("Failed to find java class: %s", WEBVIEWCLIENTSSLAUTH_FACTORY_CLASS);
            return;
        }
        jmethodID midFactory = env->GetMethodID(cls, "<init>", "()V");
        if(!midFactory)
        {
            RAWLOG_ERROR1("Failed to get constructor for java class %s", WEBVIEWCLIENTSSLAUTH_FACTORY_CLASS);
            return;
        }
        jobject jFactory = env->NewObject(cls, midFactory);
        if(env->IsSameObject(jFactory, NULL))
        {
            RAWLOG_ERROR1("Failed to create %s instance", WEBVIEWCLIENTSSLAUTH_FACTORY_CLASS);
            return;
        }
        
        RAWTRACE("Initializing Java factory");

        rho::CWebViewClientSSLAuthBase::setJavaFactory(env, jFactory);

        RAWTRACE("Deleting JNI reference");

        env->DeleteLocalRef(jFactory);

        RAWTRACE("Initializing API");

        Init_WebViewClientSSLAuth_API();

        RAWTRACE("Init_WebViewClientSSLAuth succeeded");
    }
    else
    {
        RAWLOG_ERROR("Failed to initialize WebViewClientSSLAuth API: jnienv() is failed");
    }

}

extern "C" void Init_WebViewClientSSLAuth_extension() {
    Init_WebViewClientSSLAuth();
}
