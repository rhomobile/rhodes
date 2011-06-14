QT -= core

TARGET = rholib
TEMPLATE = staticlib

# CONFIG += qt

macx {
  DESTDIR = ../../../osx/build/rholib
  OBJECTS_DIR = ../../../osx/build/rholib/tmp
}

DEFINES += RHODES_EMULATOR

INCLUDEPATH += ../..\
../../ruby

SOURCES += 

HEADERS +=
