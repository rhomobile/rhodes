/***************************************************************************
 *                                  _   _ ____  _
 *  Project                     ___| | | |  _ \| |
 *                             / __| | | | |_) | |
 *                            | (__| |_| |  _ <| |___
 *                             \___|\___/|_| \_\_____|
 *
 * Copyright (C) 1998 - 2009, Daniel Stenberg, <daniel@haxx.se>, et al.
 *
 * This software is licensed as described in the file COPYING, which
 * you should have received as part of this distribution. The terms
 * are also available at http://curl.haxx.se/docs/copyright.html.
 *
 * You may opt to use, copy, modify, merge, publish, distribute and/or sell
 * copies of the Software, and permit persons to whom the Software is
 * furnished to do so, under the terms of the COPYING file.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 * $Id: ssluse.c,v 1.243 2009-10-14 02:32:27 gknauf Exp $
 ***************************************************************************/

/*
 * Source file for all Rhodes-specific code for the TLS/SSL layer. No code
 * but sslgen.c should ever call or use these functions.
 */

#include "curl_setup.h"

#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#ifdef HAVE_LIMITS_H
#include <limits.h>
#endif
#ifdef HAVE_SYS_SOCKET_H
#include <sys/socket.h>
#endif

#include "urldata.h"
#include "sendf.h"
#include "formdata.h" /* for the boundary function */
#include "url.h" /* for the ssl config check function */
#include "inet_pton.h"
//#include "ssluse.h"
#include "connect.h"
//#include "strequal.h"
#include "select.h"
#include "vtls/vtls.h"
//#include "rawstr.h"

#define _MPRINTF_REPLACE /* use the internal *printf() functions */
#include <curl/mprintf.h>
#include <assert.h>

#ifdef USE_RHOSSL

#include "rhossl.h"

extern void * rho_ssl_create_storage();
extern void rho_ssl_free_storage(void *);
extern CURLcode rho_ssl_connect(int sockfd, int nonblocking, int *done, int ssl_verify_peer, void *storage, char* host_name);
extern void rho_ssl_shutdown(void *storage);
extern ssize_t rho_ssl_send(const void *mem, size_t len, void *storage);
extern ssize_t rho_ssl_recv(char *buf, size_t size, int *wouldblock, void *storage);
extern bool rho_ssl_rand(unsigned char *entropy, size_t length);

/**
 * Global SSL init
 *
 * @retval 0 error initializing SSL
 * @retval 1 SSL initialized successfully
 */
int Curl_rhossl_init(void)
{
    return 1;
}

void Curl_rhossl_cleanup(void)
{
}

static CURLcode rhossl_connect_common(struct connectdata *conn, int sockindex,
                                      bool nonblocking, bool *done)
{
    curl_socket_t sockfd = conn->sock[sockindex];
    struct ssl_connect_data *connssl = &conn->ssl[sockindex];
    struct ssl_config_data *config = &conn->ssl_config;
    CURLcode retcode;
	char host[255];

	int idone = *done;

    if (connssl->state == ssl_connection_complete)
        return CURLE_OK;
    
    connssl->storage = rho_ssl_create_storage();

	sprintf(host,"%s:%d\0", conn->host.name, conn->port); 

	retcode = rho_ssl_connect(sockfd, nonblocking, &idone, config->primary.verifypeer, connssl->storage, host);
    if (retcode)
        return retcode;
    
    *done = (bool)idone;
    connssl->state = ssl_connection_complete;
    conn->recv[sockindex] = Curl_rhossl_recv;
    conn->send[sockindex] = Curl_rhossl_send;
    
    return retcode;
}

CURLcode Curl_rhossl_connect_nonblocking(struct connectdata *conn,
                                         int sockindex,
                                         bool *done)
{
    return rhossl_connect_common(conn, sockindex, TRUE, done);
}

CURLcode Curl_rhossl_connect(struct connectdata *conn,
                             int sockindex)
{
    CURLcode retcode;
    bool done = FALSE;
    retcode = rhossl_connect_common(conn, sockindex, FALSE, &done);
    if (retcode)
        return retcode;
    
    DEBUGASSERT(done);
    return CURLE_OK;
}

void Curl_rhossl_session_free(void *ptr)
{
    free(ptr);
}

int Curl_rhossl_close_all(struct Curl_easy *data)
{
    (void)data;
    return 0;
}

void Curl_rhossl_close(struct connectdata *conn, int sockindex)
{
    Curl_rhossl_shutdown(conn, sockindex);
}

int Curl_rhossl_shutdown(struct connectdata *conn, int sockindex)
{
    struct ssl_connect_data *connssl = &conn->ssl[sockindex];
    if(connssl->storage != 0) {
        rho_ssl_shutdown(connssl->storage);
        rho_ssl_free_storage(connssl->storage);
        connssl->storage = NULL;
    }
    return 0;
}

#if defined(macintosh) || defined(__APPLE__) || defined(__APPLE_CC__)

ssize_t Curl_rhossl_send(struct connectdata *conn, int sockindex, const void *mem, size_t len, CURLcode *err)
{
    struct ssl_connect_data *connssl = &conn->ssl[sockindex];
    *err = CURLE_OK;
    return rho_ssl_send(mem, len, connssl->storage);
}

ssize_t Curl_rhossl_recv(struct connectdata *conn, int sockindex, char *buf, size_t size, CURLcode *err)
{
    struct ssl_connect_data *connssl = &conn->ssl[sockindex];
    int iw = 0;
    ssize_t ret = rho_ssl_recv(buf, size, &iw, connssl->storage);
    *err = CURLE_OK;
    return ret;
}

#else

ssize_t Curl_rhossl_send(struct connectdata *conn, int sockindex, const void *mem, size_t len)
{
    struct ssl_connect_data *connssl = &conn->ssl[sockindex];
    return rho_ssl_send(mem, len, connssl->storage);
}

ssize_t Curl_rhossl_recv(struct connectdata *conn, int sockindex, char *buf, size_t size, bool *wouldblock)
{
    struct ssl_connect_data *connssl = &conn->ssl[sockindex];
    int iw = *wouldblock;
    ssize_t ret = rho_ssl_recv(buf, size, &iw, connssl->storage);
    *wouldblock = (bool)iw;
    return ret;
}

#endif

size_t Curl_rhossl_version(char *buffer, size_t size)
{
    return snprintf(buffer, size, "RhoSSL/1.0");
}

CURLcode Curl_rhossl_random(struct Curl_easy *data, unsigned char *entropy,
                          size_t length)
{
    assert(entropy);
    bool result = rho_ssl_rand(entropy, length);
	return result ? CURLE_OK : CURLE_FAILED_INIT;
}

#endif /* USE_RHOSSL */
