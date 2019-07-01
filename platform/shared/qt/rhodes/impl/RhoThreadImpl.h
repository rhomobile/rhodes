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

#include "common/IRhoThreadImpl.h"
#include "logging/RhoLog.h"

#ifndef RHODES_VERSION_LIBRARY
#include <QThread>
#include <QSharedPointer>
#include <QMutex>
#include <QMutexLocker>
#include <QTimer>
#else
#include <thread>
#include <mutex>
#include <memory>
#include <sstream>
#include <condition_variable>
#endif

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
    rho::String threadId;
private:


#ifndef RHODES_VERSION_LIBRARY
    class QtThread: public QThread
    {
    public:
        QtThread():QThread(){}
        virtual ~QtThread(){
            if (isRunning()){
                quit();
            }
            if (isRunning()){
                terminate();
            }
        }
    };

    class QRhoThread: public QtThread
    {
    public:
        QRhoThread(IRhoRunnable* pRunnable): QtThread(), m_Runnable(pRunnable) {}
        void run() { m_Runnable->runObject(); }
        static void sleep(unsigned long timeout) { QtThread::sleep(timeout); }
    private:
        IRhoRunnable* m_Runnable;
    };

    QSharedPointer<QRhoThread> m_Thread;
    QSharedPointer<QtThread> m_waitThread;
    QSharedPointer<QMutex> mutex;
    QSharedPointer<QMutex> mutexWaiter;
#else

    class QRhoThread
    {
        friend CRhoThreadImpl;
        bool runnig = false;
        std::mutex waiter;
        std::condition_variable wait_cond;
    public:
        QRhoThread(IRhoRunnable* pRunnable): runnig(false), m_Runnable(pRunnable) {}

        ~QRhoThread()
        {
            if (isRunning()){
                TerminateThread(_thr.native_handle(), 0);
            }
        }

        void run() {
            runnig = true;
            m_Runnable->runObject();
            runnig = false;
        }

        bool wait(unsigned int nTimeout)
        {
            try
            {
                std::unique_lock<std::mutex> lock(waiter);
                std::chrono::duration<uint64_t, std::milli> _timeout (static_cast<uint64_t>(nTimeout));
                wait_cond.wait_for(lock, _timeout);
                return true;
            }
            catch(std::system_error& err)
            {
                LOG(ERROR) + err.what();
                return false;
            }
        }

        void stopWait()
        {
            wait_cond.notify_one();
        }

        void start()
        {
            _thr = std::thread(&QRhoThread::run, this);
        }

        static void sleep(unsigned long timeout) {
            std::chrono::duration<uint64_t, std::milli> _timeout (static_cast<uint64_t>(timeout));
            std::this_thread::sleep_for(_timeout);
        }
        bool isRunning()
        {
            return runnig;
        }
    private:
        IRhoRunnable* m_Runnable;
        std::thread _thr;
    };
    std::shared_ptr<QRhoThread> m_Thread;
    std::mutex mutex;

#endif
};

}
}


#endif // RHOTHREADIMPL_H
