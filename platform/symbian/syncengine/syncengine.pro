#-------------------------------------------------
#
# Project created by QtCreator 2011-06-15T21:21:18
#
#-------------------------------------------------

QT       -= gui

TARGET = syncengine
TEMPLATE = lib
CONFIG += staticlib
INCLUDEPATH += ../../shared/

SOURCES += \
    ../../shared/db/DBResult.cpp \
    ../../shared/db/DBAttrManager.cpp \
    ../../shared/db/DBAdapter.cpp \
    ../../shared/json/RJSONTokener.c \
    ../../shared/json/printbuf.c \
    ../../shared/json/linkhash.c \
    ../../shared/json/JSONIterator.cpp \
    ../../shared/json/json_util.c \
    ../../shared/json/json_tokener.c \
    ../../shared/json/json_object.c \
    ../../shared/json/json_debug.c \
    ../../shared/json/arraylist.c \
    ../../shared/sync/SyncThread.cpp \
    ../../shared/sync/SyncSource.cpp \
    ../../shared/sync/SyncNotify.cpp \
    ../../shared/sync/SyncEngine.cpp \
    ../../shared/sync/ClientRegister.cpp

HEADERS += \
    ../../shared/db/DBResult.h \
    ../../shared/db/DBAttrManager.h \
    ../../shared/db/DBAdapter.h \
    ../../shared/json/printbuf.h \
    ../../shared/json/linkhash.h \
    ../../shared/json/JSONIterator.h \
    ../../shared/json/json_util.h \
    ../../shared/json/json_tokener.h \
    ../../shared/json/json_object_private.h \
    ../../shared/json/json_object.h \
    ../../shared/json/json.h \
    ../../shared/json/json_debug.h \
    ../../shared/json/config.h \
    ../../shared/json/bits.h \
    ../../shared/json/arraylist.h \
    ../../shared/sync/SyncThread.h \
    ../../shared/sync/SyncSource.h \
    ../../shared/sync/SyncProtocol_3.h \
    ../../shared/sync/SyncNotify.h \
    ../../shared/sync/SyncEngine.h \
    ../../shared/sync/ISyncProtocol.h \
    ../../shared/sync/ClientRegister.h
unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
