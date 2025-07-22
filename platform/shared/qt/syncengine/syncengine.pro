equals(QT_MAJOR_VERSION, 5) {
    equals(QT_MINOR_VERSION, 6) {
        DEFINES += OS_SAILFISH OS_LINUX CPP_ELEVEN
        CONFIG += c++14
    }
    greaterThan(QT_MINOR_VERSION, 6): {
        CONFIG += c++14
        DEFINES += CPP_ELEVEN
        DEFINES += RHODES_VERSION_2
    }
}

lessThan(QT_MINOR_VERSION, 6): {
    DEFINES += RHODES_VERSION_1
}
TARGET = syncengine
TEMPLATE = lib

CONFIG += staticlib warn_on
#CONFIG += debug


INCLUDEPATH += $$PWD/../..\
$$PWD/../../ruby/include\
$$PWD/../../json

macx {
  DESTDIR = ../../../osx/bin/syncengine
  OBJECTS_DIR = ../../../osx/bin/syncengine/tmp
  INCLUDEPATH += ../../ruby/osx
}

win32 {
  DESTDIR = ../../../win32/bin/syncengine
  OBJECTS_DIR = ../../../win32/bin/syncengine/tmp
  DEFINES += WIN32 _WINDOWS _LIB _UNICODE UNICODE
  DEFINES += BUFSIZ=512 STATIC_LINKED RUBY_EXPORT WIN32_LEAN_AND_MEAN
  INCLUDEPATH += ../../ruby/win32
  Debug {
    DEFINES += _DEBUG DEBUG
  }
  Release {
    DEFINES += _NDEBUG NDEBUG
  }
}

unix:!macx {
    INCLUDEPATH += $$PWD/../../ruby/linux
    HEADERS += $$PWD/../../ruby/linux/ruby/config.h
    DESTDIR = $$PWD/../../../linux/bin/syncengine
    OBJECTS_DIR = $$PWD/../../../linux/bin/syncengine/tmp
    DEFINES += _GNU_SOURCE OS_LINUX
    QMAKE_CFLAGS += -fvisibility=hidden
    QMAKE_CXXFLAGS += -fvisibility=hidden

    INCLUDEPATH += $$PWD/../../ruby/include\
                 $$PWD/../../ruby/linux

    #QMAKE_CFLAGS_RELEASE -= -O2
    #QMAKE_CXXFLAGS_RELEASE -= -O2
}

DEFINES += RHODES_QT_PLATFORM

!isEmpty(RHOSIMULATOR_BUILD) {
  DEFINES += RHODES_EMULATOR
}

!win32 {
  QMAKE_CFLAGS_WARN_ON += -Wno-extra -Wno-unused -Wno-sign-compare -Wno-parentheses
  QMAKE_CXXFLAGS_WARN_ON += -Wno-extra -Wno-unused -Wno-sign-compare -Wno-parentheses
  MAKE_CFLAGS_RELEASE -= -Werror=format-security
  QMAKE_CXXFLAGS_RELEASE -= -Werror=format-security
  # QMAKE_CFLAGS += -fvisibility=hidden
  # QMAKE_CXXFLAGS += -fvisibility=hidden
  QMAKE_CFLAGS_DEBUG -= -O2
  QMAKE_CXXFLAGS_DEBUG -= -O2
}
win32 {
    QMAKE_CFLAGS_WARN_ON += /wd4996 /wd4100
    QMAKE_CXXFLAGS_WARN_ON += /wd4996 /wd4100
    QMAKE_CXXFLAGS_RELEASE += /MP9 /O2
    QMAKE_CXXFLAGS_DEBUG += /MP9 /O2

    QMAKE_CFLAGS_RELEASE += /O2 /MD
    QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO += /O2
    QMAKE_CFLAGS_DEBUG += /Zi /MDd
}


HEADERS += ../../json.new/arraylist.h\
../../json.new/strerror_override_private.h \
../../json.new/strerror_override.h \
../../json.new/snprintf_compat.h \
../../json.new/random_seed.h \
../../json.new/printbuf.h \
../../json.new/math_compat.h \
../../json.new/linkhash.h \
../../json.new/json_visit.h \
../../json.new/json_util.h \
../../json.new/json_tokener.h \
../../json.new/json_pointer.h \
../../json.new/json_object_private.h \
../../json.new/json_object_iterator.h \
../../json.new/json_object.h \
../../json.new/json_inttypes.h \
../../json.new/json_config.h \
../../json.new/json_c_version.h \
../../json.new/json.h \
../../json.new/debug.h \
../../json.new/config.h \
../../json.new/bits.h\
../../sync/RhoconnectClientManager.h\
../../sync/ILoginListener.h\
../../db/DBAdapter.h\
../../db/DBAttrManager.h\
../../db/DBImportTransaction.h\
../../db/DBRequestHelper.h\
../../db/DBResult.h\
../../db/DBLock.h

SOURCES += ../../json.new/arraylist.c\
../../json.new/JSONIterator.cpp\
../../json.new/RJSONTokener.c\
../../json.new/arraylist.c\
../../json.new/json_object.c\
../../json.new/json_tokener.c\
../../json.new/json_util.c\
../../json.new/linkhash.c\
../../json.new/printbuf.c\
../../json.new/debug.c\
../../json.new/json_c_version.c\
../../json.new/json_object_iterator.c\
../../json.new/json_pointer.c\
../../json.new/json_visit.c\
../../json.new/random_seed.c\
../../json.new/strerror_override.c\
../../sync/RhoconnectClientManager.cpp\
../../db/DBAdapter.cpp\
../../db/DBAttrManager.cpp\
../../db/DBImportTransaction.cpp\
../../db/DBRequestHelper.cpp\
../../db/DBResult.cpp\
../../db/DBLock.cpp

