QT -= core

TARGET = curl
TEMPLATE = lib

CONFIG += staticlib warn_on

macx {
  DESTDIR = ../../../osx/build/curl
  OBJECTS_DIR = ../../../osx/build/curl/tmp
}

DEFINES += RHODES_EMULATOR

QMAKE_CFLAGS_WARN_ON += -Wno-extra -Wno-unused -Wno-sign-compare -Wno-format -Wno-parentheses
QMAKE_CXXFLAGS_WARN_ON += -Wno-extra -Wno-unused -Wno-sign-compare -Wno-format -Wno-parentheses

INCLUDEPATH += ../..

HEADERS += ../../curl/lib/arpa_telnet.h\
../../curl/lib/connect.h\
../../curl/lib/content_encoding.h\
../../curl/lib/cookie.h\
../../curl/lib/curl_addrinfo.h\
../../curl/lib/curl_base64.h\
../../curl/lib/curl_config.h\
../../curl/lib/curl_ldap.h\
../../curl/lib/curl_md5.h\
../../curl/lib/curl_memory.h\
../../curl/lib/curl_memrchr.h\
../../curl/lib/curl_rand.h\
../../curl/lib/dict.h\
../../curl/lib/easyif.h\
../../curl/lib/escape.h\
../../curl/lib/file.h\
../../curl/lib/formdata.h\
../../curl/lib/ftp.h\
../../curl/lib/getinfo.h\
../../curl/lib/gtls.h\
../../curl/lib/hash.h\
../../curl/lib/hostip.h\
../../curl/lib/http.h\
../../curl/lib/http_chunks.h\
../../curl/lib/http_digest.h\
../../curl/lib/http_negotiate.h\
../../curl/lib/http_ntlm.h\
../../curl/lib/if2ip.h\
../../curl/lib/inet_ntop.h\
../../curl/lib/inet_pton.h\
../../curl/lib/llist.h\
../../curl/lib/memdebug.h\
../../curl/lib/multiif.h\
../../curl/lib/netrc.h\
../../curl/lib/nonblock.h\
../../curl/lib/nssg.h\
../../curl/lib/parsedate.h\
../../curl/lib/progress.h\
../../curl/lib/qssl.h\
../../curl/lib/rawstr.h\
../../curl/lib/select.h\
../../curl/lib/sendf.h\
../../curl/lib/setup.h\
../../curl/lib/setup_once.h\
../../curl/lib/share.h\
../../curl/lib/slist.h\
../../curl/lib/sockaddr.h\
../../curl/lib/socks.h\
../../curl/lib/speedcheck.h\
../../curl/lib/splay.h\
../../curl/lib/ssh.h\
../../curl/lib/sslgen.h\
../../curl/lib/ssluse.h\
../../curl/lib/strdup.h\
../../curl/lib/strequal.h\
../../curl/lib/strerror.h\
../../curl/lib/strtok.h\
../../curl/lib/strtoofft.h\
../../curl/lib/telnet.h\
../../curl/lib/tftp.h\
../../curl/lib/timeval.h\
../../curl/lib/transfer.h\
../../curl/lib/url.h\
../../curl/lib/urldata.h\
../../curl/include/curl/curl.h\
../../curl/include/curl/curlbuild.h\
../../curl/include/curl/curlrules.h\
../../curl/include/curl/curlver.h\
../../curl/include/curl/easy.h\
../../curl/include/curl/mprintf.h\
../../curl/include/curl/multi.h\
../../curl/include/curl/stdcheaders.h\
../../curl/include/curl/typecheck-gcc.h\
../../curl/include/curl/types.h\
../../curl/lib/rhossl.h

SOURCES += ../../curl/lib/amigaos.c\
../../curl/lib/base64.c\
../../curl/lib/connect.c\
../../curl/lib/content_encoding.c\
../../curl/lib/cookie.c\
../../curl/lib/curl_addrinfo.c\
../../curl/lib/curl_memrchr.c\
../../curl/lib/curl_rand.c\
../../curl/lib/curl_sspi.c\
../../curl/lib/dict.c\
../../curl/lib/easy.c\
../../curl/lib/escape.c\
../../curl/lib/file.c\
../../curl/lib/formdata.c\
../../curl/lib/ftp.c\
../../curl/lib/getenv.c\
../../curl/lib/getinfo.c\
../../curl/lib/gtls.c\
../../curl/lib/hash.c\
../../curl/lib/hostares.c\
../../curl/lib/hostasyn.c\
../../curl/lib/hostip.c\
../../curl/lib/hostip4.c\
../../curl/lib/hostip6.c\
../../curl/lib/hostsyn.c\
../../curl/lib/hostthre.c\
../../curl/lib/http.c\
../../curl/lib/http_chunks.c\
../../curl/lib/http_digest.c\
../../curl/lib/http_negotiate.c\
../../curl/lib/http_ntlm.c\
../../curl/lib/if2ip.c\
../../curl/lib/inet_ntop.c\
../../curl/lib/inet_pton.c\
../../curl/lib/krb4.c\
../../curl/lib/krb5.c\
../../curl/lib/ldap.c\
../../curl/lib/llist.c\
../../curl/lib/md5.c\
../../curl/lib/memdebug.c\
../../curl/lib/mprintf.c\
../../curl/lib/multi.c\
../../curl/lib/netrc.c\
../../curl/lib/nonblock.c\
../../curl/lib/nss.c\
../../curl/lib/nwlib.c\
../../curl/lib/nwos.c\
../../curl/lib/parsedate.c\
../../curl/lib/progress.c\
../../curl/lib/qssl.c\
../../curl/lib/rawstr.c\
../../curl/lib/security.c\
../../curl/lib/select.c\
../../curl/lib/sendf.c\
../../curl/lib/share.c\
../../curl/lib/slist.c\
../../curl/lib/socks.c\
../../curl/lib/socks_gssapi.c\
../../curl/lib/socks_sspi.c\
../../curl/lib/speedcheck.c\
../../curl/lib/splay.c\
../../curl/lib/ssh.c\
../../curl/lib/sslgen.c\
../../curl/lib/ssluse.c\
../../curl/lib/strdup.c\
../../curl/lib/strequal.c\
../../curl/lib/strerror.c\
../../curl/lib/strtok.c\
../../curl/lib/strtoofft.c\
../../curl/lib/telnet.c\
../../curl/lib/tftp.c\
../../curl/lib/timeval.c\
../../curl/lib/transfer.c\
../../curl/lib/url.c\
../../curl/lib/version.c\
../../curl/lib/rhossl.c
