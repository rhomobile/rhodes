#pragma once

#ifdef __cplusplus

#include "logging/RhoLog.h"
#include "db/DBAdapter.h"
#include "sync/SyncEngine.h"
#include "../RhoClassFactory.h"

namespace rho {
namespace sync {

#define WAIT_TIME_SECONDS 300

class CSyncThread //: public IWorkerThreadClient
{
    DEFINE_LOGCLASS;
    static CSyncThread* m_pInstance;

    static db::CDBAdapter  m_oDBAdapter;
    CSyncEngine m_oSyncEngine;
    CRhoClassFactory m_oFactory;

    boolean   m_bResetDB;

	pthread_cond_t m_condSync;
	common::CMutex m_mxSync;
	pthread_t       m_thread;
	//    HANDLE m_hEvent;
//    HANDLE m_hDoSyncEvent;
//    CWorkerThread<DefaultThreadTraits> m_thread;

public:
    ~CSyncThread(void);

    static CSyncThread* Create();
    static void Destroy();
    static CSyncThread* getInstance(){ return m_pInstance; }
    static CSyncEngine& getSyncEngine(){ return m_pInstance->m_oSyncEngine; }
    static db::CDBAdapter& getDBAdapter(){ return m_oDBAdapter; }

    //void ResumeThread(){::SetEvent(m_hEvent);}
    //void FreezeThread(){::ResetEvent(m_hEvent);}

    void TriggerSync();
    void TriggerDbReset(){ m_bResetDB = true; TriggerSync(); }
	void Execute();

private:
    CSyncThread(void);

    //HRESULT Execute( DWORD_PTR dwParam, HANDLE hObject );
	//HRESULT CloseHandle( HANDLE hHandle );

    void PerformSync();
	
};

}
}
#endif //__cplusplus

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus
	
void rho_sync_create();
void rho_sync_destroy();

void rho_sync_doSyncAllSources();
void rho_sync_lock();
void rho_sync_unlock();
int rho_sync_login(const char *login, const char *password);
int rho_sync_logged_in();
void rho_sync_logout();
void rho_sync_db_reset();
void rho_sync_set_notification(int source_id, const char *url, char* params);
void rho_sync_clear_notification(int source_id);

//struct sqlite3;
int rho_sync_openDB(const char* szDBPath, void** ppDB);
int rho_sync_closeDB(void* pDB);
	
#ifdef __cplusplus
};
#endif //__cplusplus


