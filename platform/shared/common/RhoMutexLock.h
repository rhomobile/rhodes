#ifndef _RHOMUTEXLOCK_H_
#define _RHOMUTEXLOCK_H_

#include "RhoPort.h"

namespace rho{
namespace general{

#if defined(OS_WINDOWS) || defined(OS_WINCE)
typedef CRITICAL_SECTION MutexType;
#else
typedef pthread_mutex_t MutexType;
#endif 

class CMutex{
public:
    inline CMutex();
    inline ~CMutex();

    inline void Lock();    // Block if needed until free then acquire exclusively
    inline void Unlock();  // Release a lock acquired via Lock()
private:

    MutexType m_nativeMutex;
};

#if defined(OS_WINDOWS) || defined(OS_WINCE)
CMutex::CMutex()             { InitializeCriticalSection(&m_nativeMutex); }
CMutex::~CMutex()            { DeleteCriticalSection(&m_nativeMutex); }
void CMutex::Lock()         { EnterCriticalSection(&m_nativeMutex); }
void CMutex::Unlock()       { LeaveCriticalSection(&m_nativeMutex); }
#else
CMutex::CMutex()             { pthread_mutex_init(&m_nativeMutex, NULL); }
CMutex::~CMutex()            { pthread_mutex_destroy(&m_nativeMutex); }
void CMutex::Lock()         { pthread_mutex_lock(&m_nativeMutex); }
void CMutex::Unlock()       { pthread_mutex_unlock(&m_nativeMutex); }
#endif 

class CMutexLock {
public:
    explicit CMutexLock(CMutex& mutex) : m_Mutex(mutex) { m_Mutex.Lock(); }
    ~CMutexLock() { m_Mutex.Unlock(); }
private:
    CMutex& m_Mutex;

    // Disallow "evil" constructors
    CMutexLock(const CMutexLock&);
    void operator=(const CMutexLock&);
};

}
}

#endif //_RHOMUTEXLOCK_H_
