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
#include "ruby/ext/rho/rhoruby.h"
#include "common/RhoDefs.h"
#include "common/RhoFilePath.h"
#include "logging/RhoLog.h"
#undef null

#ifndef RHODES_VERSION_LIBRARY
#include <qglobal.h>

#include <QLocale>
#include <QDesktopServices>
#ifndef OS_SAILFISH
#include <QMessageBox>
#endif
#include "MainWindowImpl.h"
#include <QUrl>
#ifdef RHODES_VERSION_2
#if QT_VERSION < 0x060000
#include <QtWebEngineVersion>
#endif
#endif
#ifdef RHODES_VERSION_1
#include <QWebPage>
#include <QtWebKit/QWebElement>
#if QT_VERSION >= 0x050000
#include <QtWebKit/qtwebkitversion.h>
#else
#include <QtWebKit/qwebkitversion.h>
#endif
#endif
#endif
#include "../RhoSimulator.h"

#ifdef RHODES_VERSION_LIBRARY
#include "common/RhodesApp.h"
#endif
using namespace rho;
using namespace rho::common;

static char qt_webview_framework[32] = "";

extern "C" {

const char* rho_sys_qt_getWebviewFramework()
{
#if !defined(OS_SAILFISH) && !defined(RHODES_VERSION_LIBRARY)
    if (qt_webview_framework[0] == '\0') {

        #if defined(RHODES_VERSION_1)
        const QByteArray ver = QString("WEBKIT/").append(qWebKitVersion()).toLatin1();
        #endif

        #if defined(RHODES_VERSION_2) && QT_VERSION < 0x060000
        const QByteArray ver = QString("WEBENGINE/").append(QTWEBENGINE_VERSION_STR).toLatin1();
        #else
        const QByteArray ver = "NONE";
        #endif
        
        if (ver.length() < 32) {
            strncpy(qt_webview_framework, ver.constData(), ver.length());
            qt_webview_framework[ver.length()] = '\0';
        }
    }
#endif
    return qt_webview_framework;
}

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
#ifndef RHODES_VERSION_LIBRARY
    return CMainWindow::getInstance()->getLogicalDpiX();
#else
    return 0.0;
#endif
}

static double get_screen_ppi_y()
{
#ifndef RHODES_VERSION_LIBRARY
    return CMainWindow::getInstance()->getLogicalDpiY();
#else
    return 0.0;
#endif
}

VALUE rho_sys_get_locale()
{
#ifndef RHODES_VERSION_LIBRARY
    return rho_ruby_create_string(QLocale::system().name().left(2).toStdString().c_str());
#else
    return rho_ruby_get_NIL();
#endif
}

int rho_sys_get_screen_width()
{
#ifndef RHODES_VERSION_LIBRARY
    return CMainWindow::getScreenWidth();
#else
    return -1;
#endif
}

int rho_sys_get_screen_height()
{
#ifndef RHODES_VERSION_LIBRARY
    return CMainWindow::getScreenHeight();
#else
    return -1;
#endif
}

int rho_sysimpl_get_property(char* szPropName, VALUE* resValue)
{
    if (strcasecmp("webview_framework",szPropName) == 0) {
        *resValue = rho_ruby_create_string(rho_sys_qt_getWebviewFramework());
        return 1;
    }

#ifndef RHODES_VERSION_LIBRARY
    if (strcasecmp("main_window_closed",szPropName) == 0) {
        *resValue = rho_ruby_create_boolean(CMainWindow::mainWindowClosed);
        return 1;
    }
#endif

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

#ifndef RHODES_VERSION_LIBRARY
    if (strcasecmp("country",szPropName) == 0) {
        *resValue = rho_ruby_create_string(QLocale::system().name().right(2).toStdString().c_str());
        return 1;
    }
#endif

    if (strcasecmp("device_name",szPropName) == 0) {
        *resValue = rho_ruby_create_string("Qt");
        return 1;
    }

#if defined(__SYMBIAN32__)
#elif defined(RHO_SYMBIAN)
    if (strcasecmp("platform",szPropName) == 0) {
        *resValue = rho_ruby_create_string("Qt Emulator");
        return 1;
    }

     if (strcasecmp("os_version",szPropName) == 0) {
        *resValue = rho_ruby_create_string("");
        return 1;
    }
#endif

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

int rho_sysimpl_has_network();

VALUE rho_sys_has_network()
{
    return rho_ruby_create_boolean(rho_sysimpl_has_network()!=0);
}

void rho_sys_app_exit()
{
#ifndef RHODES_VERSION_LIBRARY
    CMainWindow::getInstance()->exitCommand();
#else
    exit(0);
#endif
}

void rho_sys_open_url(const char* url)
{
#ifndef RHODES_VERSION_LIBRARY
    QString sUrl = QString::fromUtf8(url);
    if (sUrl.startsWith("/")) sUrl.prepend("file://");
    QDesktopServices::openUrl(QUrl(sUrl));
#endif
}

void rho_sys_run_app(const char *appname, VALUE params)
{
    //TODO: rho_sys_run_app
    RAWLOGC_INFO("SystemImpl", "rho_sys_run_app() has no implementation " RHO_PLATFORM_NAME_AT ".");
}

void rho_sys_bring_to_front()
{
#ifndef RHODES_VERSION_LIBRARY
    // TODO: rho_sys_bring_to_front
    return CMainWindow::getInstance()->bringToFront();
#endif
}

void rho_sys_report_app_started()
{
    // TODO: rho_sys_report_app_started
    //RAWLOGC_INFO("SystemImpl", "rho_sys_report_app_started() has no implementation " RHO_PLATFORM_NAME_AT ".");
#if defined(WINDOWS_PLATFORM)
    RHODESAPP().notifyAppStared();
#endif

}

int rho_sys_is_app_installed(const char *appname)
{
    int nRet = 0;
    //TODO: rho_sys_is_app_installed
    RAWLOGC_INFO("SystemImpl", "rho_sys_is_app_installed() has no implementation " RHO_PLATFORM_NAME_AT ".");
    return nRet;
}

void rho_sys_app_install(const char *url)
{
    rho_sys_open_url(url);
}

void rho_sys_app_uninstall(const char *appname)
{
    //TODO: rho_sys_app_uninstall
    RAWLOGC_INFO("SystemImpl", "rho_sys_app_uninstall() has no implementation " RHO_PLATFORM_NAME_AT ".");
}

#if !defined(OS_WINDOWS_DESKTOP) && !defined(OS_SYMBIAN)
int rho_sys_set_sleeping(int sleeping)
{
    //TODO: rho_sys_set_sleeping
    return 1;
}
#endif

void rho_sys_set_application_icon_badge(int badge_number)
{
    //TODO: rho_sys_set_application_icon_badge
    RAWLOGC_INFO1("SystemImpl", "rho_sys_set_application_icon_badge(%d) was called", badge_number);
}

void rho_sys_impl_exit_with_errormessage(const char* szTitle, const char* szMsg)
{
#if !defined(OS_SAILFISH) && !defined (RHODES_VERSION_LIBRARY)
    QMessageBox::critical(0, QString(szTitle), QString(szMsg));
#endif
}

RHO_GLOBAL void rho_platform_restart_application()
{
    //TODO: rho_platform_restart_application
    RAWLOGC_INFO("SystemImpl", "rho_platform_restart_application() has no implementation " RHO_PLATFORM_NAME_AT ".");
}

const char* rho_native_reruntimepath()
{
    return rho_native_rhopath();
}

int SymbolLicence_check(const char* company, const char* licence)
{
    return 1;
}

void rho_sys_set_window_frame(int x0, int y0, int width, int height)
{
#ifndef RHODES_VERSION_LIBRARY
    CMainWindow::getInstance()->setFrame(x0, y0, width, height);
#endif
}

void rho_sys_set_window_position(int x0, int y0)
{
#ifndef RHODES_VERSION_LIBRARY
    CMainWindow::getInstance()->setPosition(x0, y0);
#endif
}

void rho_sys_set_window_size(int width, int height)
{
#ifndef RHODES_VERSION_LIBRARY
    CMainWindow::getInstance()->setSize(width, height);
#endif
}

void rho_sys_lock_window_size(int locked)
{
#ifndef RHODES_VERSION_LIBRARY
    CMainWindow::getInstance()->lockSize(locked);
#endif
}

void rho_symimpl_createMenu()
{
#ifndef RHODES_VERSION_LIBRARY
    CMainWindow::getInstance()->createCustomMenuCommand();
#endif
}

} //extern "C"
