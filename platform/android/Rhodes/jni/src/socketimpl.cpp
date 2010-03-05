#include "JNIRhodes.h"

#include <com_rhomobile_rhodes_socket_RhoSocketImpl.h>

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

RHO_GLOBAL jobject JNICALL Java_com_rhomobile_rhodes_socket_RhoSocketImpl_getRemoteHostImpl
  (JNIEnv *env, jobject, jint sock)
{
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_INET4ADDRESS);
    if (!cls) return NULL;
    jmethodID mid = getJNIClassMethod(env, cls, "<init>", "([B)V");
    if (!mid) return NULL;

    jbyteArray array = env->NewByteArray(4);
    if (!array) return NULL;
    jbyte *arr = env->GetByteArrayElements(array, NULL);
    if (!arr) return NULL;

    sockaddr_in sa;
    socklen_t salen = sizeof(sa);
    int e = ::getpeername(sock, (sockaddr*)&sa, &salen);
    if (e == -1) {
        RAWLOG_ERROR1("getpeername failed: %d", errno);
        return NULL;
    }
    uint32_t addr = ntohl(sa.sin_addr.s_addr);
    arr[3] = (jbyte)(addr & 0xFF);
    addr >>= 8;
    arr[2] = (jbyte)(addr & 0xFF);
    addr >>= 8;
    arr[1] = (jbyte)(addr & 0xFF);
    addr >>= 8;
    arr[0] = (jbyte)(addr & 0xFF);
    env->ReleaseByteArrayElements(array, arr, 0);

    jobject result = env->NewObject(cls, mid, array);
    return result;
}

RHO_GLOBAL jint JNICALL Java_com_rhomobile_rhodes_socket_RhoSocketImpl_getRemotePortImpl
  (JNIEnv *, jobject, jint sock)
{
    sockaddr_in sa;
    socklen_t salen = sizeof(sa);
    int e = ::getpeername(sock, (sockaddr*)&sa, &salen);
    if (e == -1) {
        RAWLOG_ERROR1("getpeername failed: %d", errno);
        return 0;
    }
    return ntohs(sa.sin_port);
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
