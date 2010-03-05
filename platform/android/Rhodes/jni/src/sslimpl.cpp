#include "sslimpl.h"
#include "JNIRhodes.h"

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
    midConnect = getJNIClassMethod(env, cls, "connect", "(I)Z");
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

CURLcode SSLImpl::connect(int sockfd, int nonblocking, int *done, void *storage)
{
    if (!storage) return CURLE_SSL_CONNECT_ERROR;

    jobject obj = (jobject)storage;
    jboolean result = jnienv()->CallBooleanMethod(obj, midConnect, sockfd);
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

