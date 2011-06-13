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
