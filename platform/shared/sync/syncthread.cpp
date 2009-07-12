#include "SyncThread.h"
#include "common/RhoTime.h"

namespace rho {
namespace sync {

using namespace rho::common;

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
	m_nPollInterval = SYNC_POLL_INTERVAL_SECONDS;
	m_ptrFactory = factory;

    m_oSyncEngine.setFactory(factory);

    start(epLow);
}

CSyncThread::~CSyncThread(void)
{
    m_oSyncEngine.exitSync();
    stop(SYNC_WAIT_BEFOREKILL_SECONDS);
    LOG(INFO) + "Sync engine thread shutdown";
}

void CSyncThread::addSyncCommand(CSyncCommand* pSyncCmd)
{ 
	//TODO: check for duplicates ???
	{
        CMutexLock lockNotify(m_mxStackCommands);
		m_stackCommands.add(pSyncCmd);
	}
	stopWait(); 
}

int CSyncThread::getLastSyncInterval()
{
    CTimeInterval nowTime = CTimeInterval::getCurrentTime();
	
    DBResult( res, m_oSyncEngine.getDB().executeSQL("SELECT last_updated from sources") );
    uint64 latestTimeUpdated = 0;
    for ( ; !res.isEnd(); res.next() )
    { 
        uint64 timeUpdated = res.getUInt64ByIdx(0);
        if ( latestTimeUpdated < timeUpdated )
        	latestTimeUpdated = timeUpdated;
    }
	
	return latestTimeUpdated > 0 ? (int)(nowTime.toULong()-latestTimeUpdated) : 0;
}

void CSyncThread::run()
{
	LOG(INFO) + "Starting sync engine main routine...";

	int nLastSyncInterval = getLastSyncInterval();
	while( m_oSyncEngine.getState() != CSyncEngine::esExit )
	{
        int nWait = m_nPollInterval > 0 ? m_nPollInterval : SYNC_POLL_INTERVAL_INFINITE;

        if ( m_nPollInterval > 0 && nLastSyncInterval > 0 )
            nWait = (m_nPollInterval*1000 - nLastSyncInterval)/1000;
        if ( nWait <= 0 )
            nWait = SYNC_STARTUP_INTERVAL_SECONDS;

		if ( nWait >= 0 )
		{
		    LOG(INFO) + "Sync engine blocked for " + nWait + " seconds...";
            wait(nWait);
        }
        nLastSyncInterval = 0;

        if ( m_oSyncEngine.getState() != CSyncEngine::esExit )
    		processCommands();
	}
}

void CSyncThread::processCommands()//throws Exception
{
	if ( m_stackCommands.isEmpty() )
		addSyncCommand(new CSyncCommand(scNone));
	
	while(!m_stackCommands.isEmpty())
	{
		common::CAutoPtr<CSyncCommand> pSyncCmd = null;
    	{
        	CMutexLock lockNotify(m_mxStackCommands);
    		pSyncCmd = (CSyncCommand*)m_stackCommands.removeFirst();
    	}
		
		processCommand(*pSyncCmd);
	}
}

void CSyncThread::processCommand(CSyncCommand& oSyncCmd)
{
    switch(oSyncCmd.m_nCmdCode)
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
        m_oSyncEngine.doSyncSource(oSyncCmd.m_nCmdParam);
        break;
    }
}

void CSyncThread::setPollInterval(int nInterval)
{ 
    m_nPollInterval = nInterval; 
    if ( m_nPollInterval == 0 )
        m_oSyncEngine.stopSync();

    addSyncCommand( new CSyncCommand(scChangePollInterval) );
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
    CSyncThread::getInstance()->addSyncCommand(new CSyncCommand(CSyncThread::scSyncAll));
}

void rho_sync_doSyncSource(int nSrcID)
{
    CSyncThread::getInstance()->addSyncCommand(new CSyncCommand(CSyncThread::scSyncOne, nSrcID) );
}	
	
void rho_sync_stop()
{
	if (CSyncThread::getSyncEngine().isSyncing() )
	{
		CSyncThread::getSyncEngine().stopSync();
		while( CSyncThread::getSyncEngine().getState() != CSyncEngine::esNone )
			CSyncThread::getInstance()->sleep(100);
	}
}

void rho_sync_set_pollinterval(int nInterval)
{
    CSyncThread::getInstance()->setPollInterval(nInterval);
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
    db.setUnlockDB(false);
	return nRes;
}

void rho_sync_logout()
{
    //TODO: stop sync
	rho::db::CDBAdapter& db = CSyncThread::getDBAdapter();
	db.setUnlockDB(true);
    CSyncThread::getSyncEngine().logout();
    db.setUnlockDB(false);
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

int rho_db_startUITransaction(void * pDB)
{
    rho::db::CDBAdapter& db = rho::sync::CSyncThread::getDBAdapter();
    db.setUnlockDB(true);
    db.startTransaction();

    //TODO: get error code from DBException
    return 0;
}

int rho_db_commitUITransaction(void * pDB)
{
    CSyncThread::getDBAdapter().endTransaction();
    //TODO: get error code from DBException
    return 0;
}

int rho_db_rollbackUITransaction(void * pDB)
{
    CSyncThread::getDBAdapter().rollback();
    //TODO: get error code from DBException
    return 0;
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

}