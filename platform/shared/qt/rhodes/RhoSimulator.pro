QT += core gui webkit

TARGET = RhoSimulator
TEMPLATE = app

CONFIG += warn_on

macx {
  DESTDIR = ../../../osx/build/RhoSimulator
  MOC_DIR = ../../../osx/build/RhoSimulator/generated_files
  UI_DIR = ../../../osx/build/RhoSimulator/generated_files
  OBJECTS_DIR = ../../../osx/build/RhoSimulator/tmp
  RCC_DIR =  ../../../osx/build/RhoSimulator/resources
  LIBS += -lcurl
  # LIBS += -L../../../osx/build/curl -lcurl
  LIBS += -L../../../osx/build/rubylib -lrubylib
  LIBS += -L../../../osx/build/rholib -lrholib
  LIBS += -L../../../osx/build/sqlite3 -lsqlite3
  LIBS += -L../../../osx/build/syncengine -lsyncengine
}

DEFINES += RHODES_EMULATOR

QMAKE_CFLAGS_WARN_ON += -Wno-extra -Wno-unused -Wno-sign-compare -Wno-format -Wno-parentheses
QMAKE_CXXFLAGS_WARN_ON += -Wno-extra -Wno-unused -Wno-sign-compare -Wno-format -Wno-parentheses

INCLUDEPATH += ../..\
../../ruby\
../../ruby/include

HEADERS +=ExternalWebView.h\
MainWindowCallback.h\
QtMainWindow.h\
QtNativeTabBar.h\
QtWebInspector.h\
impl/RhoClassFactoryImpl.h\
impl/MainWindowImpl.h

SOURCES += main.cpp\
ExternalWebView.cpp\
QtMainWindow.cpp\
QtNativeTabBar.cpp\
QtWebInspector.cpp\
impl/AlertImpl.cpp\
impl/BluetoothImpl.cpp\
impl/CalendarImpl.cpp\
impl/CameraImpl.cpp\
impl/DateTimePickerImpl.cpp\
impl/GeoLocationImpl.cpp\
impl/MapViewImpl.cpp\
impl/NativeToolbarImpl.cpp\
impl/PhonebookImpl.cpp\
impl/RhoClassFactoryImpl.cpp\
impl/RhodesImpl.cpp\
impl/RhoFileImpl.cpp\
impl/RhoNativeViewManagerImpl.cpp\
impl/RhoThreadImpl.cpp\
impl/RingtoneManagerImpl.cpp\
impl/SignatureImpl.cpp\
impl/SystemImpl.cpp\
impl/WebViewImpl.cpp\
impl/MainWindowImpl.cpp

FORMS += ExternalWebView.ui\
QtMainWindow.ui\
QtWebInspector.ui
