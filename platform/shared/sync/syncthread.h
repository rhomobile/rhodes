#pragma once

#ifdef __cplusplus

#include "logging/RhoLog.h"
#include "db/DBAdapter.h"
#include "sync/SyncEngine.h"
#include "common/RhoThread.h"

namespace rho {
namespace sync {

#define SYNC_POLL_INTERVAL_SECONDS 300
#define SYNC_POLL_INTERVAL_INFINITE (unsigned int)(-1)
#define SYNC_WAIT_BEFOREKILL_SECONDS 3

class CSyncThread : public common::CRhoThread
{
public:
    enum ESyncCommands{ scNone = 0, scResetDB, scSyncAll, scSyncOne, scChangePollInterval, scExit};

private:

    DEFINE_LOGCLASS;
    static CSyncThread* m_pInstance;

    static db::CDBAdapter  m_oDBAdapter;
    CSyncEngine     m_oSyncEngine;
    common::CAutoPtr<common::IRhoClassFactory> m_ptrFactory;
    ESyncCommands m_curCommand;
	int           m_nPollInterval;

public:
    ~CSyncThread(void);

    static CSyncThread* Create(common::IRhoClassFactory* factory);
    static void Destroy();
    static CSyncThread* getInstance(){ return m_pInstance; }
    static CSyncEngine& getSyncEngine(){ return m_pInstance->m_oSyncEngine; }
    static db::CDBAdapter& getDBAdapter(){ return m_pInstance->m_oDBAdapter; }

    void addSyncCommand(ESyncCommands curCommand){ m_curCommand = curCommand; stopWait(); }
	virtual void run();

	void setPollInterval(int nInterval);
private:
    CSyncThread(common::IRhoClassFactory* factory);

    void processCommand();
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
void rho_sync_set_pollinterval(int nInterval);

//struct sqlite3;
int rho_sync_openDB(const char* szDBPath, void** ppDB);
int rho_sync_closeDB(void* pDB);
int rho_db_startUITransaction(void * pDB);
int rho_db_commitUITransaction(void * pDB);
int rho_db_rollbackUITransaction(void * pDB);
	
#ifdef __cplusplus
};
#endif //__cplusplus


