#ifndef RHO_JNI_RHODES_57d3a700b706402190ead97fd1383bee
#define RHO_JNI_RHODES_57d3a700b706402190ead97fd1383bee

#include <assert.h>
#include <stdlib.h>

#include <rhodes.h>

#include <ruby.h>
#include <ruby/ext/rho/rhoruby.h>

#include <common/RhoDefs.h>
#include <logging/RhoLogConf.h>
#include <logging/RhoLog.h>

#include <genconfig.h>

JavaVM *jvm();
void store_thr_jnienv(JNIEnv *env);

enum rho_java_class_t {
#define RHODES_DEFINE_JAVA_CLASS(x, name) x,
#include <rhodes/details/rhojava.inc>
#undef RHODES_DEFINE_JAVA_CLASS
};

extern const char *rho_java_class[];

jclass getJNIClass(int n);
jclass getJNIObjectClass(JNIEnv *env, jobject obj);
jfieldID getJNIClassField(JNIEnv *env, jclass cls, const char *name, const char *signature);
jfieldID getJNIClassStaticField(JNIEnv *env, jclass cls, const char *name, const char *signature);
jmethodID getJNIClassMethod(JNIEnv *env, jclass cls, const char *name, const char *signature);
jmethodID getJNIClassStaticMethod(JNIEnv *env, jclass cls, const char *name, const char *signature);

VALUE convertJavaMapToRubyHash(jobject objMap);

#define RHO_NOT_IMPLEMENTED RAWLOG_ERROR3("WARNING: Call not implemented function: \"%s\" (defined here: %s:%d)", __PRETTY_FUNCTION__, __FILE__, __LINE__)

//#define RHO_LOG_CALLBACK RAWLOG_INFO1("Callback \"%s\" called", __PRETTY_FUNCTION__)
//#define RHO_LOG_JNI_CALL RAWLOG_INFO1("JNI method \"%s\" called", __PRETTY_FUNCTION__)

class RhoValueConverter
{
private:
    jclass clsHashMap;
    jclass clsVector;

    jmethodID midHashMapConstructor;
    jmethodID midVectorConstructor;
    jmethodID midPut;
    jmethodID midAddElement;

    JNIEnv *env;
    bool init;

public:
    RhoValueConverter(JNIEnv *e);

    jobject createObject(rho_param *p);
};

#endif // RHO_JNI_RHODES_57d3a700b706402190ead97fd1383bee

