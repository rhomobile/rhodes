greaterThan(QT_MINOR_VERSION, 6): {
    CONFIG += c++14
    DEFINES += CPP_ELEVEN
    DEFINES += RHODES_VERSION_2
}

equals(QT_MAJOR_VERSION, 5) {
    equals(QT_MINOR_VERSION, 6) {
        QT += webkit widgets
        DEFINES += OS_SAILFISH OS_LINUX CPP_ELEVEN
        CONFIG += c++14
    }
}

lessThan(QT_MINOR_VERSION, 6): {
    DEFINES += RHODES_VERSION_1
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
#../../../lib/extensions/serialport/ext/serialport.pro \
../../../lib/extensions/zlib/ext/zlib.pro# \
#../../../lib/commonAPI/mediacapture/ext/platform/qt/MediaCapture.pro \
#../../../lib/commonAPI/barcode/ext/platform/qt/Barcode.pro

unix:!macx {
 # SUBDIRS += curl
}
macx {
 SUBDIRS += curl
}
CONFIG += ordered
SUBDIRS += rhodes

QMAKE_CXXFLAGS_RELEASE += -MP9
QMAKE_CXXFLAGS_DEBUG += -MP9
