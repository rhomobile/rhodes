QT -= core

TARGET = sqlite3
TEMPLATE = staticlib

# CONFIG += qt

macx {
  DESTDIR = ../../../osx/build/sqlite3
  OBJECTS_DIR = ../../../osx/build/sqlite3/tmp
}

DEFINES += RHODES_EMULATOR

INCLUDEPATH += ../..\
../../ruby

SOURCES += 

HEADERS +=
