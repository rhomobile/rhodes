#include "stdafx.h"

#include "SyncThread.h"

extern "C" const char* RhoGetRootPath();

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

CSyncThread::CSyncThread() : m_oSyncEngine(m_oDBAdapter)
{
    //m_bSyncInitialized = false;
    m_bResetDB = false;

    m_oSyncEngine.setFactory(&m_oFactory);

    m_hDoSyncEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    m_thread.Initialize();
    ::SetThreadPriority( m_thread.GetThreadHandle(), THREAD_PRIORITY_BELOW_NORMAL );
    m_hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    m_thread.AddHandle(m_hEvent, this, NULL);
}

HRESULT CSyncThread::Execute(DWORD_PTR dwParam, HANDLE hObject)
{
/*    if ( !m_bSyncInitialized )
    {
        LOG(INFO) + "Starting sync engine";

        String strDBPath = RhoGetRootPath();
        strDBPath += "db\\syncdb.sqlite";
        String strVer = ""; //TODO: get version from rhodes
        m_oDBAdapter.open(strDBPath,strVer);

        m_bSyncInitialized = true;
    }*/

    LOG(INFO) + "Wait Sync timeout";
    WaitForSingleObject( m_hDoSyncEvent, WAIT_TIME_SECONDS*1000 );

    PerformSync();

    return S_OK;
}

void CSyncThread::PerformSync()
{
    ::ResetEvent(m_hDoSyncEvent);
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

CSyncThread::~CSyncThread(void)
{
    m_oSyncEngine.exitSync();
    ::SetEvent(m_hDoSyncEvent);

    m_thread.RemoveHandle(m_hEvent);
    m_thread.Shutdown();

    //m_oDBAdapter.close();
    LOG(INFO) + "Sync engine thread shutdown";
}

HRESULT CSyncThread::CloseHandle(HANDLE hHandle)
{
    LOG(INFO) + "Closing sync engine handles";

    ::CloseHandle(hHandle);
    ::CloseHandle(m_hDoSyncEvent);
    return S_OK;
}

};
};

extern "C" {
    
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

int rho_sync_openDB(const char* szDBPath, sqlite3 ** ppDB)
{
    rho::db::CDBAdapter& db = rho::sync::CSyncThread::getDBAdapter();
    rho::String strVer = "";//TODO: get version from rhodes 
    db.open(szDBPath,strVer);
    *ppDB = db.getDbHandle();
    return 0;
}

int rho_sync_closeDB(sqlite3 * pDB)
{
    rho::sync::CSyncThread::getDBAdapter().close();
    return 0;
}

}