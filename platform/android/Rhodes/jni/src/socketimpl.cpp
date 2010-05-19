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
