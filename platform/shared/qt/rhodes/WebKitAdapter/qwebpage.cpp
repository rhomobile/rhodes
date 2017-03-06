#include "qwebpage.h"

QWebPage::QWebPage(QWidget *widget) : QWebEnginePage(widget)
{

}

void QWebPage::addToJavaScriptWindowObject(QString name, QObject *objectName)
{
    webChannel()->registerObject(name, objectName);
}
