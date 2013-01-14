#pragma once

#include "common/RhoStd.h"
#include "rhodes/JNIRhodes.h"
#include "MethodExecutorJni.h"

namespace rhoelements {

class MethodResultJni;

class CBarcode1 : public MethodExecutorJni
{
    static const char const * SINGLETON_CLASS;
    static const char const * INSTANCE_CLASS;
    static const char const * GETPROPS_TASK_CLASS;
    static const char const * GETPROPS1_TASK_CLASS;
    static const char const * GETPROPS2_TASK_CLASS;

    static jclass s_singletonClass;
    static jclass s_getPropsTaskClass;
    static jclass s_getProps1TaskClass;
    static jclass s_getProps2TaskClass;
    static jmethodID s_midGetInstance;
    static jmethodID s_midEnumerate;
    static jmethodID s_midGetDefaultID;
    static jmethodID s_midSetDefaultID;
    static jmethodID s_midGetApiObject;
    static jmethodID s_midGetPropsTask;
    static jmethodID s_midGetProps1Task;
    static jmethodID s_midGetProps2Task;

    static JNIEnv* jniInit();

    rho::String m_id;
    //jhobject m_jhObject;
    jhobject getObject(JNIEnv* env);

public:
    static void enumerate(MethodResultJni& result);
    static rho::String getDefaultID();
    static void setDefaultID(const rho::String& id);

    CBarcode1(const rho::String& id)
        : MethodExecutorJni(false, false), m_id(id)
        {}
    virtual ~CBarcode1() {}

    void getProps(MethodResultJni& result);
    void getProps(jhstring name, MethodResultJni& result);
    void getProps(jhobject names, MethodResultJni& result);

};

}
