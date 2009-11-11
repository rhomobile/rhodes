#ifndef RHO_JNI_RHODES_57d3a700b706402190ead97fd1383bee
#define RHO_JNI_RHODES_57d3a700b706402190ead97fd1383bee

#include <assert.h>
#include <stdlib.h>

#include <ruby.h>

#include <jni.h>

#include <common/RhoDefs.h>
#include <logging/RhoLogConf.h>

JNIEnv *jnienv();
jclass getJNIClass(const char *name);
jmethodID getJNIClassMethod(jclass cls, const char *name, const char *signature);

#endif // RHO_JNI_RHODES_57d3a700b706402190ead97fd1383bee

