#include "stdafx.h"

#include "NativeToolbarQt.h"
#include "common/rhoparams.h"
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
    getAppWindow().removeAllButtons();
}

/*static*/ CNativeToolbar& CNativeToolbar::getInstance()
{
    return getAppWindow().getToolbar();
}

void CNativeToolbar::createToolbar(rho_param *p)
{
    getAppWindow().createToolbar(p);
}


void CNativeToolbar::removeToolbar()
{
    getAppWindow().removeToolbar();
}

int CNativeToolbar::getHeight()
{
    return getAppWindow().getToolbarHeight();
}

bool CNativeToolbar::isStarted()
{
    return getAppWindow().isStarted();
}
