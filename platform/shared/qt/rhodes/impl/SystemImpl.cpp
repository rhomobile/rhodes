#include "common/RhoPort.h"
#include "ruby/ext/rho/rhoruby.h"
#include "common/RhoDefs.h"
#include "common/RhoFilePath.h"
#include "logging/RhoLog.h"
#undef null
#include <qglobal.h>
#include <QWebPage>
#include <QLocale>
#include <QDesktopServices>
#include "MainWindowImpl.h"

using namespace rho;
using namespace rho::common;

extern "C" {

VALUE phone_number()
{
    return rho_ruby_get_NIL();
}

static int has_camera()
{
    return 1;
}

static double get_screen_ppi_x()
{
    return CMainWindow::getInstance()->getLogicalDpiX();
}

static double get_screen_ppi_y()
{
    return CMainWindow::getInstance()->getLogicalDpiY();
}

VALUE rho_sys_get_locale()
{
    return rho_ruby_create_string(QLocale::system().name().left(2).toStdString().c_str());
}

int rho_sys_get_screen_width()
{
    return CMainWindow::getScreenWidth();
}

int rho_sys_get_screen_height()
{
    return CMainWindow::getScreenHeight();
}

int rho_sysimpl_get_property(char* szPropName, VALUE* resValue)
{
    if (strcasecmp("webview_framework",szPropName) == 0) {
        *resValue = rho_ruby_create_string("WEBKIT/" QTWEBKIT_VERSION_STR);
        return 1;
    }

    if (strcasecmp("main_window_closed",szPropName) == 0) {
        *resValue = rho_ruby_create_boolean(CMainWindow::mainWindowClosed);
        return 1;
    }

    if (strcasecmp("has_camera",szPropName) == 0) {
        *resValue = rho_ruby_create_boolean(has_camera());
        return 1;
    }

    if (strcasecmp("phone_number",szPropName) == 0) {
        *resValue = phone_number();
        return 1;
    }

    if (strcasecmp("ppi_x",szPropName) == 0) {
        *resValue = rho_ruby_create_double(get_screen_ppi_x());
        return 1;
    }

    if (strcasecmp("ppi_y",szPropName) == 0) {
        *resValue = rho_ruby_create_double(get_screen_ppi_y());
        return 1;
    }

    if (strcasecmp("locale",szPropName) == 0) {
        *resValue = rho_sys_get_locale();
        return 1;
    }

    if (strcasecmp("country",szPropName) == 0) {
        *resValue = rho_ruby_create_string(QLocale::system().name().right(2).toStdString().c_str());
        return 1;
    }

    if (strcasecmp("device_name",szPropName) == 0) {
        *resValue = rho_ruby_create_string("Qt");
        return 1;
    }

    if (strcasecmp("is_emulator",szPropName) == 0) {
        *resValue = rho_ruby_create_boolean(1);
        return 1;
    }

    if (strcasecmp("has_calendar",szPropName) == 0) {
        *resValue = rho_ruby_create_boolean(1);
        return 1;
    }

    if (strcasecmp("has_touchscreen",szPropName) == 0) {
        *resValue = rho_ruby_create_boolean(1);
        return 1;
    }

    if (strcasecmp("screen_orientation",szPropName) == 0) {
        if (rho_sys_get_screen_width() <= rho_sys_get_screen_height()) 
        {
            *resValue = rho_ruby_create_string("portrait");
        }
        else {
            *resValue = rho_ruby_create_string("landscape");
        }                                                          
        return 1;
    }

    return 0;
}

VALUE rho_sys_makephonecall(const char* callname, int nparams, char** param_names, char** param_values) 
{
    return rho_ruby_get_NIL();
}

static int g_rho_has_network = 1;

void rho_sysimpl_sethas_network(int nValue)
{
    g_rho_has_network = nValue;
}

VALUE rho_sys_has_network()
{
    return rho_ruby_create_boolean(g_rho_has_network!=0);
}

void rho_sys_app_exit()
{
    CMainWindow::getInstance()->exitCommand();
}

void rho_sys_open_url(const char* url)
{
    QString sUrl = QString::fromUtf8(url);
    if (sUrl.startsWith("/")) sUrl.prepend("file://");
    QDesktopServices::openUrl(QUrl(sUrl));
}

void rho_sys_run_app(const char *appname, VALUE params)
{
    //TODO: rho_sys_run_app
    RAWLOGC_INFO("SystemImpl", "rho_sys_run_app() has no implementation in RhoSimulator.");
}

void rho_sys_bring_to_front()
{
    // TODO: rho_sys_bring_to_front
    return CMainWindow::getInstance()->bringToFront();
}

void rho_sys_report_app_started()
{
    // TODO: rho_sys_report_app_started
    RAWLOGC_INFO("SystemImpl", "rho_sys_report_app_started() has no implementation in RhoSimulator.");
}

int rho_sys_is_app_installed(const char *appname)
{
    int nRet = 0;
    //TODO: rho_sys_is_app_installed
    RAWLOGC_INFO("SystemImpl", "rho_sys_is_app_installed() has no implementation in RhoSimulator.");
    return nRet;
}

void rho_sys_app_install(const char *url)
{
    rho_sys_open_url(url);
}

void rho_sys_app_uninstall(const char *appname)
{
    //TODO: rho_sys_app_uninstall
    RAWLOGC_INFO("SystemImpl", "rho_sys_app_uninstall() has no implementation in RhoSimulator.");
}

#if !defined(OS_WINDOWS) && !defined(OS_SYMBIAN)
int rho_sys_set_sleeping(int sleeping)
{
    //TODO: rho_sys_set_sleeping
    return 1;
}
#endif

} //extern "C"
