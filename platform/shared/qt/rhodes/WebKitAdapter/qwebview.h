#ifndef QWEBVIEW_H
#define QWEBVIEW_H

#include <QObject>
#include <QWebEngineView>
#include "qwebpage.h"

class QWebView : public QWebEngineView
{
    Q_OBJECT
public:
    explicit QWebView(QWidget *parent = 0);


public slots:
};

#endif // QWEBVIEW_H
