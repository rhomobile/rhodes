#include <jni/com_rhomobile_rhodes_SplashScreen.h>

#include "JNIRhodes.h"

#include <common/RhodesApp.h>

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_SplashScreen_nativeStart
  (JNIEnv *, jobject)
{
    rho::common::CSplashScreen &splashScreen = RHODESAPP().getSplashScreen();
    splashScreen.start();
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_SplashScreen_nativeHide
  (JNIEnv *, jobject)
{
    rho::common::CSplashScreen &splashScreen = RHODESAPP().getSplashScreen();
    splashScreen.hide();
}
