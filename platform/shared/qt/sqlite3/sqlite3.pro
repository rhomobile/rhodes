QT -= core
    greaterThan(QT_MINOR_VERSION, 6): {
        CONFIG += c++14
        DEFINES += CPP_ELEVEN
        DEFINES += RHODES_VERSION_2
    }

    lessThan(QT_MINOR_VERSION, 6): {
        DEFINES += RHODES_VERSION_1
    }
TARGET = sqlite3
TEMPLATE = lib

CONFIG += staticlib warn_on

INCLUDEPATH += ../..

macx {
  DESTDIR = ../../../osx/bin/sqlite3
  OBJECTS_DIR = ../../../osx/bin/sqlite3/tmp
  SOURCES += ../../sqlite/crypto.c
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
  DESTDIR = ../../../linux/bin/sqlite3
  OBJECTS_DIR = ../../../linux/bin/sqlite3/tmp
}

DEFINES += RHODES_QT_PLATFORM

!isEmpty(RHOSIMULATOR_BUILD) {
  DEFINES += RHODES_EMULATOR
}

!win32 {
    QMAKE_CFLAGS_WARN_ON += -Wno-extra -Wno-unused -Wno-sign-compare -Wno-format -Wno-parentheses
}

HEADERS += ../../sqlite/sqlite3.h\
../../sqlite/sqlite3ext.h

SOURCES += ../../sqlite/sqlite3.c
