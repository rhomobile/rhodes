QT -= core

TARGET = syncengine
TEMPLATE = staticlib

# CONFIG += qt

macx {
  DESTDIR = ../../../osx/build/syncengine
  OBJECTS_DIR = ../../../osx/build/syncengine/tmp
}

DEFINES += RHODES_EMULATOR

INCLUDEPATH += ../..\
../../ruby

SOURCES += 

HEADERS +=
