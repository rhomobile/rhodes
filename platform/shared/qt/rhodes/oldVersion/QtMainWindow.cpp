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
#include "ui_QtMainWindow.h"
#include "ExternalWebView.h"
#include "RhoSimulator.h"
#include <sstream>
#include "QtWebPage.h"
#if QT_VERSION > QT_VERSION_CHECK(4, 8, 0)
    #include "../QtCustomStyle.h"
#endif
#include "ext/rho/rhoruby.h"
#include "common/RhoStd.h"
#include "common/RhodesApp.h"
#include "common/RhoConf.h"
#include "common/RhoSimConf.h"
#include "rubyext/WebView.h"
#include "rubyext/NativeToolbarExt.h"
#undef null
#include "DateTimeDialog.h"
#include "RhoNativeApiCall.h"
#include "statistic/RhoProfiler.h"
#include <QStylePainter>
#if QT_VERSION >= 0x050000
    #include <QtWebKit/qtwebkitversion.h>
    #include <QNetworkCookieJar>
#endif
#include <QResizeEvent>
#include <QWebFrame>
#include <QWebSettings>
#include <QWebSecurityOrigin>
#include <QWebHistory>
#include <QLabel>
#include <QtNetwork/QNetworkCookie>
#include <QFileDialog>
#include <QDesktopServices>
#include <QDesktopWidget>
#include <QScroller>
#include <QScrollArea>

#if defined(OS_MACOSX) || defined(OS_LINUX)
#define stricmp strcasecmp
#define strnicmp strncasecmp
#endif

#include "qwebviewselectionsuppressor.h"
#include "qwebviewkineticscroller.h"
#include "../guithreadfunchelper.h"

IMPLEMENT_LOGCLASS(QtMainWindow,"QtMainWindow");

extern "C" {
    extern VALUE rb_thread_main(void);
    extern VALUE rb_thread_wakeup(VALUE thread);
}
using namespace rho;
using namespace rho::common;

QtMainWindow::QtMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QtMainWindow),
    webInspectorWindow(new QtWebInspector()),
    mainWindowCallback(NULL),
    cur_tbrp(0),
    m_alertDialog(0),
    m_LogicalDpiX(0),
    m_LogicalDpiY(0),
    firstShow(true), m_bFirstLoad(true),
    toolBarSeparatorWidth(0),
    m_proxy(QNetworkProxy(QNetworkProxy::DefaultProxy)),
    m_logView(0)
    //TODO: m_SyncStatusDlg
{

#if !defined(RHODES_EMULATOR)
    QPixmap icon(QCoreApplication::applicationDirPath().append(QDir::separator()).append("icon.png"));
    QApplication::setWindowIcon(icon);
    #if QT_VERSION > QT_VERSION_CHECK(5, 0, 0)
        QApplication::setApplicationDisplayName(QString::fromStdString(RHOCONF().getString("title_text")));
    #endif
    QApplication::setApplicationName(QString::fromStdString(RHOCONF().getString("app_name")));
    QApplication::setApplicationVersion(QString::fromStdString(RHOCONF().getString("app_version")));
    QApplication::setOrganizationName(QString::fromStdString(RHOCONF().getString("org_name")));
#elif defined(OS_WINDOWS_DESKTOP)
    QPixmap icon(":/images/rho.png");
    QApplication::setWindowIcon(icon);
#endif
    #if QT_VERSION > QT_VERSION_CHECK(4, 8, 0)
        QApplication::setStyle(new QtCustomStyle());
   #endif

    ui->setupUi(this);

#ifdef OS_WINDOWS_DESKTOP
	ui->menuSimulate->clear();
	ui->menuSimulate->setTitle("Navigate");
	ui->menuSimulate->insertAction(0, ui->actionBack);
#endif

    QWebSettings* qs = QWebSettings::globalSettings();
    qs->setAttribute(QWebSettings::DeveloperExtrasEnabled, true);
    qs->setOfflineStorageDefaultQuota(1024*1024*1024);

    rho::String rs_dir = RHODESAPP().getRhoRootPath()+RHO_EMULATOR_DIR;
    qs->enablePersistentStorage(rs_dir.c_str());

	this->ui->webView->setContextMenuPolicy(Qt::NoContextMenu);
	this->ui->webView->setPage(new QtWebPage());
    this->ui->webView->setAttribute(Qt::WA_AcceptTouchEvents, false);
    setUpWebPage(this->ui->webView->page());
    this->main_webView = this->ui->webView;
    this->main_webInspector = webInspectorWindow->webInspector();
    this->cur_webInspector = this->main_webInspector;

    this->move(0,0);
    this->ui->toolBar->hide();
    this->ui->toolBarRight->hide();

    this->main_webView->hide();
    //this->ui->centralWidget->hide();

    //this->ui->centralWidget->setStyleSheet("background-color: yellow");
    //this->ui->centralWidget->setStyleSheet("QWidget {background-image: url(test.jpg)}" );

    GuiThreadFuncHelper::getInstance(this);
#ifdef RHODES_EMULATOR
	int width = RHOSIMCONF().getInt("screen_width");
	int height = RHOSIMCONF().getInt("screen_height");
#else
	int width = RHOCONF().getInt("screen_width");
	int height = RHOCONF().getInt("screen_height");
#endif
	if ((width>0) && (height>0))
		this->setSize(width, height);
	else if (width>0)
		this->setSize(width, this->height());
	else if (height>0)
		this->setSize(this->width(), height);

#if defined(RHODES_EMULATOR)
    // connecting WebInspector
    main_webInspector->setPage(ui->webView->page());
#endif

    if (RHOCONF().isExist("use_kinetic_scroll_on_windows") && RHOCONF().getBool("use_kinetic_scroll_on_windows"))
    {
        QWebViewKineticScroller *newScroller = new QWebViewKineticScroller();
        newScroller->setWidget(this->ui->webView);
        QWebViewSelectionSuppressor* suppressor = new QWebViewSelectionSuppressor(this->ui->webView);
    }


#if defined(RHODES_EMULATOR)
    webInspectorWindow->show();
#endif

    if (RHOCONF().isExist("http_proxy_host"))
    {
        setProxy(QString::fromStdString(RHOCONF().getString("http_proxy_host")),
                 QString::fromStdString(RHOCONF().getString("http_proxy_port")),
                 QString::fromStdString(RHOCONF().getString("http_proxy_login")),
                 QString::fromStdString(RHOCONF().getString("http_proxy_password")));
    } else {
        unsetProxy();
    }
}

QtMainWindow::~QtMainWindow()
{
    tabbarRemoveAllTabs(false);
    if (m_alertDialog) delete m_alertDialog;
    //TODO: m_SyncStatusDlg
    LOGCONF().setLogView(NULL);
    if (m_logView)
    {
        delete m_logView;
        m_logView = 0;
    }
    delete webInspectorWindow;
    delete ui;
}

void QtMainWindow::paintEvent(QPaintEvent *p2)
{
    if ( m_bFirstLoad )
    {
        QPainter paint(this);

        QImage image;
        image.load(RHODESAPP().getLoadingPngPath().c_str());
        QSize imSize = image.size();

        QRect rcClient = this->rect();
        rcClient.setBottom( rcClient.bottom() - this->ui->toolBar->rect().height() ) ;
        RHODESAPP().getSplashScreen().start();
/*
        CSplashScreen& splash = RHODESAPP().getSplashScreen();

        int nLeft = rcClient.left(), nTop = rcClient.top(), nWidth = imSize.width(), nHeight = imSize.height(), Width = rcClient.right() - rcClient.left(), Height = rcClient.bottom() - rcClient.top();
        if (splash.isFlag(CSplashScreen::HCENTER) )
		    nLeft = (Width-nWidth)/2;
	    if (splash.isFlag(CSplashScreen::VCENTER) )
		    nTop = (Height-nHeight)/2;
	    if (splash.isFlag(CSplashScreen::VZOOM) )
		    nHeight = Height;
	    if (splash.isFlag(CSplashScreen::HZOOM) )
		    nWidth = Width;

        QRect rc( nLeft, nTop, nWidth, nHeight );*/

        paint.drawImage( rcClient, image );
    }

    QMainWindow::paintEvent(p2);

}

void QtMainWindow::setCallback(IMainWindowCallback* callback)
{
    mainWindowCallback = callback;
}

void QtMainWindow::hideEvent(QHideEvent *)
{
    if (mainWindowCallback) mainWindowCallback->onActivate(0);
}

void QtMainWindow::showEvent(QShowEvent *)
{
    if (mainWindowCallback) mainWindowCallback->onActivate(1);
}

void QtMainWindow::closeEvent(QCloseEvent *ce)
{
    if ( rho_ruby_is_started() )
        rb_thread_wakeup(rb_thread_main());

    if (mainWindowCallback) mainWindowCallback->onWindowClose();
    tabbarRemoveAllTabs(false);
    webInspectorWindow->close();
    if (m_logView)
        m_logView->close();
    QMainWindow::closeEvent(ce);
}

void QtMainWindow::resizeEvent(QResizeEvent *event)
{
    m_LogicalDpiX = this->logicalDpiY();
    m_LogicalDpiY = this->logicalDpiY();
    if (mainWindowCallback)
        mainWindowCallback->updateSizeProperties(event->size().width(), event->size().height());
    if (m_logView == 0) {
        m_logView = new QtLogView();
        LOGCONF().setLogView(m_logView);
    }
}

void QtMainWindow::adjustWebInspector()
{
	int screen_width = QApplication::desktop()->screenGeometry().width();
	int wi_x = this->x() + this->width() + 16;
	if ((wi_x + webInspectorWindow->width() + 16) > screen_width)
		wi_x = screen_width - webInspectorWindow->width() - 16;
	webInspectorWindow->move(wi_x, webInspectorWindow->y());
}

bool QtMainWindow::isStarted(void)
{
    return (tabViews.size() > 0) ||
      ( (ui->toolBar->isVisible() || ui->toolBarRight->isVisible()) &&
        ((ui->toolBar->actions().size() > 0) || (ui->toolBarRight->actions().size() > 0))
      );
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
    //for (int i=0; i<tabViews.size(); ++i) {
    //    tabViews[i]->page()->networkAccessManager()->setProxy(m_proxy);
    //}
    //ui->webView->page()->networkAccessManager()->setProxy(m_proxy);
}

void QtMainWindow::on_actionBack_triggered()
{
	RHODESAPP().navigateBack();
}

void QtMainWindow::on_actionRotateRight_triggered()
{
	this->resize(this->height(), this->width());
	this->adjustWebInspector();
	RHODESAPP().callScreenRotationCallback(this->width(), this->height(), 90);
}

void QtMainWindow::on_actionRotateLeft_triggered()
{
	this->resize(this->height(), this->width());
	this->adjustWebInspector();
	RHODESAPP().callScreenRotationCallback(this->width(), this->height(), -90);
}

void QtMainWindow::on_actionRotate180_triggered()
{
	RHODESAPP().callScreenRotationCallback(this->width(), this->height(), 180);
}

void QtMainWindow::on_actionExit_triggered()
{
    this->close();
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
        QMessageBox::information(0, "Phone call", "Call to " + sUrl);
        return true;
    }
    if (sUrl.startsWith("sms:")) {
        sUrl.remove(0, 4);
        if ((ipos = sUrl.indexOf('?')) >= 0) sUrl = sUrl.left(ipos);
        QMessageBox::information(0, "SMS", "Send SMS to " + sUrl);
        return true;
    }
    return false;
}

void QtMainWindow::on_webView_linkClicked(const QUrl& url)
{
    QString sUrl = url.toString();
    if (sUrl.contains("rho_open_target=_blank")) {
        LOG(INFO) + "WebView: open external browser";
        ExternalWebView* externalWebView = new ExternalWebView();
        externalWebView->navigate(QUrl(sUrl.remove("rho_open_target=_blank")));
        externalWebView->show();
        externalWebView->activateWindow();
    } else if (ui->webView) {
        if (!internalUrlProcessing(url)) {
            sUrl.remove(QRegExp("#+$"));
            if (sUrl.compare(ui->webView->url().toString())!=0) {
                if (mainWindowCallback && !sUrl.startsWith("javascript:", Qt::CaseInsensitive)) {
                    const QByteArray asc_url = sUrl.toLatin1();
                    mainWindowCallback->onWebViewUrlChanged(::std::string(asc_url.constData(), asc_url.length()));
                }
                ui->webView->load(QUrl(sUrl));
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
    if (ok)
        LOG(INFO) + "Page load complete.";
    else
    {
        LOG(ERROR) + "Page load failed.";
        ui->webView->setHtml("<html><head><title>Error Loading Page</title></head><body><h1>Error Loading Page.</h1></body></html>");
    }

    PROF_STOP("BROWSER_PAGE");

    if (mainWindowCallback && ok) {
        const QByteArray asc_url = ui->webView->url().toString().toLatin1();
        mainWindowCallback->onWebViewUrlChanged(::std::string(asc_url.constData(), asc_url.length()));
    }

    if ( m_bFirstLoad )
    {
        if ( ui->webView->url().toString() != "about:blank" )
        {
            long lMS = RHODESAPP().getSplashScreen().howLongWaitMs();
            if ( lMS > 0 )
                m_SplashTimer.start(lMS, this);
            else
            {
                m_bFirstLoad = false;
                main_webView->show();
            }
        }
    }

}

void QtMainWindow::timerEvent(QTimerEvent *ev)
{
    if (ev->timerId() == m_SplashTimer.timerId()) 
    {
        m_bFirstLoad = false;
        main_webView->show();
     } else {
         QWidget::timerEvent(ev);
     }
}

void QtMainWindow::on_webView_urlChanged(QUrl url)
{
    if (mainWindowCallback) {
        const QByteArray asc_url = url.toString().toLatin1();
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
    QWebView* wv = (index < tabViews.size()) && (index >= 0) ? tabViews[index] : ui->webView;
    if (wv) {
        if (url.startsWith("javascript:", Qt::CaseInsensitive)) {
            url.remove(0,11);
            //wv->stop();
            wv->page()->mainFrame()->evaluateJavaScript(url);
        } else if (!internalUrlProcessing(url)) {
            if (mainWindowCallback) {
                const QByteArray asc_url = url.toLatin1();
                mainWindowCallback->onWebViewUrlChanged(::std::string(asc_url.constData(), asc_url.length()));
            }
			QUrl test(url);
			QString errStr = test.errorString();
			if (errStr.length() > 0 )
				LOG(ERROR) + "WebView navigate: failed to parse URL: " + errStr.toStdString();

            wv->load(QUrl(url));
        }
    }
}

void QtMainWindow::GoBack(int index)
{
    QWebView* wv = (index < tabViews.size()) && (index >= 0) ? tabViews[index] : ui->webView;
    if (wv)
        wv->back();
}

void QtMainWindow::GoForward(void)
{
    if (ui->webView)
        ui->webView->forward();
}

void QtMainWindow::Refresh(int index)
{
    QWebView* wv = (index < tabViews.size()) && (index >= 0) ? tabViews[index] : ui->webView;
    if (wv) {
        if (mainWindowCallback) {
            const QByteArray asc_url = wv->url().toString().toLatin1();
            mainWindowCallback->onWebViewUrlChanged(::std::string(asc_url.constData(), asc_url.length()));
        }
        wv->reload();
    }
}

int QtMainWindow::getLogicalDpiX()
{
    return m_LogicalDpiX;
}

int QtMainWindow::getLogicalDpiY()
{
    return m_LogicalDpiY;
}


// Tabbar:

void QtMainWindow::tabbarRemoveAllTabs(bool restore)
{
    // removing WebViews
    for (int i=0; i<tabViews.size(); ++i) {
        tabbarDisconnectWebView(tabViews[i], tabInspect[i]);
        if (tabViews[i] != main_webView) {
            // destroy connected RhoNativeApiCall object
            QVariant v = tabViews[i]->page()->property("__rhoNativeApi");
            RhoNativeApiCall* rhoNativeApiCall = v.value<RhoNativeApiCall*>();
            delete rhoNativeApiCall;

            ui->verticalLayout->removeWidget(tabViews[i]);
            tabViews[i]->setParent(0);
            if (ui->webView == tabViews[i])
                ui->webView = 0;
            delete tabViews[i];
        }
        if (tabInspect[i] != main_webInspector) {
            webInspectorWindow->removeWebInspector(tabInspect[i]);
            if (cur_webInspector==tabInspect[i])
                cur_webInspector = 0;
            delete tabInspect[i];
        }
    }
    tabViews.clear();
    tabInspect.clear();

    // removing Tabs
    for (int i=ui->tabBar->count()-1; i >= 0; --i)
        ui->tabBar->removeTab(i);

    // restoring main WebView
    tabbarWebViewRestore(restore);
}

void QtMainWindow::tabbarInitialize()
{
    if (ui->webView)
        ui->webView->stop();
    tabbarRemoveAllTabs(false);
    ui->tabBar->clearStyleSheet();
}

void QtMainWindow::setUpWebPage(QWebPage* page)
{
    //page->networkAccessManager()->setProxy(m_proxy);
    page->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
    page->mainFrame()->securityOrigin().setDatabaseQuota(1024*1024*1024);
    quint16 webkit_debug_port = 
#ifdef RHODES_EMULATOR        
        RHOSIMCONF().getInt("webkit_debug_port");
#else
        0;
#endif

    if (webkit_debug_port == 0)
        webkit_debug_port = 9090;
    page->setProperty("_q_webInspectorServerPort", webkit_debug_port);
    RhoNativeApiCall* rhoNativeApiCall = new RhoNativeApiCall(page->mainFrame());
    page->setProperty("__rhoNativeApi", QVariant::fromValue(rhoNativeApiCall));
    connect(page->mainFrame(), SIGNAL(javaScriptWindowObjectCleared()),
        rhoNativeApiCall, SLOT(populateJavaScriptWindowObject()));
}

int QtMainWindow::tabbarAddTab(const QString& label, const char* icon, bool disabled, const QColor* web_bkg_color, QTabBarRuntimeParams& tbrp)
{
    QWebView* wv = main_webView;
    QWebInspector* wI = main_webInspector;
    if (!tbrp["use_current_view_for_tab"].toBool()) {
        // creating web view
        wv = new QWebView();
        wv->setMaximumSize(0,0);
        wv->setParent(ui->centralWidget);
        wv->setAttribute(Qt::WA_AcceptTouchEvents, false);
        ui->verticalLayout->addWidget(wv);
		wv->setPage(new QtWebPage());
        setUpWebPage(wv->page());
        if (web_bkg_color && (web_bkg_color->name().length()>0))
            wv->setHtml( QString("<!DOCTYPE html><html><body style=\"background:") + web_bkg_color->name() + QString("\"></body></html>") );
        // creating and attaching web inspector
        wI = new QWebInspector();
        wI->setWindowTitle("Web Inspector");
        wI->setPage(wv->page());
    }

    tabViews.push_back(wv);
    tabInspect.push_back(wI);
    webInspectorWindow->addWebInspector(wI);

    cur_tbrp = &tbrp;
    if (icon && (strlen(icon) > 0))
        ui->tabBar->addTab(QIcon(QString(icon)), label);
    else
        ui->tabBar->addTab(label);
    ui->tabBar->setTabToolTip(ui->tabBar->count()-1, label);
    if (disabled)
        ui->tabBar->setTabEnabled(ui->tabBar->count()-1, false);
    cur_tbrp = 0;
	ui->tabBar->setTabData(ui->tabBar->count()-1, tbrp);

    return ui->tabBar->count() - 1;
}

void QtMainWindow::tabbarShow()
{
    ui->tabBar->show();
}

void QtMainWindow::tabbarConnectWebView(QWebView* webView)
{
    if (webView) {
        webView->setMaximumSize(QWIDGETSIZE_MAX,QWIDGETSIZE_MAX); //->show();
        if (webView != main_webView) {
            QObject::connect(webView, SIGNAL(linkClicked(const QUrl&)), this, SLOT(on_webView_linkClicked(const QUrl&)));
            QObject::connect(webView, SIGNAL(loadStarted()), this, SLOT(on_webView_loadStarted()));
            QObject::connect(webView, SIGNAL(loadFinished(bool)), this, SLOT(on_webView_loadFinished(bool)));
            QObject::connect(webView, SIGNAL(urlChanged(QUrl)), this, SLOT(on_webView_urlChanged(QUrl)));
        }
        ui->webView = webView;
    }
    if (webInspectorWindow) {
        webInspectorWindow->showWebInspector(webInspector);
        cur_webInspector = webInspector;
    }
}

void QtMainWindow::tabbarDisconnectWebView(QWebView* webView, QWebInspector* webInspector)
{
    if (webView) {
        webView->setMaximumSize(0,0); //->hide();
        if (webView != main_webView) {
            QObject::disconnect(webView, SIGNAL(linkClicked(const QUrl&)), this, SLOT(on_webView_linkClicked(const QUrl&)));
            QObject::disconnect(webView, SIGNAL(loadStarted()), this, SLOT(on_webView_loadStarted()));
            QObject::disconnect(webView, SIGNAL(loadFinished(bool)), this, SLOT(on_webView_loadFinished(bool)));
            QObject::disconnect(webView, SIGNAL(urlChanged(QUrl)), this, SLOT(on_webView_urlChanged(QUrl)));
        }
    }
    if (webInspector)
        webInspectorWindow->hideWebInspector(webInspector);
}

void QtMainWindow::tabbarWebViewRestore(bool reload)
{
    if ((ui->webView == 0) || (ui->webView != main_webView)) {
        tabbarDisconnectWebView(ui->webView, cur_webInspector);
        tabbarConnectWebView(main_webView, main_webInspector);
    } else if (reload) {
        main_webView->setMaximumSize(QWIDGETSIZE_MAX,QWIDGETSIZE_MAX); //->show();
        main_webInspector->setMaximumSize(QWIDGETSIZE_MAX,QWIDGETSIZE_MAX); //->show();
    }
}

void QtMainWindow::tabbarHide()
{
    tabbarRemoveAllTabs(true);
}

int QtMainWindow::tabbarGetHeight()
{
    return ui->tabBar->height();
}

void QtMainWindow::tabbarSwitch(int index)
{
    ui->tabBar->setCurrentIndex(index);
}

int QtMainWindow::tabbarGetCurrent()
{
    return tabViews.size() > 0 ? ui->tabBar->currentIndex() : 0;
}

void QtMainWindow::tabbarSetBadge(int index, const char* badge)
{
    ui->tabBar->setTabButton(index, QTabBar::RightSide, (badge && (*badge != '\0') ? new QLabel(badge) : 0));
}

void QtMainWindow::tabbarSetSwitchCallback(rho::apiGenerator::CMethodResult& oResult)
{
    m_oTabBarSwitchCallback = oResult;
}

void QtMainWindow::on_tabBar_currentChanged(int index)
{
    if ((index < tabViews.size()) && (index >= 0)) {
        QTabBarRuntimeParams tbrp = cur_tbrp != 0 ? *cur_tbrp : ui->tabBar->tabData(index).toHash();
        bool use_current_view_for_tab = tbrp["use_current_view_for_tab"].toBool();

        webInspectorWindow->setWindowTitle(QString("Web Inspector - Tab ") + QVariant(index+1).toString() +
            QString(" - ") + tbrp["label"].toString());

        if (use_current_view_for_tab) {
            tabbarConnectWebView(main_webView, main_webInspector);
        } else {
            tabbarDisconnectWebView(main_webView, main_webInspector);
        }

        for (unsigned int i=0; i<tabViews.size(); ++i) {
            if (tabViews[i] != main_webView) {
                if (use_current_view_for_tab || (i != index)) {
                    tabbarDisconnectWebView(tabViews[i], tabInspect[i]);
                } else {
                    tabbarConnectWebView(tabViews[i], tabInspect[i]);
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
            Hashtable<String,String> mapRes;
            mapRes["tab_index"] = convertToStringA(index);
            m_oTabBarSwitchCallback.set(mapRes);
        }

        if (tbrp["reload"].toBool() || (ui->webView && (ui->webView->history()->count()==0))) 
        {
            const QByteArray asc = tbrp["action"].toString().toLatin1(); 
            rho::String strAction = std::string(asc.constData(), asc.length());//new rho::String(tbrp["action"].toString().toStdString());
            RHODESAPP().loadUrl(strAction);

        }
    }
}


// Toolbar:

void QtMainWindow::toolbarRemoveAllButtons()
{
    ui->toolBar->clear();
    ui->toolBarRight->clear();
    toolBarSeparatorWidth = 0;
}

void QtMainWindow::toolbarShow()
{
    ui->toolBar->show();
    ui->toolBarRight->show();
}

void QtMainWindow::toolbarHide()
{
    ui->toolBar->hide();
    ui->toolBarRight->hide();
}

int QtMainWindow::toolbarGetHeight()
{
    return ui->toolBar->height();
}

void QtMainWindow::toolbarAddAction(const QString & text)
{
    ui->toolBar->addAction(text);
}

void QtMainWindow::toolbarActionEvent(bool checked)
{
    QObject* sender = QObject::sender();
    QAction* action;
    if (sender && (action = dynamic_cast<QAction*>(sender))) 
    {
        const QByteArray asc = action->data().toString().toLatin1(); 
        rho::String strAction = std::string(asc.constData(), asc.length());//new rho::String(action->data().toString().toStdString());
        if ( strcasecmp(strAction.c_str(), "forward") == 0 )
            rho_webview_navigate_forward();
        else
            RHODESAPP().loadUrl(strAction);
    }
}

void QtMainWindow::toolbarAddAction(const QIcon & icon, const QString & text, const char* action, bool rightAlign)
{
    QAction* qAction = new QAction(icon, text, ui->toolBar);
    qAction->setData(QVariant(action));
    QObject::connect(qAction, SIGNAL(triggered(bool)), this, SLOT(toolbarActionEvent(bool)) );
    if (rightAlign)
        ui->toolBarRight->insertAction( (ui->toolBarRight->actions().size() > 0 ? ui->toolBarRight->actions().last() : 0), qAction);
    else
        ui->toolBar->addAction(qAction);
}

void QtMainWindow::toolbarAddSeparator(int width)
{
    toolBarSeparatorWidth = width;
    ui->toolBar->addSeparator();
}

void QtMainWindow::setToolbarStyle(bool border, QString background, int viewHeight)
{
    ui->toolBar->setMinimumHeight(viewHeight);
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
    }
}


// Menu:

void QtMainWindow::menuAddAction(const QString & text, int item, bool enabled)
{
    QAction* qAction = new QAction(text, ui->toolBar);
    qAction->setData(QVariant(item));
    qAction->setEnabled(enabled);
    QObject::connect(qAction, SIGNAL(triggered(bool)), this, SLOT(menuActionEvent(bool)) );
    ui->menuMain->addAction(qAction);
}

void QtMainWindow::menuClear(void)
{
    ui->menuMain->clear();
}

void QtMainWindow::menuAddSeparator()
{
    ui->menuMain->addSeparator();
}

void QtMainWindow::menuActionEvent(bool checked)
{
    QObject* sender = QObject::sender();
    QAction* action;
    if (sender && (action = dynamic_cast<QAction*>(sender)) && mainWindowCallback)
        mainWindowCallback->onCustomMenuItemCommand(action->data().toInt());
}

void QtMainWindow::on_actionAbout_triggered()
{
    QString OSDetails= QString("\nOS  : %1  \nApp Compiled with QT Version :  %2 \nRunning with QT Version %3")
    .arg(QtLogView::getOsDetails().toStdString().c_str(),QT_VERSION_STR,qVersion());
#ifndef RHO_SYMBIAN
#ifdef RHODES_EMULATOR
    QMessageBox::about(this, RHOSIMULATOR_NAME, QString(RHOSIMULATOR_NAME " v" RHOSIMULATOR_VERSION "\n(QtWebKit v" QTWEBKIT_VERSION_STR ")\n(WebKit v%1) \nPlatform : %2 %3").arg(qWebKitVersion())
       .arg(RHOSIMCONF().getString( "platform").c_str())
       .arg(OSDetails)
       );
#else
    QMessageBox::about(this, QString::fromStdString(RHOCONF().getString("title_text")), QString("%1 v%2 %3")
        .arg(QString::fromStdString(RHOCONF().getString("app_name")))
        .arg(QString::fromStdString(RHOCONF().getString("app_version")))
        .arg(OSDetails)
        );
#endif

#endif
}

// slots:

void QtMainWindow::exitCommand()
{
    this->close();
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
    if (m_logView)
        m_logView->show();
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
            QWebView* wv = (nd->index < tabViews.size()) && (nd->index >= 0) ? tabViews[nd->index] : ui->webView;
            if (wv)
                wv->page()->mainFrame()->evaluateJavaScript(QString::fromWCharArray(nd->url));
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
    rho::StringW strAppName = RHODESAPP().getAppNameW();

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
#ifdef OS_SYMBIAN
                if(i == 0)
#endif
                    m_alertDialog->addButton(QString::fromWCharArray(rho::common::convertToStringW(params->m_buttons[i].m_strCaption).c_str()), QMessageBox::ActionRole);
#ifdef OS_SYMBIAN
                else if( i == 1)
                    m_alertDialog->addButton(QString::fromWCharArray(rho::common::convertToStringW(params->m_buttons[i].m_strCaption).c_str()), QMessageBox::RejectRole);
                else if(i == 2)
                    break;
#endif
            }
            m_alertDialog->exec();
            if (m_alertDialog) {
                const QAbstractButton* btn = m_alertDialog->clickedButton();
                if (btn) {
                    for (int i = 0; i < m_alertDialog->buttons().count(); ++i) {
                        if (btn == m_alertDialog->buttons().at(i)) {
#ifdef OS_SYMBIAN
                            RHODESAPP().callPopupCallback(params->m_callback, params->m_buttons[m_alertDialog->buttons().count() - i - 1].m_strID, params->m_buttons[m_alertDialog->buttons().count() - i - 1].m_strCaption);
#else
                            doAlertCallback(params, i, params->m_buttons[i]);
#endif
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
}

void QtMainWindow::alertHidePopup()
{
    if (m_alertDialog) {
        m_alertDialog->done(QMessageBox::Accepted);
        delete m_alertDialog;
        m_alertDialog = 0;
    }
}

void QtMainWindow::dateTimePicker(CDateTimeMessage* msg)
{
    if (msg) {
        int retCode    = -1;

        DateTimeDialog timeDialog(msg, this);
        retCode = timeDialog.exec();

        rho_rhodesapp_callDateTimeCallback( msg->m_callback,
            retCode == QDialog::Accepted ? (long) timeDialog.getUnixTime() : 0,
            msg->m_data,
            retCode == QDialog::Accepted ? 0 : 1);

        delete msg;
    }
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
#if defined(OS_WINDOWS_DESKTOP) && !defined(RHODES_EMULATOR)
    if ((enable && !isFullScreen()) || (!enable && isFullScreen())) {
        ui->menubar->setVisible(RHOCONF().getBool("w32_fullscreen_menu") || (!enable));
        setWindowModality(enable ? Qt::ApplicationModal : Qt::NonModal);
        setWindowState(windowState() ^ Qt::WindowFullScreen);
    }
#else
    if ((enable && !isMaximized()) || (!enable && isMaximized()))
        setWindowState(windowState() ^ Qt::WindowMaximized);
#endif
    LOG(INFO) + (enable ? "Switched to Fullscreen mode" : "Switched to Normal mode" );
}

bool QtMainWindow::getFullScreen()
{
#if defined(OS_WINDOWS_DESKTOP) && !defined(RHODES_EMULATOR)
    return windowState() & Qt::WindowFullScreen;
#else
    return windowState() & Qt::WindowMaximized;
#endif
}

void QtMainWindow::setCookie(const char* url, const char* cookie)
{
    if (url && cookie) {
        QUrl urlStr = QUrl(QString::fromUtf8(url));
        QNetworkCookieJar* cj = ui->webView->page()->networkAccessManager()->cookieJar();
        QStringList cookieList = QString::fromUtf8(cookie).split(";");
        for (int i=0; i<cookieList.size(); ++i)
            cj->setCookiesFromUrl(QNetworkCookie::parseCookies(cookieList.at(i).toLatin1()), urlStr);
    }
}

void QtMainWindow::bringToFront()
{
    this->show();
    this->raise();
    this->activateWindow();
}

// Window frame
void QtMainWindow::setFrame(int x, int y, int width, int height)
{
    this->move(x, y);
    this->resize(width, height);
    this->adjustWebInspector();
}

void QtMainWindow::setPosition(int x, int y)
{
    this->move(x, y);
}

void QtMainWindow::setSize(int width, int height)
{
    this->resize(width, height);
    this->adjustWebInspector();
}

void QtMainWindow::lockSize(int locked)
{
	if (locked)
		this->setFixedSize(this->width(), this->height());
	else
		this->setFixedSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
}

void QtMainWindow::setTitle(const char* title)
{
    this->setWindowTitle(QString::fromUtf8(title));
}
