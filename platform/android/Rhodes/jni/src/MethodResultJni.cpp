#include "common/RhoStd.h"
#include "common/StringConverter.h"
#include "rhodes/JNIRhodes.h"
#include "rhodes/JNIRhoRuby.h"
#include "rhodes/jni/com_rhomobile_rhodes_api_MethodResult.h"
#include "api_generator/MethodResult.h"
#include "MethodResultJni.h"
#include "ruby/ext/rho/rhoruby.h"

#include "logging/RhoLog.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "MethodResultJNI"

RHO_GLOBAL int rho_webview_active_tab();

namespace rho {
namespace apiGenerator {

const char * const MethodResultJni::METHOD_RESULT_CLASS = "com.rhomobile.rhodes.api.MethodResult";

jclass MethodResultJni::s_methodResultClass = 0;
jmethodID MethodResultJni::s_midMethodResult;
jmethodID MethodResultJni::s_midGetJson;
jmethodID MethodResultJni::s_midGetResultType;
jmethodID MethodResultJni::s_midReset;

jfieldID MethodResultJni::s_fidBoolean;
jfieldID MethodResultJni::s_fidInteger;
jfieldID MethodResultJni::s_fidDouble;
jfieldID MethodResultJni::s_fidString;
jfieldID MethodResultJni::s_fidList;
jfieldID MethodResultJni::s_fidMap;

jfieldID MethodResultJni::s_fidStrCallback;
jfieldID MethodResultJni::s_fidStrCallbackData;
jfieldID MethodResultJni::s_fidRubyProcCallback;
jfieldID MethodResultJni::s_fidTabId;
jfieldID MethodResultJni::s_fidResultParamName;
jfieldID MethodResultJni::s_fidObjectClassPath;
jfieldID MethodResultJni::s_fidRubyObjectClass;
//----------------------------------------------------------------------------------------------------------------------

JNIEnv* MethodResultJni::jniInit(JNIEnv *env)
{
    RAWTRACE3("%s - env: 0x%.8x, functions: 0x%.8x", __FUNCTION__, env, env->functions);
    if(env && !s_methodResultClass)
    {
        String exceptionMessage;
        s_methodResultClass = getJNIClass(RHODES_JAVA_CLASS_METHODRESULT);
        if(!s_methodResultClass)
        {
            RAWLOG_ERROR1("Failed to find java class: %s", METHOD_RESULT_CLASS);
            s_methodResultClass = 0;
            return NULL;
        }
        RAWTRACE("<init>");

        s_midMethodResult = env->GetMethodID(s_methodResultClass, "<init>", "(Z)V");
        if(env->ExceptionCheck() == JNI_TRUE)
        {
            RAWLOG_ERROR2("Failed to get %s constructor: $s", METHOD_RESULT_CLASS, clearException(env).c_str());
            s_methodResultClass = 0;
            return NULL;
        }
        RAWTRACE("getJson");
        s_midGetJson = env->GetMethodID(s_methodResultClass, "getJson", "()Ljava/lang/String;");
        if(env->ExceptionCheck() == JNI_TRUE)
        {
            RAWLOG_ERROR2("Failed to get method %s.getJson: %s", METHOD_RESULT_CLASS, clearException(env).c_str());
            s_methodResultClass = 0;
            return NULL;
        }
        RAWTRACE("getResultType");
        s_midGetResultType = env->GetMethodID(s_methodResultClass, "getResultType", "()I");
        if(env->ExceptionCheck() == JNI_TRUE)
        {
            RAWLOG_ERROR2("Failed to get method %s.getResultType: %s", METHOD_RESULT_CLASS, clearException(env).c_str());
            s_methodResultClass = 0;
            return NULL;
        }
        RAWTRACE("reset");
        s_midReset = env->GetMethodID(s_methodResultClass, "reset", "()V");
        if(env->ExceptionCheck() == JNI_TRUE)
        {
            RAWLOG_ERROR2("Failed to get method %s.reset: %s", METHOD_RESULT_CLASS, clearException(env).c_str());
            s_methodResultClass = 0;
            return NULL;
        }
        RAWTRACE("mBooleanResult");
        s_fidBoolean = env->GetFieldID(s_methodResultClass, "mBooleanResult", "Z");
        if(env->ExceptionCheck() == JNI_TRUE)
        {
            RAWLOG_ERROR2("Failed to get field %s.mBooleanResult: %s", METHOD_RESULT_CLASS, clearException(env).c_str());
            s_methodResultClass = 0;
            return NULL;
        }
        RAWTRACE("mIntegerResult");
        s_fidInteger = env->GetFieldID(s_methodResultClass, "mIntegerResult", "I");
        if(env->ExceptionCheck() == JNI_TRUE)
        {
            RAWLOG_ERROR2("Failed to get field %s.mIntegerResult: %s", METHOD_RESULT_CLASS, clearException(env).c_str());
            s_methodResultClass = 0;
            return NULL;
        }
        RAWTRACE("mDoubleResult");
        s_fidDouble = env->GetFieldID(s_methodResultClass, "mDoubleResult", "D");
        if(env->ExceptionCheck() == JNI_TRUE)
        {
            RAWLOG_ERROR2("Failed to get fiekd %s.mDoubleResult: %s", METHOD_RESULT_CLASS, clearException(env).c_str());
            s_methodResultClass = 0;
            return NULL;
        }
        RAWTRACE("mStrResult");
        s_fidString = env->GetFieldID(s_methodResultClass, "mStrResult", "Ljava/lang/String;");
        if(env->ExceptionCheck() == JNI_TRUE)
        {
            RAWLOG_ERROR2("Failed to get field %s.mStrResult: %s", METHOD_RESULT_CLASS, clearException(env).c_str());
            s_methodResultClass = 0;
            return NULL;
        }
        RAWTRACE("mListResult");
        s_fidList = env->GetFieldID(s_methodResultClass, "mListResult", "Ljava/util/Collection;");
        if(env->ExceptionCheck() == JNI_TRUE)
        {
            RAWLOG_ERROR2("Failed to get field %s.mListResult: %s", METHOD_RESULT_CLASS, clearException(env).c_str());
            s_methodResultClass = 0;
            return NULL;
        }
        RAWTRACE("mMapResult");
        s_fidMap = env->GetFieldID(s_methodResultClass, "mMapResult", "Ljava/util/Map;");
        if(env->ExceptionCheck() == JNI_TRUE)
        {
            RAWLOG_ERROR2("Failed to get field %s.mMapResult: %s", METHOD_RESULT_CLASS, clearException(env).c_str());
            s_methodResultClass = 0;
            return NULL;
        }
        RAWTRACE("mStrCallback");
        s_fidStrCallback = env->GetFieldID(s_methodResultClass, "mStrCallback", "Ljava/lang/String;");
        if(env->ExceptionCheck() == JNI_TRUE)
        {
            RAWLOG_ERROR2("Failed to get field %s.mStrCallback: %s", METHOD_RESULT_CLASS, clearException(env).c_str());
            s_methodResultClass = 0;
            return NULL;
        }
        RAWTRACE("mStrCallbackData");
        s_fidStrCallbackData = env->GetFieldID(s_methodResultClass, "mStrCallbackData", "Ljava/lang/String;");
        if(env->ExceptionCheck() == JNI_TRUE)
        {
            RAWLOG_ERROR2("Failed to get %s.mStrCallbackData field: %s", METHOD_RESULT_CLASS, clearException(env).c_str());
            s_methodResultClass = 0;
            return NULL;
        }
        RAWTRACE("mResultParamName");
        s_fidResultParamName = env->GetFieldID(s_methodResultClass, "mResultParamName", "Ljava/lang/String;");
        if(env->ExceptionCheck() == JNI_TRUE)
        {
            RAWLOG_ERROR2("Failed to get %s.mResultParamName field: %s", METHOD_RESULT_CLASS, clearException(env).c_str());
            s_methodResultClass = 0;
            return NULL;
        }
        RAWTRACE("mRubyProcCallback");
        s_fidRubyProcCallback = env->GetFieldID(s_methodResultClass, "mRubyProcCallback", "J");
        if(env->ExceptionCheck() == JNI_TRUE)
        {
            RAWLOG_ERROR2("Failed to get field  %s.mRubyProcCallback: %s", METHOD_RESULT_CLASS, clearException(env).c_str());
            s_methodResultClass = 0;
            return NULL;
        }
        RAWTRACE("mTabId");
        s_fidTabId = env->GetFieldID(s_methodResultClass, "mTabId", "I");
        if(env->ExceptionCheck() == JNI_TRUE)
        {
            RAWLOG_ERROR2("Failed to get field  %s.mTabId: %s", METHOD_RESULT_CLASS, clearException(env).c_str());
            s_methodResultClass = 0;
            return NULL;
        }
        RAWTRACE("mObjectClassPath");
        s_fidObjectClassPath = env->GetFieldID(s_methodResultClass, "mObjectClassPath", "Ljava/lang/String;");
        if(env->ExceptionCheck() == JNI_TRUE)
        {
            RAWLOG_ERROR2("Failed to get field %s.mObjectClassPath: %s", METHOD_RESULT_CLASS, clearException(env).c_str());
            s_methodResultClass = 0;
            return NULL;
        }
        RAWTRACE("mRubyObjectClass");
        s_fidRubyObjectClass = env->GetFieldID(s_methodResultClass, "mRubyObjectClass", "J");
        if(env->ExceptionCheck() == JNI_TRUE)
        {
            RAWLOG_ERROR2("Failed to get field %s.mRubyObjectClass: %s", METHOD_RESULT_CLASS, clearException(env).c_str());
            s_methodResultClass = 0;
            return NULL;
        }
        RAWTRACE(__FUNCTION__);

    }
    return env;
}
//----------------------------------------------------------------------------------------------------------------------

jboolean MethodResultJni::getBooleanResult(JNIEnv* env) const
{
    RAWTRACE(__FUNCTION__);
    return env->GetBooleanField(m_jResult, s_fidBoolean);
}
//----------------------------------------------------------------------------------------------------------------------

jint MethodResultJni::getIntegerResult(JNIEnv* env) const
{
    RAWTRACE(__FUNCTION__);
    return env->GetIntField(m_jResult, s_fidInteger);
}
//----------------------------------------------------------------------------------------------------------------------

jdouble MethodResultJni::getDoubleResult(JNIEnv* env) const
{
    RAWTRACE(__FUNCTION__);
    return env->GetDoubleField(m_jResult, s_fidDouble);
}
//----------------------------------------------------------------------------------------------------------------------

jstring MethodResultJni::getStringResult(JNIEnv* env) const
{
    RAWTRACE(__FUNCTION__);
    return static_cast<jstring>(env->GetObjectField(m_jResult, s_fidString));
}
//----------------------------------------------------------------------------------------------------------------------

jobject MethodResultJni::getListResult(JNIEnv* env) const
{
    RAWTRACE(__FUNCTION__);
    return env->GetObjectField(m_jResult, s_fidList);
}
//----------------------------------------------------------------------------------------------------------------------

jobject MethodResultJni::getMapResult(JNIEnv* env) const
{
    RAWTRACE(__FUNCTION__);
    return env->GetObjectField(m_jResult, s_fidMap);
}
//----------------------------------------------------------------------------------------------------------------------

jstring MethodResultJni::getJSONResult(JNIEnv* env) const
{
    RAWTRACE(__FUNCTION__);
    return static_cast<jstring>(env->CallObjectMethod(m_jResult, s_midGetJson));
}
//----------------------------------------------------------------------------------------------------------------------

int MethodResultJni::getResultType(JNIEnv* env) const
{
    RAWTRACE(__FUNCTION__);
    if(m_resType == typeNone && m_jResult != 0)
    {
        m_javaResultType = env->CallIntMethod(m_jResult, s_midGetResultType);
        RAWTRACE1("Java result type: %d", m_javaResultType);

        return m_javaResultType;
    } else
    {
        RAWTRACE1("Native result type: %d", m_resType);
        return m_resType;
    }
}
//----------------------------------------------------------------------------------------------------------------------

rho::String MethodResultJni::getErrorMessage(JNIEnv* env) const
{
    RAWTRACE(__FUNCTION__);
    if(m_resType == typeNone && m_jResult != 0)
    {
        jhstring jhMessage = getStringResult(env);
        return rho_cast<std::string>(env, jhMessage.get());
    } else
    {
        return m_errMsg;
    }
}
//----------------------------------------------------------------------------------------------------------------------

jstring MethodResultJni::getStrCallback(JNIEnv* env) const
{
    RAWTRACE(__FUNCTION__);
    return static_cast<jstring>(env->GetObjectField(m_jResult, s_fidStrCallback));
}
//----------------------------------------------------------------------------------------------------------------------

jstring MethodResultJni::getStrCallbackData(JNIEnv* env) const
{
    RAWTRACE(__FUNCTION__);
    return static_cast<jstring>(env->GetObjectField(m_jResult, s_fidStrCallbackData));
}
//----------------------------------------------------------------------------------------------------------------------

jlong MethodResultJni::getRubyProcCallback(JNIEnv* env) const
{
    RAWTRACE(__FUNCTION__);
    return env->GetLongField(m_jResult, s_fidRubyProcCallback);
}
//----------------------------------------------------------------------------------------------------------------------

jlong MethodResultJni::getRubyObjectClass(JNIEnv* env) const
{
    RAWTRACE(__FUNCTION__);
    return env->GetLongField(m_jResult, s_fidRubyObjectClass);
}
//----------------------------------------------------------------------------------------------------------------------

jstring MethodResultJni::getObjectClassPath(JNIEnv* env) const
{
    RAWTRACE(__FUNCTION__);
    return static_cast<jstring>(env->GetObjectField(m_jResult, s_fidObjectClassPath));
}
//----------------------------------------------------------------------------------------------------------------------

jstring MethodResultJni::getResultParamName(JNIEnv* env) const
{
    RAWTRACE(__FUNCTION__);
    return static_cast<jstring>(env->GetObjectField(m_jResult, s_fidResultParamName));
}
//----------------------------------------------------------------------------------------------------------------------

void MethodResultJni::reset(JNIEnv* env)
{
    m_resType = typeNone;
    env->CallVoidMethod(m_jResult, s_midReset);
}
//----------------------------------------------------------------------------------------------------------------------

MethodResultJni::MethodResultJni(bool isRuby) : m_bhasLocalFrame(false), m_env(0), m_bGlobalRef(false), m_bSlaveRef(false), m_hasCallback(false), m_resType(typeNone), m_javaResultType(0)
{
    m_env = jniInit();
    if (!m_env) {
        RAWLOG_ERROR("ctor - JNI initialization failed");
        return;
    }
    //SPR 27852 fix...We don't need to push or pop localframe.
   /* if(m_env->PushLocalFrame(256) >= 0)
    {
        m_bhasLocalFrame = true;
    }
    */

    m_jResult = m_env->NewObject(s_methodResultClass, s_midMethodResult, static_cast<jboolean>(isRuby));
}
//----------------------------------------------------------------------------------------------------------------------

MethodResultJni::MethodResultJni(JNIEnv* env, jobject jResult) : m_bhasLocalFrame(false), m_bGlobalRef(false), m_bSlaveRef(false), m_hasCallback(false), m_resType(typeNone), m_javaResultType(0), m_jResult(jResult)
{
    m_env = jniInit(env);
    if (!m_env) {
        RAWLOG_ERROR("ctor - JNI initialization failed");
        return;
    }
}
//----------------------------------------------------------------------------------------------------------------------

MethodResultJni::MethodResultJni(const MethodResultJni& result) : m_bhasLocalFrame(false), m_env(0), m_bGlobalRef(false), m_bSlaveRef(false), m_hasCallback(false), m_resType(typeNone), m_javaResultType(0)
{
    JNIEnv* env = result.m_env;

    if(result.m_bGlobalRef) {
        RAWTRACE1("Copying MethodResult with global JNI reference: 0x%.8x ---------------------------------------", result.m_jResult);
        m_jResult = result.m_jResult;
        result.m_bSlaveRef = true;
    } else {
        JNIEnv* env = result.m_env;
        m_jResult = env->NewGlobalRef(result.m_jResult);
        RAWTRACE1("Copying MethodResult creating global JNI reference: 0x%.8x >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>", m_jResult);
    }
    m_bGlobalRef = true;
}
//----------------------------------------------------------------------------------------------------------------------

MethodResultJni::~MethodResultJni()
{
    jniInit();

    //SPR 27852...
    /*if(m_bhasLocalFrame)
    {
        m_env->PopLocalFrame(NULL);
    }
    */

    if(m_bGlobalRef)
    {
        if(!m_bSlaveRef)
        {
            RAWTRACE1("Deleting MethodResult global JNI reference: 0x%.8x ==========================================", m_jResult);
            m_env->DeleteGlobalRef(m_jResult);
        }
    } else {
       // if (!m_bhasLocalFrame) {
            RAWTRACE1("Deleting MethodResult local JNI reference: 0x%.8x ==========================================", m_jResult);
            m_env->DeleteLocalRef(m_jResult);
       //  }
    }
}
//----------------------------------------------------------------------------------------------------------------------

void MethodResultJni::setCallback(JNIEnv* env, jstring jUrl, jstring jData)
{
    RAWTRACE(__FUNCTION__);

    env->SetObjectField(m_jResult, s_fidStrCallback, jUrl);
    env->SetIntField(m_jResult, s_fidTabId, rho_webview_active_tab());
    if(jData)
    {
        env->SetObjectField(m_jResult, s_fidStrCallbackData, jData);
    }
    m_hasCallback = true;

    RAWTRACE("Callback has been set ^^^");
}
//----------------------------------------------------------------------------------------------------------------------

void MethodResultJni::setRubyProcCallback(JNIEnv* env, jlong jRubyProc)
{
    RAWTRACE(__FUNCTION__);

    env->SetLongField(m_jResult, s_fidRubyProcCallback, jRubyProc);

    m_hasCallback = true;

    rho_ruby_holdValue(jRubyProc);

    RAWTRACE("Callback has been set ^^^");
}
//----------------------------------------------------------------------------------------------------------------------

void MethodResultJni::setRubyObjectClass(JNIEnv* env, jlong jClass)
{
    RAWTRACE(__FUNCTION__);

    env->SetLongField(m_jResult, s_fidRubyObjectClass, jClass);

    RAWTRACE("Ruby object class has been set ^^^");
}
//----------------------------------------------------------------------------------------------------------------------

void MethodResultJni::setObjectClassPath(JNIEnv* env, jstring jClassPath)
{
    RAWTRACE(__FUNCTION__);

    env->SetObjectField(m_jResult, s_fidObjectClassPath, jClassPath);

    RAWTRACE("Object class path has been set ^^^");
}
//----------------------------------------------------------------------------------------------------------------------

void MethodResultJni::setResultParamName(JNIEnv* env, jstring jName)
{
    RAWTRACE(__FUNCTION__);

    env->SetObjectField(m_jResult, s_fidResultParamName, jName);

    RAWTRACE("Result param name has been set ^^^");
}
//----------------------------------------------------------------------------------------------------------------------

void MethodResultJni::disconnect(JNIEnv* env)
{
    if(*this)
    {
        if(m_bGlobalRef)
        {
            RAWTRACE("Disconnect from global JNI reference");
            env->DeleteGlobalRef(m_jResult);
            m_bGlobalRef = false;
        } else
        {
            RAWTRACE("Disconnect from local JNI reference");
            env->DeleteLocalRef(m_jResult);
        }
        m_jResult = 0;
    }

}
//----------------------------------------------------------------------------------------------------------------------

void MethodResultJni::callRubyBack(jboolean jReleaseCallback)
{
    RAWTRACE(__FUNCTION__);
    RAWTRACE(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");

    jhstring jhStrCallbackData = getStrCallbackData(m_env);
    jlong jRubyProc = getRubyProcCallback(m_env);

    if (jRubyProc != 0)
    {
        VALUE oProc = static_cast<VALUE>(jRubyProc);
        rho::String strResBody = RHODESAPP().addCallbackObject(new CRubyCallbackResult<MethodResultJni>(*this), "body");

        RAWTRACE1("Call Ruby proc by address: 0x%.8x", oProc);

        RHODESAPP().callCallbackProcWithData( oProc, strResBody, rho_cast<rho::String>(m_env, jhStrCallbackData.get()), true);

        if(static_cast<bool>(jReleaseCallback))
        {
            releaseRubyProcCallback(jRubyProc);
        }
    }
    else
    {
        jhstring jhStrCallback = getStrCallback(m_env);
        String strCallback = rho_cast<String>(m_env, jhStrCallback.get());
        if(strCallback.length())
        {
            RAWTRACE1("Call Ruby controller by URL: %s", rho_cast<rho::String>(jhStrCallback.get()).c_str());

            rho::String strResBody = RHODESAPP().addCallbackObject(new CRubyCallbackResult<MethodResultJni>(*this), "__rho_inline");
            RHODESAPP().callCallbackWithData(rho_cast<rho::String>(m_env, jhStrCallback.get()), strResBody, rho_cast<rho::String>(m_env, jhStrCallbackData.get()), true);
        }
    }
}
//----------------------------------------------------------------------------------------------------------------------

void MethodResultJni::callJSBack(jint jTabIndex)
{
    RAWTRACE(__FUNCTION__);

    jhstring jhStrCallbackID = getStrCallback(m_env);
    String strCallbackID = rho_cast<String>(m_env, jhStrCallbackID.get());

    jhstring jhCallbackData = getStrCallbackData(m_env);
    String strCallbackData = rho_cast<String>(m_env, jhCallbackData.get());

    String strRes(CMethodResultConvertor().toJSON(*this));

    String strCallback("Rho.callbackHandler( \"");
    strCallback += strCallbackID;
    strCallback += "\", {";
    strCallback += strRes;
    strCallback += "},\"";
    strCallback += strCallbackData;
    strCallback += "\")";

    jclass cls = getJNIClass(RHODES_JAVA_CLASS_WEB_VIEW);
    if (!cls) return;
    static jmethodID mid = getJNIClassStaticMethod(m_env, cls, "executeJs", "(Ljava/lang/String;I)V");
    if (!mid) return;

    jhstring jhCallback = rho_cast<jstring>(m_env, strCallback);
    m_env->CallStaticVoidMethod(cls, mid, jhCallback.get(), jTabIndex);
}
//----------------------------------------------------------------------------------------------------------------------

void MethodResultJni::releaseRubyProcCallback(jlong jRubyProc)
{
    RAWTRACE(__FUNCTION__);
    rho_ruby_releaseValue(static_cast<VALUE>(jRubyProc));
}
//----------------------------------------------------------------------------------------------------------------------

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_api_MethodResult_nativeCallBack
  (JNIEnv * env, jobject jResult, jint jTabId, jboolean jIsRuby, jboolean jReleaseCallback)
{
    RAWTRACE3("nativeCallBack - env: 0x%.8x, isRuby: %s, release: %s", env, jIsRuby?"true":"false", jReleaseCallback?"true":"false");

    initjnienv(env);

    MethodResultJni result(env, jResult);
    if (static_cast<bool>(jIsRuby))
    {
        result.callRubyBack(jReleaseCallback);
    } else
    {
        result.callJSBack(jTabId);
    }
}
//----------------------------------------------------------------------------------------------------------------------

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_api_MethodResult_nativeReleaseRubyProcCallback
  (JNIEnv * env, jclass , jlong jProcCallback)
{
    RAWTRACE("nativeReleaseRubyProcCallback");

    MethodResultJni::releaseRubyProcCallback(jProcCallback);
}
//----------------------------------------------------------------------------------------------------------------------


}}
