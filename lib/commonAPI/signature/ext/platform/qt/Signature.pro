QT += core gui widgets multimedia network

    lessThan(QT_VERSION, 5.6.0): {
        QT += webkit widgets webkitwidgets multimediawidgets
        DEFINES += RHODES_VERSION_1
    }

    equals(QT_MAJOR_VERSION, 5) {
        equals(QT_MINOR_VERSION, 6) {
            QT += widgets
            DEFINES += OS_SAILFISH OS_LINUX
            CONFIG += sailfishapp c++14 sailfishapp_i18n
        }
    }

    greaterThan(QT_VERSION, 5.7.0): {
        QT += webengine webenginecore webenginewidgets multimediawidgets
        CONFIG += c++14
        DEFINES += CPP_ELEVEN RHODES_VERSION_2
    }


TARGET = Signature
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
  DESTDIR = $$RHODES_ROOT/platform/osx/bin/extensions
  OBJECTS_DIR = $$RHODES_ROOT/platform/osx/bin/extensions/signature
  INCLUDEPATH += $$RHODES_ROOT/platform/shared/ruby/iphone
}
win32 {
  DESTDIR = $$RHODES_ROOT/platform/win32/bin/extensions
  OBJECTS_DIR = $$RHODES_ROOT/platform/win32/bin/extensions/signature
  DEFINES += WIN32 _WINDOWS _LIB _UNICODE UNICODE
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
    OBJECTS_DIR = $$RHODES_ROOT/platform/linux/bin/extensions/signature
  }
  contains(DEFINES, OS_SAILFISH)  {
    HEADERS += src/signatureqmlmodel.h
    SOURCES += src/signatureqmlmodel.cpp
  }
}
DEFINES += RHODES_QT_PLATFORM _XOPEN_SOURCE _DARWIN_C_SOURCE

!isEmpty(RHOSIMULATOR_BUILD) {
  DEFINES += RHODES_EMULATOR
}

!win32 {
  QMAKE_CFLAGS_WARN_ON += -Wno-extra -Wno-unused -Wno-sign-compare -Wno-format -Wno-parentheses
  QMAKE_CXXFLAGS_WARN_ON += -Wno-extra -Wno-unused -Wno-sign-compare -Wno-format -Wno-parentheses
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
..\..\shared\generated\cpp\ISignature.h\
..\..\shared\generated\cpp\SignatureBase.h

SOURCES += \
..\..\shared\generated\cpp\Signature_js_wrap.cpp\
..\..\shared\generated\cpp\Signature_ruby_wrap.cpp\
..\..\shared\generated\cpp\SignatureBase.cpp\
..\..\shared\generated\Signature_api_init.cpp\
..\..\shared\generated\Signature_js_api.cpp\
..\..\shared\generated\Signature_ruby_api.c\
src\Signature_impl.cpp
