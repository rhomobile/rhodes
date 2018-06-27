greaterThan(QT_MINOR_VERSION, 6): {
    CONFIG += c++14
    DEFINES += CPP_ELEVEN
    DEFINES += RHODES_VERSION_2
}

lessThan(QT_MINOR_VERSION, 6): {
    DEFINES += RHODES_VERSION_1
}
#DEFINES += RHODES_EMULATOR

TEMPLATE = subdirs
SUBDIRS = rubylib rholib sqlite3 syncengine

unix:!macx {
 # SUBDIRS += curl
}
macx {
 SUBDIRS += curl
 QMAKE_MAC_SDK = macosx10.13
}
CONFIG += ordered
SUBDIRS += rhodes
