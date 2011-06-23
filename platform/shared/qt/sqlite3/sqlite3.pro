QT -= core

TARGET = sqlite3
TEMPLATE = lib

CONFIG += staticlib warn_on

macx {
  DESTDIR = ../../../osx/bin/sqlite3
  OBJECTS_DIR = ../../../osx/bin/sqlite3/tmp
  SOURCES += ../../sqlite/crypto.c
}

win32 {
  DESTDIR = ../../../win32/bin/sqlite3
  OBJECTS_DIR = ../../../win32/bin/sqlite3/tmp
  DEFINES += _NDEBUG NDEBUG WIN32 _WINDOWS _LIB _UNICODE UNICODE
}

DEFINES += RHODES_EMULATOR

!win32 {
  QMAKE_CFLAGS_WARN_ON += -Wno-extra -Wno-unused -Wno-sign-compare -Wno-format -Wno-parentheses
  QMAKE_CXXFLAGS_WARN_ON += -Wno-extra -Wno-unused -Wno-sign-compare -Wno-format -Wno-parentheses
}
win32 {
  QMAKE_CFLAGS_RELEASE += /O2
  QMAKE_CXXFLAGS_RELEASE += /O2
}

INCLUDEPATH += ../..

HEADERS += ../../sqlite/sqlite3.h\
../../sqlite/sqlite3ext.h

SOURCES += ../../sqlite/sqlite3.c
