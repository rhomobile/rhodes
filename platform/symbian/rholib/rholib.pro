#-------------------------------------------------
#
# Project created by QtCreator 2011-06-15T16:44:50
#
#-------------------------------------------------

QT       -= gui

TARGET = rholib
TEMPLATE = lib
CONFIG += staticlib warn_on
INCLUDEPATH += ../../shared/

DEFINES -= UNICODE
DEFINES -= _UNICODE

SOURCES += \
    ../../shared/common/Tokenizer.cpp \
    ../../shared/common/ThreadQueue.cpp \
    ../../shared/common/SplashScreen.cpp \
    ../../shared/common/RhoTime.cpp \
    ../../shared/common/RhoThread.cpp \
    ../../shared/common/RhoSimConf.cpp \
    ../../shared/common/rhoparams.cpp \
    ../../shared/common/RhoMath.cpp \
    ../../shared/common/RhoFile.cpp \
    ../../shared/common/RhodesAppBase.cpp \
    ../../shared/common/RhodesApp.cpp \
    ../../shared/common/RhoConf.cpp \
    ../../shared/common/AppMenu.cpp \
    ../../shared/logging/RhoPlainLog.cpp \
    ../../shared/logging/RhoLogSink.cpp \
    ../../shared/logging/RhoLogConf.cpp \
    ../../shared/logging/RhoLog.cpp \
    ../../shared/common/map/BaseMapEngine.cpp \
    ../../shared/common/map/MapEngine.cpp \
    ../../shared/common/map/OSMMapEngine.cpp \
    ../../shared/common/map/GoogleMapEngine.cpp \
    ../../shared/common/map/GeocodingMapEngine.cpp \
    ../../shared/common/map/ESRIMapEngine.cpp \
    ../../shared/net/URI.cpp \
    ../../shared/net/RawSocket.cpp \
    ../../shared/net/INetRequest.cpp \
    ../../shared/net/HttpServer.cpp \
    ../../shared/net/CompatWince.cpp \
    ../../shared/net/AsyncHttp.cpp \
    ../../shared/rubyext/System.cpp \
    ../../shared/rubyext/RhoAppAdapter.cpp \
    ../../shared/rubyext/GeoLocation.cpp \
    ../../shared/statistic/RhoProfiler.cpp \
    ../../shared/unzip/unzip.cpp

HEADERS += \
    ../../shared/common/Tokenizer.h \
    ../../shared/common/ThreadQueue.h \
    ../../shared/common/StringConverter.h \
    ../../shared/common/SplashScreen.h \
    ../../shared/common/RhoTime.h \
    ../../shared/common/RhoThread.h \
    ../../shared/common/RhoSystem.h \
    ../../shared/common/RhoStd.h \
    ../../shared/common/RhoSimConf.h \
    ../../shared/common/RhoPort.h \
    ../../shared/common/rhoparams.h \
    ../../shared/common/RhoMutexLock.h \
    ../../shared/common/RhoMath.h \
    ../../shared/common/RhoFilePath.h \
    ../../shared/common/RhoFile.h \
    ../../shared/common/RhoFatalError.h \
    ../../shared/common/RhodesAppBase.h \
    ../../shared/common/RhodesApp.h \
    ../../shared/common/RhoDefs.h \
    ../../shared/common/RhoConf.h \
    ../../shared/common/IRhoThreadImpl.h \
    ../../shared/common/IRhoClassFactory.h \
    ../../shared/common/InputStream.h \
    ../../shared/common/AutoPointer.h \
    ../../shared/common/AppMenu.h \
    ../../shared/common/app_build_configs.h \
    ../../shared/logging/RhoLogSink.h \
    ../../shared/logging/RhoLogConf.h \
    ../../shared/logging/RhoLogCat.h \
    ../../shared/logging/RhoLog.h \
    ../../shared/logging/RhoATLTrace.h \
    ../../shared/common/map/BaseMapEngine.h \
    ../../shared/common/map/MapEngine.h \
    ../../shared/common/map/OSMMapEngine.h \
    ../../shared/common/map/GoogleMapEngine.h \
    ../../shared/common/map/GeocodingMapEngine.h \
    ../../shared/common/map/ESRIMapEngine.h \
    ../../shared/net/URI.h \
    ../../shared/net/RawSocket.h \
    ../../shared/net/INetRequest.h \
    ../../shared/net/HttpServer.h \
    ../../shared/net/CompatWince.h \
    ../../shared/net/AsyncHttp.h \
    ../../shared/rubyext/WebView.h \
    ../../shared/rubyext/GeoLocation.h \
    ../../shared/statistic/RhoProfiler.h \
    ../../shared/unzip/unzip.h
unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

win32 {
#DESTDIR = ../../rhodes-symbian-emulator-build
}

symbian {
INCLUDEPATH += ../../shared/curl/include
HEADERS +=  ../../shared/net/CURLNetRequest.h
SOURCES +=  ../../shared/net/CURLNetRequest.cpp
}
