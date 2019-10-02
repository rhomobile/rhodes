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

#ifndef QTLOGVIEW_H
#define QTLOGVIEW_H

#include <QTimer>
#include <QString>
#include <QQueue>
#include <QMutex>
#include <QMutexLocker>
#include "logging/RhoLogConf.h"

class QtLogView : public rho::ILogSink
{
    //Q_OBJECT
private:
    QQueue<QString> messagesKeeper;
    QTimer timer;
    QMutex mutex;
public:
    explicit QtLogView()
    {
        /*setWindowTitle("Log");
        logText = new QTextEdit();
        logText->setReadOnly(true);
        QHBoxLayout *layout = new QHBoxLayout;
        layout->addWidget(logText);
        this->setLayout(layout);
        connect(&timer, SIGNAL(timeout()), this, SLOT(processLog()));
        timer.start(100);*/
    }

    //~QtLogView();

    virtual void writeLogMessage( rho::String& strMsg )
    {
        QMutexLocker locker(&mutex);
        const char* ws = " \t\n\r\f\v";
        strMsg.erase(strMsg.find_last_not_of(ws) + 1);
        messagesKeeper.enqueue(QString::fromStdString(strMsg));
    }

    virtual int getCurPos()
    {
        return 0;//logText->toPlainText().length();
    }

    virtual void clear()
    {
        //logText->clear();
    }
    static QString getOsDetails()
    {
        QString rv = "SAILFISH";
        //QSysInfo::WinVersion winVer = QSysInfo::windowsVersion ();

        return rv;
      } 
public slots:
    void processLog(){
        while(!messagesKeeper.isEmpty()){
            QMutexLocker locker(&mutex);
            //logText->append(messagesKeeper.dequeue());
        }
    }
};

#endif // QTLOGVIEW_H
