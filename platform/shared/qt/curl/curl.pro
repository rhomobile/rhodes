equals(QT_MAJOR_VERSION, 5) {
    lessThan(QT_MINOR_VERSION, 6): {
        DEFINES += RHODES_VERSION_1
    }
    equals(QT_MINOR_VERSION, 6): {
        DEFINES += OS_SAILFISH OS_LINUX
        CONFIG +=  c++14
    }
    greaterThan(QT_MINOR_VERSION, 7): {
        CONFIG += c++14
        DEFINES += CPP_ELEVEN RHODES_VERSION_2
    }
}

TARGET = curl
TEMPLATE = lib

CONFIG += staticlib warn_on

INCLUDEPATH += ../..\
../../curl/include\
../../curl/lib

macx {
  greaterThan(QT_MINOR_VERSION, 6): {
      DEFINES += RHODES_MAC_BUILD
  }
  DESTDIR = ../../../osx/bin/curl
  OBJECTS_DIR = ../../../osx/bin/curl/tmp
  DEFINES += USE_SSLEAY MACOS_SSL_SUPPORT USE_OPENSSL BUILDING_LIBCURL # USE_RHOSSL
  HEADERS += ../../curl/lib/config-mac.h
  INCLUDEPATH += ../../../../../openssl/include\
../../curl/lib\
../../curl/lib/vauth\
../../curl/lib/vtls

SOURCES += ../../curl/lib/http_ntlm.c\
../../curl/lib/vauth/ntlm.c\
../../curl/lib/smb.c\
../../curl/lib/vtls/openssl.c\
../../curl/lib/curl_ntlm_core.c\
../../curl/lib/curl_endian.c


#../../curl/lib/qssl.c\
#../../curl/lib/ssluse.c
}

win32 {
  greaterThan(QT_MINOR_VERSION, 6): {
      DEFINES += CPP_ELEVEN
  }
  DESTDIR = ../../../win32/bin/curl
  OBJECTS_DIR = ../../../win32/bin/curl/tmp
  DEFINES += WIN32 _WINDOWS _LIB _UNICODE UNICODE WIN32_LEAN_AND_MEAN
  INCLUDEPATH += ../../../../lib/extensions/zlib/ext\
../../ruby/include\
../../common\
../../ruby/win32
  Debug {
    DEFINES += _DEBUG DEBUG
  }
  Release {
    DEFINES += _NDEBUG NDEBUG
  }
}

unix:!macx {
    INCLUDEPATH += $$PWD/../../ruby/linux
    DEFINES += _GNU_SOURCE
    DESTDIR = $$PWD/../../../linux/bin/curl
    OBJECTS_DIR = $$PWD/../../../linux/bin/curl/tmp
    DEFINES += HAVE_CONFIG_H USE_RHOSSL OS_LINUX
    QMAKE_CFLAGS += -fvisibility=hidden
    QMAKE_CXXFLAGS += -fvisibility=hidden
    
    #QMAKE_CFLAGS_RELEASE -= -O2
    #QMAKE_CXXFLAGS_RELEASE -= -O2    

    Debug {
      DEFINES += _DEBUG DEBUG
    }
    Release {
      DEFINES += _NDEBUG NDEBUG
    }

}

DEFINES += RHODES_QT_PLATFORM _XOPEN_SOURCE _DARWIN_C_SOURCE

!isEmpty(RHOSIMULATOR_BUILD) {
  DEFINES += RHODES_EMULATOR
}

!win32 {
  QMAKE_CFLAGS_WARN_ON += -Wno-extra -Wno-unused -Wno-sign-compare -Wno-parentheses
  QMAKE_CFLAGS_RELEASE -= -Werror=format-security
  QMAKE_CFLAGS_DEBUG -= -O2
  QMAKE_CXXFLAGS_DEBUG -= -O2
}
win32 {
  QMAKE_CFLAGS_WARN_ON += /wd4101 /wd4005
    QMAKE_CXXFLAGS_RELEASE += /MP9 /O2
    QMAKE_CXXFLAGS_DEBUG += /MP9 /O2

    QMAKE_CFLAGS_RELEASE += /O2 /MD
    QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO += /O2
    QMAKE_CFLAGS_DEBUG += /Zi /MDd
}

HEADERS += ../../curl/lib/arpa_telnet.h\
../../curl/lib/curl_setup.h \
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
../../curl/lib/dict.h\
../../curl/lib/easyif.h\
../../curl/lib/escape.h\
../../curl/lib/file.h\
../../curl/lib/formdata.h\
../../curl/lib/ftp.h\
../../curl/lib/getinfo.h\
../../curl/lib/vtls/gtls.h\
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
../../curl/lib/vtls/nssg.h\
../../curl/lib/parsedate.h\
../../curl/lib/progress.h\
../../curl/lib/select.h\
../../curl/lib/sendf.h\
../../curl/lib/share.h\
../../curl/lib/slist.h\
../../curl/lib/sockaddr.h\
../../curl/lib/socks.h\
../../curl/lib/speedcheck.h\
../../curl/lib/splay.h\
../../curl/lib/ssh.h\
../../curl/lib/strdup.h\
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
../../curl/include/curl/curlver.h\
../../curl/include/curl/easy.h\
../../curl/include/curl/mprintf.h\
../../curl/include/curl/multi.h\
../../curl/include/curl/stdcheaders.h\
../../curl/include/curl/typecheck-gcc.h\
../../curl/lib/rhossl.h \
../../curl/lib/vtls/gtls.h \
../../curl/lib/vtls/vtls.h \
../../curl/include/curl/system.h \
../../curl/lib/strcase.h \
../../curl/lib/asyn.h \
../../curl/lib/warnless.h \
../../curl/lib/vauth/digest.h \
../../curl/lib/vauth/vauth.h \
../../curl/lib/pingpong.h \
../../curl/lib/pipeline.h \
../../curl/lib/http_proxy.h \
../../curl/lib/conncache.h \
../../curl/lib/dotdot.h \
../../curl/lib/fileinfo.h \
../../curl/lib/ftplistparser.h \
../../curl/lib/gopher.h \
../../curl/lib/hostcheck.h \
../../curl/lib/imap.h \
../../curl/lib/pop3.h \
../../curl/lib/smb.h \
../../curl/lib/wildcard.h \
../../curl/lib/curl_sasl.h \
../../curl/lib/rtsp.h \
../../curl/lib/curl_threads.h \
../../curl/lib/curl_gethostname.h \
../../curl/lib/curl_fnmatch.h

SOURCES += ../../curl/lib/amigaos.c\
../../curl/lib/base64.c\
../../curl/lib/connect.c\
../../curl/lib/content_encoding.c\
../../curl/lib/cookie.c\
../../curl/lib/curl_addrinfo.c\
../../curl/lib/curl_memrchr.c\
../../curl/lib/rand.c\
../../curl/lib/curl_sspi.c\
../../curl/lib/dict.c\
../../curl/lib/easy.c\
../../curl/lib/escape.c\
../../curl/lib/file.c\
../../curl/lib/formdata.c\
../../curl/lib/ftp.c\
../../curl/lib/getenv.c\
../../curl/lib/getinfo.c\
../../curl/lib/vtls/gtls.c\
../../curl/lib/hash.c\
../../curl/lib/hostasyn.c\
../../curl/lib/hostip.c\
../../curl/lib/hostip4.c\
../../curl/lib/hostip6.c\
../../curl/lib/hostsyn.c\
../../curl/lib/http.c\
../../curl/lib/http_chunks.c\
../../curl/lib/http_digest.c\
../../curl/lib/http_negotiate.c\
../../curl/lib/if2ip.c\
../../curl/lib/inet_ntop.c\
../../curl/lib/inet_pton.c\
../../curl/lib/krb5.c\
../../curl/lib/ldap.c\
../../curl/lib/llist.c\
../../curl/lib/md5.c\
../../curl/lib/memdebug.c\
../../curl/lib/mprintf.c\
../../curl/lib/multi.c\
../../curl/lib/netrc.c\
../../curl/lib/nonblock.c\
../../curl/lib/vtls/nss.c\
../../curl/lib/nwlib.c\
../../curl/lib/nwos.c\
../../curl/lib/parsedate.c\
../../curl/lib/progress.c\
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
../../curl/lib/strdup.c\
../../curl/lib/strerror.c\
../../curl/lib/strtok.c\
../../curl/lib/strtoofft.c\
../../curl/lib/telnet.c\
../../curl/lib/tftp.c\
../../curl/lib/timeval.c\
../../curl/lib/transfer.c\
../../curl/lib/url.c\
../../curl/lib/version.c\
../../curl/lib/rhossl.c\
../../curl/lib/vtls/vtls.c \
../../curl/lib/strcase.c \
../../curl/lib/asyn-thread.c \
../../curl/lib/warnless.c \
../../curl/lib/vauth/cleartext.c \
../../curl/lib/vauth/cram.c \
../../curl/lib/vauth/digest.c \
../../curl/lib/vauth/oauth2.c \
../../curl/lib/vauth/vauth.c \
../../curl/lib/pingpong.c \
../../curl/lib/pipeline.c \
../../curl/lib/http_proxy.c \
../../curl/lib/conncache.c \
../../curl/lib/dotdot.c \
../../curl/lib/fileinfo.c \
../../curl/lib/ftplistparser.c \
../../curl/lib/hmac.c \
../../curl/lib/hostcheck.c \
../../curl/lib/imap.c \
../../curl/lib/pop3.c \
../../curl/lib/smtp.c \
../../curl/lib/wildcard.c \
../../curl/lib/curl_sasl.c \
../../curl/lib/rtsp.c \
../../curl/lib/gopher.c \
../../curl/lib/curl_threads.c \
../../curl/lib/curl_gethostname.c \
../../curl/lib/curl_fnmatch.c
