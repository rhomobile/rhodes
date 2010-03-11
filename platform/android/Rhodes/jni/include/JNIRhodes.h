#ifndef RHO_JNI_RHODES_57d3a700b706402190ead97fd1383bee
#define RHO_JNI_RHODES_57d3a700b706402190ead97fd1383bee

#include <assert.h>
#include <stdlib.h>

#include <ruby.h>
#include <ruby/ext/rho/rhoruby.h>

#include <jni.h>

#include <common/RhoDefs.h>
#include <logging/RhoLogConf.h>
#include <logging/RhoLog.h>

#include "gapikey.h"

JavaVM *jvm();
void store_thr_jnienv(JNIEnv *env);
JNIEnv *jnienv();

enum rho_java_class_t {
#define RHODES_DEFINE_JAVA_CLASS(x, name) x,
#include <details/rhojava.inc>
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

namespace details
{

template <typename T, typename U>
struct rho_cast_helper;

template <>
struct rho_cast_helper<std::string, jstring>
{
    std::string operator()(jstring );
};

template <>
struct rho_cast_helper<jstring, char const *>
{
    jstring operator()(char const *);
};

template <>
struct rho_cast_helper<jstring, char *>
{
    jstring operator()(char *s) {return rho_cast_helper<jstring, char const *>()(s);}
};

template <int N>
struct rho_cast_helper<jstring, char [N]>
{
    jstring operator()(char (&s)[N]) {return rho_cast_helper<jstring, char const *>()(&s[0]);}
};

template <>
struct rho_cast_helper<jstring, std::string>
{
    jstring operator()(std::string const &s) {return rho_cast_helper<jstring, char const *>()(s.c_str());}
};

} // namespace details
template <typename T, typename U>
T rho_cast(U u)
{
    return details::rho_cast_helper<T, U>()(u);
}

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

