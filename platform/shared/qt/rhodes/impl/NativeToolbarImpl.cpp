#include <time.h>
#include "ext/rho/rhoruby.h"
#include "common/rhoparams.h"
#include "common/RhoFilePath.h"
#include "logging/RhoLog.h"
#include "rubyext/NativeToolbarExt.h"
#include "rubyext/WebView.h"
#include "MainWindowImpl.h"
#include "NativeToolbarImpl.h"
#undef null
#include <QString>
#include <QApplication>
#include <QtGui/QAction>

IMPLEMENT_LOGCLASS(CNativeToolbar,"NativeToolbar");

using namespace rho;
using namespace rho::common;

CNativeToolbar& CNativeToolbar::getInstance()
{
    return CMainWindow::getInstance()->getToolbar();
}

void CNativeToolbar::createToolbar(rho_param *p)
{
    CMainWindow::getInstance()->createToolbar(p);
}

void CNativeToolbar::removeToolbar()
{
    CMainWindow::getInstance()->removeToolbar();
}

int CNativeToolbar::getHeight()
{
    return CMainWindow::getInstance()->getToolbarHeight();
}

bool CNativeToolbar::isStarted()
{
    return CMainWindow::getInstance()->isStarted();
}

extern "C" {

void remove_native_toolbar() 
{
    CMainWindow::getInstance()->executeRunnable(new CNativeToolbar::CRemoveTask() );
}

void create_native_toolbar(int bar_type, rho_param *p) 
{
    if ( bar_type == NOBAR_TYPE )
        remove_native_toolbar();
    else if ( bar_type == TOOLBAR_TYPE ) {
        CMainWindow::getInstance()->executeRunnable(new CNativeToolbar::CCreateTask(p) );
    } else {
        RAWLOGC_ERROR("NativeBar", "Only Toolbar control is supported.");
    }
}

void create_nativebar(int bar_type, rho_param *p) 
{
    RAWLOGC_INFO("NativeBar", "NativeBar.create() is DEPRECATED. Use Rho::NativeToolbar.create() or Rho::NativeTabbar.create().");
    create_native_toolbar(bar_type, p);
}

void remove_nativebar() 
{
    RAWLOGC_INFO("NativeBar", "NativeBar.remove() is DEPRECATED API ! Please use Rho::NativeToolbar.remove() or Rho::NativeTabbar.remove().");
    remove_native_toolbar();
}

VALUE nativebar_started() 
{
    bool bStarted = CNativeToolbar::getInstance().isStarted();
    return rho_ruby_create_boolean(bStarted?1:0);
}

//NavBar - iphone only
void create_navbar(rho_param *p)
{
    //TODO: create_navbar
}

void remove_navbar()
{
    //TODO: remove_navbar
}

VALUE navbar_started()
{
    //TODO: navbar_started
    return rho_ruby_create_boolean(0);
}

} //extern "C"
