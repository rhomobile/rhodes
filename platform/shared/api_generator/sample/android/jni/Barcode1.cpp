#include "Barcode1.h"

#include "rhodes/JNIRhodes.h"
#include "MethodResultJni.h"

namespace rhoelements {

const char const * CBarcode1::SINGLETON_CLASS = "com/motorolasolutions/rhoelements/Barcode1";
const char const * CBarcode1::INSTANCE_CLASS = "com/motorolasolutions/rhoelements/IBarcode1";
const char const * CBarcode1::GETPROPS_TASK_CLASS = "com/motorolasolutions/rhoelements/Barcode1.GetPropsTask";
const char const * CBarcode1::GETPROPS1_TASK_CLASS = "com/motorolasolutions/rhoelements/Barcode1.GetProps1Task";
const char const * CBarcode1::GETPROPS2_TASK_CLASS = "com/motorolasolutions/rhoelements/Barcode2.GetProps1Task";

jclass CBarcode1::s_singletonClass = NULL;
jclass CBarcode1::s_instanceClass = NULL;
jclass CBarcode1::s_getPropsTaskClass = NULL;
jclass CBarcode1::s_getProps1TaskClass = NULL;
jclass CBarcode1::s_getProps2TaskClass = NULL;

jmethodID CBarcode::s_midEnumerate;
jmethodID CBarcode::s_midGetDefaultID;
jmethodID CBarcode::s_midSetDefaultID;
jmethodID CBarcode::s_midGetProps;
jmethodID CBarcode::s_midGetProps1;
jmethodID CBarcode::s_midGetProps2;



//----------------------------------------------------------------------------------------------------------------------

JNIEnv* CBarcode1::jniInit()
{
    JNIEnv *env = MethodExecutorJni::jniInit();
    if(env && !s_singletonClass)
    {
        s_singletonClass = rho_find_class(env, SINGLETON_CLASS);
        if(!s_singletonClass)
        {
            RAWLOG_ERROR1("Failed to find java class: %s", SINGLETON_CLASS);
            s_singletonClass = 0;
            return NULL;
        }
        s_midEnumerate = env->GetStaticMethodID(s_singletonClass, "enumerate", "(V)Ljava/util/List;");
        if(!s_midEnumerate)
        {
            RAWLOG_ERROR1("Failed to get method 'enumerate' for java class %s", SINGLETON_CLASS);
            s_singletonClass = 0;
            return NULL;
        }
        s_midGetDefault = env->GetStaticMethodID(s_singletonClass, "getDefault", "()Lcom/rhomobile/rhoelements/IBarcode1;");
        if(!s_midGetDefault)
        {
            RAWLOG_ERROR1("Failed to get method 'getDefault' for java class %s", SINGLETON_CLASS);
            s_singletonClass = 0;
            return NULL;
        }
        s_midSetDefaultID = env->GetStaticMethodID(s_singletonClass, "setDefaultID", "(Ljava/util/String;)V");
        if(!s_midSetDefaultID)
        {
            RAWLOG_ERROR1("Failed to get method 'setDefaultID' for java class %s", SINGLETON_CLASS);
            s_singletonClass = 0;
            return NULL;
        }
        s_midCreate = env->GetStaticMethodID(s_singletonClass, "create", "(Ljava/util/String;)Lcom/rhomobile/rhoelements/IBarcode1;");
        if(!s_midCreate)
        {
            RAWLOG_ERROR1("Failed to get method 'create' for java class %s", SINGLETON_CLASS);
            s_singletonClass = 0;
            return NULL;
        }

        s_getPropsTaskClass = rho_find_class(env, GETPROPS_TASK_CLASS);
        if(!s_getPropsTaskClass)
        {
            RAWLOG_ERROR1("Failed to find java class: %s", GETPROPS_TASK_CLASS);
            s_singletonClass = 0;
            return NULL;
        }
        s_midGetPropsTask = env->GetMethodID(s_getPropsTaskClass, "<init>",
                        "(Lcom/rhomobile/rhoelements/IBarcode1;Lcom/rhomobile/rhoelements/IMethodResult;)V");
        if(!s_midGetPropsTask)
        {
            RAWLOG_ERROR1("Failed to get constructor for java class %s", GETPROPS_TASK_CLASS);
            s_singletonClass = 0;
            return NULL;
        }

        s_getProps1TaskClass = rho_find_class(env, GETPROPS1_TASK_CLASS);
        if(!s_getProps1TaskClass)
        {
            RAWLOG_ERROR1("Failed to find java class: %s", GETPROPS1_TASK_CLASS);
            s_singletonClass = 0;
            return NULL;
        }
        s_midGetProps1Task = env->GetMethodID(s_getProps1TaskClass, "<init>",
                        "(Lcom/rhomobile/rhoelements/IBarcode1;Ljava/util/String;Lcom/rhomobile/rhoelements/IMethodResult;)V");
        if(!s_midGetProps1Task)
        {
            RAWLOG_ERROR1("Failed to get constructor for java class %s", GETPROPS1_TASK_CLASS);
            s_singletonClass = 0;
            return NULL;
        }

        s_getProps2TaskClass = rho_find_class(env, GETPROPS2_TASK_CLASS);
        if(!s_getProps2TaskClass)
        {
            RAWLOG_ERROR1("Failed to find java class: %s", GETPROPS2_TASK_CLASS);
            s_singletonClass = 0;
            return NULL;
        }
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

jhobject CBarcode1::enumerate()
{
    JNIEnv *env = jniInit();
    if (!env) {
        RAWLOG_ERROR("JNI initialization failed");
        return;
    }

    jhobject res = env->CallStaticObjectMethod(s_class, s_midEnumerate);
    return res;
}
//----------------------------------------------------------------------------------------------------------------------

jhobject CBarcode1::getDefault()
{
    JNIEnv *env = jniInit();
    if (!env) {
        RAWLOG_ERROR("JNI initialization failed");
        return;
    }

    jhstring res = env->CallStaticObjectMethod(s_class, s_midGetDefault);
    return res;

}
//----------------------------------------------------------------------------------------------------------------------

void CBarcode1::setDefaultID(jhstring& jhId)
{
    JNIEnv *env = jniInit();
    if (!env) {
        RAWLOG_ERROR("JNI initialization failed");
        return;
    }

    env->CallStaticVoidMethod(s_class, s_midSetDefaultID, jhId.get());

}
//----------------------------------------------------------------------------------------------------------------------

jhobject CBarcode1::create(jhstring& jhId)
{
    JNIEnv *env = jniInit();
    if (!env) {
        RAWLOG_ERROR("JNI initialization failed");
        return;
    }

    jhobject res = env->CallStaticObjectMethod(s_class, s_midCreate, ghId.get());
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
                    m_jhObject.get(), static_cast<jobject>(result));
    run(jhGetPropsTask, m_uiThread, result);
}
//----------------------------------------------------------------------------------------------------------------------

void CBarcode1::getProps(jhstring name, MethodResultJni& result)
{
    JNIEnv *env = jniInit();
    if (!env) {
        RAWLOG_ERROR("JNI initialization failed");
        return;
    }

    jhobject jhGetPropsTask = env->NewObject(s_getPropsTask1Class, s_midGetProps1Task,
                    m_jhObject.get(), name.get(), static_cast<jobject>(result));
    run(jhGetPropsTask, m_uiThread, result);
}
//----------------------------------------------------------------------------------------------------------------------

void CBarcode1::getProps(jhobject names, MethodResultJni& result)
{
    JNIEnv *env = jniInit();
    if (!env) {
        RAWLOG_ERROR("JNI initialization failed");
        return;
    }

    jhobject jhGetPropsTask = env->NewObject(s_getPropsTask2Class, s_midGetProps2Task,
                    m_jhObject.get(), names.get(), static_cast<jobject>(result));
    run(jhGetPropsTask, m_uiThread, result);
}
//----------------------------------------------------------------------------------------------------------------------
}
