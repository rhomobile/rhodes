#ifndef RHONATIVEAPICALL_H
#define RHONATIVEAPICALL_H

#include <QObject>
#include <QWebFrame>

class RhoNativeApiCall: public QObject
{
    Q_OBJECT

public:
    RhoNativeApiCall(QWebFrame* frame, QObject* parent = 0): m_frame(frame) {}
    Q_INVOKABLE const QString apiCall(const QString& msg);

public slots:
    void populateJavaScriptWindowObject();

private:
    QWebFrame* m_frame;
};

#endif // RHONATIVEAPICALL_H
