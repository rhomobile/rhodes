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

#include "com_rhomobile_rhoconnect_RhomModel.h"

#include "RhoConnectJniNotify.h"

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
    char* res = reinterpret_cast<char*>(rho_sync_doSyncSourceByName(rho_cast<std::string>(env, jname).c_str()));
    jhobject jhNotify = rho::connect_jni::rhoconnect_jni_parsenotify(env, res);
    rho_sync_free_string(res);
    return jhNotify.release();
}
//----------------------------------------------------------------------------------------------------------------------

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhoconnect_RhomModel_createByName
  (JNIEnv * env, jclass, jstring jModelName, jobjectArray jKeys, jobjectArray jVals)
{
    unsigned long item = rho_connectclient_hash_create();

    unsigned n = env->GetArrayLength(jKeys);
    for(unsigned i = 0; i < n; ++i)
    {
        jhstring jkey = static_cast<jstring>(env->GetObjectArrayElement(jKeys, i));
        jhstring jval = static_cast<jstring>(env->GetObjectArrayElement(jVals, i));
    }
}
//----------------------------------------------------------------------------------------------------------------------

int java_map_inserter(const char* key, const char* val, void* pThis)
{
    JNIEnv * env = jnienv();
    jobject jMap = reinterpret_cast<jobject>(pThis);

    static jclass jMapClass = getJNIClass(RHODES_JAVA_CLASS_MAP);
    if (!jMapClass) return 0;

    static jmethodID midPut = getJNIClassMethod(env, jMapClass, "put", "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");
    if (!midPut) return 0;

    jhstring jhKey = rho_cast<jhstring>(env, key);
    jhstring jhVal = rho_cast<jhstring>(env, val);
    env->CallObjectMethod(jMap, midPut, jhKey.get(), jhVal.get());

    return 1;
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
