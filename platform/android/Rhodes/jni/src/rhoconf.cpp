#include "rhodes/jni/com_rhomobile_rhodes_RhoConf.h"

#include "rhodes/JNIRhodes.h"

#include <common/RhoConf.h>

RHO_GLOBAL jstring JNICALL Java_com_rhomobile_rhodes_RhoConf_getString
  (JNIEnv *, jclass, jstring name)
{
    return rho_cast<jstring>(RHOCONF().getString(rho_cast<std::string>(name).c_str()).c_str());
}

RHO_GLOBAL jint JNICALL Java_com_rhomobile_rhodes_RhoConf_getInt
  (JNIEnv *, jclass, jstring name)
{
    return RHOCONF().getInt(rho_cast<std::string>(name).c_str());
}

RHO_GLOBAL jboolean JNICALL Java_com_rhomobile_rhodes_RhoConf_getBool
  (JNIEnv *, jclass, jstring name)
{
    return RHOCONF().getBool(rho_cast<std::string>(name).c_str());
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_RhoConf_setString
  (JNIEnv *, jclass, jstring name, jstring value)
{
    RHOCONF().setString(rho_cast<std::string>(name).c_str(), rho_cast<std::string>(value));
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_RhoConf_setInt
  (JNIEnv *, jclass, jstring name, jint value)
{
    RHOCONF().setInt(rho_cast<std::string>(name).c_str(), value);
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_RhoConf_setBoolean
  (JNIEnv *, jclass, jstring name, jboolean value)
{
    RHOCONF().setBool(rho_cast<std::string>(name).c_str(), value);
}

RHO_GLOBAL jboolean JNICALL Java_com_rhomobile_rhodes_RhoConf_isExist
  (JNIEnv *, jclass, jstring name)
{
    return RHOCONF().isExist(rho_cast<std::string>(name).c_str());
}

