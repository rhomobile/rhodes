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
#include "sync/SyncThread.h"
#include "RhoConnectClient/RhoConnectClient.h"
#include "logging/RhoLog.h"

#include "com_rhomobile_rhoconnect_RhomModel.h"

#include "RhoConnectJniNotify.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "RhomModelJNI"

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhoconnect_RhomModel_init
  (JNIEnv * env, jobject jmodel)
{
    static jclass clsmodel = getJNIClass(RHOCONNECT_JAVA_CLASS_RHOMMODEL);
    if (!clsmodel) return;

    static jmethodID midmodeltype = getJNIClassMethod(env, clsmodel, "setModelType", "(I)V");
    if (!midmodeltype) return;
    static jmethodID midsynctype = getJNIClassMethod(env, clsmodel, "setSyncType", "(I)V");
    if (!midsynctype) return;
    static jmethodID midsyncpri = getJNIClassMethod(env, clsmodel, "setSyncPriority", "(I)V");
    if (!midsyncpri) return;
    static jmethodID midpart = getJNIClassMethod(env, clsmodel, "setPartition", "(Ljava/lang/String;)V");
    if (!midpart) return;

    RHOM_MODEL model;
    rho_connectclient_initmodel(&model);

    env->CallVoidMethod(jmodel, midmodeltype, model.type);
    env->CallVoidMethod(jmodel, midsynctype, model.sync_type);
    env->CallVoidMethod(jmodel, midsyncpri, model.sync_priority);
    env->CallVoidMethod(jmodel, midpart, rho_cast<jhstring>(env, model.partition).get());
}
//----------------------------------------------------------------------------------------------------------------------

RHO_GLOBAL jobject JNICALL Java_com_rhomobile_rhoconnect_RhomModel_syncByName
  (JNIEnv * env, jclass, jstring jname)
{
    std::string name = rho_cast<std::string>(env, jname);

    LOG(INFO) + "Syncing model: " + name;

    char* res = reinterpret_cast<char*>(rho_sync_doSyncSourceByName(name.c_str()));
    jhobject jhNotify = rho::connect_jni::rhoconnect_jni_parsenotify(env, res);
    rho_sync_free_string(res);
    return jhNotify.release();
}
//----------------------------------------------------------------------------------------------------------------------

int java_map_inserter(const char* key, const char* val, void* pThis)
{
    JNIEnv * env = jnienv();
    jobject jMap = reinterpret_cast<jobject>(pThis);

    jclass& jMapClass = getJNIClass(RHODES_JAVA_CLASS_MAP);
    if (!jMapClass) return 0;

    static jmethodID midPut = getJNIClassMethod(env, jMapClass, "put", "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");
    if (!midPut) return 0;

    jhstring jhKey = rho_cast<jhstring>(env, key);
    jhstring jhVal = rho_cast<jhstring>(env, val);
    env->CallObjectMethod(jMap, midPut, jhKey.get(), jhVal.get());

    return 1;
}
//----------------------------------------------------------------------------------------------------------------------

unsigned long rhoconnect_jni_make_object_hash(JNIEnv * env, jobjectArray jKeys, jobjectArray jVals)
{
    unsigned long item = rho_connectclient_hash_create();

    unsigned n = env->GetArrayLength(jKeys);
    for(unsigned i = 0; i < n; ++i)
    {
        jhstring jkey = static_cast<jstring>(env->GetObjectArrayElement(jKeys, i));
        jhstring jval = static_cast<jstring>(env->GetObjectArrayElement(jVals, i));

        std::string key = rho_cast<std::string>(env, jkey);
        std::string val = rho_cast<std::string>(env, jval);

        rho_connectclient_hash_put(item, key.c_str(), val.c_str());
    }
    return item;
}
//----------------------------------------------------------------------------------------------------------------------

RHO_GLOBAL jobject JNICALL Java_com_rhomobile_rhoconnect_RhomModel_createByName
  (JNIEnv * env, jclass, jstring jModelName, jobjectArray jKeys, jobjectArray jVals)
{
    std::string name = rho_cast<std::string>(env, jModelName);
    LOG(TRACE) + "createByName: " + name;

    unsigned long item = rhoconnect_jni_make_object_hash(env, jKeys, jVals);

    rho_connectclient_create_object(name.c_str(), item);
    
    static jclass jMapClass = getJNIClass(RHODES_JAVA_CLASS_HASHMAP);
    if (!jMapClass) return 0;

    static jmethodID midHashMap = getJNIClassMethod(env, jMapClass, "<init>", "()V");
    if (!midHashMap) return 0;

    jobject jMap = env->NewObject(jMapClass, midHashMap);
    rho_connectclient_hash_enumerate(item, java_map_inserter, jMap);
    rho_connectclient_hash_delete(item);
    return jMap;
}
//----------------------------------------------------------------------------------------------------------------------

RHO_GLOBAL jobject JNICALL Java_com_rhomobile_rhoconnect_RhomModel_findByName
  (JNIEnv * env, jclass, jstring jModelName, jstring jItemId)
{
    std::string modelName = rho_cast<std::string>(env, jModelName);
    std::string itemId = rho_cast<std::string>(env, jItemId);

    unsigned long item = rho_connectclient_find(modelName.c_str(), itemId.c_str());
    if (item == 0) return NULL;

    static jclass jMapClass = getJNIClass(RHODES_JAVA_CLASS_HASHMAP);
    if (!jMapClass) return 0;

    static jmethodID midHashMap = getJNIClassMethod(env, jMapClass, "<init>", "()V");
    if (!midHashMap) return 0;

    jobject jMap = env->NewObject(jMapClass, midHashMap);
    rho_connectclient_hash_enumerate(item, java_map_inserter, jMap);
    return jMap;
}
//----------------------------------------------------------------------------------------------------------------------

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhoconnect_RhomModel_saveByName
  (JNIEnv * env, jclass, jstring jModelName, jobjectArray jKeys, jobjectArray jVals)
{
    std::string name = rho_cast<std::string>(env, jModelName);

    LOG(TRACE) + "saveByName: " + name;

    unsigned long item = rhoconnect_jni_make_object_hash(env, jKeys, jVals);

    rho_connectclient_save(name.c_str(), item);
    rho_connectclient_hash_delete(item);
}
//----------------------------------------------------------------------------------------------------------------------

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhoconnect_RhomModel_destroyByName
  (JNIEnv * env, jclass, jstring jModelName, jobjectArray jKeys, jobjectArray jVals)
{
    std::string name = rho_cast<std::string>(env, jModelName);
    LOG(TRACE) + "destroyByName: " + name;
    
    unsigned long item = rhoconnect_jni_make_object_hash(env, jKeys, jVals);

    rho_connectclient_itemdestroy(name.c_str(), item);
    rho_connectclient_hash_delete(item);
}
//----------------------------------------------------------------------------------------------------------------------

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhoconnect_RhomModel_startBulkUpdateByName
  (JNIEnv * env, jclass, jstring jModelName)
{
    std::string modelName = rho_cast<std::string>(env, jModelName);
    rho_connectclient_start_bulkupdate(modelName.c_str());
}
//----------------------------------------------------------------------------------------------------------------------

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhoconnect_RhomModel_stopBulkUpdateByName
  (JNIEnv * env, jclass, jstring jModelName)
{
    std::string modelName = rho_cast<std::string>(env, jModelName);
    rho_connectclient_stop_bulkupdate(modelName.c_str());
}
//----------------------------------------------------------------------------------------------------------------------

RHO_GLOBAL jobject JNICALL Java_com_rhomobile_rhoconnect_RhomModel_findFirstByName
  (JNIEnv * env, jclass, jstring jModelName, jobjectArray jKeys, jobjectArray jVals)
{
    std::string name = rho_cast<std::string>(env, jModelName);
    LOG(TRACE) + "findFirstByName: " + name;

    unsigned long cond = rhoconnect_jni_make_object_hash(env, jKeys, jVals);

    unsigned long item = rho_connectclient_find_first(name.c_str(), cond);
    rho_connectclient_hash_delete(cond);

    LOG(TRACE) + "Item found: " + item;
    if (item == 0) return 0;

    static jclass jMapClass = getJNIClass(RHODES_JAVA_CLASS_HASHMAP);
    if (!jMapClass) return 0;

    static jmethodID midHashMap = getJNIClassMethod(env, jMapClass, "<init>", "()V");
    if (!midHashMap) return 0;

    jobject jMap = env->NewObject(jMapClass, midHashMap);

    LOG(TRACE) + "Enumerating item: " + item;

    rho_connectclient_hash_enumerate(item, java_map_inserter, jMap);
    return jMap;
}
//----------------------------------------------------------------------------------------------------------------------

RHO_GLOBAL jobject JNICALL Java_com_rhomobile_rhoconnect_RhomModel_findAllByName
  (JNIEnv * env, jclass, jstring jModelName, jobjectArray jKeys, jobjectArray jVals)
{
    std::string name = rho_cast<std::string>(env, jModelName);

    LOG(TRACE) + "findFirstByName: " + name;

    unsigned long cond = rhoconnect_jni_make_object_hash(env, jKeys, jVals);

    unsigned long items = rho_connectclient_find_all(name.c_str(), cond);
    rho_connectclient_hash_delete(cond);

    static jclass jListClass = getJNIClass(RHODES_JAVA_CLASS_ARRAYLIST);
    if (!jListClass) return 0;
    static jmethodID midList = getJNIClassMethod(env, jListClass, "<init>", "()V");
    if (!midList) return 0;
    static jmethodID midListAdd = getJNIClassMethod(env, jListClass, "add", "(Ljava/lang/Object;)Z");
    if (!midList) return 0;

    static jclass jMapClass = getJNIClass(RHODES_JAVA_CLASS_HASHMAP);
    if (!jMapClass) return 0;
    static jmethodID midHashMap = getJNIClassMethod(env, jMapClass, "<init>", "()V");
    if (!midHashMap) return 0;

    jobject jList = env->NewObject(jListClass, midList);

    int cnt = rho_connectclient_strhasharray_size(items);
    for(int i = 0; i < cnt; ++i)
    {
        jobject jMap = env->NewObject(jMapClass, midHashMap);
        rho_connectclient_hash_enumerate(rho_connectclient_strhasharray_get(items, i), java_map_inserter, jMap);
        env->CallBooleanMethod(jList, midListAdd, jMap);
        env->DeleteLocalRef(jMap);
    }
    return jList;
}
//----------------------------------------------------------------------------------------------------------------------

RHO_GLOBAL jobject JNICALL Java_com_rhomobile_rhoconnect_RhomModel_findBySqlByName
  (JNIEnv * env, jclass, jstring jModelName, jstring jQuery, jobjectArray jParams)
{
    std::string name = rho_cast<std::string>(env, jModelName);

    LOG(TRACE) + "findBySqlByName: " + name;

    std::string query = rho_cast<std::string>(env, jQuery);
    std::auto_ptr<rho::Vector<std::string> > params = (jParams != NULL)
                                                ? rho_cast<std::auto_ptr<rho::Vector<std::string> > >(env, jParams)
                                                : std::auto_ptr<rho::Vector<std::string> >(0);

    unsigned long items = rho_connectclient_findbysql(name.c_str(), query.c_str(), reinterpret_cast<unsigned long>(params.get()));

    jclass& jListClass = getJNIClass(RHODES_JAVA_CLASS_ARRAYLIST);
    if (!jListClass) return 0;
    static jmethodID midList = getJNIClassMethod(env, jListClass, "<init>", "()V");
    if (!midList) return 0;
    static jmethodID midListAdd = getJNIClassMethod(env, jListClass, "add", "(Ljava/lang/Object;)Z");
    if (!midList) return 0;

    jclass& jMapClass = getJNIClass(RHODES_JAVA_CLASS_HASHMAP);
    if (!jMapClass) return 0;
    static jmethodID midHashMap = getJNIClassMethod(env, jMapClass, "<init>", "()V");
    if (!midHashMap) return 0;

    jobject jList = env->NewObject(jListClass, midList);

    int cnt = rho_connectclient_strhasharray_size(items);
    for(int i = 0; i < cnt; ++i)
    {
        jobject jMap = env->NewObject(jMapClass, midHashMap);
        rho_connectclient_hash_enumerate(rho_connectclient_strhasharray_get(items, i), java_map_inserter, jMap);
        env->CallBooleanMethod(jList, midListAdd, jMap);
        env->DeleteLocalRef(jMap);
    }
    return jList;
}

//----------------------------------------------------------------------------------------------------------------------

