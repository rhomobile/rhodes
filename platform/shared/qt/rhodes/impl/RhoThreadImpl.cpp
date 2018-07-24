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
#define RHOQTPREFIX ("RHOQT " + threadId + " ")

CRhoThreadImpl::CRhoThreadImpl(): m_Thread(0), m_waitThread(new QtThread())
{
    threadId = QString::number((quint64)this, 16).prepend("0x").toStdString();
    QObject::connect(qApp, SIGNAL(destroyed(QObject*)), m_waitThread.data(), SLOT(quit()));
}

CRhoThreadImpl::~CRhoThreadImpl()
{

    LOG(INFO) + RHOQTPREFIX + "CRhoThreadImpl destructor";
    stopWait();
    stop(0);

    LOG(INFO) + RHOQTPREFIX + "~CRhoThreadImpl - before entering mutex locker";
    QMutexLocker locker(&mutex);
    LOG(TRACE) + RHOQTPREFIX + "~CRhoThreadImpl - after entering mutex locker";

    if (m_Thread) 
    {
        LOG(TRACE) + RHOQTPREFIX + "CRhoThreadImpl destructor before delete m_Thread";
		delete m_Thread;
        m_Thread = 0;
        LOG(TRACE) + RHOQTPREFIX + "CRhoThreadImpl destructor after delete m_Thread";
	}  

}

void CRhoThreadImpl::start(IRhoRunnable* pRunnable, IRhoRunnable::EPriority ePriority)
{
    LOG(TRACE) + RHOQTPREFIX + "start";
    if (m_Thread)
	{
        LOG(TRACE) + RHOQTPREFIX + "start - m_Thread exists before calling stop";
		stop(0);
        LOG(TRACE) + RHOQTPREFIX + "start - m_Thread exists after calling stop";
    }else{
        LOG(TRACE) + RHOQTPREFIX + "start - m_Thread does not exist do not call stop";
	}
    LOG(TRACE) + RHOQTPREFIX + "start - before Qthread start";
    m_Thread = new QRhoThread(pRunnable);

    QObject::connect(qApp, SIGNAL(destroyed(QObject*)), m_Thread, SLOT(terminate()));

    m_Thread->start();
    LOG(TRACE) + RHOQTPREFIX + "start - after Qthread start";
    setThreadPriority(ePriority);
    LOG(TRACE) + RHOQTPREFIX + "start - finish aftersetThreadPriority";

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
    LOG(INFO) + RHOQTPREFIX + "stop";

    LOG(TRACE) + RHOQTPREFIX + "stop - before enter entering mutex locker";
    QMutexLocker locker(&mutex);
    LOG(TRACE) + RHOQTPREFIX + "stop - after enter entering mutex locker";


    LOG(TRACE) + RHOQTPREFIX + "stop - before calling stopWait";
    stopWait();
    LOG(TRACE) + RHOQTPREFIX + "stop - after calling stopWait";

    if ( m_Thread ) {
        LOG(TRACE) + RHOQTPREFIX + "stop - m_Thread exists before m_Thread->quit()";
        m_Thread->quit();
        LOG(TRACE) + RHOQTPREFIX + "stop - m_Thread exists after m_Thread->quit()";
        if (!m_Thread->isRunning()){
            LOG(TRACE) + RHOQTPREFIX + "stop - m_Thread not running before m_Thread->terminate();";
            m_Thread->terminate();
            LOG(TRACE) + RHOQTPREFIX + "stop - m_Thread not running after m_Thread->terminate()";
        }
        if (nTimeoutToKill){
            LOG(TRACE) + RHOQTPREFIX + "stop - m_Thread before  m_Thread->wait()";
            m_Thread->wait(nTimeoutToKill);
            LOG(TRACE) + RHOQTPREFIX + "stop - m_Thread after  m_Thread->wait()";
        }
        LOG(TRACE) + RHOQTPREFIX + "stop - before  delete m_Thread";
        delete m_Thread;
        LOG(TRACE) + RHOQTPREFIX + "stop - after delete m_Thread";
        m_Thread = 0;
    }

    LOG(TRACE) + "RHOQT stop - finish";
}

int CRhoThreadImpl::wait(unsigned int nTimeoutMs)
{
    LOG(INFO) + RHOQTPREFIX + "wait";
	bool isVeyBigTimeoutvalue = false; 
    if ((nTimeoutMs == 4294966296)||(nTimeoutMs == 4294967295)){isVeyBigTimeoutvalue = true;}

    LOG(INFO) + RHOQTPREFIX + "wait - m_waitThread exists before calling stopWait";
    stopWait();
    LOG(TRACE) + RHOQTPREFIX + "wait - m_waitThread exists after calling stopWait";

    QMutexLocker waitLocker(&mutexWaiter);

    LOG(TRACE) + RHOQTPREFIX + "wait - before m_waitThread->start";
    m_waitThread.data()->start();
    LOG(INFO) + RHOQTPREFIX + "wait - after m_waitThread->start";
	bool result;
	if(isVeyBigTimeoutvalue)
	{
        LOG(INFO) + RHOQTPREFIX + "wait - before wait for a long time nTimeoutMs:-  "+nTimeoutMs;
        waitLocker.unlock();
        result = m_waitThread.data()->wait(1000UL*nTimeoutMs) ? 0 : 1;
        LOG(INFO) + RHOQTPREFIX + "wait - after wait for a long time Result:-  "+result;
    }else{
        LOG(INFO) + RHOQTPREFIX + "wait - before wait for a short time nTimeoutMs:-  "+nTimeoutMs;
        waitLocker.unlock();
        result = m_waitThread.data()->wait(1UL*nTimeoutMs) ? 0 : 1;
        LOG(INFO) + RHOQTPREFIX + "wait - after wait for a short time Result:-  "+result;
	}
    LOG(TRACE) + RHOQTPREFIX + "wait - finish before return";
    return result;
}

void CRhoThreadImpl::stopWait()
{
    LOG(INFO) + RHOQTPREFIX + "stopWait - before mutex";
    QMutexLocker waitLocker(&mutexWaiter);
    LOG(INFO) + RHOQTPREFIX + "stopWait - after mutex and before msleep";
    QRhoThread::msleep(2);
    LOG(TRACE) + RHOQTPREFIX + "stopWait - before quit()";
    m_waitThread.data()->quit();
    LOG(TRACE) + RHOQTPREFIX + "stopWait - after quit()";
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
