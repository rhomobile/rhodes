QT -= core

TARGET = curl
TEMPLATE = lib

CONFIG += staticlib warn_on

DESTDIR = ../bin/rhodes
OBJECTS_DIR = ../bin/rhodes
MOC_DIR = ../bin/rhodes
UI_DIR = ../bin/rhodes
RCC_DIR = ../bin/rhodes

DEFINES += __SYMBIAN32__

!win32 {
  QMAKE_CFLAGS_WARN_ON += -Wno-extra -Wno-unused -Wno-sign-compare -Wno-format -Wno-parentheses
  QMAKE_CXXFLAGS_WARN_ON += -Wno-extra -Wno-unused -Wno-sign-compare -Wno-format -Wno-parentheses
}

INCLUDEPATH += ../..
INCLUDEPATH += ../../shared/curl/include

HEADERS += ../../shared/curl/lib/arpa_telnet.h\
../../shared/curl/lib/connect.h\
../../shared/curl/lib/content_encoding.h\
../../shared/curl/lib/cookie.h\
../../shared/curl/lib/curl_addrinfo.h\
../../shared/curl/lib/curl_base64.h\
../../shared/curl/lib/curl_config.h\
../../shared/curl/lib/curl_ldap.h\
../../shared/curl/lib/curl_md5.h\
../../shared/curl/lib/curl_memory.h\
../../shared/curl/lib/curl_memrchr.h\
../../shared/curl/lib/curl_rand.h\
../../shared/curl/lib/dict.h\
../../shared/curl/lib/easyif.h\
../../shared/curl/lib/escape.h\
../../shared/curl/lib/file.h\
../../shared/curl/lib/formdata.h\
../../shared/curl/lib/ftp.h\
../../shared/curl/lib/getinfo.h\
../../shared/curl/lib/gtls.h\
../../shared/curl/lib/hash.h\
../../shared/curl/lib/hostip.h\
../../shared/curl/lib/http.h\
../../shared/curl/lib/http_chunks.h\
../../shared/curl/lib/http_digest.h\
../../shared/curl/lib/http_negotiate.h\
../../shared/curl/lib/http_ntlm.h\
../../shared/curl/lib/if2ip.h\
../../shared/curl/lib/inet_ntop.h\
../../shared/curl/lib/inet_pton.h\
../../shared/curl/lib/llist.h\
../../shared/curl/lib/memdebug.h\
../../shared/curl/lib/multiif.h\
../../shared/curl/lib/netrc.h\
../../shared/curl/lib/nonblock.h\
../../shared/curl/lib/nssg.h\
../../shared/curl/lib/parsedate.h\
../../shared/curl/lib/progress.h\
../../shared/curl/lib/qssl.h\
../../shared/curl/lib/rawstr.h\
../../shared/curl/lib/select.h\
../../shared/curl/lib/sendf.h\
../../shared/curl/lib/setup.h\
../../shared/curl/lib/setup_once.h\
../../shared/curl/lib/share.h\
../../shared/curl/lib/slist.h\
../../shared/curl/lib/sockaddr.h\
../../shared/curl/lib/socks.h\
../../shared/curl/lib/speedcheck.h\
../../shared/curl/lib/splay.h\
../../shared/curl/lib/ssh.h\
../../shared/curl/lib/sslgen.h\
../../shared/curl/lib/ssluse.h\
../../shared/curl/lib/strdup.h\
../../shared/curl/lib/strequal.h\
../../shared/curl/lib/strerror.h\
../../shared/curl/lib/strtok.h\
../../shared/curl/lib/strtoofft.h\
../../shared/curl/lib/telnet.h\
../../shared/curl/lib/tftp.h\
../../shared/curl/lib/timeval.h\
../../shared/curl/lib/transfer.h\
../../shared/curl/lib/url.h\
../../shared/curl/lib/urldata.h\
../../shared/curl/include/curl/curl.h\
../../shared/curl/include/curl/curlbuild.h\
../../shared/curl/include/curl/curlrules.h\
../../shared/curl/include/curl/curlver.h\
../../shared/curl/include/curl/easy.h\
../../shared/curl/include/curl/mprintf.h\
../../shared/curl/include/curl/multi.h\
../../shared/curl/include/curl/stdcheaders.h\
../../shared/curl/include/curl/typecheck-gcc.h\
../../shared/curl/include/curl/types.h\
../../shared/curl/lib/rhossl.h \
    ../../shared/curl/lib/config-symbian.h

SOURCES += ../../shared/curl/lib/amigaos.c\
../../shared/curl/lib/base64.c\
../../shared/curl/lib/connect.c\
../../shared/curl/lib/content_encoding.c\
../../shared/curl/lib/cookie.c\
../../shared/curl/lib/curl_addrinfo.c\
../../shared/curl/lib/curl_memrchr.c\
../../shared/curl/lib/curl_rand.c\
../../shared/curl/lib/curl_sspi.c\
../../shared/curl/lib/dict.c\
../../shared/curl/lib/easy.c\
../../shared/curl/lib/escape.c\
../../shared/curl/lib/file.c\
../../shared/curl/lib/formdata.c\
../../shared/curl/lib/ftp.c\
../../shared/curl/lib/getenv.c\
../../shared/curl/lib/getinfo.c\
../../shared/curl/lib/gtls.c\
../../shared/curl/lib/hash.c\
../../shared/curl/lib/hostares.c\
../../shared/curl/lib/hostasyn.c\
../../shared/curl/lib/hostip.c\
../../shared/curl/lib/hostip4.c\
../../shared/curl/lib/hostip6.c\
../../shared/curl/lib/hostsyn.c\
../../shared/curl/lib/hostthre.c\
../../shared/curl/lib/http.c\
../../shared/curl/lib/http_chunks.c\
../../shared/curl/lib/http_digest.c\
../../shared/curl/lib/http_negotiate.c\
../../shared/curl/lib/http_ntlm.c\
../../shared/curl/lib/if2ip.c\
../../shared/curl/lib/inet_ntop.c\
../../shared/curl/lib/inet_pton.c\
../../shared/curl/lib/krb4.c\
../../shared/curl/lib/krb5.c\
../../shared/curl/lib/ldap.c\
../../shared/curl/lib/llist.c\
../../shared/curl/lib/md5.c\
../../shared/curl/lib/memdebug.c\
../../shared/curl/lib/mprintf.c\
../../shared/curl/lib/multi.c\
../../shared/curl/lib/netrc.c\
../../shared/curl/lib/nonblock.c\
../../shared/curl/lib/nss.c\
../../shared/curl/lib/nwlib.c\
../../shared/curl/lib/nwos.c\
../../shared/curl/lib/parsedate.c\
../../shared/curl/lib/progress.c\
../../shared/curl/lib/qssl.c\
../../shared/curl/lib/rawstr.c\
../../shared/curl/lib/security.c\
../../shared/curl/lib/select.c\
../../shared/curl/lib/sendf.c\
../../shared/curl/lib/share.c\
../../shared/curl/lib/slist.c\
../../shared/curl/lib/socks.c\
../../shared/curl/lib/socks_gssapi.c\
../../shared/curl/lib/socks_sspi.c\
../../shared/curl/lib/speedcheck.c\
../../shared/curl/lib/splay.c\
../../shared/curl/lib/ssh.c\
../../shared/curl/lib/sslgen.c\
../../shared/curl/lib/ssluse.c\
../../shared/curl/lib/strdup.c\
../../shared/curl/lib/strequal.c\
../../shared/curl/lib/strerror.c\
../../shared/curl/lib/strtok.c\
../../shared/curl/lib/strtoofft.c\
../../shared/curl/lib/telnet.c\
../../shared/curl/lib/tftp.c\
../../shared/curl/lib/timeval.c\
../../shared/curl/lib/transfer.c\
../../shared/curl/lib/url.c\
../../shared/curl/lib/version.c\
../../shared/curl/lib/rhossl.c
