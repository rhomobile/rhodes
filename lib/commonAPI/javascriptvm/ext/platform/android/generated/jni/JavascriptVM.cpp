#include "JavascriptVM.h"

#include "MethodResultJni.h"


namespace rho {

IMPLEMENT_LOGCLASS(CJavascriptVMBase, "JavascriptVMJNI");

using rhoelements::MethodExecutorJni;

//JavascriptVMFactorySingleton staff
const char* const CJavascriptVMBase::FACTORY_SINGLETON_CLASS = "com.rho.javascriptvm.JavascriptVMFactorySingleton";
jclass CJavascriptVMBase::s_clsFactorySingleton = 0;
jmethodID CJavascriptVMBase::s_midFactorySetInstance;
jmethodID CJavascriptVMBase::s_midFactoryGetInstance;

//JavascriptVMFactory staff
const char* const CJavascriptVMBase::IFACTORY_CLASS = "com.rho.javascriptvm.IJavascriptVMFactory";
jclass CJavascriptVMBase::s_clsIFactory = 0;
jmethodID CJavascriptVMBase::s_midGetApiSingleton;
jmethodID CJavascriptVMBase::s_midGetApiObject;

//JavascriptVMSingletonBase staff
const char* const CJavascriptVMBase::SINGLETON_BASE_CLASS = "com.rho.javascriptvm.JavascriptVMSingletonBase";
jclass CJavascriptVMBase::s_clsSingletonBase = 0;

//JavascriptVMBase staff
const char* const CJavascriptVMBase::OBJECT_BASE_CLASS = "com.rho.javascriptvm.JavascriptVMBase";
jclass CJavascriptVMBase::s_clsObjectBase = 0;



//Method tasks

const char* const CJavascriptVMBase::EXECUTESCRIPT_TASK_CLASS = 
        "com.rho.javascriptvm.JavascriptVMSingletonBase$executeScriptTask";

jclass CJavascriptVMBase::s_clsexecuteScriptTask = 0;
jmethodID CJavascriptVMBase::s_midexecuteScriptTask;


//----------------------------------------------------------------------------------------------------------------------

JNIEnv* CJavascriptVMBase::jniInit()
{
    JNIEnv *env = jnienv();
    if(!env)
    {
        LOG(FATAL) + "JNI init failed: JNIEnv is null";
        return 0;
    }
    return jniInit(env);
}
//----------------------------------------------------------------------------------------------------------------------

JNIEnv* CJavascriptVMBase::jniInit(JNIEnv* env)
{
    static bool initialized = false;
    env = MethodExecutorJni::jniInit(env);
    if (!env) {
        LOG(FATAL) + "JNI init failed";
        return 0;
    }

    if(!initialized)
    {
        //init JavascriptVMFactorySingleton JNI
        s_clsFactorySingleton = loadClass(env, FACTORY_SINGLETON_CLASS);
        if (!s_clsFactorySingleton) return 0;

        s_midFactorySetInstance = env->GetStaticMethodID(s_clsFactorySingleton, "setInstance", "(Lcom/rho/javascriptvm/IJavascriptVMFactory;)V");
        if(!s_midFactorySetInstance)
        {
            LOG(FATAL) + "Failed to get method 'setInstance' for java class " + FACTORY_SINGLETON_CLASS;
            return NULL;
        }
        s_midFactoryGetInstance = env->GetStaticMethodID(s_clsFactorySingleton, "getInstance", "()Lcom/rho/javascriptvm/IJavascriptVMFactory;");
        if(!s_midFactoryGetInstance)
        {
            LOG(FATAL) + "Failed to get method 'getInstance' for java class " + FACTORY_SINGLETON_CLASS;
            return NULL;
        }

        //init IJavascriptVMFactory JNI
        s_clsIFactory = loadClass(env, IFACTORY_CLASS);
        if (!s_clsIFactory) return 0;
        s_midGetApiSingleton = env->GetMethodID(s_clsIFactory, "getApiSingleton", "()Lcom/rho/javascriptvm/IJavascriptVMSingleton;");
        if(!s_midGetApiSingleton)
        {
            LOG(FATAL) + "Failed to get method 'getApiSingleton' for java class " + IFACTORY_CLASS;
            return NULL;
        }
        s_midGetApiObject = env->GetMethodID(s_clsIFactory, "getApiObject", "(Ljava/lang/String;)Lcom/rho/javascriptvm/IJavascriptVM;");
        if(!s_midGetApiObject)
        {
            LOG(FATAL) + "Failed to get method 'getApiObject' for java class " + IFACTORY_CLASS;
            return NULL;
        }

        s_clsSingletonBase = loadClass(env, SINGLETON_BASE_CLASS);
        if (!s_clsSingletonBase) return 0;
        s_clsObjectBase = loadClass(env, OBJECT_BASE_CLASS);
        if (!s_clsObjectBase) return 0;



        s_clsexecuteScriptTask = loadClass(env, EXECUTESCRIPT_TASK_CLASS);
        if (!s_clsexecuteScriptTask) return 0;
        s_midexecuteScriptTask = env->GetMethodID(s_clsexecuteScriptTask, "<init>",
                        "(Lcom/rho/javascriptvm/IJavascriptVM;Ljava/lang/String;Lcom/motorolasolutions/rhoelements/IMethodResult;)V");
        if(!s_midexecuteScriptTask)
        {
            LOG(FATAL) + "Failed to get constructor for java class " + EXECUTESCRIPT_TASK_CLASS;
            return NULL;
        }



        initialized = true;
        LOG(TRACE) + "CJavascriptVM JNI init succeeded";
    }
    return env;
}
//----------------------------------------------------------------------------------------------------------------------

void CJavascriptVMBase::setJavaFactory(JNIEnv* env, jobject jFactory)
{
    LOG(TRACE) + "setJavaFactory";

    env = jniInit(env);
    if (!env) {
        LOG(FATAL) + "JNI initialization failed";
        return;
    }

    env->CallStaticVoidMethod(s_clsFactorySingleton, s_midFactorySetInstance, jFactory);

    LOG(TRACE) + "setJavaFactory succeeded";
}
//----------------------------------------------------------------------------------------------------------------------

jobject CJavascriptVMBase::getFactory(JNIEnv* env)
{
    jobject res = env->CallStaticObjectMethod(s_clsFactorySingleton, s_midFactoryGetInstance);
    return res;
}
//----------------------------------------------------------------------------------------------------------------------

jobject CJavascriptVMBase::getSingleton(JNIEnv* env)
{
    jhobject jhFactory = getFactory(env);
    jobject res = env->CallObjectMethod(jhFactory.get(), s_midGetApiSingleton);
    return res;
}
//----------------------------------------------------------------------------------------------------------------------

jobject CJavascriptVMBase::getObject(JNIEnv* env)
{
    jhstring jhId = rho_cast<jstring>(env, m_id);
    jhobject jhFactory = getFactory(env);
    jobject res = env->CallObjectMethod(jhFactory.get(), s_midGetApiObject, jhId.get());
    return res;
}
//----------------------------------------------------------------------------------------------------------------------


}
