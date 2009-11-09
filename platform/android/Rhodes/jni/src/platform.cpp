#include <com_rhomobile_rhodes_Platform.h>

#include <logging/RhoLogConf.h>
#include <common/RhodesApp.h>

const char* rho_native_rhopath()
{
  // TODO:
  return "/sdcard/rhomobile/Rhodes";
}

void startRhodesApp()
{
  std::string strRootPath = rho_native_rhopath();
  rho_logconf_Init(strRootPath.c_str());
  rho::common::CRhodesApp::Create(strRootPath );
  RHODESAPP().startApp();
}

JNIEXPORT void JNICALL Java_com_rhomobile_rhodes_Platform_startRhodesApp
  (JNIEnv *, jobject)
{
  startRhodesApp();
}

