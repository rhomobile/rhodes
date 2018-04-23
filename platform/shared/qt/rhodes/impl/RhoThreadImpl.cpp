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
#include <QApplication>
using namespace rho::common;

IMPLEMENT_LOGCLASS(CRhoThreadImpl,"RhoThreadQT");

CRhoThreadImpl::CRhoThreadImpl(): m_Thread(0), m_waitThread(0) 
{
    threadId = QString::number((quint64)this, 16).prepend("0x").toStdString();
}

CRhoThreadImpl::~CRhoThreadImpl()
{

    LOG(INFO) + "RHOQT " + threadId + " CRhoThreadImpl destructor";
    stopWait();
    stop(0);

    #if defined(NEED_MUTEX_LOCKERS)
        LOG(INFO) + "RHOQT " + threadId + " ~CRhoThreadImpl - before entering mutex locker gCS";
        QMutexLocker gCSLocker(&gCS);
        LOG(TRACE) + "RHOQT " + threadId + " ~CRhoThreadImpl - after entering mutex locker gCS";
    #endif

    if (m_Thread) 
    {
        LOG(TRACE) + "RHOQT " + threadId + " CRhoThreadImpl destructor before delete m_Thread";
		delete m_Thread;
        m_Thread = 0;
        LOG(TRACE) + "RHOQT " + threadId + " CRhoThreadImpl destructor after delete m_Thread";
	}
    #if defined(NEED_MUTEX_LOCKERS)
        LOG(TRACE) + "RHOQT " + threadId + " ~CRhoThreadImpl - before unlock mutex gCS";
        gCSLocker.unlock();
        LOG(TRACE) + "RHOQT " + threadId + " ~CRhoThreadImpl - after unlock mutex gCS";
    #endif

    if (m_waitThread){
        LOG(TRACE) + "RHOQT " + threadId + " CRhoThreadImpl destructor before delete m_waitThread ";
		delete m_waitThread;
        m_waitThread = 0;
        LOG(TRACE) + "RHOQT " + threadId + " CRhoThreadImpl destructor after delete m_waitThread ";
	}

}

void CRhoThreadImpl::start(IRhoRunnable* pRunnable, IRhoRunnable::EPriority ePriority)
{
    LOG(TRACE) + "RHOQT " + threadId + " start";
    if (m_Thread)
	{
        LOG(TRACE) + "RHOQT " + threadId + " start-m_Thread exists before calling stop";
		stop(0);
        LOG(TRACE) + "RHOQT " + threadId + " start-m_Thread exists after calling stop";
	}
	else
	{
        LOG(TRACE) + "RHOQT " + threadId + " start-m_Thread does not exist do not call stop";
	}
    LOG(TRACE) + "RHOQT " + threadId + " start-before Qthread start";
    m_Thread = new QRhoThread(pRunnable);

    QObject::connect(qApp, SIGNAL(destroyed(QObject*)), m_Thread, SLOT(terminate()));

    m_Thread->start();
    LOG(TRACE) + "RHOQT " + threadId + " start-after Qthread start";
    setThreadPriority(ePriority);
    LOG(TRACE) + "RHOQT " + threadId + " start-finish aftersetThreadPriority";

}

void CRhoThreadImpl::setThreadPriority(IRhoRunnable::EPriority ePriority)
{
    QThread::Priority nPriority = QThread::NormalPriority;
    if ( ePriority == IRhoRunnable::epHigh ) nPriority = QThread::HighestPriority;
    else if (ePriority == IRhoRunnable::epLow) nPriority = QThread::LowestPriority;
    m_Thread->setPriority(nPriority);
}

void CRhoThreadImpl::stop(unsigned int nTimeoutToKill)
{
    LOG(INFO) + "RHOQT " + threadId + " stop";

    #if defined(NEED_MUTEX_LOCKERS)
        LOG(TRACE) + "RHOQT " + threadId + " stop-before enter entering mutex locker gCS";
        QMutexLocker locker(&gCS);
        LOG(TRACE) + "RHOQT " + threadId + " stop-after enter entering mutex locker gCS";
    #endif

    LOG(TRACE) + "RHOQT " + threadId + " stop-before calling stopWait";
    stopWait();
    LOG(TRACE) + "RHOQT " + threadId + " stop-after calling stopWait";

    if ( m_Thread ) {
        LOG(TRACE) + "RHOQT " + threadId + " stop-m_Thread exists before m_Thread->quit()";
        m_Thread->quit();
        LOG(TRACE) + "RHOQT " + threadId + " stop-m_Thread exists after m_Thread->quit()";
        //m_Thread->wait(nTimeoutToKill);
        if (!m_Thread->isRunning()){
            LOG(TRACE) + "RHOQT " + threadId + " stop-m_Thread not running before m_Thread->terminate();";
            m_Thread->terminate();
            LOG(TRACE) + "RHOQT " + threadId + " stop-m_Thread not running after m_Thread->terminate()";
        }
        LOG(TRACE) + "RHOQT " + threadId + " stop-m_Thread before  m_Thread->wait()";
        m_Thread->wait(nTimeoutToKill);
        LOG(TRACE) + "RHOQT " + threadId + " stop-m_Thread after  m_Thread->wait()";

        LOG(TRACE) + "RHOQT " + threadId + " stop-before  delete m_Thread";
        delete m_Thread;
        LOG(TRACE) + "RHOQT " + threadId + " stop-after delete m_Thread";
        m_Thread = 0;
    }

    LOG(TRACE) + "RHOQT stop-finish";
}

int CRhoThreadImpl::wait(unsigned int nTimeoutMs)
{
    LOG(INFO) + "RHOQT " + threadId + " wait";
	bool isVeyBigTimeoutvalue = false;
    
    if ((nTimeoutMs == 4294966296)||(nTimeoutMs == 4294967295)){
		isVeyBigTimeoutvalue = true;
	}

    if (m_waitThread){
        LOG(INFO) + "RHOQT " + threadId + " wait-m_waitThread exists before calling stopWait";
        stopWait();
        LOG(TRACE) + "RHOQT " + threadId + " wait-m_waitThread exists after calling stopWait";
    }else{
        LOG(TRACE) + "RHOQT " + threadId + " wait-m_waitThread does not exist do not call stopWait";
	}

	LOG(TRACE) + "RHOQT wait-before m_waitThread->start";
    m_waitThread = new QtThread();
    QObject::connect(qApp, SIGNAL(destroyed(QObject*)), m_waitThread, SLOT(terminate()));
    m_waitThread->start();
    LOG(INFO) + "RHOQT " + threadId + " wait-after m_waitThread->start";
	bool result;
	
	if(isVeyBigTimeoutvalue)
	{
        LOG(INFO) + "RHOQT " + threadId + " wait-before wait for a long time nTimeoutMs:-  "+nTimeoutMs;
        result = m_waitThread->wait(1000UL*nTimeoutMs) ? 0 : 1;
        LOG(INFO) + "RHOQT " + threadId + " wait-after wait for a long time Result:-  "+result;
    }else{
        LOG(INFO) + "RHOQT " + threadId + " wait-before wait for a short time nTimeoutMs:-  "+nTimeoutMs;
        result = m_waitThread->wait(1UL*nTimeoutMs) ? 0 : 1;
        LOG(INFO) + "RHOQT " + threadId + " wait-after wait for a short time Result:-  "+result;
	}

    if(m_waitThread){
    #if defined(NEED_MUTEX_LOCKERS)
        LOG(INFO) + "RHOQT " + threadId + " wait-m_waitThread exists again-before entering mutex locker";
        QMutexLocker lock(&gCSstopwait);
        LOG(TRACE) + "RHOQT " + threadId + " wait-m_waitThread exists again-after entering mutex locker";
    #endif
        if(m_waitThread){
            LOG(INFO) + "RHOQT " + threadId + " wait:-m_waitThread exists again 2-before Delete m_waitThread";
            delete m_waitThread;
            LOG(INFO) + "RHOQT " + threadId + " wait:-m_waitThread exists again 2-after Delete m_waitThread";
            m_waitThread = 0;
        }else{
            LOG(INFO) + "RHOQT " + threadId + " wait:-m_waitThread does not exist again 2-do not Delete m_waitThread";
        }
	}
    LOG(TRACE) + "RHOQT " + threadId + " wait-finish before return ";
    return result;
}

void CRhoThreadImpl::stopWait()
{

    LOG(INFO) + "RHOQT " + threadId + " stopWait";
    LOG(INFO) + "RHOQT " + threadId + " stopWait- before 100 ms sleep";
	QRhoThread::msleep(100);
    LOG(TRACE) + "RHOQT " + threadId + " stopWait- after 100 ms sleep";
    if (m_waitThread) {
    #if defined(NEED_MUTEX_LOCKERS)
        LOG(INFO) + "RHOQT " + threadId + " stopWait-m_waitThread exists -before entering mutex locker";
        QMutexLocker lock(&gCSstopwait);
        LOG(TRACE) + "RHOQT " + threadId + " stopWait-m_waitThread exists -after entering mutex locker";
    #endif
        if (m_waitThread) {
            LOG(INFO) + "RHOQT " + threadId + " stopWait-m_waitThread exists again- before Terminate waitthread";
            m_waitThread->terminate();
            LOG(TRACE) + "RHOQT " + threadId + " stopWait-m_waitThread exists again- after Terminate waitthread";
        }else{
            LOG(INFO) + "RHOQT " + threadId + " stopWait-m_waitThread does not exist now- do not Terminate waitthread";
        }
    }else{
        LOG(TRACE) + "RHOQT " + threadId + " stopWait-m_waitThread does not exist-do not try to terminate";
	}

}

void CRhoThreadImpl::sleep(unsigned int nTimeout)
{
    QRhoThread::sleep(nTimeout);
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
