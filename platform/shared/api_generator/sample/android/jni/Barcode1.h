#pragma once

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
    static jmethodID s_midEnumerate;
    static jmethodID s_midGetDefault;
    static jmethodID s_midSetDefaultID;
    static jmethodID s_midCreate;
    static jmethodID s_midGetPropsTask;
    static jmethodID s_midGetProps1Task;
    static jmethodID s_midGetProps2Task;

    static JNIEnv* jniInit();

    jhobject m_jhObject;

public:
    static jhobject enumerate();
    static jhobject getDefault();
    static void setDefaultID(jhstring jhId);
    static jhobject create(jhstring jhId);

    CBarcode1(jhobject& jhObject, bool uiThread)
        : MethodExecutorJni(false, uiThread), m_jhObject(jhObject)
        {}
    virtual ~CBarcode1() {}

    void getProps(MethodResultJni& result);
    void getProps(jhstring name, MethodResultJni& result);
    void getProps(jhobject names, MethodResultJni& result);

};

}
