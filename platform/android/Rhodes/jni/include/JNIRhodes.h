#ifndef RHO_JNI_RHODES_57d3a700b706402190ead97fd1383bee
#define RHO_JNI_RHODES_57d3a700b706402190ead97fd1383bee

#include <assert.h>
#include <stdlib.h>

#include <ruby.h>

#include <jni.h>

#include <common/RhoDefs.h>
#include <logging/RhoLogConf.h>
#include <logging/RhoLog.h>

JavaVM *jvm();
void store_thr_jnienv(JNIEnv *env);
JNIEnv *jnienv();
jclass getJNIClass(const char *name);
jclass getJNIObjectClass(jobject obj);
jmethodID getJNIClassMethod(jclass cls, const char *name, const char *signature);
jmethodID getJNIClassStaticMethod(jclass cls, const char *name, const char *signature);

#define RHODES_JAVA_CLASS_RHODES "com/rhomobile/rhodes/Rhodes"
#define RHODES_JAVA_CLASS_WEB_VIEW "com/rhomobile/rhodes/WebView"

#define RHO_NOT_IMPLEMENTED RAWLOG_ERROR3("WARNING: Call not implemented function: \"%s\" (defined here: %s:%d)", __PRETTY_FUNCTION__, __FILE__, __LINE__)

#define RHO_LOG_CALLBACK RAWLOG_INFO1("Callback \"%s\" called", __PRETTY_FUNCTION__)
#define RHO_LOG_JNI_CALL RAWLOG_INFO1("JNI method \"%s\" called", __PRETTY_FUNCTION__)

#endif // RHO_JNI_RHODES_57d3a700b706402190ead97fd1383bee

