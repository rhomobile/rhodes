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

#include "stdafx.h"

#include "NativeTabbarQt.h"
#include "common/rhoparams.h"
#include "MainWindow.h"
#include "common/RhoFilePath.h"
#include "rubyext/WebView.h"
#undef null
#include <QString>
#include <QApplication>
#include <QtGui/QAction>

extern CMainWindow& getAppWindow();
IMPLEMENT_LOGCLASS(CNativeTabbar,"NativeTabbar");

using namespace rho;
using namespace rho::common;

CNativeTabbar::CNativeTabbar(void)
{
}

CNativeTabbar::~CNativeTabbar(void)
{
}

void CNativeTabbar::OnFinalMessage(HWND /*hWnd*/)
{
    getAppWindow().removeAllTabs(false);
}

/*static*/ CNativeTabbar& CNativeTabbar::getInstance()
{
    return getAppWindow().getTabbar();
}

void CNativeTabbar::createTabbar(int bar_type, rho_param *p)
{
    getAppWindow().createTabbar(bar_type, p);
}

void CNativeTabbar::removeTabbar()
{
    getAppWindow().removeTabbar();
}

int CNativeTabbar::getHeight()
{
    return getAppWindow().getTabbarHeight();
}

bool CNativeTabbar::isStarted()
{
    return getAppWindow().isStarted();
}

void CNativeTabbar::tabbarSwitch(int index)
{
    getAppWindow().tabbarSwitch(index);
}

void CNativeTabbar::tabbarBadge(int index, char* badge)
{
    getAppWindow().tabbarBadge(index, badge);
}
