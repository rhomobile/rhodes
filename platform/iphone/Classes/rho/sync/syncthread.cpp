//#include "stdafx.h"

#include "SyncThread.h"

#include <sys/time.h>

namespace rho {
namespace sync {

IMPLEMENT_LOGCLASS(CSyncThread,"Sync");
CSyncThread* CSyncThread::m_pInstance = 0;
db::CDBAdapter CSyncThread::m_oDBAdapter;

/*static*/ CSyncThread* CSyncThread::Create()
{
    if ( m_pInstance ) 
        return m_pInstance;

    m_pInstance = new CSyncThread;
    return m_pInstance;
}

/*static*/void CSyncThread::Destroy()
{
    if ( m_pInstance )
        delete m_pInstance;

    m_pInstance = 0;
}

void* sync_engine_main_routine(void* data) 
{
	CSyncThread* pThis = (CSyncThread*)data;
	pThis->Execute();
	
	return NULL;
}
	
CSyncThread::CSyncThread() : m_oSyncEngine(m_oDBAdapter)
{
    m_bResetDB = false;

    m_oSyncEngine.setFactory(&m_oFactory);

	// Initialize thread
	pthread_condattr_t sync_details;
	pthread_condattr_init(&sync_details);
	pthread_cond_init(&m_condSync, &sync_details);
	pthread_condattr_destroy(&sync_details);

    pthread_attr_t  attr;
    int return_val = pthread_attr_init(&attr);
    return_val = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    RHO_ASSERT(!return_val);
	
    int thread_error = pthread_create(&m_thread, &attr, &sync_engine_main_routine, this);
    return_val = pthread_attr_destroy(&attr);
    RHO_ASSERT(!return_val);
    RHO_ASSERT(thread_error==0);
}

void CSyncThread::Execute()
{
	LOG(INFO) + "Starting sync engine main routine...";
	
	common::CMutexLock oLock(m_mxSync);
	
	while( m_oSyncEngine.getState() != CSyncEngine::esExit )
	{
		struct timespec   ts;
		struct timeval    tp;
		gettimeofday(&tp, NULL);
		/* Convert from timeval to timespec */
		ts.tv_sec  = tp.tv_sec;
		ts.tv_nsec = tp.tv_usec * 1000;
		ts.tv_sec += WAIT_TIME_SECONDS;
			
		LOG(INFO) + "Sync engine blocked for " + WAIT_TIME_SECONDS + " seconds...";
		pthread_cond_timedwait(&m_condSync, m_mxSync.getNativeMutex(), &ts);
		
		PerformSync();
	}
}
	
/*HRESULT CSyncThread::Execute(DWORD_PTR dwParam, HANDLE hObject)
{
    LOG(INFO) + "Wait Sync timeout";
    WaitForSingleObject( m_hDoSyncEvent, WAIT_TIME_SECONDS*1000 );

    PerformSync();

    return S_OK;
}*/

void CSyncThread::PerformSync()
{
    //::ResetEvent(m_hDoSyncEvent);
    if ( m_bResetDB ) 
    {
        m_oSyncEngine.resetSyncDB();
        m_bResetDB = false;
    }else if ( m_oSyncEngine.isContinueSync() )
    {
        LOG(INFO) + "Performing sync";
        m_oSyncEngine.doSyncAllSources();
    }
}

void CSyncThread::TriggerSync() 
{
	pthread_cond_broadcast(&m_condSync);
}

CSyncThread::~CSyncThread(void)
{
    m_oSyncEngine.exitSync();
	TriggerSync();
	void* status;
	pthread_join(m_thread,&status);
    pthread_cond_destroy(&m_condSync);
	
    //m_thread.RemoveHandle(m_hEvent);
    //m_thread.Shutdown();

    //m_oDBAdapter.close();
    LOG(INFO) + "Sync engine thread shutdown";
}
/*
HRESULT CSyncThread::CloseHandle(HANDLE hHandle)
{
    LOG(INFO) + "Closing sync engine handles";

    ::CloseHandle(hHandle);
    ::CloseHandle(m_hDoSyncEvent);
    return S_OK;
}*/

};
};

extern "C" {

void rho_sync_create()
{
	rho::sync::CSyncThread::Create();
}
	
void rho_sync_destroy()
{
	rho::sync::CSyncThread::Destroy();
}
	
void rho_sync_doSyncAllSources()
{
    rho::sync::CSyncThread::getInstance()->TriggerSync();
}

void rho_sync_lock()
{
    rho::db::CDBAdapter& db = rho::sync::CSyncThread::getDBAdapter();
    db.setUnlockDB(true);
    db.Lock();
}

void rho_sync_unlock()
{
    rho::db::CDBAdapter& db = rho::sync::CSyncThread::getDBAdapter();
    db.Unlock();
}

int rho_sync_login(const char *name, const char *password)
{
    //TODO: stop sync
    return rho::sync::CSyncThread::getSyncEngine().login(name,password) ? 1 : 0;
}

int rho_sync_logged_in()
{
    return rho::sync::CSyncThread::getSyncEngine().isLoggedIn() ? 1 : 0;
}

void rho_sync_logout()
{
    //TODO: stop sync
    return rho::sync::CSyncThread::getSyncEngine().logout();
}

void rho_sync_db_reset()
{
    return rho::sync::CSyncThread::getInstance()->TriggerDbReset();
}

void rho_sync_set_notification(int source_id, const char *url, char* params)
{
    return rho::sync::CSyncThread::getSyncEngine().setNotification(source_id, url, params);
}

void rho_sync_clear_notification(int source_id)
{
    return rho::sync::CSyncThread::getSyncEngine().clearNotification(source_id);
}

int rho_sync_openDB(const char* szDBPath, void ** ppDB)
{
    rho::db::CDBAdapter& db = rho::sync::CSyncThread::getDBAdapter();
    rho::String strVer = "";//TODO: get version from rhodes 
    db.open(szDBPath,strVer);
    *ppDB = db.getDbHandle();
    return 0;
}

int rho_sync_closeDB(void * pDB)
{
    rho::sync::CSyncThread::getDBAdapter().close();
    return 0;
}

}