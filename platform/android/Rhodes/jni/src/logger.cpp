#include "rhodes/JNIRhodes.h"

#include "rhodes/jni/com_rhomobile_rhodes_Logger.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "Logger"

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_Logger_E
  (JNIEnv *env, jclass, jstring tag, jstring msg)
{
    RAWLOGC_ERROR(rho_cast<std::string>(env, tag).c_str(), rho_cast<std::string>(env, msg).c_str());
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_Logger_W
  (JNIEnv *env, jclass, jstring tag, jstring msg)
{
    RAWLOGC_ERROR(rho_cast<std::string>(env, tag).c_str(), rho_cast<std::string>(env, msg).c_str());
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_Logger_I
  (JNIEnv *env, jclass, jstring tag, jstring msg)
{
    RAWLOGC_INFO(rho_cast<std::string>(env, tag).c_str(), rho_cast<std::string>(env, msg).c_str());
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_Logger_D
  (JNIEnv *env, jclass, jstring tag, jstring msg)
{
    RAWLOGC_INFO(rho_cast<std::string>(env, tag).c_str(), rho_cast<std::string>(env, msg).c_str());
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_Logger_T
  (JNIEnv *env, jclass, jstring tag, jstring msg)
{
    RAWTRACEC(rho_cast<std::string>(env, tag).c_str(), rho_cast<std::string>(env, msg).c_str());
}

