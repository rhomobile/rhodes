#ifndef __RHO_SSL_H
#define __RHO_SSL_H
/***************************************************************************
 *                                  _   _ ____  _
 *  Project                     ___| | | |  _ \| |
 *                             / __| | | | |_) | |
 *                            | (__| |_| |  _ <| |___
 *                             \___|\___/|_| \_\_____|
 *
 * Copyright (C) 1998 - 2008, Daniel Stenberg, <daniel@haxx.se>, et al.
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
 ***************************************************************************/
#include "curl_setup.h"
 
#ifdef USE_RHOSSL
/*
 * This header should only be needed to get included by sslgen.c and rhossl.c
 */

#include "urldata.h"
CURLcode Curl_rhossl_connect(struct connectdata *conn, int sockindex);
CURLcode Curl_rhossl_connect_nonblocking(struct connectdata *conn,
                                         int sockindex,
                                         bool *done);

/* close a SSL connection */
void Curl_rhossl_close(struct connectdata *conn, int sockindex);

/* tell SSL engine to close down all open information regarding connections (and
   thus session ID caching etc) */
int Curl_rhossl_close_all(struct Curl_easy *data);

/* function provided for the generic SSL-layer, called when a session id
   should be freed */
void Curl_rhossl_session_free(void *ptr);

int Curl_rhossl_init(void);
void Curl_rhossl_cleanup(void);

#if defined(macintosh) || defined(__APPLE__) || defined(__APPLE_CC__) || defined(ANDROID) || defined(OS_LINUX)

ssize_t Curl_rhossl_send(struct connectdata *conn, 
                         int sockindex,
                         const void *mem,
                         size_t len,
                         CURLcode *err);
ssize_t Curl_rhossl_recv(struct connectdata *conn, 
                         int sockindex,
                         char *buf,
                         size_t size,
                         CURLcode *err);

#else

ssize_t Curl_rhossl_send(struct connectdata *conn,
                         int sockindex,
                         const void *mem,
                         size_t len);
ssize_t Curl_rhossl_recv(struct connectdata *conn,
                         int sockindex,
                         char *buf,
                         size_t size,
                         bool *wouldblock);

#endif

size_t Curl_rhossl_version(char *buffer, size_t size);

int Curl_rhossl_shutdown(struct connectdata *conn, int sockindex);

CURLcode Curl_rhossl_random(struct Curl_easy *data, unsigned char *entropy,
                          size_t length);

#define CURL_SSL_BACKEND CURLSSLBACKEND_RHOSSL

/* API setup for OpenSSL */
#define curlssl_init Curl_rhossl_init
#define curlssl_cleanup Curl_rhossl_cleanup
#define curlssl_connect Curl_rhossl_connect
#define curlssl_connect_nonblocking Curl_rhossl_connect_nonblocking
#define curlssl_session_free(x) Curl_rhossl_session_free(x)
#define curlssl_close_all Curl_rhossl_close_all
#define curlssl_close Curl_rhossl_close
#define curlssl_shutdown(x,y) Curl_rhossl_shutdown(x,y)
#define curlssl_set_engine(x,y) (x=x, y=y, CURLE_FAILED_INIT)
#define curlssl_set_engine_default(x) (x=x, CURLE_FAILED_INIT)
#define curlssl_engines_list(x) (x=x, (struct curl_slist *)NULL)
#define curlssl_send Curl_rhossl_send
#define curlssl_recv Curl_rhossl_recv
#define curlssl_version Curl_rhossl_version
#define curlssl_check_cxn(x) (x=x, -1)
#define curlssl_data_pending(x,y) (x=x, y=y, 0)
#define curlssl_random(x,y,z) Curl_rhossl_random(x,y,z)

#endif /* USE_RHOSSL */
#endif /* __RHO_SSL_H */
