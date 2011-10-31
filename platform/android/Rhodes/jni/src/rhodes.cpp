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
#include <sync/SyncThread.h>
#include <sync/ClientRegister.h>

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
        return std::string();
    } else
    {
        const char *ts = env->GetStringUTFChars(s, JNI_FALSE);
        std::string ret(ts);
        env->ReleaseStringUTFChars(s, ts);
        return ret;
    }
}

jhstring rho_cast_helper<jhstring, char const *>::operator()(JNIEnv *env, char const *s)
{
    return jhstring(env->NewStringUTF(s));
}

jclass RhoMapConvertor::clsString;
jclass RhoMapConvertor::clsMap;
jclass RhoMapConvertor::clsSet;
jclass RhoMapConvertor::clsIterator;
jmethodID RhoMapConvertor::midMapGet;
jmethodID RhoMapConvertor::midMapKeySet;
jmethodID RhoMapConvertor::midSetIterator;
jmethodID RhoMapConvertor::midIteratorHasNext;
jmethodID RhoMapConvertor::midIteratorNext;

bool RhoMapConvertor::initConvertor(JNIEnv *env)
{
    static bool initialized = false;

    if (initialized) return initialized;

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

    return initialized = true;
}
//----------------------------------------------------------------------------------------------------------------------

std::auto_ptr<rho::Hashtable<std::string, std::string> >
rho_cast_helper<std::auto_ptr<rho::Hashtable<std::string, std::string> >, jobject>::operator()(JNIEnv *env, jobject jObj)
{
        value_type result(new element_type);

        if (!RhoMapConvertor::initConvertor(env)) return value_type(0);
        jobject jSet = env->CallObjectMethod(jObj, midMapKeySet);
        if (!jSet) return value_type(0);
        jobject jIterator = env->CallObjectMethod(jSet, midSetIterator);
        if (!jIterator) return value_type(0);

        while(env->CallBooleanMethod(jIterator, midIteratorHasNext))
        {
            jhstring jkey = static_cast<jstring>(env->CallObjectMethod(jIterator, midIteratorNext));
            jhstring jval = static_cast<jstring>(env->CallObjectMethod(jObj, midMapGet, jkey.get()));

            std::string key = rho_cast<std::string>(env, jkey);
            std::string val = rho_cast<std::string>(env, jval);

            result->put(key, val);
        }
        return result;
}
//----------------------------------------------------------------------------------------------------------------------

std::auto_ptr<rho::Hashtable<std::string,std::string> >
rho_cast_helper<std::auto_ptr<rho::Hashtable<std::string,std::string> >, jobjectArray>::operator()(JNIEnv *env,
                                                                                         jobjectArray jKeys,
                                                                                         jobjectArray jVals)
{
    value_type result(new element_type);

    unsigned n = env->GetArrayLength(jKeys);
    for(unsigned i = 0; i < n; ++i)
    {
        jhstring jkey = static_cast<jstring>(env->GetObjectArrayElement(jKeys, i));
        jhstring jval = static_cast<jstring>(env->GetObjectArrayElement(jVals, i));

        std::string key = rho_cast<std::string>(env, jkey);
        std::string val = rho_cast<std::string>(env, jval);

        result->put(key, val);
    }
    return result;
}
//----------------------------------------------------------------------------------------------------------------------

std::auto_ptr<rho::Vector<std::string> >
rho_cast_helper<std::auto_ptr<rho::Vector<std::string> >, jobjectArray>::operator ()(JNIEnv *env, jobjectArray jArr)
{
    value_type result(new element_type);

    unsigned n = env->GetArrayLength(jArr);
    result->reserve(n);

    for(unsigned i = 0; i < n; ++i)
    {
        jhstring jval = static_cast<jstring>(env->GetObjectArrayElement(jArr, i));
        std::string val = rho_cast<std::string>(env, jval);
        result->push_back(val);
    }
    return result;
}
//----------------------------------------------------------------------------------------------------------------------

} // namespace details
