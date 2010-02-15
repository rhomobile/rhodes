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
 * Source file for all iPhone-specific code for the TLS/SSL layer. No code
 * but sslgen.c should ever call or use these functions.
 */

#include "setup.h"

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
#include "ssluse.h"
#include "connect.h"
#include "strequal.h"
#include "select.h"
#include "sslgen.h"
#include "rawstr.h"

#define _MPRINTF_REPLACE /* use the internal *printf() functions */
#include <curl/mprintf.h>

#ifdef USE_SSL_IPHONE

#include "ssl_iphone.h"

/**
 * Global SSL init
 *
 * @retval 0 error initializing SSL
 * @retval 1 SSL initialized successfully
 */
int Curl_ssl_iphone_init(void)
{
  return 1;
}

void Curl_ssl_iphone_cleanup(void)
{
}

CURLcode Curl_ssl_iphone_connect_nonblocking(struct connectdata *conn,
                                             int sockindex,
                                             bool *done)
{
  // TODO: implement
  return CURLE_OK;
}

CURLcode Curl_ssl_iphone_connect(struct connectdata *conn,
                                 int sockindex)
{
  // TODO: implement
  return CURLE_OK;
}

void Curl_ssl_iphone_session_free(void *ptr)
{
  // TODO: implement
}

int Curl_ssl_iphone_close_all(struct SessionHandle *data)
{
  // TODO: implement
  return 0;
}

void Curl_ssl_iphone_close(struct connectdata *conn, int sockindex)
{
  // TODO: implement
}

int Curl_ssl_iphone_shutdown(struct connectdata *conn, int sockindex)
{
  // TODO: implement
  return 0;
}

CURLcode Curl_ssl_iphone_set_engine(struct SessionHandle *data, const char *engine)
{
  // TODO: implement
  return CURLE_OK;
}

CURLcode Curl_ssl_iphone_set_engine_default(struct SessionHandle *data)
{
  // TODO: implement
  return CURLE_OK;
}

struct curl_slist *Curl_ssl_iphone_engines_list(struct SessionHandle *data)
{
  // TODO: implement
  return NULL;
}

ssize_t Curl_ssl_iphone_send(struct connectdata *conn, int sockindex, const void *mem, size_t len)
{
  // TODO: implement
  return -1;
}

ssize_t Curl_ssl_iphone_recv(struct connectdata *conn, int sockindex, char *buf, size_t size, bool *wouldblock)
{
  // TODO: implement
  return -1;
}

size_t Curl_ssl_iphone_version(char *buffer, size_t size)
{
  // TODO: implement
  return 0;
}

int Curl_ssl_iphone_check_cxn(struct connectdata *cxn)
{
  // TODO: implement
  return 0;
}

bool Curl_ssl_iphone_data_pending(const struct connectdata *conn,
                                  int connindex)
{
  // TODO: implement
  return 0;
}

#endif /* USE_SSL_IPHONE */
