#ifndef RHOCONNECTJNINOTIFY_H_INCLUDED
#define RHOCONNECTJNINOTIFY_H_INCLUDED

#include "rhodes/JNIRhodes.h"

namespace rho { namespace connect_jni {

jobject rhoconnect_jni_parsenotify(JNIEnv * env, const char* res);
jobject rhoconnect_jni_parseobjectnotify(JNIEnv * env, const char* res);

}}

#endif // RHOCONNECTJNINOTIFY_H_INCLUDED
