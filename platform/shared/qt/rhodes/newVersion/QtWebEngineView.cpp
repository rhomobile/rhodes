#include "QtWebEngineView.h"
#include <QWebEngineSettings>

QtWebEngineView::QtWebEngineView(QWidget *parent) : QWebEngineView(parent)
{
    #ifdef DISABLE_WEB_SECURITY
    settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessFileUrls, true);
    settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessRemoteUrls, true);
    settings()->setAttribute(QWebEngineSettings::LocalStorageEnabled, true);
    #endif
    connect(page(), SIGNAL(linkHovered(QString)), this, SLOT(linkHovered(QString)));
}



void QtWebEngineView::linkHovered(QString url)
{
    emit linkClicked(QUrl(url));
}
