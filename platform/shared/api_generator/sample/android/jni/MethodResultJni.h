#pragma once

#include "rhodes/JNIRhodes.h"

#include <string>

namespace rhoelements {


class MethodResultJni
{
    static const char const * METHOD_RESULT_CLASS;
    enum ErrType{ eNone = 0, eError, eArgError};

    static jclass s_methodResultClass;
    static jmethodID s_midMethodResult;
    static jmethodID s_midSetCallBack;
    static jfieldID s_fidString;
    static jfieldID s_fidStringList;
    static jfieldID s_fidStringMap;

    static JNIEnv* jniInit();

    jhobject m_jhResult;

    bool m_hasCallbackUrl;

    ErrType m_errType;
    std::string m_errMsg;

    jhstring getStringResult(JNIEnv*);
    jhobject getListResult(JNIEnv*);
    jhobject getMapResult(JNIEnv*);

public:
    MethodResultJni();

    void setCallBack(const char* url, const char* data);
    bool hasCallBackUrl() { return m_hasBackUrl != 0; }

    operator jobject () { return m_jhResult.get(); }
    operator bool () { return jobject(this) != 0; }

    VALUE enumerateRubyObjects();
    std::string enumerateJSObjects();

    VALUE toRuby();
    std::string toJson();

    void setError(const char* msg) {
        m_errType = eError;
        m_errMsg = msg;
    }
    void setArgError(const char *msg) {
        m_errType = eArgError;
        m_errMsg = msg;
    }
};

}
