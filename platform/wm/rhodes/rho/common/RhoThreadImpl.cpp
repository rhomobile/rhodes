#include "stdafx.h"

#include "RhoThreadImpl.h"

namespace rho{
namespace common{
IMPLEMENT_LOGCLASS(CRhoThreadImpl,"RhoThread");

CRhoThreadImpl::CRhoThreadImpl() : m_hAwakeEvent(0), m_hThread(0)
{

}

static DWORD WINAPI runProc(void* pv) throw()
{
	IRhoRunnable* p = static_cast<IRhoRunnable*>(pv);
	p->run();
    ::ExitThread(0);
	return 0;
}

void CRhoThreadImpl::start(IRhoRunnable* pRunnable, IRhoRunnable::EPriority ePriority)
{
    if ( !m_hThread )
    {
        m_hAwakeEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
	    DWORD dwThreadID;
	    m_hThread = ::CreateThread(NULL, 0, runProc, pRunnable, 0, &dwThreadID);
        setThreadPriority(ePriority);
    }
}

void CRhoThreadImpl::setThreadPriority(IRhoRunnable::EPriority ePriority)
{
    int nPriority = THREAD_PRIORITY_NORMAL;
    if ( ePriority == IRhoRunnable::epHigh )
        nPriority = THREAD_PRIORITY_HIGHEST;
    else if (ePriority == IRhoRunnable::epLow)
        nPriority = THREAD_PRIORITY_LOWEST;

    ::SetThreadPriority(m_hThread,nPriority);
}

void CRhoThreadImpl::stop(unsigned int nTimeoutToKill)
{
    stopWait();
    if ( m_hThread )
    {
        DWORD dwRes = ::WaitForSingleObject( m_hThread, nTimeoutToKill*1000 );
        if ( dwRes != WAIT_OBJECT_0 )
        {
            LOG(INFO) + "Terminate thread. ID: " + ::GetCurrentThreadId() + "; Result: " + dwRes;
            ::TerminateThread(m_hThread,0);
        }
        ::CloseHandle(m_hThread);
        m_hThread = NULL;
    }

    if ( m_hAwakeEvent )
        ::CloseHandle(m_hAwakeEvent);
}

void CRhoThreadImpl::wait(unsigned int nTimeout)
{
    DWORD dwRes = ::WaitForSingleObject( m_hAwakeEvent, nTimeout*1000 );
    if ( dwRes == WAIT_FAILED )
        LOG(ERROR) + "WaitForSingleObject failed. ID: " + ::GetCurrentThreadId() + "; Result: " + dwRes;
}

void CRhoThreadImpl::stopWait()
{
    ::SetEvent(m_hAwakeEvent);
}

void CRhoThreadImpl::sleep(unsigned int nTimeout)
{
    ::Sleep(nTimeout);
}

}
}