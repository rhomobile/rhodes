
#include "sync/SyncThread.h"
#include "RhoConnectClient/RhoConnectClient.h"

#include "RhoConnectJniNotify.h"

namespace rho { namespace connect_jni {

jhobject rhoconnect_jni_parsenotify(JNIEnv * env, const char* res)
{
    RHO_CONNECT_NOTIFY notify;
    memset(&notify, 0, sizeof(notify));

    rho_connectclient_parsenotify(res, &notify);

    static jclass clsNotify = getJNIClass(RHOCONNECT_JAVA_CLASS_NOTIFY);
    if (!clsNotify) return NULL;

    static jmethodID midNotify = getJNIClassMethod(env, clsNotify, "<init>", "()V");
    if (!midNotify) return NULL;
    static jfieldID fidErrorCode = getJNIClassField(env, clsNotify, "mErrorCode", "I");
    if (!fidErrorCode) return NULL;

    jhobject jhNotify = jhobject(env->NewObject(clsNotify, midNotify));
    if (!jhNotify) return NULL;

    env->SetIntField(jhNotify.get(), fidErrorCode, notify.error_code);

    rho_connectclient_free_syncnotify(&notify);

    return jhNotify;

}

}}
