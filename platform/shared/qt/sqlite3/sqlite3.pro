equals(QT_MAJOR_VERSION, 5) {
    equals(QT_MINOR_VERSION, 6) {
        DEFINES += OS_SAILFISH OS_LINUX CPP_ELEVEN
        CONFIG += c++14
    }
    greaterThan(QT_MINOR_VERSION, 6): {
        CONFIG += c++14
        DEFINES += CPP_ELEVEN
        DEFINES += RHODES_VERSION_2
    }
}

lessThan(QT_MINOR_VERSION, 6): {
    DEFINES += RHODES_VERSION_1
}
TARGET = sqlite3
TEMPLATE = lib

CONFIG += staticlib warn_on
#CONFIG += debug

INCLUDEPATH += ../..

macx {
  DESTDIR = ../../../osx/bin/sqlite3
  OBJECTS_DIR = ../../../osx/bin/sqlite3/tmp
}

win32 {
  DESTDIR = ../../../win32/bin/sqlite3
  OBJECTS_DIR = ../../../win32/bin/sqlite3/tmp
  DEFINES += WIN32 _WINDOWS _LIB _UNICODE UNICODE WIN32_LEAN_AND_MEAN
  Debug {
    DEFINES += _DEBUG DEBUG
  }
  Release {
    DEFINES += _NDEBUG NDEBUG
  }

    QMAKE_CFLAGS_WARN_ON += /wd4101

    QMAKE_CXXFLAGS_RELEASE += /MP9 /O2 /DSQLITE_ENABLE_FTS4 /DSQLITE_ENABLE_RTREE /DSQLITE_ENABLE_COLUMN_METADATA
    QMAKE_CXXFLAGS_DEBUG += /MP9 /O2 /DSQLITE_ENABLE_FTS4 /DSQLITE_ENABLE_RTREE /DSQLITE_ENABLE_COLUMN_METADATA

    QMAKE_CFLAGS_RELEASE += /O2 /MD /DSQLITE_ENABLE_FTS4 /DSQLITE_ENABLE_RTREE /DSQLITE_ENABLE_COLUMN_METADATA
    QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO += /O2 /DSQLITE_ENABLE_FTS4 /DSQLITE_ENABLE_RTREE /DSQLITE_ENABLE_COLUMN_METADATA
    QMAKE_CFLAGS_DEBUG += /Zi /MDd /DSQLITE_ENABLE_FTS4 /DSQLITE_ENABLE_RTREE /DSQLITE_ENABLE_COLUMN_METADATA
}

unix:!macx {
    DEFINES += OS_LINUX
    INCLUDEPATH += $$PWD/../../ruby/linux
    HEADERS += $$PWD/../../ruby/linux/ruby/config.h

    DESTDIR = ../../../linux/bin/sqlite3
    OBJECTS_DIR = ../../../linux/bin/sqlite3/tmp

    QMAKE_CFLAGS += -fvisibility=hidden
    QMAKE_CXXFLAGS += -fvisibility=hidden

    #QMAKE_CFLAGS_RELEASE -= -O2
    #QMAKE_CXXFLAGS_RELEASE -= -O2
}

DEFINES += RHODES_QT_PLATFORM

!isEmpty(RHOSIMULATOR_BUILD) {
  DEFINES += RHODES_EMULATOR
}

!win32 {
  QMAKE_CFLAGS_WARN_ON += -Wno-extra -Wno-unused -Wno-sign-compare -Wno-parentheses
  QMAKE_CFLAGS_RELEASE -= -Werror=format-security
  QMAKE_CFLAGS_DEBUG -= -O2
  QMAKE_CXXFLAGS_DEBUG -= -O2
}
win32 {
  QMAKE_CFLAGS_WARN_ON += /wd4101
  QMAKE_CFLAGS_RELEASE += /O2
  QMAKE_CXXFLAGS_RELEASE += -MP9
  QMAKE_CXXFLAGS_DEBUG += -MP9
}

HEADERS += ../../sqlite/sqlite3.h\
../../sqlite/sqlite3ext.h

SOURCES += ../../sqlite/sqlite3.c \
    ../../sqlite/crypto.c
