#include <strstream>

#include "common/RhoStd.h"
#include "rhodes/JNIRhodes.h"
#include "rhodes/JNIRhoRuby.h"
#include "rhodes/jni/com_rhomobile_rhodes_api_MethodResult.h"
#include "api_generator/MethodResult.h"
#include "MethodResultJni.h"
#include "ruby/ext/rho/rhoruby.h"

#include "logging/RhoLog.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "MethodResultJNI"

namespace rhoelements {

const char * const MethodResultJni::METHOD_RESULT_CLASS = "com.rhomobile.rhodes.api.MethodResult";

jclass MethodResultJni::s_methodResultClass = 0;
jmethodID MethodResultJni::s_midMethodResult;
jmethodID MethodResultJni::s_midSetCallBack;
jmethodID MethodResultJni::s_midGetResultType;
jmethodID MethodResultJni::s_midReset;
jfieldID MethodResultJni::s_fidBoolean;
jfieldID MethodResultJni::s_fidInteger;
jfieldID MethodResultJni::s_fidDouble;
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
            RAWLOG_ERROR1("Failed to get method 'setCallback' for java class %s", METHOD_RESULT_CLASS);
            s_methodResultClass = 0;
            return NULL;
        }
        s_midGetResultType = env->GetMethodID(s_methodResultClass, "getResultType", "()I");
        if(!s_midGetResultType)
        {
            RAWLOG_ERROR1("Failed to get method 'getResultType' for java class %s", METHOD_RESULT_CLASS);
            s_methodResultClass = 0;
            return NULL;
        }
        s_midReset = env->GetMethodID(s_methodResultClass, "reset", "()V");
        if(!s_midReset)
        {
            RAWLOG_ERROR1("Failed to get method 'reset' for java class %s", METHOD_RESULT_CLASS);
            s_methodResultClass = 0;
            return NULL;
        }
        s_fidBoolean = env->GetFieldID(s_methodResultClass, "mBooleanResult", "Z");
        if(!s_fidBoolean)
        {
            RAWLOG_ERROR1("Failed to get 'mBooleanResult' field for java class %s", METHOD_RESULT_CLASS);
            s_methodResultClass = 0;
            return NULL;
        }
        s_fidInteger = env->GetFieldID(s_methodResultClass, "mIntegerResult", "I");
        if(!s_fidInteger)
        {
            RAWLOG_ERROR1("Failed to get 'mIntegerResult' field for java class %s", METHOD_RESULT_CLASS);
            s_methodResultClass = 0;
            return NULL;
        }
        s_fidDouble = env->GetFieldID(s_methodResultClass, "mDoubleResult", "D");
        if(!s_fidDouble)
        {
            RAWLOG_ERROR1("Failed to get 'mDoubleResult' field for java class %s", METHOD_RESULT_CLASS);
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

jboolean MethodResultJni::getBooleanResult(JNIEnv* env)
{
    return env->GetBooleanField(m_jhResult.get(), s_fidBoolean);
}
//----------------------------------------------------------------------------------------------------------------------

jint MethodResultJni::getIntegerResult(JNIEnv* env)
{
    return env->GetIntField(m_jhResult.get(), s_fidInteger);
}
//----------------------------------------------------------------------------------------------------------------------

jdouble MethodResultJni::getDoubleResult(JNIEnv* env)
{
    return env->GetDoubleField(m_jhResult.get(), s_fidDouble);
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

int MethodResultJni::getResultType(JNIEnv* env)
{
    if(m_resType == typeNone)
    {
        return env->CallIntMethod(m_jhResult.get(), s_midGetResultType);
    } else
    {
        return m_resType;
    }
}
//----------------------------------------------------------------------------------------------------------------------

rho::String MethodResultJni::getErrorMessage(JNIEnv* env)
{
    if(m_resType == typeNone)
    {
        jhstring jhMessage = getStringResult(env);
        return rho_cast<std::string>(env, jhMessage.get());
    } else
    {
        return m_errMsg;
    }
}
//----------------------------------------------------------------------------------------------------------------------

void MethodResultJni::reset(JNIEnv* env)
{
    m_resType = typeNone;
    env->CallVoidMethod(m_jhResult.get(), s_midReset);
}
//----------------------------------------------------------------------------------------------------------------------

MethodResultJni::MethodResultJni() : m_jhResult(0), m_hasCallbackUrl(false), m_resType(typeNone)
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

    JNIEnv *env = jniInit();
    if (!env) {
        RAWLOG_ERROR("JNI initialization failed");
        rb_raise(rb_eRuntimeError,"JNI initialization failed");
        return Qnil;;
    }

    if(getResultType(env) == typeList)
    {
        CHoldRubyValue valArray(rho_ruby_create_array());

        HStringVector pIDs = rho_cast<HStringVector>(env, getListResult(env));

        for(HStringVector::element_type::size_type i = 0; i < pIDs->size(); ++i)
        {
            VALUE valObj = rho_ruby_create_object_with_id( klass, (*pIDs)[i].c_str() );
            rho_ruby_add_to_array(valArray, valObj);
        }

        RAWTRACE("has enumerated");
        reset(env);
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
        RAWLOG_FATAL("JNI initialization failed");
        rb_raise(rb_eRuntimeError,"JNI initialization failed");
        return Qnil;
    }

    int type = getResultType(env);
    switch(type)
    {
    case typeNone:
        break;
    case typeBoolean:
        {
            bool booleanResult = static_cast<bool>(getBooleanResult(env));
            res = booleanResult ? Qtrue : Qfalse;
        }
        break;
    case typeInteger:
    {
        int intResult = static_cast<int>(getIntegerResult(env));
        res = rho_ruby_create_integer(intResult);
    }
    break;
    case typeDouble:
    {
        double doubleResult = static_cast<double>(getDoubleResult(env));
        res = rho_ruby_create_double(doubleResult);
    }
    break;
    case typeString:
        {
            jhstring jhStrResult = getStringResult(env);
            res = rho_cast<VALUE>(env, jhStrResult);
        }
        break;
    case typeList:
        {
            jhobject jhListResult = getListResult(env);
            res = rho_cast<VALUE>(env, jhListResult);
        }
        break;
    case typeMap:
        {
            jhobject jhMapResult = getMapResult(env);
            res = rho_cast<VALUE>(env, jhMapResult);
        }
        break;
    case typeArgError:
        rho_ruby_raise_argerror(getErrorMessage(env).c_str());
        break;
    case typeError:
        rb_raise(rb_eRuntimeError, getErrorMessage(env).c_str());
        break;
    default:
        RAWLOG_FATAL("Unknown runtime error in MethodResultJni class");
        rb_raise(rb_eRuntimeError,"Unknown runtime error in MethodResultJni class");
    }

    reset(env);
    return res;
}
//----------------------------------------------------------------------------------------------------------------------

std::string MethodResultJni::toJson()
{
    std::string res = "{}";
    JNIEnv *env = jniInit();
    if (!env) {
        RAWLOG_FATAL("JNI initialization failed");
        rb_raise(rb_eRuntimeError,"JNI initialization failed");
        return res;
    }

    int type = getResultType(env);
    switch(type)
    {
    case typeNone:
        break;
    case typeBoolean:
//        {
//            bool booleanResult = static_cast<bool>(getBooleanResult(env));
//            res = booleanResult ? Qtrue : Qfalse;
//        }
        break;
    case typeInteger:
//        {
//            int intResult = static_cast<int>(getIntegerResult(env));
//            res = rho_ruby_create_integer(intResult);
//        }
        break;
    case typeDouble:
//        {
//            double doubleResult = static_cast<double>(getDoubleResult(env));
//            res = rho_ruby_create_double(doubleResult);
//        }
        break;
    case typeString:
//        {
//            jhstring jhStrResult = getStringResult(env);
//            res = rho_cast<VALUE>(env, jhStrResult);
//        }
        break;
    case typeList:
//        {
//            jhobject jhListResult = getListResult(env);
//            res = rho_cast<VALUE>(env, jhListResult);
//        }
        break;
    case typeMap:
//        {
//            jhobject jhMapResult = getMapResult(env);
//            res = rho_cast<VALUE>(env, jhMapResult);
//        }
        break;
    case typeArgError:
    case typeError:
        {
            std::strstream buf;
            buf << "{'error' : { 'message' : '" << getErrorMessage(env) << "' } }";
            res = buf.str();
        }
        break;
    default:
        RAWLOG_ERROR("Unknown runtime error in MethodResultJni class");
        res = "{'error' : { 'message' : 'Unknown runtime error in MethodResultJni class' } }";
    }

    reset(env);
    return res;
}
//----------------------------------------------------------------------------------------------------------------------

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_api_MethodResult_nativeSetBoolean
  (JNIEnv * env, jclass, jboolean jRes, jstring jUrl, jstring jUrlData)
{
    RAWTRACE("nativeSetBoolean");

    bool res = static_cast<bool>(jRes);
    rho::String url = rho_cast<rho::String>(env, jUrl);
    rho::String data = rho_cast<rho::String>(env, jUrlData);

    rho::apiGenerator::CMethodResult result;
    result.setRubyCallback(url);
    result.setCallbackParam(data);
    result.set(res);
}
//----------------------------------------------------------------------------------------------------------------------

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_api_MethodResult_nativeSetInteger
  (JNIEnv * env, jclass, jint jRes, jstring jUrl, jstring jUrlData)
{
    RAWTRACE("nativeSetInteger");

    int res = static_cast<int>(jRes);
    rho::String url = rho_cast<rho::String>(env, jUrl);
    rho::String data = rho_cast<rho::String>(env, jUrlData);

    rho::apiGenerator::CMethodResult result;
    result.setRubyCallback(url);
    result.setCallbackParam(data);
    result.set(res);
}
//----------------------------------------------------------------------------------------------------------------------

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_api_MethodResult_nativeSetDouble
  (JNIEnv * env, jclass, jdouble jRes, jstring jUrl, jstring jUrlData)
{
    RAWTRACE("nativeSetDouble");

    double res = static_cast<double>(jRes);
    rho::String url = rho_cast<rho::String>(env, jUrl);
    rho::String data = rho_cast<rho::String>(env, jUrlData);

    rho::apiGenerator::CMethodResult result;
    result.setRubyCallback(url);
    result.setCallbackParam(data);
    result.set(res);
}
//----------------------------------------------------------------------------------------------------------------------

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_api_MethodResult_nativeSetString
  (JNIEnv * env, jclass, jstring jRes, jstring jUrl, jstring jUrlData)
{
    RAWTRACE("nativeSetString");

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
    RAWTRACE("nativeSetStringList");

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
    RAWTRACE("nativeSetStringMap");

    HStringMap pRes = rho_cast<HStringMap>(env, jRes);
    rho::String url = rho_cast<rho::String>(env, jUrl);
    rho::String data = rho_cast<rho::String>(env, jUrlData);

    rho::apiGenerator::CMethodResult result;
    result.setRubyCallback(url);
    result.setCallbackParam(data);
    result.set(*pRes);
}
//----------------------------------------------------------------------------------------------------------------------

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_api_MethodResult_nativeSetArgError
  (JNIEnv * env, jclass, jstring jRes, jstring jUrl, jstring jUrlData)
{
    RAWTRACE("nativeSetArgError");

    rho::String res = rho_cast<rho::String>(env, jRes);
    rho::String url = rho_cast<rho::String>(env, jUrl);
    rho::String data = rho_cast<rho::String>(env, jUrlData);

    rho::apiGenerator::CMethodResult result;
    result.setRubyCallback(url);
    result.setCallbackParam(data);
    result.setArgError(res);
}
//----------------------------------------------------------------------------------------------------------------------

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_api_MethodResult_nativeSetError
  (JNIEnv * env, jclass, jstring jRes, jstring jUrl, jstring jUrlData)
{
    RAWTRACE("nativeSetError");

    rho::String res = rho_cast<rho::String>(env, jRes);
    rho::String url = rho_cast<rho::String>(env, jUrl);
    rho::String data = rho_cast<rho::String>(env, jUrlData);

    rho::apiGenerator::CMethodResult result;
    result.setRubyCallback(url);
    result.setCallbackParam(data);
    result.setError(res);
}
//----------------------------------------------------------------------------------------------------------------------


}
