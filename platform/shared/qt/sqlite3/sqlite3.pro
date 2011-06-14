QT -= core

TARGET = sqlite3
TEMPLATE = staticlib

# CONFIG += qt

macx {
  DESTDIR = ../../../osx/build/sqlite3
  OBJECTS_DIR = ../../../osx/build/sqlite3/tmp
}

DEFINES += RHODES_EMULATOR

INCLUDEPATH += ../..

HEADERS += ../../sqlite/sqlite3.h\
../../sqlite/sqlite3ext.h

SOURCES += ../../sqlite/sqlite3.c\
../../sqlite/crypto.c 
