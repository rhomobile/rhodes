#ifndef RHO_SSL_IMPL_EC601E9D39EFA5CBA3F57583992797A
#define RHO_SSL_IMPL_EC601E9D39EFA5CBA3F57583992797A

#include <net/ssl.h>
#include <jni.h>

namespace rho {
namespace net {

class SSLImpl : public ISSL
{
public:
    SSLImpl();

    void* createStorage();
    void freeStorage(void *ptr);

    CURLcode connect(int sockfd, int nonblocking, int *done, int ssl_verify_peer, void *storage);
    void shutdown(void *storage);
    ssize_t send(const void *mem, size_t len, void *storage);
    ssize_t recv(char *buf, size_t size, int *wouldblock, void *storage);

private:
    jclass cls;
    jmethodID midConstructor;
    jmethodID midConnect;
    jmethodID midShutdown;
    jmethodID midSend;
    jmethodID midRecv;
};

}
}

#endif // RHO_SSL_IMPL_EC601E9D39EFA5CBA3F57583992797A

