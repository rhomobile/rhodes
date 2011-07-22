LOCAL_PATH := $(call my-dir)
SHARED_PATH_INC := $(LOCAL_PATH)/../../../shared
SHARED_PATH := ../../../shared

include $(CLEAR_VARS)

LOCAL_MODULE    := sqlite
LOCAL_SRC_FILES := $(SHARED_PATH)/sqlite/sqlite3.c
LOCAL_C_INCLUDES := $(SHARED_PATH_INC)/sqlite $(SHARED_PATH_INC)

include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE    := curl
LOCAL_SRC_FILES := \
    $(SHARED_PATH)/curl/lib/amigaos.c \
    $(SHARED_PATH)/curl/lib/base64.c \
    $(SHARED_PATH)/curl/lib/connect.c \
    $(SHARED_PATH)/curl/lib/content_encoding.c \
    $(SHARED_PATH)/curl/lib/cookie.c \
    $(SHARED_PATH)/curl/lib/curl_addrinfo.c \
    $(SHARED_PATH)/curl/lib/curl_memrchr.c \
    $(SHARED_PATH)/curl/lib/curl_rand.c \
    $(SHARED_PATH)/curl/lib/curl_sspi.c \
    $(SHARED_PATH)/curl/lib/dict.c \
    $(SHARED_PATH)/curl/lib/easy.c \
    $(SHARED_PATH)/curl/lib/escape.c \
    $(SHARED_PATH)/curl/lib/file.c \
    $(SHARED_PATH)/curl/lib/formdata.c \
    $(SHARED_PATH)/curl/lib/ftp.c \
    $(SHARED_PATH)/curl/lib/getenv.c \
    $(SHARED_PATH)/curl/lib/getinfo.c \
    $(SHARED_PATH)/curl/lib/gtls.c \
    $(SHARED_PATH)/curl/lib/hash.c \
    $(SHARED_PATH)/curl/lib/hostares.c \
    $(SHARED_PATH)/curl/lib/hostasyn.c \
    $(SHARED_PATH)/curl/lib/hostip.c \
    $(SHARED_PATH)/curl/lib/hostip4.c \
    $(SHARED_PATH)/curl/lib/hostip6.c \
    $(SHARED_PATH)/curl/lib/hostsyn.c \
    $(SHARED_PATH)/curl/lib/hostthre.c \
    $(SHARED_PATH)/curl/lib/http.c \
    $(SHARED_PATH)/curl/lib/http_chunks.c \
    $(SHARED_PATH)/curl/lib/http_digest.c \
    $(SHARED_PATH)/curl/lib/http_negotiate.c \
    $(SHARED_PATH)/curl/lib/if2ip.c \
    $(SHARED_PATH)/curl/lib/inet_ntop.c \
    $(SHARED_PATH)/curl/lib/inet_pton.c \
    $(SHARED_PATH)/curl/lib/krb4.c \
    $(SHARED_PATH)/curl/lib/krb5.c \
    $(SHARED_PATH)/curl/lib/ldap.c \
    $(SHARED_PATH)/curl/lib/llist.c \
    $(SHARED_PATH)/curl/lib/md5.c \
    $(SHARED_PATH)/curl/lib/memdebug.c \
    $(SHARED_PATH)/curl/lib/mprintf.c \
    $(SHARED_PATH)/curl/lib/multi.c \
    $(SHARED_PATH)/curl/lib/netrc.c \
    $(SHARED_PATH)/curl/lib/nonblock.c \
    $(SHARED_PATH)/curl/lib/nss.c \
    $(SHARED_PATH)/curl/lib/nwlib.c \
    $(SHARED_PATH)/curl/lib/nwos.c \
    $(SHARED_PATH)/curl/lib/parsedate.c \
    $(SHARED_PATH)/curl/lib/progress.c \
    $(SHARED_PATH)/curl/lib/rawstr.c \
    $(SHARED_PATH)/curl/lib/rhossl.c \
    $(SHARED_PATH)/curl/lib/security.c \
    $(SHARED_PATH)/curl/lib/select.c \
    $(SHARED_PATH)/curl/lib/sendf.c \
    $(SHARED_PATH)/curl/lib/share.c \
    $(SHARED_PATH)/curl/lib/slist.c \
    $(SHARED_PATH)/curl/lib/socks.c \
    $(SHARED_PATH)/curl/lib/socks_gssapi.c \
    $(SHARED_PATH)/curl/lib/socks_sspi.c \
    $(SHARED_PATH)/curl/lib/speedcheck.c \
    $(SHARED_PATH)/curl/lib/splay.c \
    $(SHARED_PATH)/curl/lib/ssh.c \
    $(SHARED_PATH)/curl/lib/sslgen.c \
    $(SHARED_PATH)/curl/lib/strdup.c \
    $(SHARED_PATH)/curl/lib/strequal.c \
    $(SHARED_PATH)/curl/lib/strerror.c \
    $(SHARED_PATH)/curl/lib/strtok.c \
    $(SHARED_PATH)/curl/lib/strtoofft.c \
    $(SHARED_PATH)/curl/lib/telnet.c \
    $(SHARED_PATH)/curl/lib/tftp.c \
    $(SHARED_PATH)/curl/lib/timeval.c \
    $(SHARED_PATH)/curl/lib/transfer.c \
    $(SHARED_PATH)/curl/lib/url.c \
    $(SHARED_PATH)/curl/lib/version.c

LOCAL_C_INCLUDES := \
    $(SHARED_PATH_INC)/curl/include \
    $(SHARED_PATH_INC)/curl/lib \
    $(SHARED_PATH_INC)

LOCAL_CFLAGS := -DHAVE_CONFIG_H

include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE    := ruby
LOCAL_SRC_FILES := \
    $(SHARED_PATH)/ruby/array.c \
    $(SHARED_PATH)/ruby/bignum.c \
    $(SHARED_PATH)/ruby/blockinlining.c \
    $(SHARED_PATH)/ruby/class.c \
    $(SHARED_PATH)/ruby/compar.c \
    $(SHARED_PATH)/ruby/compile.c \
    $(SHARED_PATH)/ruby/complex.c \
    $(SHARED_PATH)/ruby/cont.c \
    $(SHARED_PATH)/ruby/debug.c \
    $(SHARED_PATH)/ruby/dir.c \
    $(SHARED_PATH)/ruby/dln.c \
    $(SHARED_PATH)/ruby/dmyencoding.c \
    $(SHARED_PATH)/ruby/dmyext.c \
    $(SHARED_PATH)/ruby/enc/ascii.c \
    $(SHARED_PATH)/ruby/enc/unicode.c \
    $(SHARED_PATH)/ruby/enc/us_ascii.c \
    $(SHARED_PATH)/ruby/enc/utf_8.c \
    $(SHARED_PATH)/ruby/enum.c \
    $(SHARED_PATH)/ruby/enumerator.c \
    $(SHARED_PATH)/ruby/error.c \
    $(SHARED_PATH)/ruby/eval.c \
    $(SHARED_PATH)/ruby/ext/alert/alert_wrap.c \
    $(SHARED_PATH)/ruby/ext/asynchttp/asynchttp_wrap.c \
    $(SHARED_PATH)/ruby/ext/bluetooth/bluetooth_wrap.c \
    $(SHARED_PATH)/ruby/ext/nativeviewmanager/nativeviewmanager_wrap.c \
    $(SHARED_PATH)/ruby/ext/calendar/calendar_wrap.c \
    $(SHARED_PATH)/ruby/ext/calendar/event_wrap.c \
    $(SHARED_PATH)/ruby/ext/camera/camera_wrap.c \
    $(SHARED_PATH)/ruby/ext/datetimepicker/datetimepicker_wrap.c \
    $(SHARED_PATH)/ruby/ext/geolocation/geolocation_wrap.c \
    $(SHARED_PATH)/ruby/ext/mapview/mapview_wrap.c \
    $(SHARED_PATH)/ruby/ext/nativebar/nativebar_wrap.c \
    $(SHARED_PATH)/ruby/ext/navbar/navbar_wrap.c \
    $(SHARED_PATH)/ruby/ext/phonebook/phonebook_wrap.c \
    $(SHARED_PATH)/ruby/ext/rho/extensions.c \
    $(SHARED_PATH)/ruby/ext/rho/rhoruby.c \
    $(SHARED_PATH)/ruby/ext/rho/rhosupport.c \
    $(SHARED_PATH)/ruby/ext/rhoconf/rhoconf_wrap.c \
    $(SHARED_PATH)/ruby/ext/ringtones/ringtones_wrap.c \
    $(SHARED_PATH)/ruby/ext/signature/signature_wrap.c \
    $(SHARED_PATH)/ruby/ext/socket/socket.c \
    $(SHARED_PATH)/ruby/ext/sqlite3_api/sqlite3_api_wrap.c \
    $(SHARED_PATH)/ruby/ext/stringio/stringio.c \
    $(SHARED_PATH)/ruby/ext/strscan/strscan.c \
    $(SHARED_PATH)/ruby/ext/syncengine/syncengine_wrap.c \
    $(SHARED_PATH)/ruby/ext/system/system_wrap.c \
    $(SHARED_PATH)/ruby/ext/webview/webview_wrap.c \
    $(SHARED_PATH)/ruby/file.c \
    $(SHARED_PATH)/ruby/gc.c \
    $(SHARED_PATH)/ruby/generated/parse.c \
    $(SHARED_PATH)/ruby/hash.c \
    $(SHARED_PATH)/ruby/inits.c \
    $(SHARED_PATH)/ruby/io.c \
    $(SHARED_PATH)/ruby/iseq.c \
    $(SHARED_PATH)/ruby/load.c \
    $(SHARED_PATH)/ruby/marshal.c \
    $(SHARED_PATH)/ruby/math.c \
    $(SHARED_PATH)/ruby/miniprelude.c \
    $(SHARED_PATH)/ruby/missing/acosh.c \
    $(SHARED_PATH)/ruby/missing/cbrt.c \
    $(SHARED_PATH)/ruby/missing/crypt.c \
    $(SHARED_PATH)/ruby/missing/dup2.c \
    $(SHARED_PATH)/ruby/missing/erf.c \
    $(SHARED_PATH)/ruby/missing/hypot.c \
    $(SHARED_PATH)/ruby/missing/lgamma_r.c \
    $(SHARED_PATH)/ruby/missing/stdlib.c \
    $(SHARED_PATH)/ruby/missing/strlcat.c \
    $(SHARED_PATH)/ruby/missing/strlcpy.c \
    $(SHARED_PATH)/ruby/missing/tgamma.c \
    $(SHARED_PATH)/ruby/newline.c \
    $(SHARED_PATH)/ruby/numeric.c \
    $(SHARED_PATH)/ruby/object.c \
    $(SHARED_PATH)/ruby/pack.c \
    $(SHARED_PATH)/ruby/proc.c \
    $(SHARED_PATH)/ruby/process.c \
    $(SHARED_PATH)/ruby/random.c \
    $(SHARED_PATH)/ruby/range.c \
    $(SHARED_PATH)/ruby/rational.c \
    $(SHARED_PATH)/ruby/re.c \
    $(SHARED_PATH)/ruby/regcomp.c \
    $(SHARED_PATH)/ruby/regenc.c \
    $(SHARED_PATH)/ruby/regerror.c \
    $(SHARED_PATH)/ruby/regexec.c \
    $(SHARED_PATH)/ruby/regparse.c \
    $(SHARED_PATH)/ruby/regsyntax.c \
    $(SHARED_PATH)/ruby/ruby.c \
    $(SHARED_PATH)/ruby/safe.c \
    $(SHARED_PATH)/ruby/signal.c \
    $(SHARED_PATH)/ruby/sprintf.c \
    $(SHARED_PATH)/ruby/st.c \
    $(SHARED_PATH)/ruby/strftime.c \
    $(SHARED_PATH)/ruby/string.c \
    $(SHARED_PATH)/ruby/struct.c \
    $(SHARED_PATH)/ruby/thread.c \
    $(SHARED_PATH)/ruby/time.c \
    $(SHARED_PATH)/ruby/transcode.c \
    $(SHARED_PATH)/ruby/util.c \
    $(SHARED_PATH)/ruby/variable.c \
    $(SHARED_PATH)/ruby/version.c \
    $(SHARED_PATH)/ruby/vm.c \
    $(SHARED_PATH)/ruby/vm_dump.c
LOCAL_C_INCLUDES := \
    $(SHARED_PATH_INC)/ruby/include \
    $(SHARED_PATH_INC)/ruby/linux \
    $(SHARED_PATH_INC)/ruby/generated \
    $(SHARED_PATH_INC)/ruby/ \
    $(SHARED_PATH_INC) \
    $(SHARED_PATH_INC)/sqlite
    

include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE    := json
LOCAL_SRC_FILES := \
    $(SHARED_PATH)/json/JSONIterator.cpp \
    $(SHARED_PATH)/json/RJSONTokener.c \
    $(SHARED_PATH)/json/arraylist.c \
    $(SHARED_PATH)/json/json_debug.c \
    $(SHARED_PATH)/json/json_object.c \
    $(SHARED_PATH)/json/json_tokener.c \
    $(SHARED_PATH)/json/json_util.c \
    $(SHARED_PATH)/json/linkhash.c \
    $(SHARED_PATH)/json/printbuf.c
LOCAL_C_INCLUDES := $(SHARED_PATH_INC)/json $(SHARED_PATH_INC)

include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE    := rholog
LOCAL_SRC_FILES := \
    $(SHARED_PATH)/logging/RhoLogConf.cpp \
    $(SHARED_PATH)/logging/RhoLog.cpp \
    $(SHARED_PATH)/logging/RhoLogSink.cpp \
    $(SHARED_PATH)/logging/RhoPlainLog.cpp
LOCAL_C_INCLUDES := $(SHARED_PATH_INC)

include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE    := rhomain
LOCAL_SRC_FILES := \
    $(SHARED_PATH)/common/RhodesApp.cpp \
    $(SHARED_PATH)/common/RhodesAppBase.cpp
LOCAL_C_INCLUDES := $(SHARED_PATH_INC)

include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE    := rhocommon
LOCAL_SRC_FILES := \
    $(SHARED_PATH)/common/app_build_configs.c \
    $(SHARED_PATH)/common/AppMenu.cpp \
    $(SHARED_PATH)/common/map/ESRIMapEngine.cpp \
    $(SHARED_PATH)/common/map/GoogleMapEngine.cpp \
    $(SHARED_PATH)/common/map/MapEngine.cpp \
    $(SHARED_PATH)/common/PosixThreadImpl.cpp \
    $(SHARED_PATH)/common/RhoConf.cpp \
    $(SHARED_PATH)/common/RhoFile.cpp \
    $(SHARED_PATH)/common/RhoMath.cpp \
    $(SHARED_PATH)/common/rhoparams.cpp \
    $(SHARED_PATH)/common/RhoThread.cpp \
    $(SHARED_PATH)/common/SplashScreen.cpp \
    $(SHARED_PATH)/common/ThreadQueue.cpp \
    $(SHARED_PATH)/common/Tokenizer.cpp \
    $(SHARED_PATH)/common/RhoTime.cpp \
    $(SHARED_PATH)/net/AsyncHttp.cpp \
    $(SHARED_PATH)/net/CURLNetRequest.cpp \
    $(SHARED_PATH)/net/HttpServer.cpp \
    $(SHARED_PATH)/net/ssl.cpp \
    $(SHARED_PATH)/net/URI.cpp \
    $(SHARED_PATH)/net/INetRequest.cpp \
    $(SHARED_PATH)/rubyext/GeoLocation.cpp \
    $(SHARED_PATH)/rubyext/RhoAppAdapter.cpp \
    $(SHARED_PATH)/rubyext/System.cpp \
    $(SHARED_PATH)/statistic/RhoProfiler.cpp \
    $(SHARED_PATH)/unzip/unzip.cpp
LOCAL_C_INCLUDES := \
    $(SHARED_PATH_INC) \
    $(SHARED_PATH_INC)/curl/include \
    $(SHARED_PATH_INC)/ruby/include \
    $(SHARED_PATH_INC)/ruby/linux

include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE    := rhodb
LOCAL_SRC_FILES := \
    $(SHARED_PATH)/db/DBAdapter.cpp \
    $(SHARED_PATH)/db/DBAttrManager.cpp \
    $(SHARED_PATH)/db/DBResult.cpp
LOCAL_C_INCLUDES := $(SHARED_PATH_INC)/db $(SHARED_PATH_INC) $(SHARED_PATH_INC)/sqlite

include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE    := rhosync
LOCAL_SRC_FILES := \
    $(SHARED_PATH)/sync/ClientRegister.cpp \
    $(SHARED_PATH)/sync/SyncEngine.cpp \
    $(SHARED_PATH)/sync/SyncNotify.cpp \
    $(SHARED_PATH)/sync/SyncSource.cpp \
    $(SHARED_PATH)/sync/SyncThread.cpp
LOCAL_C_INCLUDES := $(SHARED_PATH_INC)/sync $(SHARED_PATH_INC) $(SHARED_PATH_INC)/sqlite

include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE    := rhodes
LOCAL_SRC_FILES := \
    src/RhoClassFactory.cpp \
    src/JNIRhoRuby.cpp \
    src/alert.cpp \
    src/bluetooth.cpp \
    src/callbacks.cpp \
    src/camera.cpp \
    src/datetimepicker.cpp \
    src/event.cpp \
    src/fileapi.cpp \
    src/geolocation.cpp \
    src/logconf.cpp \
    src/logger.cpp \
    src/mapview.cpp \
    src/menu.cpp \
    src/nativebar.cpp \
    src/nativeview.cpp \
    src/navbar.cpp \
    src/phonebook.cpp \
    src/rhoconf.cpp \
    src/rhodes.cpp \
    src/rhodesapp.cpp \
    src/rhodessystem.cpp \
    src/ringtones.cpp \
    src/signature.cpp \
    src/socketimpl.cpp \
    src/splashscreen.cpp \
    src/sslimpl.cpp \
    src/webview.cpp \
    src/rhocryptimpl.cpp

LOCAL_C_INCLUDES := \
    $(LOCAL_PATH)/include \
    $(LOCAL_PATH)/include/rhodes/details \
    $(SHARED_PATH_INC) \
    $(SHARED_PATH_INC)/common \
    $(SHARED_PATH_INC)/ruby/include \
    $(SHARED_PATH_INC)/ruby/linux \
    $(SHARED_PATH_INC)/curl/include \
    $(SHARED_PATH_INC)/sqlite

LOCAL_STATIC_LIBRARIES := ruby json rhomain rhocommon rhodb rhosync curl sqlite rholog rhocommon
LOCAL_LDLIBS := -llog -ldl -lz

include $(BUILD_SHARED_LIBRARY)

