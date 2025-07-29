/*------------------------------------------------------------------------
* (The MIT License)
* 
* Copyright (c) 2008-2011 Rhomobile, Inc.
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
* 
* http://rhomobile.com
*------------------------------------------------------------------------*/

#include "common/rhoparams.h"
#include "common/RhoFilePath.h"
#include "rubyext/WebView.h"
#include "rubyext/NativeToolbarExt.h"
#include "rubyext/WebView.h"
#include "NativeTabbarImpl.h"
#undef null
#ifndef RHODES_VERSION_LIBRARY
#include "MainWindowImpl.h"
#include <QString>
#ifndef OS_SAILFISH
#include <QApplication>
#if QT_VERSION >= 0x060000
#include <QAction>
#elif QT_VERSION < 0x060000 && QT_VERSION >= 0x050000
#include <QtWidgets/QAction>
#else
#include <QtGui/QAction>
#endif
#endif
#else
#include "api_generator/MethodResult.h"
#endif //RHODES_VERSION_LIBRARY
//IMPLEMENT_LOGCLASS(CNativeTabbar,"NativeTabbar");

using namespace rho;
using namespace rho::common;

/////////////////////////
//Common API Support

bool rho_osximpl_tabbar_isStarted()
{
#ifndef RHODES_VERSION_LIBRARY
    return CMainWindow::getInstance()->isStarted();
#else
    return false;
#endif
}

void rho_osximpl_tabbar_create( const rho::Vector<rho::String>& tabbarElements,  const rho::Hashtable<rho::String, rho::String>& tabBarProperties, rho::apiGenerator::CMethodResult& oResult)
{
#ifndef RHODES_VERSION_LIBRARY
    CMainWindow::getInstance()->createTabbarEx(tabbarElements, tabBarProperties, oResult);
#endif
}

void rho_osximpl_tabbar_remove()
{
#ifndef RHODES_VERSION_LIBRARY
    CMainWindow::getInstance()->removeTabbar();
#endif
}

int rho_osximpl_tabbar_currentTabIndex()
{
#ifndef RHODES_VERSION_LIBRARY
    return CMainWindow::getInstance()->tabbarGetCurrent();
#else
    return -1;
#endif
}

void rho_osximpl_tabbar_switchTab(int tabIndex)
{
#ifndef RHODES_VERSION_LIBRARY
    CMainWindow::getInstance()->tabbarSwitch(tabIndex);
#endif
}

void rho_osximpl_tabbar_setTabBadge(int tabIndex,  const rho::String& badge)
{
#ifndef RHODES_VERSION_LIBRARY
    CMainWindow::getInstance()->tabbarBadge(tabIndex, badge.c_str());
#endif
}
