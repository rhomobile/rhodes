#include "qtwebenginepage.h"

QtWebEnginePage::QtWebEnginePage(QObject *parent) : QWebEnginePage(parent){

}

IMPLEMENT_LOGCLASS(QtWebEnginePage,"JavaScript");

void QtWebEnginePage::javaScriptConsoleMessage(const QString& message, int lineNumber, const QString& sourceID)
{
    QString origin = "Console message at " + sourceID + ":" + QString::number(lineNumber);
    const QByteArray asc_origin = origin.toLatin1();
    rho::String str_origin = ::std::string(asc_origin.constData(), asc_origin.length());
    LOG(INFO) + str_origin.c_str();

    const QByteArray asc_message = message.toLatin1();
    rho::String str_message = ::std::string(asc_message.constData(), asc_message.length());
    LOG(INFO) + str_message.c_str();
}

bool QtWebEnginePage::acceptNavigationRequest(const QUrl &url, QWebEnginePage::NavigationType type, bool isMainFrame)
{
    bool flag = QWebEnginePage::acceptNavigationRequest(url, type, isMainFrame);
    if (type == QWebEnginePage::NavigationTypeLinkClicked){
        emit onLinkClicked(url);
    }
    emit linkClicked();
    return flag;
}
