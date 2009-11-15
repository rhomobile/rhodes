#include "JNIRhodes.h"

#include <com_rhomobile_rhodes_Logger.h>

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "Logger"

JNIEXPORT void JNICALL Java_com_rhomobile_rhodes_Logger_E
  (JNIEnv *, jclass, jstring tag, jstring msg)
{
    JNIEnv *env = jnienv();
    const char *strTag = env->GetStringUTFChars(tag, JNI_FALSE);
    const char *strMsg = env->GetStringUTFChars(msg, JNI_FALSE);
    RAWLOGC_ERROR(strTag, strMsg);
    env->ReleaseStringUTFChars(tag, strTag);
    env->ReleaseStringUTFChars(msg, strMsg);
}

JNIEXPORT void JNICALL Java_com_rhomobile_rhodes_Logger_W
  (JNIEnv *, jclass, jstring tag, jstring msg)
{
    JNIEnv *env = jnienv();
    const char *strTag = env->GetStringUTFChars(tag, JNI_FALSE);
    const char *strMsg = env->GetStringUTFChars(msg, JNI_FALSE);
    RAWLOGC_ERROR(strTag, strMsg);
    env->ReleaseStringUTFChars(tag, strTag);
    env->ReleaseStringUTFChars(msg, strMsg);
}

JNIEXPORT void JNICALL Java_com_rhomobile_rhodes_Logger_I
  (JNIEnv *, jclass, jstring tag, jstring msg)
{
    JNIEnv *env = jnienv();
    const char *strTag = env->GetStringUTFChars(tag, JNI_FALSE);
    const char *strMsg = env->GetStringUTFChars(msg, JNI_FALSE);
    RAWLOGC_INFO(strTag, strMsg);
    env->ReleaseStringUTFChars(tag, strTag);
    env->ReleaseStringUTFChars(msg, strMsg);
}

JNIEXPORT void JNICALL Java_com_rhomobile_rhodes_Logger_D
  (JNIEnv *, jclass, jstring tag, jstring msg)
{
    JNIEnv *env = jnienv();
    const char *strTag = env->GetStringUTFChars(tag, JNI_FALSE);
    const char *strMsg = env->GetStringUTFChars(msg, JNI_FALSE);
    RAWLOGC_INFO(strTag, strMsg);
    env->ReleaseStringUTFChars(tag, strTag);
    env->ReleaseStringUTFChars(msg, strMsg);
}

JNIEXPORT void JNICALL Java_com_rhomobile_rhodes_Logger_T
  (JNIEnv *, jclass, jstring tag, jstring msg)
{
    JNIEnv *env = jnienv();
    const char *strTag = env->GetStringUTFChars(tag, JNI_FALSE);
    const char *strMsg = env->GetStringUTFChars(msg, JNI_FALSE);
    RAWTRACEC(strTag, strMsg);
    env->ReleaseStringUTFChars(tag, strTag);
    env->ReleaseStringUTFChars(msg, strMsg);
}

