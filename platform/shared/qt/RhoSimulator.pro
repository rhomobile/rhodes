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


#DEFINES += RHODES_EMULATOR
CONFIG -= qml


TEMPLATE = subdirs
SUBDIRS = rubylib rholib sqlite3 syncengine

unix:!macx {
    SUBDIRS += curl
    DEFINES += CPP_ELEVEN
    DEFINES += OS_LINUX
    QMAKE_CXXFLAGS_RELEASE += -MP9
    QMAKE_CXXFLAGS_DEBUG += -MP9

    QMAKE_CFLAGS += -fvisibility=hidden
    QMAKE_CXXFLAGS += -fvisibility=hidden

    #QMAKE_CFLAGS_RELEASE -= -O2
    #QMAKE_CXXFLAGS_RELEASE -= -O2

}
macx {
 SUBDIRS += curl
 QMAKE_MAC_SDK = macosx11.1
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
