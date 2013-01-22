#include "Barcode1.h"

#include "MethodResultJni.h"

namespace rhoelements {

const char* const CBarcode1::SINGLETON_CLASS = "com/motorolasolutions/rhoelements/barcode1/Barcode1Sigleton";
const char* const CBarcode1::INSTANCE_CLASS = "com/motorolasolutions/rhoelements/IBarcode1";
const char* const CBarcode1::GETPROPS_TASK_CLASS = "com/motorolasolutions/rhoelements/Barcode1SingletonBase.GetPropsTask";
const char* const CBarcode1::GETPROPS1_TASK_CLASS = "com/motorolasolutions/rhoelements/Barcode1SingletonBase.GetProps1Task";
const char* const CBarcode1::GETPROPS2_TASK_CLASS = "com/motorolasolutions/rhoelements/Barcode1SingletonBase.GetProps1Task";

jclass CBarcode1::s_singletonClass = 0;
jclass CBarcode1::s_getPropsTaskClass = 0;
jclass CBarcode1::s_getProps1TaskClass = 0;
jclass CBarcode1::s_getProps2TaskClass = 0;

jmethodID CBarcode1::s_midGetInstance;
jmethodID CBarcode1::s_midEnumerate;
jmethodID CBarcode1::s_midGetDefaultID;
jmethodID CBarcode1::s_midSetDefaultID;
jmethodID CBarcode1::s_midGetApiObject;
jmethodID CBarcode1::s_midGetPropsTask;
jmethodID CBarcode1::s_midGetProps1Task;
jmethodID CBarcode1::s_midGetProps2Task;

#include "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "Barcode1JNI"

//----------------------------------------------------------------------------------------------------------------------

JNIEnv* CBarcode1::jniInit()
{
    JNIEnv *env = MethodExecutorJni::jniInit();
    if(env && !s_singletonClass)
    {
        jclass cls = rho_find_class(env, SINGLETON_CLASS);
        if(!cls)
        {
            RAWLOG_ERROR1("Failed to find java class: %s", SINGLETON_CLASS);
            s_singletonClass = 0;
            return NULL;
        }
        s_singletonClass = static_cast<jclass>(env->NewGlobalRef(cls));
        env->DeleteLocalRef(cls);

        s_midGetInstance = env->GetStaticMethodID(s_singletonClass, "getInstance", "(V)Lcom/motorolasolutions/rhoelements/barcode1/Barcode1Singleton;");
        if(!s_midGetInstance)
        {
            RAWLOG_ERROR1("Failed to get method 'getInstance' for java class %s", SINGLETON_CLASS);
            s_singletonClass = 0;
            return NULL;
        }
        s_midEnumerate = env->GetMethodID(s_singletonClass, "enumerate", "(Lcom/motorolasolutions/rhoelements/IMethodResult;)V");
        if(!s_midEnumerate)
        {
            RAWLOG_ERROR1("Failed to get method 'enumerate' for java class %s", SINGLETON_CLASS);
            s_singletonClass = 0;
            return NULL;
        }
        s_midGetDefaultID = env->GetMethodID(s_singletonClass, "getDefaultID", "()Ljava/lang/String;");
        if(!s_midGetDefaultID)
        {
            RAWLOG_ERROR1("Failed to get method 'getDefaultID' for java class %s", SINGLETON_CLASS);
            s_singletonClass = 0;
            return NULL;
        }
        s_midSetDefaultID = env->GetMethodID(s_singletonClass, "setDefaultID", "(Ljava/lang/String;)V");
        if(!s_midSetDefaultID)
        {
            RAWLOG_ERROR1("Failed to get method 'setDefaultID' for java class %s", SINGLETON_CLASS);
            s_singletonClass = 0;
            return NULL;
        }
        s_midGetApiObject = env->GetMethodID(s_singletonClass, "getApiObject", "(Ljava/lang/String;)Lcom/rhomobile/rhoelements/IBarcode1;");
        if(!s_midGetApiObject)
        {
            RAWLOG_ERROR1("Failed to get method 'getApiObject' for java class %s", SINGLETON_CLASS);
            s_singletonClass = 0;
            return NULL;
        }

        jclass clsTask = rho_find_class(env, GETPROPS_TASK_CLASS);
        if(!clsTask)
        {
            RAWLOG_ERROR1("Failed to find java class: %s", GETPROPS_TASK_CLASS);
            s_singletonClass = 0;
            return NULL;
        }
        s_getPropsTaskClass = static_cast<jclass>(env->NewGlobalRef(clsTask));
        env->DeleteLocalRef(clsTask);

        s_midGetPropsTask = env->GetMethodID(s_getPropsTaskClass, "<init>",
                        "(Lcom/rhomobile/rhoelements/IBarcode1;Lcom/rhomobile/rhoelements/IMethodResult;)V");
        if(!s_midGetPropsTask)
        {
            RAWLOG_ERROR1("Failed to get constructor for java class %s", GETPROPS_TASK_CLASS);
            s_singletonClass = 0;
            return NULL;
        }

        jclass clsTask1 = rho_find_class(env, GETPROPS1_TASK_CLASS);
        if(!clsTask1)
        {
            RAWLOG_ERROR1("Failed to find java class: %s", GETPROPS1_TASK_CLASS);
            s_singletonClass = 0;
            return NULL;
        }
        s_getProps1TaskClass = static_cast<jclass>(env->NewGlobalRef(clsTask1));
        env->DeleteLocalRef(clsTask1);

        s_midGetProps1Task = env->GetMethodID(s_getProps1TaskClass, "<init>",
                        "(Lcom/rhomobile/rhoelements/IBarcode1;Ljava/util/String;Lcom/rhomobile/rhoelements/IMethodResult;)V");
        if(!s_midGetProps1Task)
        {
            RAWLOG_ERROR1("Failed to get constructor for java class %s", GETPROPS1_TASK_CLASS);
            s_singletonClass = 0;
            return NULL;
        }

        jclass clsTask2 = rho_find_class(env, GETPROPS2_TASK_CLASS);
        if(!clsTask2)
        {
            RAWLOG_ERROR1("Failed to find java class: %s", GETPROPS2_TASK_CLASS);
            s_singletonClass = 0;
            return NULL;
        }
        s_getProps2TaskClass = static_cast<jclass>(env->NewGlobalRef(clsTask2));
        env->DeleteLocalRef(clsTask2);

        s_midGetProps2Task = env->GetMethodID(s_getProps2TaskClass, "<init>",
                        "(Lcom/rhomobile/rhoelements/IBarcode1;Ljava/util/List;Lcom/rhomobile/rhoelements/IMethodResult;)V");
        if(!s_midGetProps2Task)
        {
            RAWLOG_ERROR1("Failed to get constructor for java class %s", GETPROPS2_TASK_CLASS);
            s_singletonClass = 0;
            return NULL;
        }
    }
    return env;
}
//----------------------------------------------------------------------------------------------------------------------

void CBarcode1::enumerate(MethodResultJni& res)
{
    JNIEnv *env = jniInit();
    if (!env) {
        RAWLOG_ERROR("JNI initialization failed");
        return;
    }

    jhobject instance = env->CallStaticObjectMethod(s_singletonClass, s_midGetInstance);
    env->CallVoidMethod(instance.get(), s_midEnumerate, static_cast<jobject>(res));
}
//----------------------------------------------------------------------------------------------------------------------

rho::String CBarcode1::getDefaultID()
{
    JNIEnv *env = jniInit();
    if (!env) {
        RAWLOG_ERROR("JNI initialization failed");
        return rho::String();
    }

    jhstring res = static_cast<jstring>(env->CallStaticObjectMethod(s_singletonClass, s_midGetDefaultID));
    return rho_cast<rho::String>(env, res);
}
//----------------------------------------------------------------------------------------------------------------------

void CBarcode1::setDefaultID(const rho::String& id)
{
    JNIEnv *env = jniInit();
    if (!env) {
        RAWLOG_ERROR("JNI initialization failed");
        return;
    }

    jhobject instance = env->CallStaticObjectMethod(s_singletonClass, s_midGetInstance);
    jhstring jhId = rho_cast<jstring>(env, id);
    env->CallVoidMethod(instance.get(), s_midSetDefaultID, jhId.get());
}
//----------------------------------------------------------------------------------------------------------------------

jhobject CBarcode1::getObject(JNIEnv* env)
{
    jhstring jhId = rho_cast<jstring>(env, m_id);
    jhobject instance = env->CallStaticObjectMethod(s_singletonClass, s_midGetInstance);
    jhobject res = env->CallObjectMethod(instance.get(), s_midGetApiObject, jhId.get());
    return res;
}
//----------------------------------------------------------------------------------------------------------------------

void CBarcode1::getProps(MethodResultJni& result)
{
    JNIEnv *env = jniInit();
    if (!env) {
        RAWLOG_ERROR("JNI initialization failed");
        return;
    }

    jhobject jhGetPropsTask = env->NewObject(s_getPropsTaskClass, s_midGetPropsTask,
                    getObject(env).get(), static_cast<jobject>(result));
    run(jhGetPropsTask, result);
}
//----------------------------------------------------------------------------------------------------------------------

void CBarcode1::getProps(jhstring name, MethodResultJni& result)
{
    JNIEnv *env = jniInit();
    if (!env) {
        RAWLOG_ERROR("JNI initialization failed");
        return;
    }

    jhobject jhGetPropsTask = env->NewObject(s_getProps1TaskClass, s_midGetProps1Task,
                    getObject(env).get(), name.get(), static_cast<jobject>(result));
    run(jhGetPropsTask, result);
}
//----------------------------------------------------------------------------------------------------------------------

void CBarcode1::getProps(jhobject names, MethodResultJni& result)
{
    JNIEnv *env = jniInit();
    if (!env) {
        RAWLOG_ERROR("JNI initialization failed");
        return;
    }

    jhobject jhGetPropsTask = env->NewObject(s_getProps2TaskClass, s_midGetProps2Task,
                    getObject(env).get(), names.get(), static_cast<jobject>(result));
    run(jhGetPropsTask, result);
}
//----------------------------------------------------------------------------------------------------------------------

}

