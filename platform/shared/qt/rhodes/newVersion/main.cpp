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

#include <string>
#include "common/RhoPort.h"
#include "ruby/ext/rho/rhoruby.h"
#include "logging/RhoLog.h"
#include "common/RhoConf.h"
#include "common/RhodesApp.h"
#include "common/rhoparams.h"
#include "common/StringConverter.h"
#include "sync/RhoconnectClientManager.h"
#include "common/RhoFilePath.h"
#undef null
#include <qglobal.h>
#include <QApplication>
#include <QMessageBox>
#include <QDir>
#include "impl/MainWindowImpl.h"
#include "QtMainWindow.h"
#include "QtLogView.h"

#include "../../platform/shared/qt/rhodes/RhoSimulator.h"
#ifdef OS_WINDOWS_DESKTOP
#include "AppRunningFlag.h"
#endif

using namespace rho;
using namespace rho::common;
using namespace std;

static String g_strCmdLine;

static bool m_isJSApplication = false;
static String m_strRootPath, m_strRhodesPath, m_logPort;
static String m_strHttpProxy;

extern void parseHttpProxyURI(const String &http_proxy);
extern "C" {
    void rho_ringtone_manager_stop();

    const char* rho_native_rhopath() 
    {
        return m_strRootPath.c_str();
    }
}


namespace rho{
    //TODO: include intents to Win RhoSIm
    void waitIntentEvent(const rho::String& appName){}
}

char* parseToken(const char* start)
{
    int len = strlen(start);
    int nNameLen = 0;
    while (*start==' ') { start++; len--; }

    int i = 0;
    for (i = 0; i < len; i++) {
        if (start[i] == '=') {
            if (i > 0) {
                int s = i-1;
                for (; s >= 0 && start[s]==' '; s--);
                nNameLen = s+1;
                break;
            } else
                break;
        }
    }

    if ( nNameLen == 0 )
        return NULL;

    const char* szValue = start + i+1;
    int nValueLen = 0;

    while (*szValue==' ' || *szValue=='\'' || *szValue=='"' && nValueLen >= 0) { szValue++; }
    while (szValue[nValueLen] && szValue[nValueLen] !='\'' && szValue[nValueLen] != '"') { nValueLen++; }

    //while (nValueLen > 0 && (szValue[nValueLen-1]==' ' || szValue[nValueLen-1]=='\'' || szValue[nValueLen-1]=='"')) nValueLen--;

    char* value = (char*) malloc(nValueLen+2);
    strncpy(value, szValue, nValueLen);
    value[nValueLen] = '\0';

    return value;
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
#ifdef RHODES_MAC_BUILD
    qputenv("QTWEBENGINE_REMOTE_DEBUGGING", QString::number(QtMainWindow::getDebPort()).toLocal8Bit());
#endif
#if QT_VERSION < 0x060000
    QtWebEngine::initialize();
#endif
    qRegisterMetaType<QTextCursor>("QTextCursor");
    qRegisterMetaType<QString>("QString");
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QString OSDetailsString= QString("Running on : %1 Application Compiled with QT Version :  %2 Running with QT Version %3")
    .arg(QtLogView::getOsDetails().toStdString().c_str(),QT_VERSION_STR,qVersion());
#ifdef RHODES_EMULATOR
    bool isJSApp = false;
#endif
    CMainWindow* m_appWindow = CMainWindow::getInstance();

    m_logPort = String("11000");
    for (int i=1; i<argc; ++i) {
        g_strCmdLine += String(argv[i]) + " ";
        if (strncasecmp("-log",argv[i],4)==0) {
            char* port = parseToken(argv[i]);
            if (port) {
                    String strLogPort = port;
                    m_logPort = strLogPort;
                    free(port);
            }
        } else if (strncasecmp("-http_proxy_url",argv[i],15)==0) {
            char *proxy = parseToken(argv[i]);
            if (proxy) {
                m_strHttpProxy = proxy;
                free(proxy);
            } else
                RAWLOGC_INFO("QTMain", "invalid value for \"http_proxy_url\" cmd parameter");
        } else if (strncasecmp("-http_proxy_uri",argv[i],15)==0) {
            char *proxy = parseToken(argv[i]);
            if (proxy) {
                m_strHttpProxy = proxy;
                free(proxy);
            } else
                RAWLOGC_INFO("QTMain", "invalid value for \"http_proxy_uri\" cmd parameter");
#ifdef RHODES_EMULATOR
        } else if ((strncasecmp("-approot",argv[i],8)==0) || (isJSApp = (strncasecmp("-jsapproot",argv[i],10)==0))) {
            char* path = parseToken(argv[i]);
            if (path) {
                int len = strlen(path);
                if (!(path[len-1]=='\\' || path[len-1]=='/')) {
                    path[len] = '/';
                    path[len+1]  = 0;
                }
                m_strRootPath = path;
                free(path);
                if (m_strRootPath.substr(0,7).compare("file://")==0)
                    m_strRootPath.erase(0,7);
                String_replace(m_strRootPath, '\\', '/');
            }
            m_isJSApplication = isJSApp;
        } else if (strncasecmp("-rhodespath",argv[i],11)==0) {
            char* path = parseToken(argv[i]);
            if (path) {
                m_strRhodesPath = path;
                free(path);
                if (m_strRhodesPath.substr(0,7).compare("file://")==0)
                    m_strRhodesPath.erase(0,7);
                String_replace(m_strRhodesPath, '\\', '/');
            }
#endif
        } else {
            RAWLOGC_INFO1("QTMain", "wrong cmd parameter: %s", argv[i]);
        }
    }

#if defined(RHO_SYMBIAN)
    m_strRootPath = (QDir::currentPath()+"/").toUtf8().data();
#endif

#ifndef RHODES_EMULATOR
    const QByteArray dir = QFileInfo(QCoreApplication::applicationFilePath()).absolutePath().toLatin1();
    m_strRootPath = std::string(dir.constData(), dir.length());
    m_strRootPath += "/rho/";
#endif

    // PreMessageLoop:
    rho_logconf_Init(m_strRootPath.c_str(), m_strRootPath.c_str(), m_logPort.c_str());
#ifdef RHODES_EMULATOR
    RHOSIMCONF().setAppConfFilePath(CFilePath::join(m_strRootPath, RHO_EMULATOR_DIR"/rhosimconfig.txt").c_str());
    RHOSIMCONF().loadFromFile();
    if ( m_strRhodesPath.length() > 0 )
        RHOSIMCONF().setString("rhodes_path", m_strRhodesPath, false );
    RHOCONF().setString( "rhosim_platform", RHOSIMCONF().getString( "platform"), false);
    RHOCONF().setString( "app_version", RHOSIMCONF().getString( "app_version"), false);
    RHOSIMCONF().setString("os_version",String(RHOSIMULATOR_VERSION),false);
    String start_path = RHOSIMCONF().getString("start_path");
    if ( start_path.length() > 0 )
        RHOCONF().setString("start_path", start_path, false);
    RHOSIMCONF().setString( "ext_path", RHOSIMCONF().getString( "ext_path") + CFilePath::join( m_strRhodesPath, "/lib/extensions/debugger;"), false);
    RHOSIMCONF().setString( "ext_path", RHOSIMCONF().getString( "ext_path") + CFilePath::join( m_strRhodesPath, "/lib/extensions/uri;"), false);
    RHOSIMCONF().setString( "ext_path", RHOSIMCONF().getString( "ext_path") + CFilePath::join( m_strRhodesPath, "/lib/extensions/timeout;"), false);
    RHOSIMCONF().setString( "ext_path", RHOSIMCONF().getString( "ext_path") + CFilePath::join( m_strRhodesPath, "/lib/extensions/digest;"), false);
    RHOSIMCONF().setString( "ext_path", RHOSIMCONF().getString( "ext_path") + CFilePath::join( m_strRhodesPath, "/lib/extensions/openssl;"), false);
#endif

    if ( !rho_rhodesapp_canstartapp(g_strCmdLine.c_str(), " /-,") )
    {
        QMessageBox::critical(0,QString("This is hidden app and can be started only with security key."), QString("Security Token Verification Failed"));
        RAWLOGC_INFO("QTMain", "This is hidden app and can be started only with security key.");
        if (RHOCONF().getString("invalid_security_token_start_path").length() <= 0)
            return 1;
    }
    RAWLOGC_INFO("QTMain" ,OSDetailsString.toStdString().c_str());
    //Identify QT is hardcoded path . hardcoded by compiler paths will be displayed.
    for(int nCounter = 0; nCounter <QCoreApplication::libraryPaths().size(); nCounter ++)
    {
         QString path = QCoreApplication::libraryPaths().at(nCounter);
         RAWLOGC_INFO1("QTMain", "%d Paths",nCounter);
         RAWLOGC_INFO("QTMain", path.toStdString().c_str());
    }

    RAWLOGC_INFO("QTMain", "Rhodes started");
    if (m_strHttpProxy.length() > 0) {
        parseHttpProxyURI(m_strHttpProxy);
    } else {
        if (RHOCONF().isExist("http_proxy_url")) {
            parseHttpProxyURI(RHOCONF().getString("http_proxy_url"));
        } else if (RHOCONF().isExist("http_proxy_uri")) {
            parseHttpProxyURI(RHOCONF().getString("http_proxy_uri"));
        }
    }

#ifdef RHODES_EMULATOR
    if (RHOSIMCONF().getString("debug_host").length() > 0)
#ifdef OS_WINDOWS_DESKTOP
        SetEnvironmentVariableA("RHOHOST", RHOSIMCONF().getString("debug_host").c_str() );
#else // !OS_WINDOWS_DESKTOP
        setenv("RHOHOST", RHOSIMCONF().getString("debug_host").c_str(), 1 );
#endif // OS_WINDOWS_DESKTOP
    if (RHOSIMCONF().getString("debug_port").length() > 0)
#ifdef OS_WINDOWS_DESKTOP
        SetEnvironmentVariableA("rho_debug_port", RHOSIMCONF().getString("debug_port").c_str() );
#else // RHODES_EMULATOR
        setenv("rho_debug_port", RHOSIMCONF().getString("debug_port").c_str(), 1 );
#endif // OS_WINDOWS_DESKTOP
#endif // RHODES_EMULATOR
    rho::common::CRhodesApp::Create(m_strRootPath, m_strRootPath, m_strRootPath);

    RHODESAPP().setJSApplication(m_isJSApplication);

    // Create the main application window
#ifdef RHODES_EMULATOR
    m_appWindow->Initialize(convertToStringW(RHOSIMCONF().getString("app_name")).c_str());
#else
    m_appWindow->Initialize(convertToStringW(RHODESAPP().getAppTitle()).c_str());
#endif

#ifdef OS_WINDOWS_DESKTOP
    AppRunningFlag appRunning(QString::fromStdString(RHODESAPP().getAppName()));
#endif

    RHODESAPP().startApp();

    // Navigate to the "loading..." page
    //m_appWindow->navigate(L"about:blank", -1);

    if (RHOCONF().getString("test_push_client").length() > 0 )
        rho_clientregister_create(RHOCONF().getString("test_push_client").c_str());//"qt_client");

    RAWLOGC_INFO("QTMain argv", QString::number(argc).toStdString().c_str());
    for(int i = 0; i < argc; i++){
        RAWLOGC_INFO("QTMain args", argv[i]);
    }

    // RunMessageLoop:
    m_appWindow->messageLoop();
    app.exec();

    // stopping Rhodes application
    //rho_ringtone_manager_stop(); not implemented.
    #ifdef OS_SAILFISH
        m_appWindow->DestroyUi();
        rho::common::CRhodesApp::Destroy();
    #endif
    return 0;
}


