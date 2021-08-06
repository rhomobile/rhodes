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

#include <android/log.h>

#include "rhodes/jni/com_rhomobile_rhodes_RhodesService.h"
#include "rhodes/jni/com_rhomobile_rhodes_RhodesAppOptions.h"

#include <common/RhoConf.h>
#include <logging/RhoLogConf.h>

#include <sys/stat.h>
#include <sys/resource.h>
#include <pwd.h>

#include "rhodes/JNIRhodes.h"
#include "rhodes/RhoClassFactory.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "Rhodes"

const char *rho_java_class[] = {
#define RHODES_DEFINE_JAVA_CLASS(x, name) name,
#include "rhojava.inc"
#include "rhojava_extra.inc"                    //generated from extensions-declarated preloads
#undef RHODES_DEFINE_JAVA_CLASS
};

static pthread_key_t g_thrkey;

static JavaVM *g_jvm = NULL;
JavaVM *jvm()
{
    return g_jvm;
}

namespace rho
{
namespace common
{

void AndroidLogSink::writeLogMessage(String &strMsg)
{
    __android_log_write(ANDROID_LOG_INFO, "APP", strMsg.c_str());
}

String AndroidMemoryInfoCollector::collect()
{
    String ret;
    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_MEMORY_INFO_COLLECTOR);
    if (!cls) return ret;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "collect", "()Ljava/lang/String;");
    if (!mid) return ret;
    jhstring jStr( static_cast<jstring>(env->CallStaticObjectMethod(cls, mid)) );
    return rho_cast<String>(env,jStr);
}

String clearException(JNIEnv* env)
{
    jholder<jthrowable> jhexc = env->ExceptionOccurred();
    env->ExceptionClear();
    jholder<jclass> jhclass = env->GetObjectClass(jhexc.get());
    jmethodID mid = env->GetMethodID(jhclass.get(), "toString", "()Ljava/lang/String;");

    jmethodID midDump = env->GetMethodID(jhclass.get(), "printStackTrace", "()V");
    
    jhstring jhmsg = (jstring)env->CallObjectMethod(jhexc.get(), mid);

    env->CallVoidMethod(jhexc.get(), midDump);
    
    return rho_cast<rho::String>(env, jhmsg);
}

} // namespace common
} // namespace rho

void store_thr_jnienv(JNIEnv *env)
{
    pthread_setspecific(g_thrkey, env);
}

JNIEnv *jnienv()
{
    JNIEnv *env = (JNIEnv *)pthread_getspecific(g_thrkey);
    if (!env)
        RAWLOG_ERROR("JNIEnv is not set for this thread!!!");
    return env;
}

void initjnienv(JNIEnv* env)
{
    if(!pthread_getspecific(g_thrkey))
    {
        store_thr_jnienv(env);
    }
}

std::vector<jclass> g_classes;

jclass& getJNIClass(int n)
{
    static jclass jcNull = 0;
    if (n < 0 || (size_t)n >= g_classes.size())
    {
        RAWLOG_ERROR1("Illegal index when call getJNIClass: %d", n);
        return jcNull;
    }
    return g_classes[n];
}

jclass getJNIObjectClass(JNIEnv *env, jobject obj)
{
    jclass cls = env->GetObjectClass(obj);
    if (!cls)
        RAWLOG_ERROR1("Can not get class for object: %p (JNI)", obj);
    return cls;
}

jfieldID getJNIClassField(JNIEnv *env, jclass cls, const char *name, const char *signature)
{
    jfieldID fid = env->GetFieldID(cls, name, signature);
    if (!fid)
        RAWLOG_ERROR3("Can not get field %s of signature %s for class %p", name, signature, cls);
    return fid;
}

jfieldID getJNIClassStaticField(JNIEnv *env, jclass cls, const char *name, const char *signature)
{
    jfieldID fid = env->GetStaticFieldID(cls, name, signature);
    if (!fid)
        RAWLOG_ERROR3("Can not get static field %s of signature %s for class %p", name, signature, cls);
    return fid;
}

jmethodID getJNIClassMethod(JNIEnv *env, jclass cls, const char *name, const char *signature)
{
    jmethodID mid = env->GetMethodID(cls, name, signature);
    if (!mid)
        RAWLOG_ERROR3("Can not get method %s of signature %s for class %p", name, signature, cls);
    return mid;
}

jmethodID getJNIClassStaticMethod(JNIEnv *env, jclass cls, const char *name, const char *signature)
{
    jmethodID mid = env->GetStaticMethodID(cls, name, signature);
    if (!mid)
        RAWLOG_ERROR3("Can not get static method %s of signature %s for class %p", name, signature, cls);
    return mid;
}

jint JNI_OnLoad(JavaVM* vm, void* /*reserved*/)
{
    g_jvm = vm;
    jint jversion = JNI_VERSION_1_4;
    JNIEnv *env;
    if (vm->GetEnv((void**)&env, jversion) != JNI_OK)
        return -1;

    pthread_key_create(&g_thrkey, NULL);
    store_thr_jnienv(env);

    for(size_t i = 0, lim = sizeof(rho_java_class)/sizeof(rho_java_class[0]); i != lim; ++i)
    {
        const char *className = rho_java_class[i];
        jclass cls = env->FindClass(className);
        if (!cls)
            return -1;
        jclass globalCls = static_cast<jclass>(env->NewGlobalRef(cls));
        if (!globalCls)
            return -1;
        env->DeleteLocalRef(cls);
        g_classes.push_back(globalCls);
    }

    return jversion;
}

namespace details
{

std::string rho_cast_helper<std::string, jstring>::operator()(JNIEnv *env, jstring s)
{
    if(env->IsSameObject(s, NULL) == JNI_TRUE)
    {
        //Avoid crash in case of null java reference
        //RAWTRACE("rho_cast<string, jstring>: \"\"");
        return std::string();
    } else
    {
        const char *ts = env->GetStringUTFChars(s, JNI_FALSE);
        std::string ret(ts);
        env->ReleaseStringUTFChars(s, ts);
        //RAWTRACE1("rho_cast<string, jstring>: %s", ret.c_str());
        return ret;
    }
}

jstring rho_cast_helper<jstring, char const *>::operator()(JNIEnv *env, char const *s)
{
    //RAWTRACE1("rho_cast<jstring, string>: %s", s);

    return s ? env->NewStringUTF(s) : (jstring)0;
}

jclass RhoJniConvertor::clsBoolean;
jclass RhoJniConvertor::clsInteger;
jclass RhoJniConvertor::clsDouble;
jclass RhoJniConvertor::clsString;
jclass RhoJniConvertor::clsCollection;
jclass RhoJniConvertor::clsMap;
jclass RhoJniConvertor::clsSet;
jclass RhoJniConvertor::clsHashMap;
jclass RhoJniConvertor::clsArrayList;
jclass RhoJniConvertor::clsIterator;
jmethodID RhoJniConvertor::midCollectionIterator;
jmethodID RhoJniConvertor::midMapGet;
jmethodID RhoJniConvertor::midMapKeySet;
jmethodID RhoJniConvertor::midArrayList;
jmethodID RhoJniConvertor::midArrayListAdd;
jmethodID RhoJniConvertor::midHashMap;
jmethodID RhoJniConvertor::midHashMapPut;
jmethodID RhoJniConvertor::midSetIterator;
jmethodID RhoJniConvertor::midIteratorHasNext;
jmethodID RhoJniConvertor::midIteratorNext;
jmethodID RhoJniConvertor::midBooleanValue;
jmethodID RhoJniConvertor::midBooleanValueOf;
jmethodID RhoJniConvertor::midIntValue;
jmethodID RhoJniConvertor::midInteger;
jmethodID RhoJniConvertor::midDoubleValue;
jmethodID RhoJniConvertor::midDoubleValueOf;

bool RhoJniConvertor::initConvertor(JNIEnv *env)
{
    RAWTRACE("RhoJniConvertor::initConvertor");

    m_env = env;

    static bool initialized = false;

    if (initialized)
        return initialized;

    static rho::common::CMutex rho_cast_java_ruby_mtx;
    rho::common::CMutexLock guard(rho_cast_java_ruby_mtx);

    if (initialized)
        return initialized;

    clsBoolean = getJNIClass(RHODES_JAVA_CLASS_BOOLEAN);
    if (!clsBoolean) return false;
    clsInteger = getJNIClass(RHODES_JAVA_CLASS_INTEGER);
    if (!clsInteger) return false;
    clsDouble = getJNIClass(RHODES_JAVA_CLASS_DOUBLE);
    if (!clsDouble) return false;
    clsString = getJNIClass(RHODES_JAVA_CLASS_STRING);
    if (!clsString) return false;
    clsCollection = getJNIClass(RHODES_JAVA_CLASS_COLLECTION);
    if (!clsCollection) return false;
    clsMap = getJNIClass(RHODES_JAVA_CLASS_MAP);
    if (!clsMap) return false;
    clsSet = getJNIClass(RHODES_JAVA_CLASS_SET);
    if (!clsSet) return false;
    clsArrayList = getJNIClass(RHODES_JAVA_CLASS_ARRAYLIST);
    if (!clsArrayList) return false;
    clsHashMap = getJNIClass(RHODES_JAVA_CLASS_HASHMAP);
    if (!clsHashMap) return false;
    clsIterator = getJNIClass(RHODES_JAVA_CLASS_ITERATOR);
    if (!clsIterator) return false;

    midCollectionIterator = getJNIClassMethod(env, clsCollection, "iterator", "()Ljava/util/Iterator;");
    if (!midCollectionIterator) return false;
    midMapGet = getJNIClassMethod(env, clsMap, "get", "(Ljava/lang/Object;)Ljava/lang/Object;");
    if (!midMapGet) return false;
    midMapKeySet = getJNIClassMethod(env, clsMap, "keySet", "()Ljava/util/Set;");
    if (!midMapKeySet) return false;
    midArrayList = getJNIClassMethod(env, clsArrayList, "<init>", "()V");
    if (!midArrayList) return false;
    midArrayListAdd = getJNIClassMethod(env, clsArrayList, "add", "(Ljava/lang/Object;)Z");
    if (!midArrayListAdd) return false;
    midHashMap = getJNIClassMethod(env, clsHashMap, "<init>", "()V");
    if (!midHashMap) return false;
    midHashMapPut = getJNIClassMethod(env, clsHashMap, "put", "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");
    if (!midHashMapPut) return false;
    midSetIterator = getJNIClassMethod(env, clsSet, "iterator", "()Ljava/util/Iterator;");
    if (!midSetIterator) return false;
    midIteratorHasNext = getJNIClassMethod(env, clsIterator, "hasNext", "()Z");
    if (!midIteratorHasNext) return false;
    midIteratorNext = getJNIClassMethod(env, clsIterator, "next", "()Ljava/lang/Object;");
    if (!midIteratorNext) return false;
    midBooleanValue = getJNIClassMethod(env, clsBoolean, "booleanValue", "()Z");
    if (!midBooleanValue) return false;
    midBooleanValueOf = getJNIClassStaticMethod(env, clsBoolean, "valueOf", "(Z)Ljava/lang/Boolean;");
    if (!midBooleanValueOf) return false;
    midIntValue = getJNIClassMethod(env, clsInteger, "intValue", "()I");
    if (!midIntValue) return false;
    midInteger = getJNIClassMethod(env, clsInteger, "<init>", "(I)V");
    if (!midInteger) return false;
    midDoubleValue = getJNIClassMethod(env, clsDouble, "doubleValue", "()D");
    if (!midDoubleValue) return false;
    midDoubleValueOf = getJNIClassStaticMethod(env, clsDouble, "valueOf", "(D)Ljava/lang/Double;");
    if (!midDoubleValueOf) return false;

    return initialized = true;
}
//----------------------------------------------------------------------------------------------------------------------
jobject RhoJniConvertor::getBooleanObject(bool val)
{
    static jfieldID fidTRUE = getJNIClassStaticField(m_env, clsBoolean, "TRUE", "Ljava/lang/Boolean;");
    static jfieldID fidFALSE = getJNIClassStaticField(m_env, clsBoolean, "FALSE", "Ljava/lang/Boolean;");
    return m_env->GetStaticObjectField(clsBoolean, val ? fidTRUE : fidFALSE);
}
//----------------------------------------------------------------------------------------------------------------------
jobject RhoJniConvertor::getIntegerObject(int val)
{
    jint jres = static_cast<jint>(val);
    return m_env->NewObject(clsInteger, midInteger, jres);
}
//----------------------------------------------------------------------------------------------------------------------
jobject RhoJniConvertor::getDoubleObject(double val)
{
    jdouble jres = static_cast<jdouble>(val);
    return m_env->CallStaticObjectMethod(clsDouble, midDoubleValueOf, jres);
}
//----------------------------------------------------------------------------------------------------------------------

HStringMap
rho_cast_helper<HStringMap, jobject>::operator()(JNIEnv *env, jobject jObj)
{
    RAWTRACE("rho_cast<HStringMap, jobject>");

    value_type result(new value_type::element_type);

    if (!initConvertor(env)) return value_type(0);

    jhobject jSet = env->CallObjectMethod(jObj, midMapKeySet);
    if (!jSet) return value_type(0);
    jhobject jIterator = env->CallObjectMethod(jSet.get(), midSetIterator);
    if (!jIterator) return value_type(0);

    while(env->CallBooleanMethod(jIterator.get(), midIteratorHasNext))
    {
            jhstring jkey = static_cast<jstring>(env->CallObjectMethod(jIterator.get(), midIteratorNext));
            jhstring jval = static_cast<jstring>(env->CallObjectMethod(jObj, midMapGet, jkey.get()));

            std::string key = rho_cast<std::string>(env, jkey);
            std::string val = rho_cast<std::string>(env, jval);

            result->put(key, val);
    }
    return result;
}
//----------------------------------------------------------------------------------------------------------------------

HStringMap
rho_cast_helper<HStringMap, jobjectArray>::operator()(JNIEnv *env, jobjectArray jKeys, jobjectArray jVals)
{
    RAWTRACE("rho_cast<HStringMap, jobjectArray>");

    value_type result(new value_type::element_type);

    unsigned n = env->GetArrayLength(jKeys);
    for(unsigned i = 0; i < n; ++i)
    {
		jobject jkeyraw = env->GetObjectArrayElement(jKeys, i);
		jobject jvalraw = env->GetObjectArrayElement(jVals, i);
		
        jhstring jkey = static_cast<jstring>(jkeyraw);
        jhstring jval = static_cast<jstring>(jvalraw);

        std::string key = rho_cast<std::string>(env, jkey);
        std::string val = rho_cast<std::string>(env, jval);
		
		env->DeleteLocalRef(jkeyraw);
		env->DeleteLocalRef(jvalraw);

        result->put(key, val);
    }
    return result;
}
//----------------------------------------------------------------------------------------------------------------------

HStringVector
rho_cast_helper<HStringVector, jobjectArray>::operator ()(JNIEnv *env, jobjectArray jArr)
{
    RAWTRACE("rho_cast<HStringVector, jobjectArray>");

    value_type result(new value_type::element_type);

    unsigned n = env->GetArrayLength(jArr);
    result->reserve(n);

    for(unsigned i = 0; i < n; ++i)
    {
		jobject jvalraw = env->GetObjectArrayElement(jArr, i);
		
        jhstring jval = static_cast<jstring>(jvalraw);
        std::string val = rho_cast<std::string>(env, jval);
		
		env->DeleteLocalRef(jvalraw);
		
        result->push_back(val);
    }
    return result;
}
//----------------------------------------------------------------------------------------------------------------------

HStringVector
rho_cast_helper<HStringVector, jobject>::operator ()(JNIEnv *env, jobject jList)
{
    RAWTRACE("rho_cast<HStringVector, jobject>");

    value_type result(new value_type::element_type);

    if (!initConvertor(env)) return value_type(0);

    jhobject jhIterator = env->CallObjectMethod(jList, midCollectionIterator);
    if (!jhIterator) return value_type(0);

    while(env->CallBooleanMethod(jhIterator.get(), midIteratorHasNext))
    {
        jhstring jhVal = static_cast<jstring>(env->CallObjectMethod(jhIterator.get(), midIteratorNext));
        std::string val = rho_cast<std::string>(env, jhVal);
        result->push_back(val);
    }
    return result;
}
//----------------------------------------------------------------------------------------------------------------------

jobject rho_cast_helper<jobject, rho::Vector<std::string> >::operator()(JNIEnv* env, const rho::Vector<std::string>& stringVector)
{
    RAWTRACE("rho_cast<jobject, HStringVector>");

    if (!initConvertor(env)) return 0;

    jobject jArray = env->NewObject(clsArrayList, midArrayList);
    if(!jArray) return 0;

    for(std::vector<std::string>::const_iterator iter = stringVector.begin(); iter != stringVector.end(); ++iter)
    {
        jhstring jhItem = rho_cast<jstring>(env, *iter);
        env->CallBooleanMethod(jArray, midArrayListAdd, jhItem.get());
    }
    return jArray;
}
//----------------------------------------------------------------------------------------------------------------------

} // namespace details
