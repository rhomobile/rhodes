#ifndef QTWEBENGINEPAGE_H
#define QTWEBENGINEPAGE_H

#include "QtWebEngineView.h"
#include <QWebEnginePage>
#include <QString>
#include "logging/RhoLog.h"

class QtWebEnginePage : public QWebEnginePage
{
    Q_OBJECT
    DEFINE_LOGCLASS
public:
    explicit QtWebEnginePage(QtWebEngineView *webView);
    void triggerAction(QWebEnginePage::WebAction action, bool checked = false) override;
protected:
    bool acceptNavigationRequest(const QUrl &url, NavigationType type, bool isMainFrame) override;
    void javaScriptConsoleMessage(QWebEnginePage::JavaScriptConsoleMessageLevel level, const QString &message, int lineNumber, const QString &sourceID) override;
    QtWebEngineView *webView;
    bool stopBeforeNavigationRequest = false;
/*signals:
    void linkClicked();
    void onLinkClicked(QUrl);*/
public slots:
};

#endif // QTWEBENGINEPAGE_H
