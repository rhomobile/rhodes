#include "rhodes.h"
#include "Signature.h"

#include "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "Signature_impl"

#ifndef RHO_NO_RUBY_API
extern "C" unsigned long rb_require(const char *fname);
extern "C" int  rho_ruby_is_started();
#endif



#define SIGNATURE_FACTORY_CLASS "com.rho.signature.SignatureFactory"

extern "C" void Init_Signature_API(void);

extern "C" void Init_Signature(void)
{
    RAWTRACE(__FUNCTION__);

    JNIEnv *env = jnienv();
    if(env)
    {
        jclass cls = rho_find_class(env, SIGNATURE_FACTORY_CLASS);
        if(!cls)
        {
            RAWLOG_ERROR1("Failed to find java class: %s", SIGNATURE_FACTORY_CLASS);
            return;
        }
        jmethodID midFactory = env->GetMethodID(cls, "<init>", "()V");
        if(!midFactory)
        {
            RAWLOG_ERROR1("Failed to get constructor for java class %s", SIGNATURE_FACTORY_CLASS);
            return;
        }
        jobject jFactory = env->NewObject(cls, midFactory);
        if(env->IsSameObject(jFactory, NULL))
        {
            RAWLOG_ERROR1("Failed to create %s instance", SIGNATURE_FACTORY_CLASS);
            return;
        }
        
        RAWTRACE("Initializing Java factory");

        rho::CSignatureBase::setJavaFactory(env, jFactory);

        RAWTRACE("Deleting JNI reference");

        env->DeleteLocalRef(jFactory);

        RAWTRACE("Initializing API");

        Init_Signature_API();

        RAWTRACE("Init_Signature succeeded");
    }
    else
    {
        RAWLOG_ERROR("Failed to initialize Signature API: jnienv() is failed");
    }

}

extern "C" void Init_Signature_extension() {
    Init_Signature();
#ifndef RHO_NO_RUBY_API
    if (rho_ruby_is_started())
    {
        rb_require("RhoSignatureApi");
    }
#endif
}
