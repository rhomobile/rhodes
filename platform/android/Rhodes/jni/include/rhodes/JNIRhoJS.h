
#ifndef JNIRHOJS_H_INCLUDED
#define JNIRHOJS_H_INCLUDED

#include "json/JSONIterator.h"
#include "rhodes.h"

template <>
struct ArgumentsAdapter<rho::json::CJSONArray>
{
    typedef rho::json::CJSONEntry value_type;
    typedef rho::json::CJSONArray container_type;
    typedef int size_type;
    const container_type& m_args;
    ArgumentsAdapter(const container_type& args) : m_args(args) {}
    ArgumentsAdapter(const ArgumentsAdapter& adapter) : m_args(adapter.m_args) {}
    value_type operator[] (size_type n) const { return const_cast<container_type&>(m_args)[n]; }
    size_type size() const { return const_cast<container_type&>(m_args).getSize(); }
};

namespace details
{

using rho::json::CJSONEntry;
using rho::json::CJSONArray;

//template <>
//struct rho_cast_helper<CJSONEntry, jobject>: public RhoJniConvertor
//{
//    CJSONEntry operator()(JNIEnv *env, jobject obj);
//};
//
//template <>
//struct rho_cast_helper<CJSONArray, jobjectArray>: public RhoJniConvertor
//{
//    CJSONArray operator()(JNIEnv *env, jobjectArray jArr);
//};

template <>
struct rho_cast_helper<jstring, CJSONEntry>: public RhoJniConvertor
{
    jstring operator()(JNIEnv *env, CJSONEntry jsonEntry);
};

template <>
struct rho_cast_helper<jobject, CJSONEntry>: public RhoJniConvertor
{
//    jobject m_jObject;
    jobject operator()(JNIEnv *env, CJSONEntry jsonEntry);
//    jobject convertRubyArrayToJavaCollection(VALUE array);
//    jobject convertRubyHashToJavaMap(VALUE array);
};

template <>
struct rho_cast_helper<jobjectArray, CJSONArray>
{
    jobjectArray operator()(JNIEnv *env, const CJSONArray& jsonArray);
};

}
#endif // JNIRHORUBY_H_INCLUDED
