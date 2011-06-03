#pragma warning(disable:4996)
#include "QtMainWindow.h"
#include "ui_QtMainWindow.h"
#include "ExternalWebView.h"
#include <QResizeEvent>
#include <QWebFrame>
#include <QWebSettings>
#include <QWebSecurityOrigin>
#include <QWebHistory>
#include "ext/rho/rhoruby.h"
#include "common/RhoStd.h"
#include "common/RhodesApp.h"
#include "rubyext/WebView.h"
#include "rubyext/NativeToolbarExt.h"
#undef null

extern "C" {
    extern VALUE rb_thread_main(void);
    extern VALUE rb_thread_wakeup(VALUE thread);
}

QtMainWindow::QtMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QtMainWindow),
    wi(new QWebInspector()),
    cb(NULL),
    cur_tbrp(0)
{
    ui->setupUi(this);

    QWebSettings* qs = QWebSettings::globalSettings(); //this->ui->webView->settings();
    qs->setAttribute(QWebSettings::DeveloperExtrasEnabled, true);
    qs->setAttribute(QWebSettings::LocalStorageEnabled, true);
    qs->setAttribute(QWebSettings::OfflineStorageDatabaseEnabled, true);
    qs->setAttribute(QWebSettings::OfflineWebApplicationCacheEnabled, true);
    qs->setOfflineStorageDefaultQuota(1024*1024*1024);

    rho::String rs_dir = RHODESAPP().getRhoRootPath()+RHO_EMULATOR_DIR;
    qs->setOfflineWebApplicationCachePath(rs_dir.c_str());
    qs->setLocalStoragePath(rs_dir.c_str());
    qs->setOfflineStoragePath(rs_dir.c_str());

    this->ui->webView->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
    this->ui->webView->page()->mainFrame()->securityOrigin().setDatabaseQuota(1024*1024*1024);
    this->main_webView = this->ui->webView;

    this->move(0,0);
    this->ui->toolBar->hide();
    this->ui->toolBarRight->hide();

    // connecting WebInspector
    wi->setWindowTitle("Web Inspector");
    wi->setPage(ui->webView->page());
    wi->move(416, this->geometry().y());
    wi->resize(850, 600);
    wi->show();
}

QtMainWindow::~QtMainWindow()
{
    tabbarRemoveAllTabs(false);
    delete wi;
    delete ui;
}

void QtMainWindow::setCallback(IMainWindowCallback* callback)
{
    cb = callback;
}

void QtMainWindow::hideEvent(QHideEvent *)
{
    if (cb) cb->onActivate(0);
}

void QtMainWindow::showEvent(QShowEvent *)
{
    if (cb) cb->onActivate(1);
}

void QtMainWindow::closeEvent(QCloseEvent *ce)
{
    rb_thread_wakeup(rb_thread_main());
    if (cb) cb->onWindowClose();
    wi->close();
    QMainWindow::closeEvent(ce);
}

void QtMainWindow::resizeEvent(QResizeEvent *event)
{
    if (cb)
        cb->updateSizeProperties(event->size().width(), event->size().height()); // ui->webView
}

void QtMainWindow::on_actionBack_triggered()
{
    ui->webView->back();
}

void QtMainWindow::on_webView_linkClicked(const QUrl& url)
{
    QString sUrl = url.toString();
    if (sUrl.contains("rho_open_target=_blank")) {
        if (cb) cb->logEvent("WebView: open external browser");
        ExternalWebView* externalWebView = new ExternalWebView();
        externalWebView->navigate(QUrl(sUrl.remove("rho_open_target=_blank")));
        externalWebView->show();
        externalWebView->activateWindow();
    } else {
        sUrl.remove(QRegExp("#+$"));
        if (sUrl.compare(ui->webView->url().toString())!=0)
            ui->webView->load(QUrl(sUrl));
    }
}

void QtMainWindow::on_webView_loadStarted()
{
    if (cb) cb->logEvent("WebView: loading...");
}

void QtMainWindow::on_webView_loadFinished(bool ok)
{
    if (cb) cb->logEvent((ok?"WebView: loaded ":"WebView: failed ")); // +ui->webView->url().toString()
}

void QtMainWindow::on_webView_urlChanged(QUrl url)
{
    if (cb) cb->logEvent("WebView: URL changed"); // url.toString()
}

void QtMainWindow::on_menuMain_aboutToShow()
{
    if (cb) cb->createCustomMenu();
}

void QtMainWindow::navigate(QString url)
{
    if (url.startsWith("javascript:", Qt::CaseInsensitive)) {
        url.remove(0,11);
        ui->webView->stop();
        ui->webView->page()->mainFrame()->evaluateJavaScript(url);
    } else
        ui->webView->load(QUrl(url));
}

void QtMainWindow::GoBack(void)
{
    ui->webView->back();
}

void QtMainWindow::GoForward(void)
{
    ui->webView->forward();
}

void QtMainWindow::Refresh(void)
{
    ui->webView->reload();
}


// Tabbar:

void QtMainWindow::tabbarRemoveAllTabs(bool restore)
{
    // removing WebViews
    if (tabViews.size()>0) {
        for (std::vector<QWebView*>::iterator itab=tabViews.begin(); itab!=tabViews.end(); ++itab)
            tabbarDisconnectWebView(*itab);
        tabViews.clear();
    }

    // removing Tabs
    for (int i=ui->tabBar->count()-1; i >= 0; --i)
        ui->tabBar->removeTab(i);

    // restoring main WebView
    tabbarWebViewRestore(restore);
}

void QtMainWindow::tabbarInitialize()
{
    tabbarRemoveAllTabs(false);
}

int QtMainWindow::tabbarAddTab(const QString& label, const char* icon, bool disabled, const QColor* web_bkg_color, QTabBarRuntimeParams& tbrp)
{
    QWebView* wv = new QWebView();
    wv->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
    wv->page()->mainFrame()->securityOrigin().setDatabaseQuota(1024*1024*1024);
    if (web_bkg_color && (web_bkg_color->name().length()>0))
        wv->setHtml( QString("<html><body style=\"background:") + web_bkg_color->name() + QString("\"></body></html>") );
    tabViews.push_back(wv);

    cur_tbrp = &tbrp;
    if (icon && (strlen(icon) > 0))
        ui->tabBar->addTab(QIcon(QString(icon)), label);
    else
        ui->tabBar->addTab(label);
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
    if (webView->parent()==0) {
        webView->setParent(ui->centralWidget);
        ui->verticalLayout->addWidget(webView);
        webView->show();
    }
    if (webView != main_webView) {
        QObject::connect(webView, SIGNAL(linkClicked(const QUrl&)), this, SLOT(on_webView_linkClicked(const QUrl&)));
        QObject::connect(webView, SIGNAL(loadStarted()), this, SLOT(on_webView_loadStarted()));
        QObject::connect(webView, SIGNAL(loadFinished(bool)), this, SLOT(on_webView_loadFinished(bool)));
        QObject::connect(webView, SIGNAL(urlChanged(QUrl)), this, SLOT(on_webView_urlChanged(QUrl)));
    }
    ui->webView = webView;
}

void QtMainWindow::tabbarDisconnectWebView(QWebView* webView)
{
    if (webView->parent()!=0) {
        webView->setParent(0);
        ui->verticalLayout->removeWidget(webView);
        webView->hide();
    }
    if (webView != main_webView) {
        QObject::disconnect(webView, SIGNAL(linkClicked(const QUrl&)), this, SLOT(on_webView_linkClicked(const QUrl&)));
        QObject::disconnect(webView, SIGNAL(loadStarted()), this, SLOT(on_webView_loadStarted()));
        QObject::disconnect(webView, SIGNAL(loadFinished(bool)), this, SLOT(on_webView_loadFinished(bool)));
        QObject::disconnect(webView, SIGNAL(urlChanged(QUrl)), this, SLOT(on_webView_urlChanged(QUrl)));
    }
}

void QtMainWindow::tabbarWebViewRestore(bool reload)
{
    if (ui->webView != main_webView) {
        tabbarDisconnectWebView(ui->webView);
        tabbarConnectWebView(main_webView);
        if (reload)
            main_webView->back();
    }
}

void QtMainWindow::tabbarHide()
{
    tabbarWebViewRestore(true);
    ui->tabBar->hide();
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
    return ui->tabBar->currentIndex();
}

void QtMainWindow::on_tabBar_currentChanged(int index)
{
    if (index < tabViews.size()) {
        QTabBarRuntimeParams tbrp = cur_tbrp != 0 ? *cur_tbrp : ui->tabBar->tabData(index).toHash();
        bool use_current_view_for_tab = tbrp["use_current_view_for_tab"].toBool();

		//wi->setWindowTitle(QString(index));
	    //wi->setPage(0);
	    //wi->setPage(tabViews[index]->page());

		// setTabTextColor(index, ...) <- tbri["selected_color"]

        if (use_current_view_for_tab) {
            tabbarConnectWebView(main_webView);
        } else {
            tabbarDisconnectWebView(main_webView);
        }

        for (unsigned int i=0; i<tabViews.size(); ++i) {
            if (use_current_view_for_tab || (i != index)) {
                tabbarDisconnectWebView(tabViews[i]);
            } else {
                tabbarConnectWebView(tabViews[i]);
            }
        }

        if (tbrp["reload"].toBool() || (ui->webView->history()->count()==0)) {
            rho::String* strAction = new rho::String(tbrp["action"].toString().toStdString());
            RHODESAPP().loadUrl(*strAction);
        }

        // TODO: tbrp["selected_color"] / tbrp["background_color"]
    }
}

void QtMainWindow::setTabbarStyle(QString background)
{
    QString style = ""; // "border:0px"
    if (background.length()>0) {
        if (style.length()>0) style += ";";
        style += "background:"+background;
    }
    if (style.length()>0) {
        style = "QTabBar{"+style+"}";
        ui->tabBar->setStyleSheet(style);
    }
}

// Toolbar:

void QtMainWindow::toolbarRemoveAllButtons()
{
    ui->toolBar->clear();
    ui->toolBarRight->clear();
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

void QtMainWindow::on_toolbarAction_triggered(bool checked)
{
    QObject* sender = QObject::sender();
    QAction* action;
    if (sender && (action = dynamic_cast<QAction*>(sender))) {
        rho::String* strAction = new rho::String(action->data().toString().toStdString());
        if ( strcasecmp(strAction->c_str(), "forward") == 0 )
            rho_webview_navigate_forward();
        else
            RHODESAPP().loadUrl(*strAction);
    }
}

void QtMainWindow::toolbarAddAction(const QIcon & icon, const QString & text, const char* action, bool rightAlign)
{
    QAction* qAction = new QAction(icon, text, ui->toolBar);
    qAction->setData(QVariant(action));
    QObject::connect(qAction, SIGNAL(triggered(bool)), this, SLOT(on_toolbarAction_triggered(bool)) );
    if (rightAlign)
        ui->toolBarRight->insertAction( (ui->toolBarRight->actions().size() > 0 ? ui->toolBarRight->actions().last() : 0), qAction);
    else
        ui->toolBar->addAction(qAction);
}

void QtMainWindow::toolbarAddSeparator()
{
    ui->toolBar->addSeparator();
}

void QtMainWindow::setToolbarStyle(bool border, QString background)
{
    QString style = "";
    if (!border) style += "border:0px";
    if (background.length()>0) {
        if (style.length()>0) style += ";";
        style += "background:"+background;
    }
    if (style.length()>0) {
        style = "QToolBar{"+style+"}";
        ui->toolBar->setStyleSheet(style);
        ui->toolBarRight->setStyleSheet(style);
    }
}


// Menu:

void QtMainWindow::menuAddAction(const QString & text, int item)
{
    QAction* qAction = new QAction(text, ui->toolBar);
    qAction->setData(QVariant(item));
    QObject::connect(qAction, SIGNAL(triggered(bool)), this, SLOT(on_menuAction_triggered(bool)) );
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

void QtMainWindow::on_menuAction_triggered(bool checked)
{
    QObject* sender = QObject::sender();
    QAction* action;
    if (sender && (action = dynamic_cast<QAction*>(sender)) && cb)
        cb->onCustomMenuItemCommand(action->data().toInt());
}
