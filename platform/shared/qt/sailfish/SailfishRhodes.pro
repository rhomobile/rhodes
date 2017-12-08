TARGET = SailfishRhodes
QT += webkit widgets core
CONFIG += sailfishapp c++11

OTHER_FILES += ./data/*

DEPLOYMENT_PATH = /usr/share/$${TARGET}
data.files = data
data.path = $${DEPLOYMENT_PATH}

INSTALLS += data

SOURCES += \
    src/main.cpp

DISTFILES += qml/SailfishRhodes.qml \
    qml/cover/CoverPage.qml \
    qml/pages/FirstPage.qml \
    rpm/SailfishRhodes.changes.in \
    rpm/SailfishRhodes.changes.run.in \
    rpm/SailfishRhodes.spec \
    rpm/SailfishRhodes.yaml \
    SailfishRhodes.desktop \
    translations/*.ts \
    qml/pages/MenuPage.qml

SAILFISHAPP_ICONS = 86x86 108x108 128x128

CONFIG += sailfishapp_i18n

HEADERS += src/custommenuitem.h

DEFINES += CPP_ELEVEN
DEFINES += OS_SAILFISH

TEMPLATE = subdirs
SUBDIRS = ../rubylib ../rholib ../sqlite3 ../syncengine \
../../../../lib/commonAPI/coreapi/ext/platform/qt/coreapi.pro \
#../../../../lib/commonAPI/logCapture/ext/platform/qt/logCapture.pro \
#../../../../../rhoconnect-client/ext/rhoconnect-client/ext/platform/qt/rhoconnect-client.pro \
#../../../../lib/extensions/digest/ext/digest.pro \
#../../../../lib/extensions/digest-md5/ext/digest-md5.pro \
#../../../../lib/extensions/digest-sha1/ext/digest-sha1.pro \
#../../../../lib/extensions/digest-sha2/ext/digest-sha2.pro \
#../../../../lib/extensions/fcntl/ext/fcntl.pro \
#../../../../lib/extensions/openssl/ext/openssl.pro \
#../../../../lib/extensions/serialport/ext/serialport.pro \
../../../../lib/extensions/zlib/ext/zlib.pro# \
#../../../../lib/commonAPI/mediacapture/ext/platform/qt/MediaCapture.pro \
#../../../../lib/commonAPI/barcode/ext/platform/qt/Barcode.pro

unix:!macx {
 # SUBDIRS += ../curl
}
macx {
 SUBDIRS += ../curl
}
CONFIG += ordered
SUBDIRS += ../rhodes

QMAKE_CXXFLAGS_RELEASE += -MP9
QMAKE_CXXFLAGS_DEBUG += -MP9
