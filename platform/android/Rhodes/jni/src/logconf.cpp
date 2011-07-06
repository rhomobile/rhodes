#include "rhodes/JNIRhodes.h"

#include "rhodes/jni/com_rhomobile_rhodes_RhoLogConf.h"

#include <logging/RhoLogConf.h>

extern "C" int rho_conf_send_log();

RHO_GLOBAL jstring JNICALL Java_com_rhomobile_rhodes_RhoLogConf_getEnabledCategories
  (JNIEnv *env, jobject)
{
    return rho_cast<jhstring>(env, LOGCONF().getEnabledCategories().c_str()).release();
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_RhoLogConf_setEnabledCategories
  (JNIEnv *env, jobject, jstring value)
{
    if (value == NULL)
        return;
    LOGCONF().setEnabledCategories(rho_cast<std::string>(env, value).c_str());
}

RHO_GLOBAL jstring JNICALL Java_com_rhomobile_rhodes_RhoLogConf_getDisabledCategories
  (JNIEnv *env, jobject)
{
    return rho_cast<jhstring>(env, LOGCONF().getDisabledCategories().c_str()).release();
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_RhoLogConf_setDisabledCategories
  (JNIEnv *env, jobject, jstring value)
{
    if (value == NULL)
        return;
    LOGCONF().setDisabledCategories(rho_cast<std::string>(env, value).c_str());
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
    return rho_cast<jhstring>(env, logText.c_str()).release();
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

