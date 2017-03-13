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
#ifndef QTMAINWINDOW_H
#define QTMAINWINDOW_H
#include <vector>
#include <QMainWindow>
#include <QUrl>

#if QT_VERSION >= 0x050000
#include <QtWebKitWidgets/QWebView>
#include <QtWebKitWidgets/QWebInspector>
#else
#include <QtWebKit/QWebView>
#include <QtWebKit/QWebInspector>
#endif
#include "QtWebInspector.h"

#include <QAction>
#include <QMessageBox>
#include <QBasicTimer>
#include <QNetworkProxy>

#include "../MainWindowCallback.h"
#include "common/IRhoThreadImpl.h"
#include "common/RhoNativeViewManager.h"
#include "logging/RhoLog.h"
#include "impl/AlertImpl.h"
#include "impl/DateTimePickerImpl.h"
#include "api_generator/MethodResult.h"
#include "../QtLogView.h"

namespace Ui {
    class QtMainWindow;
}

class QtMainWindow : public QMainWindow
{
    Q_OBJECT
    DEFINE_LOGCLASS
public:
    typedef QHash<QString, QVariant> QTabBarRuntimeParams;

public:
    explicit QtMainWindow(QWidget *parent = 0);
    ~QtMainWindow();
    virtual void hideEvent(QHideEvent *);
    virtual void showEvent(QShowEvent *);
    virtual void closeEvent(QCloseEvent *);
    virtual void paintEvent(QPaintEvent *);
    virtual void timerEvent(QTimerEvent *ev);
    void setCallback(IMainWindowCallback* callback);

    int getLogicalDpiX();
    int getLogicalDpiY();
    void bringToFront();
    void adjustWebInspector();
    // webview
    void navigate(QString url, int index);
    void GoBack(int index);
    void GoForward(void);
    void Refresh(int index);
    bool isStarted(void);
    void unsetProxy();
    void setProxy(QString host, QString port, QString login, QString password);
    // toolbar
    void toolbarRemoveAllButtons(void);
    void toolbarShow(void);
    void toolbarHide(void);
    int toolbarGetHeight(void);
    void toolbarAddAction(const QString & text);
    void toolbarAddAction(const QIcon & icon, const QString & text, const char* action, bool rightAlign = false);
    void toolbarAddSeparator(int width);
    void setToolbarStyle(bool border, QString background, int viewHeight);
    // menu
    void menuClear(void);
    void menuAddAction(const QString & text, int item, bool enabled);
    void menuAddSeparator(void);
    // tabbar
    void tabbarInitialize(void);
    void tabbarRemoveAllTabs(bool restore);
    void tabbarShow(void);
    void tabbarHide(void);
    int tabbarGetHeight(void);
    void tabbarSwitch(int index);
    int tabbarGetCurrent();
    int tabbarAddTab(const QString& label, const char* icon, bool disabled, const QColor* web_bkg_color, QTabBarRuntimeParams& tbri);
    void tabbarSetBadge(int index, const char* badge);
    void tabbarSetSwitchCallback(rho::apiGenerator::CMethodResult& oResult);
private:
    void tabbarWebViewRestore(bool reload);
    void tabbarConnectWebView(QWebView* webView, QWebInspector* webInspector);
    void tabbarDisconnectWebView(QWebView* webView, QWebInspector* webInspector);
    bool internalUrlProcessing(const QUrl& url);
    void setUpWebPage(QWebPage* page);
    void doAlertCallback(CAlertParams* params, int btnNum, CAlertParams::CAlertButton &button);
    void internalSetProxy();

private:
    Ui::QtMainWindow *ui;
    QWebInspector *main_webInspector;
    QtWebInspector* webInspectorWindow;
    IMainWindowCallback* mainWindowCallback;
    std::vector<QWebView*> tabViews;
    std::vector<QWebInspector*> tabInspect;
    QWebView* main_webView;
    QWebInspector* cur_webInspector;
    QTabBarRuntimeParams* cur_tbrp;
    QMessageBox *m_alertDialog;
    //TODO: CSyncStatusDlg *m_SyncStatusDlg;
    int m_LogicalDpiX;
    int m_LogicalDpiY;
	bool firstShow, m_bFirstLoad;
    QBasicTimer m_SplashTimer;
    rho::apiGenerator::CMethodResult m_oTabBarSwitchCallback;
    int toolBarSeparatorWidth;
    QNetworkProxy m_proxy;
    QtLogView* m_logView;

private slots:
    void on_webView_urlChanged(QUrl );
    void on_webView_loadFinished(bool);
    void on_webView_loadStarted();
    void on_webView_linkClicked(const QUrl&);
    void on_actionBack_triggered();
    void on_actionRotateRight_triggered();
    void on_actionRotateLeft_triggered();
    void on_actionRotate180_triggered();
    void on_actionExit_triggered();
    void on_tabBar_currentChanged(int index);
    void on_menuMain_aboutToShow();
    void on_actionAbout_triggered();
private slots:
    void toolbarActionEvent(bool);
    void menuActionEvent(bool);
public slots:
    void exitCommand(void);
    void navigateBackCommand(void);
    void navigateForwardCommand(void);
    void webviewNavigateBackCommand(int tab_index);
    void logCommand(void);
    void refreshCommand(int tab_index);
    void navigateCommand(TNavigateData* nd);
    void executeJavaScriptCommand(TNavigateData*);
    void takePicture(char* callbackUrl);
    void selectPicture(char* callbackUrl);
    void alertShowPopup(CAlertParams *);
    void alertHidePopup(void);
    void dateTimePicker(CDateTimeMessage *);
    void executeCommand(RhoNativeViewRunnable*);
    void executeRunnable(rho::common::IRhoRunnable* pTask);
    void takeSignature(void*); //TODO: Signature::Params*
    void fullscreenCommand(int);
    bool getFullScreen();
    void setCookie(const char* url, const char* cookie);
    void setFrame(int x, int y, int width, int height);
    void setPosition(int x, int y);
    void setSize(int width, int height);
    void lockSize(int locked);
    void setTitle(const char* title);

protected:
    void resizeEvent(QResizeEvent *);
};

#endif // QTMAINWINDOW_H
