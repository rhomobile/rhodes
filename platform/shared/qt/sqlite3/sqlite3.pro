QT -= core
    greaterThan(QT_VERSION, 5.7.0): {
        CONFIG += c++14
        DEFINES += CPP_ELEVEN
        DEFINES += RHODES_VERSION_2
    }

    lessThan(QT_VERSION, 5.7.0): {
        DEFINES += RHODES_VERSION_1
    }
TARGET = sqlite3
TEMPLATE = lib

CONFIG += staticlib warn_on

INCLUDEPATH += ../..

macx {
  DESTDIR = ../../../osx/bin/sqlite3
  OBJECTS_DIR = ../../../osx/bin/sqlite3/tmp
  SOURCES += ../../sqlite/sqlite3.c
}

win32 {
  DESTDIR = ../../../win32/bin/sqlite3
  OBJECTS_DIR = ../../../win32/bin/sqlite3/tmp
  DEFINES += WIN32 _WINDOWS _LIB _UNICODE UNICODE
  Debug {
    DEFINES += _DEBUG DEBUG
  }
  Release {
    DEFINES += _NDEBUG NDEBUG
  }
}

unix:!macx {
  DESTDIR = ../../../linux/bin/sqlite3
  OBJECTS_DIR = ../../../linux/bin/sqlite3/tmp
  SOURCES += ../../sqlite/sqlite3.c
}

DEFINES += RHODES_QT_PLATFORM

!isEmpty(RHOSIMULATOR_BUILD) {
  DEFINES += RHODES_EMULATOR
}

!win32 {
  QMAKE_CFLAGS_WARN_ON += -Wno-extra -Wno-unused -Wno-sign-compare -Wno-format -Wno-parentheses
}
win32 {
  QMAKE_CFLAGS_WARN_ON += /wd4101
  QMAKE_CFLAGS_RELEASE += /O2
  QMAKE_CXXFLAGS_RELEASE += -MP9
  QMAKE_CXXFLAGS_DEBUG += -MP9
}

HEADERS += ../../sqlite/sqlite3.h\
../../sqlite/sqlite3ext.h

SOURCES += ../../sqlite/sqlite3.c \
    ../../sqlite/crypto.c
