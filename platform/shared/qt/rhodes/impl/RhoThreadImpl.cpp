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

#include "common/RhoDefs.h"

#ifdef OS_WINDOWS_DESKTOP

#include "common/RhoPort.h"
#include "RhoThreadImpl.h"
#ifndef RHODES_VERSION_LIBRARY
#include <QApplication>
#endif
using namespace rho::common;


#ifndef RHODES_VERSION_LIBRARY
IMPLEMENT_LOGCLASS(CRhoThreadImpl,"RhoThreadQT");
#define RHOQTPREFIX ("RHOQT " + threadId + " ")
#else
IMPLEMENT_LOGCLASS(CRhoThreadImpl,"RhoThreadSTD");
#define RHOQTPREFIX ("RHOSTD " + threadId + " ")
#endif

CRhoThreadImpl::CRhoThreadImpl()
#ifndef RHODES_VERSION_LIBRARY
    : m_waitThread(new QtThread())
#endif
{
#ifndef RHODES_VERSION_LIBRARY
    mutex = QSharedPointer<QMutex>(new QMutex());
    mutexWaiter = QSharedPointer<QMutex>(new QMutex());
    threadId = QString::number((quint64)this, 16).prepend("0x").toStdString();
    QObject::connect(qApp, SIGNAL(destroyed(QObject*)), m_waitThread.data(), SLOT(quit()));
#else
    threadId = std::to_string(GetThreadId(GetCurrentThread()));
#endif
}

CRhoThreadImpl::~CRhoThreadImpl()
{
#ifndef RHODES_VERSION_LIBRARY
    LOG(TRACE) + RHOQTPREFIX + "CRhoThreadImpl destructor";
    stopWait();
    stop(0);

    LOG(TRACE) + RHOQTPREFIX + "~CRhoThreadImpl - before entering mutex locker";
    QSharedPointer<QMutex> l_mutex = mutex;
    QMutexLocker locker(l_mutex.data());
    LOG(TRACE) + RHOQTPREFIX + "~CRhoThreadImpl - after entering mutex locker";

    if (!m_Thread.isNull())
    {
        LOG(TRACE) + RHOQTPREFIX + "CRhoThreadImpl destructor before delete m_Thread";
        m_Thread = QSharedPointer<QRhoThread>();
        LOG(TRACE) + RHOQTPREFIX + "CRhoThreadImpl destructor after delete m_Thread";
	}  
#endif
}

void CRhoThreadImpl::start(IRhoRunnable* pRunnable, IRhoRunnable::EPriority ePriority)
{
    LOG(TRACE) + RHOQTPREFIX + "start";
    LOG(TRACE) + RHOQTPREFIX + "start";
    if (
        #ifndef RHODES_VERSION_LIBRARY
            !m_Thread.isNull()
        #else
            m_Thread
        #endif
       )
	{
        LOG(TRACE) + RHOQTPREFIX + "start - m_Thread exists before calling stop";
		stop(0);
        LOG(TRACE) + RHOQTPREFIX + "start - m_Thread exists after calling stop";
    }else{
        LOG(TRACE) + RHOQTPREFIX + "start - m_Thread does not exist do not call stop";
	}
    LOG(TRACE) + RHOQTPREFIX + "start - before thread start";
#ifndef RHODES_VERSION_LIBRARY
    m_Thread = QSharedPointer<QRhoThread>(new QRhoThread(pRunnable));

    QObject::connect(qApp, SIGNAL(destroyed(QObject*)), m_Thread.data(), SLOT(terminate()));

    m_Thread->start();
    LOG(TRACE) + RHOQTPREFIX + "start - after Qthread start";
    setThreadPriority(ePriority);
    LOG(TRACE) + RHOQTPREFIX + "start - finish aftersetThreadPriority";
#else
    m_Thread = std::make_shared<QRhoThread>(pRunnable);
    m_Thread->start();
    setThreadPriority(ePriority);
    LOG(TRACE) + RHOQTPREFIX + "start - after thread start";
#endif
}

void CRhoThreadImpl::setThreadPriority(IRhoRunnable::EPriority ePriority)
{
#ifndef RHODES_VERSION_LIBRARY
    QThread::Priority nPriority = QThread::NormalPriority;
    if ( ePriority == IRhoRunnable::epHigh ) nPriority = QThread::HighestPriority;
    else if (ePriority == IRhoRunnable::epLow) nPriority = QThread::LowestPriority;
    m_Thread.data()->setPriority(nPriority);
#else
    int nPriority = THREAD_PRIORITY_NORMAL;
    if ( ePriority == IRhoRunnable::epHigh )
        nPriority = THREAD_PRIORITY_HIGHEST;
    else if (ePriority == IRhoRunnable::epLow)
        nPriority = THREAD_PRIORITY_LOWEST;
    else if (ePriority == IRhoRunnable::epCritical)
        nPriority = THREAD_PRIORITY_TIME_CRITICAL;

    SetThreadPriority(m_Thread->_thr.native_handle(), nPriority);
#endif
}

void CRhoThreadImpl::stop(unsigned int nTimeoutToKill)
{
#ifndef RHODES_VERSION_LIBRARY
    LOG(TRACE) + RHOQTPREFIX + "stop";

    LOG(TRACE) + RHOQTPREFIX + "stop - before enter entering mutex locker";
    QSharedPointer<QMutex> l_mutex = mutex;
    QMutexLocker locker(l_mutex.data());
    LOG(TRACE) + RHOQTPREFIX + "stop - after enter entering mutex locker";


    LOG(TRACE) + RHOQTPREFIX + "stop - before calling stopWait";
    stopWait();
    LOG(TRACE) + RHOQTPREFIX + "stop - after calling stopWait";

    if ( !m_Thread.isNull() ) {
        LOG(TRACE) + RHOQTPREFIX + "stop - m_Thread exists before m_Thread->quit()";
        m_Thread.data()->quit();
        LOG(TRACE) + RHOQTPREFIX + "stop - m_Thread exists after m_Thread->quit()";
        if (!m_Thread.data()->isRunning()){
            LOG(TRACE) + RHOQTPREFIX + "stop - m_Thread not running before m_Thread->terminate();";
            m_Thread.data()->terminate();
            LOG(TRACE) + RHOQTPREFIX + "stop - m_Thread not running after m_Thread->terminate()";
        }
        if (nTimeoutToKill){
            LOG(TRACE) + RHOQTPREFIX + "stop - m_Thread before  m_Thread->wait()";
            m_Thread.data()->wait(nTimeoutToKill);
            LOG(TRACE) + RHOQTPREFIX + "stop - m_Thread after  m_Thread->wait()";
        }
        LOG(TRACE) + RHOQTPREFIX + "stop - before  delete m_Thread";
        m_Thread = QSharedPointer<QRhoThread>();
        LOG(TRACE) + RHOQTPREFIX + "stop - after delete m_Thread";
    }

    LOG(TRACE) + "RHOQT stop - finish";
#else
    if(m_Thread)
    {
        stopWait();
        if(nTimeoutToKill && static_cast<int>(nTimeoutToKill) > 0)
            m_Thread->wait(nTimeoutToKill);
        m_Thread.reset();
    }
#endif
}

int CRhoThreadImpl::wait(unsigned int nTimeoutMs)
{
#ifndef RHODES_VERSION_LIBRARY
    LOG(TRACE) + RHOQTPREFIX + "wait";
	bool isVeyBigTimeoutvalue = false; 
    if ((nTimeoutMs == 4294966296)||(nTimeoutMs == 4294967295)){isVeyBigTimeoutvalue = true;}

    LOG(TRACE) + RHOQTPREFIX + "wait - m_waitThread exists before calling stopWait";
    stopWait();
    LOG(TRACE) + RHOQTPREFIX + "wait - m_waitThread exists after calling stopWait";

    QSharedPointer<QMutex> l_mutexWaiter = mutexWaiter;
    QMutexLocker waitLocker(l_mutexWaiter.data());

    QSharedPointer<QtThread> l_waitThread = m_waitThread;
    LOG(TRACE) + RHOQTPREFIX + "wait - before m_waitThread->start";
    l_waitThread.data()->start();
    LOG(TRACE) + RHOQTPREFIX + "wait - after m_waitThread->start";
	bool result;
	if(isVeyBigTimeoutvalue)
	{
        LOG(TRACE) + RHOQTPREFIX + "wait - before wait for a long time nTimeoutMs:-  "+nTimeoutMs;
        waitLocker.unlock();
        result = l_waitThread.data()->wait(1000UL*nTimeoutMs) ? 0 : 1;
        LOG(TRACE) + RHOQTPREFIX + "wait - after wait for a long time Result:-  "+result;
    }else{
        LOG(TRACE) + RHOQTPREFIX + "wait - before wait for a short time nTimeoutMs:-  "+nTimeoutMs;
        waitLocker.unlock();
        result = l_waitThread.data()->wait(1UL*nTimeoutMs) ? 0 : 1;
        LOG(TRACE) + RHOQTPREFIX + "wait - after wait for a short time Result:-  "+result;
	}
    LOG(TRACE) + RHOQTPREFIX + "wait - finish before return";
    return result;
#else
    LOG(TRACE) + RHOQTPREFIX + "wait - before wait for a long time nTimeoutMs:-  "+nTimeoutMs;
    return m_Thread->wait(nTimeoutMs);
#endif
}

void CRhoThreadImpl::stopWait()
{
#ifndef RHODES_VERSION_LIBRARY
    LOG(TRACE) + RHOQTPREFIX + "stopWait - before mutex";
    QSharedPointer<QMutex> l_mutexWaiter = mutexWaiter;
    QMutexLocker waitLocker(l_mutexWaiter.data());
    LOG(TRACE) + RHOQTPREFIX + "stopWait - after mutex and before msleep";
    QRhoThread::msleep(2);
    LOG(TRACE) + RHOQTPREFIX + "stopWait - before quit()";
    m_waitThread.data()->quit();
    LOG(TRACE) + RHOQTPREFIX + "stopWait - after quit()";
#else
    m_Thread->stopWait();
#endif
}

void CRhoThreadImpl::sleep(unsigned int nTimeout)
{
#ifndef RHODES_VERSION_LIBRARY
    QRhoThread::sleep(nTimeout);
#else
    std::chrono::duration<uint64_t, std::milli> _timeout (static_cast<uint64_t>(nTimeout));
    std::this_thread::sleep_for(_timeout);
#endif
}

#endif // OS_WINDOWS_DESKTOP

extern "C" {

void* rho_nativethread_start()
{
    //TODO: rho_nativethread_start
    return 0;
}

void rho_nativethread_end(void* pData)
{
    //TODO: rho_nativethread_end
}

} //extern "C"
