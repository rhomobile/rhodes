#pragma once

#include "rhodes/JNIRhodes.h"

namespace rhoelements {

class CBarcode1
{
    static const char const * CLASS;
    static jclass s_class;
    static jmethodID s_midEnumerate;
    static jmethodID s_midGetDefault;
    static jmethodID s_midSetDefaultID;
    static jmethodID s_midCreate;

    static JNIEnv* jniInit();

    jhobject m_jhObject;
    bool m_uiThread;

public:
    static jhobject enumerate();
    static jhobject getDefault();
    static void setDefaultID(jhstring jhId);
    static jhobject create(jhstring jhId);

    CBarcode1(jhobject& jhObject, bool uiThread = false)
        :m_jhObject(jhObject), m_uiThread(uiThread) {}

    void getProps(MethodResultJni& result);
    void getProps(jhstring name, MethodResultJni& result);
    void getProps(jobject names, MethodResultJni& result);

};

}
