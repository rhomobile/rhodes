equals(QT_MAJOR_VERSION, 5) {
    lessThan(QT_MINOR_VERSION, 6): {
        DEFINES += RHODES_VERSION_1
    }
    equals(QT_MINOR_VERSION, 6): {
        DEFINES += OS_SAILFISH OS_LINUX
        CONFIG +=  c++14
    }
    greaterThan(QT_MINOR_VERSION, 7): {
        CONFIG += c++14
        DEFINES += CPP_ELEVEN RHODES_VERSION_2
    }
}

TARGET = zlib
TEMPLATE = lib

CONFIG += staticlib warn_on

INCLUDEPATH += \
../../../../platform/shared/ruby/include\
../../../../platform/shared/ruby\
../../../../platform/shared\
../../../../platform/shared/common

macx {
  DESTDIR = ../../../../platform/osx/bin/extensions
  OBJECTS_DIR = ../../../../platform/osx/bin/extensions/zlib
  INCLUDEPATH += ../../../../platform/shared/ruby/iphone
}

unix:!macx {
    DEFINES += OS_LINUX
    DESTDIR = ../../../../platform/linux/bin/extensions
    OBJECTS_DIR = ../../../../platform/linux/bin/extensions/zlib
    INCLUDEPATH += ../../../../platform/shared/ruby/linux
    QMAKE_CFLAGS += -fvisibility=hidden
    QMAKE_CXXFLAGS += -fvisibility=hidden
}

win32 {
  DESTDIR = ../../../../platform/win32/bin/extensions
  OBJECTS_DIR = ../../../../platform/win32/bin/extensions/zlib
  DEFINES += WIN32 _WINDOWS _LIB _UNICODE UNICODE WIN32_LEAN_AND_MEAN 
  Debug {
    DEFINES += _DEBUG DEBUG
  }
  Release {
    DEFINES += _NDEBUG NDEBUG
  }
  INCLUDEPATH += ../../../../platform/shared/ruby/win32
}

unix:!macx {
    DEFINES += OS_LINUX
    DESTDIR = $$PWD/../../../../platform/linux/bin/extensions
    OBJECTS_DIR = $$PWD/../../../../platform/linux/bin/extensions/zlib
    INCLUDEPATH += $$PWD/../../../../platform/shared/ruby/linux
    QMAKE_CFLAGS += -fvisibility=hidden
    QMAKE_CXXFLAGS += -fvisibility=hidden
}

DEFINES += RHODES_QT_PLATFORM _XOPEN_SOURCE _DARWIN_C_SOURCE

!isEmpty(RHOSIMULATOR_BUILD) {
  DEFINES += RHODES_EMULATOR
}

!win32 {
  QMAKE_CFLAGS_WARN_ON += -Wno-extra -Wno-unused -Wno-sign-compare -Wno-parentheses
  QMAKE_CXXFLAGS_WARN_ON += -Wno-extra -Wno-unused -Wno-sign-compare -Wno-parentheses
  QMAKE_CFLAGS_RELEASE -= -Werror=format-security
  QMAKE_CXXFLAGS_RELEASE -= -Werror=format-security
  QMAKE_CFLAGS_DEBUG -= -O2
  QMAKE_CXXFLAGS_DEBUG -= -O2

}
win32 {
    QMAKE_CFLAGS_WARN_ON += /wd4996 /wd4100 /wd4005
    QMAKE_CXXFLAGS_WARN_ON += /wd4996 /wd4100 /wd4005
    QMAKE_CXXFLAGS_RELEASE += /MP9 /O2
    QMAKE_CXXFLAGS_DEBUG += /MP9 /O2

    QMAKE_CFLAGS_RELEASE += /O2 /MD
    QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO += /O2
    QMAKE_CFLAGS_DEBUG += /Zi /MDd
}

HEADERS += \
crc32.h\
deflate.h\
gzguts.h\
inffast.h\
inffixed.h\
inflate.h\
inftrees.h\
trees.h\
zconf.h\
zlib.h\
zutil.h

SOURCES += \
adler32.c\
compress.c\
crc32.c\
deflate.c\
gzclose.c\
gzlib.c\
gzread.c\
gzwrite.c\
infback.c\
inffast.c\
inflate.c\
inftrees.c\
trees.c\
uncompr.c\
zlib.c\
zutil.c
