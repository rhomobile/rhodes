QT -= core

TARGET = serialport
TEMPLATE = lib

CONFIG += staticlib warn_on

INCLUDEPATH += ../../../../platform/shared/ruby/include\
../../../../platform/shared



equals(QT_MAJOR_VERSION, 5) {
    lessThan(QT_MINOR_VERSION, 6): {
        QT += webkit widgets webkitwidgets multimediawidgets
        DEFINES += RHODES_VERSION_1
    }
    equals(QT_MINOR_VERSION, 6): {
        QT += webkit widgets
        DEFINES += OS_SAILFISH OS_LINUX
        CONFIG += auroraapp c++14 auroraapp_i18n
    }
    greaterThan(QT_MINOR_VERSION, 7): {
        QT += webengine webenginecore webenginewidgets multimediawidgets
        CONFIG += c++14
        DEFINES += CPP_ELEVEN RHODES_VERSION_2
    }
}


macx {
  DESTDIR = ../../../../platform/osx/bin/extensions
  OBJECTS_DIR = ../../../../platform/osx/bin/extensions/serialport
  INCLUDEPATH += ../../../../platform/shared/ruby/iphone
}
win32 {
  DESTDIR = ../../../../platform/win32/bin/extensions
  OBJECTS_DIR = ../../../../platform/win32/bin/extensions/serialport
  DEFINES += WIN32 _WINDOWS _LIB _UNICODE UNICODE WIN32_LEAN_AND_MEAN OS_MSWIN
  Debug {
    DEFINES += _DEBUG DEBUG
  }
  Release {
    DEFINES += _NDEBUG NDEBUG
  }
  INCLUDEPATH += ../../../../platform/shared/ruby/win32
}

DEFINES += RHODES_QT_PLATFORM _XOPEN_SOURCE _DARWIN_C_SOURCE

!isEmpty(RHOSIMULATOR_BUILD) {
  DEFINES += RHODES_EMULATOR
}

unix:!macx {
  INCLUDEPATH += ../../../../platform/shared/ruby/sailfish
  INCLUDEPATH += ../../../../platform/shared/qt/sailfish/src
  INCLUDEPATH += ../../../../platform/shared/qt/sailfish
  DESTDIR = ../../../../platform/linux/bin/extensions
}


!win32 {
  QMAKE_CFLAGS_WARN_ON += -Wno-extra -Wno-unused -Wno-sign-compare -Wno-parentheses
  QMAKE_CXXFLAGS_WARN_ON += -Wno-extra -Wno-unused -Wno-sign-compare -Wno-parentheses
  QMAKE_CFLAGS_RELEASE -= -Werror=format-security
  QMAKE_CXXFLAGS_RELEASE -= -Werror=format-security
  QMAKE_CFLAGS += -fvisibility=hidden
  QMAKE_CXXFLAGS += -fvisibility=hidden 
}
win32 {
  QMAKE_CFLAGS_WARN_ON += /wd4996 /wd4100 /wd4005
  QMAKE_CXXFLAGS_WARN_ON += /wd4996 /wd4100 /wd4005
  QMAKE_CFLAGS_RELEASE += /O2
  QMAKE_CXXFLAGS_RELEASE += /O2
  SOURCES += win_serialport_impl.c
}

HEADERS += serialport.h

SOURCES += serialport.c posix_serialport_impl.c 
