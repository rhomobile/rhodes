#pragma once

#include "common/RhoStd.h"
#include "common/IRhoClassFactory.h"
#include "common/RhoMutexLock.h"
#include "net/INetRequest.h"
#include "ISyncProtocol.h"

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
static const int ERR_SYNCVERSION = 11;
static const int ERR_GEOLOCATION = 12;

static String getMessageText(const char* szName);
static String getErrorText(int nError);
};
extern const _CRhoRuby& RhoRuby;

namespace sync {

class CSyncEngine : public net::IRhoSession
{
    DEFINE_LOGCLASS;
public:
    enum ESyncState{ esNone, esSyncAllSources, esSyncSource, esSearch, esStop, esExit };

    struct CSourceID
    {
        String m_strName;
        int m_nID;

        CSourceID(int id, const String& strName ){ m_nID = id; m_strName = strName; }
        CSourceID(const String& strName ){ m_strName = strName; }

        String toString()const;
        boolean isEqual(CSyncSource& src)const;
    };

private:
    VectorPtr<CSyncSource*> m_sources;
    db::CDBAdapter& m_dbUserAdapter;
    db::CDBAdapter& m_dbAppAdapter;
    common::CAutoPtr<net::INetRequest> m_NetRequest;
    common::CAutoPtr<ISyncProtocol> m_SyncProtocol;
    ESyncState m_syncState;
    String     m_clientID;
    common::CMutex m_mxLoadClientID;
    String m_strSession;
    CSyncNotify m_oSyncNotify;
    boolean m_bStopByUser;
    int m_nSyncPageSize;
	boolean m_bNoThreaded;
    boolean m_bHasUserPartition;
    boolean m_bHasAppPartition;

public:
    CSyncEngine(db::CDBAdapter& dbUser, db::CDBAdapter& dbApp);
    ~CSyncEngine(void){}

    void setFactory(common::IRhoClassFactory* factory){ 
        m_NetRequest = factory->createNetRequest();
    }

    void doSyncAllSources();
    void doSyncSource(const CSourceID& oSrcID);
    void doSearch(rho::Vector<rho::String>& arSources, String strParams, String strAction, boolean bSearchSyncChanges, int nProgressStep);

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
    boolean isSessionExist(){ return m_strSession.length() > 0; }
//IRhoSession
    virtual const String& getSession(){ return m_strSession; }
    virtual const String& getContentType(){ return getProtocol().getContentType();}

    //CSyncEngine(): m_dbAdapter(db::CDBAdapter()), m_NetRequest(0), m_isLoggedIn(true){}

    void loadAllSources();
    void syncAllSources();
    void prepareSync(ESyncState eState);

    VectorPtr<CSyncSource*>& getSources(){ return m_sources; }
    int getStartSource();
    String loadClientID();
    String requestClientIDByNet();
    boolean resetClientIDByNet(const String& strClientID);//throws Exception
    void doBulkSync();//throws Exception

    db::CDBAdapter& getDB(){ return m_dbUserAdapter; }
    db::CDBAdapter& getAppDB(){ return m_dbAppAdapter; }

    CSyncNotify& getNotify(){ return m_oSyncNotify; }
    net::INetRequest& getNet(){ return *m_NetRequest; }
    ISyncProtocol& getProtocol(){ return *m_SyncProtocol; }

    CSyncSource* findSourceByName(const String& strSrcName);

    int getSyncPageSize() { return m_nSyncPageSize; }
    void setSyncPageSize(int nPageSize){ m_nSyncPageSize = nPageSize; }

    boolean isNoThreadedMode(){ return m_bNoThreaded; }
    void setNonThreadedMode(bool b){m_bNoThreaded = b;}
private:
 
    CSyncSource* findSource(const CSourceID& oSrcID);

    void loadBulkPartition(db::CDBAdapter& dbPartition, const String& strPartition);
    String makeBulkDataFileName(String strDataUrl, String strDbPath, String strExt);

    void initProtocol();

    friend class CSyncSource;
};

}
}
