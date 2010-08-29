#include "SyncThread.h"
#include "common/RhoTime.h"
#include "common/RhoConf.h"
#include "common/RhoFilePath.h"

#ifndef RHO_NO_RUBY
#include "ruby/ext/rho/rhoruby.h"
#endif //RHO_NO_RUBY
#include "sync/ClientRegister.h"

namespace rho {
namespace sync {

using namespace rho::common;
using namespace rho::db;

IMPLEMENT_LOGCLASS(CSyncThread,"Sync");
CSyncThread* CSyncThread::m_pInstance = 0;

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

CSyncThread::CSyncThread(common::IRhoClassFactory* factory) : CThreadQueue(factory)
{
    CThreadQueue::setLogCategory(getLogCategory());

    if( RHOCONF().isExist("sync_poll_interval") )
        setPollInterval(RHOCONF().getInt("sync_poll_interval"));

    m_oSyncEngine.setFactory(factory);

    if ( RHOCONF().getString("syncserver").length() > 0 )
        start(epLow);
}

CSyncThread::~CSyncThread(void)
{
    m_oSyncEngine.exitSync();
    stop(SYNC_WAIT_BEFOREKILL_SECONDS);

    db::CDBAdapter::closeAll();
}

#ifndef RHO_NO_RUBY
unsigned long CSyncThread::getRetValue()
{
    unsigned long ret = rho_ruby_get_NIL();
    if ( isNoThreadedMode()  )
    {
        ret = rho_ruby_create_string( getSyncEngine().getNotify().getNotifyBody().c_str() );
        getSyncEngine().getNotify().cleanNotifyBody();
    }

    return ret;
}
#else
unsigned long CSyncThread::getRetValue()
{
    unsigned long ret = 0;
    if ( isNoThreadedMode()  )
    {
        ret = (unsigned long)rho_sync_create_string( getSyncEngine().getNotify().getNotifyBody().c_str() );
        getSyncEngine().getNotify().cleanNotifyBody();
    }

    return ret;
}
#endif 

int CSyncThread::getLastPollInterval()
{
    uint64 nowTime = CLocalTime().toULong();
	
    DBResult( res, CDBAdapter::getUserDB().executeSQL("SELECT last_updated from sources") );
    uint64 latestTimeUpdated = 0;
    for ( ; !res.isEnd(); res.next() )
    { 
        uint64 timeUpdated = res.getUInt64ByIdx(0);
        if ( latestTimeUpdated < timeUpdated )
        	latestTimeUpdated = timeUpdated;
    }
	
	return latestTimeUpdated > 0 ? (int)(nowTime-latestTimeUpdated) : 0;
}

void CSyncThread::processCommands()//throws Exception
{
    if ( isNoCommands() )
        addQueueCommand(new CSyncCommand(scNone,false));

    CThreadQueue::processCommands();
}

void CSyncThread::checkShowStatus(CSyncCommand& oSyncCmd)
{
	boolean bShowStatus = oSyncCmd.m_bShowStatus;
	m_oSyncEngine.getNotify().enableReporting(bShowStatus);
	//if (bShowStatus)
		//m_statusListener.createStatusPopup(RhoRuby.getMessageText("syncronizing_data"));
}	

void CSyncThread::processCommand(IQueueCommand* pCmd)
{
    CSyncCommand& oSyncCmd = *((CSyncCommand*)pCmd);
    switch(oSyncCmd.m_nCmdCode)
    {
    case scNone:
        if ( getPollInterval() )
        {
            checkShowStatus(oSyncCmd);
            m_oSyncEngine.doSyncAllSources();
        }
        break;
    case scSyncAll:
        checkShowStatus(oSyncCmd);
        m_oSyncEngine.doSyncAllSources();
        break;
    case scSyncOne:
        {
			checkShowStatus(oSyncCmd);
            m_oSyncEngine.doSyncSource(CSyncEngine::CSourceID(oSyncCmd.m_nCmdParam,oSyncCmd.m_strCmdParam));
        }
        break;
    case scSearchOne:
        {
			checkShowStatus(oSyncCmd);
            m_oSyncEngine.doSearch( ((CSyncSearchCommand&)oSyncCmd).m_arSources, oSyncCmd.m_strCmdParam, 
                ((CSyncSearchCommand&)oSyncCmd).m_strFrom, ((CSyncSearchCommand&)oSyncCmd).m_bSyncChanges,
                oSyncCmd.m_nCmdParam);
        }
        break;
    case scLogin:
    	{
    		CSyncLoginCommand& oLoginCmd = (CSyncLoginCommand&)oSyncCmd;

            checkShowStatus(oSyncCmd);
    		m_oSyncEngine.login(oLoginCmd.m_strName, oLoginCmd.m_strPassword, oLoginCmd.m_strCmdParam );
    	}
        break;
    }
}

void CSyncThread::setPollInterval(int nInterval)
{ 
    if ( nInterval == 0 )
        m_oSyncEngine.stopSync();

    CThreadQueue::setPollInterval(nInterval);
}

String CSyncThread::CSyncCommand::toString()
{
    switch(m_nCmdCode)
    {
    case scNone:
        return "CheckPollInterval";

    case scSyncAll:
        return "SyncAll";
    case scSyncOne:
        return "SyncOne";
    case scLogin:
        return "Login";
    case scSearchOne:
        return "Search";
    }

    return "Unknown; Code : " + convertToStringA(m_nCmdCode);
}

};
};

extern "C" {

using namespace rho::sync;
using namespace rho::db;
	
unsigned long rho_sync_doSyncAllSources(int show_status_popup)
{
    CSyncThread::getInstance()->addQueueCommand(new CSyncThread::CSyncCommand(CSyncThread::scSyncAll,show_status_popup!=0));

    return CSyncThread::getInstance()->getRetValue();
}

unsigned long rho_sync_doSyncSourceByID(int nSrcID)
{
    CSyncThread::getInstance()->addQueueCommand(new CSyncThread::CSyncCommand(CSyncThread::scSyncOne, "", nSrcID, false ) );
    return CSyncThread::getInstance()->getRetValue();
}

unsigned long rho_sync_doSyncSourceByName(const char* szSrcName)
{
    CSyncThread::getInstance()->addQueueCommand(new CSyncThread::CSyncCommand(CSyncThread::scSyncOne, szSrcName, 0, false ) );
    return CSyncThread::getInstance()->getRetValue();
}

#ifndef RHO_NO_RUBY
unsigned long rho_sync_doSyncSource(unsigned long nSrcID,int show_status_popup)
{
    CRhoRubyStringOrInt oSrcID = rho_ruby_getstringorint(nSrcID);
    CSyncThread::getInstance()->addQueueCommand(new CSyncThread::CSyncCommand(CSyncThread::scSyncOne, oSrcID.m_szStr, (int)oSrcID.m_nInt, show_status_popup!=0 ) );

    return CSyncThread::getInstance()->getRetValue();
}	
#endif //RHO_NO_RUBY

void rho_sync_stop()
{
	if (CSyncThread::getSyncEngine().isSyncing() )
	{
		CSyncThread::getSyncEngine().stopSyncByUser();
        CSyncThread::getInstance()->stopWait();

        while( CDBAdapter::isAnyInsideTransaction() )
			CSyncThread::getInstance()->sleep(100);
	}
}

extern "C" void
source_iter(const char* szName, void* parSources)
{
    rho::Vector<rho::String>& arSources = *((rho::Vector<rho::String>*)(parSources));
    arSources.addElement(szName);
}

#ifndef RHO_NO_RUBY
unsigned long rho_sync_doSearch(unsigned long ar_sources, const char *from, const char *params, bool sync_changes, int nProgressStep, 
    const char* callback, const char* callback_params)
{
    rho_sync_stop();
    if ( callback && *callback )
        CSyncThread::getSyncEngine().getNotify().setSearchNotification( callback, callback_params ? callback_params : "");

    rho::Vector<rho::String> arSources;
    rho_ruby_enum_strary(ar_sources, source_iter, &arSources);

    CSyncThread::getInstance()->addQueueCommand(new CSyncThread::CSyncSearchCommand(from,params,arSources,sync_changes,nProgressStep) );

    return CSyncThread::getInstance()->getRetValue();
}	
#endif //RHO_NO_RUBY

unsigned long rho_sync_doSearchByNames(unsigned long ar_sources, const char *from, const char *params, bool sync_changes, int nProgressStep, 
    const char* callback, const char* callback_params)
{
    rho_sync_stop();
    if ( callback && *callback )
        CSyncThread::getSyncEngine().getNotify().setSearchNotification( callback, callback_params ? callback_params : "");

    rho::Vector<rho::String>& arSources = *((rho::Vector<rho::String>*)ar_sources);

    CSyncThread::getInstance()->addQueueCommand(new CSyncThread::CSyncSearchCommand(from,params,arSources,sync_changes,nProgressStep) );

    return CSyncThread::getInstance()->getRetValue();
}	

void rho_sync_set_pollinterval(int nInterval)
{
    CSyncThread::getInstance()->setPollInterval(nInterval);
}
	
void rho_sync_set_syncserver(const char* syncserver)
{
    rho_sync_stop();

	CSyncThread::getSyncEngine().setSyncServer(syncserver);

    if ( syncserver && *syncserver )
    {
        CSyncThread::getInstance()->start(CSyncThread::epLow);
        if ( CClientRegister::getInstance() != null )
            CClientRegister::getInstance()->startUp();
    }
    else
    {
        CSyncThread::getInstance()->stop(CSyncThread::SYNC_WAIT_BEFOREKILL_SECONDS);
        if ( CClientRegister::getInstance() != null )
            CClientRegister::getInstance()->stop(CSyncThread::SYNC_WAIT_BEFOREKILL_SECONDS);
    }
}

unsigned long rho_sync_login(const char *name, const char *password, const char* callback)
{
    rho_sync_stop();
    CSyncThread::getInstance()->addQueueCommand(new CSyncThread::CSyncLoginCommand(name, password, callback) );

    return CSyncThread::getInstance()->getRetValue();
}

int rho_sync_logged_in()
{
	//CDBAdapter& db = CDBAdapter::getUserDB();
    return CSyncThread::getSyncEngine().isLoggedIn() ? 1 : 0;
}

void rho_sync_logout()
{
    rho_sync_stop();

	//CDBAdapter& db = CDBAdapter::getUserDB();
    CSyncThread::getSyncEngine().logout();
}

void rho_sync_set_notification(int source_id, const char *url, char* params)
{
    return CSyncThread::getSyncEngine().getNotify().setSyncNotification(source_id, url, params ? params : "");
}

void rho_sync_clear_notification(int source_id)
{
    return CSyncThread::getSyncEngine().getNotify().clearSyncNotification(source_id);
}

#ifndef RHO_NO_RUBY
unsigned long rho_sync_get_attrs(const char* szPartition, int nSrcID)
{
    return (VALUE)CDBAdapter::getDB(szPartition).getAttrMgr().getAttrsBySrc(nSrcID);
}

unsigned long rho_sync_is_blob_attr(const char* szPartition, int nSrcID, const char* szAttrName)
{
    return rho_ruby_create_boolean( CDBAdapter::getDB(szPartition).getAttrMgr().isBlobAttr(nSrcID, szAttrName) );
}
#endif //RHO_NO_RUBY

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
    CSyncThread::getSyncEngine().setSyncPageSize(nPageSize);
}

void rho_sync_set_threaded_mode(int b)
{
    CSyncThread::getInstance()->setNonThreadedMode(b==0);
    CSyncThread::getSyncEngine().setNonThreadedMode(b==0);
}

char* rho_sync_create_string(const char* szStr)
{
    return strdup(szStr);
}

void rho_sync_free_string(char* szStr)
{
    return free(szStr);
}

}
