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

//typedef std::vector<RHOM_MODEL> model_vector;
//typedef std::vector<rho::String> string_vector;

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhoconnect_RhomModel_init
  (JNIEnv * env, jobject jmodel)
{
    jclass clsmodel = getJNIClass(RHOCONNECT_JAVA_CLASS_RHOMMODEL);
    if (!clsmodel) return;

    jmethodID midmodeltype = getJNIClassMethod(env, clsmodel, "setModelType", "(I)V");
    if (!midmodeltype) return;
    jmethodID midsynctype = getJNIClassMethod(env, clsmodel, "setSyncType", "(I)V");
    if (!midsynctype) return;
    jmethodID midsyncpri = getJNIClassMethod(env, clsmodel, "setSyncPriority", "(I)V");
    if (!midsyncpri) return;
    jmethodID midpart = getJNIClassMethod(env, clsmodel, "setPartition", "(Ljava/lang/String;)V");
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
    RHO_CONNECT_NOTIFY notify;
    memset(&notify, 0, sizeof(notify));

    char* res = reinterpret_cast<char*>(rho_sync_doSyncSourceByName(rho_cast<std::string>(env, jname).c_str()));

    rho_connectclient_parsenotify(res, &notify);
    rho_sync_free_string(res);

    jclass clsNotify = getJNIClass(RHOCONNECT_JAVA_CLASS_NOTIFY);
    if (!clsNotify) return NULL;

    jmethodID midNotify = getJNIClassMethod(env, clsNotify, "<init>", "()V");
    if (!midNotify) return NULL;
    jfieldID fidErrorCode = getJNIClassField(env, clsNotify, "mErrorCode", "I");
    if (!fidErrorCode) return NULL;

    jhobject jhNotify = jhobject(env->NewObject(clsNotify, midNotify));
    if (!jhNotify) return NULL;

    env->SetIntField(jhNotify.get(), fidErrorCode, notify.error_code);

    rho_connectclient_free_syncnotify(&notify);

    return jhNotify.release();

}
//----------------------------------------------------------------------------------------------------------------------
