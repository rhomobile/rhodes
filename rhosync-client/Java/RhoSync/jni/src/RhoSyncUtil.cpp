#include <android/native_activity.h>

#include "rhodes/JNIRhodes.h"

RHO_GLOBAL void delete_files_in_folder(const char *szFolderPath)
{
//    JNIEnv *env = jnienv();
//    jclass cls = getJNIClass(RHODES_JAVA_CLASS_RHODES_SERVICE);
//    if (!cls) return;
//    jmethodID mid = getJNIClassStaticMethod(env, cls, "deleteFilesInFolder", "(Ljava/lang/String;)V");
//    if (!mid) return;
//    jhstring objFolderPath = rho_cast<jhstring>(szFolderPath);
//    env->CallStaticVoidMethod(cls, mid, objFolderPath.get());
}

RHO_GLOBAL rho::String rho_sysimpl_get_phone_id()
{
    return "";
}

RHO_GLOBAL void rho_free_callbackdata(void* pData)
{
}

RHO_GLOBAL int rho_net_ping_network(const char* szHost)
{
    return 1;
}
