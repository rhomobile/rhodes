#include "common/PosixThreadImpl.h"

extern "C"
{
void* rho_nativethread_start();
void rho_nativethread_end(void*);
}

namespace rho
{
namespace common
{

IMPLEMENT_LOGCLASS(CPosixThreadImpl, "RhoThread");

CPosixThreadImpl::CPosixThreadImpl()
    :m_started(false)
{}

void *runProc(void *pv)
{
    IRhoRunnable *p = static_cast<IRhoRunnable *>(pv);
    void *pData = rho_nativethread_start();
    p->run();
    rho_nativethread_end(pData);
    return 0;
}

void CPosixThreadImpl::start(IRhoRunnable *pRunnable, IRhoRunnable::EPriority ePriority)
{
    {
        common::CMutexLock lock(m_mxSync);
        if (m_started)
            return;
        m_started = true;
    }

#if defined(OS_ANDROID)
    // Android has no pthread_condattr_xxx API
    pthread_cond_init(&m_condSync, NULL);
#else
    pthread_condattr_t sync_details;
    pthread_condattr_init(&sync_details);
    pthread_cond_init(&m_condSync, &sync_details);
    pthread_condattr_destroy(&sync_details);
#endif

    pthread_attr_t  attr;
    int return_val = pthread_attr_init(&attr);
    //return_val = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    RHO_ASSERT(!return_val);

    if ( ePriority != IRhoRunnable::epNormal)
    {
        sched_param param;
        return_val = pthread_attr_getschedparam (&attr, &param);
        param.sched_priority = ePriority == IRhoRunnable::epLow ? 20 : 100; //TODO: sched_priority
        return_val = pthread_attr_setschedparam (&attr, &param);
    }

    int thread_error = pthread_create(&m_thread, &attr, &runProc, pRunnable);
    return_val = pthread_attr_destroy(&attr);
    RHO_ASSERT(!return_val);
    RHO_ASSERT(thread_error==0);
}

void CPosixThreadImpl::stop(unsigned int nTimeoutToKill)
{
    stopWait();

    //TODO: wait for nTimeoutToKill and kill thread
    void* status;
    pthread_join(m_thread,&status);

    pthread_cond_destroy(&m_condSync);
}

void CPosixThreadImpl::wait(unsigned int nTimeout)
{
    common::CMutexLock oLock(m_mxSync);

    struct timespec   ts;
    struct timeval    tp;
    gettimeofday(&tp, NULL);
    /* Convert from timeval to timespec */
    ts.tv_sec  = tp.tv_sec;
    ts.tv_nsec = tp.tv_usec * 1000;

    if ( (unsigned)ts.tv_sec + nTimeout < (unsigned)ts.tv_sec )
        pthread_cond_wait(&m_condSync, m_mxSync.getNativeMutex() );
    else
    {
        ts.tv_sec += nTimeout;
        pthread_cond_timedwait(&m_condSync, m_mxSync.getNativeMutex(), &ts);
    }
}

void CPosixThreadImpl::sleep(unsigned int nTimeout)
{
    ::usleep(1000*nTimeout);
}

void CPosixThreadImpl::stopWait()
{
    pthread_cond_broadcast(&m_condSync);
}

} // namespace common
} // namespace rho

