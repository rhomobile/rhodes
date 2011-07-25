TEMPLATE = lib
CONFIG *= staticlib create_prl
DEPENDPATH += ../include
INCLUDEPATH += ../include
TARGET = QtScroller
DESTDIR = ../lib

!no-webkit:QT += webkit
no-webkit:DEFINES += QTSCROLLER_NO_WEBKIT

# Input
HEADERS += qtflickgesture_p.h \
           qtscroller.h \
           qtscroller_p.h \
           qtscrollerfilter_p.h \
           qtscrollerproperties.h \
           qtscrollerproperties_p.h \
           qtscrollevent.h \
           qtscrollevent_p.h
SOURCES += qtflickgesture.cpp \
           qtscroller.cpp \
           qtscrollerfilter.cpp \
           qtscrollerproperties.cpp \
           qtscrollevent.cpp
