#pragma once

#include "common/RhoStd.h"
#include "logging/RhoLog.h"
#include "rhodes/JNIRhodes.h"
#include "MethodExecutorJni.h"

namespace rhoelements {
  class MethodResultJni;
}


namespace rho {
namespace examples {

using rhoelements::MethodResultJni;
using rhoelements::MethodExecutorJni;

class CSimpleOnlyStaticModuleBase : public MethodExecutorJni
{
protected:
    DEFINE_LOGCLASS;

    static const char* const FACTORY_SINGLETON_CLASS;
    static jclass s_clsFactorySingleton;
    static jmethodID s_midFactorySetInstance;
    static jmethodID s_midFactoryGetInstance;

    //ISimpleOnlyStaticModuleFactory staff
    static const char* const IFACTORY_CLASS;
    static jclass s_clsIFactory;
    static jmethodID s_midGetApiSingleton;
    static jmethodID s_midGetApiObject;

    //SimpleOnlyStaticModuleSingletonBase staff
    static const char* const SINGLETON_BASE_CLASS;
    static jclass s_clsSingletonBase;

    //SimpleOnlyStaticModuleBase staff
    static const char* const OBJECT_BASE_CLASS;
    static jclass s_clsObjectBase;

    //Method tasks

    static const char* const CALCSUMM_TASK_CLASS;
    static jclass s_clscalcSummTask;
    static jmethodID s_midcalcSummTask;
    static const char* const JOINSTRINGS_TASK_CLASS;
    static jclass s_clsjoinStringsTask;
    static jmethodID s_midjoinStringsTask;
    static const char* const GETPLATFORM_TASK_CLASS;
    static jclass s_clsgetPlatformTask;
    static jmethodID s_midgetPlatformTask;
    static const char* const SHOWALERTFROMUITHREAD_TASK_CLASS;
    static jclass s_clsshowAlertFromUIThreadTask;
    static jmethodID s_midshowAlertFromUIThreadTask;


    static JNIEnv* jniInit(JNIEnv* env);
    static JNIEnv* jniInit();

    static jobject getFactory(JNIEnv* env);
    static jobject getSingleton(JNIEnv* env);

    rho::String m_id;
    jobject getObject(JNIEnv* env);
public:
    static void setJavaFactory(JNIEnv* env, jobject jFactory);


    CSimpleOnlyStaticModuleBase(const rho::String& id)
        : MethodExecutorJni(), m_id(id)
        {}
    virtual ~CSimpleOnlyStaticModuleBase() {}
};

template <typename T>
class CSimpleOnlyStaticModuleProxy : public CSimpleOnlyStaticModuleBase
{
public:
    CSimpleOnlyStaticModuleProxy(const rho::String& id)
        : CSimpleOnlyStaticModuleBase(id)
        {}
    virtual ~CSimpleOnlyStaticModuleProxy() {}

    static
    void calcSumm(const T& argsAdapter, MethodResultJni& result)
    {
        LOG(TRACE) + "calcSumm";

        JNIEnv *env = jniInit();
        if (!env) {
            LOG(FATAL) + "JNI initialization failed";
            return;
        }

        RHO_ASSERT(argsAdapter.size() >= 2);
        RHO_ASSERT(argsAdapter.size() <= (2 + 2));

        jhobject jhObject = 
            getSingleton(env); 

        jholder< jint > jhx
            = rho_cast< jint >(env, argsAdapter[0]);

        jholder< jint > jhy
            = rho_cast< jint >(env, argsAdapter[1]);
        jhobject jhTask = env->NewObject(s_clscalcSummTask, s_midcalcSummTask,
                    jhObject.get(), 
                    jhx.get(),
                    jhy.get(),
                    static_cast<jobject>(result));

        run(env, jhTask.get(), result, true, true);
    }

    static
    void joinStrings(const T& argsAdapter, MethodResultJni& result)
    {
        LOG(TRACE) + "joinStrings";

        JNIEnv *env = jniInit();
        if (!env) {
            LOG(FATAL) + "JNI initialization failed";
            return;
        }

        RHO_ASSERT(argsAdapter.size() >= 1);
        RHO_ASSERT(argsAdapter.size() <= (1 + 2));

        jhobject jhObject = 
            getSingleton(env); 

        jholder< jobject > jhstrings
            = rho_cast< jobject >(env, argsAdapter[0]);
        jhobject jhTask = env->NewObject(s_clsjoinStringsTask, s_midjoinStringsTask,
                    jhObject.get(), 
                    jhstrings.get(),
                    static_cast<jobject>(result));

        run(env, jhTask.get(), result, true, true);
    }

    static
    void getPlatform(const T& argsAdapter, MethodResultJni& result)
    {
        LOG(TRACE) + "getPlatform";

        JNIEnv *env = jniInit();
        if (!env) {
            LOG(FATAL) + "JNI initialization failed";
            return;
        }

        RHO_ASSERT(argsAdapter.size() >= 0);
        RHO_ASSERT(argsAdapter.size() <= (0 + 2));

        jhobject jhObject = 
            getSingleton(env); 
        jhobject jhTask = env->NewObject(s_clsgetPlatformTask, s_midgetPlatformTask,
                    jhObject.get(), 
                    static_cast<jobject>(result));

        run(env, jhTask.get(), result, true, true);
    }

    static
    void showAlertFromUIThread(const T& argsAdapter, MethodResultJni& result)
    {
        LOG(TRACE) + "showAlertFromUIThread";

        JNIEnv *env = jniInit();
        if (!env) {
            LOG(FATAL) + "JNI initialization failed";
            return;
        }

        RHO_ASSERT(argsAdapter.size() >= 0);
        RHO_ASSERT(argsAdapter.size() <= (0 + 2));

        jhobject jhObject = 
            getSingleton(env); 
        jhobject jhTask = env->NewObject(s_clsshowAlertFromUIThreadTask, s_midshowAlertFromUIThreadTask,
                    jhObject.get(), 
                    static_cast<jobject>(result));

        run(env, jhTask.get(), result, true, false);
    }


};


}
}
