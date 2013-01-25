#pragma once

#include "common/RhoStd.h"
#include "rhodes/JNIRhodes.h"
#include "MethodExecutorJni.h"

namespace rhoelements {

class MethodResultJni;

class CBarcode1 : public MethodExecutorJni
{
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
    static void setJavaFactory(JNIEnv* env, jobject jFactory);

    static void enumerate(MethodResultJni& result);
    static rho::String getDefaultID();
    static void setDefaultID(const rho::String& id);

    CBarcode1(const rho::String& id)
        : MethodExecutorJni(false, true), m_id(id)
        {}
    virtual ~CBarcode1() {}

    void getProps(MethodResultJni& result);
    void getProps(jstring name, MethodResultJni& result);
    void getProps(jobject names, MethodResultJni& result);
    void takeBarcode(MethodResultJni& result);

};

}
