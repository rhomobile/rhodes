#include "rhodes.h"
#include "<%= namecamelcase %>.h"

#include "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "<%= namecamelcase %>_impl"

#define <%= namefixed.upcase %>_FACTORY_CLASS "com.rho.<%= namefixed %>.<%= namecamelcase %>Factory"

extern "C" void Init_<%= namecamelcase %>_API(void);

extern "C" void Init_<%= namecamelcase %>(void)
{
    RAWTRACE(__FUNCTION__);

    JNIEnv *env = jnienv();
    if(env)
    {
        jclass cls = rho_find_class(env, <%= namefixed.upcase %>_FACTORY_CLASS);
        if(!cls)
        {
            RAWLOG_ERROR1("Failed to find java class: %s", <%= namefixed.upcase %>_FACTORY_CLASS);
            return;
        }
        jmethodID midFactory = env->GetMethodID(cls, "<init>", "()V");
        if(!midFactory)
        {
            RAWLOG_ERROR1("Failed to get constructor for java class %s", <%= namefixed.upcase %>_FACTORY_CLASS);
            return;
        }
        jobject jFactory = env->NewObject(cls, midFactory);
        if(env->IsSameObject(jFactory, NULL))
        {
            RAWLOG_ERROR1("Failed to create %s instance", <%= namefixed.upcase %>_FACTORY_CLASS);
            return;
        }
        
        RAWTRACE("Initializing Java factory");

        rho::C<%= namecamelcase %>Base::setJavaFactory(env, jFactory);

        RAWTRACE("Deleting JNI reference");

        env->DeleteLocalRef(jFactory);

        RAWTRACE("Initializing API");

        Init_<%= namecamelcase %>_API();

        RAWTRACE("Init_<%= namecamelcase %> succeeded");
    }
    else
    {
        RAWLOG_ERROR("Failed to initialize <%= namecamelcase %> API: jnienv() is failed");
    }

}

extern "C" void Init_<%= namecamelcase %>_extension() {
    Init_<%= namecamelcase %>();
}
