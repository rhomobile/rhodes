#pragma once

#include "common/RhoStd.h"
#include "common/AutoPointer.h"
#include "curl/curl.h"

namespace rho {
namespace net {

struct ISSL
{
    virtual ~ISSL() {}

    virtual void* createStorage() = 0;
    virtual void freeStorage(void *ptr) = 0;

    virtual CURLcode connect(int sockfd, int nonblocking, int *done, int ssl_verify_peer, void *storage) = 0;
    virtual void shutdown(void *storage) = 0;
    virtual ssize_t send(const void *mem, size_t len, void *storage) = 0;
    virtual ssize_t recv(char *buf, size_t size, int *wouldblock, void *storage) = 0;
};

}
}

extern "C" {
void *rho_ssl_create_storage();
void rho_ssl_free_storage(void *);
CURLcode rho_ssl_connect(int sockfd, int nonblocking, int *done, int ssl_verify_peer, void *storage);
void rho_ssl_shutdown(void *storage);
ssize_t rho_ssl_send(const void *mem, size_t len, void *storage);
ssize_t rho_ssl_recv(char *buf, size_t size, int *wouldblock, void *storage);
}
