#include "JNIRhodes.h"

#include <com_rhomobile_rhodes_Logger.h>

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "Logger"

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_Logger_E
  (JNIEnv *env, jclass, jstring tag, jstring msg)
{
    const char *strTag = env->GetStringUTFChars(tag, JNI_FALSE);
    const char *strMsg = env->GetStringUTFChars(msg, JNI_FALSE);
    RAWLOGC_ERROR(strTag, strMsg);
    env->ReleaseStringUTFChars(tag, strTag);
    env->ReleaseStringUTFChars(msg, strMsg);
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_Logger_W
  (JNIEnv *env, jclass, jstring tag, jstring msg)
{
    const char *strTag = env->GetStringUTFChars(tag, JNI_FALSE);
    const char *strMsg = env->GetStringUTFChars(msg, JNI_FALSE);
    RAWLOGC_ERROR(strTag, strMsg);
    env->ReleaseStringUTFChars(tag, strTag);
    env->ReleaseStringUTFChars(msg, strMsg);
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_Logger_I
  (JNIEnv *env, jclass, jstring tag, jstring msg)
{
    const char *strTag = env->GetStringUTFChars(tag, JNI_FALSE);
    const char *strMsg = env->GetStringUTFChars(msg, JNI_FALSE);
    RAWLOGC_INFO(strTag, strMsg);
    env->ReleaseStringUTFChars(tag, strTag);
    env->ReleaseStringUTFChars(msg, strMsg);
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_Logger_D
  (JNIEnv *env, jclass, jstring tag, jstring msg)
{
    const char *strTag = env->GetStringUTFChars(tag, JNI_FALSE);
    const char *strMsg = env->GetStringUTFChars(msg, JNI_FALSE);
    RAWLOGC_INFO(strTag, strMsg);
    env->ReleaseStringUTFChars(tag, strTag);
    env->ReleaseStringUTFChars(msg, strMsg);
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_Logger_T
  (JNIEnv *env, jclass, jstring tag, jstring msg)
{
    const char *strTag = env->GetStringUTFChars(tag, JNI_FALSE);
    const char *strMsg = env->GetStringUTFChars(msg, JNI_FALSE);
    RAWTRACEC(strTag, strMsg);
    env->ReleaseStringUTFChars(tag, strTag);
    env->ReleaseStringUTFChars(msg, strMsg);
}

