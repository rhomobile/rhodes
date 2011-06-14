QT -= core

TARGET = rubylib
TEMPLATE = staticlib

# CONFIG += qt

macx {
  DESTDIR = ../../../osx/build/rubylib
  OBJECTS_DIR = ../../../osx/build/rubylib/tmp
}

DEFINES += RHODES_EMULATOR

INCLUDEPATH += ../..\
../../ruby

SOURCES += 

HEADERS +=
