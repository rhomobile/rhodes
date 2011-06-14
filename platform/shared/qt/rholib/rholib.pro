QT -= core

TARGET = rholib
TEMPLATE = lib

CONFIG += staticlib

macx {
  DESTDIR = ../../../osx/build/rholib
  OBJECTS_DIR = ../../../osx/build/rholib/tmp
}

DEFINES += RHODES_EMULATOR

# CPLUSPLUSFLAGS: -fvisibility=hidden

INCLUDEPATH += ../..\
../../ruby\
../../curl/include

HEADERS += ../../common/RhoAppAdapter.h\
../../common/IRhoThreadImpl.h\
../../common/RhoThread.h\
../../unzip/unzip.h\
../../common/RhodesAppBase.h\
../../common/RhodesApp.h\
../../common/RhoConf.h\
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
../../sqlite/sqlite3.h\
../../common/PosixThreadImpl.h\
../../net/CURLNetRequest.h\
../../net/INetRequest.h\
../../net/HttpServer.h\
../../common/SplashScreen.h\
../../common/rhoparams.h\
../../net/ssl.h\
../../common/app_build_configs.h

SOURCES += ../../common/RhoTime.cpp\
../../rubyext/RhoAppAdapter.cpp\
../../common/RhoThread.cpp\
../../unzip/unzip.cpp\
../../common/RhodesAppBase.cpp\
../../common/RhodesApp.cpp\
../../common/RhoConf.cpp\
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
../../sqlite/sqlite3.c\
../../common/rhoparams.cpp\
../../rubyext/System.cpp\
../../common/PosixThreadImpl.cpp\
../../net/CURLNetRequest.cpp\
../../net/HttpServer.cpp\
../../common/SplashScreen.cpp\
../../net/ssl.cpp\
../../common/app_build_configs.c
