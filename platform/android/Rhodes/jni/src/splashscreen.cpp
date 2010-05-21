#include "rhodes/jni/com_rhomobile_rhodes_SplashScreen.h"

#include "rhodes/JNIRhodes.h"

#include <common/RhodesApp.h>

namespace rc = rho::common;

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_SplashScreen_nativeStart
  (JNIEnv *, jobject)
{
    rc::CSplashScreen &splashScreen = RHODESAPP().getSplashScreen();
    splashScreen.start();
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_SplashScreen_nativeHide
  (JNIEnv *, jobject)
{
    rc::CSplashScreen &splashScreen = RHODESAPP().getSplashScreen();
    splashScreen.hide();
}
