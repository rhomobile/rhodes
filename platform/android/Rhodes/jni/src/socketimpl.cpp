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

#include "rhodes/jni/com_rhomobile_rhodes_socket_RhoSocketImpl.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "RhoSocketImpl"

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_socket_RhoSocketImpl_initImpl
  (JNIEnv *env, jobject obj, jint sock)
{
    jclass cls = getJNIObjectClass(env, obj);
    if (!cls) return;
    jfieldID fid = getJNIClassField(env, cls, "fd", "Ljava/io/FileDescriptor;");
    if (!fid) return;

    jclass clsFd = getJNIClass(RHODES_JAVA_CLASS_FILEDESCRIPTOR);
    if (!clsFd) return;
    jmethodID midFdInit = getJNIClassMethod(env, clsFd, "<init>", "()V");
    if (!midFdInit) return;
    jobject fdObj = env->NewObject(clsFd, midFdInit);
    if (!fdObj) return;

    jclass fdClass = getJNIObjectClass(env, fdObj);
    if (!fdClass) return;
    jfieldID fidDescriptor = getJNIClassField(env, fdClass, "descriptor", "I");
    if (!fidDescriptor) return;

    env->SetIntField(fdObj, fidDescriptor, sock);
    env->SetObjectField(obj, fid, fdObj);
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_socket_RhoSocketImpl_closeImpl
  (JNIEnv *, jobject, jint sock)
{
    ::close(sock);
}

RHO_GLOBAL jobject JNICALL Java_com_rhomobile_rhodes_socket_RhoSocketImpl_getOptionImpl
  (JNIEnv *, jobject, jint sock, jint option)
{
    RHO_NOT_IMPLEMENTED;
    return NULL;
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_socket_RhoSocketImpl_setOptionImpl
  (JNIEnv *, jobject, jint, jint, jobject)
{
    RHO_NOT_IMPLEMENTED;
}
