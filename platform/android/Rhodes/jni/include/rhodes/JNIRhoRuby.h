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

#ifndef JNIRHORUBY_H_INCLUDED
#define JNIRHORUBY_H_INCLUDED

#include "ruby.h"
#include "ruby/ext/rho/rhoruby.h"

#include <vector>

#include <rhodes.h>

class RhoValueConverter
{
private:
    static jclass clsHashMap;
    static jclass clsVector;

    static jmethodID midHashMapConstructor;
    static jmethodID midVectorConstructor;
    static jmethodID midPut;
    static jmethodID midAddElement;

    static bool init;

    JNIEnv* env;

public:
    RhoValueConverter(JNIEnv *e);

    jobject createObject(rho_param *p);
};

template <>
struct ArgumentsAdapter<std::vector<VALUE> >
{
    typedef VALUE value_type;
    typedef std::vector<value_type> container_type;
    typedef container_type::const_reference const_reference;
    typedef container_type::size_type size_type;
    const container_type& m_args;
    ArgumentsAdapter(const container_type& args) : m_args(args) {}
    ArgumentsAdapter(const ArgumentsAdapter& adapter) : m_args(adapter.m_args) {}
    const_reference operator[] (size_type n) const { return m_args[n]; }
    size_type size() const { return m_args.size(); }
};

namespace details
{

template <>
struct rho_cast_helper<VALUE, jobject>: public RhoJniConvertor
{
    VALUE operator()(JNIEnv *env, jobject obj);
    VALUE convertJavaMapToRubyHash(jobject jMap);
    VALUE convertJavaCollectionToRubyArray(jobject jList);
    VALUE getBoolean(jobject jBoolean);
    VALUE getInteger(jobject jInteger);
    VALUE getDouble(jobject jDouble);
};

template <>
struct rho_cast_helper<VALUE, jobjectArray>: public RhoJniConvertor
{
    VALUE operator()(JNIEnv *env, jobjectArray jArr);
};

template <>
struct rho_cast_helper<VALUE, jstring>
{
    VALUE operator()(JNIEnv *env, jstring obj);
};

template <>
struct rho_cast_helper<jobject, VALUE>: public RhoJniConvertor
{
    jobject m_jObject;
    jobject operator()(JNIEnv *env, VALUE value);
    jobject convertRubyArrayToJavaCollection(VALUE array);
    jobject convertRubyHashToJavaMap(VALUE array);
};

template <>
struct rho_cast_helper<jstring, VALUE>
{
    jstring operator()(JNIEnv *env, VALUE value);
};

template <>
struct rho_cast_helper<jobjectArray, VALUE>: public RhoJniConvertor
{
    jobjectArray operator()(JNIEnv *env, VALUE value);
};

template <>
struct rho_cast_helper<jboolean, VALUE>
{
    jboolean operator()(JNIEnv *env, VALUE value);
};

template <>
struct rho_cast_helper<jint, VALUE>
{
    jint operator()(JNIEnv *env, VALUE value);
};

template <>
struct rho_cast_helper<jdouble, VALUE>
{
    jdouble operator()(JNIEnv *env, VALUE value);
};

template <>
struct rho_cast_helper<VALUE, jboolean>
{
    VALUE operator()(JNIEnv *env, jboolean jValue);
};

template <>
struct rho_cast_helper<VALUE, jint>
{
    VALUE operator()(JNIEnv *env, jint jValue);
};

template <>
struct rho_cast_helper<VALUE, jdouble>
{
    VALUE operator()(JNIEnv *env, jdouble jValue);
};


}
#endif // JNIRHORUBY_H_INCLUDED
