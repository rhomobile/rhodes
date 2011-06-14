QT += core gui webkit

TARGET = RhoSimulator
TEMPLATE = app

# CONFIG += qt

macx {
  DESTDIR = ../../../osx/build/RhoSimulator
  MOC_DIR = ../../../osx/build/RhoSimulator/generated_files
  UI_DIR = ../../../osx/build/RhoSimulator/generated_files
  OBJECTS_DIR = ../../../osx/build/RhoSimulator/tmp
  RCC_DIR =  ../../../osx/build/RhoSimulator/resources
}

DEFINES += RHODES_EMULATOR

INCLUDEPATH += ../..\
../../ruby

SOURCES += ExternalWebView.cpp\
QtMainWindow.cpp\
QtNativeTabBar.cpp\
QtWebInspector.cpp

HEADERS +=ExternalWebView.h\
MainWindowCallback.h\
QtMainWindow.h\
QtNativeTabBar.h\
QtWebInspector.h

FORMS += ExternalWebView.ui\
QtMainWindow.ui\
QtWebInspector.ui