LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := curl
LOCAL_SRC_FILES := \
    lib/amigaos.c \
    lib/base64.c \
    lib/connect.c \
    lib/content_encoding.c \
    lib/cookie.c \
    lib/curl_addrinfo.c \
    lib/curl_memrchr.c \
    lib/curl_rand.c \
    lib/curl_sspi.c \
    lib/dict.c \
    lib/easy.c \
    lib/escape.c \
    lib/file.c \
    lib/formdata.c \
    lib/ftp.c \
    lib/getenv.c \
    lib/getinfo.c \
    lib/gtls.c \
    lib/hash.c \
    lib/hostares.c \
    lib/hostasyn.c \
    lib/hostip.c \
    lib/hostip4.c \
    lib/hostip6.c \
    lib/hostsyn.c \
    lib/hostthre.c \
    lib/http.c \
    lib/http_chunks.c \
    lib/http_digest.c \
    lib/http_negotiate.c \
    lib/if2ip.c \
    lib/inet_ntop.c \
    lib/inet_pton.c \
    lib/krb4.c \
    lib/krb5.c \
    lib/ldap.c \
    lib/llist.c \
    lib/md5.c \
    lib/memdebug.c \
    lib/mprintf.c \
    lib/multi.c \
    lib/netrc.c \
    lib/nonblock.c \
    lib/nss.c \
    lib/nwlib.c \
    lib/nwos.c \
    lib/parsedate.c \
    lib/progress.c \
    lib/rawstr.c \
    lib/rhossl.c \
    lib/security.c \
    lib/select.c \
    lib/sendf.c \
    lib/share.c \
    lib/slist.c \
    lib/socks.c \
    lib/socks_gssapi.c \
    lib/socks_sspi.c \
    lib/speedcheck.c \
    lib/splay.c \
    lib/ssh.c \
    lib/sslgen.c \
    lib/strdup.c \
    lib/strequal.c \
    lib/strerror.c \
    lib/strtok.c \
    lib/strtoofft.c \
    lib/telnet.c \
    lib/tftp.c \
    lib/timeval.c \
    lib/transfer.c \
    lib/url.c \
    lib/version.c

LOCAL_C_INCLUDES := \
    $(LOCAL_PATH)/include \
    $(LOACL_PATH)/lib \
    $(SHARED_PATH_INC)

LOCAL_CFLAGS := -DHAVE_CONFIG_H

include $(BUILD_STATIC_LIBRARY)
