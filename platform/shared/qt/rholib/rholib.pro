QT -= core

TARGET = rholib
TEMPLATE = lib

CONFIG += staticlib warn_on

macx {
  DESTDIR = ../../../osx/bin/rholib
  OBJECTS_DIR = ../../../osx/bin/rholib/tmp
  INCLUDEPATH += ../../curl/include
  HEADERS += ../../common/PosixThreadImpl.h\
../../net/CURLNetRequest.h\
../../net/ssl.h
  SOURCES += ../../common/PosixThreadImpl.cpp\
../../net/CURLNetRequest.cpp\
../../net/ssl.cpp
}

win32 {
  DESTDIR = ../../../win32/bin/rholib
  OBJECTS_DIR = ../../../win32/bin/rholib/tmp
  DEFINES += _NDEBUG NDEBUG WIN32 _WINDOWS _UNICODE UNICODE
  HEADERS += ../../common/RhoMath.h\
../../common/map/ESRIMapEngine.h\
../../common/map/GoogleMapEngine.h\
../../common/map/MapEngine.h\
../../rubyext/WebView.h
  SOURCES += ../../common/RhoMath.cpp\
../../common/map/ESRIMapEngine.cpp\
../../common/map/GoogleMapEngine.cpp\
../../common/map/MapEngine.cpp
}

DEFINES += RHODES_EMULATOR

!win32 {
  QMAKE_CFLAGS_WARN_ON += -Wno-extra -Wno-unused -Wno-sign-compare -Wno-format -Wno-parentheses
  QMAKE_CXXFLAGS_WARN_ON += -Wno-extra -Wno-unused -Wno-sign-compare -Wno-format -Wno-parentheses
  # QMAKE_CFLAGS += -fvisibility=hidden
  # QMAKE_CXXFLAGS += -fvisibility=hidden
}
win32 {
  QMAKE_CFLAGS_WARN_ON += /wd4189 /wd4018 /wd4189
  QMAKE_CXXFLAGS_WARN_ON += /wd4189 /wd4018 /wd4189
  QMAKE_CFLAGS_RELEASE += /O2
  QMAKE_CXXFLAGS_RELEASE += /O2
}

INCLUDEPATH += ../..\
../../ruby

HEADERS += ../../common/RhoAppAdapter.h\
../../common/IRhoThreadImpl.h\
../../common/RhoThread.h\
../../unzip/unzip.h\
../../common/RhodesAppBase.h\
../../common/RhodesApp.h\
../../common/RhoConf.h\
../../common/RhoSimConf.h\
../../statistic/RhoProfiler.h\
../../common/RhoDefs.h\
../../common/RhoFatalError.h\
../../common/RhoFile.h\
../../common/RhoFilePath.h\
../../common/RhoMutexLock.h\
../../common/RhoPort.h\
../../common/RhoStd.h\
../../common/RhoSystem.h\
../../common/RhoTime.h\
../../common/StringConverter.h\
../../logging/RhoLog.h\
../../logging/RhoLogCat.h\
../../logging/RhoLogConf.h\
../../logging/RhoLogSink.h\
../../common/Tokenizer.h\
../../rubyext/GeoLocation.h\
../../net/AsyncHttp.h\
../../common/AutoPointer.h\
../../common/InputStream.h\
../../common/irhoclassfactory.h\
../../common/ThreadQueue.h\
../../net/URI.h\
../../common/AppMenu.h\
../../net/INetRequest.h\
../../net/HttpServer.h\
../../common/SplashScreen.h\
../../common/rhoparams.h\
../../common/app_build_configs.h

SOURCES += ../../common/RhoTime.cpp\
../../rubyext/RhoAppAdapter.cpp\
../../common/RhoThread.cpp\
../../unzip/unzip.cpp\
../../common/RhodesAppBase.cpp\
../../common/RhodesApp.cpp\
../../common/RhoConf.cpp\
../../common/RhoSimConf.cpp\
../../statistic/RhoProfiler.cpp\
../../common/RhoFile.cpp\
../../logging/RhoLog.cpp\
../../logging/RhoLogConf.cpp\
../../logging/RhoLogSink.cpp\
../../logging/RhoPlainLog.cpp\
../../common/Tokenizer.cpp\
../../rubyext/GeoLocation.cpp\
../../net/AsyncHttp.cpp\
../../common/ThreadQueue.cpp\
../../net/INetRequest.cpp\
../../net/URI.cpp\
../../common/AppMenu.cpp\
../../common/rhoparams.cpp\
../../rubyext/System.cpp\
../../net/HttpServer.cpp\
../../common/SplashScreen.cpp\
../../common/app_build_configs.c
