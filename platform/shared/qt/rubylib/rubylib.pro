equals(QT_MAJOR_VERSION, 5) {
    lessThan(QT_MINOR_VERSION, 6): {
        DEFINES += RHODES_VERSION_1
    }
    equals(QT_MINOR_VERSION, 6) {
        QT += core
        DEFINES += OS_SAILFISH OS_LINUX CPP_ELEVEN
        CONFIG += c++14
    }
    greaterThan(QT_MINOR_VERSION, 6): {
      CONFIG += c++14
      DEFINES += RHODES_VERSION_2
      QMAKE_CXXFLAGS += -nologo -DNT=1 -ML -Zi -O2b2x -G5
      LIBS += -lmsvcrt.lib
      LIBS += -lvcruntime.lib
      LIBS += -lucrt.lib
  }
}


TARGET = rubylib
TEMPLATE = lib

CONFIG += staticlib warn_on
#CONFIG += debug

INCLUDEPATH += ../../ruby/include\
../../ruby\
../../ruby/generated\
../..

macx {
  greaterThan(QT_MINOR_VERSION, 6): {
      DEFINES += RHODES_MAC_BUILD
      DEFINES += CPP_ELEVEN
  }
  DEFINES += RUBY_EXPORT
  DESTDIR = ../../../osx/bin/rubylib
  OBJECTS_DIR = ../../../osx/bin/rubylib/tmp
  INCLUDEPATH += ../../ruby/osx
  HEADERS += ../../ruby/ext/socket/constants.h\
  ../../ruby/osx/ruby/config.h\
  ../../ruby/osx/crt_externs.h\
  ../../ruby/iseq.h\
  ../../ruby/thread_pthread.h
  SOURCES += ../../ruby/newline.c\
  ../../ruby/thread_pthread.c\
  ../../ruby/missing/setproctitle.c \
  ../../ruby/missing/explicit_bzero.c
}

win32 {
  greaterThan(QT_MINOR_VERSION, 6): {
      DEFINES += CPP_ELEVEN
  }
  DESTDIR = ../../../win32/bin/rubylib
  OBJECTS_DIR = ../../../win32/bin/rubylib/tmp
  INCLUDEPATH += ../../ruby/win32
  DEFINES -= _UNICODE UNICODE
  DEFINES += _WIN32 WIN32 _WINDOWS _LIB BUFSIZ=512 STATIC_LINKED RUBY_EXPORT WIN32_LEAN_AND_MEAN
  Debug {
    DEFINES += _DEBUG DEBUG
  }
  Release {
    DEFINES += _NDEBUG NDEBUG
  }

  HEADERS += ../../ruby/win32/ruby/config.h\
../../ruby/win32/dir.h

  SOURCES += ../../ruby/missing/acosh.c \
../../ruby/missing/alloca.c \
../../ruby/missing/cbrt.c \
../../ruby/missing/crypt.c \
../../ruby/missing/dup2.c \
../../ruby/missing/erf.c \
../../ruby/missing/explicit_bzero.c \
../../ruby/missing/ffs.c \
../../ruby/missing/fileblocks.c \
../../ruby/missing/finite.c \
../../ruby/missing/flock.c \
../../ruby/missing/hypot.c \
../../ruby/missing/isinf.c \
../../ruby/missing/isnan.c \
../../ruby/missing/langinfo.c \
../../ruby/missing/lgamma_r.c \
../../ruby/missing/memmove.c \
../../ruby/missing/nextafter.c \
../../ruby/missing/setproctitle.c \
../../ruby/missing/stdlib.c \
../../ruby/missing/strchr.c \
../../ruby/missing/strerror.c \
../../ruby/missing/strlcat.c \
../../ruby/missing/strlcpy.c \
../../ruby/missing/strstr.c \
../../ruby/missing/tgamma.c \
../../ruby/win32/win32.c \
#../../ruby/wince/io_wce.c\
#../../ruby/wince/process_wce.c\
#../../ruby/wince/time_wce.c
../../ruby/win32/file_win32.c \
../../ruby/win32/newline_win32.c
}

unix:!macx {
    DEFINES += OS_LINUX
    DESTDIR = $$PWD/../../../linux/bin/rubylib
    OBJECTS_DIR = $$PWD/../../../linux/bin/rubylib/tmp
    INCLUDEPATH += $$PWD/../../ruby/linux
    HEADERS += $$PWD/../../ruby/linux/ruby/config.h
    SOURCES += \
        #../../ruby/miniprelude.c\
        ../../ruby/missing/acosh.c\
        ../../ruby/missing/cbrt.c\
        ../../ruby/missing/crypt.c\
        ../../ruby/missing/dup2.c\
        ../../ruby/missing/erf.c\
        ../../ruby/missing/hypot.c\
        ../../ruby/missing/stdlib.c\
        ../../ruby/missing/strlcat.c\
        ../../ruby/missing/strlcpy.c\
        ../../ruby/missing/tgamma.c\
        ../../ruby/newline.c\
        ../../ruby/missing/explicit_bzero.c\
        ../../ruby/missing/setproctitle.c
    QMAKE_CFLAGS += -fvisibility=hidden
    QMAKE_CXXFLAGS += -fvisibility=hidden

    QMAKE_CFLAGS_RELEASE -= -O2
    QMAKE_CXXFLAGS_RELEASE -= -O2

}

DEFINES += RHODES_QT_PLATFORM

!isEmpty(RHOSIMULATOR_BUILD) {
  DEFINES += RHODES_EMULATOR
}

!win32 {
  QMAKE_CFLAGS_WARN_ON += -Wno-extra -Wno-unused -Wno-sign-compare -Wno-format -Wno-format-security -Wno-parentheses
  # incompatible with gcc 4.2 / Xcode 4: -Wself-assign
  QMAKE_CFLAGS_DEBUG -= -O2
  QMAKE_CXXFLAGS_DEBUG -= -O2
}
win32 {
    QMAKE_CFLAGS_WARN_ON += /wd4244 /wd4133 /wd4996 /wd4554 /wd4018 /wd4101 /wd4005 /wd4146 /wd4047 /wd4100 /wd4189 /wd4646 /wd4645
    QMAKE_CXXFLAGS_RELEASE += /MP9 /O2
    QMAKE_CXXFLAGS_DEBUG += /MP9 /O2

    QMAKE_CFLAGS_RELEASE += /O2 /MD
    QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO += /O2
    QMAKE_CFLAGS_DEBUG += /Zi /MDd
}

HEADERS += ../../ruby/ext/rho/rhoruby.h\
../../ruby/ext/calendar/event.h\
../../ruby/dln.h\
../../ruby/eval_intern.h\
../../ruby/gc.h\
../../ruby/id.h\
../../ruby/node.h\
../../ruby/regenc.h\
../../ruby/regint.h\
../../ruby/regparse.h\
../../ruby/revision.h\
../../ruby/transcode_data.h\
../../ruby/version.h\
../../ruby/vm_core.h\
../../ruby/vm_opts.h

SOURCES += \
../../ruby/array.c \
../../ruby/bignum.c \
../../ruby/class.c \
../../ruby/compar.c \
../../ruby/compile.c \
../../ruby/complex.c \
../../ruby/cont.c \
../../ruby/debug.c \
../../ruby/dir.c \
../../ruby/dln.c \
../../ruby/dln_find.c \
../../ruby/dmyencoding.c \
../../ruby/dmyext.c \
../../ruby/enc/ascii.c \
../../ruby/enc/unicode.c \
../../ruby/enc/us_ascii.c \
../../ruby/enc/utf_8.c \
../../ruby/enc/encdb.c \
../../ruby/enc/iso_8859_1.c \
../../ruby/enc/iso_8859_9.c \
../../ruby/enc/utf_16be.c \
../../ruby/enc/utf_16le.c \
../../ruby/enc/utf_32be.c \
../../ruby/enc/utf_32le.c \
../../ruby/enc/euc_jp.c \
../../ruby/enc/windows_31j.c \
../../ruby/enc/trans/single_byte.c \
../../ruby/enc/trans/utf_16_32.c \
../../ruby/enum.c \
../../ruby/enumerator.c \
../../ruby/error.c \
../../ruby/eval.c \
../../ruby/ext/bluetooth/bluetooth_wrap.c \
../../ruby/ext/nativeviewmanager/nativeviewmanager_wrap.c \
../../ruby/ext/calendar/calendar_wrap.c \
../../ruby/ext/camera/camera_wrap.c \
../../ruby/ext/calendar/event_wrap.c \
../../ruby/ext/datetimepicker/datetimepicker_wrap.c \
../../ruby/ext/geolocation/geolocation_wrap.c \
../../ruby/ext/mapview/mapview_wrap.c \
../../ruby/ext/nativebar/nativebar_wrap.c \
../../ruby/ext/navbar/navbar_wrap.c \
../../ruby/ext/phonebook/phonebook_wrap.c \
../../ruby/ext/rho/extensions.c \
../../ruby/ext/rho/rhoruby.c \
../../ruby/ext/rho/rhosupport.c \
../../ruby/ext/rhoconf/rhoconf_wrap.c \
../../ruby/ext/ringtones/ringtones_wrap.c \
../../ruby/ext/date/date_parse.c \
../../ruby/ext/date/date_strptime.c \
../../ruby/ext/date/date_core.c \
../../ruby/ext/date/date_strftime.c \
../../ruby/ext/socket/ancdata.c \
../../ruby/ext/socket/basicsocket.c \
../../ruby/ext/socket/constants.c \
../../ruby/ext/socket/constdefs.c \
#../../ruby/ext/socket/getaddrinfo.c \
#../../ruby/ext/socket/getnameinfo.c \
../../ruby/ext/socket/ifaddr.c \
../../ruby/ext/socket/init.c \
../../ruby/ext/socket/ipsocket.c \
../../ruby/ext/socket/option.c \
../../ruby/ext/socket/raddrinfo.c \
../../ruby/ext/socket/socket.c \
../../ruby/ext/socket/sockssocket.c \
../../ruby/ext/socket/tcpserver.c \
../../ruby/ext/socket/tcpsocket.c \
../../ruby/ext/socket/udpsocket.c \
../../ruby/ext/socket/unixserver.c \
../../ruby/ext/socket/unixsocket.c \
../../ruby/file.c \
../../ruby/gc.c \
../../ruby/parse.c \
../../ruby/hash.c \
../../ruby/inits.c \
../../ruby/io.c \
../../ruby/iseq.c \
../../ruby/load.c \
../../ruby/loadpath.c \
../../ruby/localeinit.c \
../../ruby/marshal.c \
../../ruby/math.c \
../../ruby/prelude.c \
#../../ruby/newline.c \
../../ruby/node.c \
../../ruby/numeric.c \
../../ruby/object.c \
../../ruby/pack.c \
../../ruby/proc.c \
../../ruby/process.c \
../../ruby/random.c \
../../ruby/range.c \
../../ruby/rational.c \
../../ruby/re.c \
../../ruby/regcomp.c \
../../ruby/regenc.c \
../../ruby/regerror.c \
../../ruby/regexec.c \
../../ruby/regparse.c \
../../ruby/regsyntax.c \
../../ruby/ruby.c \
../../ruby/safe.c \
../../ruby/signal.c \
../../ruby/sprintf.c \
../../ruby/st.c \
../../ruby/strftime.c \
../../ruby/string.c \
../../ruby/struct.c \
../../ruby/symbol.c \
../../ruby/thread.c \
../../ruby/time.c \
../../ruby/transcode.c \
../../ruby/util.c \
../../ruby/variable.c \
../../ruby/version.c \
../../ruby/vm.c \
../../ruby/vm_backtrace.c \
../../ruby/vm_dump.c \
../../ruby/vm_trace.c \
../../ruby/ext/strscan/strscan.c \
../../ruby/ext/stringio/stringio.c \
../../ruby/rhoinit.c \
../../ruby/ext/io/nonblock/nonblock.c \
../../ruby/ext/io/wait/wait.c
