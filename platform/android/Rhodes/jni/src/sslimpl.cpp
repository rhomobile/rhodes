#include "rhodes/sslimpl.h"
#include "rhodes/JNIRhodes.h"

#include "rhodes/jni/com_rhomobile_rhodes_socket_SSLImpl.h"

#include <arpa/inet.h>

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "SSLImpl"

RHO_GLOBAL jobject JNICALL Java_com_rhomobile_rhodes_socket_SSLImpl_getRemoteSockAddr
  (JNIEnv *env, jobject, jint sock)
{
    jclass clsSockAddr = getJNIClass(RHODES_JAVA_CLASS_RHOSOCKADDR);
    if (!clsSockAddr) return NULL;
    jclass clsInetAddr = getJNIClass(RHODES_JAVA_CLASS_INET4ADDRESS);
    if (!clsInetAddr) return NULL;
    jmethodID midSockAddr = getJNIClassMethod(env, clsSockAddr, "<init>", "()V");
    if (!midSockAddr) return NULL;
    jmethodID midInetAddr = getJNIClassMethod(env, clsInetAddr, "<init>", "([BLjava/lang/String;)V");
    if (!midInetAddr) return NULL;
    jfieldID fidInetAddr = getJNIClassField(env, clsSockAddr, "host", "Ljava/net/InetAddress;");
    if (!fidInetAddr) return NULL;
    jfieldID fidPort = getJNIClassField(env, clsSockAddr, "port", "I");
    if (!fidPort) return NULL;

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

    jobject inetaddrObj = env->NewObject(clsInetAddr, midInetAddr, array, rho_cast<jstring>(::inet_ntoa(sa.sin_addr)));
    if (!inetaddrObj) return NULL;

    jobject sockaddrObj = env->NewObject(clsSockAddr, midSockAddr);
    if (!sockaddrObj) return NULL;

    env->SetObjectField(sockaddrObj, fidInetAddr, inetaddrObj);
    env->SetIntField(sockaddrObj, fidPort, ntohs(sa.sin_port));

    return sockaddrObj;
}

namespace rho
{
namespace net
{

SSLImpl::SSLImpl()
{
    JNIEnv *env = jnienv();
    cls = getJNIClass(RHODES_JAVA_CLASS_SSLIMPL);
    if (!cls) return;
    midConstructor = getJNIClassMethod(env, cls, "<init>", "()V");
    if (!midConstructor) return;
    midConnect = getJNIClassMethod(env, cls, "connect", "(IZ)Z");
    if (!midConnect) return;
    midShutdown = getJNIClassMethod(env, cls, "shutdown", "()V");
    if (!midShutdown) return;
    midSend = getJNIClassMethod(env, cls, "send", "([B)Z");
    if (!midSend) return;
    midRecv = getJNIClassMethod(env, cls, "recv", "([B)I");
    if (!midRecv) return;
}

void* SSLImpl::createStorage()
{
    JNIEnv *env = jnienv();
    jobject obj = env->NewObject(cls, midConstructor);
    env->NewGlobalRef(obj);
    return obj;
}

void SSLImpl::freeStorage(void *ptr)
{
    if (!ptr) return;
    jobject obj = (jobject)ptr;
    jnienv()->DeleteGlobalRef(obj);
}

CURLcode SSLImpl::connect(int sockfd, int nonblocking, int *done, int ssl_verify_peer, void *storage)
{
    if (!storage) return CURLE_SSL_CONNECT_ERROR;

    jobject obj = (jobject)storage;
    jboolean result = jnienv()->CallBooleanMethod(obj, midConnect, sockfd, (jboolean)ssl_verify_peer);
    if (!result)
        return CURLE_SSL_CONNECT_ERROR;

    *done = 1;
    return CURLE_OK;
}

void SSLImpl::shutdown(void *storage)
{
    if (!storage) return;
    jobject obj = (jobject)storage;
    jnienv()->CallVoidMethod(obj, midShutdown);
}

ssize_t SSLImpl::send(const void *mem, size_t len, void *storage)
{
    if (!storage) return -1;

    JNIEnv *env = jnienv();
    jbyteArray array = env->NewByteArray(len);
    if (!array) return -1;
    jbyte *arr = env->GetByteArrayElements(array, NULL);
    if (!arr) return -1;
    std::memmove(arr, mem, len);
    env->ReleaseByteArrayElements(array, arr, 0);

    jobject obj = (jobject)storage;
    jboolean result = env->CallBooleanMethod(obj, midSend, array);
    if (!result) return -1;
    return len;
}

ssize_t SSLImpl::recv(char *buf, size_t size, int *wouldblock, void *storage)
{
    *wouldblock = 0;
    if (!storage) return -1;

    jobject obj = (jobject)storage;
    JNIEnv *env = jnienv();
    jclass cls = getJNIObjectClass(env, obj);
    if (!cls) return -1;
    jfieldID fid = getJNIClassField(env, cls, "sockfd", "I");
    if (!fid) return -1;

    jint sock = env->GetIntField(obj, fid);

    fd_set rfd;
    FD_ZERO(&rfd);
    FD_SET(sock, &rfd);
    timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    int n = ::select(sock + 1, &rfd, NULL, NULL, &tv);
    if (n == 0) {
        *wouldblock = 1;
        return -1;
    }

    jbyteArray array = env->NewByteArray(size);
    jint result = env->CallIntMethod(obj, midRecv, array);

    if (result > 0) {
        jbyte *arr = env->GetByteArrayElements(array, NULL);
        if (!arr) return -1;
        std::memmove(buf, arr, result);
        env->ReleaseByteArrayElements(array, arr, JNI_ABORT);
    }
    return result;
}

} // namespace net
} // namespace rho

