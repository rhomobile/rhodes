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
#include "MainWindowImpl.h"
#include "rubyext/NativeToolbarExt.h"
#include "rubyext/WebView.h"
#include "NativeTabbarImpl.h"
#undef null
#include <QString>
#ifndef OS_SAILFISH
#include <QApplication>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QAction>
#else
#include <QtGui/QAction>
#endif

#endif
//IMPLEMENT_LOGCLASS(CNativeTabbar,"NativeTabbar");

using namespace rho;
using namespace rho::common;

/////////////////////////
//Common API Support

bool rho_osximpl_tabbar_isStarted()
{
    return CMainWindow::getInstance()->isStarted();
}

void rho_osximpl_tabbar_create( const rho::Vector<rho::String>& tabbarElements,  const rho::Hashtable<rho::String, rho::String>& tabBarProperties, rho::apiGenerator::CMethodResult& oResult)
{
    CMainWindow::getInstance()->createTabbarEx(tabbarElements, tabBarProperties, oResult);
}

void rho_osximpl_tabbar_remove()
{
    CMainWindow::getInstance()->removeTabbar();
}

int rho_osximpl_tabbar_currentTabIndex()
{
    return CMainWindow::getInstance()->tabbarGetCurrent();
}

void rho_osximpl_tabbar_switchTab(int tabIndex)
{
    CMainWindow::getInstance()->tabbarSwitch(tabIndex);
}

void rho_osximpl_tabbar_setTabBadge(int tabIndex,  const rho::String& badge)
{
    CMainWindow::getInstance()->tabbarBadge(tabIndex, badge.c_str());
}
