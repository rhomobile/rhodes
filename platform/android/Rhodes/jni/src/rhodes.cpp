#include <com_rhomobile_rhodes_Platform.h>

#include <logging/RhoLogConf.h>
#include <common/RhodesApp.h>

const char* rho_native_rhopath()
{
    // TODO:
    return "/sdcard/rhomobile/JNI";
}

void startRhodesApp()
{
    //const char* szRootPath = rho_native_rhopath();
    //rho_logconf_Init(szRootPath);
    //rho_rhodesapp_create(szRootPath);
    //rho_rhodesapp_start();
}

void stopRhodesApp()
{
    //rho_rhodesapp_destroy();
}

JNIEXPORT void JNICALL Java_com_rhomobile_rhodes_Platform_startRhodesApp
    (JNIEnv *, jobject)
{
    startRhodesApp();
}

JNIEXPORT void JNICALL Java_com_rhomobile_rhodes_Platform_stopRhodesApp
    (JNIEnv *, jobject)
{
    stopRhodesApp();
}

