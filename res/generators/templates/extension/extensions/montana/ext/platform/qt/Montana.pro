QT -= core

TARGET = <%= name.downcase %>
TEMPLATE = lib

CONFIG += staticlib warn_on

isEmpty(RHODES_ROOT) {
  RHODES_ROOT = ../../../../../../rhodes
}

INCLUDEPATH += \
$$RHODES_ROOT/lib/commonAPI/coreapi/ext/shared\
$$RHODES_ROOT/platform/shared/common\
$$RHODES_ROOT/platform/shared/rubyext\
$$RHODES_ROOT/platform/shared/ruby/include\
$$RHODES_ROOT/platform/shared\
../../shared

macx {
  DESTDIR = $$RHODES_ROOT/platform/osx/bin/extensions
  OBJECTS_DIR = $$RHODES_ROOT/platform/osx/bin/extensions/<%= name.downcase %>
  INCLUDEPATH += $$RHODES_ROOT/platform/shared/ruby/iphone
}
win32 {
  DESTDIR = $$RHODES_ROOT/platform/win32/bin/extensions
  OBJECTS_DIR = $$RHODES_ROOT/platform/win32/bin/extensions/<%= name.downcase %>
  DEFINES += WIN32 _WINDOWS _LIB _UNICODE UNICODE
  debug {
    DEFINES += _DEBUG DEBUG
  }
  release {
    DEFINES += _NDEBUG NDEBUG
  }
  INCLUDEPATH += $$RHODES_ROOT/platform/shared/ruby/win32
}

DEFINES += RHODES_QT_PLATFORM _XOPEN_SOURCE _DARWIN_C_SOURCE

!isEmpty(RHOSIMULATOR_BUILD) {
  DEFINES += RHODES_EMULATOR
}

!win32 {
  QMAKE_CFLAGS_WARN_ON += -Wno-extra -Wno-unused -Wno-sign-compare -Wno-format -Wno-parentheses
  QMAKE_CXXFLAGS_WARN_ON += -Wno-extra -Wno-unused -Wno-sign-compare -Wno-format -Wno-parentheses
}
win32 {
  QMAKE_CFLAGS_WARN_ON += /wd4996 /wd4100 /wd4005
  QMAKE_CXXFLAGS_WARN_ON += /wd4996 /wd4100 /wd4005
  QMAKE_CFLAGS_RELEASE += /O2
  QMAKE_CXXFLAGS_RELEASE += /O2
}

HEADERS += \
..\..\shared\generated\cpp\I<%= name.camel_case() %>.h\
..\..\shared\generated\cpp\<%= name.camel_case() %>Base.h

SOURCES += \
..\..\shared\generated\cpp\<%= name.camel_case() %>_js_wrap.cpp\
..\..\shared\generated\cpp\<%= name.camel_case() %>_ruby_wrap.cpp\
..\..\shared\generated\cpp\<%= name.camel_case() %>Base.cpp\
..\..\shared\generated\<%= name.downcase() %>_api_init.cpp\
..\..\shared\generated\<%= name.downcase() %>_js_api.cpp\
..\..\shared\generated\<%= name.downcase() %>_ruby_api.c\
src\<%= name.camel_case() %>_impl.cpp
