QT -= core

TARGET = syncengine
TEMPLATE = lib

CONFIG += staticlib warn_on

macx {
  DESTDIR = ../../../osx/build/syncengine
  OBJECTS_DIR = ../../../osx/build/syncengine/tmp
}

DEFINES += RHODES_EMULATOR

QMAKE_CFLAGS_WARN_ON += -Wno-extra -Wno-unused -Wno-sign-compare -Wno-format -Wno-parentheses
QMAKE_CXXFLAGS_WARN_ON += -Wno-extra -Wno-unused -Wno-sign-compare -Wno-format -Wno-parentheses

# QMAKE_CFLAGS += -fvisibility=hidden
# QMAKE_CXXFLAGS += -fvisibility=hidden

INCLUDEPATH += ../..

HEADERS += ../../json/arraylist.h\
../../json/bits.h\
../../json/config.h\
../../json/debug.h\
../../json/json.h\
../../json/json_object.h\
../../json/json_object_private.h\
../../json/json_tokener.h\
../../json/json_util.h\
../../json/linkhash.h\
../../json/printbuf.h\
../../sync/syncthread.h\
../../db/DBAttrManager.h\
../../sync/SyncNotify.h\
../../json/JSONIterator.h\
../../sync/SyncEngine.h\
../../sync/SyncSource.h\
../../db/DBAdapter.h\
../../db/DBResult.h\
../../sync/ClientRegister.h

SOURCES += ../../json/arraylist.c\
../../json/debug.c\
../../json/json_object.c\
../../json/json_tokener.c\
../../json/json_util.c\
../../json/linkhash.c\
../../json/printbuf.c\
../../json/RJSONTokener.c\
../../sync/syncthread.cpp\
../../db/DBAttrManager.cpp\
../../sync/SyncNotify.cpp\
../../json/JSONIterator.cpp\
../../sync/SyncEngine.cpp\
../../sync/SyncSource.cpp\
../../db/DBAdapter.cpp\
../../db/DBResult.cpp\
../../sync/ClientRegister.cpp
