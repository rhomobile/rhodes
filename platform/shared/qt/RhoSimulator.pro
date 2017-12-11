    greaterThan(QT_VERSION, 5.7.0): {
        CONFIG += c++14
        DEFINES += CPP_ELEVEN
        DEFINES += RHODES_VERSION_2
    }

    equals(QT_VERSION, 5.6.2) {
        QT += webkit widgets
        DEFINES += OS_SAILFISH OS_LINUX CPP_ELEVEN
        CONFIG += c++14
    }

    lessThan(QT_VERSION, 5.6.0): {
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
}
CONFIG += ordered
SUBDIRS += rhodes
