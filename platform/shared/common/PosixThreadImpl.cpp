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
    :m_stop_wait(false)
{
#if defined(OS_ANDROID)
    // Android has no pthread_condattr_xxx API
    pthread_cond_init(&m_condSync, NULL);
#else
    pthread_condattr_t sync_details;
    pthread_condattr_init(&sync_details);
    pthread_cond_init(&m_condSync, &sync_details);
    pthread_condattr_destroy(&sync_details);
#endif
}

CPosixThreadImpl::~CPosixThreadImpl()
{
    pthread_cond_destroy(&m_condSync);
}

void *runProc(void *pv)
{
    IRhoRunnable *p = static_cast<IRhoRunnable *>(pv);
    void *pData = rho_nativethread_start();
    p->runObject();
    rho_nativethread_end(pData);
    return 0;
}

void CPosixThreadImpl::start(IRhoRunnable *pRunnable, IRhoRunnable::EPriority ePriority)
{
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
}

void CPosixThreadImpl::wait(unsigned int nTimeout)
{
    struct timespec   ts;
    struct timeval    tp;
    gettimeofday(&tp, NULL);
    /* Convert from timeval to timespec */
    ts.tv_sec  = tp.tv_sec;
    ts.tv_nsec = tp.tv_usec * 1000;
    
    unsigned long long max;
    bool timed_wait;
    if ( (unsigned)ts.tv_sec + nTimeout >= (unsigned)ts.tv_sec )
    {
        timed_wait = true;
        ts.tv_sec += nTimeout;
        max = ((unsigned long long)tp.tv_sec + nTimeout)*1000000 + tp.tv_usec;
    }
    else
        timed_wait = false;


    common::CMutexLock oLock(m_mxSync);

    while (!m_stop_wait)
    {
        if (timed_wait) {
            pthread_cond_timedwait(&m_condSync, m_mxSync.getNativeMutex(), &ts);
            
            gettimeofday(&tp, NULL);
            unsigned long long now = ((unsigned long long)tp.tv_sec)*1000000 + tp.tv_usec;
            if (now > max)
                m_stop_wait = true;
        }
        else
            pthread_cond_wait(&m_condSync, m_mxSync.getNativeMutex());
    }
    m_stop_wait = false;
}

void CPosixThreadImpl::sleep(unsigned int nTimeout)
{
    ::usleep(1000*nTimeout);
}

void CPosixThreadImpl::stopWait()
{
    common::CMutexLock oLock(m_mxSync);
    m_stop_wait = true;
    pthread_cond_broadcast(&m_condSync);
}

} // namespace common
} // namespace rho

