// License information is available from LICENSE file

#ifndef __ANDROID_JNI_HELPER_H__
#define __ANDROID_JNI_HELPER_H__

#include <jni.h>
#include <string>

namespace jxcore {

typedef struct JniMethodInfo_ {
  JNIEnv *env;
  jclass classID;
  jmethodID methodID;
} JniMethodInfo;

class JniHelper {
 public:
  static void setJavaVM(JavaVM *javaVM);
  static JavaVM *getJavaVM();
  static JNIEnv *getEnv();

  static bool setClassLoaderFrom(jobject activityInstance);
  static bool getStaticMethodInfo(JniMethodInfo &methodinfo,
                                  const char *className, const char *methodName,
                                  const char *paramCode);
  static bool getMethodInfo(JniMethodInfo &methodinfo, const char *className,
                            const char *methodName, const char *paramCode);

  static jmethodID loadclassMethod_methodID;
  static jobject classloader;

  static JNIEnv *cacheEnv(JavaVM *jvm);

 private:
  static bool getMethodInfo_DefaultClassLoader(JniMethodInfo &methodinfo,
                                               const char *className,
                                               const char *methodName,
                                               const char *paramCode);

  static JavaVM *_psJavaVM;
};
}

#endif  // __ANDROID_JNI_HELPER_H__
