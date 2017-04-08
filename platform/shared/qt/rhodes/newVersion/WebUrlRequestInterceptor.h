#ifndef WEBURLREQUESTINTERCEPTOR_H
#define WEBURLREQUESTINTERCEPTOR_H

#include <QObject>
#include <QWebEngineUrlRequestInterceptor>

class WebUrlRequestInterceptor : public QWebEngineUrlRequestInterceptor
{
    Q_OBJECT
public:
    explicit WebUrlRequestInterceptor(QObject *parent = 0);

    void interceptRequest(QWebEngineUrlRequestInfo &info);
signals:

public slots:
};

#endif // WEBURLREQUESTINTERCEPTOR_H
