#ifndef RHOTHREADIMPL_H
#define RHOTHREADIMPL_H

#include <QThread>
#include "common/IRhoThreadImpl.h"
#include "logging/RhoLog.h"

namespace rho{
namespace common{

class CRhoThreadImpl : public IRhoThreadImpl
{
    DEFINE_LOGCLASS

public:
    CRhoThreadImpl(): m_Thread(0), m_waitThread(0) {}
    ~CRhoThreadImpl()
    {
        if (m_Thread) delete m_Thread;
        if (m_waitThread) delete m_waitThread;
    }
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
};

}
}

#endif // RHOTHREADIMPL_H
