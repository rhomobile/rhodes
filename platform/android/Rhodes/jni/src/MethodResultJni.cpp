#include "common/RhoStd.h"
#include "rhodes/JNIRhodes.h"
#include "rhodes/JNIRhoRuby.h"
#include "rhodes/jni/com_rhomobile_rhodes_api_MethodResult.h"
#include "MethodResult.h"
#include "MethodResultJni.h"

#include "logging/RhoLog.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "MethodResultJNI"

namespace rhoelements {

const char * const MethodResultJni::METHOD_RESULT_CLASS = "com.rhomobile.rhodes.api.MethodResult";

jclass MethodResultJni::s_methodResultClass = 0;
jmethodID MethodResultJni::s_midMethodResult;
jmethodID MethodResultJni::s_midSetCallBack;
jfieldID MethodResultJni::s_fidString;
jfieldID MethodResultJni::s_fidStringList;
jfieldID MethodResultJni::s_fidStringMap;
//----------------------------------------------------------------------------------------------------------------------

//template <>
CallbackSelector<VALUE>::CallbackSelector(MethodResultJni& result, JNIEnv* env, jstring jUrl, jstring jData) {
    result.setRubyCallBack(env, jUrl, jData);
}

//template <>
CallbackSelector<rho::json::CJSONEntry>::CallbackSelector(MethodResultJni& result, JNIEnv* env, jstring jUrl, jstring jData) {
    result.setJSCallBack(env, jUrl, jData);
}
//----------------------------------------------------------------------------------------------------------------------

JNIEnv* MethodResultJni::jniInit()
{
    JNIEnv *env = jnienv();
    if(env && !s_methodResultClass)
    {
        jclass cls = rho_find_class(env, METHOD_RESULT_CLASS);
        if(!cls)
        {
            RAWLOG_ERROR1("Failed to find java class: %s", METHOD_RESULT_CLASS);
            s_methodResultClass = 0;
            return NULL;
        }
        s_methodResultClass = static_cast<jclass>(env->NewGlobalRef(cls));
        env->DeleteLocalRef(cls);

        s_midMethodResult = env->GetMethodID(s_methodResultClass, "<init>", "()V");
        if(!s_midMethodResult)
        {
            RAWLOG_ERROR1("Failed to get constructor for java class %s", METHOD_RESULT_CLASS);
            s_methodResultClass = 0;
            return NULL;
        }
        s_midSetCallBack = env->GetMethodID(s_methodResultClass, "setCallBack", "(Ljava/lang/String;Ljava/lang/String;)V");
        if(!s_midSetCallBack)
        {
            RAWLOG_ERROR1("Failed to get constructor for java class %s", METHOD_RESULT_CLASS);
            s_methodResultClass = 0;
            return NULL;
        }
        s_fidString = env->GetFieldID(s_methodResultClass, "mStrResult", "Ljava/lang/String;");
        if(!s_fidString)
        {
            RAWLOG_ERROR1("Failed to get 'mStrResult' field for java class %s", METHOD_RESULT_CLASS);
            s_methodResultClass = 0;
            return NULL;
        }
        s_fidStringList = env->GetFieldID(s_methodResultClass, "mListResult", "Ljava/util/List;");
        if(!s_fidStringList)
        {
            RAWLOG_ERROR1("Failed to get 'mListResult' field for java class %s", METHOD_RESULT_CLASS);
            s_methodResultClass = 0;
            return NULL;
        }
        s_fidStringMap = env->GetFieldID(s_methodResultClass, "mMapResult", "Ljava/util/Map;");
        if(!s_fidStringMap)
        {
            RAWLOG_ERROR1("Failed to get 'mMapResult' field for java class %s", METHOD_RESULT_CLASS);
            s_methodResultClass = 0;
            return NULL;
        }

    }
    return env;
}
//----------------------------------------------------------------------------------------------------------------------

jstring MethodResultJni::getStringResult(JNIEnv* env)
{
    return static_cast<jstring>(env->GetObjectField(m_jhResult.get(), s_fidString));
}
//----------------------------------------------------------------------------------------------------------------------

jobject MethodResultJni::getListResult(JNIEnv* env)
{
    return env->GetObjectField(m_jhResult.get(), s_fidStringList);
}
//----------------------------------------------------------------------------------------------------------------------

jobject MethodResultJni::getMapResult(JNIEnv* env)
{
    return env->GetObjectField(m_jhResult.get(), s_fidStringMap);
}
//----------------------------------------------------------------------------------------------------------------------

MethodResultJni::MethodResultJni() : m_jhResult(0), m_hasCallbackUrl(false), m_errType(eNone)
{
    JNIEnv *env = jniInit();
    if (!env) {
        RAWLOG_ERROR("ctor - JNI initialization failed");
        return;
    }

    m_jhResult = env->NewObject(s_methodResultClass, s_midMethodResult);
}
//----------------------------------------------------------------------------------------------------------------------

void MethodResultJni::setRubyCallBack(JNIEnv* env, jstring jUrl, jstring jData)
{
    RAWTRACE("setRubyCallBack");

    env->CallVoidMethod(m_jhResult.get(), s_midSetCallBack, jUrl, jData);
    m_hasCallbackUrl = true;

    RAWTRACE("Callback has set");
}
//----------------------------------------------------------------------------------------------------------------------

void MethodResultJni::setJSCallBack(JNIEnv* env, jstring jUrl, jstring jData)
{
    RAWTRACE("setJSCallBack");

    //TODO: Implement setJSCallBack()
//    env->CallVoidMethod(m_jhResult.get(), s_midSetCallBack, jUrl, jData);
//    m_hasCallbackUrl = true;
//
//    RAWTRACE("Callback has set");

}
//----------------------------------------------------------------------------------------------------------------------

void MethodResultJni::setRubyProcCallBack(unsigned long value)
{
    //TODO: Implement
    RAWLOG_ERROR("Only URL is supported as call back handler");
    setArgError("Only URL is supported as call back handler");
}
//----------------------------------------------------------------------------------------------------------------------

VALUE MethodResultJni::enumerateRubyObjects(VALUE klass)
{
    RAWTRACE("enumerateRubyObjects");

    if(m_errType == eNone)
    {

        CHoldRubyValue valArray(rho_ruby_create_array());

        JNIEnv *env = jniInit();
        if (!env) {
            RAWLOG_ERROR("JNI initialization failed");
            rb_raise(rb_eRuntimeError,"JNI initialization failed");
            return Qnil;;
        }

        HStringVector pIDs = rho_cast<HStringVector>(env, getListResult(env));

        for(HStringVector::element_type::size_type i = 0; i < pIDs->size(); ++i)
        {
            VALUE valObj = rho_ruby_create_object_with_id( klass, (*pIDs)[i].c_str() );
            rho_ruby_add_to_array(valArray, valObj);
        }

        RAWTRACE("has enumerated");
        return valArray;
    } else
    {
        return toRuby();
    }

}
//----------------------------------------------------------------------------------------------------------------------

rho::String MethodResultJni::enumerateJSObjects()
{
    return rho::String();
}
//----------------------------------------------------------------------------------------------------------------------

VALUE MethodResultJni::toRuby()
{
    RAWTRACE("toRuby");

    VALUE res = Qnil;

    JNIEnv *env = jniInit();
    if (!env) {
        RAWLOG_ERROR("JNI initialization failed");
        rb_raise(rb_eRuntimeError,"JNI initialization failed");
        return Qnil;
    }

    if(m_errType == eNone)
    {
        jhstring jhStrResult = getStringResult(env);
        if(!jhStrResult)
        {
            jhobject jhListResult = getListResult(env);
            if(!jhListResult)
            {
                jhobject jhMapResult = getMapResult(env);
                if(!jhMapResult)
                {
                    res = Qnil;
                }
                else
                {
                    res = rho_cast<VALUE>(env, jhMapResult);
                }
            }
            else
            {
                res = rho_cast<VALUE>(env, jhListResult);
            }
        }
        else
        {
            res = rho_cast<VALUE>(env, jhStrResult);
        }
    }
    else if(m_errType == eArgError)
    {
        rho_ruby_raise_argerror(m_errMsg.c_str());
    }
    else if(m_errType == eError)
    {
        rb_raise(rb_eRuntimeError,m_errMsg.c_str());
    }
    else
    {
        rb_raise(rb_eRuntimeError,"Unknown runtime error in MethodResultJni class");
    }

    return res;
}
//----------------------------------------------------------------------------------------------------------------------

std::string MethodResultJni::toJson()
{
    std::string res;
//    if(m_errType == eArgError )
//    {
//        res = "{'_RhoArgError':" + CJSONEntry::quoteValue(m_errMsg) + "}";
//    }
//    else if(m_errType == eError)
//    {
//        res = "{'_RhoRuntimeError':" + CJSONEntry::quoteValue(m_errMsg) + "}";
//    }
//
    return res;
}
//----------------------------------------------------------------------------------------------------------------------

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_api_MethodResult_nativeSetString
  (JNIEnv * env, jclass, jstring jRes, jstring jUrl, jstring jUrlData)
{
    rho::String res = rho_cast<rho::String>(env, jRes);
    rho::String url = rho_cast<rho::String>(env, jUrl);
    rho::String data = rho_cast<rho::String>(env, jUrlData);

    rho::apiGenerator::CMethodResult result;
    result.setRubyCallback(url);
    result.setCallbackParam(data);
    result.set(res);

}
//----------------------------------------------------------------------------------------------------------------------

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_api_MethodResult_nativeSetStringList
  (JNIEnv * env, jclass, jobject jRes, jstring jUrl, jstring jUrlData)
{
    HStringVector pRes = rho_cast<HStringVector>(env, jRes);
    rho::String url = rho_cast<rho::String>(env, jUrl);
    rho::String data = rho_cast<rho::String>(env, jUrlData);

    rho::apiGenerator::CMethodResult result;
    result.setRubyCallback(url);
    result.setCallbackParam(data);
    result.set(*pRes);
}
//----------------------------------------------------------------------------------------------------------------------

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_api_MethodResult_nativeSetStringMap
  (JNIEnv * env, jclass, jobject jRes, jstring jUrl, jstring jUrlData)
{
    HStringMap pRes = rho_cast<HStringMap>(env, jRes);
    rho::String url = rho_cast<rho::String>(env, jUrl);
    rho::String data = rho_cast<rho::String>(env, jUrlData);

    rho::apiGenerator::CMethodResult result;
    result.setRubyCallback(url);
    result.setCallbackParam(data);
    result.set(*pRes);
}
//----------------------------------------------------------------------------------------------------------------------


}
