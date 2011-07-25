isEmpty(QMAKE_EXTENSION_STATICLIB) {
  win32|symbian:QMAKE_EXTENSION_STATICLIB = "lib"
  else:QMAKE_EXTENSION_STATICLIB = "a"
}

!no-webkit:QT *= webkit
CONFIG *= link_prl

INCLUDEPATH *= $$IN_PWD/include
symbian:LIBS *= -lQtScroller.$$QMAKE_EXTENSION_STATICLIB
else:LIBS *= -L$$QTSCROLLER_OUT/lib -lQtScroller

!symbian:PRE_TARGETDEPS *= $$QTSCROLLER_OUT/lib/libQtScroller.$$QMAKE_EXTENSION_STATICLIB
