#ifndef QTMAINWINDOW_H
#define QTMAINWINDOW_H

#include <vector>
#include <QMainWindow>
#include <QUrl>
#include <QtWebKit/QWebView>
#include <QtWebKit/QWebInspector>
#include <QAction>
#include <QMessageBox>
#include "QtWebInspector.h"
#include "MainWindowCallback.h"
#include "common/IRhoThreadImpl.h"
#include "common/RhoNativeViewManager.h"
#include "logging/RhoLog.h"
#include "impl/AlertImpl.h"
#include "impl/DateTimePickerImpl.h"

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
    void setCallback(IMainWindowCallback* callback);
    int getLogicalDpiX();
    int getLogicalDpiY();
    void bringToFront();
    // webview
    void navigate(QString url, int index);
    void GoBack(void);
    void GoForward(void);
    void Refresh(int index);
    bool isStarted(void);
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
    void tabbarSetBadge(int index, char* badge);
private:
    void tabbarWebViewRestore(bool reload);
    void tabbarConnectWebView(QWebView* webView, QWebInspector* webInspector);
    void tabbarDisconnectWebView(QWebView* webView, QWebInspector* webInspector);
    bool internalUrlProcessing(const QUrl& url);

private:
    Ui::QtMainWindow *ui;
    QWebInspector *main_webInspector;
    IMainWindowCallback* cb;
    std::vector<QWebView*> tabViews;
    std::vector<QWebInspector*> tabInspect;
    QWebView* main_webView;
    QWebInspector* cur_webInspector;
    QTabBarRuntimeParams* cur_tbrp;
    QtWebInspector* webInspectorWindow;
    QMessageBox *m_alertDialog;
    //TODO: CSyncStatusDlg *m_SyncStatusDlg;
    int m_LogicalDpiX;
    int m_LogicalDpiY;

private slots:
    void on_webView_urlChanged(QUrl );
    void on_webView_loadFinished(bool);
    void on_webView_loadStarted();
    void on_webView_linkClicked(const QUrl&);
    void on_actionBack_triggered();
    void on_tabBar_currentChanged(int index);
    void on_menuMain_aboutToShow();
private slots:
    void toolbarActionEvent(bool);
    void menuActionEvent(bool);
public slots:
    void exitCommand(void);
    void navigateBackCommand(void);
    void navigateForwardCommand(void);
    void logCommand(void);
    void refreshCommand(int tab_index);
    void navigateCommand(TNavigateData* nd);
    void takePicture(char* callbackUrl);
    void selectPicture(char* callbackUrl);
    void alertShowPopup(CAlertParams *);
    void alertHidePopup(void);
    void dateTimePicker(CDateTimeMessage *);
    void executeCommand(RhoNativeViewRunnable*);
    void executeRunnable(rho::common::IRhoRunnable* pTask);
    void takeSignature(void*); //TODO: Signature::Params*
    void fullscreenCommand(int);
    void setCookie(const char* url, const char* cookie);
protected:
    void resizeEvent(QResizeEvent *);
};

#endif // QTMAINWINDOW_H
