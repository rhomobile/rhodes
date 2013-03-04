#include "SimpleOnlyStaticModule.h"

#include "MethodResultJni.h"


namespace rho {
namespace examples {

IMPLEMENT_LOGCLASS(CSimpleOnlyStaticModuleBase, "SimpleOnlyStaticModuleJNI");

using rhoelements::MethodExecutorJni;

//SimpleOnlyStaticModuleFactorySingleton staff
const char* const CSimpleOnlyStaticModuleBase::FACTORY_SINGLETON_CLASS = "com.rho.examples.simpleonlystaticmodule.SimpleOnlyStaticModuleFactorySingleton";
jclass CSimpleOnlyStaticModuleBase::s_clsFactorySingleton = 0;
jmethodID CSimpleOnlyStaticModuleBase::s_midFactorySetInstance;
jmethodID CSimpleOnlyStaticModuleBase::s_midFactoryGetInstance;

//SimpleOnlyStaticModuleFactory staff
const char* const CSimpleOnlyStaticModuleBase::IFACTORY_CLASS = "com.rho.examples.simpleonlystaticmodule.ISimpleOnlyStaticModuleFactory";
jclass CSimpleOnlyStaticModuleBase::s_clsIFactory = 0;
jmethodID CSimpleOnlyStaticModuleBase::s_midGetApiSingleton;
jmethodID CSimpleOnlyStaticModuleBase::s_midGetApiObject;

//SimpleOnlyStaticModuleSingletonBase staff
const char* const CSimpleOnlyStaticModuleBase::SINGLETON_BASE_CLASS = "com.rho.examples.simpleonlystaticmodule.SimpleOnlyStaticModuleSingletonBase";
jclass CSimpleOnlyStaticModuleBase::s_clsSingletonBase = 0;

//SimpleOnlyStaticModuleBase staff
const char* const CSimpleOnlyStaticModuleBase::OBJECT_BASE_CLASS = "com.rho.examples.simpleonlystaticmodule.SimpleOnlyStaticModuleBase";
jclass CSimpleOnlyStaticModuleBase::s_clsObjectBase = 0;



//Method tasks

const char* const CSimpleOnlyStaticModuleBase::CALCSUMM_TASK_CLASS = 
        "com.rho.examples.simpleonlystaticmodule.SimpleOnlyStaticModuleSingletonBase$calcSummTask";

jclass CSimpleOnlyStaticModuleBase::s_clscalcSummTask = 0;
jmethodID CSimpleOnlyStaticModuleBase::s_midcalcSummTask;

const char* const CSimpleOnlyStaticModuleBase::JOINSTRINGS_TASK_CLASS = 
        "com.rho.examples.simpleonlystaticmodule.SimpleOnlyStaticModuleSingletonBase$joinStringsTask";

jclass CSimpleOnlyStaticModuleBase::s_clsjoinStringsTask = 0;
jmethodID CSimpleOnlyStaticModuleBase::s_midjoinStringsTask;

const char* const CSimpleOnlyStaticModuleBase::GETPLATFORM_TASK_CLASS = 
        "com.rho.examples.simpleonlystaticmodule.SimpleOnlyStaticModuleSingletonBase$getPlatformTask";

jclass CSimpleOnlyStaticModuleBase::s_clsgetPlatformTask = 0;
jmethodID CSimpleOnlyStaticModuleBase::s_midgetPlatformTask;

const char* const CSimpleOnlyStaticModuleBase::SHOWALERTFROMUITHREAD_TASK_CLASS = 
        "com.rho.examples.simpleonlystaticmodule.SimpleOnlyStaticModuleSingletonBase$showAlertFromUIThreadTask";

jclass CSimpleOnlyStaticModuleBase::s_clsshowAlertFromUIThreadTask = 0;
jmethodID CSimpleOnlyStaticModuleBase::s_midshowAlertFromUIThreadTask;


//----------------------------------------------------------------------------------------------------------------------

JNIEnv* CSimpleOnlyStaticModuleBase::jniInit()
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

JNIEnv* CSimpleOnlyStaticModuleBase::jniInit(JNIEnv* env)
{
    static bool initialized = false;
    env = MethodExecutorJni::jniInit(env);
    if (!env) {
        LOG(FATAL) + "JNI init failed";
        return 0;
    }

    if(!initialized)
    {
        //init SimpleOnlyStaticModuleFactorySingleton JNI
        s_clsFactorySingleton = loadClass(env, FACTORY_SINGLETON_CLASS);
        if (!s_clsFactorySingleton) return 0;

        s_midFactorySetInstance = env->GetStaticMethodID(s_clsFactorySingleton, "setInstance", "(Lcom/rho/examples/simpleonlystaticmodule/ISimpleOnlyStaticModuleFactory;)V");
        if(!s_midFactorySetInstance)
        {
            LOG(FATAL) + "Failed to get method 'setInstance' for java class " + FACTORY_SINGLETON_CLASS;
            return NULL;
        }
        s_midFactoryGetInstance = env->GetStaticMethodID(s_clsFactorySingleton, "getInstance", "()Lcom/rho/examples/simpleonlystaticmodule/ISimpleOnlyStaticModuleFactory;");
        if(!s_midFactoryGetInstance)
        {
            LOG(FATAL) + "Failed to get method 'getInstance' for java class " + FACTORY_SINGLETON_CLASS;
            return NULL;
        }

        //init ISimpleOnlyStaticModuleFactory JNI
        s_clsIFactory = loadClass(env, IFACTORY_CLASS);
        if (!s_clsIFactory) return 0;
        s_midGetApiSingleton = env->GetMethodID(s_clsIFactory, "getApiSingleton", "()Lcom/rho/examples/simpleonlystaticmodule/ISimpleOnlyStaticModuleSingleton;");
        if(!s_midGetApiSingleton)
        {
            LOG(FATAL) + "Failed to get method 'getApiSingleton' for java class " + IFACTORY_CLASS;
            return NULL;
        }
        s_midGetApiObject = env->GetMethodID(s_clsIFactory, "getApiObject", "(Ljava/lang/String;)Lcom/rho/examples/simpleonlystaticmodule/ISimpleOnlyStaticModule;");
        if(!s_midGetApiObject)
        {
            LOG(FATAL) + "Failed to get method 'getApiObject' for java class " + IFACTORY_CLASS;
            return NULL;
        }

        s_clsSingletonBase = loadClass(env, SINGLETON_BASE_CLASS);
        if (!s_clsSingletonBase) return 0;
        s_clsObjectBase = loadClass(env, OBJECT_BASE_CLASS);
        if (!s_clsObjectBase) return 0;



        s_clscalcSummTask = loadClass(env, CALCSUMM_TASK_CLASS);
        if (!s_clscalcSummTask) return 0;
        s_midcalcSummTask = env->GetMethodID(s_clscalcSummTask, "<init>",
                        "(Lcom/rho/examples/simpleonlystaticmodule/ISimpleOnlyStaticModuleSingleton;IILcom/rhomobile/rhodes/api/IMethodResult;)V");
        if(!s_midcalcSummTask)
        {
            LOG(FATAL) + "Failed to get constructor for java class " + CALCSUMM_TASK_CLASS;
            return NULL;
        }

        s_clsjoinStringsTask = loadClass(env, JOINSTRINGS_TASK_CLASS);
        if (!s_clsjoinStringsTask) return 0;
        s_midjoinStringsTask = env->GetMethodID(s_clsjoinStringsTask, "<init>",
                        "(Lcom/rho/examples/simpleonlystaticmodule/ISimpleOnlyStaticModuleSingleton;Ljava/util/List;Lcom/rhomobile/rhodes/api/IMethodResult;)V");
        if(!s_midjoinStringsTask)
        {
            LOG(FATAL) + "Failed to get constructor for java class " + JOINSTRINGS_TASK_CLASS;
            return NULL;
        }

        s_clsgetPlatformTask = loadClass(env, GETPLATFORM_TASK_CLASS);
        if (!s_clsgetPlatformTask) return 0;
        s_midgetPlatformTask = env->GetMethodID(s_clsgetPlatformTask, "<init>",
                        "(Lcom/rho/examples/simpleonlystaticmodule/ISimpleOnlyStaticModuleSingleton;Lcom/rhomobile/rhodes/api/IMethodResult;)V");
        if(!s_midgetPlatformTask)
        {
            LOG(FATAL) + "Failed to get constructor for java class " + GETPLATFORM_TASK_CLASS;
            return NULL;
        }

        s_clsshowAlertFromUIThreadTask = loadClass(env, SHOWALERTFROMUITHREAD_TASK_CLASS);
        if (!s_clsshowAlertFromUIThreadTask) return 0;
        s_midshowAlertFromUIThreadTask = env->GetMethodID(s_clsshowAlertFromUIThreadTask, "<init>",
                        "(Lcom/rho/examples/simpleonlystaticmodule/ISimpleOnlyStaticModuleSingleton;Lcom/rhomobile/rhodes/api/IMethodResult;)V");
        if(!s_midshowAlertFromUIThreadTask)
        {
            LOG(FATAL) + "Failed to get constructor for java class " + SHOWALERTFROMUITHREAD_TASK_CLASS;
            return NULL;
        }



        initialized = true;
        LOG(TRACE) + "CSimpleOnlyStaticModule JNI init succeeded";
    }
    return env;
}
//----------------------------------------------------------------------------------------------------------------------

void CSimpleOnlyStaticModuleBase::setJavaFactory(JNIEnv* env, jobject jFactory)
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

jobject CSimpleOnlyStaticModuleBase::getFactory(JNIEnv* env)
{
    jobject res = env->CallStaticObjectMethod(s_clsFactorySingleton, s_midFactoryGetInstance);
    return res;
}
//----------------------------------------------------------------------------------------------------------------------

jobject CSimpleOnlyStaticModuleBase::getSingleton(JNIEnv* env)
{
    jhobject jhFactory = getFactory(env);
    jobject res = env->CallObjectMethod(jhFactory.get(), s_midGetApiSingleton);
    return res;
}
//----------------------------------------------------------------------------------------------------------------------

jobject CSimpleOnlyStaticModuleBase::getObject(JNIEnv* env)
{
    jhstring jhId = rho_cast<jstring>(env, m_id);
    jhobject jhFactory = getFactory(env);
    jobject res = env->CallObjectMethod(jhFactory.get(), s_midGetApiObject, jhId.get());
    return res;
}
//----------------------------------------------------------------------------------------------------------------------


}
}
