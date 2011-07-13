#-------------------------------------------------
#
# Project created by QtCreator 2011-06-15T21:19:15
#
#-------------------------------------------------

QT       -= gui

TARGET = sqlite3
TEMPLATE = lib
CONFIG += staticlib warn_on
INCLUDEPATH += ../../shared/
DEFINES += __BSD_VISIBLE=1 \
           SQLITE_OS_UNIX=1 \
           HAVE_LOCALTIME_R=1 \
           OS_SYMBIAN=1

SOURCES += \
    #../../shared/sqlite/sqlite3.c
    src/sqlite3.c \
    src/os_symbian.cpp \
    src/mutex.cpp

HEADERS += \
    src/sqlite3.h \
    src/os_symbian.h
    #../../shared/sqlite/sqlite3ext.h \
    #../../shared/sqlite/sqlite3.h
