#include "SyncThread.h"

namespace rho {
namespace sync {

IMPLEMENT_LOGCLASS(CSyncThread,"Sync");
CSyncThread* CSyncThread::m_pInstance = 0;
db::CDBAdapter  CSyncThread::m_oDBAdapter;

/*static*/ CSyncThread* CSyncThread::Create(common::IRhoClassFactory* factory)
{
    if ( m_pInstance ) 
        return m_pInstance;

    m_pInstance = new CSyncThread(factory);
    return m_pInstance;
}

/*static*/void CSyncThread::Destroy()
{
    if ( m_pInstance )
        delete m_pInstance;

    m_pInstance = 0;
}

CSyncThread::CSyncThread(common::IRhoClassFactory* factory) : CRhoThread(factory), m_oSyncEngine(m_oDBAdapter)
{
    m_curCommand = scNone;
	m_nPollInterval = SYNC_POLL_INTERVAL_SECONDS;
	m_ptrFactory = factory;

    m_oSyncEngine.setFactory(factory);

    start(epLow);
}

void CSyncThread::run()
{
	LOG(INFO) + "Starting sync engine main routine...";

	while( m_oSyncEngine.getState() != CSyncEngine::esExit )
	{
        int nWait = m_nPollInterval ? m_nPollInterval : SYNC_POLL_INTERVAL_INFINITE;
		LOG(INFO) + "Sync engine blocked for " + nWait + " seconds...";
        wait(nWait);

        if ( m_oSyncEngine.getState() != CSyncEngine::esExit )
    		processCommand();
	}
}

void CSyncThread::processCommand()
{
    //TODO: implement stack of commands
    switch(m_curCommand)
    {
    case scNone:
        if ( m_nPollInterval )
            m_oSyncEngine.doSyncAllSources();
        break;
    case scSyncAll:
        m_oSyncEngine.doSyncAllSources();
        break;
    case scChangePollInterval:
        break;
    case scSyncOne:
        //TODO:scSyncOne
        break;
    case scResetDB:
        m_oSyncEngine.resetSyncDB();
        break;

    }
    m_curCommand = scNone;
}

CSyncThread::~CSyncThread(void)
{
    m_oSyncEngine.exitSync();
    stop(SYNC_WAIT_BEFOREKILL_SECONDS);
    LOG(INFO) + "Sync engine thread shutdown";
}

void CSyncThread::setPollInterval(int nInterval)
{ 
    m_nPollInterval = nInterval; 
    if ( m_nPollInterval == 0 )
        m_oSyncEngine.stopSync();

    addSyncCommand(scChangePollInterval); 
}

};
};

extern "C" {

using namespace rho::sync;
void rho_sync_destroy()
{
	CSyncThread::Destroy();
}
	
void rho_sync_doSyncAllSources()
{
    CSyncThread::getInstance()->addSyncCommand(CSyncThread::scSyncAll);
}

void rho_sync_db_reset()
{
    CSyncThread::getInstance()->addSyncCommand(CSyncThread::scResetDB);
}

void rho_sync_set_pollinterval(int nInterval)
{
    CSyncThread::getInstance()->setPollInterval(nInterval);
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
    return CSyncThread::getSyncEngine().login(name,password) ? 1 : 0;
}

int rho_sync_logged_in()
{
	rho::db::CDBAdapter& db = CSyncThread::getDBAdapter();
	db.setUnlockDB(true);
    int nRes = CSyncThread::getSyncEngine().isLoggedIn() ? 1 : 0;
	return nRes;
}

void rho_sync_logout()
{
    //TODO: stop sync
	rho::db::CDBAdapter& db = CSyncThread::getDBAdapter();
	db.setUnlockDB(true);
    CSyncThread::getSyncEngine().logout();
}

void rho_sync_set_notification(int source_id, const char *url, char* params)
{
    return CSyncThread::getSyncEngine().setNotification(source_id, url, params);
}

void rho_sync_clear_notification(int source_id)
{
    return CSyncThread::getSyncEngine().clearNotification(source_id);
}

int rho_sync_openDB(const char* szDBPath, void ** ppDB)
{
    rho::db::CDBAdapter& db = CSyncThread::getDBAdapter();
    rho::String strVer = "";//TODO: get version from rhodes 
    db.open(szDBPath,strVer);
    *ppDB = db.getDbHandle();
    return 0;
}

int rho_sync_closeDB(void * pDB)
{
    CSyncThread::getDBAdapter().close();
    return 0;
}

}