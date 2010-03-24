/*
 *  sslimpl.h
 *  rhorunner
 *
 *  Created by Dmitry Moskalchuk on 19.02.10.
 *  Copyright 2010 Rhomobile. All rights reserved.
 *
 */

#pragma once

#include "net/ssl.h"

namespace rho
{
namespace net
{

class SSLImpl : public ISSL
{
public:
    void *createStorage();
    void freeStorage(void *ptr);
    
    CURLcode connect(int sockfd, int nonblocking, int *done, int ssl_verify_peer, void *storage);
    void shutdown(void *storage);
    ssize_t send(const void *mem, size_t len, void *storage);
    ssize_t recv(char *buf, size_t size, int *wouldblock, void *storage);
};

} // namespace net
} // namespace rho
