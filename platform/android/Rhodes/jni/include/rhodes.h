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

template <typename T>
class jholder
{
public:
    jholder(T obj) :m_object(obj) {}

    jholder(jholder const &c)
        :m_object(c.m_object)
    {
        if (m_object)
            jnienv()->NewLocalRef(m_object);
    }

    ~jholder()
    {
        if (m_object)
            jnienv()->DeleteLocalRef(m_object);
    }

    jholder &operator=(jholder const &rhs)
    {
        jholder copy(rhs);
        swap(copy);
        return *this;
    }

    T get() const {return m_object;}

    T release()
    {
        T ret = m_object;
        m_object = NULL;
        return ret;
    }

    bool operator!() const {return !m_object;}

private:
    void swap(jholder &c)
    {
        std::swap(m_object, c.m_object);
    }

private:
    T m_object;
};

typedef jholder<jobject> jhobject;
typedef jholder<jstring> jhstring;

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
struct rho_cast_helper<std::string, jhstring>
{
    std::string operator()(JNIEnv *env, jhstring s) {return rho_cast_helper<std::string, jstring>()(env, s.get());}
};

template <>
struct rho_cast_helper<jhstring, char const *>
{
    jhstring operator()(JNIEnv *env, char const *);
};

template <>
struct rho_cast_helper<jhstring, char *>
{
    jhstring operator()(JNIEnv *env, char *s) {return rho_cast_helper<jhstring, char const *>()(env, s);}
};

template <int N>
struct rho_cast_helper<jhstring, char [N]>
{
    jhstring operator()(JNIEnv *env, char (&s)[N]) {return rho_cast_helper<jhstring, char const *>()(env, &s[0]);}
};

template <>
struct rho_cast_helper<jhstring, std::string>
{
    jhstring operator()(JNIEnv *env, std::string const &s) {return rho_cast_helper<jhstring, char const *>()(env, s.c_str());}
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
