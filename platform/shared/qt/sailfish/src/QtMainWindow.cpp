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


#ifdef _MSC_VER
#pragma warning(disable:4018)
#pragma warning(disable:4996)
#endif
#include "QtMainWindow.h"
#include <sstream>
#include "ext/rho/rhoruby.h"
#include "common/RhoStd.h"
#include "common/RhodesApp.h"
#include "common/RhoConf.h"
#include "common/RhoSimConf.h"
#include "rubyext/WebView.h"
#include "rubyext/NativeToolbarExt.h"
#include "RhoNativeApiCall.h"
#include "statistic/RhoProfiler.h"
#include <QtNetwork/QNetworkCookie>
#include <QDesktopServices>
#include <auroraapp.h>
#include "impl/MainWindowImpl.h"
#include <QCameraInfo>
#include <QCryptographicHash>

#if defined(OS_MACOSX) || defined(OS_LINUX)
#define stricmp strcasecmp
#define strnicmp strncasecmp
#endif

#include "guithreadfunchelper.h"

IMPLEMENT_LOGCLASS(QtMainWindow,"QtMainWindow");

extern "C" {
    extern VALUE rb_thread_main(void);
    extern VALUE rb_thread_wakeup(VALUE thread);
}
using namespace rho;
using namespace rho::common;

QtMainWindow * QtMainWindow::lastInstance = nullptr;
QQuickView * QtMainWindow::view = nullptr;
QString QtMainWindow::writableDir = "";


QString QtMainWindow::getMainWindowTitle() const
{
    return mainWindowTitle;
}

void QtMainWindow::setMainWindowTitle(const QString &value)
{
    mainWindowTitle = value;
}

QString QtMainWindow::getUsingDeviceID() const
{
    return usingDeviceID;
}

void QtMainWindow::setUsingDeviceID(const QString &value)
{
    usingDeviceID = value;
    emit usingDeviceIDChanged();
}

QString QtMainWindow::getTargetFilePath() const
{
    return targetFilePath;
}

void QtMainWindow::setTargetFilePath(const QString &value)
{
    targetFilePath = value;
    emit targetFilePathChanged();
}


QtMainWindow::QtMainWindow(QObject *parent) : QObject(parent),
    mainWindowCallback(NULL),
    cur_tbrp(0),
    firstShow(true), m_bFirstLoad(true),
    toolBarSeparatorWidth(0),
    m_proxy(QNetworkProxy(QNetworkProxy::DefaultProxy))
{
    logicalDpiX = 0;
    logicalDpiY = 0;
    m_logView = nullptr;
    QPixmap icon(QCoreApplication::applicationDirPath().append(QDir::separator()).append("icon.png"));
    QGuiApplication::setApplicationDisplayName(QString::fromStdString(RHOCONF().getString("title_text")));
    QGuiApplication::setApplicationName(QString::fromStdString(RHOCONF().getString("app_name")));
    QGuiApplication::setApplicationVersion(QString::fromStdString(RHOCONF().getString("app_version")));
    QGuiApplication::setOrganizationName(QString::fromStdString(RHOCONF().getString("org_name")));

    //QWebSettings* qs = QWebSettings::globalSettings();
    //qs->setAttribute(QWebSettings::DeveloperExtrasEnabled, true);
    //qs->setOfflineStorageDefaultQuota(1024*1024*1024);
    //rho::String rs_dir = RHODESAPP().getRhoRootPath()+RHO_EMULATOR_DIR;
    //qs->enablePersistentStorage(rs_dir.c_str());



    GuiThreadFuncHelper::getInstance(this);
    connect(QGuiApplication::instance(), SIGNAL(aboutToQuit()), this, SLOT(closeEvent()));
    connect(this, SIGNAL(logicalDpiXChanged()), this, SLOT(resizeEvent()));
    connect(this, SIGNAL(logicalDpiYChanged()), this, SLOT(resizeEvent()));
    connect(this, SIGNAL(rotationChanged()), this, SLOT(resizeEvent()));
    connect(this, &QtMainWindow::rotationChanged, [&](){RHODESAPP().callScreenRotationCallback(getLogicalDpiX(), getLogicalDpiY(), getRotation());});

    connect(this, SIGNAL(setCover(QString)), RootDelegate::getInstance(), SLOT(setCover(QString)), Qt::QueuedConnection);

    if (RHOCONF().isExist("http_proxy_host"))
    {
        setProxy(QString::fromStdString(RHOCONF().getString("http_proxy_host")),
                 QString::fromStdString(RHOCONF().getString("http_proxy_port")),
                 QString::fromStdString(RHOCONF().getString("http_proxy_login")),
                 QString::fromStdString(RHOCONF().getString("http_proxy_password")));
    } else {
        unsetProxy();
    }

    lastInstance = this;

    webViewsList.append(new CustomWebViewTab("MainWebView", "about:blanc", this));

    QObject::connect(webViewsList.first(), SIGNAL(linkClicked(const QString&)), this, SLOT(on_webView_linkClicked(const QString&)));
    QObject::connect(webViewsList.first(), SIGNAL(loadStarted()), this, SLOT(on_webView_loadStarted()));
    QObject::connect(webViewsList.first(), SIGNAL(loadFinished(bool)), this, SLOT(on_webView_loadFinished(bool)));
    QObject::connect(webViewsList.first(), SIGNAL(urlHasBeenChanged(QString)), this, SLOT(on_webView_urlChanged(QString)));
    QObject::connect(webViewsList.first(), SIGNAL(msg(QString,QString)), this, SLOT(rawLog(QString,QString)), Qt::QueuedConnection);

    exitItem = new CustomMenuItem("Exit", this);
    connect(exitItem, SIGNAL(isClicked()), this, SLOT(exitCommand()));

    commitMenuItemsList();
    commitToolBarButtonsList();
    commitWebViewsList();

    QFileInfo loadingPageInfo(QString::fromStdString(CRhodesApp::getInstance()->getLoadingPngPath()));
    if (loadingPageInfo.exists()){
        webViewsList.first()->setUrl(QString::fromStdString(CRhodesApp::getInstance()->getLoadingPagePath()));
    }
    QFileInfo pngInfo(QString::fromStdString(CRhodesApp::getInstance()->getLoadingPngPath()));
    if (pngInfo.exists()){
        setCover(QString::fromStdString(CRhodesApp::getInstance()->getLoadingPngPath()));
        QTimer::singleShot(30000, [&](){emit this->setCover("");});
    }

    //qDebug() << "Available cameras: " + QString::number(QCameraInfo::availableCameras().size());
    QTimer::singleShot(500, [&](){this->showEvent();});

    qDebug() << "End of main window cunstruction";
}

QtMainWindow::~QtMainWindow()
{
    tabbarRemoveAllTabs(false);
    //if (m_alertDialog) delete m_alertDialog;
    //TODO: m_SyncStatusDlg
    LOGCONF().setLogView(NULL);
    if (m_logView){
        delete m_logView;
        m_logView = nullptr;
    }
}

void QtMainWindow::paintEvent()
{
    if ( m_bFirstLoad )
    {
        /*QPainter paint(this);
        QImage image(RHODESAPP().getLoadingPngPath().c_str());
        rcClient.setBottom( rcClient.bottom() - this->ui->toolBar->rect().height() ) ;
        RHODESAPP().getSplashScreen().start();
        paint.drawImage( rcClient, image );*/ //TODO: make splash screen
    }
}

void QtMainWindow::setCallback(IMainWindowCallback* callback)
{
    mainWindowCallback = callback;
}

void QtMainWindow::hideEvent()
{
    if (mainWindowCallback) mainWindowCallback->onActivate(0);
}

void QtMainWindow::showEvent()
{
    if (mainWindowCallback) mainWindowCallback->onActivate(1);
}

void QtMainWindow::closeEvent()
{
    if ( rho_ruby_is_started() ) rb_thread_wakeup(rb_thread_main());

    if (mainWindowCallback) mainWindowCallback->onWindowClose();
    tabbarRemoveAllTabs(false);
}

void QtMainWindow::resizeEvent()
{
    if (mainWindowCallback) mainWindowCallback->updateSizeProperties(getLogicalDpiY(), getLogicalDpiX());
    /*if (m_logView == 0) {
        m_logView = new QtLogView();
        LOGCONF().setLogView(m_logView);
    }*/
}

bool QtMainWindow::isStarted(void)
{
    return true;
}

void QtMainWindow::unsetProxy()
{
    m_proxy = QNetworkProxy(QNetworkProxy::DefaultProxy);
    internalSetProxy();
}

void QtMainWindow::setProxy(QString host, QString port, QString login, QString password)
{
    m_proxy = QNetworkProxy(QNetworkProxy::HttpProxy, host, port.toUInt(), login, password);
    internalSetProxy();
}

void QtMainWindow::internalSetProxy()
{
    QNetworkProxy::setApplicationProxy(m_proxy);
}

void QtMainWindow::setView(QQuickView *value)
{
    view = value;
}

void QtMainWindow::on_actionBack_triggered()
{
    RHODESAPP().navigateBack();
}
/*
void QtMainWindow::on_actionRotateRight_triggered()
{

	RHODESAPP().callScreenRotationCallback(this->width(), this->height(), 90);
}

void QtMainWindow::on_actionRotateLeft_triggered()
{

	RHODESAPP().callScreenRotationCallback(this->width(), this->height(), -90);
}

void QtMainWindow::on_actionRotate180_triggered()
{
	RHODESAPP().callScreenRotationCallback(this->width(), this->height(), 180);
}
*/
void QtMainWindow::doExit(bool wait){
    static bool finalize = true;
    if (finalize){
        if (wait){
            QTimer::singleShot(50, [=](){
                CMainWindow::getInstance()->diconnectFromUI();
                CMainWindow::getInstance()->DestroyUi();
                rho::common::CRhodesApp::Destroy();
                QGuiApplication::instance()->quit();
            }
            );
        }else{
            CMainWindow::getInstance()->diconnectFromUI();
            CMainWindow::getInstance()->DestroyUi();
            rho::common::CRhodesApp::Destroy();
            QGuiApplication::instance()->quit();
        }

        finalize = false;
    }

}

void QtMainWindow::on_actionExit_triggered()
{
    doExit();

}

bool QtMainWindow::internalUrlProcessing(const QUrl& url)
{
    int ipos;
    QString sUrl = url.toString();
    if (sUrl.startsWith("mailto:")) {
        QDesktopServices::openUrl(url);
        return true;
    }
    if (sUrl.startsWith("tel:")) {
        sUrl.remove(0, 4);
        if ((ipos = sUrl.indexOf('?')) >= 0) sUrl = sUrl.left(ipos);
        //QMessageBox::information(0, "Phone call", "Call to " + sUrl); IMPLEMENT THIS
        return true;
    }
    if (sUrl.startsWith("sms:")) {
        sUrl.remove(0, 4);
        if ((ipos = sUrl.indexOf('?')) >= 0) sUrl = sUrl.left(ipos);
        //QMessageBox::information(0, "SMS", "Send SMS to " + sUrl); IMPLEMENT THIS
        return true;
    }
    return false;
}

void QtMainWindow::on_webView_linkClicked(const QString& url)
{
    qDebug() << "linkClicked: " + url;
    QString sUrl = url;
    if (sUrl.contains("rho_open_target=_blank")) {
        LOG(INFO) + "WebView: open external browser";
        QDesktopServices::openUrl(QUrl(sUrl.remove("rho_open_target=_blank")));
        /*ExternalWebView * externalWebView = new ExternalWebView();
        externalWebView->navigate(QUrl(sUrl.remove("rho_open_target=_blank")));
        externalWebView->show();
        externalWebView->activateWindow();*/
    } else if (!webViewsList.isEmpty()) {
        if (!internalUrlProcessing(url)) {
            sUrl.remove(QRegExp("#+$"));
            if (sUrl.compare( ((CustomWebViewTab *)sender())->getUrl()) != 0) {
                if (mainWindowCallback && !sUrl.startsWith("javascript:", Qt::CaseInsensitive)) {
                    const QByteArray asc_url = sUrl.toLatin1();
                    mainWindowCallback->onWebViewUrlChanged(::std::string(asc_url.constData(), asc_url.length()));
                }
                QUrl url(sUrl);
                webViewsList.first()->loadUrl(url.toString());

            }
        }
    }
}

void QtMainWindow::on_webView_loadStarted()
{
	if (firstShow && RHOCONF().getBool("full_screen")) {
		firstShow = false;
		fullscreenCommand(1);
	}
    LOG(INFO) + "WebView: loading...";
    PROF_START("BROWSER_PAGE");
}

void QtMainWindow::on_webView_loadFinished(bool ok)
{
    CustomWebViewTab * senderTab = SENDER_TAB;
    if (ok)
        LOG(INFO) + "Page load complete.";
    else
    {
        LOG(ERROR) + "Page load failed.";
        senderTab->setHtml("<html><head><title>Error Loading Page</title></head><body><h1>Error Loading Page.</h1></body></html>");
    }

    PROF_STOP("BROWSER_PAGE");

    if (mainWindowCallback && ok) {
        const QByteArray asc_url = senderTab->getUrl().toLatin1();
        mainWindowCallback->onWebViewUrlChanged(::std::string(asc_url.constData(), asc_url.length()));
    }

    if ( m_bFirstLoad )
    {
        if ( senderTab->getUrl() != "about:blank" )
        {
            long lMS = RHODESAPP().getSplashScreen().howLongWaitMs();
            if ( lMS > 0 )
                m_SplashTimer.start(lMS, this);
            else
            {
                m_bFirstLoad = false;
                //main_webView->show();
            }
        }
    }

}

void QtMainWindow::on_webView_urlChanged(QString url)
{
    if (mainWindowCallback) {
        const QByteArray asc_url = url.toLatin1();
        ::std::string sUrl = ::std::string(asc_url.constData(), asc_url.length());
        LOG(INFO) + "WebView: URL changed to " + sUrl;
        mainWindowCallback->onWebViewUrlChanged(sUrl);
    }
}

void QtMainWindow::on_menuMain_aboutToShow()
{
    if (mainWindowCallback) mainWindowCallback->createCustomMenu();
}

void QtMainWindow::navigate(QString url, int index)
{
    if (webViewsList.size() <= ((index + 1) + 1)){
        CustomWebViewTab * tab = webViewsList.at(index+1);

        if (url.startsWith("javascript:", Qt::CaseInsensitive)) {
            url.remove(0,11);
            tab->evaluateJavaScript(url);
        } else if (!internalUrlProcessing(url)) {
            if (mainWindowCallback) {
                const QByteArray asc_url = url.toLatin1();
                mainWindowCallback->onWebViewUrlChanged(::std::string(asc_url.constData(), asc_url.length()));
            }
            QUrl test(url);
            QString errStr = test.errorString();
            if (errStr.length() > 0 )
                LOG(ERROR) + "WebView navigate: failed to parse URL: " + errStr.toStdString();


            tab->loadUrl(url);
        }
    }


}

void QtMainWindow::GoBack(int index)
{
    if (webViewsList.size() < (index + 1)){
        webViewsList.at(index+1)->goBack();
    }
}

void QtMainWindow::GoForward(void)
{
    webViewsList.first()->goForward();
}

void QtMainWindow::Refresh(int index)
{
    if (webViewsList.size() < (index + 1)){
        if (mainWindowCallback) {
            const QByteArray asc_url = webViewsList.at(index+1)->getUrl().toLatin1();
            mainWindowCallback->onWebViewUrlChanged(::std::string(asc_url.constData(), asc_url.length()));
        }
        webViewsList.at(index+1)->refresh();
    }
}

// Tabbar:

void QtMainWindow::tabbarRemoveAllTabs(bool)
{
    // removing WebViews
    for (int i=0; i < webViewsList.size(); ++i) {
        CustomWebViewTab * tab = webViewsList.at(i);
        tabbarDisconnectWebView(tab);

        if (tab != webViewsList.first()) {

            // destroy connected RhoNativeApiCall object
            /*QVariant v = tabViews[i]->page()->property("__rhoNativeApi");
            RhoNativeApiCall* rhoNativeApiCall = v.value<RhoNativeApiCall*>();
            delete rhoNativeApiCall;*/

            //TODO: nativeApiCall

            webViewsList.removeOne(tab);
            tab->deleteLater();

            commitWebViewsList();
        }
    }

}

void QtMainWindow::tabbarInitialize()
{
}


int QtMainWindow::tabbarAddTab(const QString& label, const char* icon, bool disabled, const QColor* web_bkg_color, QTabBarRuntimeParams& tbrp)
{
    CustomWebViewTab* tab = webViewsList.first();
    if (!tbrp["use_current_view_for_tab"].toBool()) {
        // creating web view

        tab = new CustomWebViewTab(label, this);
        //setUpWebPage(tab);

        //page->networkAccessManager()->setProxy(m_proxy);
        /*page->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
        page->mainFrame()->securityOrigin().setDatabaseQuota(1024*1024*1024);

        RhoNativeApiCall* rhoNativeApiCall = new RhoNativeApiCall(page->mainFrame());
        page->setProperty("__rhoNativeApi", QVariant::fromValue(rhoNativeApiCall));
        connect(page->mainFrame(), SIGNAL(javaScriptWindowObjectCleared()),
                rhoNativeApiCall, SLOT(populateJavaScriptWindowObject()));*/

        //TODO: nativeApiCall


        if (web_bkg_color && (web_bkg_color->name().length()>0))
            tab->setHtml(QString("<!DOCTYPE html><html><body style=\"background:") +
                         web_bkg_color->name() + QString("\"></body></html>") );

    }

    if(!webViewsList.contains(tab)) webViewsList.append(tab);
    commitWebViewsList();
    return webViewsList.size() - 1;
}

void QtMainWindow::tabbarShow()
{
    //ui->tabBar->show();
}

void QtMainWindow::tabbarConnectWebView(CustomWebViewTab* webView)
{
    if (webView) {
        if (webView != webViewsList.first()) {
            QObject::connect(webView, SIGNAL(linkClicked(const QString&)), this, SLOT(on_webView_linkClicked(const QString&)));
            QObject::connect(webView, SIGNAL(loadStarted()), this, SLOT(on_webView_loadStarted()));
            QObject::connect(webView, SIGNAL(loadFinished(bool)), this, SLOT(on_webView_loadFinished(bool)));
            QObject::connect(webView, SIGNAL(urlHasBeenChanged(QString)), this, SLOT(on_webView_urlChanged(QString)));
            QObject::connect(webView, SIGNAL(msg(QString,QString)), this, SLOT(rawLog(QString,QString)), Qt::QueuedConnection);
        }
    }
}

void QtMainWindow::tabbarDisconnectWebView(CustomWebViewTab *webView)
{
    if (webView) {
        if (webView != webViewsList.first()) {
            QObject::disconnect(webView, SIGNAL(linkClicked(const QUrl&)), this, SLOT(on_webView_linkClicked(const QUrl&)));
            QObject::disconnect(webView, SIGNAL(loadStarted()), this, SLOT(on_webView_loadStarted()));
            QObject::disconnect(webView, SIGNAL(loadFinished(bool)), this, SLOT(on_webView_loadFinished(bool)));
            QObject::disconnect(webView, SIGNAL(urlHasBeenChanged(QUrl)), this, SLOT(on_webView_urlChanged(QUrl)));
            QObject::disconnect(webView, SIGNAL(msg(QString,QString)), this, SLOT(rawLog(QString,QString)));
        }
    }
}


void QtMainWindow::tabbarHide()
{
    tabbarRemoveAllTabs(true);
}

int QtMainWindow::tabbarGetHeight()
{
    return 100;
}

void QtMainWindow::tabbarSwitch(int index)
{
    if(webViewsList.size() < (index + 1)){
        webViewsList.at(index+1)->setActiveTab(true);
    }

}

int QtMainWindow::tabbarGetCurrent()
{
    for(int i = 0; i < webViewsList.size(); i++){
        if(webViewsList.at(i)->isTabActive()){
            return i-1;
        }
    }
    return -1;
}

void QtMainWindow::tabbarSetSwitchCallback(rho::apiGenerator::CMethodResult& oResult)
{
    m_oTabBarSwitchCallback = oResult;
}

void QtMainWindow::on_tabBar_currentChanged(int index)
{
    /*if ((index < tabViews.size()) && (index >= 0)) {
        QTabBarRuntimeParams tbrp = cur_tbrp != 0 ? *cur_tbrp : ui->tabBar->tabData(index).toHash();
        bool use_current_view_for_tab = tbrp["use_current_view_for_tab"].toBool();

        if (use_current_view_for_tab) {
            tabbarConnectWebView(main_webView);
        } else {
            tabbarDisconnectWebView(main_webView);
        }

        for (unsigned int i=0; i<tabViews.size(); ++i) {
            if (tabViews[i] != main_webView) {
                if (use_current_view_for_tab || (i != index)) {
                    tabbarDisconnectWebView(tabViews[i]);
                } else {
                    tabbarConnectWebView(tabViews[i]);
                }
            }
        }

        if (m_oTabBarSwitchCallback.hasCallback()) 
        {
            /*QString body = QString("&rho_callback=1&tab_index=") + QVariant(index).toString();
            rho::String* cbStr = new rho::String(tbrp["on_change_tab_callback"].toString().toStdString());
            rho::String* bStr = new rho::String(body.toStdString());
            const char* b = bStr->c_str();
            rho_net_request_with_data(RHODESAPP().canonicalizeRhoUrl(*cbStr).c_str(), b);*/
            /*Hashtable<String,String> mapRes;
            mapRes["tab_index"] = convertToStringA(index);
            m_oTabBarSwitchCallback.set(mapRes);
        }

        if (tbrp["reload"].toBool() || (ui->webView && (ui->webView->history()->count()==0))) 
        {
            const QByteArray asc = tbrp["action"].toString().toLatin1(); 
            rho::String strAction = std::string(asc.constData(), asc.length());//new rho::String(tbrp["action"].toString().toStdString());
            RHODESAPP().loadUrl(strAction);

        }
    }*/
}


// Toolbar:

void QtMainWindow::toolbarRemoveAllButtons()
{
    QList<CustomToolBarItem *> killer = toolBarButtonsList;
    toolBarButtonsList.clear();
    commitToolBarButtonsList();
    foreach (CustomToolBarItem * item, killer) {
        item->deleteLater();
    }

    toolBarSeparatorWidth = 0;
}

void QtMainWindow::toolbarShow()
{
    /*ui->toolBar->show();
    ui->toolBarRight->show();*/
}

void QtMainWindow::toolbarHide()
{
    /*ui->toolBar->hide();
    ui->toolBarRight->hide();*/
}

int QtMainWindow::toolbarGetHeight()
{
    return logicalDpiY;
}

void QtMainWindow::toolbarAddAction(const QString & text)
{
    CustomToolBarItem * item = new CustomToolBarItem(text, this);
    toolBarButtonsList.append(item);
    commitToolBarButtonsList();

}

void QtMainWindow::toolbarActionEvent()
{

    CustomToolBarItem* action;
    if (sender() && (action = qobject_cast<CustomToolBarItem*>(sender())))
    {
        rho::String strAction = action->getAction().toStdString();
        if ( strcasecmp(strAction.c_str(), "forward") == 0 )
            rho_webview_navigate_forward();
        else
            RHODESAPP().loadUrl(strAction);
    }
}

void QtMainWindow::toolbarAddAction(const QIcon & icon, const QString & text, const char* action, bool rightAlign)
{
    CustomToolBarItem * item = new CustomToolBarItem(text, QString(action), this);
    QObject::connect(item, SIGNAL(isClicked()), this, SLOT(toolbarActionEvent()));
    toolBarButtonsList.append(item);
    commitToolBarButtonsList();
}

void QtMainWindow::toolbarAddSeparator(int width)
{
    /*toolBarSeparatorWidth = width;
    ui->toolBar->addSeparator();*/
}

void QtMainWindow::setToolbarStyle(bool border, QString background, int viewHeight)
{
    /*ui->toolBar->setMinimumHeight(viewHeight);
    ui->toolBarRight->setMinimumHeight(viewHeight);
    QString style = "";
    if (!border) style += "border:0px;";
    if (background.length()>0)
        style += "background:"+background+";";
    if (style.length()>0) {
        style = "QToolBar{"+style+"}";
        if (toolBarSeparatorWidth > 0)
            style += "QToolBar::separator{width:"+QString::number(toolBarSeparatorWidth)+"px;}";
        ui->toolBar->setStyleSheet(style);
        ui->toolBarRight->setStyleSheet(style);
    }*/
}


// Menu:

void QtMainWindow::menuAddAction(const QString & text, int item, bool enabled)
{
    qDebug() << "Menu adding action";
    CustomMenuItem * m = new CustomMenuItem(text, item, this);
    connect(m, SIGNAL(isClicked()), this, SLOT(menuActionEvent()));
    menuItemsList.append(m);

    commitMenuItemsList();
}

void QtMainWindow::menuClear(void)
{
    qDebug() << "Clearing menu";
    menuItemsList.clear();
    commitMenuItemsList();
}

void QtMainWindow::menuAddSeparator()
{
    //ui->menuMain->addSeparator();
}

void QtMainWindow::menuActionEvent()
{
    CustomMenuItem* action;
    if (sender() && (action = qobject_cast<CustomMenuItem*>(sender())) && mainWindowCallback)
        mainWindowCallback->onCustomMenuItemCommand(action->getItem());
}


void QtMainWindow::openQMLDocument(QString document)
{
    if(!webViewsList.isEmpty()){
        emit webViewsList.first()->openQMLDocument(document);
    }
}

void QtMainWindow::rawLog(QString category, QString msg)
{
    RAWLOGC_INFO(category.toStdString().c_str() , msg.toStdString().c_str());
}

void QtMainWindow::on_actionAbout_triggered()
{
    QString OSDetails= QString("\nOS  : %1  \nApp Compiled with QT Version :  %2 \nRunning with QT Version %3")
    .arg(QtLogView::getOsDetails().toStdString().c_str(),QT_VERSION_STR,qVersion());
    /*QMessageBox::about(this, QString::fromStdString(RHOCONF().getString("title_text")), QString("%1 v%2 %3")
        .arg(QString::fromStdString(RHOCONF().getString("app_name")))
        .arg(QString::fromStdString(RHOCONF().getString("app_version")))
        .arg(OSDetails)
        );*/
}

// slots:

void QtMainWindow::exitCommand()
{
    doExit();
}

void QtMainWindow::navigateBackCommand()
{
    RHODESAPP().navigateBack();
}

void QtMainWindow::navigateForwardCommand()
{
    this->GoForward();
}

void QtMainWindow::webviewNavigateBackCommand(int tab_index)
{
    this->GoBack(tab_index);
}

void QtMainWindow::logCommand()
{
    //if (m_logView) m_logView->show();
}

void QtMainWindow::refreshCommand(int tab_index)
{
    this->Refresh(tab_index);
}

void QtMainWindow::navigateCommand(TNavigateData* nd)
{
    if (nd) {
        if (nd->url) {
            this->navigate(QString::fromWCharArray(nd->url), nd->index);
            free(nd->url);
        }
        free(nd);
    }
}

void QtMainWindow::executeJavaScriptCommand(TNavigateData* nd)
{
    if (nd) {
        if (nd->url) {

            if (webViewsList.size() < (nd->index + 1)){
                webViewsList.at(nd->index+1)->evaluateJavaScript(QString::fromWCharArray(nd->url));
            }
            free(nd->url);
        }
        free(nd);
    }
}

void QtMainWindow::takePicture(char* callbackUrl)
{
    selectPicture(callbackUrl);
}

void QtMainWindow::selectPicture(char* callbackUrl)
{
    /*
    bool wasError = false;
    rho::StringW strBlobRoot = rho::common::convertToStringW( RHODESAPP().getBlobsDirPath() );
    QString fileName = QFileDialog::getOpenFileName(this, "Open Image", QString::fromStdWString(strBlobRoot), "Image Files (*.png *.jpg *.gif *.bmp)");
    char image_uri[4096];
    image_uri[0] = '\0';

    if (!fileName.isNull()) {

        int ixExt = fileName.lastIndexOf('.');
        QString szExt = (ixExt >= 0) && (fileName.lastIndexOf('/') < ixExt) ? fileName.right(fileName.length()-ixExt) : "";
        QDateTime now = QDateTime::currentDateTimeUtc();
        int tz = (int)(now.secsTo(QDateTime::currentDateTime())/3600);

        char file_name[4096];
        const QByteArray asc = szExt.toLatin1(); 
        rho::String strExt = std::string(asc.constData(), asc.length());

        ::sprintf(file_name, "Image_%02i-%02i-%0004i_%02i.%02i.%02i_%c%03i%s",
            now.date().month(), now.date().day(), now.date().year(),
            now.time().hour(), now.time().minute(), now.time().second(),
            tz>0?'_':'-',abs(tz), strExt.c_str() );

        QString full_name = QString::fromStdWString(strBlobRoot);
        full_name.append("/");
        full_name.append(file_name);

        if (QFile::copy(fileName,full_name))
            strcpy( image_uri, file_name );
        else
            wasError = true;
    }

    RHODESAPP().callCameraCallback( (const char*)callbackUrl, rho::common::convertToStringA(image_uri),
        (wasError ? "Error" : ""), fileName.isNull());

    free(callbackUrl);
    */
}


bool QtMainWindow::copyDirRecursive(QString fromDir, QString toDir)
{
    QDir dir(fromDir);
    {
        QDir tempDir(toDir);
        tempDir.mkpath(toDir);
    }
    if (!fromDir.endsWith(QDir::separator())) fromDir.append(QDir::separator());
    if (!toDir.endsWith(QDir::separator())) toDir.append(QDir::separator());

    //qDebug() << "Recursive copy from dir " + fromDir + " to " + toDir;
    foreach (QString copyFile, dir.entryList(QDir::Files)){
        QString from = fromDir + copyFile;
        QString to = toDir + copyFile;

        bool needToCopy = true;

        if (QFile::exists(to)){
            needToCopy = !isFilesEqual(from, to);

            if (needToCopy){
                qDebug() << "Copy from: " + from + " to: " + to;
                if (!QFile::remove(to)){
                    qDebug() << "Can't remove file: " + to;
                    return false;
                }
            }
        }
        if (needToCopy){
            if (QFile::copy(from, to) == false){
                qDebug() << "Can't copy file: " + from;
                return false;
            }
        }
    }

    foreach (QString copyDir, dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)){
        QString from = fromDir + copyDir;
        QString to = toDir + copyDir;
        if (dir.mkpath(to) == false){return false;}
        if (copyDirRecursive(from, to) == false){return false;}
    }
    return true;
}

bool QtMainWindow::isFilesEqual(QString fileName1, QString fileName2)
{
    if (!QFile::exists(fileName1)) return false;
    if (!QFile::exists(fileName2)) return false;

    QByteArray hash1 = getHashFromFile(fileName1);
    QByteArray hash2 = getHashFromFile(fileName2);

    if (hash1.size() == 0 || hash2.size() == 0){return false;}
    if (hash1.size() != hash2.size()){ return false;}


    for(int i = 0; i < hash1.size(); i++){
        if (hash1.at(i) != hash2.at(i)) return false;
    }

    return true;

}



QByteArray QtMainWindow::getHashFromFile(QString &fileName)
{
    QFile file(fileName);
    file.open(QFile::ReadOnly);
    QByteArray hash = QCryptographicHash::hash(file.readAll(), QCryptographicHash::Md5);
    file.close();
    return hash;
}


void QtMainWindow::doAlertCallback(CAlertParams* params, int btnNum, CAlertParams::CAlertButton &button)
{
    if (params->m_callback.length()==0) {
        rho::Hashtable<rho::String, rho::String> mapRes;
        std::ostringstream sBtnIndex;
        sBtnIndex << btnNum;
        mapRes["button_index"] = sBtnIndex.str();
        mapRes["button_id"] = button.m_strID;
        mapRes["button_title"] = button.m_strCaption;
        params->m_callback_ex.set(mapRes);
    } else
        RHODESAPP().callPopupCallback(params->m_callback, button.m_strID, button.m_strCaption);
}

void QtMainWindow::alertShowPopup(CAlertParams * params)
{
    /*rho::StringW strAppName = RHODESAPP().getAppNameW();

    //In some scenarios m_alertDialog variable is not cleaned properly now the following code will ensure the proper cleanup

    if (m_alertDialog!=NULL)
    {
        delete m_alertDialog;
        m_alertDialog=NULL;
    }

    if (params->m_dlgType == CAlertParams::DLG_STATUS) {
        m_alertDialog = new QMessageBox(QMessageBox::NoIcon,
            QString::fromWCharArray(rho::common::convertToStringW(params->m_title).c_str()),
            QString::fromWCharArray(rho::common::convertToStringW(params->m_message).c_str()));
        m_alertDialog->setStandardButtons(QMessageBox::Cancel);

        //exec() is model popup and show() is modeless popup
        //m_alertDialog->exec();
        m_alertDialog->show();
        
    } else if (params->m_dlgType == CAlertParams::DLG_DEFAULT) {
        QMessageBox::warning(0, QString::fromWCharArray(strAppName.c_str()),
            QString::fromWCharArray(rho::common::convertToStringW(params->m_message).c_str()));
    } else if (params->m_dlgType == CAlertParams::DLG_CUSTOM) {
        if ( params->m_buttons.size() == 1 && strcasecmp(params->m_buttons[0].m_strCaption.c_str(), "ok") == 0)
            QMessageBox::warning(0, QString::fromWCharArray(rho::common::convertToStringW(params->m_title).c_str()),
                QString::fromWCharArray(rho::common::convertToStringW(params->m_message).c_str()));
        else if (params->m_buttons.size() == 2 && strcasecmp(params->m_buttons[0].m_strCaption.c_str(), "ok") == 0 &&
            strcasecmp(params->m_buttons[1].m_strCaption.c_str(), "cancel") == 0)
        {
            QMessageBox::StandardButton response = QMessageBox::warning(0,
                QString::fromWCharArray(rho::common::convertToStringW(params->m_title).c_str()),
                QString::fromWCharArray(rho::common::convertToStringW(params->m_message).c_str()),
                QMessageBox::Ok | QMessageBox::Cancel);
            int nBtn = response == QMessageBox::Cancel ? 1 : 0;
            doAlertCallback(params, nBtn, params->m_buttons[nBtn]);
        } else if (m_alertDialog == NULL) {
            QMessageBox::Icon icon = QMessageBox::NoIcon;
            if (stricmp(params->m_icon.c_str(),"alert")==0) {
                icon = QMessageBox::Warning;
            } else if (stricmp(params->m_icon.c_str(),"question")==0) {
                icon = QMessageBox::Question;
            } else if (stricmp(params->m_icon.c_str(),"info")==0) {
                icon = QMessageBox::Information;
            }
            m_alertDialog = new QMessageBox(icon, //new DateTimeDialog(params, 0);
                QString::fromWCharArray(rho::common::convertToStringW(params->m_title).c_str()),
                QString::fromWCharArray(rho::common::convertToStringW(params->m_message).c_str()));
            m_alertDialog->setStandardButtons(0);
            for (int i = 0; i < (int)params->m_buttons.size(); i++) {
                    m_alertDialog->addButton(QString::fromWCharArray(rho::common::convertToStringW(params->m_buttons[i].m_strCaption).c_str()), QMessageBox::ActionRole);
            }
            m_alertDialog->exec();
            if (m_alertDialog) {
                const QAbstractButton* btn = m_alertDialog->clickedButton();
                if (btn) {
                    for (int i = 0; i < m_alertDialog->buttons().count(); ++i) {
                        if (btn == m_alertDialog->buttons().at(i)) {
                            doAlertCallback(params, i, params->m_buttons[i]);

                            break;
                        }
                    }
                }
                delete m_alertDialog;
                m_alertDialog = 0;
            }
        } else {
            LOG(WARNING) + "Trying to show alert dialog while it exists.";
        }
    }

    if (params)
        delete params;
        */
}

void QtMainWindow::alertHidePopup()
{
    /*if (m_alertDialog) {
        m_alertDialog->done(QMessageBox::Accepted);
        delete m_alertDialog;
        m_alertDialog = 0;
    }*/
}

void QtMainWindow::dateTimePicker(CDateTimeMessage* msg)
{
    /*if (msg) {
        int retCode    = -1;

        DateTimeDialog timeDialog(msg, this);
        retCode = timeDialog.exec();

        rho_rhodesapp_callDateTimeCallback( msg->m_callback,
            retCode == QDialog::Accepted ? (long) timeDialog.getUnixTime() : 0,
            msg->m_data,
            retCode == QDialog::Accepted ? 0 : 1);

        delete msg;
    }*/
}

void QtMainWindow::executeCommand(RhoNativeViewRunnable* runnable)
{
    if (runnable) {
        runnable->run();
        delete runnable;
    }
}

void QtMainWindow::executeRunnable(rho::common::IRhoRunnable* pTask)
{
    if (pTask) {
        pTask->runObject();
        delete pTask;
    }
}

void QtMainWindow::takeSignature(void*) //TODO: Signature::Params*
{
    //TODO: takeSignature
}

void QtMainWindow::fullscreenCommand(int enable)
{
    LOG(INFO) + (enable ? "Switched to Fullscreen mode" : "Switched to Normal mode" );
}

bool QtMainWindow::getFullScreen()
{
    return true;
}

void QtMainWindow::setCookie(const char* url, const char* cookie)
{
    /*if (url && cookie) {
        QUrl urlStr = QUrl(QString::fromUtf8(url));
        QNetworkCookieJar* cj = ui->webView->page()->networkAccessManager()->cookieJar();
        QStringList cookieList = QString::fromUtf8(cookie).split(";");
        for (int i=0; i<cookieList.size(); ++i)
            cj->setCookiesFromUrl(QNetworkCookie::parseCookies(cookieList.at(i).toLatin1()), urlStr);
    }*/
}

void QtMainWindow::bringToFront()
{

}

// Window frame
void QtMainWindow::setFrame(int x, int y, int width, int height)
{

}

void QtMainWindow::setPosition(int x, int y)
{

}

void QtMainWindow::setSize(int width, int height)
{

}

void QtMainWindow::lockSize(int locked)
{

}

void QtMainWindow::setTitle(const char* title)
{
    QString wTitle = QString::fromUtf8(title);
    setMainWindowTitle(wTitle);
}
