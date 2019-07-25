#ifndef QTWEBENGINEVIEW_H
#define QTWEBENGINEVIEW_H

#include <QObject>
#include <QWebEngineView>

class QtWebEngineView : public QWebEngineView
{
    Q_OBJECT
public:
    explicit QtWebEngineView(QWidget *parent = 0);
    /*void setPage(QWebEnginePage *page)
    {
        disconnect(this->page(), SIGNAL(linkHovered(QString)), this, SLOT(linkHovered(QString)));
        QWebEngineView::setPage(page);
        connect(this->page(), SIGNAL(linkHovered(QString)), this, SLOT(linkHovered(QString)));

    }*/
signals:
    void linkClicked(const QUrl&);
public slots:
    void linkHovered(QString url);
};

#endif // QTWEBENGINEVIEW_H
