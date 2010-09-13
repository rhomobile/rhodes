#ifndef _RHODES_H_D9BFC7B4FD394BECAF7EC535453253CC
#define _RHODES_H_D9BFC7B4FD394BECAF7EC535453253CC

#include <jni.h>
#include <string>

JNIEnv *jnienv();

jclass rho_find_class(JNIEnv *env, const char *c);

extern "C" void *rho_nativethread_start();
extern "C" void rho_nativethread_end(void *);

std::string const &rho_root_path();
std::string const &rho_apk_path();

namespace details
{

template <typename T, typename U>
struct rho_cast_helper;

template <>
struct rho_cast_helper<std::string, jstring>
{
    std::string operator()(JNIEnv *env, jstring );
};

template <>
struct rho_cast_helper<jstring, char const *>
{
    jstring operator()(JNIEnv *env, char const *);
};

template <>
struct rho_cast_helper<jstring, char *>
{
    jstring operator()(JNIEnv *env, char *s) {return rho_cast_helper<jstring, char const *>()(env, s);}
};

template <int N>
struct rho_cast_helper<jstring, char [N]>
{
    jstring operator()(JNIEnv *env, char (&s)[N]) {return rho_cast_helper<jstring, char const *>()(env, &s[0]);}
};

template <>
struct rho_cast_helper<jstring, std::string>
{
    jstring operator()(JNIEnv *env, std::string const &s) {return rho_cast_helper<jstring, char const *>()(env, s.c_str());}
};

} // namespace details

template <typename T, typename U>
T rho_cast(JNIEnv *env, U u)
{
    return details::rho_cast_helper<T, U>()(env, u);
}

template <typename T, typename U>
T rho_cast(U u)
{
    return details::rho_cast_helper<T, U>()(jnienv(), u);
}

#endif // _RHODES_H_D9BFC7B4FD394BECAF7EC535453253CC
