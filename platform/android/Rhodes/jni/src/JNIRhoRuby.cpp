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

jclass RhoValueConverter::clsHashMap;
jclass RhoValueConverter::clsVector;

jmethodID RhoValueConverter::midHashMapConstructor;
jmethodID RhoValueConverter::midVectorConstructor;
jmethodID RhoValueConverter::midPut;
jmethodID RhoValueConverter::midAddElement;

bool RhoValueConverter::init = false;


RhoValueConverter::RhoValueConverter(JNIEnv *e) : env(e)
{
    if(!init)
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
}

jobject RhoValueConverter::createObject(rho_param *p)
{
    if (!init || !p)
        return 0;

    switch (p->type) {
    case RHO_PARAM_STRING:
        return rho_cast<jstring>(env, p->v.string);
        break;
    case RHO_PARAM_ARRAY:
        {
            jobject v = env->NewObject(clsVector, midVectorConstructor);
            if (!v) return NULL;

            for (int i = 0, lim = p->v.array->size; i < lim; ++i) {
                jhobject value = createObject(p->v.array->value[i]);
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
                jhstring key = rho_cast<jstring>(p->v.hash->name[i]);
                jhobject value = createObject(p->v.hash->value[i]);
                env->CallObjectMethod(v, midPut, key.get(), value.get());
            }
            return v;
        }
        break;
    default:
        return 0;
    }
}

namespace details {

VALUE rho_cast_helper<VALUE, jobject>::convertJavaMapToRubyHash(jobject objMap)
{
    jhobject objSet = m_env->CallObjectMethod(objMap, midMapKeySet);
    if (!objSet) return Qnil;
    jhobject objIterator = m_env->CallObjectMethod(objSet.get(), midSetIterator);
    if (!objIterator) return Qnil;
                                  
    CHoldRubyValue retval(rho_ruby_createHash());
    while(m_env->CallBooleanMethod(objIterator.get(), midIteratorHasNext))
    {
        jhobject jhKey = m_env->CallObjectMethod(objIterator.get(), midIteratorNext);
        if (!jhKey) return Qnil;
        jhobject jhVal = m_env->CallObjectMethod(objMap, midMapGet, jhKey.get());
        if (!jhVal) return Qnil;

        CHoldRubyValue key(rho_cast<VALUE>(m_env, jhKey));
        CHoldRubyValue val(rho_cast<VALUE>(m_env, jhVal));

        rho_ruby_add_to_hash(retval, key, val);
    }
    return retval;
}

VALUE rho_cast_helper<VALUE, jobject>::convertJavaCollectionToRubyArray(jobject jList)
{
    jhobject jhIterator = m_env->CallObjectMethod(jList, midCollectionIterator);
    if (!jhIterator) return Qnil;

    CHoldRubyValue retval(rho_ruby_create_array());
    while(m_env->CallBooleanMethod(jhIterator.get(), midIteratorHasNext))
    {
        jhobject jhVal = m_env->CallObjectMethod(jhIterator.get(), midIteratorNext);
        if (!jhVal) return Qnil;

        CHoldRubyValue val(rho_cast<VALUE>(m_env, jhVal));
        rho_ruby_add_to_array(retval, val);
    }
    return retval;
}

extern "C"
{

static void ruby_array_each(const char* val, void* param)
{
    rho_cast_helper<jobject, VALUE>* pThis = reinterpret_cast<rho_cast_helper<jobject, VALUE>* >(param);
    jhstring jhVal = rho_cast<jstring>(pThis->m_env, val);

    pThis->m_env->CallBooleanMethod(pThis->m_jObject, RhoJniConvertor::midArrayListAdd, jhVal.get());
}

static void ruby_hash_each(const char* key, const char* val, void* param)
{
    rho_cast_helper<jobject, VALUE>* pThis = reinterpret_cast<rho_cast_helper<jobject, VALUE>* >(param);
    jhstring jhKey = rho_cast<jstring>(pThis->m_env, key);
    jhstring jhVal = rho_cast<jstring>(pThis->m_env, val);

    jhobject jhPrev = pThis->m_env->CallObjectMethod(pThis->m_jObject, RhoJniConvertor::midHashMapPut, jhKey.get(), jhVal.get());
}
}

jobject rho_cast_helper<jobject, VALUE>::convertRubyArrayToJavaCollection(VALUE array)
{
    m_jObject = m_env->NewObject(clsArrayList, midArrayList);
    if (!m_jObject) return m_jObject;

    rho_ruby_enum_strary(array, ruby_array_each, this);

    return m_jObject;
}

jobject rho_cast_helper<jobject, VALUE>::convertRubyHashToJavaMap(VALUE hash)
{
    m_jObject = m_env->NewObject(clsHashMap, midHashMap);
    if (!m_jObject) return m_jObject;

    rho_ruby_enum_strhash(hash, ruby_hash_each, this);

    return m_jObject;
}


jobject rho_cast_helper<jobject, VALUE>::operator()(JNIEnv *env, VALUE value)
{
    RAWTRACE("rho_cast<jobject, VALUE>");

    if (NIL_P(value))
        return 0;

    if (!initConvertor(env))
        return 0;

    switch(TYPE(value))
    {
    case T_STRING:
        return env->NewStringUTF(RSTRING_PTR(value));
    case T_ARRAY:
        return convertRubyArrayToJavaCollection(value);
    case T_HASH:
        return convertRubyHashToJavaMap(value);
    }

    RAWLOG_ERROR("rho_cast<jobject, VALUE>: wrong type of VALUE");
    return 0;
}

VALUE rho_cast_helper<VALUE, jobject>::operator()(JNIEnv *env, jobject obj)
{
    RAWTRACE("rho_cast<VALUE, jobject>");

    if(env->IsSameObject(obj, NULL) == JNI_TRUE)
        return Qnil;

    if(!initConvertor(env))
    {
        env->ThrowNew(getJNIClass(RHODES_JAVA_CLASS_RUNTIME_EXCEPTION), "Java <=> Ruby conversion initialization failed");
        return Qnil;
    }

    if(env->IsInstanceOf(obj, clsString))
    {
        const char *str = env->GetStringUTFChars(static_cast<jstring>(obj), JNI_FALSE);
        VALUE res = rho_ruby_create_string(str);
        env->ReleaseStringUTFChars(static_cast<jstring>(obj), str);
        return res;
    }
    else
    if(env->IsInstanceOf(obj, clsCollection))
        return convertJavaCollectionToRubyArray(obj);
    else
    if(env->IsInstanceOf(obj, clsMap))
        return convertJavaMapToRubyHash(obj);

    RAWLOG_ERROR("rho_cast<VALUE, jobject>: unknown type of value");
    return Qnil;
}

VALUE rho_cast_helper<VALUE, jstring>::operator()(JNIEnv *env, jstring jStr)
{
    RAWTRACE("rho_cast<VALUE, jstring>");

    if(env->IsSameObject(jStr, NULL) == JNI_TRUE)
        return Qnil;

    const char *str = env->GetStringUTFChars(jStr, JNI_FALSE);
    VALUE res = rho_ruby_create_string(str);
    env->ReleaseStringUTFChars(jStr, str);

    return res;
}

jstring rho_cast_helper<jstring, VALUE>::operator()(JNIEnv *env, VALUE value)
{
    RAWTRACE("rho_cast<jstring, VALUE>");

    if (NIL_P(value))
        return 0;

    return env->NewStringUTF(RSTRING_PTR(value));
}

jobjectArray rho_cast_helper<jobjectArray, VALUE>::operator()(JNIEnv *env, VALUE value)
{
    RAWTRACE("rho_cast<jobjectArray, VALUE>");

    if (NIL_P(value))
        return 0;

    if(TYPE(value) == T_ARRAY)
    {
        //TODO: implement cast
    }

    RAWLOG_ERROR("rho_cast<jobjectArray, VALUE>: wrong type of VALUE");
    return 0;
}

}
