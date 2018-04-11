#include "qtwebengineview.h"


QtWebEngineView::QtWebEngineView(QWidget *parent) : QWebEngineView(parent)
{
    connect(page(), SIGNAL(linkHovered(QString)), this, SLOT(linkHovered(QString)));
}



void QtWebEngineView::linkHovered(QString url)
{
    emit linkClicked(QUrl(url));
}
