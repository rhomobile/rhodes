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
  INCLUDEPATH += ../../ruby/iphone
  HEADERS += ../../ruby/iphone/ruby/config.h\
../../ruby/iphone/crt_externs.h
}

DEFINES += RHODES_EMULATOR

QMAKE_CFLAGS_WARN_ON += -Wno-extra -Wno-unused -Wno-sign-compare -Wno-format -Wno-parentheses
QMAKE_CXXFLAGS_WARN_ON += -Wno-extra -Wno-unused -Wno-sign-compare -Wno-format -Wno-parentheses

INCLUDEPATH += ../..\
../../ruby\
../../ruby/include\
../../ruby/generated\
../../curl/include

HEADERS +=ExternalWebView.h\
MainWindowCallback.h\
QtMainWindow.h\
QtNativeTabBar.h\
QtWebInspector.h\
impl/RhoClassFactoryImpl.h\
impl/MainWindowImpl.h\
../../ruby/ext/rho/rhoruby.h\
../../ruby/ext/socket/addrinfo.h\
../../ruby/ext/socket/constants.h\
../../ruby/ext/socket/sockport.h\
../../ruby/ext/calendar/event.h\
../../ruby/debug.h\
../../ruby/dln.h\
../../ruby/eval_intern.h\
../../ruby/gc.h\
../../ruby/id.h\
../../ruby/iseq.h\
../../ruby/regenc.h\
../../ruby/regint.h\
../../ruby/regparse.h\
../../ruby/revision.h\
../../ruby/transcode_data.h\
../../ruby/version.h\
../../ruby/vm_core.h\
../../ruby/vm_opts.h\
../../ruby/thread_pthread.h\
../../common/RhoAppAdapter.h\
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
../../common/PosixThreadImpl.h\
../../net/CURLNetRequest.h\
../../net/INetRequest.h\
../../net/HttpServer.h\
../../common/SplashScreen.h\
../../common/rhoparams.h\
../../net/ssl.h\
../../common/app_build_configs.h\
../../sqlite/sqlite3.h\
../../sqlite/sqlite3ext.h\
../../json/arraylist.h\
../../json/bits.h\
../../json/config.h\
../../json/debug.h\
../../json/json.h\
../../json/json_object.h\
../../json/json_object_private.h\
../../json/json_tokener.h\
../../json/json_util.h\
../../json/linkhash.h\
../../json/printbuf.h\
../../sync/syncthread.h\
../../db/DBAttrManager.h\
../../sync/SyncNotify.h\
../../json/JSONIterator.h\
../../sync/SyncEngine.h\
../../sync/SyncSource.h\
../../db/DBAdapter.h\
../../db/DBResult.h\
../../sync/ClientRegister.h

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
impl/MainWindowImpl.cpp\
../../ruby/ext/datetimepicker/datetimepicker_wrap.c\
../../ruby/ext/rhoconf/rhoconf_wrap.c\
../../ruby/ext/nativebar/nativebar_wrap.c\
../../ruby/ext/sqlite3_api/sqlite3_api_wrap.c\
../../ruby/ext/rho/rhoruby.c\
../../ruby/ext/rho/rhosupport.c\
../../ruby/ext/stringio/stringio.c\
../../ruby/ext/geolocation/geolocation_wrap.c\
../../ruby/ext/asynchttp/asynchttp_wrap.c\
../../ruby/ext/socket/socket.c\
../../ruby/ext/ringtones/ringtones_wrap.c\
../../ruby/ext/calendar/calendar_wrap.c\
../../ruby/ext/calendar/event_wrap.c\
../../ruby/ext/navbar/navbar_wrap.c\
../../ruby/ext/rho/extensions.c\
../../ruby/ext/alert/alert_wrap.c\
../../ruby/ext/system/system_wrap.c\
../../ruby/ext/phonebook/phonebook_wrap.c\
../../ruby/ext/camera/camera_wrap.c\
../../ruby/ext/webview/webview_wrap.c\
../../ruby/array.c\
../../ruby/bignum.c\
../../ruby/blockinlining.c\
../../ruby/class.c\
../../ruby/compar.c\
../../ruby/compile.c\
../../ruby/complex.c\
../../ruby/cont.c\
../../ruby/debug.c\
../../ruby/dir.c\
../../ruby/dln.c\
../../ruby/dmyencoding.c\
../../ruby/dmyext.c\
../../ruby/enum.c\
../../ruby/enumerator.c\
../../ruby/error.c\
../../ruby/eval.c\
../../ruby/file.c\
../../ruby/gc.c\
../../ruby/hash.c\
../../ruby/inits.c\
../../ruby/io.c\
../../ruby/iseq.c\
../../ruby/load.c\
../../ruby/marshal.c\
../../ruby/math.c\
../../ruby/numeric.c\
../../ruby/object.c\
../../ruby/pack.c\
../../ruby/proc.c\
../../ruby/process.c\
../../ruby/random.c\
../../ruby/range.c\
../../ruby/rational.c\
../../ruby/re.c\
../../ruby/regcomp.c\
../../ruby/regenc.c\
../../ruby/regerror.c\
../../ruby/regexec.c\
../../ruby/regparse.c\
../../ruby/regsyntax.c\
../../ruby/ruby.c\
../../ruby/safe.c\
../../ruby/signal.c\
../../ruby/sprintf.c\
../../ruby/st.c\
../../ruby/strftime.c\
../../ruby/string.c\
../../ruby/struct.c\
../../ruby/thread.c\
../../ruby/time.c\
../../ruby/transcode.c\
../../ruby/util.c\
../../ruby/variable.c\
../../ruby/version.c\
../../ruby/vm.c\
../../ruby/vm_dump.c\
../../ruby/enc/ascii.c\
../../ruby/enc/unicode.c\
../../ruby/enc/us_ascii.c\
../../ruby/enc/utf_8.c\
../../ruby/ext/strscan/strscan.c\
../../ruby/ext/syncengine/syncengine_wrap.c\
../../ruby/generated/parse.c\
../../ruby/thread_pthread.c\
../../ruby/miniprelude.c\
../../ruby/missing/lgamma_r.c\
../../ruby/newline.c\
../../ruby/ext/mapview/mapview_wrap.c\
../../ruby/ext/signature/signature_wrap.c\
../../ruby/ext/nativeviewmanager/nativeviewmanager_wrap.c\
../../ruby/ext/bluetooth/bluetooth_wrap.c\
../../common/RhoTime.cpp\
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
../../common/PosixThreadImpl.cpp\
../../net/CURLNetRequest.cpp\
../../net/HttpServer.cpp\
../../common/SplashScreen.cpp\
../../net/ssl.cpp\
../../common/app_build_configs.c\
../../sqlite/sqlite3.c\
../../sqlite/crypto.c\
../../json/arraylist.c\
../../json/json_debug.c\
../../json/json_object.c\
../../json/json_tokener.c\
../../json/json_util.c\
../../json/linkhash.c\
../../json/printbuf.c\
../../json/RJSONTokener.c\
../../sync/syncthread.cpp\
../../db/DBAttrManager.cpp\
../../sync/SyncNotify.cpp\
../../json/JSONIterator.cpp\
../../sync/SyncEngine.cpp\
../../sync/SyncSource.cpp\
../../db/DBAdapter.cpp\
../../db/DBResult.cpp\
../../sync/ClientRegister.cpp

FORMS += ExternalWebView.ui\
QtMainWindow.ui\
QtWebInspector.ui
