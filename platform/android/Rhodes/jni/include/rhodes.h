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
#include <memory>

JNIEnv *jnienv();

jclass rho_find_class(JNIEnv *env, const char *c);

extern "C" void *rho_nativethread_start();
extern "C" void rho_nativethread_end(void *);

std::string const &rho_root_path();
std::string const &rho_root_path_real();
std::string const &rho_apk_path();
std::string rho_cur_path();

template <typename T>
class jholder
{
    jholder<T>& operator=(const jholder<T>& );
public:
    jholder(T obj) : m_object(obj) {}

    ~jholder()
    {
        if (m_object)
            jnienv()->DeleteLocalRef(m_object);
    }

    jholder& operator=(T obj)
    {
        m_object = obj;
        return *this;
    }

    T get() { return m_object; }
    const T get() const { return m_object; }

    T release()
    {
        T ret = m_object;
        m_object = NULL;
        return ret;
    }

    void deleteRef()
    {
        if (m_object)
            jnienv()->DeleteLocalRef(m_object);
        m_object = 0;
    }

    operator bool () const { return m_object != 0; }
    bool operator!() const { return !m_object; }

private:
    T m_object;
};

template <>
class jholder<jboolean>
{
    typedef jboolean T;
    T m_value;
public:
    jholder() : m_value(JNI_FALSE) {}
    jholder(T val) : m_value(val) {}
    jholder& operator=(T val)
    {
        m_value = val;
        return *this;
    }
    T get() const { return m_value; }
    T release() { return m_value; }
    operator bool () const { return true; }
    bool operator! () const { return false; }
};

template <>
class jholder<jdouble>
{
    typedef jdouble T;
    T m_value;
public:
    jholder() : m_value(0.0) {}
    jholder(T val) : m_value(val) {}
    jholder& operator=(T val)
    {
        m_value = val;
        return *this;
    }
    T get() const { return m_value; }
    T release() { return m_value; }
    operator bool () const { return true; }
    bool operator! () const { return false; }
};

template <>
class jholder<jint>
{
    typedef jint T;
    T m_value;
public:
    jholder() : m_value(0) {}
    jholder(T val) : m_value(val) {}
    jholder& operator=(T val)
    {
        m_value = val;
        return *this;
    }
    T get() const { return m_value; }
    T release() { return m_value; }
    operator bool () const { return true; }
    bool operator! () const { return false; }
};

typedef jholder<jobject> jhobject;
typedef jholder<jstring> jhstring;
typedef jholder<jobjectArray> jharray;

namespace rho {
    template <typename K, typename V> class Hashtable;
    template <typename T> class Vector;
}

typedef std::auto_ptr<rho::Vector<std::string> > HStringVector;
typedef std::auto_ptr<rho::Hashtable<std::string, std::string> > HStringMap;

template <typename T, typename U> T rho_cast(JNIEnv *env, U u);
template <typename T, typename U> T rho_cast(U u);
template <typename T, typename U> T rho_cast(JNIEnv *env, U keys, U vals);
template <typename T, typename U> T rho_cast(U keys, U vals);

template <typename T>
struct ArgumentsAdapter;

template <typename T>
ArgumentsAdapter<T> argumentsAdapter(const T& arguments) {
    return ArgumentsAdapter<T>(arguments);
}

template <typename T>
struct PropertyMapConvertor;

template <typename T>
jobject convertToPropertyMap(JNIEnv *env, T value) {
    PropertyMapConvertor<T> convertor;
    return convertor.convertToPropertyMap(env, value);
}

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
    jstring operator()(JNIEnv *env, char *s) { return rho_cast_helper<jstring, char const *>()(env, s);}
};

template <int N>
struct rho_cast_helper<jstring, char [N]>
{
    jstring operator()(JNIEnv *env, char (&s)[N]) { return rho_cast_helper<jstring, char const *>()(env, &s[0]);}
};

template <>
struct rho_cast_helper<jstring, std::string>
{
    jstring operator()(JNIEnv *env, std::string const &s) { return rho_cast_helper<jstring, char const *>()(env, s.c_str());}
};

template <>
struct rho_cast_helper<jboolean, bool>
{
    jboolean operator()(JNIEnv *env, bool arg) { return static_cast<jboolean>(arg); }
};

template <>
struct rho_cast_helper<jint, int>
{
    jint operator()(JNIEnv *env, int arg) { return static_cast<jint>(arg); }
};

template <>
struct rho_cast_helper<jdouble, double>
{
    jdouble operator()(JNIEnv *env, double arg) { return static_cast<jdouble>(arg); }
};

struct RhoJniConvertor
{
    static jclass clsBoolean;
    static jclass clsInteger;
    static jclass clsDouble;
    static jclass clsString;
    static jclass clsCollection;
    static jclass clsMap;
    static jclass clsSet;
    static jclass clsHashMap;
    static jclass clsArrayList;
    static jclass clsIterator;
    static jmethodID midCollectionIterator;
    static jmethodID midMapGet;
    static jmethodID midMapKeySet;
    static jmethodID midArrayList;
    static jmethodID midArrayListAdd;
    static jmethodID midHashMap;
    static jmethodID midHashMapPut;
    static jmethodID midSetIterator;
    static jmethodID midIteratorHasNext;
    static jmethodID midIteratorNext;
    static jmethodID midBooleanValue;
    static jmethodID midBooleanValueOf;
    static jmethodID midIntValue;
    static jmethodID midInteger;
    static jmethodID midDoubleValue;
    static jmethodID midDoubleValueOf;

    RhoJniConvertor() : m_env(0) {}
    bool initConvertor(JNIEnv *env);

    JNIEnv* m_env;

    jobject getBooleanObject(bool val);
    jobject getIntegerObject(int val);
    jobject getDoubleObject(double val);
};

template <>
struct rho_cast_helper<HStringMap, jobject>: public RhoJniConvertor
{
    typedef HStringMap value_type;
    value_type operator()(JNIEnv *env, jobject jObj);
};

template <>
struct rho_cast_helper<HStringMap, jobjectArray>
{
    typedef HStringMap value_type;
    value_type operator()(JNIEnv *env, jobjectArray jKeys, jobjectArray jVals);
};

template <>
struct rho_cast_helper<HStringVector, jobjectArray>
{
    typedef HStringVector value_type;
    value_type operator()(JNIEnv *env, jobjectArray jArr);
};

template <>
struct rho_cast_helper<HStringVector, jobject>: public RhoJniConvertor
{
    typedef HStringVector value_type;
    value_type operator()(JNIEnv *env, jobject jList);
};

template <>
struct rho_cast_helper<jobject, rho::Vector<std::string> >: public RhoJniConvertor
{
    jobject operator()(JNIEnv* env, const rho::Vector<std::string>& stringVector);
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

template <typename T, typename U>
T rho_cast(JNIEnv *env, U keys, U vals)
{
    return details::rho_cast_helper<T, U>()(env, keys, vals);
}

template <typename T, typename U>
T rho_cast(U keys, U vals)
{
    return details::rho_cast_helper<T, U>()(jnienv(), keys, vals);
}

template <typename T, typename U>
T rho_cast(JNIEnv *env, const jholder<U>& u)
{
    return details::rho_cast_helper<T, U>()(env, u.get());
}

template <typename T, typename U>
T rho_cast(const jholder<U>& u)
{
    return details::rho_cast_helper<T, U>()(jnienv(), u.get());
}

template <typename T, typename U>
T rho_cast(JNIEnv *env, const jholder<U>& keys, const jholder<U>& vals)
{
    return details::rho_cast_helper<T, U>()(env, keys.get(), vals.get());
}

template <typename T, typename U>
T rho_cast(const jholder<U>& keys, const jholder<U>& vals)
{
    return details::rho_cast_helper<T, U>()(jnienv(), keys.get(), vals.get());
}

#endif // _RHODES_H_D9BFC7B4FD394BECAF7EC535453253CC
