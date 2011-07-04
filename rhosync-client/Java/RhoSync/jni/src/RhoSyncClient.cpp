#include "rhodes/JNIRhodes.h"
#include "sync/SyncThread.h"

#include "com_rhomobile_rhosync_RhoSyncClient.h"

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhosync_RhoSyncClient_setSyncServer
  (JNIEnv *, jobject, jstring)
{
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhosync_RhoSyncClient_setThreadedMode
  (JNIEnv *, jobject, jboolean jmode)
{
    bool mode = jmode;
    rho_sync_set_threaded_mode(mode ? 1 : 0);
}

//RHO_GLOBAL jboolean JNICALL Java_com_rhomobile_rhosync_RhoSyncClient_getThreadedMode
//  (JNIEnv *, jobject)
//{
//    return (jboolean)(rho_sync_get_threaded_mode() != 0);
//}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhosync_RhoSyncClient_setPollInterval
  (JNIEnv *, jobject, jint time)
{
    rho_sync_set_pollinterval(time);
}

RHO_GLOBAL jint JNICALL Java_com_rhomobile_rhosync_RhoSyncClient_getPollInterval
  (JNIEnv *, jobject)
{
    return (jint)rho_sync_get_pollinterval();
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhosync_RhoSyncClient_setBulkSyncState
  (JNIEnv *, jobject, jint)
{
}

RHO_GLOBAL jint JNICALL Java_com_rhomobile_rhosync_RhoSyncClient_getBulkSyncState
  (JNIEnv *, jobject)
{
    return (jint)0;
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhosync_RhoSyncClient_setConfigString
  (JNIEnv *, jobject, jstring, jstring)
{
}

RHO_GLOBAL jstring JNICALL Java_com_rhomobile_rhosync_RhoSyncClient_getConfigString
  (JNIEnv * env, jobject, jstring)
{
    const char* cs = "";
    return rho_cast<jhstring>(env, cs).release();
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhosync_RhoSyncClient_initDatabase
  (JNIEnv *, jobject)
{
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhosync_RhoSyncClient_addModels
  (JNIEnv *, jobject, jobjectArray)
{
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhosync_RhoSyncClient_databaseFullResetAndLogout
  (JNIEnv *, jobject)
{
}

RHO_GLOBAL jboolean JNICALL Java_com_rhomobile_rhosync_RhoSyncClient_isLoggedIn
  (JNIEnv *, jobject)
{
    return (jboolean)false;
}

RHO_GLOBAL jobject JNICALL Java_com_rhomobile_rhosync_RhoSyncClient_loginWithUser
  (JNIEnv *, jobject, jstring, jstring)
{
    return (jobject)NULL;
}

RHO_GLOBAL jobject JNICALL Java_com_rhomobile_rhosync_RhoSyncClient_syncAll
  (JNIEnv *, jobject)
{
    return (jobject)NULL;
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhosync_RhoSyncClient_nativeInit
  (JNIEnv * env, jclass, jstring root, jstring sqliteJournals)
{
    android_set_path(rho_cast<rho::String>(root), rho_cast<rho::String>(sqliteJournals));
    android_setup(env);
}
