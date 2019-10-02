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

win32{
    QMAKE_CXXFLAGS_RELEASE += /MP9 /O2
    QMAKE_CXXFLAGS_DEBUG += /MP9 /O2

    QMAKE_CFLAGS_RELEASE += /O2 /MD
    QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO += /O2
    QMAKE_CFLAGS_DEBUG += /Zi /MDd
}
CONFIG += ordered
SUBDIRS += rhodes
