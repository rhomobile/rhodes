#include "QtWebEnginePage.h"
#include <QWebEngineSettings>
#include <QtDebug>
#include <QTimer>

QtWebEnginePage::QtWebEnginePage(QtWebEngineView *webView) : QWebEnginePage(webView), webView(webView){
    settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessFileUrls, true);
    settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessRemoteUrls, true);
    settings()->setAttribute(QWebEngineSettings::LocalStorageEnabled, true);
}

IMPLEMENT_LOGCLASS(QtWebEnginePage,"JavaScript");

void QtWebEnginePage::javaScriptConsoleMessage(QWebEnginePage::JavaScriptConsoleMessageLevel level, const QString& message, int lineNumber, const QString& sourceID)
{
    QString origin = "Console message at " + sourceID + ":" + QString::number(lineNumber);
    const QByteArray asc_origin = origin.toLatin1();
    rho::String str_origin = ::std::string(asc_origin.constData(), asc_origin.length());
    LOG(INFO) + str_origin.c_str();

    const QByteArray asc_message = message.toLatin1();
    rho::String str_message = ::std::string(asc_message.constData(), asc_message.length());
    LOG(INFO) + str_message.c_str();
}

void QtWebEnginePage::triggerAction(QWebEnginePage::WebAction action, bool checked)
{
    switch (action) {
    case Back:
        qDebug() << "triggerAction Back";
        break;
    case Forward:
        qDebug() << "triggerAction Forward";
        break;
    case Reload:
        qDebug() << "triggerAction Reload";
        break;
    case OpenLinkInThisWindow:
        qDebug() << "triggerAction OpenLinkInThisWindow";
        break;
    default:
        break;
    }
    QWebEnginePage::triggerAction(action, checked);
}

bool QtWebEnginePage::acceptNavigationRequest(const QUrl &url, QWebEnginePage::NavigationType type, bool isMainFrame)
{
#ifdef DEBUG
    switch (type) {
    case QWebEnginePage::NavigationTypeLinkClicked:
        qDebug() << "acceptNavigationRequest" << "NavigationTypeLinkClicked" << url.toString();
        break;
    case QWebEnginePage::NavigationTypeTyped:
        qDebug() << "acceptNavigationRequest" << "NavigationTypeTyped" << url.toString();
        break;
    case QWebEnginePage::NavigationTypeFormSubmitted:
        qDebug() << "acceptNavigationRequest" << "NavigationTypeFormSubmitted" << url.toString();
        break;
    case QWebEnginePage::NavigationTypeBackForward:
        qDebug() << "acceptNavigationRequest" << "NavigationTypeBackForward" << url.toString();
        break;
    case QWebEnginePage::NavigationTypeReload:
        qDebug() << "acceptNavigationRequest" << "NavigationTypeReload" << url.toString();
        break;
    case QWebEnginePage::NavigationTypeOther:
        qDebug() << "acceptNavigationRequest" << "NavigationTypeOther" << url.toString();
        break;
    case QWebEnginePage::NavigationTypeRedirect:
        qDebug() << "acceptNavigationRequest"  << "NavigationTypeRedirect" << url.toString();
        break;
    default:
        break;
    }
#endif



    if (type == QWebEnginePage::NavigationTypeLinkClicked){
        QTimer::singleShot(1, this, [url, this](){
            LOG(INFO) + "Request to url has been manualy reconfigured from link clicked to loading link in webview";
            webView->stop();
            webView->load(url);
        });
        return false;
        //emit onLinkClicked(url);
    }
    //emit linkClicked();
    bool flag = QWebEnginePage::acceptNavigationRequest(url, type, isMainFrame);
    return flag;
}
