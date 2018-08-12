QT += core gui network
message(Qt version: $$[QT_VERSION])
isEqual(QT_MAJOR_VERSION, 5):{
    lessThan(QT_MINOR_VERSION, 6): {
        QT += webkit widgets webkitwidgets multimedia multimediawidgets
        message(Deprecated webkit enabled)
        DEFINES += RHODES_VERSION_1
        INCLUDEPATH += oldVersion
    }

    equals(QT_MAJOR_VERSION, 5) {
        equals(QT_MINOR_VERSION, 6) {
            DEFINES += ENABLE_Q_WEB_ENGINE

            !contains(DEFINES, ENABLE_Q_WEB_ENGINE)  {
                QT += webkit
                message(Deprecated sailfish webkit enabled)
            }
            contains(DEFINES, ENABLE_Q_WEB_ENGINE)  {
                QT += webengine
            }

            QT += quick multimedia dbus bluetooth
            DEFINES += OS_SAILFISH OS_LINUX
            CONFIG += sailfishapp c++14 sailfishapp_i18n qmlcache
        }
    }


    greaterThan(QT_MINOR_VERSION, 6): {
        QT += webengine webenginecore webenginewidgets multimedia multimediawidgets
        message(Webengine enabled)
        CONFIG += c++14
        DEFINES += RHODES_VERSION_2
        INCLUDEPATH += newVersion
    }
}

#DEFINES += RHODES_EMULATOR

!contains(DEFINES, OS_SAILFISH)  {
    TARGET = RhoSimulator
    TEMPLATE = app
}
contains(DEFINES, OS_SAILFISH)  {
    TARGET = harbour-sailfishrhodes
    TEMPLATE = app
}

CONFIG += warn_on

INCLUDEPATH += ../..\
../../ruby\
../../ruby/include

macx {
  greaterThan(QT_MINOR_VERSION, 6): {
      DEFINES += RHODES_MAC_BUILD
  }
  ICON = resources/rho.icns
  QMAKE_INFO_PLIST = resources/Info.plist
  DESTDIR = ../../../osx/bin/RhoSimulator
  MOC_DIR = ../../../osx/bin/RhoSimulator/generated_files
  UI_DIR = ../../../osx/bin/RhoSimulator/generated_files
  OBJECTS_DIR = ../../../osx/bin/RhoSimulator/tmp
  RCC_DIR =  ../../../osx/bin/RhoSimulator/resources
  HEADERS += impl/SSLImpl.h
  SOURCES += impl/SSLImpl.cpp\
../../../../lib/commonAPI/coreapi/ext/platform/qt/src/CSystemImpl.cpp
  LIBS += -lz -lldap
  LIBS += -L../../../osx/bin/curl -lcurl
  LIBS += -L../../../osx/bin/rubylib -lrubylib
  LIBS += -L../../../osx/bin/rholib -lrholib
  LIBS += -L../../../osx/bin/sqlite3 -lsqlite3
  LIBS += -L../../../osx/bin/syncengine -lsyncengine
  LIBS += -L../../../../../openssl/libopenssl_macosx -lcrypto
  LIBS += -L../../../../../openssl/libopenssl_macosx -lssl
  LIBS += -framework CoreFoundation
  PRE_TARGETDEPS += ../../../osx/bin/rubylib/librubylib.a\
../../../osx/bin/rholib/librholib.a\
../../../osx/bin/sqlite3/libsqlite3.a\
../../../osx/bin/syncengine/libsyncengine.a
  exists("../../../osx/bin/extensions/extensions.pri") {
    include("../../../osx/bin/extensions/extensions.pri")
  }
}

win32 {
  greaterThan(QT_MINOR_VERSION, 6): {
      DEFINES += CPP_ELEVEN
  }
  CONFIG += embed_manifest_exe
  CONFIG += no_batch
  DESTDIR = ../../../win32/bin/RhoSimulator
  MOC_DIR = ../../../win32/bin/RhoSimulator/generated_files
  UI_DIR = ../../../win32/bin/RhoSimulator/generated_files
  OBJECTS_DIR = ../../../win32/bin/RhoSimulator/tmp
  RCC_DIR =  ../../../win32/bin/RhoSimulator/resources
  RC_FILE = resources/simulator.rc
  HEADERS += ../../../wm/rhodes/rho/net/NetRequestImpl.h\
    ../../../wm/rhodes/stdafx.h\
    newVersion/AppRunningFlag.h
  SOURCES += ../../../wm/rhodes/rho/net/NetRequestImpl.cpp\
    ../../../wm/rhodes/rho/rubyext/SystemImpl.cpp\
    ../../../wm/rhodes/rho/common/ExtManager.cpp
  RESOURCES += resources/simulator.qrc
  INCLUDEPATH += ../../../wm/rhodes\
    ../../wtl80/include
  DEFINES -= _UNICODE
  DEFINES += WIN32 _WINDOWS UNICODE QT_LARGEFILE_SUPPORT QT_CORE_LIB QT_GUI_LIB QT_NETWORK_LIB QT_WEBKIT_LIB _CRT_SECURE_NO_WARNINGS _CRT_NON_CONFORMING_SWPRINTFS WIN32_LEAN_AND_MEAN

  Debug {
    DEFINES += _DEBUG DEBUG
    LIBS += comsuppwd.lib
  }
  Release {
    DEFINES += _NDEBUG NDEBUG QT_NO_DEBUG
    LIBS += comsuppw.lib
  }
  LIBS += ../../../win32/bin/rubylib/rubylib.lib\
../../../win32/bin/rholib/rholib.lib\
../../../win32/bin/sqlite3/sqlite3.lib\
../../../win32/bin/syncengine/syncengine.lib\
oldnames.lib wininet.lib Iphlpapi.lib Dbghelp.lib ws2_32.lib Crypt32.lib gdiplus.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib
  PRE_TARGETDEPS += ../../../win32/bin/rubylib/rubylib.lib\
../../../win32/bin/rholib/rholib.lib\
../../../win32/bin/sqlite3/sqlite3.lib\
../../../win32/bin/syncengine/syncengine.lib
  exists("../../../win32/bin/extensions/extensions.pri") {
    include("../../../win32/bin/extensions/extensions.pri")
  }
}

unix:!macx {
!contains(DEFINES, OS_SAILFISH) {
  DESTDIR = $$PWD/../../../linux/bin/RhoSimulator
}
contains(DEFINES, OS_SAILFISH) {
  #DESTDIR = $$PWD/../Build_Sailfish_Application
}
  MOC_DIR = $$PWD/../../../linux/bin/RhoSimulator/generated_files
  UI_DIR = $$PWD/../../../linux/bin/RhoSimulator/generated_files
  OBJECTS_DIR = $$PWD/../../../linux/bin/RhoSimulator/tmp
  RCC_DIR =  $$PWD/../../../linux/bin/RhoSimulator/resources
  HEADERS += $$PWD/../../net/linux/SSLImpl.h
  SOURCES += $$PWD/../../net/linux/SSLImpl.cpp\
  ../../../../lib/commonAPI/coreapi/ext/platform/qt/src/CSystemImpl.cpp
  INCLUDEPATH += ../../curl/include\
  #../../ruby/linux\
  LIBS += -lpthread
  LIBS += -ldl -lz
  #LIBS += -L../../../linux/bin/extensions -lcoreapi
  LIBS += -L../../../linux/bin/sqlite3 -lsqlite3
  #LIBS += -L../../../linux/bin/curl -lcurl
  #LIBS += -L../../../linux/bin/rubylib -lrubylib
  #LIBS += -L../../../linux/bin/rholib -lrholib
  #LIBS += -L../../../linux/bin/syncengine -lsyncengine
  #PRE_TARGETDEPS += \
  #../../../linux/bin/rubylib/librubylib.a\
  #../../../linux/bin/curl/libcurl.a\
  #../../../linux/bin/rholib/librholib.a\
  #../../../linux/bin/sqlite3/libsqlite3.a\
  #../../../linux/bin/extensions/libcoreapi.a\
  #../../../linux/bin/syncengine/libsyncengine.a
  DEFINES += QT_LARGEFILE_SUPPORT QT_CORE_LIB QT_GUI_LIB QT_NETWORK_LIB QT_WEBKIT_LIB OS_LINUX
}

DEFINES += RHODES_QT_PLATFORM

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
    QMAKE_CXXFLAGS_RELEASE += /MP9 /O2
    QMAKE_CXXFLAGS_DEBUG += /MP9 /O2

    QMAKE_CFLAGS_RELEASE += /O2 /MD
    QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO += /O2
    QMAKE_CFLAGS_DEBUG += /Zi /MDd
}

HEADERS += impl/RhoClassFactoryImpl.h\
impl/MainWindowImpl.h\
impl/NativeToolbarImpl.h\
impl/NativeTabbarImpl.h\
../../rubyext/NativeToolbarExt.h\
impl/DateTimePickerImpl.h\
impl/AlertImpl.h\
impl/RhoThreadImpl.h\
iexecutable.h\
MainWindowCallback.h\
QtCustomStyle.h\
mainwindowinterface.h \
guithreadfunchelper.h \
impl/notificationsound.h

SOURCES += impl/AlertImpl.cpp\
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
impl/QtSystemImpl.cpp\
impl/WebViewImpl.cpp\
impl/MainWindowImpl.cpp\
impl/NativeTabbarImpl.cpp\
../../../../lib/commonAPI/coreapi/ext/platform/qt/src/CWebViewImpl.cpp \
    impl/notificationsound.cpp \
    ../../../../lib/commonAPI/coreapi/ext/shared/SystemImplBase.cpp
#TODO: make this like normal developer do

contains(DEFINES, OS_SAILFISH)  {
SOURCES += \
    $$PWD/../sailfish/src/main.cpp \
    $$PWD/../sailfish/src/QtMainWindow.cpp \
    ../../net/ssl.cpp \
    ../../unzip/zip.cpp \
    $$PWD/../sailfish/src/RhoNativeApiCall.cpp

HEADERS += $$PWD/../sailfish/src/custommenuitem.h \
    $$PWD/../sailfish/src/customtoolbaritem.h \
    $$PWD/../sailfish/src/customwebviewtab.h \
    $$PWD/../sailfish/src/QtLogView.h \
    $$PWD/../sailfish/src/QtMainWindow.h \
    $$PWD/../sailfish/src/RootDelegate.h \
    ../../net/ssl.h \
    ../../unzip/zip.h \
    $$PWD/../sailfish/src/RhoNativeApiCall.h


}
contains(DEFINES, RHODES_VERSION_1)  {
HEADERS += oldVersion/ExternalWebView.h\
QtLogView.h \
oldVersion/qwebviewkineticscroller.h\
oldVersion/QtMainWindow.h\
oldVersion/qkineticscroller.h\
oldVersion/qkineticscroller_p.h\
oldVersion/qwebviewselectionsuppressor.h\
oldVersion/qtscroller.h\
oldVersion/qtscroller_p.h\
oldVersion/qtscrollerproperties.h\
oldVersion/qtscrollerproperties_p.h\
oldVersion/qtflickgesture_p.h\
oldVersion/qtscrollerfilter_p.h\
oldVersion/qtscrollevent.h\
oldVersion/qtscrollevent_p.h\
oldVersion/QtNativeTabBar.h\
oldVersion/QtWebPage.h\
oldVersion/DateTimeDialog.h\
oldVersion/RhoNativeApiCall.h

SOURCES += oldVersion/main.cpp\
oldVersion/ExternalWebView.cpp\
oldVersion/QtMainWindow.cpp\
oldVersion/QtNativeTabBar.cpp\
oldVersion/QtWebPage.cpp\
oldVersion/qkineticscroller.cpp\
oldVersion/qwebviewkineticscroller.cpp\
oldVersion/qtflickgesture.cpp\
oldVersion/qtscroller.cpp\
oldVersion/qtscrollerproperties.cpp\
oldVersion/qtscrollerfilter.cpp\
oldVersion/qtscrollevent.cpp\
oldVersion/DateTimeDialog.cpp\
oldVersion/RhoNativeApiCall.cpp

FORMS += oldVersion/ExternalWebView.ui\
oldVersion/QtMainWindow.ui\
oldVersion/QtWebInspector.ui\
oldVersion/DateTimeDialog.ui
}

contains(DEFINES, RHODES_VERSION_2) {
HEADERS += newVersion/QtMainWindow.h\
QtLogView.h \
newVersion/RhoNativeApiCall.h\
newVersion/QtNativeTabBar.h\
newVersion/QtWebEngineView.h\
newVersion/QtWebEnginePage.h\
newVersion/ExternalWebView.h\
newVersion/QWebEngineViewSelectionSuppressor.h\
newVersion/DateTimeDialog.h \
newVersion/WebUrlRequestInterceptor.h

SOURCES += newVersion/QtMainWindow.cpp\
newVersion/RhoNativeApiCall.cpp\
newVersion/QtNativeTabBar.cpp\
newVersion/QtWebEngineView.cpp\
newVersion/QtWebEnginePage.cpp\
newVersion/ExternalWebView.cpp\
newVersion/DateTimeDialog.cpp\
newVersion/main.cpp\
newVersion/WebUrlRequestInterceptor.cpp

}
RESOURCES += resources/common.qrc


contains(DEFINES, OS_SAILFISH)  {
unix:!macx:
LIBS += -L$$PWD/../../../linux/bin/sqlite3/ -lsqlite3
INCLUDEPATH += $$PWD/../../sqlite
DEPENDPATH += $$PWD/../../sqlite

unix:!macx: LIBS += -L$$PWD/../../../linux/bin/curl/ -lcurl
INCLUDEPATH += $$PWD/../../curl/include
DEPENDPATH += $$PWD/../../curl/include
unix:!macx: PRE_TARGETDEPS += $$PWD/../../../linux/bin/curl/libcurl.a\

unix:!macx: LIBS += -L$$PWD/../../../linux/bin/rubylib/ -lrubylib
INCLUDEPATH += $$PWD/../../ruby/include
DEPENDPATH += $$PWD/../../ruby/include
unix:!macx: PRE_TARGETDEPS += $$PWD/../../../linux/bin/rubylib/librubylib.a

unix:!macx: LIBS += -L$$PWD/../../../linux/bin/rholib/ -lrholib -lcurl
unix:!macx: PRE_TARGETDEPS += $$PWD/../../../linux/bin/rholib/librholib.a

unix:!macx: LIBS += -L$$PWD/../../../linux/bin/syncengine/ -lsyncengine -lsqlite3
unix:!macx: PRE_TARGETDEPS += $$PWD/../../../linux/bin/syncengine/libsyncengine.a

unix:!macx: LIBS += -L$$PWD/../../../linux/bin/extensions/ -lcoreapi
unix:!macx: PRE_TARGETDEPS += $$PWD/../../../linux/bin/extensions/libcoreapi.a

unix:!macx: LIBS += -L$$PWD/../../../linux/bin/extensions/ -lzlib
unix:!macx: PRE_TARGETDEPS += $$PWD/../../../linux/bin/extensions/libzlib.a

unix:!macx: LIBS += -L$$PWD/../../../linux/bin/extensions/ -lMediacapture
unix:!macx: PRE_TARGETDEPS += $$PWD/../../../linux/bin/extensions/libMediacapture.a

unix:!macx: LIBS += -L$$PWD/../../../linux/bin/extensions/ -lBarcode
unix:!macx: PRE_TARGETDEPS += $$PWD/../../../linux/bin/extensions/libBarcode.a

unix:!macx: LIBS += -L$$PWD/../../../linux/bin/extensions/ -lSignature
unix:!macx: PRE_TARGETDEPS += $$PWD/../../../linux/bin/extensions/libSignature.a

unix:!macx: LIBS += -L$$PWD/../../../linux/bin/extensions/ -lRhoconnect-client
unix:!macx: PRE_TARGETDEPS += $$PWD/../../../linux/bin/extensions/libRhoconnect-client.a

unix:!macx: LIBS += -L$$PWD/../../../linux/bin/extensions/ -lserialport
unix:!macx: PRE_TARGETDEPS += $$PWD/../../../linux/bin/extensions/libserialport.a

unix:!macx: LIBS += -L$$PWD/../../../linux/bin/extensions/ -lbluetooth
unix:!macx: PRE_TARGETDEPS += $$PWD/../../../linux/bin/extensions/libbluetooth.a
}
