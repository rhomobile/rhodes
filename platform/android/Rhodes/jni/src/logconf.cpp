#include <com_rhomobile_rhodes_RhoLogConf.h>
#include <logging/RhoLogConf.h>

JNIEXPORT jstring JNICALL Java_com_rhomobile_rhodes_RhoLogConf_getEnabledCategories
  (JNIEnv *env, jobject)
{
    return env->NewStringUTF(LOGCONF().getEnabledCategories().c_str());
}

JNIEXPORT void JNICALL Java_com_rhomobile_rhodes_RhoLogConf_setEnabledCategories
  (JNIEnv *env, jobject, jstring value)
{
    const char *str = env->GetStringUTFChars(value, JNI_FALSE);
    if (str == NULL)
        return;
    LOGCONF().setEnabledCategories(str);
    env->ReleaseStringUTFChars(value, str);
}

JNIEXPORT jstring JNICALL Java_com_rhomobile_rhodes_RhoLogConf_getDisabledCategories
  (JNIEnv *env, jobject)
{
    return env->NewStringUTF(LOGCONF().getDisabledCategories().c_str());
}

JNIEXPORT void JNICALL Java_com_rhomobile_rhodes_RhoLogConf_setDisabledCategories
  (JNIEnv *env, jobject, jstring value)
{
    const char *str = env->GetStringUTFChars(value, JNI_FALSE);
    if (str == NULL)
        return;
    LOGCONF().setDisabledCategories(str);
    env->ReleaseStringUTFChars(value, str);
}

JNIEXPORT jint JNICALL Java_com_rhomobile_rhodes_RhoLogConf_getMinSeverity
  (JNIEnv *, jobject)
{
    return LOGCONF().getMinSeverity();
}

JNIEXPORT void JNICALL Java_com_rhomobile_rhodes_RhoLogConf_setMinSeverity
  (JNIEnv *, jobject, jint severity)
{
    LOGCONF().setMinSeverity(severity);
}

JNIEXPORT void JNICALL Java_com_rhomobile_rhodes_RhoLogConf_saveToFile
  (JNIEnv *, jobject)
{
    LOGCONF().saveToFile();
}

JNIEXPORT void JNICALL Java_com_rhomobile_rhodes_RhoLogConf_clearLog
  (JNIEnv *, jobject)
{
    LOGCONF().clearLog();
}

JNIEXPORT jstring JNICALL Java_com_rhomobile_rhodes_RhoLogConf_getLogText
  (JNIEnv *env, jobject)
{
    rho::String logText;
    LOGCONF().getLogText(logText);
    return env->NewStringUTF(logText.c_str());
}

JNIEXPORT jint JNICALL Java_com_rhomobile_rhodes_RhoLogConf_getLogTextPos
  (JNIEnv *, jobject)
{
    return LOGCONF().getLogTextPos();
}

RHO_GLOBAL void rho_conf_show_log()
{
}

