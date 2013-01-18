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

jhobject RhoValueConverter::createObject(rho_param *p)
{
    if (!init || !p)
        return jhobject(0);

    switch (p->type) {
    case RHO_PARAM_STRING:
        return rho_cast<jhstring>(env, p->v.string);
        break;
    case RHO_PARAM_ARRAY:
        {
            jhobject v = env->NewObject(clsVector, midVectorConstructor);
            if (!v) return NULL;

            for (int i = 0, lim = p->v.array->size; i < lim; ++i) {
                jhobject value = jhobject(createObject(p->v.array->value[i]));
                env->CallVoidMethod(v.get(), midAddElement, value.get());
            }
            return v;
        }
        break;
    case RHO_PARAM_HASH:
        {
            jhobject v = env->NewObject(clsHashMap, midHashMapConstructor);
            if (!v) return NULL;

            for (int i = 0, lim = p->v.hash->size; i < lim; ++i) {
                jhstring key = rho_cast<jhstring>(p->v.hash->name[i]);
                jhobject value = jhobject(createObject(p->v.hash->value[i]));
                env->CallObjectMethod(v.get(), midPut, key.get(), value.get());
            }
            return v;
        }
        break;
    default:
        return NULL;
    }
}

namespace details {

VALUE rho_cast_helper<VALUE, jobject>::convertJavaMapToRubyHash(JNIEnv *env, jobject objMap)
{
    jhobject objSet = env->CallObjectMethod(objMap, midMapKeySet);
    if (!objSet) return Qnil;
    jhobject objIterator = env->CallObjectMethod(objSet.get(), midSetIterator);
    if (!objIterator) return Qnil;
                                  
    CHoldRubyValue retval(rho_ruby_createHash());
    while(env->CallBooleanMethod(objIterator.get(), midIteratorHasNext))
    {
        jhstring objKey = (jstring)env->CallObjectMethod(objIterator.get(), midIteratorNext);
        if (!objKey) return Qnil;
        jhstring objValue = (jstring)env->CallObjectMethod(objMap, midMapGet, objKey.get());
        if (!objValue) return Qnil;

        std::string const &strKey = rho_cast<std::string>(objKey);
        std::string const &strValue = rho_cast<std::string>(objValue);
        addStrToHash(retval, strKey.c_str(), strValue.c_str());
    }
    return retval;
}

jobject rho_cast_helper<jobject, VALUE>::operator()(JNIEnv *env, VALUE value)
{
    if (NIL_P(value))
        return NULL;

    if (TYPE(value) == T_STRING)
        return env->NewStringUTF(RSTRING_PTR(value));

    RAWLOG_ERROR("rho_cast<jobject, VALUE>: unknown type of value");
    return NULL;
}

VALUE rho_cast_helper<VALUE, jobject>::operator()(JNIEnv *env, jobject obj)
{
    if (env->IsSameObject(obj, NULL) == JNI_TRUE)
        return Qnil;

    if (!initConvertor(env))
    {
        env->ThrowNew(getJNIClass(RHODES_JAVA_CLASS_RUNTIME_EXCEPTION), "Java <=> Ruby conversion initialization failed");
        return Qnil;
    }

    if (env->IsInstanceOf(obj, clsString))
    {
        const char *str = env->GetStringUTFChars(static_cast<jstring>(obj), JNI_FALSE);
        VALUE res = rho_ruby_create_string(str);
        env->ReleaseStringUTFChars(static_cast<jstring>(obj), str);
        return res;
    }

    if (env->IsInstanceOf(obj, clsMap))
        return convertJavaMapToRubyHash(env, obj);

    RAWLOG_ERROR("rho_cast<VALUE, jobject>: unknown type of value");
    return Qnil;
}

VALUE rho_cast_helper<VALUE, jstring>::operator()(JNIEnv *env, jstring jStr)
{
    if(env->IsSameObject(jStr, NULL) == JNI_TRUE)
        return Qnil;

    const char *str = env->GetStringUTFChars(jStr, JNI_FALSE);
    VALUE res = rho_ruby_create_string(str);
    env->ReleaseStringUTFChars(jStr, str);

    return res;
}

jstring rho_cast_helper<jstring, VALUE>::operator()(JNIEnv *env, VALUE value)
{
    if (NIL_P(value))
        return NULL;

    return env->NewStringUTF(RSTRING_PTR(value));
}

}
