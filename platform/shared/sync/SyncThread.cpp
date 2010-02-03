#include "SyncThread.h"
#include "common/RhoTime.h"
#include "common/RhoConf.h"

#include "ruby/ext/rho/rhoruby.h"

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
    if( RHOCONF().isExist("sync_poll_interval") )
        m_nPollInterval = RHOCONF().getInt("sync_poll_interval");

    m_ptrFactory = factory;

    m_oSyncEngine.setFactory(factory);

    start(epLow);
}

CSyncThread::~CSyncThread(void)
{
    m_oSyncEngine.exitSync();
    stop(SYNC_WAIT_BEFOREKILL_SECONDS);

    getDBAdapter().close();

    LOG(INFO) + "Sync engine thread shutdown";
}

void CSyncThread::addSyncCommand(CSyncCommand* pSyncCmd)
{ 
	{
        synchronized(m_mxStackCommands);

		boolean bExist = false;
		for ( int i = 0; i < (int)m_stackCommands.size(); i++ )
		{
			if ( m_stackCommands.get(i)->equals(*pSyncCmd) )
			{
				bExist = true;
				break;
			}
		}
		
		if ( !bExist )
    		m_stackCommands.add(pSyncCmd);
	}

    if ( getSyncEngine().isNoThreadedMode() )
        processCommands();
    else
	    stopWait(); 
}

int CSyncThread::getLastSyncInterval()
{
    uint64 nowTime = CLocalTime().toULong();
	
    DBResult( res, m_oSyncEngine.getDB().executeSQL("SELECT last_updated from sources") );
    uint64 latestTimeUpdated = 0;
    for ( ; !res.isEnd(); res.next() )
    { 
        uint64 timeUpdated = res.getUInt64ByIdx(0);
        if ( latestTimeUpdated < timeUpdated )
        	latestTimeUpdated = timeUpdated;
    }
	
	return latestTimeUpdated > 0 ? (int)(nowTime-latestTimeUpdated) : 0;
}

void CSyncThread::run()
{
	LOG(INFO) + "Starting sync engine main routine...";

	int nLastSyncInterval = getLastSyncInterval();
	while( m_oSyncEngine.getState() != CSyncEngine::esExit )
	{
        unsigned int nWait = m_nPollInterval > 0 ? m_nPollInterval : SYNC_POLL_INTERVAL_INFINITE;

        if ( m_nPollInterval > 0 && nLastSyncInterval > 0 )
        {
            int nWait2 = m_nPollInterval - nLastSyncInterval;
            if ( nWait2 <= 0 )
                nWait = SYNC_STARTUP_INTERVAL_SECONDS;
            else
                nWait = nWait2;
        }

        if ( nWait >= 0 && m_oSyncEngine.getState() != CSyncEngine::esExit && 
		     isNoCommands() )
		{
		    LOG(INFO) + "Sync engine blocked for " + nWait + " seconds...";
            wait(nWait);
        }
        nLastSyncInterval = 0;

        if ( m_oSyncEngine.getState() != CSyncEngine::esExit )
    		processCommands();
	}
}

boolean CSyncThread::isNoCommands()
{
	boolean bEmpty = false;
	synchronized(m_mxStackCommands)
    {		
		bEmpty = m_stackCommands.isEmpty();
	}

	return bEmpty;
}

void CSyncThread::processCommands()//throws Exception
{
    if ( isNoCommands() )
        addSyncCommand(new CSyncCommand(scNone));

	while(!isNoCommands())
	{
		common::CAutoPtr<CSyncCommand> pSyncCmd = null;
    	{
        	synchronized(m_mxStackCommands);
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
    case scSyncOneByUrl:
        {
            CSyncEngine::CSourceID oSrcID;
            oSrcID.m_strUrl = oSyncCmd.m_strCmdParam;

            m_oSyncEngine.doSyncSource(oSrcID,"","",false, -1 );
        }
        break;
    case scSyncOne:
        {
            CSyncEngine::CSourceID oSrcID;
            oSrcID.m_nID = oSyncCmd.m_nCmdParam;
            oSrcID.m_strName = oSyncCmd.m_strCmdParam;

            m_oSyncEngine.doSyncSource(oSrcID,"","",false, -1 );
        }
        break;
    case scSearchOne:
        {
            CSyncEngine::CSourceID oSrcID;
            oSrcID.m_nID = oSyncCmd.m_nCmdParam;

            m_oSyncEngine.doSyncSource(oSrcID,oSyncCmd.m_strCmdParam, 
                ((CSyncSearchCommand&)oSyncCmd).m_strFrom, ((CSyncSearchCommand&)oSyncCmd).m_bSyncChanges,
                ((CSyncSearchCommand&)oSyncCmd).m_nProgressStep);
        }
        break;
    case scLogin:
    	{
    		CSyncLoginCommand& oLoginCmd = (CSyncLoginCommand&)oSyncCmd;
    		m_oSyncEngine.login(oLoginCmd.m_strName, oLoginCmd.m_strPassword, oLoginCmd.m_strCmdParam );
    	}
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
	
void rho_sync_doSyncAllSources(int show_status_popup)
{
    CSyncThread::getInstance()->addSyncCommand(new CSyncThread::CSyncCommand(CSyncThread::scSyncAll));
    //rho_sync_doSyncSourceByUrl("http://dev.rhosync.rhohub.com/apps/SugarCRM/sources/SugarAccounts");
}

void rho_sync_doSyncSource(unsigned long nSrcID,int show_status_popup)
{
    CRhoRubyStringOrInt oSrcID = rho_ruby_getstringorint(nSrcID);
    CSyncThread::getInstance()->addSyncCommand(new CSyncThread::CSyncCommand(CSyncThread::scSyncOne, oSrcID.m_szStr, (int)oSrcID.m_nInt ) );
}	

void rho_sync_stop()
{
	if (CSyncThread::getSyncEngine().isSyncing() )
	{
		CSyncThread::getSyncEngine().stopSyncByUser();
//		while( CSyncThread::getSyncEngine().getState() != CSyncEngine::esNone )
        while( CSyncThread::getDBAdapter().isInsideTransaction() )
			CSyncThread::getInstance()->sleep(100);
	}
}

void rho_sync_doSearchSource(int source_id, const char *from, const char *params, bool sync_changes, int nProgressStep, 
    const char* callback, const char* callback_params)
{
    rho_sync_stop();
    if ( callback && *callback )
        CSyncThread::getSyncEngine().getNotify().setSearchNotification(source_id, callback, callback_params ? callback_params : "");

    CSyncThread::getInstance()->addSyncCommand(new CSyncThread::CSyncSearchCommand(from,params,source_id,sync_changes,nProgressStep) );
}	

void rho_sync_doSyncSourceByUrl(const char* szSrcUrl)
{
    CSyncThread::getInstance()->addSyncCommand(new CSyncThread::CSyncCommand(CSyncThread::scSyncOneByUrl, szSrcUrl) );
}	

void rho_sync_set_pollinterval(int nInterval)
{
    CSyncThread::getInstance()->setPollInterval(nInterval);
}
	
void rho_sync_set_syncserver(char* syncserver)
{
	CSyncThread::getSyncEngine().setSyncServer(syncserver);	
}

void rho_sync_login(const char *name, const char *password, const char* callback)
{
    rho_sync_stop();
    CSyncThread::getInstance()->addSyncCommand(new CSyncThread::CSyncLoginCommand(name, password, callback) );
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
    rho_sync_stop();

	rho::db::CDBAdapter& db = CSyncThread::getDBAdapter();
	db.setUnlockDB(true);
    CSyncThread::getSyncEngine().logout();
    db.setUnlockDB(false);
}

void rho_sync_set_notification(int source_id, const char *url, char* params)
{
    return CSyncThread::getSyncEngine().getNotify().setSyncNotification(source_id, url, params ? params : "");
}

void rho_sync_clear_notification(int source_id)
{
    return CSyncThread::getSyncEngine().getNotify().clearSyncNotification(source_id);
}

void rho_sync_set_initial_notification(const char *url, char* params)
{
    return CSyncThread::getSyncEngine().getNotify().setInitialSyncNotification(url, params ? params : "");
}

void rho_sync_clear_initial_notification()
{
    return CSyncThread::getSyncEngine().getNotify().clearInitialSyncNotification();
}

int rho_sync_openDB(const char* szDBPath)
{
    rho::db::CDBAdapter& db = CSyncThread::getDBAdapter();
    rho::String strVer = RhoRuby_getRhoDBVersion(); 
    db.open(szDBPath,strVer, false);
    return 0;
}

int rho_sync_closeDB()
{
    CSyncThread::getDBAdapter().close();
    return 0;
}

int rho_db_startUITransaction()
{
    rho::db::CDBAdapter& db = rho::sync::CSyncThread::getDBAdapter();
    db.setUnlockDB(true);
    db.startTransaction();

    //TODO: get error code from DBException
    return 0;
}

int rho_db_commitUITransaction()
{
    CSyncThread::getDBAdapter().endTransaction();
    //TODO: get error code from DBException
    return 0;
}

int rho_db_rollbackUITransaction()
{
    CSyncThread::getDBAdapter().rollback();
    //TODO: get error code from DBException
    return 0;
}

int rho_db_destroy_table(const char* szTableName)
{
    CSyncThread::getDBAdapter().destroy_table(szTableName);
    return 0;
}

void* rho_db_get_handle()
{
    return CSyncThread::getDBAdapter().getDbHandle();
}

unsigned long rho_sync_get_attrs(int nSrcID)
{
    return (VALUE)CSyncThread::getDBAdapter().getAttrMgr().getAttrsBySrc(nSrcID);
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

void rho_sync_setobjectnotify_url(const char* szUrl)
{
    CSyncNotify::setObjectNotifyUrl(szUrl);
}

void rho_sync_addobjectnotify(int nSrcID, const char* szObject)
{
    CSyncThread::getSyncEngine().getNotify().addObjectNotify(nSrcID, szObject);
}

void rho_sync_addobjectnotify_bysrcname(const char* szSrcName, const char* szObject)
{
    CSyncThread::getSyncEngine().getNotify().addObjectNotify(szSrcName, szObject);
}

void rho_sync_cleanobjectnotify()
{
    CSyncThread::getSyncEngine().getNotify().cleanObjectNotifications();
}

int rho_sync_get_lastsync_objectcount(int nSrcID)
{
    return CSyncThread::getSyncEngine().getNotify().getLastSyncObjectCount(nSrcID);
}

int rho_sync_get_pagesize()
{
    return CSyncThread::getSyncEngine().getSyncPageSize();
}

void rho_sync_set_pagesize(int nPageSize)
{
    return CSyncThread::getSyncEngine().setSyncPageSize(nPageSize);
}

void rho_sync_set_threaded_mode(int b)
{
    return CSyncThread::getSyncEngine().setNonThreadedMode(b==0);
}

}
