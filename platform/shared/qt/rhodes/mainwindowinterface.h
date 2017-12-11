#ifndef MAINWINDOWINTERFACE_H
#define MAINWINDOWINTERFACE_H

#endif // MAINWINDOWINTERFACE_H

#ifdef RHODES_VERSION_1
#include "oldVersion/QtMainWindow.h"
#endif

#ifdef RHODES_VERSION_2
#include "newVersion/QtMainWindow.h"
#endif

#ifdef OS_SAILFISH
#include "../sailfish/src/QtMainWindow.h"
#endif
