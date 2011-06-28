#ifndef JNIRHORUBY_H_INCLUDED
#define JNIRHORUBY_H_INCLUDED

#include <ruby.h>
#include <ruby/ext/rho/rhoruby.h>

#include <rhodes.h>

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

namespace details
{

template <>
struct rho_cast_helper<VALUE, jobject>
{
    VALUE operator()(JNIEnv *env, jobject obj);
};

template <>
struct rho_cast_helper<jobject, VALUE>
{
    jobject operator()(JNIEnv *env, VALUE value);
};

}
#endif // JNIRHORUBY_H_INCLUDED
