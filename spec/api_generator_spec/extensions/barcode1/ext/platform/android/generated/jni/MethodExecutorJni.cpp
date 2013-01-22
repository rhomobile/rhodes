
#include "MethodExecutorJni.h"
#include "MethodResultJni.h"

#include "rhodes/JNIRhodes.h"

#include "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "MethodExecutorJNI"

namespace rhoelements {

const char * const MethodExecutorJni::METHOD_EXECUTOR_CLASS = "com/motorolasolutions/rhoelements/MethodExecutor";

jclass MethodExecutorJni::s_MethodExecutorClass = 0;
jmethodID MethodExecutorJni::s_midRun;
jmethodID MethodExecutorJni::s_midRunWithSeparateThread;
jmethodID MethodExecutorJni::s_midRunWithUiThread;

//----------------------------------------------------------------------------------------------------------------------

JNIEnv* MethodExecutorJni::jniInit()
{
    JNIEnv *env = jnienv();
    if(env && !s_MethodExecutorClass)
    {
        jclass cls = rho_find_class(env, METHOD_EXECUTOR_CLASS);
        if(!cls)
        {
            RAWLOG_ERROR1("Failed to find java class: %s", METHOD_EXECUTOR_CLASS);
            return NULL;
        }
        s_MethodExecutorClass = static_cast<jclass>(env->NewGlobalRef(cls));
        env->DeleteLocalRef(cls);

        s_midRun = env->GetStaticMethodID(s_MethodExecutorClass, "run", "(Ljava/lang/Runnable;)V");
        if(!s_midRun)
        {
            RAWLOG_ERROR1("Failed to get method 'run' for java class %s", METHOD_EXECUTOR_CLASS);
            s_MethodExecutorClass = 0;
            return NULL;
        }
        s_midRunWithSeparateThread = env->GetStaticMethodID(s_MethodExecutorClass, "runWithSeparateThread", "(Ljava/lang/Runnable;)V");
        if(!s_midRunWithSeparateThread)
        {
            RAWLOG_ERROR1("Failed to get method 'runWithSeparateThread' for java class %s", METHOD_EXECUTOR_CLASS);
            s_MethodExecutorClass = 0;
            return NULL;
        }
        s_midRunWithUiThread = env->GetStaticMethodID(s_MethodExecutorClass, "runWithUiThread", "(Ljava/lang/Runnable;)V");
        if(!s_midRunWithUiThread)
        {
            RAWLOG_ERROR1("Failed to get method 'runWithUiThread' for java class %s", METHOD_EXECUTOR_CLASS);
            s_MethodExecutorClass = 0;
            return NULL;
        }
    }
    return env;
}
//----------------------------------------------------------------------------------------------------------------------

void MethodExecutorJni::run(jhobject jhTask)
{
    JNIEnv *env = jniInit();
    if (!env) {
        RAWLOG_ERROR("JNI initialization failed");
        return;
    }

    env->CallStaticVoidMethod(s_MethodExecutorClass, s_midRun, jhTask.get());
}
//----------------------------------------------------------------------------------------------------------------------

void MethodExecutorJni::runWithSeparateThread(jhobject jhTask)
{
    JNIEnv *env = jniInit();
    if (!env) {
        RAWLOG_ERROR("JNI initialization failed");
        return;
    }

    env->CallStaticVoidMethod(s_MethodExecutorClass, s_midRunWithSeparateThread, jhTask.get());
}
//----------------------------------------------------------------------------------------------------------------------

void MethodExecutorJni::runWithUiThread(jhobject jhTask)
{
    JNIEnv *env = jniInit();
    if (!env) {
        RAWLOG_ERROR("JNI initialization failed");
        return;
    }

    env->CallStaticVoidMethod(s_MethodExecutorClass, s_midRunWithUiThread, jhTask.get());
}
//----------------------------------------------------------------------------------------------------------------------

void MethodExecutorJni::run(jhobject jhTask, MethodResultJni& result)
{
    if(shouldRunWithUiThread())
    {
        runWithUiThread(jhTask);
    }
    else if(result.hasCallBackUrl() || shouldRunWithThread())
    {
        runWithSeparateThread(jhTask);
    }
    else
    {
        run(jhTask);
    }
}
//----------------------------------------------------------------------------------------------------------------------

}
