#include <auroraapp.h>
#include <QObject>
#include <QDebug>
#include <QFileInfo>
#include <QDirIterator>
#include <QAbstractListModel>
#include <QtQuick>
#include "common/RhoPort.h"
#include "ruby/ext/rho/rhoruby.h"
#include "logging/RhoLog.h"
#include "common/RhoConf.h"
#include "common/RhodesApp.h"
#include "common/rhoparams.h"
#include "common/StringConverter.h"
#include "sync/RhoconnectClientManager.h"
#include "common/RhoFilePath.h"
#include <qglobal.h>
#include <QDir>
#include "impl/MainWindowImpl.h"
#include "QtMainWindow.h"
#include "QtLogView.h"
#ifndef ENABLE_Q_WEB_ENGINE
#include <QWebSettings>
#else
#include <QtWebEngine/QtWebEngine>
#endif
#include "../../platform/shared/qt/rhodes/RhoSimulator.h"

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

namespace rho
{
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
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling); // DPI support
    QGuiApplication::setAttribute(Qt::AA_UseHighDpiPixmaps); //HiDPI pixmaps
#ifdef ENABLE_Q_WEB_ENGINE
    QString debugAddress;
    foreach (const QHostAddress &address, QNetworkInterface::allAddresses()) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress(QHostAddress::LocalHost) && debugAddress.isEmpty())
        debugAddress = address.toString();
    }
    if (!debugAddress.isEmpty()){debugAddress.append(":");}
    debugAddress.append("9090");
    qputenv("QTWEBENGINE_REMOTE_DEBUGGING", debugAddress.toLocal8Bit());
#endif

    QScopedPointer<QGuiApplication> pApplication(Aurora::Application::application(argc, argv));
    QGuiApplication * application = const_cast<QGuiApplication *>(pApplication.data());
    qRegisterMetaType<QString>("QString");
    qRegisterMetaType<rho::apiGenerator::CMethodResult>("rho::apiGenerator::CMethodResult");
    qmlRegisterUncreatableType<RootDelegate>("RootDelegate",1,0,"RootDelegate","Err");

    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
        QString OSDetailsString= QString("Running on : %1 Application Compiled with QT Version :  %2 Running with QT Version %3")
    .arg(QtLogView::getOsDetails().toStdString().c_str(),QT_VERSION_STR,qVersion());

    #ifdef ENABLE_Q_WEB_ENGINE
    QtWebEngine::initialize();
    QThread::currentThread()->setPriority(QThread::TimeCriticalPriority);
    #endif

    qDebug() << "Executable file: " + QString::fromLocal8Bit(argv[0]);
    QScopedPointer<QQuickView> pView(Aurora::Application::createView());
    QQuickView * view = const_cast<QQuickView * >(pView.data());
    RootDelegate::getInstance(view->rootContext()->engine())->moveToThread(view->rootContext()->engine()->thread());
    view->rootContext()->engine()->thread()->setPriority(QThread::TimeCriticalPriority);

    #ifndef ENABLE_Q_WEB_ENGINE
    /*QWebSettings::globalSettings( )->setAttribute( QWebSettings::PrivateBrowsingEnabled, true );
    QWebSettings::globalSettings( )->setAttribute( QWebSettings::LocalContentCanAccessRemoteUrls, true );
    QWebSettings::globalSettings( )->setAttribute( QWebSettings::OfflineWebApplicationCacheEnabled, true );
    QWebSettings::setMaximumPagesInCache( 0 );
    QWebSettings::setObjectCacheCapacities( 0, 0, 0 );
    QWebSettings::clearMemoryCaches( );*/
    #endif
    QtMainWindow::setView(view);
    CMainWindow* m_appWindow = CMainWindow::getInstance();

    // Create the main application window
    QObject::connect(view, &QQuickView::activeChanged, [=](){qDebug() << (view->isActive()?"Active":"Not active");});
    QObject::connect(view->engine(), &QQmlEngine::quit, application, &QGuiApplication::quit);
    qDebug() << "Main path to QML: " + Aurora::Application::pathToMainQml().toString();

    qDebug() << "Writable location is: " + QStandardPaths::writableLocation(QStandardPaths::DataLocation);
    const QByteArray dir = QStandardPaths::writableLocation(QStandardPaths::DataLocation).toLatin1();
    m_strRootPath = std::string(dir.constData(), dir.length());
    m_strRootPath += "/rho/";
    qDebug() << "Main directory is: " + QString::fromStdString(m_strRootPath);
    QString dataDirectory("/usr/share/" + application->applicationName() + "/data/rho/");

    //if (!QtMainWindow::isFilesEqual(dataDirectory + "RhoBundleMap.txt",  QString::fromStdString(m_strRootPath) + "RhoBundleMap.txt")){
        //QDir dirToDelete(QString::fromStdString(m_strRootPath));
        //dirToDelete.removeRecursively();
        QtMainWindow::copyDirRecursive(dataDirectory, QString::fromStdString(m_strRootPath));
    //}

    QtMainWindow::setWritableDir(QString::fromStdString(m_strRootPath));
    QDir::setCurrent(QString::fromStdString(m_strRootPath));

    rho_logconf_Init(m_strRootPath.c_str(), m_strRootPath.c_str(), m_logPort.c_str());
    LOGCONF().setLogToOutput(true);
    LOGCONF().setLogToFile(true);

    if ( !rho_rhodesapp_canstartapp(g_strCmdLine.c_str(), " /-,") )
    {
        //QMessageBox::critical(0,QString("This is hidden app and can be started only with security key."), QString("Security Token Verification Failed"));
        //TODO: create message boxex

        RAWLOGC_INFO("QTMain", "This is hidden app and can be started only with security key.");
        if (RHOCONF().getString("invalid_security_token_start_path").length() <= 0)
            return 1;
    }
    RAWLOGC_INFO("QTMain" ,OSDetailsString.toStdString().c_str());
    //Identify QT is hardcoded path . hardcoded by compiler paths will be displayed.
    for(int nCounter = 0; nCounter < QCoreApplication::libraryPaths().size(); nCounter ++)
    {
         QString path = QCoreApplication::libraryPaths().at(nCounter);
         RAWLOGC_INFO1("QTMain", "%d Paths",nCounter);
         RAWLOGC_INFO("QTMain", path.toStdString().c_str());
    }

    RAWLOGC_INFO("QTMain", "Rhodes started");

    rho::common::CRhodesApp::Create(m_strRootPath, m_strRootPath, m_strRootPath);

    RHODESAPP().setJSApplication(m_isJSApplication);

    m_appWindow->Initialize(convertToStringW(RHODESAPP().getAppTitle()).c_str());
    view->rootContext()->setContextProperty("rootDelegate", RootDelegate::getInstance());
    view->setSource(Aurora::Application::pathToMainQml());
    view->showFullScreen();
    RHODESAPP().startApp();
    //QTimer::singleShot(1000, ()[&]{rho_rhodesapp_callUiCreatedCallback();});
    rho_rhodesapp_callUiCreatedCallback();

    if (RHOCONF().getString("test_push_client").length() > 0 )
        rho_clientregister_create(RHOCONF().getString("test_push_client").c_str());//"qt_client");

#ifdef ENABLE_Q_WEB_ENGINE
    debugAddress.prepend("Debug address is ");
    RAWLOGC_INFO("QTMain", debugAddress.toStdString().c_str());
#endif

    application->exec();

    QtMainWindow::doExit(false);

    return  0;
}


