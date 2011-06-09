#include "QtWebInspector.h"
#include "ui_QtWebInspector.h"

QtWebInspector::QtWebInspector(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QtWebInspector)
{
    ui->setupUi(this);
    this->move(416,0);
}

QtWebInspector::~QtWebInspector()
{
}

QWebInspector* QtWebInspector::webInspector(void)
{
    return ui->webInspector;
}

void QtWebInspector::addWebInspector(QWebInspector* wi)
{
    wi->setMaximumSize(0,0);
    wi->setParent(this);
    ui->verticalLayout->addWidget(wi);
    wi->show();
}

void QtWebInspector::removeWebInspector(QWebInspector* wi)
{
    wi->setParent(0);
    ui->verticalLayout->removeWidget(wi);
}

void QtWebInspector::showWebInspector(QWebInspector* wi)
{
    wi->setMaximumSize(QWIDGETSIZE_MAX,QWIDGETSIZE_MAX);
    if (ui->webInspector == wi)
        setWindowTitle("Web Inspector");
}

void QtWebInspector::hideWebInspector(QWebInspector* wi)
{
    wi->setMaximumSize(0,0);
}