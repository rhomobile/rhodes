#include "common/RhoStd.h"
#include "rhodes/JNIRhodes.h"
#include "com_motorolasolutions_rhoelements_MethodResult.h"
#include "common/MethodResult.h"
#include "MethodResultJni.h"

#include "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "MethodResultJNI"

namespace rhoelements {

const char const * MethodResultJni::METHOD_RESULT_CLASS = "com/motorolasolutions/rhoelements/MethodResult";

jclass MethodResultJni::s_methodResultClass = 0;
jmethodID MethodResultJni::s_midMethodResult;
jmethodID MethodResultJni::s_midSetCallBack;
jfieldID MethodResultJni::s_fidString;
jfieldID MethodResultJni::s_fidStringList;
jfieldID MethodResultJni::s_fidStringMap;

//----------------------------------------------------------------------------------------------------------------------
JNIEnv* MethodResultJni::jniInit()
{
    JNIEnv *env = jnienv();
    if(env && !s_methodResultClass)
    {
        s_methodResultClass = rho_find_class(env, METHOD_RESULT_CLASS);
        if(!s_methodResultClass)
        {
            RAWLOG_ERROR1("Failed to find java class: %s", METHOD_RESULT_CLASS);
            s_methodResultClass = 0;
            return NULL;
        }
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
        s_fidString = env->GetFieldID(s_methodResultClass, "mSrtResult", "Ljava/lang/String;");
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

jhstring MethodResultJni::getStringResult(JNIEnv* env)
{
    jhstring jhRes = static_cast<jstring>(env->GetObjectField(m_jhResult.get(), s_fidString));
    return jhRes;
}
//----------------------------------------------------------------------------------------------------------------------

jhobject MethodResultJni::getListResult(JNIEnv* env)
{
    jhobject jhRes = env->GetObjectField(m_jhResult.get(), s_fidStringList);
    return jhRes;
}
//----------------------------------------------------------------------------------------------------------------------

jhobject MethodResultJni::getMapResult(JNIEnv* env)
{
    jhobject jhRes = env->GetObjectField(m_jhResult.get(), s_fidStringMap);
    return jhRes;
}
//----------------------------------------------------------------------------------------------------------------------

MethodResultJni::MethodResultJni() : m_hasBackUrl(false), m_errtype(eNone)
{
    JNIEnv *env = jniInit();
    if (!env) {
        RAWLOG_ERROR("JNI initialization failed");
        return;
    }

    m_jhResult = env->NewObject(s_methodResultClass, s_midMethodResult);
}
//----------------------------------------------------------------------------------------------------------------------

void MethodResultJni::setCallBack(const char* url, const char* data)
{
    m_hasBackUrl = true;

    JNIEnv* env = jniInit();
    if (!env) {
        RAWLOG_ERROR("JNI initialization failed");
        return;
    }

    jhstring jhUrl = rho_cast<jhstring>(url);
    jhstring jhData = rho_cast<jhstring>(data);
    env->CallVoidMethod(m_jhResult.get(), s_midSetCallBack, jhUrl.get(), jhData.get());
}
//----------------------------------------------------------------------------------------------------------------------

VALUE MethodResultJni::enumerateRubyObjects()
{
    CHoldRubyValue valArray(rho_ruby_create_array());

    JNIEnv *env = jniInit();
    if (!env) {
        RAWLOG_ERROR("JNI initialization failed");
        rho_ruby_raise_runtime("JNI initialization failed");
        return rho_ruby_get_NIL();;
    }

    rho::Vector<rho::String> arIDs = rho_cast<rho::Vector<rho::String> >(env, getListResult(env));

    for( int i = 0; i < arIDs.size(); i++ )
    {
        //TODO: ASk Evgeny about this staff
        //if ( !CBarcode1SingletonBase::getInstance()->getModules().containsKey(arIDs[i]) )
        //{
        //    IBarcode1* pObj = CBarcode1SingletonBase::getInstance()->create(arIDs[i]);
        //    CBarcode1SingletonBase::getInstance()->getModules().put(arIDs[i], pObj );
        //}

        VALUE valObj = rho_create_object_with_id( klass, arIDs[i].c_str() );
        rho_ruby_add_to_array( valArray, valObj );
    }

    return valArray;

}
//----------------------------------------------------------------------------------------------------------------------

std::string MethodResultJni::enumerateJSObjects()
{

}
//----------------------------------------------------------------------------------------------------------------------

VALUE MethodResultJni::toRuby()
{
    VALUE res;

    JNIEnv *env = jniInit();
    if (!env) {
        RAWLOG_ERROR("JNI initialization failed");
        rho_ruby_raise_runtime("JNI initialization failed");
        return rho_ruby_get_NIL();
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
                    res = rho_ruby_get_NIL();
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
            res = rho_cast<VALUE>(env, jhStringRes);
        }
    }
    else if(m_errType == eArgError)
    {
        rho_ruby_raise_argerror(m_errMsg.c_str());
    }
    else if(m_errType == eError)
    {
        rho_ruby_raise_runtime(m_errMsg.c_str());
    }
    else
    {
        rho_ruby_raise_runtime("Unknown runtime error in MethodResultJni class");
    }

    return res;
}
//----------------------------------------------------------------------------------------------------------------------

std::string MethodResultJni::toJson()
{
    std::string res;
    if(m_errType == eArgError )
    {
        res = "{'_RhoArgError':" + CJSONEntry::quoteValue(m_errMsg) + "}";
    }
    else if(m_errType == eError)
    {
        res = "{'_RhoRuntimeError':" + CJSONEntry::quoteValue(m_errMsg) + "}";
    }

    return res;
}
//----------------------------------------------------------------------------------------------------------------------

RHO_GLOBAL void JNICALL Java_com_motorolasolutions_rhoelements_MethodResult_nativeSetString
  (JNIEnv *, jclass, jstring jRes, jstring jUrl, jstring jUrlData)
{
    rho::String res = rho_cast<rho::String>(jRes);
    rho::String url = rho_cast<rho::String>(jUrl);
    rho::String data = rho_cast<rho::String>(jUrlData);

    MethodResult result;
    result.setRubyCallback(url);
    result.setCallbackParam(data);
    result.set(res);

}
//----------------------------------------------------------------------------------------------------------------------

RHO_GLOBAL void JNICALL Java_com_motorolasolutions_rhoelements_MethodResult_nativeSetStringList
  (JNIEnv *, jclass, jobject jRes, jstring jUrl, jstring jUrlData)
{
    rho::Vector<rho::String> res = rho_cast<rho::Vector<rho::String> >(jRes);
    rho::String url = rho_cast<rho::String>(jUrl);
    rho::String data = rho_cast<rho::String>(jUrlData);

    MethodResult result;
    result.setRubyCallback(url);
    result.setCallbackParam(data);
    result.set(res);

}
//----------------------------------------------------------------------------------------------------------------------

RHO_GLOBAL void JNICALL Java_com_motorolasolutions_rhoelements_MethodResult_nativeSetStringMap
  (JNIEnv *, jclass, jobject jRes, jstring jUrl, jstring jUrlData)
{
    rho::Hashtable<rho::String, rho::String> res = rho_cast<rho::Hashtable<rho::String, rho::String> >(jRes);
    rho::String url = rho_cast<rho::String>(jUrl);
    rho::String data = rho_cast<rho::String>(jUrlData);

    MethodResult result;
    result.setRubyCallback(url);
    result.setCallbackParam(data);
    result.set(res);

}
//----------------------------------------------------------------------------------------------------------------------


}
