#pragma once

#include "common/RhoStd.h"
#include "rhodes/JNIRhodes.h"
#include "MethodExecutorJni.h"

namespace rhoelements {

class MethodResultJni;

class CBarcode1Base : public MethodExecutorJni
{
protected:
    static const char* const FACTORY_SINGLETON_CLASS;
    static jclass s_clsFactorySingleton;
    static jmethodID s_midFactorySetInstance;
    static jmethodID s_midFactoryGetInstance;

    //IBarcode1Factory staff
    static const char* const IFACTORY_CLASS;
    static jclass s_clsIFactory;
    static jmethodID s_midGetApiSingleton;
    static jmethodID s_midGetApiObject;

    //Barcode1SingletonBase staff
//    static const char* const SINGLETON_BASE_CLASS;
//    static jclass s_clsSingletonBase;

    //Barcode1Base staff
    static const char* const OBJECT_BASE_CLASS;
    static const char* const GETPROPS_TASK_CLASS;
    static const char* const GETPROPS1_TASK_CLASS;
    static const char* const GETPROPS2_TASK_CLASS;
    static const char* const TAKEBARCODE_TASK_CLASS;
    static jclass s_clsObjectBase;
    static jclass s_clsGetPropsTask;
    static jclass s_clsGetProps1Task;
    static jclass s_clsGetProps2Task;
    static jclass s_clsTakeBarcodeTask;
    static jmethodID s_midGetPropsTask;
    static jmethodID s_midGetProps1Task;
    static jmethodID s_midGetProps2Task;
    static jmethodID s_midTakeBarcodeTask;

    //Barcode1Singleton staff
    static const char* const ISINGLETON_CLASS;
    static jclass s_clsISingleton;
    static jmethodID s_midEnumerate;
    static jmethodID s_midGetDefaultID;
    static jmethodID s_midSetDefaultID;

    //Barcode1 staff
    //static const char* const IOBJECT_CLASS;

    static JNIEnv* jniInit(JNIEnv* env);
    static JNIEnv* jniInit();

    static jobject getFactory(JNIEnv* env);
    static jobject getSingleton(JNIEnv* env);

    rho::String m_id;
    jobject getObject(JNIEnv* env);
public:
    CBarcode1Base(const rho::String& id)
        : MethodExecutorJni(false, true), m_id(id)
        {}
    virtual ~CBarcode1Base() {}

    static void setJavaFactory(JNIEnv* env, jobject jFactory);
    static rho::String getDefaultID();
    static void setDefaultID(const rho::String& id);
};

template<typename T>
class CBarcode1 : public CBarcode1Base
{

public:
    static void enumerate(T& args, MethodResultJni& result)
    {
        RAWTRACEC("Barcode1JNI", "enumerate");

        JNIEnv *env = jniInit();
        if (!env) {
            RAWLOGC_FATAL("Barcode1JNI", "JNI initialization failed");
            return;
        }

        jhobject jhSingleton = getSingleton(env);
        env->CallVoidMethod(jhSingleton.get(), s_midEnumerate, static_cast<jobject>(result));

    }

    CBarcode1(const rho::String& id)
        : CBarcode1Base(id)
        {}
    virtual ~CBarcode1() {}

    void getAllProperties(T& args, MethodResultJni& result)
    {
        RAWTRACEC("Barcode1JNI", "getAllProperties");

        JNIEnv *env = jniInit();
        if (!env) {
            RAWLOGC_FATAL("Barcode1JNI", "JNI initialization failed");
            return;
        }

        jhobject jhObject = getObject(env);
        jhobject jhTask = env->NewObject(s_clsGetPropsTask, s_midGetPropsTask,
                        jhObject.get(), static_cast<jobject>(result));
        run(env, jhTask.get(), result);

    }
    void getProperty(T& args, MethodResultJni& result)
    {
        RAWTRACEC("Barcode1JNI", "getProperty");

        JNIEnv *env = jniInit();
        if (!env) {
            RAWLOGC_FATAL("Barcode1JNI", "JNI initialization failed");
            return;
        }

        jhobject jhObject = getObject(env);
        jhstring jhName = rho_cast<jstring>(args[0]);
        jhobject jhTask = env->NewObject(s_clsGetProps1Task, s_midGetProps1Task,
                        jhObject.get(), jhName.get(), static_cast<jobject>(result));
        run(env, jhTask.get(), result);
    }

    void getProperties(T& args, MethodResultJni& result)
    {
        RAWTRACEC("Barcode1JNI", "getProperties");

        JNIEnv *env = jniInit();
        if (!env) {
            RAWLOGC_FATAL("Barcode1JNI", "JNI initialization failed");
            return;
        }

        jhobject jhObject = getObject(env);
        jhobject jhNames = rho_cast<jobject>(args[0]);
        jhobject jhTask = env->NewObject(s_clsGetProps2Task, s_midGetProps2Task,
                        jhObject.get(), jhNames.get(), static_cast<jobject>(result));
        run(env, jhTask.get(), result);
    }

    void takeBarcode(T& args, MethodResultJni& result)
    {
        RAWTRACEC("Barcode1JNI", "takeBarcode");

        JNIEnv *env = jniInit();
        if (!env) {
            RAWLOGC_FATAL("Barcode1JNI", "JNI initialization failed");
            return;
        }

        jhobject jhObject = getObject(env);
        jhobject jhTask = env->NewObject(s_clsTakeBarcodeTask, s_midTakeBarcodeTask,
                        jhObject.get(), static_cast<jobject>(result));
        run(env, jhTask.get(), result);

    }

};

}
