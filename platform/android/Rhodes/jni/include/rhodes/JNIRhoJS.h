
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

template <>
struct PropertyMapConvertor<rho::json::CJSONEntry>: public details::RhoJniConvertor
{
    jobject convertToPropertyMap(JNIEnv *env, const rho::json::CJSONEntry& entry);
};


namespace details
{

using rho::json::CJSONEntry;

template <>
struct rho_cast_helper<jstring, CJSONEntry>
{
    jstring operator()(JNIEnv *env, const CJSONEntry& jsonEntry);
};

template <>
struct rho_cast_helper<jobject, CJSONEntry>: public RhoJniConvertor
{
    jobject operator()(JNIEnv *env, const CJSONEntry& jsonEntry);
    jobject convertJsonEntryToJavaCollection(const CJSONEntry& entry);
    jobject convertJsonEntryToJavaMap(const CJSONEntry& entry);
};

template <>
struct rho_cast_helper<jobjectArray, CJSONEntry>: public RhoJniConvertor
{
    jobjectArray operator()(JNIEnv *env, const CJSONEntry& entry);
};

template <>
struct rho_cast_helper<jboolean, CJSONEntry>
{
    jboolean operator()(JNIEnv *env, const CJSONEntry& entry);
};

template <>
struct rho_cast_helper<jint, CJSONEntry>
{
    jint operator()(JNIEnv *env, const CJSONEntry& entry);
};

template <>
struct rho_cast_helper<jdouble, CJSONEntry>
{
    jdouble operator()(JNIEnv *env, const CJSONEntry& entry);
};

}
#endif // JNIRHORUBY_H_INCLUDED
