#include "JNIRhodes.h"

#include <com_rhomobile_rhodes_RhoLogConf.h>

#include <common/RhodesApp.h>

RHO_GLOBAL jstring JNICALL Java_com_rhomobile_rhodes_RhoLogConf_getEnabledCategories
  (JNIEnv *env, jobject)
{
    return env->NewStringUTF(LOGCONF().getEnabledCategories().c_str());
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_RhoLogConf_setEnabledCategories
  (JNIEnv *env, jobject, jstring value)
{
    const char *str = env->GetStringUTFChars(value, JNI_FALSE);
    if (str == NULL)
        return;
    LOGCONF().setEnabledCategories(str);
    env->ReleaseStringUTFChars(value, str);
}

RHO_GLOBAL jstring JNICALL Java_com_rhomobile_rhodes_RhoLogConf_getDisabledCategories
  (JNIEnv *env, jobject)
{
    return env->NewStringUTF(LOGCONF().getDisabledCategories().c_str());
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_RhoLogConf_setDisabledCategories
  (JNIEnv *env, jobject, jstring value)
{
    const char *str = env->GetStringUTFChars(value, JNI_FALSE);
    if (str == NULL)
        return;
    LOGCONF().setDisabledCategories(str);
    env->ReleaseStringUTFChars(value, str);
}

RHO_GLOBAL jint JNICALL Java_com_rhomobile_rhodes_RhoLogConf_getMinSeverity
  (JNIEnv *, jobject)
{
    return LOGCONF().getMinSeverity();
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_RhoLogConf_setMinSeverity
  (JNIEnv *, jobject, jint severity)
{
    LOGCONF().setMinSeverity(severity);
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_RhoLogConf_saveToFile
  (JNIEnv *, jobject)
{
    LOGCONF().saveToFile();
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_RhoLogConf_clearLog
  (JNIEnv *, jobject)
{
    LOGCONF().clearLog();
}

RHO_GLOBAL jstring JNICALL Java_com_rhomobile_rhodes_RhoLogConf_getLogText
  (JNIEnv *env, jobject)
{
    rho::String logText;
    LOGCONF().getLogText(logText);
    return env->NewStringUTF(logText.c_str());
}

RHO_GLOBAL jint JNICALL Java_com_rhomobile_rhodes_RhoLogConf_getLogTextPos
  (JNIEnv *, jobject)
{
    return LOGCONF().getLogTextPos();
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_RhoLogConf_sendLog
  (JNIEnv *, jobject)
{
    rho_conf_send_log();
}

RHO_GLOBAL void rho_conf_show_log()
{
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_RHODES);
    if (!cls) return;
    jmethodID mid = getJNIClassStaticMethod(cls, "showLogView", "()V");
    if (!mid) return;

    jnienv()->CallStaticVoidMethod(cls, mid);
}

