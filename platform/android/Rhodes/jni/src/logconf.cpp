#include "rhodes/JNIRhodes.h"

#include "rhodes/jni/com_rhomobile_rhodes_RhoLogConf.h"

#include <logging/RhoLogConf.h>

extern "C" int rho_conf_send_log(const char* callback_url);

RHO_GLOBAL jstring JNICALL Java_com_rhomobile_rhodes_RhoLogConf_getEnabledCategories
  (JNIEnv *env, jclass)
{
    return rho_cast<jhstring>(env, LOGCONF().getEnabledCategories().c_str()).release();
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_RhoLogConf_setEnabledCategories
  (JNIEnv *env, jclass, jstring value)
{
    if (value == NULL)
        return;
    LOGCONF().setEnabledCategories(rho_cast<std::string>(env, value).c_str());
}

RHO_GLOBAL jstring JNICALL Java_com_rhomobile_rhodes_RhoLogConf_getDisabledCategories
  (JNIEnv *env, jclass)
{
    return rho_cast<jhstring>(env, LOGCONF().getDisabledCategories().c_str()).release();
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_RhoLogConf_setDisabledCategories
  (JNIEnv *env, jclass, jstring value)
{
    if (value == NULL)
        return;
    LOGCONF().setDisabledCategories(rho_cast<std::string>(env, value).c_str());
}

RHO_GLOBAL jint JNICALL Java_com_rhomobile_rhodes_RhoLogConf_getMinSeverity
  (JNIEnv *, jclass)
{
    return LOGCONF().getMinSeverity();
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_RhoLogConf_setMinSeverity
  (JNIEnv *, jclass, jint severity)
{
    LOGCONF().setMinSeverity(severity);
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_RhoLogConf_saveToFile
  (JNIEnv *, jclass)
{
    LOGCONF().saveToFile();
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_RhoLogConf_clearLog
  (JNIEnv *, jclass)
{
    LOGCONF().clearLog();
}

RHO_GLOBAL jstring JNICALL Java_com_rhomobile_rhodes_RhoLogConf_getLogText
  (JNIEnv *env, jclass)
{
    rho::String logText;
    LOGCONF().getLogText(logText);
    return rho_cast<jhstring>(env, logText.c_str()).release();
}

RHO_GLOBAL jint JNICALL Java_com_rhomobile_rhodes_RhoLogConf_getLogTextPos
  (JNIEnv *, jclass)
{
    return LOGCONF().getLogTextPos();
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_RhoLogConf_sendLog
  (JNIEnv *, jclass)
{
    rho_conf_send_log("");
}

