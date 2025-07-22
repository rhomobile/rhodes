QT -= core

TARGET = logCapture
TEMPLATE = lib

CONFIG += staticlib warn_on

INCLUDEPATH += ../../shared\
../../shared/generated\
../../shared/generated/cpp\
../../../../../../platform/shared/common\
../../../../../../platform/shared/rubyext\
../../../../../../platform/shared/ruby/include\
../../../../../../platform/shared

macx {
  DESTDIR = ../../../../../../platform/osx/bin/extensions
  OBJECTS_DIR = ../../../../../../platform/osx/bin/extensions/logCapture
  INCLUDEPATH += ../../../../../../platform/shared/ruby/iphone
}
win32 {
  DESTDIR = ../../../../../../platform/win32/bin/extensions
  OBJECTS_DIR = ../../../../../../platform/win32/bin/extensions/logCapture
  DEFINES += WIN32 _WINDOWS _LIB _UNICODE UNICODE WIN32_LEAN_AND_MEAN
  Debug {
    DEFINES += _DEBUG DEBUG
  }
  Release {
    DEFINES += _NDEBUG NDEBUG
  }
  INCLUDEPATH += ../../../../../../platform/shared/ruby/win32
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
}
win32 {
  QMAKE_CFLAGS_WARN_ON += /wd4996 /wd4100 /wd4005
  QMAKE_CXXFLAGS_WARN_ON += /wd4996 /wd4100 /wd4005
  QMAKE_CFLAGS_RELEASE += /O2
  QMAKE_CXXFLAGS_RELEASE += /O2
}

HEADERS += \
../../shared/generated/cpp/ILogCapture.h\
../../shared/generated/cpp/LogCaptureBase.h

SOURCES += \
../../shared/generated/logcapture_ruby_api.c\
../../shared/generated/logcapture_api_init.cpp\
../../shared/generated/logcapture_js_api.cpp\
../../shared/generated/cpp/LogCaptureBase.cpp\
../../shared/generated/cpp/LogCapture_js_wrap.cpp\
../../shared/generated/cpp/LogCapture_ruby_wrap.cpp\
../../shared/LogCaptureImpl.cpp
