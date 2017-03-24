#ifndef QTWEBENGINEPAGE_H
#define QTWEBENGINEPAGE_H

#include <QWebEnginePage>
#include <QString>
#include "logging/RhoLog.h"

class QtWebEnginePage : public QWebEnginePage
{
    Q_OBJECT
    DEFINE_LOGCLASS
public:
    explicit QtWebEnginePage(QObject *parent = 0);

    virtual void javaScriptConsoleMessage(const QString &message, int lineNumber, const QString &sourceID);
signals:

public slots:
};

#endif // QTWEBENGINEPAGE_H
