/*------------------------------------------------------------------------
* (The MIT License)
* 
* Copyright (c) 2008-2011 Rhomobile, Inc.
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
* 
* http://rhomobile.com
*------------------------------------------------------------------------*/

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
std::string rho_cur_path();

template <typename T>
class jholder
{
public:
    jholder(T obj) :m_object(obj) {}

    jholder(jholder const &c)
        : m_object(NULL)
    {
        if (c.m_object)
            m_object = static_cast<T>(jnienv()->NewLocalRef(c.m_object));
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
