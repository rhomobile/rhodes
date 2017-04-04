#ifndef QTWEBENGINEVIEW_H
#define QTWEBENGINEVIEW_H

#include <QObject>
#include <QWebEngineView>

class QtWebEngineView : public QWebEngineView
{
    Q_OBJECT
public:
    explicit QtWebEngineView(QWidget *parent = 0);

signals:

public slots:
};

#endif // QTWEBENGINEVIEW_H
