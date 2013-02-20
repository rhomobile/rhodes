#pragma once

#include "rhodes/JNIRhodes.h"
#include "ruby.h"
#include "json/JSONIterator.h"

#include <string>

namespace rhoelements {

template <typename T> struct CallbackSelector;

class MethodResultJni;

template <>
struct CallbackSelector<VALUE> {
    CallbackSelector(MethodResultJni& result, JNIEnv* env, jstring jUrl, jstring jData);
};

template <>
struct CallbackSelector<rho::json::CJSONEntry> {
    CallbackSelector(MethodResultJni& result, JNIEnv* env, jstring jUrl, jstring jData);
};

class MethodResultJni
{
    static const char * const METHOD_RESULT_CLASS;
    enum ResultType { typeNone = 0, typeBoolean, typeInteger, typeDouble, typeString, typeList, typeMap, typeError, typeArgError };

    static jclass s_methodResultClass;
    static jmethodID s_midMethodResult;
    static jmethodID s_midSetCallBack;
    static jmethodID s_midGetResultType;
    static jmethodID s_midReset;
    static jfieldID s_fidBoolean;
    static jfieldID s_fidInteger;
    static jfieldID s_fidDouble;
    static jfieldID s_fidString;
    static jfieldID s_fidStringList;
    static jfieldID s_fidStringMap;

    static JNIEnv* jniInit();

    jhobject m_jhResult;

    bool m_hasCallbackUrl;

    ResultType m_resType;
    std::string m_errMsg;

    jboolean getBooleanResult(JNIEnv*);
    jint getIntegerResult(JNIEnv*);
    jdouble getDoubleResult(JNIEnv*);
    jstring getStringResult(JNIEnv*);
    jobject getListResult(JNIEnv*);
    jobject getMapResult(JNIEnv*);
    int getResultType(JNIEnv*);
    rho::String getErrorMessage(JNIEnv*);
    void reset(JNIEnv*);

public:
    MethodResultJni();

    bool hasCallBackUrl() const { return m_hasCallbackUrl; }
    void setRubyCallBack(JNIEnv* env, jstring jUrl, jstring jData);
    void setJSCallBack(JNIEnv* env, jstring jUrl, jstring jData);
    template <typename T>
    void setCallBack(T url)
    {
        JNIEnv* env = jniInit();
        if (!env) {
            setError("JNI initialization failed");
            RAWLOGC_ERROR("MethodResultJNI", "JNI initialization failed");
            return;
        }
        jhstring jhUrl = rho_cast<jstring>(env, url);
        if(!jhUrl) {
            setArgError("Callback URL must be a string");
            RAWLOGC_ERROR("MethodResultJNI", "Callback URL must be string");
            return;
        }

        CallbackSelector<T>(*this, env, jhUrl.get(), 0);
        //setCallback(jhUrl.get(), 0);
    }

    template <typename T>
    void setCallBack(T url, T data)
    {
        JNIEnv* env = jniInit();
        if (!env) {
            RAWLOGC_ERROR("MethodResultJNI", "JNI initialization failed");
            return;
        }
        jhstring jhUrl = rho_cast<jstring>(env, url);
        if(!jhUrl) {
            setArgError("Callback URL must be a string");
            RAWLOGC_ERROR("MethodResultJNI", "Callback URL must be string");
            return;
        }
        jhstring jhData = rho_cast<jstring>(env, data);
        if(!jhData) {
            setArgError("Callback URL data must be a string");
            RAWLOGC_ERROR("MethodResultJNI", "Callback URL must be string");
            return;
        }

        CallbackSelector<T>(*this, env, jhUrl.get(), jhData.get());
        //setCallback(jhUrl.get(), jhData.get());
    }
    void setRubyProcCallBack(unsigned long value);
    operator jobject () { return m_jhResult.get(); }
    operator bool () const { return jobject(this) != 0; }

    VALUE enumerateRubyObjects(VALUE klass);
    std::string enumerateJSObjects();

    VALUE toRuby();
    std::string toJson();

    void setError(const char* msg) {
        m_resType = typeError;
        m_errMsg = msg;
    }
    void setArgError(const char *msg) {
        m_resType = typeArgError;
        m_errMsg = msg;
    }
};

}
