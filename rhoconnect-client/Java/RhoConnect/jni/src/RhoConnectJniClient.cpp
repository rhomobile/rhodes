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

#include <vector>
#include <algorithm>

#include "common/RhoStd.h"
#include "common/RhoConf.h"
#include "rhodes/JNIRhodes.h"
#include "sync/SyncThread.h"
#include "RhoConnectClient/RhoConnectClient.h"
#include "logging/RhoLog.h"

#include "com_rhomobile_rhoconnect_RhoConnectClient.h"

#include "RhoConnectJniNotify.h"

typedef std::vector<RHOM_MODEL> model_vector;
typedef std::vector<rho::String> string_vector;

static jobject s_jNotifyDelegate = 0;
static jobject s_jObjectNotifyDelegate = 0;

static jclass clsString = 0;
static jclass clsMap = 0;
static jclass clsSet = 0;
static jclass clsIterator = 0;
static jmethodID midMapGet;
static jmethodID midMapKeySet;
static jmethodID midSetIterator;
static jmethodID midIteratorHasNext;
static jmethodID midIteratorNext;

static bool initMapConvert(JNIEnv *env)
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

static unsigned long convertMap(JNIEnv *env, jobject objMap)
{
    if (!initMapConvert(env))
        return 0;
    
    jobject objSet = env->CallObjectMethod(objMap, midMapKeySet);
    if (!objSet) return 0;
    jobject objIterator = env->CallObjectMethod(objSet, midSetIterator);
    if (!objIterator) return 0;
                                  
    unsigned long retval = rho_connectclient_hash_create();
    while(env->CallBooleanMethod(objIterator, midIteratorHasNext))
    {
        jstring objKey = (jstring)env->CallObjectMethod(objIterator, midIteratorNext);
        if (!objKey) return 0;
        jstring objValue = (jstring)env->CallObjectMethod(objMap, midMapGet, objKey);
        if (!objValue) return 0;

        std::string const &strKey = rho_cast<std::string>(objKey);
        std::string const &strValue = rho_cast<std::string>(objValue);
        rho_connectclient_hash_put(retval, strKey.c_str(), strValue.c_str());

        env->DeleteLocalRef(objKey);
        env->DeleteLocalRef(objValue);
    }
    return retval;
}


RHO_GLOBAL void JNICALL Java_com_rhomobile_rhoconnect_RhoConnectClient_initialize
  (JNIEnv * env, jobject, jobjectArray jmodels)
{
    jclass& clsmodel = getJNIClass(RHOCONNECT_JAVA_CLASS_RHOMMODEL);
    if (!clsmodel) return;
    static jmethodID midname = getJNIClassMethod(env, clsmodel, "getName", "()Ljava/lang/String;");
    if (!midname) return;
    static jmethodID midmodeltype = getJNIClassMethod(env, clsmodel, "getModelType", "()I");
    if (!midmodeltype) return;
    static jmethodID midsynctype = getJNIClassMethod(env, clsmodel, "getSyncType", "()I");
    if (!midsynctype) return;
    static jmethodID midsyncpri = getJNIClassMethod(env, clsmodel, "getSyncPriority", "()I");
    if (!midsyncpri) return;
    static jmethodID midpart = getJNIClassMethod(env, clsmodel, "getPartition", "()Ljava/lang/String;");
    if (!midpart) return;
    static jmethodID midblobattribs = getJNIClassMethod(env, clsmodel, "getBlobAttribsAsString", "()Ljava/lang/String;");
    if (!midblobattribs) return;
    static jmethodID midassocs = getJNIClassMethod(env, clsmodel, "getAssociations", "()Ljava/util/Map;");
    if (!midblobattribs) return;

    size_t cnt = static_cast<model_vector::size_type>(env->GetArrayLength(jmodels));
    model_vector models(cnt);
    string_vector names(cnt);
    string_vector partitions(cnt);
    string_vector blob_attribs(cnt);

    for(model_vector::size_type i = 0, cnt = models.size(); i < cnt; ++i)
    {
        jobject jmodel = env->GetObjectArrayElement(jmodels, i);
        RHOM_MODEL& model = models[i];
        rho::String& name = names[i];
        rho::String& partition = partitions[i];
        rho::String& cur_blob_attribs = blob_attribs[i];

        jhobject jname = env->CallObjectMethod(jmodel, midname);
        name = rho_cast<rho::String>(env, static_cast<jstring>(jname.get()));
        model.name = /*const_cast<char*>*/(name.c_str());

        model.type = static_cast<RHOM_MODEL_TYPE>(env->CallIntMethod(jmodel, midmodeltype));
        model.sync_type = static_cast<RHOM_SYNC_TYPE>(env->CallIntMethod(jmodel, midsynctype));
        model.sync_priority = env->CallIntMethod(jmodel, midsyncpri);

        jhobject jpart = env->CallObjectMethod(jmodel, midpart);
        partition = rho_cast<rho::String>(env, static_cast<jstring>(jpart.get()));
        model.partition = /*const_cast<char*>*/(partition.c_str());

        jhobject jattribs = env->CallObjectMethod(jmodel, midblobattribs);
        cur_blob_attribs = rho_cast<rho::String>(env, static_cast<jstring>(jattribs.get()));
        model.blob_attribs = cur_blob_attribs.c_str();

        jhobject jhassociations = env->CallObjectMethod(jmodel, midassocs);
        model.associations = convertMap(env, jhassociations.get());
    }

    rho_connectclient_init(&models.front(), models.size());

    for(model_vector::size_type i = 0, cnt = models.size(); i < cnt; ++i)
    {
        rho_connectclient_destroymodel(&models[i]);
    }
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhoconnect_RhoConnectClient_destroy
  (JNIEnv *, jobject)
{
    void rho_connectclient_destroy();
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhoconnect_RhoConnectClient_setSyncServer
  (JNIEnv * env, jobject, jstring host)
{
    rho_sync_set_syncserver(rho_cast<std::string>(env, host).c_str());
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhoconnect_RhoConnectClient_setThreadedMode
  (JNIEnv *, jobject, jboolean jmode)
{
    bool mode = jmode;
    rho_sync_set_threaded_mode(mode ? 1 : 0);
}

//RHO_GLOBAL jboolean JNICALL Java_com_rhomobile_rhoconnect_RhoConnectClient_getThreadedMode
//  (JNIEnv *, jobject)
//{
//    return (jboolean)(rho_sync_get_threaded_mode() != 0);
//}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhoconnect_RhoConnectClient_setPollInterval
  (JNIEnv *, jobject, jint time)
{
    rho_sync_set_pollinterval(time);
}

RHO_GLOBAL jint JNICALL Java_com_rhomobile_rhoconnect_RhoConnectClient_getPollInterval
  (JNIEnv *, jobject)
{
    return (jint)rho_sync_get_pollinterval();
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhoconnect_RhoConnectClient_setBulkSyncState
  (JNIEnv *, jobject, jint state)
{
    rho_conf_setInt("bulksync_state", state);
}

RHO_GLOBAL jint JNICALL Java_com_rhomobile_rhoconnect_RhoConnectClient_getBulkSyncState
  (JNIEnv *, jobject)
{
    return rho_conf_getInt("bulksync_state");
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhoconnect_RhoConnectClient_setConfigString
  (JNIEnv * env, jobject, jstring jname, jstring jvalue)
{
    std::string name = rho_cast<std::string>(env, jname);
    std::string value = rho_cast<std::string>(env, jvalue);
    if (name.compare("MinSeverity") == 0) {
        int severity;
        rho::common::convertFromStringA(value.c_str(), severity);
        rho_logconf_setSeverity(severity);
    } else
        rho_conf_setString(name.c_str(), value.c_str());
}

RHO_GLOBAL jstring JNICALL Java_com_rhomobile_rhoconnect_RhoConnectClient_getConfigString
  (JNIEnv * env, jobject, jstring jname)
{
    std::string name = rho_cast<std::string>(env, jname);
    const char* value = rho_conf_getString(name.c_str());

    jhstring jhvalue = rho_cast<jhstring>(env, value ? value : "");
    if (value)
        rho_conf_freeString(const_cast<char*>(value));

    return jhvalue.release();}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhoconnect_RhoConnectClient_databaseFullResetAndLogout
  (JNIEnv *, jobject)
{
    rho_connectclient_database_full_reset_and_logout();
}

RHO_GLOBAL jboolean JNICALL Java_com_rhomobile_rhoconnect_RhoConnectClient_isLoggedIn
  (JNIEnv *, jobject)
{
    return (jboolean)rho_sync_logged_in() == 1;
}

RHO_GLOBAL jobject JNICALL Java_com_rhomobile_rhoconnect_RhoConnectClient_loginWithUserSync
  (JNIEnv * env, jobject, jstring juser, jstring jpass)
{
    char* res = reinterpret_cast<char*>(rho_sync_login(rho_cast<std::string>(env, juser).c_str(),
                                                       rho_cast<std::string>(env, jpass).c_str(),
                                                       ""));
    jobject jNotify = rho::connect_jni::rhoconnect_jni_parsenotify(env, res);
    rho_sync_free_string(res);
    return jNotify;
}
//----------------------------------------------------------------------------------------------------------------------

template <int JAVACLASS>
int rhoconnect_jni_callback(const char* res,
                            jobject jNotifyDelegate,
                            jobject (*parse_notify)(JNIEnv*, const char*),
                            const char* param_sig)
{
    static jclass clsDelegate = getJNIClass(JAVACLASS);
    if (!clsDelegate) {
        LOG(ERROR) + "Cannot get Java delegate class";
        return 1;
    }
    
    JNIEnv * env = jnienv();
    static jmethodID midCall = getJNIClassMethod(env, clsDelegate, "call", param_sig);
    if (!midCall) {
        LOG(ERROR) + "Cannot get Java delegate 'call' method, param signature: " + param_sig;
        return 1;
    }

    jobject jNotify = parse_notify(env, res);
    if (!jNotify) {
        LOG(ERROR) + "Parsing notify object failed";
        return 1;
    }

    LOG(TRACE) + "Calling Java callback";
    env->CallVoidMethod(jNotifyDelegate, midCall, jNotify);

    return 0;
}
//----------------------------------------------------------------------------------------------------------------------

int login_callback(const char* res, void* data)
{
    using namespace rho::connect_jni;
    LOG(TRACE) + "login_callback, res: " + res + ", data: " + (int)data;
    if (data == 0) {
        LOG(ERROR) + "notify_callback: data is NULL, cannot call Java callback";
        return 1;
    }
    jobject jNotifyDelegate = reinterpret_cast<jobject>(data);
    int result = rhoconnect_jni_callback<(int)RHOCONNECT_JAVA_CLASS_NOTIFY_DELEGATE>(res, jNotifyDelegate,
                                                                          &rhoconnect_jni_parsenotify,
                                                                          "(Lcom/rhomobile/rhoconnect/RhoConnectNotify;)V");
    JNIEnv * env = jnienv();
    env->DeleteGlobalRef(jNotifyDelegate);
    return result;
}
//----------------------------------------------------------------------------------------------------------------------

int notify_callback(const char* res, void* data)
{
    using namespace rho::connect_jni;
    LOG(TRACE) + "notify_callback, res: " + res + ", data: " + (int)data;
    if (data == 0) {
        LOG(ERROR) + "notify_callback: data is NULL, cannot call Java callback";
        return 1;
    }
    return rhoconnect_jni_callback<(int)RHOCONNECT_JAVA_CLASS_NOTIFY_DELEGATE>(res, reinterpret_cast<jobject>(data),
                                                                          &rhoconnect_jni_parsenotify,
                                                                          "(Lcom/rhomobile/rhoconnect/RhoConnectNotify;)V");
}
//----------------------------------------------------------------------------------------------------------------------

int object_notify_callback(const char* res, void* data)
{
    using namespace rho::connect_jni;
    LOG(TRACE) + "object_notify_callback, res: " + res + ", data: " + (int)data;
    if (data == 0) {
        LOG(ERROR) + "object_notify_callback: data is NULL, cannot call Java callback";
        return 1;
    }
    return rhoconnect_jni_callback<RHOCONNECT_JAVA_CLASS_OBJECTNOTIFY_DELEGATE>(res, reinterpret_cast<jobject>(data),
                                                                                rhoconnect_jni_parseobjectnotify,
                                                                                "(Lcom/rhomobile/rhoconnect/RhoConnectObjectNotify;)V");
}
//----------------------------------------------------------------------------------------------------------------------

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhoconnect_RhoConnectClient_loginWithUserAsync
  (JNIEnv * env, jobject, jstring jUser, jstring jPass, jobject jref)
{
    jobject jNotifyDelegate = env->NewGlobalRef(jref);

    rho_sync_login_c(rho_cast<std::string>(env, jUser).c_str(),
                     rho_cast<std::string>(env, jPass).c_str(),
                     reinterpret_cast<void*>(login_callback), jNotifyDelegate);
}
//----------------------------------------------------------------------------------------------------------------------

RHO_GLOBAL jobject JNICALL Java_com_rhomobile_rhoconnect_RhoConnectClient_syncAll
  (JNIEnv * env, jobject)
{
    char* res = reinterpret_cast<char*>(rho_sync_doSyncAllSources(0, ""));
    jhobject jhNotify = rho::connect_jni::rhoconnect_jni_parsenotify(env, res);
    rho_sync_free_string(res);
    return jhNotify.release();
}
//----------------------------------------------------------------------------------------------------------------------

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhoconnect_RhoConnectClient_setObjectNotification
  (JNIEnv * env, jobject, jobject jref)
{
    if(s_jObjectNotifyDelegate)
        env->DeleteGlobalRef(s_jObjectNotifyDelegate);

    s_jObjectNotifyDelegate = env->NewGlobalRef(jref);
    rho_sync_setobjectnotify_url_c(reinterpret_cast<void*>(object_notify_callback), s_jObjectNotifyDelegate);
}
//----------------------------------------------------------------------------------------------------------------------

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhoconnect_RhoConnectClient_clearObjectNotification
  (JNIEnv * env, jobject)
{
    rho_sync_clear_object_notification();
    if(s_jObjectNotifyDelegate)
    {
        env->DeleteGlobalRef(s_jObjectNotifyDelegate);
        s_jObjectNotifyDelegate = 0;
    }
}
//----------------------------------------------------------------------------------------------------------------------
RHO_GLOBAL void JNICALL Java_com_rhomobile_rhoconnect_RhoConnectClient_addObjectNotify
  (JNIEnv * env, jobject, jint jSrcId, jstring jObject)
{
    std::string object = rho_cast<std::string>(env, jObject);
    rho_sync_addobjectnotify(jSrcId, object.c_str());
}
//----------------------------------------------------------------------------------------------------------------------

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhoconnect_RhoConnectClient_setNotification
  (JNIEnv * env, jobject, jobject jref)
{
    if(s_jNotifyDelegate)
        env->DeleteGlobalRef(s_jNotifyDelegate);

    s_jNotifyDelegate = env->NewGlobalRef(jref);
    rho_sync_set_notification_c(-1, reinterpret_cast<void*>(notify_callback), s_jNotifyDelegate);
}
//----------------------------------------------------------------------------------------------------------------------

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhoconnect_RhoConnectClient_clearNotification
  (JNIEnv * env, jobject)
{
    rho_sync_clear_notification(-1);
    if(s_jNotifyDelegate)
    {
        env->DeleteGlobalRef(s_jNotifyDelegate);
        s_jNotifyDelegate = 0;
    }
}
//----------------------------------------------------------------------------------------------------------------------

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhoconnect_RhoConnectClient_nativeInit
  (JNIEnv * env, jclass)
{
    android_setup(env);
}
