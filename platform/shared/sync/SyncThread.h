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
#define SYNC_STARTUP_INTERVAL_SECONDS 10

class CSyncThread : public common::CRhoThread
{
public:
    enum ESyncCommands{ scNone = 0, scSyncAll, scSyncOne, scSyncOneByUrl, scChangePollInterval, scExit, scLogin, scSearchOne};

private:

    DEFINE_LOGCLASS;
public:
    class CSyncCommand
    {
    public:
	    int m_nCmdCode;
	    int m_nCmdParam;
	    String m_strCmdParam;

	    CSyncCommand(int nCode, int nParam)
	    {
		    m_nCmdCode = nCode;
		    m_nCmdParam = nParam;
	    }
	    CSyncCommand(int nCode, String strParam)
	    {
		    m_nCmdCode = nCode;
		    m_strCmdParam = strParam;
	    }
	    CSyncCommand(int nCode, String strParam, int nCmdParam)
	    {
		    m_nCmdCode = nCode;
		    m_strCmdParam = strParam;
            m_nCmdParam = nCmdParam;
	    }

	    CSyncCommand(int nCode)
	    {
		    m_nCmdCode = nCode;
		    m_nCmdParam = 0;
	    }

	    boolean equals(const CSyncCommand& oSyncCmd)
	    {
		    return m_nCmdCode == oSyncCmd.m_nCmdCode && m_nCmdParam == oSyncCmd.m_nCmdParam &&
			    m_strCmdParam == oSyncCmd.m_strCmdParam;
	    }

    };

    class CSyncLoginCommand : public CSyncCommand
    {
    public:
	    String m_strName, m_strPassword;
        CSyncLoginCommand(String name, String password, String callback) : CSyncCommand(CSyncThread::scLogin,callback)
	    {
		    m_strName = name;
		    m_strPassword = password;
	    }
    };
    class CSyncSearchCommand : public CSyncCommand
    {
    public:
	    String m_strFrom;
        boolean m_bSyncChanges;
        int     m_nProgressStep;

        CSyncSearchCommand(String from, String params, int source_id, boolean sync_changes, int nProgressStep) : CSyncCommand(CSyncThread::scSearchOne,params,source_id)
	    {
		    m_strFrom = from;
            m_bSyncChanges = sync_changes;
            m_nProgressStep = nProgressStep;
	    }
    };

private:
    static CSyncThread* m_pInstance;

    static db::CDBAdapter  m_oDBAdapter;
    CSyncEngine     m_oSyncEngine;
    common::CAutoPtr<common::IRhoClassFactory> m_ptrFactory;
	int           m_nPollInterval;
   	common::CMutex m_mxStackCommands;
	LinkedListPtr<CSyncCommand*> m_stackCommands;
public:
    ~CSyncThread(void);

    static CSyncThread* Create(common::IRhoClassFactory* factory);
    static void Destroy();
    static CSyncThread* getInstance(){ return m_pInstance; }
    static CSyncEngine& getSyncEngine(){ return m_pInstance->m_oSyncEngine; }
    static db::CDBAdapter& getDBAdapter(){ return m_pInstance->m_oDBAdapter; }

    void addSyncCommand(CSyncCommand* pSyncCmd);

	virtual void run();

	void setPollInterval(int nInterval);

private:
    CSyncThread(common::IRhoClassFactory* factory);
    int getLastSyncInterval();

    void processCommands();
    void processCommand(CSyncCommand& oSyncCmd);
    boolean isNoCommands();

};

}
}
#endif //__cplusplus

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus
	
void rho_sync_create();
void rho_sync_destroy();

void rho_sync_doSyncAllSources(int show_status_popup);
void rho_sync_doSyncSource(unsigned long nSrcID,int show_status_popup);
void rho_sync_doSearchSource(int source_id, const char *from, const char *params, bool sync_changes, int nProgressStep, const char* callback, const char* callback_params);
void rho_sync_doSyncSourceByUrl(const char* szSrcID);
void rho_sync_lock();
void rho_sync_unlock();
void rho_sync_login(const char *login, const char *password, const char* callback);
int rho_sync_logged_in();
void rho_sync_logout();
void rho_sync_set_notification(int source_id, const char *url, char* params);
void rho_sync_clear_notification(int source_id);
void rho_sync_set_pollinterval(int nInterval);
void rho_sync_set_syncserver(char* syncserver);
void rho_sync_setobjectnotify_url(const char* szUrl);
void rho_sync_addobjectnotify(int nSrcID, const char* szObject);
void rho_sync_cleanobjectnotify();
int rho_sync_get_pagesize();
void rho_sync_set_pagesize(int nPageSize);
void rho_sync_set_initial_notification(const char *url, char* params);
void rho_sync_clear_initial_notification();
void rho_sync_set_threaded_mode(int b);

//struct sqlite3;
int rho_sync_openDB(const char* szDBPath);
int rho_sync_closeDB();
int rho_db_startUITransaction();
int rho_db_commitUITransaction();
int rho_db_rollbackUITransaction();
int rho_db_destroy_table(const char* szTableName);
void* rho_db_get_handle();
unsigned long rho_sync_get_attrs(int nSrcID);
int rho_sync_get_lastsync_objectcount(int nSrcID);

#ifdef __cplusplus
};
#endif //__cplusplus


