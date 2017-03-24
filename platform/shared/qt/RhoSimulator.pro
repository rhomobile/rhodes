    greaterThan(QT_VERSION, 5.6.0): {
        CONFIG += c++14
        DEFINES += CPP_ELEVEN
        DEFINES += RHODES_VERSION_2
    }

    lessThan(QT_VERSION, 5.6.0): {
        DEFINES += RHODES_VERSION_1
    }

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
