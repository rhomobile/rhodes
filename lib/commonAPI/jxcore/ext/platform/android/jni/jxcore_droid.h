// License information is available from LICENSE file

#ifndef JXCORE_H
#define JXCORE_H
#include "JniHelper.h"
#include "jx.h"

#include <android/log.h>

#define ALOG_TAG "jxcore-app-log"
#define log_console(...) \
  __android_log_print(ANDROID_LOG_DEBUG, ALOG_TAG, __VA_ARGS__)
#define warn_console(...) \
  __android_log_print(ANDROID_LOG_WARN, ALOG_TAG, __VA_ARGS__)
#define error_console(...) \
  __android_log_print(ANDROID_LOG_ERROR, ALOG_TAG, __VA_ARGS__)

namespace jxcore {

void Callback(long id) {
  JniMethodInfo t;
  if (JniHelper::getStaticMethodInfo(t, "io/jxcore/node/jxcore",
                                     "callback", "(J)V")) {
    jvalue jv;
    jv.j = id;
    t.env->CallStaticVoidMethod(t.classID, t.methodID, jv);
    t.env->DeleteLocalRef(t.classID);
  }
}

void CallJava(jobject arr) {
  JniMethodInfo t;
  if (JniHelper::getStaticMethodInfo(t, "io/jxcore/node/jxcore",
                                  "javaCall", "(Ljava/util/ArrayList;)V")) {

    t.env->CallStaticVoidMethod(t.classID, t.methodID, arr);
    t.env->DeleteLocalRef(t.classID);
  }
}

void OnThread(const int attached) {
  if (attached == 0) {
    JniHelper::getEnv();  // attach current thread
  } else {
    JniHelper::getJavaVM()->DetachCurrentThread();
  }
}

int Initialize(JavaVM *vm) {
  JniHelper::setJavaVM(vm);

  return JNI_VERSION_1_4;
}

}
#endif
