#pragma once

#include "common/RhoStd.h"
#include "common/IRhoClassFactory.h"
#include "common/RhoMutexLock.h"
#include "net/INetRequest.h"

#include "SyncSource.h"
#include "SyncNotify.h"
#include "db/DBAdapter.h"
#include "logging/RhoLog.h"

namespace rho {
struct _CRhoRuby {
static const int ERR_NONE = 0;
static const int ERR_NETWORK = 1;
static const int ERR_REMOTESERVER = 2;
static const int ERR_RUNTIME = 3;
static const int ERR_UNEXPECTEDSERVERRESPONSE = 4;
static const int ERR_DIFFDOMAINSINSYNCSRC = 5;
static const int ERR_NOSERVERRESPONSE = 6;
static const int ERR_CLIENTISNOTLOGGEDIN = 7;
static const int ERR_CUSTOMSYNCSERVER = 8;
static const int ERR_UNATHORIZED = 9;
static const int ERR_CANCELBYUSER = 10;

static String getMessageText(const char* szName);
static String getErrorText(int nError);
};
extern const _CRhoRuby& RhoRuby;

namespace sync {

class CSyncEngine : public net::IRhoSession
{
    DEFINE_LOGCLASS;
public:
    enum ESyncState{ esNone, esSyncAllSources, esSyncSource, esStop, esExit };

    static String SYNC_SOURCE_FORMAT() { return "?format=json"; }
    static int SYNC_VERSION() { return 2; }

    static String SYNC_ASK_ACTION() { return "/ask"; }
//    static int MAX_SYNC_TRY_COUNT() { return 2; }

    struct CSourceID
    {
        String m_strName;
        String m_strUrl;
        int m_nID;

        String toString()const;
        boolean isEqual(CSyncSource& src)const;
    };

private:
    VectorPtr<CSyncSource*> m_sources;
    db::CDBAdapter& m_dbAdapter;
    common::CAutoPtr<net::INetRequest> m_NetRequest;
    ESyncState m_syncState;
    String     m_clientID;
    common::CMutex m_mxLoadClientID;
    String m_strSession;
    CSyncNotify m_oSyncNotify;
    boolean m_bStopByUser;
    int m_nSyncPageSize;
    boolean m_bNoThreaded;

public:
    CSyncEngine(db::CDBAdapter& db);
    ~CSyncEngine(void){}

    void setFactory(common::IRhoClassFactory* factory){ 
        m_NetRequest = factory->createNetRequest();
    }

    void doSyncAllSources();
    void doSyncSource(const CSourceID& oSrcID, String strParams, String strAction, boolean bSearchSyncChanges, int nProgressStep);
    void login(String name, String password, String callback);
    boolean isLoggedIn();
    String loadSession();
    void logout();
	void setSyncServer(char* syncserver);

    void setState(ESyncState eState){ m_syncState = eState; }
    ESyncState getState()const{ return m_syncState; }
    boolean isContinueSync()const{ return m_syncState != esExit && m_syncState != esStop; }
	boolean isSyncing()const{ return m_syncState == esSyncAllSources || m_syncState == esSyncSource; }
    void stopSync(){ if (isContinueSync()){ setState(esStop); m_NetRequest->cancel();} }
    void stopSyncByUser(){ m_bStopByUser = true; stopSync(); }
    void exitSync(){ setState(esExit); m_NetRequest->cancel(); }
    boolean isStoppedByUser(){ return m_bStopByUser; }
//private:
    String getClientID()const{ return m_clientID; }
    void setSession(String strSession){m_strSession=strSession;}
    const String& getSession(){ return m_strSession; }
    boolean isSessionExist(){ return m_strSession.length() > 0; }

    //CSyncEngine(): m_dbAdapter(db::CDBAdapter()), m_NetRequest(0), m_isLoggedIn(true){}

    void loadAllSources();
    void syncAllSources();
    VectorPtr<CSyncSource*>& getSources(){ return m_sources; }
    int getStartSource();
    String loadClientID();
    String requestClientIDByNet();
    boolean resetClientIDByNet(const String& strClientID);//throws Exception
    void doInitialSync(String strClientID);//throws Exception

    db::CDBAdapter& getDB(){ return m_dbAdapter; }
    CSyncNotify& getNotify(){ return m_oSyncNotify; }
    net::INetRequest& getNet(){ return *m_NetRequest; }

    CSyncSource* findSourceByName(const String& strSrcName);

    String SYNC_PAGE_SIZE();
    int getSyncPageSize() { return m_nSyncPageSize; }
    void setSyncPageSize(int nPageSize){ m_nSyncPageSize = nPageSize; }

    boolean isNoThreadedMode(){ return m_bNoThreaded; }
    void setNonThreadedMode(bool b){m_bNoThreaded = b;}
private:
 
    CSyncSource* findSource(const CSourceID& oSrcID);

    boolean checkAllSourcesFromOneDomain();
    friend class CSyncSource;
};

}
}
