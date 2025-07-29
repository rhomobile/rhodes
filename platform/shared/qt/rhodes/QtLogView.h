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

#include <QWidget>
#include <QHBoxLayout>
#include <QTextEdit>
#include <QTimer>
#include <QString>
#include <QQueue>
#include <QApplication>
#include <QMutex>
#include <QMutexLocker>
#include "logging/RhoLogConf.h"

class QtLogView : public QWidget, public rho::ILogSink
{
    Q_OBJECT
    QTextEdit* logText;
private:
    QQueue<QString> messagesKeeper;
    QTimer timer;
    QMutex mutex;
public:
#if QT_VERSION >= 0x060000
    explicit QtLogView(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags()): QWidget(parent, f)
#else
    explicit QtLogView(QWidget *parent = 0, Qt::WindowFlags f = 0): QWidget(parent, f)
#endif
    {
        setWindowTitle("Log");
        logText = new QTextEdit();
        logText->setReadOnly(true);
        QHBoxLayout *layout = new QHBoxLayout;
        layout->addWidget(logText);
        this->setLayout(layout);
        connect(&timer, SIGNAL(timeout()), this, SLOT(processLog()));
        timer.start(100);
    }

    //~QtLogView();

    virtual void writeLogMessage( rho::String& strMsg )
    {
        QMutexLocker locker(&mutex);
        const char* ws = " \t\n\r\f\v";
        strMsg.erase(strMsg.find_last_not_of(ws) + 1);
        //logText->append(QString::fromStdString(strMsg));
        messagesKeeper.enqueue(QString::fromStdString(strMsg));
    }

    virtual int getCurPos()
    {
        return logText->toPlainText().length();
    }

    virtual void clear()
    {
        logText->clear();
    }
    static QString getOsDetails()
    {
                QString rv = "Unknown";
            #ifdef Q_OS_WIN
                    rv = "Windows";
                QSysInfo::WinVersion winVer = QSysInfo::windowsVersion ();
                switch (winVer) {
                case QSysInfo::WV_32s:
                    rv += " 3.1 with Win 32s";
                    break;
                case QSysInfo::WV_95:
                    rv += " 95";
                    break;
                case QSysInfo::WV_98:
                    rv += " 98";
                    break;
                case QSysInfo::WV_Me:
                    rv += " Me";
                    break;
                case QSysInfo::WV_4_0:
                    rv += " 4.0 (NT)";
                    break;
                case QSysInfo::WV_5_0:
                    rv += " 5.0 (2000)";
                    break;
                case QSysInfo::WV_5_1:
                    rv += " 5.1 (XP)";
                    break;
                case QSysInfo::WV_5_2:
                    rv += " 5.2 (2003)";
                    break;
                case QSysInfo::WV_6_0:
                    rv += " 6.0 (Vista)";
                    break;
                case QSysInfo::WV_6_1: // QSysInfo::WV_WINDOWS7
                    rv += " 6.1 (Win 7)";
                    break;
                case QSysInfo::WV_6_2: // QSysInfo::WV_WINDOWS8
                    rv += " 6.2 (Win 8)";
                    break;
            #if (QT_VERSION >= QT_VERSION_CHECK(5,2,0))
                case QSysInfo::WV_6_3: // QSysInfo::WV_WINDOWS8_1
                    rv += " 6.3 (Win 8.1)";
                    break;
            #endif
                case QSysInfo::WV_CE:
                    rv += " CE";
                    break;
                case QSysInfo::WV_CENET:
                    rv += " CENET";
                    break;
                case QSysInfo::WV_CE_5:
                    rv += " CE 5.x";
                    break;
                case QSysInfo::WV_CE_6:
                    rv += " CE 6.x";
                    break;
                default:
                    rv += QString(" Unknown (%1)").arg(winVer);
                    break;
                }
            #endif
            #if defined Q_WS_MAC
                rv = "Mac";
                switch (QSysInfo::MacintoshVersion)
                {
                    case QSysInfo::MV_Unknown:    rv +=  "Unknown Mac";
                    case QSysInfo::MV_9:          rv +=  "Mac OS 9";
                    case QSysInfo::MV_10_0:       rv +=  "Mac OS X 10.0";
                    case QSysInfo::MV_10_1:       rv +=  "Mac OS X 10.1";
                    case QSysInfo::MV_10_2:       rv +=  "Mac OS X 10.2";
                    case QSysInfo::MV_10_3:       rv +=  "Mac OS X 10.3";
                    case QSysInfo::MV_10_4:       rv +=  "Mac OS X 10.4";
                    case QSysInfo::MV_10_5:       rv +=  "Mac OS X 10.5";
                    case QSysInfo::MV_10_6:       rv +=  "Mac OS X 10.6";
                    case QSysInfo::MV_10_7:       rv +=  "Mac OS X 10.7";

                    default:                      rv +=  "Unknown";
                }
            #endif	
                return rv;
      } 
public slots:
    void processLog(){
        while(!messagesKeeper.isEmpty()){
            QMutexLocker locker(&mutex);
            logText->append(messagesKeeper.dequeue());
        }
        //QApplication::processEvents();

    }
};

#endif // QTLOGVIEW_H
