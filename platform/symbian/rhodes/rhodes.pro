# Add files and directories to ship with the application 
# by adapting the examples below.
# file1.source = myfile
QT += core gui webkit
rho.source = rho
DEPLOYMENTFOLDERS = rho

CONFIG += warn_on

symbian:TARGET.UID3 = 0xE17AE169

# Smart Installer package's UID
# This UID is from the protected range 
# and therefore the package will fail to install if self-signed
# By default qmake uses the unprotected range value if unprotected UID is defined for the application
# and 0x2002CCCF value if protected UID is given to the application
#symbian:DEPLOYMENT.installer_header = 0x2002CCCF

# Allow network access on Symbian
symbian:TARGET.CAPABILITY += NetworkServices

# If your application uses the Qt Mobility libraries, uncomment
# the following lines and add the respective components to the 
# MOBILITY variable. 
# CONFIG += mobility
# MOBILITY +=

DESTDIR = ../bin/rhodes
OBJECTS_DIR = ../bin/rhodes
MOC_DIR = ../bin/rhodes
UI_DIR = ../bin/rhodes
RCC_DIR = ../bin/rhodes

SOURCES += ../../shared/qt/rhodes/QtWebInspector.cpp \
    ../../shared/qt/rhodes/QtNativeTabBar.cpp \
    ../../shared/qt/rhodes/QtMainWindow.cpp \
    ../../shared/qt/rhodes/ExternalWebView.cpp \
    ../../shared/qt/rhodes/main.cpp \
    ../../shared/qt/rhodes/impl/WebViewImpl.cpp \
    ../../shared/qt/rhodes/impl/SystemImpl.cpp \
    ../../shared/qt/rhodes/impl/SignatureImpl.cpp \
    ../../shared/qt/rhodes/impl/RingtoneManagerImpl.cpp \
    ../../shared/qt/rhodes/impl/RhoThreadImpl.cpp \
    ../../shared/qt/rhodes/impl/RhoNativeViewManagerImpl.cpp \
    ../../shared/qt/rhodes/impl/RhoFileImpl.cpp \
    ../../shared/qt/rhodes/impl/RhodesImpl.cpp \
    ../../shared/qt/rhodes/impl/RhoClassFactoryImpl.cpp \
    ../../shared/qt/rhodes/impl/PhonebookImpl.cpp \
    ../../shared/qt/rhodes/impl/NativeToolbarImpl.cpp \
    ../../shared/qt/rhodes/impl/NativeTabbarImpl.cpp \
    ../../shared/qt/rhodes/impl/MapViewImpl.cpp \
    ../../shared/qt/rhodes/impl/MainWindowImpl.cpp \
    ../../shared/qt/rhodes/impl/GeoLocationImpl.cpp \
    ../../shared/qt/rhodes/impl/DateTimePickerImpl.cpp \
    ../../shared/qt/rhodes/impl/CameraImpl.cpp \
    ../../shared/qt/rhodes/impl/CalendarImpl.cpp \
    ../../shared/qt/rhodes/impl/BluetoothImpl.cpp \
    ../../shared/qt/rhodes/impl/AlertImpl.cpp
HEADERS += ../../shared/qt/rhodes/QtWebInspector.h \
    ../../shared/qt/rhodes/QtNativeTabBar.h \
    ../../shared/qt/rhodes/QtMainWindow.h \
    ../../shared/qt/rhodes/MainWindowCallback.h \
    ../../shared/qt/rhodes/ExternalWebView.h \
    ../../shared/qt/rhodes/impl/RhoClassFactoryImpl.h \
    ../../shared/qt/rhodes/impl/NativeToolbarImpl.h \
    ../../shared/qt/rhodes/impl/NativeTabbarImpl.h \
    ../../shared/qt/rhodes/impl/MainWindowImpl.h
FORMS += \
    ../../shared/qt/rhodes/QtWebInspector.ui \
    ../../shared/qt/rhodes/QtMainWindow.ui \
    ../../shared/qt/rhodes/ExternalWebView.ui

INCLUDEPATH += ../../shared
INCLUDEPATH += ../../shared/ruby
INCLUDEPATH += ../../shared/ruby/generated
INCLUDEPATH += ../../shared/ruby/include
INCLUDEPATH += ../../shared/ruby/symbian


#LIBS += -lrholib -lrubylib -lsyncengine -lsqlite3
LIBS += ../bin/rubylib/rubylib.lib\
        ../bin/rholib/rholib.lib\
        ../bin/sqlite3/sqlite3.lib\
        ../bin/syncengine/syncengine.lib\
        wininet.lib comsuppwd.lib ws2_32.lib\
        Crypt32.lib gdiplus.lib kernel32.lib user32.lib\
        gdi32.lib winspool.lib comdlg32.lib advapi32.lib\
        shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib

PRE_TARGETDEPS += ../bin/rubylib/rubylib.lib\
                  ../bin/rholib/rholib.lib\
                  ../bin/sqlite3/sqlite3.lib\
                  ../bin/syncengine/syncengine.lib

# Please do not modify the following two lines. Required for deployment.
include(deployment.pri)
qtcAddDeployment()
