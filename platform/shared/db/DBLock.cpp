#include "DBLock.h"
#include "DBAdapter.h"

#ifndef RHO_NO_RUBY
#include "ruby/ruby.h"
#include "ruby/ext/rho/rhoruby.h"

#ifdef WINDOWS_PLATFORM
#undef open
#undef close
#endif
#else
#include "sync/RhoconnectClientManager.h"
#include "common/RhoSystem.h"
#endif



namespace rho {

namespace db {

class CRubyMutexImpl
{
#ifndef RHO_NO_RUBY
    int m_nLockCount;
    VALUE m_valThread;
    VALUE m_valMutex;
    bool m_bIgnore;
#endif
public:
    CRubyMutexImpl(bool bIgnore);
    ~CRubyMutexImpl();
    void create();
    bool isMainRubyThread();
    void Lock();
    void Unlock();
    void close();
};


#ifndef RHO_NO_RUBY
CRubyMutexImpl::CRubyMutexImpl(bool bIgnore) : m_nLockCount(0), m_valThread(Qnil), m_valMutex(Qnil)
{
    
}

void CRubyMutexImpl::create()
{
    if ( !m_bIgnore &&  ( m_valMutex==Qnil ) )
    {
        unsigned long curThread = rho_ruby_current_thread();

        if ( curThread != Qnil)
        {
            m_valMutex = rho_ruby_create_mutex();
        }
    }
}

CRubyMutexImpl::~CRubyMutexImpl()
{
    close();    
}

void CRubyMutexImpl::close()
{
    if ( m_valMutex )
    {
        rho_ruby_destroy_mutex(m_valMutex);
        m_valMutex = Qnil;
    }
}

boolean CRubyMutexImpl::isMainRubyThread()
{
    return rho_ruby_main_thread() == rho_ruby_current_thread();
}

void CRubyMutexImpl::Lock()
{
    if ( m_valMutex == Qnil )
        return;

    VALUE curThread = rho_ruby_current_thread();
    if ( curThread == Qnil )
        return;

    if ( m_valThread != curThread )
    {
        rho_ruby_lock_mutex(m_valMutex);
        m_valThread = curThread;
        m_nLockCount = 1;
    }else
        m_nLockCount += 1;
}

void CRubyMutexImpl::Unlock()
{
    if ( m_valMutex == Qnil || m_nLockCount == 0)
        return;

    m_nLockCount--;
    if ( m_nLockCount == 0 )
    {
        m_valThread = Qnil;
        rho_ruby_unlock_mutex(m_valMutex);
    }
}
#else //RHO_NO_RUBY
CRubyMutexImpl::CRubyMutexImpl(bool bIgnore)
{
}

CRubyMutexImpl::~CRubyMutexImpl()
{
}

boolean CRubyMutexImpl::isMainRubyThread()
{
	if ( (!sync::RhoconnectClientManager::haveRhoconnectClientImpl()) || (!sync::RhoconnectClientManager::haveSyncThread()))
        return true;

    return sync::RhoconnectClientManager::syncThreadGetThreadID() != rho::common::CSystem::getThreadID();
}

void CRubyMutexImpl::Lock()
{
}

void CRubyMutexImpl::Unlock()
{
}

void CRubyMutexImpl::close()
{
}

void CRubyMutexImpl::create()
{
}
#endif //RHO_NO_RUBY	


CRubyMutex::CRubyMutex(bool bIgnore) : m_pImpl( new CRubyMutexImpl(bIgnore) ) {  }
CRubyMutex::~CRubyMutex() 							{ }
void CRubyMutex::create() 							{ m_pImpl->create(); }
bool CRubyMutex::isMainRubyThread()                 { return m_pImpl->isMainRubyThread(); }
void CRubyMutex::Lock()								{ m_pImpl->Lock(); }
void CRubyMutex::Unlock()							{ m_pImpl->Unlock(); }
void CRubyMutex::close()							{ m_pImpl->close(); }


DBLock::DBLock(CDBAdapter& db) : _db(db) { _db.Lock(); }
DBLock::~DBLock() { _db.Unlock(); }

} //db

} //rho
