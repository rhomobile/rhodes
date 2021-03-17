equals(QT_MAJOR_VERSION, 5) {
    lessThan(QT_MINOR_VERSION, 6): {
        DEFINES += RHODES_VERSION_1
    }
    equals(QT_MINOR_VERSION, 6): {
        QQT += webkit widgets
        DEFINES += OS_SAILFISH OS_LINUX CPP_ELEVEN
        CONFIG +=  c++14
    }
    greaterThan(QT_MINOR_VERSION, 7): {
        CONFIG += c++14
        DEFINES += CPP_ELEVEN RHODES_VERSION_2 _WINSOCKAPI_
    }
}



TEMPLATE = subdirs
SUBDIRS = rubylib rholib sqlite3 syncengine \
../../../lib/commonAPI/coreapi/ext/platform/qt/coreapi.pro \
#../../../lib/commonAPI/logCapture/ext/platform/qt/logCapture.pro \
#../../../../rhoconnect-client/ext/rhoconnect-client/ext/platform/qt/rhoconnect-client.pro \
#../../../lib/extensions/digest/ext/digest.pro \
#../../../lib/extensions/digest-md5/ext/digest-md5.pro \
#../../../lib/extensions/digest-sha1/ext/digest-sha1.pro \
#../../../lib/extensions/digest-sha2/ext/digest-sha2.pro \
#../../../lib/extensions/fcntl/ext/fcntl.pro \
#../../../lib/extensions/openssl/ext/openssl.pro \
#../../../lib/extensions/bigdecimal/ext/bigdecimal.pro \
../../../lib/extensions/zlib/ext/zlib.pro #\#\
#../../../../RMS-Testing/auto/feature_def/auto_common_spec/extensions/genPropBag/ext/platform/qt/genPropBag.pro \
#../../../lib/extensions/serialport/ext/serialport.pro \
#../../../lib/commonAPI/signature/ext/platform/qt/Signature.pro \
#../../../lib/commonAPI/mediacapture/ext/platform/qt/Mediacapture.pro \
#../../../lib/commonAPI/barcode/ext/platform/qt/Barcode.pro

unix:!macx {
    SUBDIRS += curl
    DEFINES += CPP_ELEVEN
    DEFINES += OS_LINUX
    QMAKE_CXXFLAGS_RELEASE += -MP9
    QMAKE_CXXFLAGS_DEBUG += -MP9

    QMAKE_CFLAGS_DEBUG -= -O2
    QMAKE_CXXFLAGS_DEBUG -= -O2

    QMAKE_CFLAGS += -fvisibility=hidden
    QMAKE_CXXFLAGS += -fvisibility=hidden
}
macx {
 SUBDIRS += curl
}
CONFIG += ordered
SUBDIRS += rhodes

QMAKE_CXXFLAGS_RELEASE += -MP9
QMAKE_CXXFLAGS_DEBUG += -MP9


