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

#include "NativeToolbarQt.h"
#include "common/rhoparams.h"
#include "MainWindowQt.h"
#include "common/RhoFilePath.h"
#include "rubyext/WebView.h"
#undef null
#include <QString>
#include <QApplication>
#include <QtGui/QAction>

extern IMainWindow& getAppWindow();
extern IMainWindow* getMainWindowObject();

IMPLEMENT_LOGCLASS(CNativeToolbarQt,"NativeToolbar");

using namespace rho;
using namespace rho::common;

CNativeToolbarQt::CNativeToolbarQt(void)
{
}

CNativeToolbarQt::~CNativeToolbarQt(void)
{
}

void CNativeToolbarQt::OnFinalMessage(HWND /*hWnd*/)
{
    getAppWindow().removeAllButtons();
}

IToolbar* CNativeToolbarQt::getInstance()
{
    CMainWindowQt *qtMainWnd = static_cast<CMainWindowQt*>(getMainWindowObject());
    return qtMainWnd->getToolbar();
}

void CNativeToolbarQt::createToolbar(rho_param *p)
{
    CMainWindowQt *qtMainWnd = static_cast<CMainWindowQt*>(getMainWindowObject());
    qtMainWnd->createToolbar(p);
    //getAppWindow().createToolbar(p);
}

void CNativeToolbarQt::removeToolbar()
{
    CMainWindowQt *qtMainWnd = static_cast<CMainWindowQt*>(getMainWindowObject());
    qtMainWnd->removeToolbar();
    //getAppWindow().removeToolbar();
}

int CNativeToolbarQt::getHeight()
{
    CMainWindowQt *qtMainWnd = static_cast<CMainWindowQt*>(getMainWindowObject());
    return qtMainWnd->getTabbarHeight();
    //return getAppWindow().getToolbarHeight();
}

bool CNativeToolbarQt::isStarted()
{
    CMainWindowQt *qtMainWnd = static_cast<CMainWindowQt*>(getMainWindowObject());
    return qtMainWnd->isStarted();
    //return getAppWindow().isStarted();
}
