#ifndef QWEBPAGE_H
#define QWEBPAGE_H

#include <QWebEnginePage>
#include <QWebChannel>

class QWebPage : public QWebEnginePage
{
    Q_OBJECT
public:
    QWebPage(QWidget * widget);
    void addToJavaScriptWindowObject(QString name, QObject * objectName);
};

#endif // QWEBPAGE_H
