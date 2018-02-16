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
    #if defined(OS_WINDOWS_DESKTOP)
        LOG(INFO) + "RHOQT CRhoThreadImpl cons-before InitializeCriticalSections";
        InitializeCriticalSection(&gCS);
        InitializeCriticalSection(&gCSstopwait);
        LOG(TRACE) + "RHOQT CRhoThreadImpl cons-after InitializeCriticalSections";
    #endif
}

CRhoThreadImpl::~CRhoThreadImpl()
{

	LOG(INFO) + "RHOQT CRhoThreadImpl destructor";
    stopWait();
    stop(0);
    QMutexLocker locker(&mutex);
    if (m_Thread) 
	{
		LOG(TRACE) + "RHOQT CRhoThreadImpl destructor before delete m_Thread";
		delete m_Thread;
		LOG(TRACE) + "RHOQT CRhoThreadImpl destructor after delete m_Thread";
	}
    if (m_waitThread)
	{
		LOG(TRACE) + "RHOQT CRhoThreadImpl destructor before delete m_waitThread ";
		delete m_waitThread;
		LOG(TRACE) + "RHOQT CRhoThreadImpl destructor before after m_waitThread ";
	}
    locker.unlock();

    #if defined(OS_WINDOWS_DESKTOP)
        LOG(TRACE) + "RHOQT CRhoThreadImpl destructor-before DeleteCriticalSection stopwait";
        DeleteCriticalSection(&gCSstopwait);
        LOG(TRACE) + "RHOQT CRhoThreadImpl destructor-after DeleteCriticalSection stopwait";
        LOG(TRACE) + "RHOQT CRhoThreadImpl destructor-before DeleteCriticalSection stop";
        DeleteCriticalSection(&gCS);
        LOG(TRACE) + "RHOQT CRhoThreadImpl destructor-after DeleteCriticalSection stop";
    #endif
}

void CRhoThreadImpl::start(IRhoRunnable* pRunnable, IRhoRunnable::EPriority ePriority)
{
    QMutexLocker locker(&mutex);
    LOG(TRACE) + "RHOQT start";
    if (m_Thread)
	{
        LOG(TRACE) + "RHOQT start-m_Thread exists before calling stop";
		stop(0);
		LOG(TRACE) + "RHOQT start-m_Thread exists after calling stop";
	}
	else
	{
        LOG(TRACE) + "RHOQT start-m_Thread does not exist do not call stop";
	}
    LOG(TRACE) + "RHOQT start-before Qthread start";
    m_Thread = new QRhoThread(pRunnable);

    QObject::connect(qApp, SIGNAL(aboutToQuit()), m_Thread, SLOT(quit()));
    QObject::connect(qApp, SIGNAL(aboutToQuit()), m_Thread, SLOT(deleteLater()));

    m_Thread->start();
    LOG(TRACE) + "RHOQT start-after Qthread start";
    setThreadPriority(ePriority);
    LOG(TRACE) + "RHOQT start-finish aftersetThreadPriority";

    locker.unlock();
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
    LOG(INFO) + "RHOQT stop";


    #if defined(OS_WINDOWS_DESKTOP)
        LOG(TRACE) + "RHOQT stop-before enter Crticalsection";
        EnterCriticalSection(&gCS);
        LOG(TRACE) + "RHOQT stop-after enter Crticalsection";
    #endif

    LOG(TRACE) + "RHOQT stop-before calling stopWait";
    stopWait();
    LOG(TRACE) + "RHOQT stop-after calling stopWait";

    if ( m_Thread ) {
        LOG(TRACE) + "RHOQT stop-m_Thread exists before m_Thread->quit()";
        m_Thread->quit();
        LOG(TRACE) + "RHOQT stop-m_Thread exists after m_Thread->quit()";
        //m_Thread->wait(nTimeoutToKill);
        if (!m_Thread->isRunning()){
            LOG(TRACE) + "RHOQT stop-m_Thread not running before m_Thread->terminate();";
            m_Thread->terminate();
            LOG(TRACE) + "RHOQT stop-m_Thread not running after m_Thread->terminate()";
            LOG(TRACE) + "RHOQT stop-m_Thread not running before  m_Thread->wait()";
            m_Thread->wait();
            LOG(TRACE) + "RHOQT stop-m_Thread not running after  m_Thread->wait()";
        }else{
            LOG(TRACE) + "RHOQT stop-m_Thread running-before  m_Thread->wait()";
            m_Thread->wait();
            LOG(TRACE) + "RHOQT stop-m_Thread running-after  m_Thread->wait()";
        }
        LOG(TRACE) + "RHOQT stop-before  delete m_Thread";
        delete m_Thread;
        LOG(TRACE) + "RHOQT stop-after delete m_Thread";
        m_Thread = 0;
    }

    #if defined(OS_WINDOWS_DESKTOP)
        LOG(TRACE) + "RHOQT stop-before LeaveCriticalSection";
        LeaveCriticalSection(&gCS);
        LOG(TRACE) + "RHOQT stop-after LeaveCriticalSection";
    #endif
    LOG(TRACE) + "RHOQT stop-finish";
}

int CRhoThreadImpl::wait(unsigned int nTimeoutMs)
{
    LOG(INFO) + "RHOQT wait";
	bool isVeyBigTimeoutvalue = false;
    
    if ((nTimeoutMs == 4294966296)||(nTimeoutMs == 4294967295)){
		isVeyBigTimeoutvalue = true;
	}
    QMutexLocker locker0(&mutex);

    if (m_waitThread){
        LOG(INFO) + "RHOQT wait-m_waitThread exists before calling stopWait";
        locker0.unlock();
        stopWait();
        LOG(TRACE) + "RHOQT wait-m_waitThread exists after calling stopWait";
    }else{
        LOG(TRACE) + "RHOQT wait-m_waitThread does not existdo not call stopWait";
	}
    locker0.unlock();

    QMutexLocker locker1(&mutex);
	LOG(TRACE) + "RHOQT wait-before m_waitThread->start";
    m_waitThread = new QThread();
    QObject::connect(qApp, SIGNAL(aboutToQuit()), m_waitThread, SLOT(quit()));
    QObject::connect(qApp, SIGNAL(aboutToQuit()), m_waitThread, SLOT(deleteLater()));
    m_waitThread->start();
	LOG(INFO) + "RHOQT wait-after m_waitThread->start";
    locker1.unlock();
	bool result;
	
	if(isVeyBigTimeoutvalue)
	{
        LOG(INFO) + "RHOQT wait-before wait for a long time nTimeoutMs:-  "+nTimeoutMs;
        result = m_waitThread->wait(1000UL*nTimeoutMs) ? 0 : 1;
        LOG(INFO) + "RHOQT wait-after wait for a long time Result:-  "+result;
    }else{
        LOG(INFO) + "RHOQT wait-before wait for a short time nTimeoutMs:-  "+nTimeoutMs;
        result = m_waitThread->wait(1UL*nTimeoutMs) ? 0 : 1;
        LOG(INFO) + "RHOQT wait-after wait for a short time Result:-  "+result;
	}

    QMutexLocker locker2(&mutex);
    if(m_waitThread){
        LOG(INFO) + "RHOQT wait-m_waitThread exists again-before EnterCriticalSection";
    #if defined(OS_WINDOWS_DESKTOP)
        EnterCriticalSection(&gCSstopwait);
        LOG(TRACE) + "RHOQT wait-m_waitThread exists again-after EnterCriticalSection";
    #endif
        if(m_waitThread){
            LOG(INFO) + "RHOQT wait:-m_waitThread exists again 2-before Delete m_waitThread";
            m_waitThread->terminate();
            m_waitThread->deleteLater();
            LOG(INFO) + "RHOQT wait:-m_waitThread exists again 2-after Delete m_waitThread";
            m_waitThread = 0;
        }else{
            LOG(INFO) + "RHOQT wait:-m_waitThread does not exist again 2-do not Delete m_waitThread";
        }
#if defined(OS_WINDOWS_DESKTOP)
    LOG(TRACE) + "RHOQT wait-m_waitThread exists again-before LeaveCriticalSection";
	LeaveCriticalSection(&gCSstopwait);
    LOG(TRACE) + "RHOQT wait-m_waitThread exists again-after LeaveCriticalSection";
#endif
	}
	LOG(TRACE) + "RHOQT wait-finish before return ";
    return result;
}

void CRhoThreadImpl::stopWait()
{

    LOG(INFO) + "RHOQT stopWait"; 
	LOG(INFO) + "RHOQT stopWait- before 100 ms sleep";
	QRhoThread::msleep(100);
    LOG(TRACE) + "RHOQT stopWait- after 100 ms sleep";
    QMutexLocker locker(&mutex);
    if (m_waitThread) {
    #if defined(OS_WINDOWS_DESKTOP)
        LOG(INFO) + "RHOQT stopWait-m_waitThread exists -before EnterCriticalSection";
        EnterCriticalSection(&gCSstopwait);
        LOG(TRACE) + "RHOQT stopWait-m_waitThread exists -after EnterCriticalSection";
    #endif
        if (m_waitThread) {
            LOG(INFO) + "RHOQT stopWait-m_waitThread exists again- before Terminate waitthread";
            m_waitThread->terminate();
            LOG(TRACE) + "RHOQT stopWait-m_waitThread exists again- after Terminate waitthread";
        }else{
            LOG(INFO) + "RHOQT stopWait-m_waitThread does not exist now- do not Terminate waitthread";
        }

    #if defined(OS_WINDOWS_DESKTOP)
        LOG(TRACE) + "RHOQT stopWait-m_waitThread exists-before LeaveCriticalSection";
        LeaveCriticalSection(&gCSstopwait);
        LOG(TRACE) + "RHOQT stopWait-m_waitThread exists-after LeaveCriticalSection";
    #endif

    }else{
        LOG(TRACE) + "RHOQT stopWait-m_waitThread does not exist-do not try to terminate";
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
