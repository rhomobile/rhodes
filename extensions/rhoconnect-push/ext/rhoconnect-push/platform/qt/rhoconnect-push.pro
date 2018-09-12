QT -= core

TARGET = rhoconnect-push
TEMPLATE = lib

CONFIG += staticlib warn_on

INCLUDEPATH += ../wm/src\
../../shared\
../../shared/generated\
../../shared/generated/cpp\
../../../shared\
$$(RHO_ROOT)/platform/shared/common\
$$(RHO_ROOT)/platform/shared/rubyext\
$$(RHO_ROOT)/platform/shared/ruby/include\
$$(RHO_ROOT)/platform/shared/ruby/win32\
$$(RHO_ROOT)/lib/commonapi/coreapi/ext/shared\
$$(RHO_ROOT)/platform/shared

win32 {
  DEFINES += WIN32 _WINDOWS _LIB _UNICODE UNICODE WIN32_LEAN_AND_MEAN
  debug {
    DEFINES += _DEBUG DEBUG
	DESTDIR = $$(RHO_ROOT)/platform/win32/bin/extensions/rhoconnect-push/debug
    OBJECTS_DIR = $$(RHO_ROOT)/platform/win32/bin/extensions/rhoconnect-push/debug
  }
  release {
    DEFINES += _NDEBUG NDEBUG
	DESTDIR = $$(RHO_ROOT)/platform/win32/bin/extensions/rhoconnect-push/release
    OBJECTS_DIR = $$(RHO_ROOT)/platform/win32/bin/extensions/rhoconnect-push/release
  }
}

DEFINES += RHODES_QT_PLATFORM

!isEmpty(RHOSIMULATOR_BUILD) {
  DEFINES += RHODES_EMULATOR
}

win32 {
  QMAKE_CFLAGS_WARN_ON += /wd4996 /wd4100 /wd4005
  QMAKE_CXXFLAGS_WARN_ON += /wd4996 /wd4100 /wd4005
  QMAKE_CFLAGS_RELEASE += /O2
  QMAKE_CXXFLAGS_RELEASE += /O2
}

HEADERS += \
../wm/src/PushClient.h\
../wm/src/P2PMessageQueue.h\
../wm/src/AnsSync.h

SOURCES += \
../wm/src/PushClient.cpp\
../wm/src/push_wm.cpp\
../wm/src/MSMQ.cpp\
../wm/src/AnsSync.cpp\
../../shared/src/anspushclient.cpp\
../../shared/ruby/anspush_wrap.c\
../../../shared/generated/rhoconnectpush_api_init.cpp\
../../../shared/generated/rhoconnectpush_js_api.cpp\
../../../shared/generated/rhoconnectpush_ruby_api.c\
../../../shared/generated/cpp/RhoconnectPush_js_wrap.cpp\
../../../shared/generated/cpp/RhoconnectPush_ruby_wrap.cpp\
../../../shared/generated/cpp/RhoconnectPushBase.cpp\
../../../shared/generated/stub/rhoconnectpush_stub_impl.cpp
