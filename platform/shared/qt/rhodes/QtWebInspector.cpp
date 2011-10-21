/*------------------------------------------------------------------------
* (The MIT License)
*
* Copyright (c) 2008-2011 Rhomobile, Inc.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*
* http://rhomobile.com
*------------------------------------------------------------------------*/

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