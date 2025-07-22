QT += core bluetooth

equals(QT_MAJOR_VERSION, 5) {
    lessThan(QT_MINOR_VERSION, 6): {
        DEFINES += RHODES_VERSION_1
    }
    equals(QT_MINOR_VERSION, 6): {
        QT += dbus
        DEFINES += OS_SAILFISH OS_LINUX
        CONFIG += auroraapp c++14 auroraapp_i18n
    }
    greaterThan(QT_MINOR_VERSION, 7): {
        CONFIG += c++14
        DEFINES += CPP_ELEVEN RHODES_VERSION_2
    }
}

TARGET = Bluetooth
TEMPLATE = lib
CONFIG += staticlib warn_on

isEmpty(RHODES_ROOT) {
   RHODES_ROOT = ../../../../../..
}

INCLUDEPATH += \
$$RHODES_ROOT/lib/commonAPI/coreapi/ext/shared\
$$RHODES_ROOT/platform/shared/common\
$$RHODES_ROOT/platform/shared/rubyext\
$$RHODES_ROOT/platform/shared/ruby/include\
$$RHODES_ROOT/platform/shared\
$$RHODES_ROOT/platform/shared/qt/rhodes\
../../shared

macx {
  DESTDIR = $$RHODES_ROOT/platform/osx/bin/extensions
  OBJECTS_DIR = $$RHODES_ROOT/platform/osx/bin/extensions/bluetooth
  INCLUDEPATH += $$RHODES_ROOT/platform/shared/ruby/iphone
}
win32 {
  DESTDIR = $$RHODES_ROOT/platform/win32/bin/extensions
  OBJECTS_DIR = $$RHODES_ROOT/platform/win32/bin/extensions/bluetooth 
  DEFINES += WIN32 _WINDOWS _LIB _UNICODE UNICODE WIN32_LEAN_AND_MEAN OS_WINDOWS_DESKTOP
  debug {
    DEFINES += _DEBUG DEBUG
  }
  release {
    DEFINES += _NDEBUG NDEBUG
  }
  INCLUDEPATH += $$RHODES_ROOT/platform/shared/ruby/win32
}

unix:!macx {
  INCLUDEPATH += $$RHODES_ROOT/platform/shared/ruby/sailfish
  INCLUDEPATH += $$RHODES_ROOT/platform/shared/qt/sailfish/src
  INCLUDEPATH += $$RHODES_ROOT/platform/shared/qt/sailfish

  contains(DEFINES, OS_LINUX)  {
    DESTDIR = $$RHODES_ROOT/platform/linux/bin/extensions
    OBJECTS_DIR = $$RHODES_ROOT/platform/linux/bin/extensions/bluetooth
  }
  contains(DEFINES, OS_SAILFISH)  {

  }

QMAKE_CFLAGS += -fvisibility=hidden
QMAKE_CXXFLAGS += -fvisibility=hidden

}

#INCLUDEPATH += $$RHODES_ROOT/platform/shared/qt/rhodes
#INCLUDEPATH += $$RHODES_ROOT/platform/shared
#INCLUDEPATH += $$RHODES_ROOT/platform/shared/ruby

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
  QMAKE_CFLAGS_RELEASE += /O2
  QMAKE_CXXFLAGS_RELEASE += /O2

  QMAKE_CXXFLAGS_RELEASE += -MP9
  QMAKE_CXXFLAGS_DEBUG += -MP9
}

HEADERS += \
../../shared/generated/cpp/IBluetoothConnection.h\
../../shared/generated/cpp/BluetoothConnectionBase.h\
    src/bluetooth/bluetoothclient.h \
    src/bluetooth/bluetoothdevicelabel.h \
    src/bluetooth/bluetoothhelper.h \
    src/bluetooth/bluetoothsender.h \
    src/bluetooth/bluetoothserver.h \
    src/bluetooth/bluetooththread.h


SOURCES += \
../../shared/generated/cpp/BluetoothConnection_js_wrap.cpp\
../../shared/generated/cpp/BluetoothConnection_ruby_wrap.cpp\
../../shared/generated/cpp/BluetoothConnectionBase.cpp\
../../shared/generated/BluetoothConnection_api_init.cpp\
../../shared/generated/BluetoothConnection_js_api.cpp\
../../shared/generated/BluetoothConnection_ruby_api.c\
src/BluetoothConnection_impl.cpp
