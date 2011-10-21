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

#include "rhodes/JNIRhodes.h"
#include "rhodes/JNIRhoRuby.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "JNIRhoRuby"


RhoValueConverter::RhoValueConverter(JNIEnv *e)
    :env(e), init(false)
{
    clsHashMap = getJNIClass(RHODES_JAVA_CLASS_HASHMAP);
    if (!clsHashMap) return;
    clsVector = getJNIClass(RHODES_JAVA_CLASS_VECTOR);
    if (!clsVector) return;
    midHashMapConstructor = getJNIClassMethod(env, clsHashMap, "<init>", "()V");
    if (!midHashMapConstructor) return;
    midVectorConstructor = getJNIClassMethod(env, clsVector, "<init>", "()V");
    if (!midVectorConstructor) return;
    midPut = getJNIClassMethod(env, clsHashMap, "put", "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");
    if (!midPut) return;
    midAddElement = getJNIClassMethod(env, clsVector, "addElement", "(Ljava/lang/Object;)V");
    if (!midAddElement) return;
    init = true;
}

jobject RhoValueConverter::createObject(rho_param *p)
{
    if (!init || !p) return NULL;
    switch (p->type) {
    case RHO_PARAM_STRING:
        return rho_cast<jhstring>(p->v.string).release();
        break;
    case RHO_PARAM_ARRAY:
        {
            jobject v = env->NewObject(clsVector, midVectorConstructor);
            if (!v) return NULL;

            for (int i = 0, lim = p->v.array->size; i < lim; ++i) {
                jhobject value = jhobject(createObject(p->v.array->value[i]));
                env->CallVoidMethod(v, midAddElement, value.get());
            }
            return v;
        }
        break;
    case RHO_PARAM_HASH:
        {
            jobject v = env->NewObject(clsHashMap, midHashMapConstructor);
            if (!v) return NULL;

            for (int i = 0, lim = p->v.hash->size; i < lim; ++i) {
                jhstring key = rho_cast<jhstring>(p->v.hash->name[i]);
                jhobject value = jhobject(createObject(p->v.hash->value[i]));
                env->CallObjectMethod(v, midPut, key.get(), value.get());
            }
            return v;
        }
        break;
    default:
        return NULL;
    }
}

namespace details {

static rho::common::CMutex rho_cast_java_ruby_mtx;

static jclass clsString;
static jclass clsMap;
static jclass clsSet;
static jclass clsIterator;

static jmethodID midMapGet;
static jmethodID midMapKeySet;
static jmethodID midSetIterator;
static jmethodID midIteratorHasNext;
static jmethodID midIteratorNext;

static bool rho_cast_java_ruby_init(JNIEnv *env)
{
    static rho::common::CMutex rho_fd_mtx;
    static bool initialized = false;
    if (initialized)
        return true;

    rho::common::CMutexLock guard(rho_cast_java_ruby_mtx);
    if (initialized)
        return true;

    clsString = getJNIClass(RHODES_JAVA_CLASS_STRING);
    if (!clsString) return false;
    clsMap = getJNIClass(RHODES_JAVA_CLASS_MAP);
    if (!clsMap) return false;
    clsSet = getJNIClass(RHODES_JAVA_CLASS_SET);
    if (!clsSet) return false;
    clsIterator = getJNIClass(RHODES_JAVA_CLASS_ITERATOR);
    if (!clsIterator) return false;

    midMapGet = getJNIClassMethod(env, clsMap, "get", "(Ljava/lang/Object;)Ljava/lang/Object;");
    if (!midMapGet) return false;
    midMapKeySet = getJNIClassMethod(env, clsMap, "keySet", "()Ljava/util/Set;");
    if (!midMapKeySet) return false;
    midSetIterator = getJNIClassMethod(env, clsSet, "iterator", "()Ljava/util/Iterator;");
    if (!midSetIterator) return false;
    midIteratorHasNext = getJNIClassMethod(env, clsIterator, "hasNext", "()Z");
    if (!midIteratorHasNext) return false;
    midIteratorNext = getJNIClassMethod(env, clsIterator, "next", "()Ljava/lang/Object;");
    if (!midIteratorNext) return false;

    initialized = true;
    return true;
}


static VALUE convertJavaMapToRubyHash(JNIEnv *env, jobject objMap)
{
    jobject objSet = env->CallObjectMethod(objMap, midMapKeySet);
    if (!objSet) return Qnil;
    jobject objIterator = env->CallObjectMethod(objSet, midSetIterator);
    if (!objIterator) return Qnil;

    CHoldRubyValue retval(rho_ruby_createHash());
    while(env->CallBooleanMethod(objIterator, midIteratorHasNext))
    {
        jstring objKey = (jstring)env->CallObjectMethod(objIterator, midIteratorNext);
        if (!objKey) return Qnil;
        jstring objValue = (jstring)env->CallObjectMethod(objMap, midMapGet, objKey);
        if (!objValue) return Qnil;

        std::string const &strKey = rho_cast<std::string>(objKey);
        std::string const &strValue = rho_cast<std::string>(objValue);
        addStrToHash(retval, strKey.c_str(), strValue.c_str());

        env->DeleteLocalRef(objKey);
        env->DeleteLocalRef(objValue);
    }
    return retval;
}

jobject rho_cast_helper<jobject, VALUE>::operator()(JNIEnv *env, VALUE value)
{
    if (!rho_cast_java_ruby_init(env))
    {
        env->ThrowNew(getJNIClass(RHODES_JAVA_CLASS_RUNTIME_EXCEPTION), "Java <=> Ruby conversion initialization failed");
        return NULL;
    }

    if (NIL_P(value))
        return NULL;

    if (TYPE(value) == T_STRING)
        return env->NewStringUTF(RSTRING_PTR(value));

    RAWLOG_ERROR("rho_cast<jobject, VALUE>: unknown type of value");
    return NULL;
}

VALUE rho_cast_helper<VALUE, jobject>::operator()(JNIEnv *env, jobject obj)
{
    if (!rho_cast_java_ruby_init(env))
    {
        env->ThrowNew(getJNIClass(RHODES_JAVA_CLASS_RUNTIME_EXCEPTION), "Java <=> Ruby conversion initialization failed");
        return Qnil;
    }

    if (!obj)
        return Qnil;

    if (env->IsInstanceOf(obj, clsString))
        return rho_ruby_create_string(rho_cast<std::string>(env, (jstring)obj).c_str());

    if (env->IsInstanceOf(obj, clsMap))
        return convertJavaMapToRubyHash(env, obj);

    RAWLOG_ERROR("rho_cast<VALUE, jobject>: unknown type of value");
    return Qnil;
}

}
