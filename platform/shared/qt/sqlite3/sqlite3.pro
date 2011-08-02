QT -= core

TARGET = sqlite3
TEMPLATE = lib

CONFIG += staticlib warn_on

INCLUDEPATH += ../..

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

unix:!macx {
  DESTDIR = ../../../linux/bin/sqlite3
  OBJECTS_DIR = ../../../linux/bin/sqlite3/tmp
}

DEFINES += RHODES_EMULATOR

!win32 {
  QMAKE_CFLAGS_WARN_ON += -Wno-extra -Wno-unused -Wno-sign-compare -Wno-format -Wno-parentheses
}
win32 {
  QMAKE_CFLAGS_WARN_ON += /wd4101
  QMAKE_CFLAGS_RELEASE += /O2
}

HEADERS += ../../sqlite/sqlite3.h\
../../sqlite/sqlite3ext.h

SOURCES += ../../sqlite/sqlite3.c
