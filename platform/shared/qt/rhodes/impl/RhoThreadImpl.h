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

#ifndef RHOTHREADIMPL_H
#define RHOTHREADIMPL_H

#include <QThread>
#include "common/IRhoThreadImpl.h"
#include "logging/RhoLog.h"
#include <QMutex>
#include <QMutexLocker>

namespace rho{
namespace common{

class CRhoThreadImpl : public IRhoThreadImpl
{
    DEFINE_LOGCLASS

public:
    CRhoThreadImpl();
    virtual ~CRhoThreadImpl();
    virtual void start(IRhoRunnable* pRunnable, IRhoRunnable::EPriority ePriority);
    virtual void stop(unsigned int nTimeoutToKill);
    virtual int wait(unsigned int nTimeout);
    virtual void stopWait();
    virtual void sleep(unsigned int nTimeout);
private:
    void setThreadPriority(IRhoRunnable::EPriority ePriority);
private:
    class QRhoThread: public QThread
    {
    public:
        QRhoThread(IRhoRunnable* pRunnable): QThread(), m_Runnable(pRunnable) {}
        void run() { m_Runnable->runObject(); }
        static void sleep(unsigned long timeout) { QThread::sleep(timeout); }
    private:
        IRhoRunnable* m_Runnable;
    };
    QRhoThread* m_Thread;
    QThread* m_waitThread;
    QMutex mutex;

    #if defined(OS_WINDOWS_DESKTOP)
        CRITICAL_SECTION gCS;
        CRITICAL_SECTION gCSstopwait;
    #endif
};

}
}

#endif // RHOTHREADIMPL_H
