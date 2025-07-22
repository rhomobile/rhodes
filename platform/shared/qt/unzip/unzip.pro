equals(QT_MAJOR_VERSION, 5) {
    lessThan(QT_MINOR_VERSION, 6): {
        DEFINES += RHODES_VERSION_1
    }
    equals(QT_MINOR_VERSION, 6): {
        DEFINES += OS_SAILFISH OS_LINUX
        CONFIG +=  c++14
        QT += core
    }
    greaterThan(QT_MINOR_VERSION, 7): {
        CONFIG += c++14
        DEFINES += CPP_ELEVEN RHODES_VERSION_2 AJAXSERVER
    }
}

TARGET = unzip
TEMPLATE = lib

CONFIG += staticlib warn_on
#CONFIG += debug

INCLUDEPATH += ../..\
../../ruby\
../../../../lib/commonAPI\
../../ruby/include

macx {
  greaterThan(QT_MINOR_VERSION, 6): {
      DEFINES += RHODES_MAC_BUILD
  }
  DESTDIR = ../../../osx/bin/unzip
  OBJECTS_DIR = ../../../osx/bin/unzip/tmp
  INCLUDEPATH += ../../curl/include\
                 ../../ruby/osx
}

win32 {
  greaterThan(QT_MINOR_VERSION, 6): {
      DEFINES += CPP_ELEVEN
  }
  DESTDIR = ../../../win32/bin/unzip
  OBJECTS_DIR = ../../../win32/bin/unzip/tmp
  DEFINES += WIN32 _WINDOWS _CRT_SECURE_NO_WARNINGS _UNICODE UNICODE WIN32_LEAN_AND_MEAN
  DEFINES += _XKEYCHECK_H

  Debug {
    DEFINES += _DEBUG DEBUG
  }
  Release {
    DEFINES += _NDEBUG NDEBUG
  }
  INCLUDEPATH += ../../../win32/include\
                 ../../ruby/win32

    QMAKE_CXXFLAGS_RELEASE += /MP9 /O2
    QMAKE_CXXFLAGS_DEBUG += /MP9 /O2

    QMAKE_CFLAGS_RELEASE += /O2 /MD
    QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO += /O2
    QMAKE_CFLAGS_DEBUG += /Zi /MDd
}

unix:!macx {
    INCLUDEPATH += $$PWD/../../ruby/linux
    HEADERS += $$PWD/../../ruby/linux/ruby/config.h
    DESTDIR = $$PWD/../../../linux/bin/unzip
    OBJECTS_DIR = $$PWD/../../../linux/bin/unzip/tmp
    INCLUDEPATH += $$PWD/../../curl/include
    DEFINES += _GNU_SOURCE

    DEFINES += OS_LINUX

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
  QMAKE_CXXFLAGS_WARN_ON += -Wno-extra -Wno-unused -Wno-sign-compare -Wno-parentheses
  QMAKE_CFLAGS_RELEASE -= -Werror=format-security
  QMAKE_CXXFLAGS_RELEASE -= -Werror=format-security
  # QMAKE_CFLAGS += -fvisibility=hidden
  # QMAKE_CXXFLAGS += -fvisibility=hidden
  QMAKE_CFLAGS_DEBUG -= -O2
  QMAKE_CXXFLAGS_DEBUG -= -O2
}
win32 {
  QMAKE_CFLAGS_WARN_ON += /wd4189 /wd4018 /wd4189 /wd4996
  QMAKE_CXXFLAGS_WARN_ON += /wd4189 /wd4018 /wd4189 /wd4996
  QMAKE_CFLAGS_RELEASE += /O2
  QMAKE_CXXFLAGS_RELEASE += /O2
}

HEADERS += ../../unzip/zip.h\
../../unzip/unzip.h\
../../unzip/gunzip.h\
../../unzip/crc32.h\
../../unzip/deflate.h\
../../unzip/inffast.h\
../../unzip/inffixed.h\
../../unzip/inflate.h\
../../unzip/gzguts.h\
../../unzip/inftrees.h\
../../unzip/trees.h\
../../unzip/unzip.h\
../../unzip/zconf.h\
../../unzip/zip.h\
../../unzip/zlib.h\
../../unzip/zutil.h
SOURCES += ../../unzip/unzip.cpp\
../../unzip/adler32.cpp\
../../unzip/crc32.cpp\
../../unzip/deflate.cpp\
../../unzip/gunzip.cpp\
../../unzip/gzclose.cpp\
../../unzip/gzlib.cpp\
../../unzip/gzread.cpp\
../../unzip/gzwrite.cpp\
../../unzip/infback.cpp\
../../unzip/inffast.cpp\
../../unzip/inflate.cpp\
../../unzip/inftrees.cpp\
../../unzip/trees.cpp\
../../unzip/unzip.cpp\
../../unzip/zip.cpp\
../../unzip/zutil.cpp
