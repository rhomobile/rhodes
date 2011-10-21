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
