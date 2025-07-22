equals(QT_MAJOR_VERSION, 5) {
    lessThan(QT_MINOR_VERSION, 6): {
        DEFINES += RHODES_VERSION_1
    }
    equals(QT_MINOR_VERSION, 6): {
        QT += core
        DEFINES += OS_SAILFISH OS_LINUX
        CONFIG +=  c++14
    }
    greaterThan(QT_MINOR_VERSION, 7): {
        CONFIG += c++14
        DEFINES += CPP_ELEVEN RHODES_VERSION_2
    }
}
TARGET = coreapi
TEMPLATE = lib

CONFIG += staticlib warn_on
#CONFIG += debug

INCLUDEPATH += ../../shared\
../../shared/generated\
../../shared/generated/cpp\
../../../../../../platform/shared/common\
../../../../../../platform/shared/rubyext\
../../../../../../platform/shared/ruby/include\
../../../../../../platform/shared

macx {
  DESTDIR = ../../../../../../platform/osx/bin/extensions
  OBJECTS_DIR = ../../../../../../platform/osx/bin/extensions/coreapi
  INCLUDEPATH += ../../../../../../platform/shared/ruby/iphone
  SOURCES += src/CNetworkDetect.cpp
}

unix:!macx {
    DEFINES += OS_LINUX
    DESTDIR = $$PWD/../../../../../../platform/linux/bin/extensions
    OBJECTS_DIR = $$PWD/../../../../../../platform/linux/bin/extensions/coreapi
    INCLUDEPATH += $$PWD/../../../../../../platform/shared/ruby/linux

    SOURCES += src/CNetworkDetect.cpp
    QMAKE_CFLAGS += -fvisibility=hidden
    QMAKE_CXXFLAGS += -fvisibility=hidden
}

win32 {
  greaterThan(QT_MINOR_VERSION, 6): {
      DEFINES += CPP_ELEVEN
  }
  DESTDIR = ../../../../../../platform/win32/bin/extensions
  OBJECTS_DIR = ../../../../../../platform/win32/bin/extensions/coreapi
  DEFINES += WIN32 _WINDOWS _LIB _UNICODE UNICODE WIN32_LEAN_AND_MEAN
  Debug {
    DEFINES += _DEBUG DEBUG
  }
  Release {
    DEFINES += _NDEBUG NDEBUG
  }
  INCLUDEPATH += ../../../../../../platform/shared/ruby/win32\
../../../../../../lib/extensions/zlib/ext
  HEADERS += ../../../../../../platform/shared/ruby/win32/ruby/config.h\
../../../../../../platform/shared/ruby/win32/dir.h\
../wm/src/NetworkDetect.h
  SOURCES += ../wm/src/NetworkDetect.cpp\
../wm/src/SystemImpl.cpp\
../wm/src/Registry.cpp
}

DEFINES += RHODES_QT_PLATFORM

!isEmpty(RHOSIMULATOR_BUILD) {
  DEFINES += RHODES_EMULATOR
}

!win32 {
  QMAKE_CFLAGS_WARN_ON += -Wno-extra -Wno-unused -Wno-sign-compare -Wno-parentheses
  QMAKE_CXXFLAGS_WARN_ON += -Wno-extra -Wno-unused -Wno-sign-compare -Wno-parentheses
  QMAKE_CFLAGS_RELEASE -= -Werror=format-security
  QMAKE_CXXFLAGS_RELEASE -= -Werror=format-security
  # QMAKE_CFLAGS += -fvisibility=hidden
  # QMAKE_CXXFLAGS += -fvisibility=hidden
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

HEADERS += \
../../shared/Application.h\
../../shared/INetworkDetect.h\
../../shared/Network.h\
../../shared/NetworkDetectBase.h\
../../shared/Push.h\
../../shared/System.h\
../../shared/SystemImplBase.h\
../../shared/IntentImplBase.h\
../../shared/generated/cpp/ApplicationBase.h\
../../shared/generated/cpp/DatabaseBase.h\
../../shared/generated/cpp/IApplication.h\
../../shared/generated/cpp/IConfig.h\
../../shared/generated/cpp/IDatabase.h\
../../shared/generated/cpp/ILog.h\
../../shared/generated/cpp/INativeMenubar.h\
../../shared/generated/cpp/INativeTabbar.h\
../../shared/generated/cpp/INativeToolbar.h\
../../shared/generated/cpp/INavbar.h\
../../shared/generated/cpp/INetwork.h\
../../shared/generated/cpp/INewORM.h\
../../shared/generated/cpp/INewORMModel.h\
../../shared/generated/cpp/INotification.h\
../../shared/generated/cpp/IProcess.h\
../../shared/generated/cpp/IPush.h\
../../shared/generated/cpp/IRhoFile.h\
../../shared/generated/cpp/ISQLite3.h\
../../shared/generated/cpp/ISystem.h\
../../shared/generated/cpp/IWebView.h\
../../shared/generated/cpp/LogBase.h\
../../shared/generated/cpp/NativeMenubarBase.h\
../../shared/generated/cpp/NativeTabbarBase.h\
../../shared/generated/cpp/NativeToolbarBase.h\
../../shared/generated/cpp/NavbarBase.h\
../../shared/generated/cpp/NetworkBase.h\
../../shared/generated/cpp/NewORMBase.h\
../../shared/generated/cpp/NewORMModelBase.h\
../../shared/generated/cpp/NotificationBase.h\
../../shared/generated/cpp/ProcessBase.h\
../../shared/generated/cpp/PushBase.h\
../../shared/generated/cpp/RhoFileBase.h\
../../shared/generated/cpp/SQLite3Base.h\
../../shared/generated/cpp/SystemBase.h\
../../shared/generated/cpp/WebViewBase.h\
../../shared/generated/cpp/ITimer.h\
../../shared/generated/cpp/TimerBase.h\
../../shared/generated/cpp/ConfigBase.h

SOURCES += \
../../shared/SQLite3ImplRuby.c\
../../shared/ApplicationImpl.cpp\
../../shared/ConfigImpl.cpp\
../../shared/FileImpl.cpp\
../../shared/InitExtension.cpp\
../../shared/LogImpl.cpp\
../../shared/MenubarImpl.cpp\
../../shared/NetworkDetectBase.cpp\
../../shared/NetworkImpl.cpp\
../../shared/NewORMImpl.cpp\
../../shared/NewORMModelImpl.cpp\
../../shared/PushImpl.cpp\
../../shared/SQLite3Impl.cpp\
../../shared/SystemImplBase.cpp\
../../shared/IntentImplBase.cpp\
../../shared/TimerImpl.cpp\
../../shared/generated/application_api_init.cpp\
../../shared/generated/application_js_api.cpp\
../../shared/generated/application_ruby_api.c\
../../shared/generated/database_api_init.cpp\
../../shared/generated/database_js_api.cpp\
../../shared/generated/database_ruby_api.c\
../../shared/generated/config_api_init.cpp\
../../shared/generated/config_js_api.cpp\
../../shared/generated/config_ruby_api.c\
../../shared/generated/log_api_init.cpp\
../../shared/generated/log_js_api.cpp\
../../shared/generated/log_ruby_api.c\
../../shared/generated/nativemenubar_api_init.cpp\
../../shared/generated/nativemenubar_js_api.cpp\
../../shared/generated/nativemenubar_ruby_api.c\
../../shared/generated/nativetabbar_api_init.cpp\
../../shared/generated/nativetabbar_js_api.cpp\
../../shared/generated/nativetabbar_ruby_api.c\
../../shared/generated/nativetoolbar_api_init.cpp\
../../shared/generated/nativetoolbar_js_api.cpp\
../../shared/generated/nativetoolbar_ruby_api.c\
../../shared/generated/navbar_api_init.cpp\
../../shared/generated/navbar_js_api.cpp\
../../shared/generated/navbar_ruby_api.c\
../../shared/generated/network_api_init.cpp\
../../shared/generated/network_js_api.cpp\
../../shared/generated/network_ruby_api.c\
../../shared/generated/neworm_api_init.cpp\
../../shared/generated/neworm_js_api.cpp\
../../shared/generated/neworm_ruby_api.c\
../../shared/generated/newormmodel_api_init.cpp\
../../shared/generated/newormmodel_js_api.cpp\
../../shared/generated/newormmodel_ruby_api.c\
../../shared/generated/notification_api_init.cpp\
../../shared/generated/notification_js_api.cpp\
../../shared/generated/notification_ruby_api.c\
../../shared/generated/process_api_init.cpp\
../../shared/generated/process_js_api.cpp\
../../shared/generated/process_ruby_api.c\
../../shared/generated/push_api_init.cpp\
../../shared/generated/push_js_api.cpp\
../../shared/generated/push_ruby_api.c\
../../shared/generated/rhofile_api_init.cpp\
../../shared/generated/rhofile_js_api.cpp\
../../shared/generated/rhofile_ruby_api.c\
../../shared/generated/sqlite3_api_init.cpp\
../../shared/generated/sqlite3_js_api.cpp\
../../shared/generated/sqlite3_ruby_api.c\
../../shared/generated/system_api_init.cpp\
../../shared/generated/system_js_api.cpp\
../../shared/generated/system_ruby_api.c\
../../shared/generated/webview_api_init.cpp\
../../shared/generated/webview_js_api.cpp\
../../shared/generated/webview_ruby_api.c\
../../shared/generated/timer_api_init.cpp\
../../shared/generated/timer_js_api.cpp\
../../shared/generated/timer_ruby_api.c\
../../shared/generated/cpp/ApplicationBase.cpp\
../../shared/generated/cpp/Application_js_wrap.cpp\
../../shared/generated/cpp/Application_ruby_wrap.cpp\
../../shared/generated/cpp/ConfigBase.cpp\
../../shared/generated/cpp/Config_js_wrap.cpp\
../../shared/generated/cpp/Config_ruby_wrap.cpp\
../../shared/generated/cpp/DatabaseBase.cpp\
../../shared/generated/cpp/Database_js_wrap.cpp\
../../shared/generated/cpp/Database_ruby_wrap.cpp\
../../shared/generated/cpp/LogBase.cpp\
../../shared/generated/cpp/Log_js_wrap.cpp\
../../shared/generated/cpp/Log_ruby_wrap.cpp\
../../shared/generated/cpp/NativeMenubarBase.cpp\
../../shared/generated/cpp/NativeMenubar_js_wrap.cpp\
../../shared/generated/cpp/NativeMenubar_ruby_wrap.cpp\
../../shared/generated/cpp/NativeTabbarBase.cpp\
../../shared/generated/cpp/NativeTabbar_js_wrap.cpp\
../../shared/generated/cpp/NativeTabbar_ruby_wrap.cpp\
../../shared/generated/cpp/NativeToolbarBase.cpp\
../../shared/generated/cpp/NativeToolbar_js_wrap.cpp\
../../shared/generated/cpp/NativeToolbar_ruby_wrap.cpp\
../../shared/generated/cpp/NavbarBase.cpp\
../../shared/generated/cpp/Navbar_js_wrap.cpp\
../../shared/generated/cpp/Navbar_ruby_wrap.cpp\
../../shared/generated/cpp/NetworkBase.cpp\
../../shared/generated/cpp/Network_js_wrap.cpp\
../../shared/generated/cpp/Network_ruby_wrap.cpp\
../../shared/generated/cpp/NewORMBase.cpp\
../../shared/generated/cpp/NewORMModelBase.cpp\
../../shared/generated/cpp/NewORMModel_js_wrap.cpp\
../../shared/generated/cpp/NewORMModel_ruby_wrap.cpp\
../../shared/generated/cpp/NewORM_js_wrap.cpp\
../../shared/generated/cpp/NewORM_ruby_wrap.cpp\
../../shared/generated/cpp/NotificationBase.cpp\
../../shared/generated/cpp/Notification_js_wrap.cpp\
../../shared/generated/cpp/Notification_ruby_wrap.cpp\
../../shared/generated/cpp/ProcessBase.cpp\
../../shared/generated/cpp/Process_js_wrap.cpp\
../../shared/generated/cpp/Process_ruby_wrap.cpp\
../../shared/generated/cpp/PushBase.cpp\
../../shared/generated/cpp/Push_js_wrap.cpp\
../../shared/generated/cpp/Push_ruby_wrap.cpp\
../../shared/generated/cpp/RhoFileBase.cpp\
../../shared/generated/cpp/RhoFile_js_wrap.cpp\
../../shared/generated/cpp/RhoFile_ruby_wrap.cpp\
../../shared/generated/cpp/SQLite3Base.cpp\
../../shared/generated/cpp/SQLite3_js_wrap.cpp\
../../shared/generated/cpp/SQLite3_ruby_wrap.cpp\
../../shared/generated/cpp/SystemBase.cpp\
../../shared/generated/cpp/System_js_wrap.cpp\
../../shared/generated/cpp/System_ruby_wrap.cpp\
../../shared/generated/cpp/WebViewBase.cpp\
../../shared/generated/cpp/WebView_js_wrap.cpp\
../../shared/generated/cpp/WebView_ruby_wrap.cpp\
../../shared/generated/cpp/Timer_js_wrap.cpp\
../../shared/generated/cpp/Timer_ruby_wrap.cpp\
../../shared/generated/cpp/TimerBase.cpp\
../../shared/qt/NavbarImpl.cpp\
src/CNativeTabbarImpl.cpp\
src/CNativeToolbarImpl.cpp\
src/CNotificationImpl.cpp

# CSystemImpl.cpp & CWebViewImpl.cpp must be compiled in Qt environment (see /platform/shared/qt/rhodes/rhodes.pro)
#src/CSystemImpl.cpp
#src/CWebViewImpl.cpp
