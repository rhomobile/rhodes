#pragma once

#include "common/RhoStd.h"
#include "common/IRhoClassFactory.h"
#include "common/RhoMutexLock.h"
#include "net/INetRequest.h"

#include "SyncSource.h"
#include "db/DBAdapter.h"
#include "logging/RhoLog.h"

namespace rho {
namespace sync {

class CSyncEngine
{
    DEFINE_LOGCLASS;
    struct CSyncNotification
    {
        String m_strUrl, m_strParams;
        CSyncNotification(String strUrl, String strParams) : m_strUrl(strUrl), m_strParams(strParams){}
    };

public:
    enum ESyncState{ esNone, esSyncAllSources, esSyncSource, esStop, esExit };

    static String SYNC_SOURCE_FORMAT() { return "?format=json"; }
    static String SYNC_ASK_ACTION() { return "/ask"; }
//    static int MAX_SYNC_TRY_COUNT() { return 2; }
#ifdef OS_SYMBIAN
    static String SYNC_PAGE_SIZE() { return "200"; }
#else
    static String SYNC_PAGE_SIZE() { return "1000"; }
#endif

private:
    VectorPtr<CSyncSource*> m_sources;
    db::CDBAdapter& m_dbAdapter;
    net::INetRequest* m_NetRequest;
    ESyncState m_syncState;
    String     m_clientID;
    HashtablePtr<int,CSyncNotification*> m_mapNotifications;
    common::CMutex m_mxNotifications;
    boolean m_isLoggedIn;

public:
    CSyncEngine(db::CDBAdapter& db): m_dbAdapter(db), m_NetRequest(0), m_syncState(esNone), m_isLoggedIn(false){}
    ~CSyncEngine(void){}

    void setFactory(common::IRhoClassFactory* factory){ 
        m_NetRequest = factory->createNetRequest();
    }

    void doSyncAllSources();
    void doSyncSource();
    boolean login(String name, String password);
    boolean isLoggedIn();
    void logout();
    void resetSyncDB();
    void setNotification(int source_id, String strUrl, String strParams );
    void clearNotification(int source_id);

    void setState(ESyncState eState){ m_syncState = eState; }
    ESyncState getState()const{ return m_syncState; }
    boolean isContinueSync()const{ return m_syncState != esExit && m_syncState != esStop; }
    void stopSync(){ if (isContinueSync()) setState(esStop); }
    void exitSync(){ setState(esExit); getNet().cancelAll(); }
//private:
    String getClientID()const{ return m_clientID; }

    //CSyncEngine(): m_dbAdapter(db::CDBAdapter()), m_NetRequest(0), m_isLoggedIn(true){}

    void setLoggedIn(boolean b){m_isLoggedIn=b;}

    void loadAllSources();
    void syncAllSources();
    VectorPtr<CSyncSource*>& getSources(){ return m_sources; }
    int getStartSource();
    void loadClientID();
    String requestClientIDByNet();

    boolean doLogin(String name, String password);

    void fireNotification( int nSourceID, int nSyncObjectsCount);

private:
    db::CDBAdapter& getDB(){ return m_dbAdapter; }
    net::INetRequest& getNet(){ return *m_NetRequest;}

    friend class CSyncSource;
};

}
}