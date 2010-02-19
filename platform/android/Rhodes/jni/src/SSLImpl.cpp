#include "SSLImpl.h"

namespace rho
{
namespace net
{

void* SSLImpl::createStorage()
{
    // TODO: implement
    return NULL;
}

void SSLImpl::freeStorage(void *ptr)
{
    // TODO: implement
}

CURLcode SSLImpl::connect(int sockfd, int nonblocking, int *done, void *storage)
{
    // TODO: implement
    return CURLE_SSL_CONNECT_ERROR;
}

void SSLImpl::shutdown(void *storage)
{
    // TODO: implement
}

ssize_t SSLImpl::send(const void *mem, size_t len, void *storage)
{
    // TODO: implement
    return -1;
}

ssize_t SSLImpl::recv(char *buf, size_t size, int *wouldblock, void *storage)
{
    // TODO: implement
    return -1;
}

} // namespace net
} // namespace rho

