#-------------------------------------------------
#
# Project created by QtCreator 2011-06-15T18:21:46
#
#-------------------------------------------------

QT       -= gui
DEFINES += HAVE_SOCKADDR_STORAGE=1\
           RHO_SYMBIAN _LIB STATIC_LINKED

TARGET = rubylib
TEMPLATE = lib
CONFIG += staticlib warn_on
INCLUDEPATH += ../../shared
INCLUDEPATH += ../../shared/ruby
INCLUDEPATH += ../../shared/ruby/generated
INCLUDEPATH += ../../shared/ruby/include
INCLUDEPATH += /epoc32/include/stdapis/glib-2.0

DESTDIR = ../bin/rubylib
OBJECTS_DIR = ../bin/rubylib
MOC_DIR = ../bin/rubylib
UI_DIR = ../bin/rubylib
RCC_DIR = ../bin/rubylib

SOURCES += \
    ../../shared/ruby/enc/utf_8.c \
    ../../shared/ruby/enc/us_ascii.c \
    ../../shared/ruby/enc/unicode.c \
    ../../shared/ruby/enc/ascii.c \
    ../../shared/ruby/generated/parse.c \
    ../../shared/ruby/missing/tgamma.c \
    ../../shared/ruby/missing/strlcpy.c \
    ../../shared/ruby/missing/strlcat.c \
    ../../shared/ruby/missing/lgamma_r.c \
    ../../shared/ruby/missing/hypot.c \
    ../../shared/ruby/missing/erf.c \
    ../../shared/ruby/missing/dup2.c \
    ../../shared/ruby/missing/crypt.c \
    ../../shared/ruby/missing/cbrt.c \
    ../../shared/ruby/missing/acosh.c \
    ../../shared/ruby/regexec.c \
    ../../shared/ruby/regerror.c \
    ../../shared/ruby/regenc.c \
    ../../shared/ruby/regcomp.c \
    ../../shared/ruby/re.c \
    ../../shared/ruby/rational.c \
    ../../shared/ruby/range.c \
    ../../shared/ruby/random.c \
    ../../shared/ruby/process.c \
    ../../shared/ruby/proc.c \
    ../../shared/ruby/pack.c \
    ../../shared/ruby/object.c \
    ../../shared/ruby/numeric.c \
    ../../shared/ruby/math.c \
    ../../shared/ruby/marshal.c \
    ../../shared/ruby/load.c \
    ../../shared/ruby/iseq.c \
    ../../shared/ruby/io.c \
    ../../shared/ruby/inits.c \
    ../../shared/ruby/hash.c \
    ../../shared/ruby/gc.c \
    ../../shared/ruby/file.c \
    ../../shared/ruby/eval.c \
    ../../shared/ruby/error.c \
    ../../shared/ruby/enumerator.c \
    ../../shared/ruby/enum.c \
    ../../shared/ruby/dmyext.c \
    ../../shared/ruby/dmyencoding.c \
    ../../shared/ruby/dln.c \
    ../../shared/ruby/dir.c \
    ../../shared/ruby/debug.c \
    ../../shared/ruby/cont.c \
    ../../shared/ruby/complex.c \
    ../../shared/ruby/compile.c \
    ../../shared/ruby/compar.c \
    ../../shared/ruby/class.c \
    ../../shared/ruby/blockinlining.c \
    ../../shared/ruby/bignum.c \
    ../../shared/ruby/array.c \
    ../../shared/ruby/vm_dump.c \
    ../../shared/ruby/vm.c \
    ../../shared/ruby/version.c \
    ../../shared/ruby/variable.c \
    ../../shared/ruby/util.c \
    ../../shared/ruby/transcode.c \
    ../../shared/ruby/time.c \
    ../../shared/ruby/thread.c \
    ../../shared/ruby/struct.c \
    ../../shared/ruby/string.c \
    ../../shared/ruby/strftime.c \
    ../../shared/ruby/st.c \
    ../../shared/ruby/sprintf.c \
    ../../shared/ruby/signal.c \
    ../../shared/ruby/safe.c \
    ../../shared/ruby/ruby.c \
    ../../shared/ruby/regsyntax.c \
    ../../shared/ruby/regparse.c \
    ../../shared/ruby/ext/alert/alert_wrap.c \
    ../../shared/ruby/ext/asynchttp/asynchttp_wrap.c \
    ../../shared/ruby/ext/bluetooth/bluetooth_wrap.c \
    ../../shared/ruby/ext/calendar/event_wrap.c \
    ../../shared/ruby/ext/calendar/calendar_wrap.c \
    ../../shared/ruby/ext/camera/camera_wrap.c \
    ../../shared/ruby/ext/datetimepicker/datetimepicker_wrap.c \
    ../../shared/ruby/ext/geolocation/geolocation_wrap.c \
    ../../shared/ruby/ext/mapview/mapview_wrap.c \
    ../../shared/ruby/ext/nativebar/nativebar_wrap.c \
    ../../shared/ruby/ext/nativeviewmanager/nativeviewmanager_wrap.c \
    ../../shared/ruby/ext/navbar/navbar_wrap.c \
    ../../shared/ruby/ext/phonebook/phonebook_wrap.c \
    ../../shared/ruby/ext/rho/rhosupport.c \
    ../../shared/ruby/ext/rho/rhoruby.c \
    ../../shared/ruby/ext/rhoconf/rhoconf_wrap.c \
    ../../shared/ruby/ext/ringtones/ringtones_wrap.c \
    ../../shared/ruby/ext/signature/signature_wrap.c \
    ../../shared/ruby/ext/socket/socket.c \
    ../../shared/ruby/ext/sqlite3_api/sqlite3_api_wrap.c \
    ../../shared/ruby/ext/stringio/stringio.c \
    ../../shared/ruby/ext/strscan/strscan.c \
    ../../shared/ruby/ext/syncengine/syncengine_wrap.c \
    ../../shared/ruby/ext/system/system_wrap.c \
    ../../shared/ruby/ext/webview/webview_wrap.c \
    ../../shared/ruby/strftime.c \
    ../../shared/ruby/newline.c

HEADERS += \
    ../../shared/ruby/id.h \
    ../../shared/ruby/gc.h \
    ../../shared/ruby/eval_intern.h \
    ../../shared/ruby/dln.h \
    ../../shared/ruby/debug.h \
    ../../shared/ruby/vm_opts.h \
    ../../shared/ruby/vm_core.h \
    ../../shared/ruby/version.h \
    ../../shared/ruby/transcode_data.h \
    ../../shared/ruby/revision.h \
    ../../shared/ruby/regparse.h \
    ../../shared/ruby/ext/calendar/event.h \
    ../../shared/ruby/ext/phonebook/phonebook.h \
    ../../shared/ruby/ext/rho/rhoruby.h \
    ../../shared/ruby/ext/socket/sockport.h \
    ../../shared/ruby/ext/socket/addrinfo.h \
    ../../shared/ruby/regint.h \
    ../../shared/ruby/regenc.h
unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

win32 {
SOURCES += ../../shared/ruby/win32/win32.c
HEADERS += ../../shared/ruby/win32/assert.h
INCLUDEPATH += ../../shared/ruby/win32
}

symbian {
INCLUDEPATH += ../../shared/ruby/symbian
HEADERS += ../../shared/ruby/symbian/ruby/config.h \
    ../../shared/ruby/symbian/symbian.h \
    ../../shared/ruby/symbian/assert.h
SOURCES +=  ../../shared/ruby/ext/socket/getnameinfo.c \
            ../../shared/ruby/ext/socket/getaddrinfo.c
}
