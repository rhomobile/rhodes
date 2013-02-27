#pragma once

#include "common/RhoStd.h"
#include "logging/RhoLog.h"
#include "rhodes/JNIRhodes.h"
#include "MethodExecutorJni.h"

namespace rhoelements {
  class MethodResultJni;
}


namespace rho {

using rhoelements::MethodResultJni;
using rhoelements::MethodExecutorJni;

class CJavascriptVMBase : public MethodExecutorJni
{
protected:
    DEFINE_LOGCLASS;

    static const char* const FACTORY_SINGLETON_CLASS;
    static jclass s_clsFactorySingleton;
    static jmethodID s_midFactorySetInstance;
    static jmethodID s_midFactoryGetInstance;

    //IJavascriptVMFactory staff
    static const char* const IFACTORY_CLASS;
    static jclass s_clsIFactory;
    static jmethodID s_midGetApiSingleton;
    static jmethodID s_midGetApiObject;

    //JavascriptVMSingletonBase staff
    static const char* const SINGLETON_BASE_CLASS;
    static jclass s_clsSingletonBase;

    //JavascriptVMBase staff
    static const char* const OBJECT_BASE_CLASS;
    static jclass s_clsObjectBase;

    //Method tasks

    static const char* const EXECUTESCRIPT_TASK_CLASS;
    static jclass s_clsexecuteScriptTask;
    static jmethodID s_midexecuteScriptTask;


    static JNIEnv* jniInit(JNIEnv* env);
    static JNIEnv* jniInit();

    static jobject getFactory(JNIEnv* env);
    static jobject getSingleton(JNIEnv* env);

    rho::String m_id;
    jobject getObject(JNIEnv* env);
public:
    static void setJavaFactory(JNIEnv* env, jobject jFactory);


    CJavascriptVMBase(const rho::String& id)
        : MethodExecutorJni(), m_id(id)
        {}
    virtual ~CJavascriptVMBase() {}
};

template <typename T>
class CJavascriptVMProxy : public CJavascriptVMBase
{
public:
    CJavascriptVMProxy(const rho::String& id)
        : CJavascriptVMBase(id)
        {}
    virtual ~CJavascriptVMProxy() {}

    static
    void executeScript(const T& argsAdapter, MethodResultJni& result)
    {
        LOG(TRACE) + "executeScript";

        JNIEnv *env = jniInit();
        if (!env) {
            LOG(FATAL) + "JNI initialization failed";
            return;
        }

        RHO_ASSERT(argsAdapter.size() <= 3);

        jhobject jhObject = 
            getSingleton(env); 

        jholder< jstring > jhscript = (argsAdapter.size() <= 0) ?
            static_cast< jstring >(0) :
                rho_cast< jstring >(env, argsAdapter[0]);
        jhobject jhTask = env->NewObject(s_clsexecuteScriptTask, s_midexecuteScriptTask,
                    jhObject.get(), 
                    jhscript.get(),
                    static_cast<jobject>(result));

        run(env, jhTask.get(), result, true, true);
    }


};


}
