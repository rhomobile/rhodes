QT -= core
    greaterThan(QT_VERSION, 5.6.0): {
        CONFIG += c++14
        DEFINES += CPP_ELEVEN
        DEFINES += RHODES_VERSION_2
    }

    lessThan(QT_VERSION, 5.6.0): {
        DEFINES += RHODES_VERSION_1
    }
TARGET = syncengine
TEMPLATE = lib

CONFIG += staticlib warn_on

INCLUDEPATH += ../..\
#../../ruby/include\
../../json

macx {
  DESTDIR = ../../../osx/bin/syncengine
  OBJECTS_DIR = ../../../osx/bin/syncengine/tmp
  #INCLUDEPATH += ../../ruby/osx
}

win32 {
  DESTDIR = ../../../win32/bin/syncengine
  OBJECTS_DIR = ../../../win32/bin/syncengine/tmp
  DEFINES += WIN32 _WINDOWS _LIB _UNICODE UNICODE
  DEFINES += BUFSIZ=512 STATIC_LINKED RUBY_EXPORT
  #INCLUDEPATH += ../../ruby/win32
  Debug {
    DEFINES += _DEBUG DEBUG
  }
  Release {
    DEFINES += _NDEBUG NDEBUG
  }
}

unix:!macx {
  DESTDIR = ../../../linux/bin/syncengine
  OBJECTS_DIR = ../../../linux/bin/syncengine/tmp
  DEFINES += _GNU_SOURCE
}

DEFINES += RHODES_QT_PLATFORM

!isEmpty(RHOSIMULATOR_BUILD) {
  DEFINES += RHODES_EMULATOR
}

!win32 {
  QMAKE_CFLAGS_WARN_ON += -Wno-extra -Wno-unused -Wno-sign-compare -Wno-format -Wno-parentheses
  QMAKE_CXXFLAGS_WARN_ON += -Wno-extra -Wno-unused -Wno-sign-compare -Wno-format -Wno-parentheses
  # QMAKE_CFLAGS += -fvisibility=hidden
  # QMAKE_CXXFLAGS += -fvisibility=hidden
}
win32 {
  QMAKE_CFLAGS_WARN_ON += /wd4996 /wd4100
  QMAKE_CXXFLAGS_WARN_ON += /wd4996 /wd4100
  QMAKE_CFLAGS_RELEASE += /O2
  QMAKE_CXXFLAGS_RELEASE += /O2
  QMAKE_CXXFLAGS_RELEASE += -MP9
  QMAKE_CXXFLAGS_DEBUG += -MP9
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
../../db/DBResult.h

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
../../db/DBResult.cpp
