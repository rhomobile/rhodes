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

extern "C"
{
    static void rubyHashElementToProperty(const char* key, VALUE valElement, void* data)
    {
        PropertyMapConvertor<VALUE>* pThis = reinterpret_cast<PropertyMapConvertor<VALUE>* >(data);
        jhstring jhKey = rho_cast<jstring>(pThis->m_env, key);
        jhobject jhVal = rho_cast<jstring>(pThis->m_env, rb_funcall(valElement, rb_intern("to_s"), 0));

        jhobject jhPrev = pThis->m_env->CallObjectMethod(pThis->m_jObject, details::RhoJniConvertor::midHashMapPut, jhKey.get(), jhVal.get());
    }
}

jobject PropertyMapConvertor<VALUE>::convertToPropertyMap(JNIEnv *env, VALUE value)
{
    if (NIL_P(value))
        return 0;

    if (!initConvertor(env))
        return 0;

    m_jObject = env->NewObject(clsHashMap, midHashMap);
    if(env->ExceptionCheck() == JNI_TRUE)
    {
        rho::String message = rho::common::clearException(env);
        RAWLOG_ERROR(message.c_str());
        return 0;
    }

    rho_ruby_enum_hash(value, rubyHashElementToProperty, this);

    return m_jObject;
}

namespace details {

VALUE rho_cast_helper<VALUE, jobject>::convertJavaMapToRubyHash(jobject objMap)
{
    jhobject objSet = m_env->CallObjectMethod(objMap, midMapKeySet);
    if(m_env->ExceptionCheck() == JNI_TRUE)
    {
        rho::String message = rho::common::clearException(m_env);
        RAWLOG_ERROR(message.c_str());
        return Qnil;
    }
    jhobject objIterator = m_env->CallObjectMethod(objSet.get(), midSetIterator);
    if(m_env->ExceptionCheck() == JNI_TRUE)
    {
        rho::String message = rho::common::clearException(m_env);
        RAWLOG_ERROR(message.c_str());
        return Qnil;
    }
                                  
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
    if(m_env->ExceptionCheck() == JNI_TRUE)
    {
        rho::String message = rho::common::clearException(m_env);
        RAWLOG_ERROR(message.c_str());
        return Qnil;
    }

    CHoldRubyValue retval(rho_ruby_create_array());
    while(m_env->CallBooleanMethod(jhIterator.get(), midIteratorHasNext))
    {
        jhobject jhVal = m_env->CallObjectMethod(jhIterator.get(), midIteratorNext);
        if(m_env->ExceptionCheck() == JNI_TRUE)
        {
            rho::String message = rho::common::clearException(m_env);
            RAWLOG_ERROR(message.c_str());
            return Qnil;
        }

        CHoldRubyValue val(rho_cast<VALUE>(m_env, jhVal));
        rho_ruby_add_to_array(retval, val);
    }
    return retval;
}

VALUE rho_cast_helper<VALUE, jobject>::getBoolean(jobject jBoolean)
{
    jboolean jRes = m_env->CallBooleanMethod(jBoolean, midBooleanValue);
    return static_cast<bool>(jRes) ? Qtrue : Qfalse;
}

VALUE rho_cast_helper<VALUE, jobject>::getInteger(jobject jInteger)
{
    jint jRes = m_env->CallIntMethod(jInteger, midIntValue);
    return rho_ruby_create_integer(jRes);
}

VALUE rho_cast_helper<VALUE, jobject>::getDouble(jobject jDouble)
{
    jboolean jRes = m_env->CallDoubleMethod(jDouble, midDoubleValue);
    return rho_ruby_create_double(jRes);
}

extern "C"
{

static void ruby_array_each(VALUE val, void* param)
{
    rho_cast_helper<jobject, VALUE>* pThis = reinterpret_cast<rho_cast_helper<jobject, VALUE>* >(param);
    jhobject jhVal = rho_cast<jobject>(pThis->m_env, val);

    pThis->m_env->CallBooleanMethod(pThis->m_jObject, RhoJniConvertor::midArrayListAdd, jhVal.get());
}

static void ruby_hash_each(const char* key, VALUE val, void* param)
{
    rho_cast_helper<jobject, VALUE>* pThis = reinterpret_cast<rho_cast_helper<jobject, VALUE>* >(param);
    jhstring jhKey = rho_cast<jstring>(pThis->m_env, key);
    jhobject jhVal = rho_cast<jobject>(pThis->m_env, val);

    jhobject jhPrev = pThis->m_env->CallObjectMethod(pThis->m_jObject, RhoJniConvertor::midHashMapPut, jhKey.get(), jhVal.get());
}
}

jobject rho_cast_helper<jobject, VALUE>::convertRubyArrayToJavaCollection(VALUE array)
{
    m_jObject = m_env->NewObject(clsArrayList, midArrayList);
    if(m_env->ExceptionCheck() == JNI_TRUE)
    {
        rho::String message = rho::common::clearException(m_env);
        RAWLOG_ERROR(message.c_str());
        return 0;
    }

    rho_ruby_enum_ary(array, ruby_array_each, this);

    return m_jObject;
}

jobject rho_cast_helper<jobject, VALUE>::convertRubyHashToJavaMap(VALUE hash)
{
    m_jObject = m_env->NewObject(clsHashMap, midHashMap);
    if(m_env->ExceptionCheck() == JNI_TRUE)
    {
        rho::String message = rho::common::clearException(m_env);
        RAWLOG_ERROR(message.c_str());
        return 0;
    }

    rho_ruby_enum_hash(hash, ruby_hash_each, this);

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
    case RUBY_T_SYMBOL:
        value = rb_funcall(value, rb_intern("to_s"), 0);
    case RUBY_T_STRING:
        RAWTRACE("Convert to String object");
        return rho_cast<jstring>((const char*)RSTRING_PTR(value));
    case RUBY_T_ARRAY:
        RAWTRACE("Convert to Collection object");
        return convertRubyArrayToJavaCollection(value);
    case RUBY_T_HASH:
        RAWTRACE("Convert to Map object");
        return convertRubyHashToJavaMap(value);
    case RUBY_T_TRUE:
        RAWTRACE("Convert to TRUE Boolean obeject");
        return RhoJniConvertor::getBooleanObject(true);
    case RUBY_T_FALSE:
        RAWTRACE("Convert to FALSE Boolean object");
        return RhoJniConvertor::getBooleanObject(false);
    case RUBY_T_FIXNUM:
    case RUBY_T_BIGNUM:
        RAWTRACE("Convert to Integer object");
        return RhoJniConvertor::getIntegerObject(NUM2LONG(value));
    case RUBY_T_FLOAT:
    case RUBY_T_RATIONAL:
        RAWTRACE("Convert to Double object");
        return RhoJniConvertor::getDoubleObject(NUM2DBL(value));
    }

    RAWLOG_ERROR1("rho_cast<jobject, VALUE>: wrong type of VALUE: %d", TYPE(value));
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
    if(env->IsInstanceOf(obj, clsBoolean))
        return getBoolean(obj);
    else
    if(env->IsInstanceOf(obj, clsInteger))
        return getInteger(obj);
    else
    if(env->IsInstanceOf(obj, clsDouble))
        return getDouble(obj);
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

    return rho_cast_helper<jstring,const char*>()(env,(const char*)RSTRING_PTR(value));
}

jobjectArray rho_cast_helper<jobjectArray, VALUE>::operator()(JNIEnv *env, VALUE value)
{
    RAWTRACE("rho_cast<jobjectArray, VALUE>");

    if (!initConvertor(env))
        return 0;

    if (NIL_P(value))
        return 0;

    if(TYPE(value) == RUBY_T_ARRAY)
    {
        int size = RARRAY_LEN(value);
        jobjectArray jArray = env->NewObjectArray(size, clsString, 0);

        if(env->ExceptionCheck() == JNI_TRUE)
        {
            rho::String message = rho::common::clearException(env);
            RAWLOG_ERROR(message.c_str());
            return 0;
        }

        for (int i = 0; i < size; ++i)
        {
            jhstring jhElement = rho_cast<jstring>(env, rb_ary_entry(value, i));
            env->SetObjectArrayElement(jArray, i, jhElement.get());
        }
        return jArray;
    }

    RAWLOG_ERROR1("rho_cast<jobjectArray, VALUE>: wrong type of VALUE: %d", TYPE(value));
    return 0;
}

jboolean rho_cast_helper<jboolean, VALUE>::operator()(JNIEnv *env, VALUE value)
{
    RAWTRACE("rho_cast<jboolean, VALUE>");

    if (NIL_P(value))
        return 0;

    if(TYPE(value) == RUBY_T_TRUE)
    {
        return static_cast<jboolean>(true);
    }
    if(TYPE(value) == RUBY_T_FALSE)
    {
        return static_cast<jboolean>(false);
    }
    if(TYPE(value) == RUBY_T_FIXNUM)
    {
        return static_cast<jboolean>(rho_ruby_get_int(value) != 0);
    }
    RAWLOG_ERROR1("rho_cast<jboolean, VALUE>: wrong type of VALUE: %d", TYPE(value));
    return 0;
}

jint rho_cast_helper<jint, VALUE>::operator()(JNIEnv *env, VALUE value)
{
    RAWTRACE("rho_cast<jint, VALUE>");

    if (NIL_P(value))
        return 0;

    if(TYPE(value) == RUBY_T_FIXNUM || TYPE(value) == RUBY_T_BIGNUM)
    {
        return static_cast<jint>(NUM2LONG(value));
    }

    RAWLOG_ERROR1("rho_cast<jint, VALUE>: wrong type of VALUE: %d", TYPE(value));
    return 0;
}

jdouble rho_cast_helper<jdouble, VALUE>::operator()(JNIEnv *env, VALUE value)
{
    RAWTRACE("rho_cast<jdouble, VALUE>");

    if (NIL_P(value))
        return 0;

    if(TYPE(value) == RUBY_T_FLOAT ||
       TYPE(value) == RUBY_T_FIXNUM ||
       TYPE(value) == RUBY_T_BIGNUM ||
       TYPE(value) == RUBY_T_RATIONAL)
    {
        return static_cast<jdouble>(NUM2DBL(value));
    }

    RAWLOG_ERROR1("rho_cast<jdouble, VALUE>: wrong type of VALUE: %d", TYPE(value));
    return 0;
}

VALUE rho_cast_helper<VALUE, jboolean>::operator ()(JNIEnv *env, jboolean jValue)
{
    return rho_ruby_create_boolean(static_cast<bool>(jValue));
}

VALUE rho_cast_helper<VALUE, jint>::operator ()(JNIEnv *env, jint jValue)
{
    return rho_ruby_create_integer(static_cast<int>(jValue));
}

VALUE rho_cast_helper<VALUE, jdouble>::operator ()(JNIEnv *env, jdouble jValue)
{
    return rho_ruby_create_double(static_cast<double>(jValue));
}

}
