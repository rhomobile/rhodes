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

#include <QQuickView>
#include <QBasicTimer>
#include <QNetworkProxy>

#include "../rhodes/MainWindowCallback.h"
#include "common/IRhoThreadImpl.h"
#include "common/RhoNativeViewManager.h"
#include "logging/RhoLog.h"
#include "impl/AlertImpl.h"
#include "impl/DateTimePickerImpl.h"
#include "api_generator/MethodResult.h"
#include "QtLogView.h"
#include "custommenuitem.h"
#include "customwebviewtab.h"
#include "customtoolbaritem.h"
#include <sailfishapp.h>
#include <QDesktopServices>


class QtMainWindow : public QObject
{
    Q_OBJECT
    DEFINE_LOGCLASS

    Q_PROPERTY(QString mainWindowTitle READ getMainWindowTitle WRITE setMainWindowTitle NOTIFY mainWindowTitleChanged)
    Q_PROPERTY(int logicalDpiX READ getLogicalDpiX WRITE setLogicalDpiX NOTIFY logicalDpiXChanged)
    Q_PROPERTY(int logicalDpiY READ getLogicalDpiY WRITE setLogicalDpiY NOTIFY logicalDpiYChanged)
    Q_PROPERTY(int rotation READ getRotation WRITE setRotation NOTIFY rotationChanged)

public:
    QString mainWindowTitle;

    int rotation;

    typedef QHash<QString, QVariant> QTabBarRuntimeParams;

    explicit QtMainWindow(QObject *parent = 0);
    ~QtMainWindow();
    virtual void hideEvent(QHideEvent *);
    virtual void showEvent(QShowEvent *);

    void paintEvent();
    void setCallback(IMainWindowCallback* callback);




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
    void tabbarRemoveAllTabs(bool);
    void tabbarShow(void);
    void tabbarHide(void);
    int tabbarGetHeight(void);
    void tabbarSwitch(int index);
    int tabbarGetCurrent();
    int tabbarAddTab(const QString& label, const char* icon, bool disabled, const QColor* web_bkg_color, QTabBarRuntimeParams& tbri);
    //void tabbarSetBadge(int index, const char* badge);
    void tabbarSetSwitchCallback(rho::apiGenerator::CMethodResult& oResult);
    static void setView(QQuickView *value);
    static QtMainWindow * getLastInstance(){
        return lastInstance;
    }




    QList<CustomMenuItem *> menuItemsList;
    void commitMenuItemsList(){
        QQmlContext *context = view->rootContext();
        QList<QObject *> objectList;
        foreach (CustomMenuItem * obj, menuItemsList) {objectList.append(obj);}
        context->setContextProperty("mainMenuListModel", QVariant::fromValue(objectList));
    }

    QList<CustomWebViewTab *> webViewsList;

    void commitWebViewsList(){
        QQmlContext *context = view->rootContext();
        QList<QObject *> objectList;
        foreach (CustomWebViewTab * obj, webViewsList) {
            objectList.append(obj);
        }
        context->setContextProperty("webViewsModel", QVariant::fromValue(objectList));
    }

    QList<CustomToolBarItem *> toolBarButtonsList;
    void commitToolBarButtonsList(){
        QQmlContext *context = view->rootContext();
        QList<QObject *> objectList;
        foreach (CustomToolBarItem * obj, toolBarButtonsList) {objectList.append(obj);}
        context->setContextProperty("toolBarModel", QVariant::fromValue(objectList));
    }


    static bool copyDirRecursive(QString fromDir, QString toDir);
private:
    //void tabbarWebViewRestore(bool reload);
    void tabbarConnectWebView(CustomWebViewTab *webView);
    void tabbarDisconnectWebView(CustomWebViewTab* webView);
    bool internalUrlProcessing(const QUrl& url);
    //void setUpWebPage(QWebPage* page);
    void doAlertCallback(CAlertParams* params, int btnNum, CAlertParams::CAlertButton &button);
    void internalSetProxy();
    static QQuickView * view;
    static QtMainWindow * lastInstance;
private:
    IMainWindowCallback* mainWindowCallback;
    //std::vector<QWebView*> tabViews;
    //QWebView* main_webView;
    QTabBarRuntimeParams* cur_tbrp;
    int logicalDpiX;
    int logicalDpiY;
	bool firstShow, m_bFirstLoad;
    QBasicTimer m_SplashTimer;
    rho::apiGenerator::CMethodResult m_oTabBarSwitchCallback;
    int toolBarSeparatorWidth;
    QNetworkProxy m_proxy;
    QtLogView* m_logView;

private slots:
    void on_webView_urlChanged(QString );
    void on_webView_loadFinished(bool);
    void on_webView_loadStarted();
    void on_webView_linkClicked(const QString &);
    void on_actionBack_triggered();
    //void on_actionRotateRight_triggered();
    //void on_actionRotateLeft_triggered();
    //void on_actionRotate180_triggered();
    void on_actionExit_triggered();
    void on_tabBar_currentChanged(int index);
    void on_menuMain_aboutToShow();
    void on_actionAbout_triggered();
    void closeEvent();


public slots:

    int getLogicalDpiX(){return logicalDpiX;}
    int getLogicalDpiY(){return logicalDpiY;}
    void setLogicalDpiX(int x){logicalDpiX = x;}
    void setLogicalDpiY(int y){logicalDpiY = y;}
    int getRotation() const{return rotation;}
    void setRotation(int value){rotation = value;}
    QString getMainWindowTitle() const;
    void setMainWindowTitle(const QString &value);


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
    void resizeEvent();

    void toolbarActionEvent();
    void menuActionEvent();

signals:
    void logicalDpiXChanged();
    void logicalDpiYChanged();
    void rotationChanged();
    void mainWindowTitleChanged();

};

#endif // QTMAINWINDOW_H
