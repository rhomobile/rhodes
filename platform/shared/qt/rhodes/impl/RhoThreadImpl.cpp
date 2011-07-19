#include "common/RhoDefs.h"

#ifdef OS_WINDOWS

#include "common/RhoPort.h"
#include "RhoThreadImpl.h"

using namespace rho::common;

IMPLEMENT_LOGCLASS(CRhoThreadImpl,"RhoThread");

void CRhoThreadImpl::start(IRhoRunnable* pRunnable, IRhoRunnable::EPriority ePriority)
{
    if (m_Thread)
        stop(0);
    m_Thread = new QRhoThread(pRunnable);
    m_Thread->start();
    setThreadPriority(ePriority);
}

void CRhoThreadImpl::setThreadPriority(IRhoRunnable::EPriority ePriority)
{
    QThread::Priority nPriority = QThread::NormalPriority;
    if ( ePriority == IRhoRunnable::epHigh )
        nPriority = QThread::HighestPriority;
    else if (ePriority == IRhoRunnable::epLow)
        nPriority = QThread::LowestPriority;
    m_Thread->setPriority(nPriority);
}

void CRhoThreadImpl::stop(unsigned int nTimeoutToKill)
{
    stopWait();
    if ( m_Thread ) {
        m_Thread->quit();
        m_Thread->wait(nTimeoutToKill);
        if (!m_Thread->isRunning()) {
            m_Thread->terminate();
            m_Thread->wait();
        }
        LOG(INFO) + "Terminate thread.";
        delete m_Thread;
        m_Thread = 0;
    }
}

int CRhoThreadImpl::wait(unsigned int nTimeout)
{
    if (m_waitThread)
        stopWait();
    m_waitThread = new QThread();
    m_waitThread->start();
    bool result = m_waitThread->wait(1000000UL*nTimeout) ? 0 : 1;
    delete m_waitThread;
    m_waitThread = 0;
    return result;
}

void CRhoThreadImpl::stopWait()
{
    if (m_waitThread) {
        m_waitThread->terminate(); // quit();
        if (m_waitThread)
            m_waitThread->wait();
    }
}

void CRhoThreadImpl::sleep(unsigned int nTimeout)
{
    QRhoThread::sleep(nTimeout);
}

#endif // OS_WINDOWS

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
