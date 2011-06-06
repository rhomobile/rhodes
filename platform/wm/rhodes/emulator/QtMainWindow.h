#ifndef QTMAINWINDOW_H
#define QTMAINWINDOW_H

#include <vector>
#include <QMainWindow>
#include <QUrl>
#include <QtWebKit/QWebView>
#include <QtWebKit/QWebInspector>
#include <QAction>
#include "MainWindowCallback.h"
#include "common/IRhoThreadImpl.h"

namespace Ui {
    class QtMainWindow;
}

class QtMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    typedef QHash<QString, QVariant> QTabBarRuntimeParams;

public:
    explicit QtMainWindow(QWidget *parent = 0);
    ~QtMainWindow();
    virtual void hideEvent(QHideEvent *);
    virtual void showEvent(QShowEvent *);
    virtual void closeEvent(QCloseEvent *);
    void setCallback(IMainWindowCallback* callback);
    // webview
    void navigate(QString url);
    void GoBack(void);
    void GoForward(void);
    void Refresh(void);
    // toolbar
    void toolbarRemoveAllButtons(void);
    void toolbarShow(void);
    void toolbarHide(void);
    int toolbarGetHeight(void);
    void toolbarAddAction(const QString & text);
    void toolbarAddAction(const QIcon & icon, const QString & text, const char* action, bool rightAlign = false);
    void toolbarAddSeparator(void);
    void setToolbarStyle(bool border, QString background);
    // menu
    void menuClear(void);
    void menuAddAction(const QString & text, int item);
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
    void setTabbarStyle(QString background);
private:
    void tabbarWebViewRestore(bool reload);
    void tabbarConnectWebView(QWebView* webView, QWebInspector* webInspector);
    void tabbarDisconnectWebView(QWebView* webView, QWebInspector* webInspector);

private:
    Ui::QtMainWindow *ui;
    QWebInspector *main_webInspector;
    IMainWindowCallback* cb;
    std::vector<QWebView*> tabViews;
    std::vector<QWebInspector*> tabInspect;
    QWebView* main_webView;
    QWebInspector* cur_webInspector;
    QTabBarRuntimeParams* cur_tbrp;

private slots:
    void on_webView_urlChanged(QUrl );
    void on_webView_loadFinished(bool);
    void on_webView_loadStarted();
    void on_webView_linkClicked(const QUrl&);
    void on_actionBack_triggered();
    void on_tabBar_currentChanged(int index);
    void on_toolbarAction_triggered(bool);
    void on_menuAction_triggered(bool);
    void on_menuMain_aboutToShow();
protected:
    void resizeEvent(QResizeEvent *);
};

#endif // QTMAINWINDOW_H
