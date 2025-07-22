QT -= core

TARGET = <%= name.downcase %>
TEMPLATE = lib

CONFIG += staticlib warn_on

isEmpty(RHODES_ROOT) {
  RHODES_ROOT = ../../../../../../rhodes
}

greaterThan(QT_MINOR_VERSION, 6): {
    CONFIG += c++14
    DEFINES += RHODES_VERSION_2
  QT += core
}

equals(QT_MAJOR_VERSION, 5) {
    equals(QT_MINOR_VERSION, 6) {
        QT += core
        DEFINES += OS_SAILFISH OS_LINUX
        CONFIG += c++14
    }
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

unix:!macx {
    DEFINES += OS_LINUX
    DESTDIR = $$RHODES_ROOT/platform/linux/bin/extensions
    OBJECTS_DIR = $$RHODES_ROOT/platform/linux/bin/extensions/<%= name.downcase %> 
    INCLUDEPATH += $$RHODES_ROOT/platform/shared/ruby/linux

    QMAKE_CFLAGS += -fvisibility=hidden
    QMAKE_CXXFLAGS += -fvisibility=hidden
}

win32 {
  DESTDIR = $$RHODES_ROOT/platform/win32/bin/extensions
  OBJECTS_DIR = $$RHODES_ROOT/platform/win32/bin/extensions/<%= name.downcase %> 
  DEFINES += WIN32 _WINDOWS _LIB _UNICODE UNICODE WIN32_LEAN_AND_MEAN
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
  QMAKE_CFLAGS_WARN_ON += -Wno-extra -Wno-unused -Wno-sign-compare -Wno-parentheses
  QMAKE_CXXFLAGS_WARN_ON += -Wno-extra -Wno-unused -Wno-sign-compare -Wno-parentheses
  QMAKE_CFLAGS_RELEASE -= -Werror=format-security
  QMAKE_CXXFLAGS_RELEASE -= -Werror=format-security
}
win32 {
  QMAKE_CFLAGS_WARN_ON += /wd4996 /wd4100 /wd4005
  QMAKE_CXXFLAGS_WARN_ON += /wd4996 /wd4100 /wd4005
  QMAKE_CFLAGS_RELEASE += /O2
  QMAKE_CXXFLAGS_RELEASE += /O2
}

HEADERS += \
..\..\shared\generated\cpp\I<%= namecamelcase() %>.h\
..\..\shared\generated\cpp\<%= namecamelcase() %>Base.h

SOURCES += \
..\..\shared\generated\cpp\<%= namecamelcase() %>_js_wrap.cpp\
..\..\shared\generated\cpp\<%= namecamelcase() %>_ruby_wrap.cpp\
..\..\shared\generated\cpp\<%= namecamelcase() %>Base.cpp\
..\..\shared\generated\<%= namecamelcase() %>_api_init.cpp\
..\..\shared\generated\<%= namecamelcase() %>_js_api.cpp\
..\..\shared\generated\<%= namecamelcase() %>_ruby_api.c\
src\<%= namecamelcase() %>_impl.cpp
