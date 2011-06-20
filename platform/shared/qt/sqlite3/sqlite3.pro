QT -= core

TARGET = sqlite3
TEMPLATE = lib

CONFIG += staticlib warn_on

macx {
  DESTDIR = ../../../osx/build/sqlite3
  OBJECTS_DIR = ../../../osx/build/sqlite3/tmp
}

DEFINES += RHODES_EMULATOR

QMAKE_CFLAGS_WARN_ON += -Wno-extra -Wno-unused -Wno-sign-compare -Wno-format -Wno-parentheses
QMAKE_CXXFLAGS_WARN_ON += -Wno-extra -Wno-unused -Wno-sign-compare -Wno-format -Wno-parentheses

INCLUDEPATH += ../..

HEADERS += ../../sqlite/sqlite3.h\
../../sqlite/sqlite3ext.h

SOURCES += ../../sqlite/sqlite3.c\
../../sqlite/crypto.c 
