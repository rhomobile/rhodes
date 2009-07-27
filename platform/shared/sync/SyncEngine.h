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
    common::CAutoPtr<net::INetRequest> m_NetRequest;
    ESyncState m_syncState;
    String     m_clientID;
    HashtablePtr<int,CSyncNotification*> m_mapNotifications;
    common::CMutex m_mxNotifications;
    common::CMutex m_mxLoadClientID;
    String m_strSession;

public:
    CSyncEngine(db::CDBAdapter& db): m_dbAdapter(db), m_NetRequest(0), m_syncState(esNone){}
    ~CSyncEngine(void){}

    void setFactory(common::IRhoClassFactory* factory){ 
        m_NetRequest = factory->createNetRequest();
    }

    void doSyncAllSources();
    void doSyncSource(int nSrcId, String strSrcUrl);
    void login(String name, String password, String callback);
    boolean isLoggedIn();
    String loadSession();
    void logout();
    void setNotification(int source_id, String strUrl, String strParams );
    void clearNotification(int source_id);
	void setSyncServer(char* syncserver);

    void setState(ESyncState eState){ m_syncState = eState; }
    ESyncState getState()const{ return m_syncState; }
    boolean isContinueSync()const{ return m_syncState != esExit && m_syncState != esStop; }
	boolean isSyncing()const{ return m_syncState == esSyncAllSources || m_syncState == esSyncSource; }
    void stopSync(){ if (isContinueSync()){ setState(esStop); m_NetRequest->cancel();} }
    void exitSync(){ setState(esExit); m_NetRequest->cancel(); }
//private:
    String getClientID()const{ return m_clientID; }
    void setSession(String strSession){m_strSession=strSession;}
    String getSession(){ return m_strSession; }
    boolean isSessionExist(){ return m_strSession.length() > 0; }

    //CSyncEngine(): m_dbAdapter(db::CDBAdapter()), m_NetRequest(0), m_isLoggedIn(true){}

    void loadAllSources();
    void syncAllSources();
    VectorPtr<CSyncSource*>& getSources(){ return m_sources; }
    int getStartSource();
    String loadClientID();
    String requestClientIDByNet();
    boolean resetClientIDByNet();//throws Exception

    void fireNotification( CSyncSource& src, boolean bFinish );

    db::CDBAdapter& getDB(){ return m_dbAdapter; }

private:
    net::INetRequest& getNet(){ return *m_NetRequest; }

    CSyncSource* findSourceByID(int nSrcId);
    CSyncSource* findSourceByUrl(const String& strSrcUrl);

    void callLoginCallback(String callback, int nErrCode, String strMessage);
    boolean checkAllSourcesFromOneDomain();

    friend class CSyncSource;
};

}
}