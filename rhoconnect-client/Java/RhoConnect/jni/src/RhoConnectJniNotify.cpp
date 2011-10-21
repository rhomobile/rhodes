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

#include "RhoConnectJniNotify.h"

namespace rho { namespace connect_jni {

jobject rhoconnect_jni_parsenotify(JNIEnv * env, const char* res)
{
    LOG(TRACE) + "rhoconnect_jni_parsenotify: " + res;

    RHO_CONNECT_NOTIFY notify;
    memset(&notify, 0, sizeof(notify));

    rho_connectclient_parsenotify(res, &notify);

    static jclass clsNotify = getJNIClass(RHOCONNECT_JAVA_CLASS_NOTIFY);
    if (!clsNotify) return NULL;

    static jmethodID midNotify = getJNIClassMethod(env, clsNotify, "<init>", "()V");
    if (!midNotify) return NULL;
    static jfieldID fidTotalCount = getJNIClassField(env, clsNotify, "mTotalCount", "I");
    if (!fidTotalCount) return NULL;
    static jfieldID fidProcCount = getJNIClassField(env, clsNotify, "mProcessedCount", "I");
    if (!fidProcCount) return NULL;
    static jfieldID fidCumulativeCount = getJNIClassField(env, clsNotify, "mCumulativeCount", "I");
    if (!fidCumulativeCount) return NULL;
    static jfieldID fidSrcId = getJNIClassField(env, clsNotify, "mSourceId", "I");
    if (!fidSrcId) return NULL;
    static jfieldID fidErrorCode = getJNIClassField(env, clsNotify, "mErrorCode", "I");
    if (!fidErrorCode) return NULL;
    static jfieldID fidSrcName = getJNIClassField(env, clsNotify, "mSourceName", "Ljava/lang/String;");
    if (!fidSrcName) return NULL;
    static jfieldID fidStatus = getJNIClassField(env, clsNotify, "mStatus", "Ljava/lang/String;");
    if (!fidStatus) return NULL;
    static jfieldID fidSyncType = getJNIClassField(env, clsNotify, "mSyncType", "Ljava/lang/String;");
    if (!fidSyncType) return NULL;
    static jfieldID fidErrMsg = getJNIClassField(env, clsNotify, "mErrorMessage", "Ljava/lang/String;");
    if (!fidErrMsg) return NULL;
    static jfieldID fidParams = getJNIClassField(env, clsNotify, "mParams", "Ljava/lang/String;");
    if (!fidParams) return NULL;

    jobject jNotify = env->NewObject(clsNotify, midNotify);
    if (!jNotify) return NULL;

    env->SetIntField(jNotify, fidTotalCount, notify.total_count);
    env->SetIntField(jNotify, fidProcCount, notify.processed_count);
    env->SetIntField(jNotify, fidCumulativeCount, notify.cumulative_count);
    env->SetIntField(jNotify, fidSrcId, notify.source_id);
    env->SetIntField(jNotify, fidErrorCode, notify.error_code);

    jhstring jhSrcName = rho_cast<jhstring>(env, notify.source_name);
    jhstring jhStatus = rho_cast<jhstring>(env, notify.status);
    jhstring jhSyncType = rho_cast<jhstring>(env, notify.sync_type);
    jhstring jhErrMsg = rho_cast<jhstring>(env, notify.error_message);
    jhstring jhParams = rho_cast<jhstring>(env, notify.callback_params);

    env->SetObjectField(jNotify, fidSrcName, jhSrcName.get());
    env->SetObjectField(jNotify, fidStatus, jhStatus.get());
    env->SetObjectField(jNotify, fidSyncType, jhSyncType.get());
    env->SetObjectField(jNotify, fidErrMsg, jhErrMsg.get());
    env->SetObjectField(jNotify, fidParams, jhParams.get());

    rho_connectclient_free_syncnotify(&notify);

    return jNotify;

}
//----------------------------------------------------------------------------------------------------------------------
void rhoconnect_jni_fill_objectnotify(JNIEnv * env, jobject jNotify, int cnt, const char* const * objects, int* src_ids,
                                      jfieldID fidObjects, jfieldID fidSrcIds)
{
    static jclass clsString = getJNIClass(RHODES_JAVA_CLASS_STRING);
    if (!clsString) return;

    LOG(TRACE) + "rhoconnect_jni_fill_objectnotify: cnt: " + cnt + ", Java notify reference: " + (int)jNotify;

    if(cnt > 0)
    {
        jobjectArray jObjects = env->NewObjectArray(cnt, clsString, NULL);
        if (!jObjects) return;
        jintArray jSrcIds = env->NewIntArray(cnt);
        if (!jSrcIds) return;

        for(int i = 0; i < cnt; ++i)
        {
            jhstring jhObject = rho_cast<jhstring>(env, objects[i]);
            env->SetObjectArrayElement(jObjects, i, jhObject.release());
        }
        env->SetIntArrayRegion(jSrcIds, 0, cnt, src_ids);

        env->SetObjectField(jNotify, fidObjects, jObjects);
        env->SetObjectField(jNotify, fidSrcIds, jSrcIds);
    }
}

//----------------------------------------------------------------------------------------------------------------------

jobject rhoconnect_jni_parseobjectnotify(JNIEnv * env, const char* res)
{
    LOG(TRACE) + "rhoconnect_jni_parseobjectnotify: " + res;

    RHO_CONNECT_OBJECT_NOTIFY notify;
    memset(&notify, 0, sizeof(notify));

    rho_connectclient_parse_objectnotify(res, &notify);

    static jclass clsNotify = getJNIClass(RHOCONNECT_JAVA_CLASS_OBJECTNOTIFY);
    if (!clsNotify) {
        LOG(ERROR) + "Cannot get RhoConnectObjectNotify class";
        return NULL;
    }
    static jmethodID midNotify = getJNIClassMethod(env, clsNotify, "<init>", "()V");
    if (!midNotify) {
        LOG(ERROR) + "Cannot get RhoConnectObjectNotify constructor method id";
        return NULL;
    }
    static jfieldID fidDeletedObjects = getJNIClassField(env, clsNotify, "mDeletedObjects", "[Ljava/lang/String;");
    if (!fidDeletedObjects) {
        LOG(ERROR) + "Cannot get RhoConnectObjectNotify.mDeletedObjects field id";
        return NULL;
    }
    static jfieldID fidUpdatedObjects = getJNIClassField(env, clsNotify, "mUpdatedObjects", "[Ljava/lang/String;");
    if (!fidUpdatedObjects) {
        LOG(ERROR) + "Cannot get RhoConnectObjectNotify.mUpdatedObjects field id";
        return NULL;
    }
    static jfieldID fidCreatedObjects = getJNIClassField(env, clsNotify, "mCreatedObjects", "[Ljava/lang/String;");
    if (!fidCreatedObjects) {
        LOG(ERROR) + "Cannot get RhoConnectObjectNotify.mCreatedObjects field id";
        return NULL;
    }
    static jfieldID fidDeletedIds = getJNIClassField(env, clsNotify, "mDeletedSrcIds", "[I");
    if (!fidDeletedIds) {
        LOG(ERROR) + "Cannot get RhoConnectObjectNotify.mDeletedSrcIds field id";
        return NULL;
    }
    static jfieldID fidUpdatedIds = getJNIClassField(env, clsNotify, "mUpdatedSrcIds", "[I");
    if (!fidUpdatedIds) {
        LOG(ERROR) + "Cannot get RhoConnectObjectNotify.mUpdatedSrcIds field id";
        return NULL;
    }
    static jfieldID fidCreatedIds = getJNIClassField(env, clsNotify, "mCreatedSrcIds", "[I");
    if (!fidCreatedIds) {
        LOG(ERROR) + "Cannot get RhoConnectObjectNotify.mCreatedSrcIds field id";
        return NULL;
    }

    jobject jNotify = env->NewObject(clsNotify, midNotify);
    if (!jNotify) {
        LOG(ERROR) + "Cannot create RhoConnectObjectNotify instance";
        return NULL;
    }

    rhoconnect_jni_fill_objectnotify(env, jNotify, notify.deleted_count,
                                     notify.deleted_objects, notify.deleted_source_ids,
                                     fidDeletedObjects, fidDeletedIds);

    rhoconnect_jni_fill_objectnotify(env, jNotify, notify.updated_count,
                                     notify.updated_objects, notify.updated_source_ids,
                                     fidUpdatedObjects, fidUpdatedIds);

    rhoconnect_jni_fill_objectnotify(env, jNotify, notify.created_count,
                                     notify.created_objects, notify.created_source_ids,
                                     fidCreatedObjects, fidCreatedIds);

    rho_connectclient_free_sync_objectnotify(&notify);

    LOG(TRACE) + "Returning Java RhoConnectObjectNotify reference: " + (int)jNotify;
    return jNotify;
}

}}
