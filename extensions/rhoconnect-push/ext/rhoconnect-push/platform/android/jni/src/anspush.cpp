#include "rhodes/JNIRhodes.h"
#include "logging/RhoLog.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "anspushJNI"

#define ANS_FACADE "com.rhomobile.rhoelements.ans.ANSFacade"

RHO_GLOBAL void anspush_install_service() {

    RAWTRACE("anspush_install_service()");
//    JNIEnv *env = jnienv();
//    jclass cls = rho_find_class(env, ANS_FACADE);
//    if (!cls) return;
//
//    jmethodID mid = env->GetStaticMethodID(cls, "installService", "()V");
//    if (!mid) return;
//
//    env->CallStaticObjectMethod(cls, mid);
}

RHO_GLOBAL void anspush_ans_register(const char* username, const char* password, const char* session) {
    RAWTRACE3("anspush_ans_register(%s, %s, %s)", username, password, session);

    JNIEnv *env = jnienv();
    //jclass cls = env->FindClass(ANS_FACADE);
    jclass cls = rho_find_class(env, ANS_FACADE);
    if (!cls) return;

    jmethodID mid = env->GetStaticMethodID(cls, "register", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
    if (!mid) return;

    jhstring jUser = rho_cast<jstring>(env, username);
    jhstring jPass = rho_cast<jstring>(env, password);
    jhstring jSession = rho_cast<jstring>(env, session);

    env->CallStaticVoidMethod(cls, mid, jUser.get(), jPass.get(), jSession.get());
}

RHO_GLOBAL void anspush_ans_unregister(const char* session) {
    RAWTRACE1("anspush_ans_unregister(%s)", session);

    JNIEnv *env = jnienv();
    //jclass cls = env->FindClass(ANS_FACADE);
    jclass cls = rho_find_class(env, ANS_FACADE);
    if (!cls) return;

    jmethodID mid = env->GetStaticMethodID(cls, "unregister", "(Ljava/lang/String;)V");
    if (!mid) return;

    jhstring jSession = rho_cast<jstring>(env, session);

    env->CallStaticVoidMethod(cls, mid, jSession.get());
}

RHO_GLOBAL void anspush_check_ans_registration(const char* session){
    RAWTRACE1("anspush_check_ans_registration(%s)", session);

    JNIEnv *env = jnienv();
    //jclass cls = env->FindClass(ANS_FACADE);
    jclass cls = rho_find_class(env, ANS_FACADE);
    if (!cls) return;

    jmethodID mid = env->GetStaticMethodID(cls, "checkRegistration", "(Ljava/lang/String;)V");
    if (!mid) return;

    jhstring jSession = rho_cast<jstring>(env, session);

    env->CallStaticVoidMethod(cls, mid, jSession.get());
}

