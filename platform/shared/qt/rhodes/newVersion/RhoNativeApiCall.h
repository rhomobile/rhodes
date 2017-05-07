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

#ifndef RHONATIVEAPICALL_H
#define RHONATIVEAPICALL_H

#include <QObject>
#include <QWebEnginePage>
#include <QWebChannel>
#include <QFile>
#include <QApplication>
#include <QEventLoop>
#include <QTimer>
#include <QThread>
#include <QMutex>
#include <QWebEngineProfile>
#include <QWebEngineScript>
#include <QJsonObject>
#include <QJsonValue>

class RhoNativeApiCall: public QObject
{
    Q_OBJECT

public:
    RhoNativeApiCall(QWebEnginePage* page, QObject* parent = 0);
    ~RhoNativeApiCall();
    Q_INVOKABLE const QString toLowerCase(const QString& msg);
    Q_INVOKABLE QString apiCall(const QString& msg, bool async);

    Q_INVOKABLE QString read(){return currentResult;}
    Q_INVOKABLE void write(const QString &value){currentResult = value; resultChanged(value); qApp->processEvents();}
    QString currentResult;
    Q_PROPERTY(QString CurrentResult READ read NOTIFY resultChanged)

private:
    QTimer timer;
    QEventLoop loop;
    QMutex mutex;
public slots:
    void populateJavaScriptWindowObject();

signals:
    void resultChanged(QString);

private:
    QWebEnginePage* m_page;


};

Q_DECLARE_METATYPE(RhoNativeApiCall*)
Q_DECLARE_METATYPE(QString)
Q_DECLARE_METATYPE(QJsonObject)
Q_DECLARE_METATYPE(QJsonValue)

#endif // RHONATIVEAPICALL_H
