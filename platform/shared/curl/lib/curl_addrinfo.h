#ifndef HEADER_CURL_ADDRINFO_H
#define HEADER_CURL_ADDRINFO_H
/***************************************************************************
 *                                  _   _ ____  _
 *  Project                     ___| | | |  _ \| |
 *                             / __| | | | |_) | |
 *                            | (__| |_| |  _ <| |___
 *                             \___|\___/|_| \_\_____|
 *
 * Copyright (C) 1998 - 2016, Daniel Stenberg, <daniel@haxx.se>, et al.
 *
 * This software is licensed as described in the file COPYING, which
 * you should have received as part of this distribution. The terms
 * are also available at https://curl.haxx.se/docs/copyright.html.
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

#ifdef HAVE_NETINET_IN_H
#  include <netinet/in.h>
#endif
#ifdef HAVE_NETDB_H
#  include <netdb.h>
#endif
#ifdef HAVE_ARPA_INET_H
#  include <arpa/inet.h>
#endif

#ifdef __VMS
#  include <in.h>
#  include <inet.h>
#  include <stdlib.h>
#endif

#ifdef OS_LINUX
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
//#include "ruby/ext/socket/addrinfo.h"
#endif

/*
 * Curl_addrinfo is our internal struct definition that we use to allow
 * consistent internal handling of this data. We use this even when the
 * system provides an addrinfo structure definition. And we use this for
 * all sorts of IPv4 and IPV6 builds.
 */

struct Curl_addrinfo {
  int                   ai_flags;
  int                   ai_family;
  int                   ai_socktype;
  int                   ai_protocol;
  curl_socklen_t        ai_addrlen;   /* Follow rfc3493 struct addrinfo */
  char                 *ai_canonname;
  struct sockaddr      *ai_addr;
  struct Curl_addrinfo *ai_next;
};
typedef struct Curl_addrinfo Curl_addrinfo;

#ifdef OS_LINUX
//RHO: this defines and struct addrinfo

#define	EAI_ADDRFAMILY	 1	/* address family for hostname not supported */
#define	EAI_AGAIN	 2	/* temporary failure in name resolution */
#define	EAI_BADFLAGS	 3	/* invalid value for ai_flags */
#define	EAI_FAIL	 4	/* non-recoverable failure in name resolution */
#define	EAI_FAMILY	 5	/* ai_family not supported */
#define	EAI_MEMORY	 6	/* memory allocation failure */
#define	EAI_NODATA	 7	/* no address associated with hostname */
#define	EAI_NONAME	 8	/* hostname nor servname provided, or not known */
#define	EAI_SERVICE	 9	/* servname not supported for ai_socktype */
#define	EAI_SOCKTYPE	10	/* ai_socktype not supported */
#define	EAI_SYSTEM	11	/* system error returned in errno */
#define EAI_BADHINTS	12
#define EAI_PROTOCOL	13
#define EAI_MAX		14

struct addrinfo {
               int              ai_flags;
               int              ai_family;
               int              ai_socktype;
               int              ai_protocol;
               socklen_t        ai_addrlen;
               struct sockaddr *ai_addr;
               char            *ai_canonname;
               struct addrinfo *ai_next;
           };
#endif
void
Curl_freeaddrinfo(Curl_addrinfo *cahead);

#ifdef HAVE_GETADDRINFO
int
Curl_getaddrinfo_ex(const char *nodename,
                    const char *servname,
                    const struct addrinfo *hints,
                    Curl_addrinfo **result);
#endif

Curl_addrinfo *
Curl_he2ai(const struct hostent *he, int port);

Curl_addrinfo *
Curl_ip2addr(int af, const void *inaddr, const char *hostname, int port);

Curl_addrinfo *Curl_str2addr(char *dotted, int port);

#ifdef USE_UNIX_SOCKETS
Curl_addrinfo *Curl_unix2addr(const char *path, bool *longpath, bool abstract);
#endif

#if defined(CURLDEBUG) && defined(HAVE_GETADDRINFO) && \
    defined(HAVE_FREEADDRINFO)
void
curl_dofreeaddrinfo(struct addrinfo *freethis,
                    int line, const char *source);
#endif

#if defined(CURLDEBUG) && defined(HAVE_GETADDRINFO)
int
curl_dogetaddrinfo(const char *hostname,
                   const char *service,
                   const struct addrinfo *hints,
                   struct addrinfo **result,
                   int line, const char *source);
#endif

#ifdef HAVE_GETADDRINFO
#ifdef USE_RESOLVE_ON_IPS
void Curl_addrinfo_set_port(Curl_addrinfo *addrinfo, int port);
#else
#define Curl_addrinfo_set_port(x,y)
#endif
#endif

#endif /* HEADER_CURL_ADDRINFO_H */
