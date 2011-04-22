#include "stdafx.h"

#include "NativeToolbarQt.h"
#include "common/rhoparams.h"
#include "MainWindowProxy.h"
#include "MainWindow.h"
#include "common/RhoFilePath.h"
#include "rubyext/WebView.h"
#undef null
#include <QString>
#include <QApplication>
#include <QtGui/QAction>

extern CMainWindow& getAppWindow();
IMPLEMENT_LOGCLASS(CNativeToolbar,"NativeToolbar");

using namespace rho;
using namespace rho::common;

CNativeToolbar::CNativeToolbar(void)
{
}

CNativeToolbar::~CNativeToolbar(void)
{
}

void CNativeToolbar::OnFinalMessage(HWND /*hWnd*/)
{
    getAppWindow().getProxy().removeAllButtons();
}

/*static*/ CNativeToolbar& CNativeToolbar::getInstance()
{
    return getAppWindow().getToolbar();
}

void CNativeToolbar::createToolbar(rho_param *p)
{
    getAppWindow().getProxy().createToolbar(p);
}


void CNativeToolbar::removeToolbar()
{
    getAppWindow().getProxy().removeToolbar();
}

int CNativeToolbar::getHeight()
{
    return getAppWindow().getProxy().getHeight();
}

bool CNativeToolbar::isStarted()
{
    return getAppWindow().getProxy().isStarted();
}
