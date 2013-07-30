TEMPLATE = subdirs
SUBDIRS = rubylib rholib sqlite3 syncengine ../../../lib/commonAPI/coreapi/ext/platform/osx/coreapi.pro
unix:!macx {
 # SUBDIRS += curl
}
macx {
 SUBDIRS += curl
}
CONFIG += ordered
SUBDIRS += rhodes
