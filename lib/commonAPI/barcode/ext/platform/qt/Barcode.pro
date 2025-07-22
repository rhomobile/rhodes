QT += core gui multimedia network

equals(QT_MAJOR_VERSION, 5) {
    lessThan(QT_MINOR_VERSION, 6): {
        QT += webkit widgets webkitwidgets multimediawidgets widgets
        DEFINES += RHODES_VERSION_1
    }
    equals(QT_MINOR_VERSION, 6): {
        QT += webkit quick
        DEFINES += OS_SAILFISH OS_LINUX
        CONFIG += auroraapp c++14 auroraapp_i18n
    }
    greaterThan(QT_MINOR_VERSION, 7): {
        QT += webengine webenginecore webenginewidgets multimediawidgets widgets
        CONFIG += c++14
        DEFINES += CPP_ELEVEN RHODES_VERSION_2
    }
}


include(src/qzxing/QZXing.pri)

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
../../shared


macx {
  TARGET = Barcode
  DESTDIR = $$RHODES_ROOT/platform/osx/bin/extensions
  OBJECTS_DIR = $$RHODES_ROOT/platform/osx/bin/extensions/barcode
  INCLUDEPATH += $$RHODES_ROOT/platform/shared/ruby/iphone
}
win32 {
  TARGET = Barcode
  DESTDIR = $$RHODES_ROOT/platform/win32/bin/extensions
  OBJECTS_DIR = $$RHODES_ROOT/platform/win32/bin/extensions/barcode 
  DEFINES += WIN32 _WINDOWS _LIB _UNICODE UNICODE WIN32_LEAN_AND_MEAN _WINSOCKAPI_
  debug {
    DEFINES += _DEBUG DEBUG
  }
  release {
    DEFINES += _NDEBUG NDEBUG
    #DEFINES += _ITERATOR_DEBUG_LEVEL=2
  }
  INCLUDEPATH += $$RHODES_ROOT/platform/shared/ruby/win32

}


unix:!macx {
TARGET = barcode
DEFINES += OS_LINUX
DESTDIR = $$RHODES_ROOT/platform/linux/bin/extensions
OBJECTS_DIR = $$RHODES_ROOT/platform/linux/bin/extensions/barcode
INCLUDEPATH += $$RHODES_ROOT/platform/shared/ruby/linux
INCLUDEPATH += $$RHODES_ROOT/platform/shared/qt/rhodes

QMAKE_CFLAGS += -fvisibility=hidden
QMAKE_CXXFLAGS += -fvisibility=hidden

  contains(DEFINES, OS_SAILFISH)  {
    INCLUDEPATH += $$RHODES_ROOT/platform/shared/ruby/sailfish
    INCLUDEPATH += $$RHODES_ROOT/platform/shared/qt/sailfish/src
    INCLUDEPATH += $$RHODES_ROOT/platform/shared/qt/sailfish
    HEADERS += src/barcodeqmlmodel.h
    SOURCES += src/barcodeqmlmodel.cpp
  }
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
  QMAKE_CFLAGS_RELEASE += /O2
  QMAKE_CXXFLAGS_RELEASE += /O2

  QMAKE_CXXFLAGS_RELEASE += -MP9
  QMAKE_CXXFLAGS_DEBUG += -MP9
}


HEADERS += \
    ..\..\shared\generated\cpp\IBarcode.h\
    ..\..\shared\generated\cpp\BarcodeBase.h \
    src/BarcodeController.h \
    src/BarcodeDialogBuilder.h \
    src/BarcodeDialogView.h\
    src/decoderthread.h

SOURCES += \
    ..\..\shared\generated\cpp\Barcode_js_wrap.cpp\
    ..\..\shared\generated\cpp\Barcode_ruby_wrap.cpp\
    ..\..\shared\generated\cpp\BarcodeBase.cpp\
    ..\..\shared\generated\Barcode_api_init.cpp\
    ..\..\shared\generated\Barcode_js_api.cpp\
    ..\..\shared\generated\Barcode_ruby_api.c\
    src\Barcode_impl.cpp \
    src/BarcodeController.cpp \
    src/BarcodeDialogView.cpp \
    src/decoderthread.cpp


RESOURCES += $$RHODES_ROOT/lib/commonAPI/barcode/ext/platform/qt/resources/barcode.qrc
