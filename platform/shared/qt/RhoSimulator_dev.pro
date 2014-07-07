TEMPLATE = subdirs
SUBDIRS = rubylib rholib sqlite3 syncengine \
../../../lib/commonAPI/coreapi/ext/platform/qt/coreapi.pro \
../../../lib/commonAPI/logCapture/ext/platform/qt/logCapture.pro \
../../../../rhoconnect-client/ext/rhoconnect-client/ext/platform/qt/rhoconnect-client.pro \
../../../lib/extensions/digest/ext/digest.pro \
../../../lib/extensions/digest-md5/ext/digest-md5.pro \
../../../lib/extensions/digest-sha1/ext/digest-sha1.pro \
../../../lib/extensions/digest-sha2/ext/digest-sha2.pro \
../../../lib/extensions/fcntl/ext/fcntl.pro \
../../../lib/extensions/openssl/ext/openssl.pro \
../../../lib/extensions/serialport/ext/serialport.pro \
../../../lib/extensions/zlib/ext/zlib.pro

unix:!macx {
 # SUBDIRS += curl
}
macx {
 SUBDIRS += curl
}
CONFIG += ordered
SUBDIRS += rhodes
