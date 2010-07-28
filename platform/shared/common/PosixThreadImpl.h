#ifndef RHO_POSIX_THREAD_IMPL_9e4b35f90ae146ae9c809ca56e4ef856
#define RHO_POSIX_THREAD_IMPL_9e4b35f90ae146ae9c809ca56e4ef856

#include "common/IRhoThreadImpl.h"
#include "logging/RhoLog.h"

namespace rho
{
namespace common
{

class CPosixThreadImpl : public IRhoThreadImpl
{
    DEFINE_LOGCLASS;
public:
    CPosixThreadImpl();
    ~CPosixThreadImpl();

    void start(IRhoRunnable *pRunnable, IRhoRunnable::EPriority ePriority);
    void stop(unsigned int nTimeoutToKill);
    void wait(unsigned int nTimeout);
    void stopWait();
    void sleep(unsigned int nTimeout);

private:
    pthread_cond_t m_condSync;
    common::CMutex m_mxSync;
    pthread_t m_thread;
    bool m_stop_wait;
};

} // namespace common
} // namespace rho

#endif // RHO_POSIX_THREAD_IMPL_9e4b35f90ae146ae9c809ca56e4ef856

