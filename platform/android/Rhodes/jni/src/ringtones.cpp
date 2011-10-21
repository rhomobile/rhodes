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
#define DEFAULT_LOGCATEGORY "Ringtone"

RHO_GLOBAL VALUE rho_ringtone_manager_get_all()
{
    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_RINGTONE_MANAGER);
    if (!cls) return Qnil;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "getAllRingtones", "()Ljava/util/Map;");
    if (!mid) return Qnil;

    jobject obj = env->CallStaticObjectMethod(cls, mid);
    if (!obj) return Qnil;
    return rho_cast<VALUE>(obj);
}

RHO_GLOBAL void rho_ringtone_manager_stop()
{
    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_RINGTONE_MANAGER);
    if (!cls) return;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "stop", "()V");
    if (!mid) return;
    env->CallStaticVoidMethod(cls, mid);
}

RHO_GLOBAL void rho_ringtone_manager_play(char* file_name)
{
    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_RINGTONE_MANAGER);
    if (!cls) return;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "play", "(Ljava/lang/String;)V");
    if (!mid) return;
    jhstring objFileName = rho_cast<jhstring>(file_name);
    env->CallStaticVoidMethod(cls, mid, objFileName.get());
}

