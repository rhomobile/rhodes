#include "net/ssl.h"
#include "common/IRhoClassFactory.h"

extern "C" {

#define GETSSL rho::common::rho_impl_createClassFactory()->createSSLEngine()

void *rho_ssl_create_storage()
{
    return GETSSL->createStorage();
}

void rho_ssl_free_storage(void *ptr)
{
    return GETSSL->freeStorage(ptr);
}

CURLcode rho_ssl_connect(int sockfd, int nonblocking, int *done, int ssl_verify_peer, void *storage)
{
    return GETSSL->connect(sockfd, nonblocking, done, ssl_verify_peer, storage);
}

void rho_ssl_shutdown(void *storage)
{
    return GETSSL->shutdown(storage);
}

ssize_t rho_ssl_send(const void *mem, size_t len, void *storage)
{
    return GETSSL->send(mem, len, storage);
}

ssize_t rho_ssl_recv(char *buf, size_t size, int *wouldblock, void *storage)
{
    return GETSSL->recv(buf, size, wouldblock, storage);
}

}
