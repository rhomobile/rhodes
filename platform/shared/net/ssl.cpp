#include "net/ssl.h"
#include "common/IRhoClassFactory.h"

#include "logging/RhoLog.h"

extern "C" {

#define GETSSL rho_get_RhoClassFactory()->createSSLEngine()

void *rho_ssl_create_storage()
{
    RAWTRACEC("rho_ssl", "rho_ssl_create_storage");
    void* storage = GETSSL->createStorage();
    RAWTRACEC1("rho_ssl", "rho_ssl_create_storage: 0x%.8X", storage);
    return storage;
}

void rho_ssl_free_storage(void *ptr)
{
    RAWTRACEC1("rho_ssl", "rho_ssl_free_storage: 0x%.8X", ptr);
    return GETSSL->freeStorage(ptr);
}

CURLcode rho_ssl_connect(int sockfd, int nonblocking, int *done, int ssl_verify_peer, void *storage)
{
    RAWTRACEC1("rho_ssl", "rho_ssl_connect: 0x%.8X", storage);
    return GETSSL->connect(sockfd, nonblocking, done, ssl_verify_peer, storage);
}

void rho_ssl_shutdown(void *storage)
{
    RAWTRACEC1("rho_ssl", "rho_ssl_shutdown: 0x%.8X", storage);
    return GETSSL->shutdown(storage);
}

ssize_t rho_ssl_send(const void *mem, size_t len, void *storage)
{
    RAWTRACEC1("rho_ssl", "rho_ssl_send: 0x%.8X", storage);
    return GETSSL->send(mem, len, storage);
}

ssize_t rho_ssl_recv(char *buf, size_t size, int *wouldblock, void *storage)
{
    RAWTRACEC1("rho_ssl", "rho_ssl_recv: 0x%.8X", storage);
    return GETSSL->recv(buf, size, wouldblock, storage);
}

}
