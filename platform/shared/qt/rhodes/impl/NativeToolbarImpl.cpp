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

#include "common/RhoPort.h"
#include "ext/rho/rhoruby.h"
#include "common/rhoparams.h"
#include "common/RhoFilePath.h"
#include "logging/RhoLog.h"
#include "api_generator/MethodResult.h"
#include "rubyext/NativeToolbarExt.h"
#include "rubyext/WebView.h"
#include "MainWindowImpl.h"
#include "NativeToolbarImpl.h"
#include "rubyext/NativeToolbarExt.h"
#undef null
#ifndef RHODES_VERSION_LIBRARY
#include <QString>
#ifndef OS_SAILFISH
#if QT_VERSION >= 0x050000
#include <QAction>
#else
#include <QtGui/QAction>
#endif
#endif
#endif //RHODES_VERSION_LIBRARY

//IMPLEMENT_LOGCLASS(CNativeToolbar,"NativeToolbar");

using namespace rho;
using namespace rho::common;

/////////////////////////
//Common API Support

bool rho_osximpl_toolbar_isStarted()
{
#ifndef RHODES_VERSION_LIBRARY
    return CMainWindow::getInstance()->isStarted();
#else
    return false;
#endif
}

void rho_osximpl_toolbar_create( const rho::Vector<rho::String>& toolbarElements,  const rho::Hashtable<rho::String, rho::String>& toolBarProperties)
{
#ifndef RHODES_VERSION_LIBRARY
    CMainWindow::getInstance()->createToolbarEx(toolbarElements, toolBarProperties);
#endif
}

void rho_osximpl_toolbar_remove()
{
#ifndef RHODES_VERSION_LIBRARY
    CMainWindow::getInstance()->removeToolbar();
#endif
}
