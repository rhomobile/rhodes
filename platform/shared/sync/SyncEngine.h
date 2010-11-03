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
    Vector<String>          m_arPartitions;
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
    int m_nErrCode;
    String m_strError;
    boolean m_bIsSearch, m_bIsSchemaChanged;

public:
    CSyncEngine();
    ~CSyncEngine(void){}

    void setFactory(common::IRhoClassFactory* factory){ 
        m_NetRequest = factory->createNetRequest();
    }

    void doSyncAllSources();
    void doSyncSource(const CSourceID& oSrcID);
    void doSearch(rho::Vector<rho::String>& arSources, String strParams, String strAction, boolean bSearchSyncChanges, int nProgressStep);

    void login(String name, String password, const CSyncNotification& oNotify);
    boolean isLoggedIn();
    String loadSession();
    void logout();
	void setSyncServer(const char* syncserver);

    void setState(ESyncState eState){ m_syncState = eState; }
    ESyncState getState()const{ return m_syncState; }
    boolean isSearch()const{ return m_bIsSearch; }
    boolean isContinueSync()const{ return m_syncState != esExit && m_syncState != esStop; }
	boolean isSyncing()const{ return m_syncState == esSyncAllSources || m_syncState == esSyncSource; }
    void stopSync(){ if (isContinueSync()){ setState(esStop); if(m_NetRequest) m_NetRequest->cancel();} }
    void stopSyncByUser(){ m_bStopByUser = true; stopSync(); }
    void exitSync(){ setState(esExit); m_NetRequest->cancel(); }
    boolean isStoppedByUser(){ return m_bStopByUser; }
//private:
    String getClientID()const{ return m_clientID; }
    void setSession(String strSession){m_strSession=strSession;}
    boolean isSessionExist(){ return m_strSession.length() > 0; }

    void setSchemaChanged(boolean bChanged){ m_bIsSchemaChanged = bChanged; }
    boolean isSchemaChanged(){ return m_bIsSchemaChanged; }
//IRhoSession
    virtual const String& getSession(){ return m_strSession; }
    virtual const String& getContentType(){ return getProtocol().getContentType();}

    void loadAllSources();
    void prepareSync(ESyncState eState, const CSourceID* oSrcID);

    VectorPtr<CSyncSource*>& getSources(){ return m_sources; }
    int getStartSource();
    String loadClientID();
    String readClientID();
    String requestClientIDByNet();
    boolean resetClientIDByNet(const String& strClientID);//throws Exception
    void doBulkSync();//throws Exception

    CSyncNotify& getNotify(){ return m_oSyncNotify; }
    net::INetRequest& getNet(){ return *m_NetRequest; }
    ISyncProtocol& getProtocol(){ return *m_SyncProtocol; }

    CSyncSource* findSourceByName(const String& strSrcName);

    int getSyncPageSize() { return m_nSyncPageSize; }
    void setSyncPageSize(int nPageSize){ m_nSyncPageSize = nPageSize; }

    boolean isNoThreadedMode(){ return m_bNoThreaded; }
    void setNonThreadedMode(boolean b){m_bNoThreaded = b;}

    void applyChangedValues(db::CDBAdapter& db);
private:
 
    CSyncSource* findSource(const CSourceID& oSrcID);

    void loadBulkPartition(const String& strPartition);
    String makeBulkDataFileName(String strDataUrl, String strDbPath, String strExt);
    db::CDBAdapter& getUserDB(){ return db::CDBAdapter::getUserDB(); }
    db::CDBAdapter& getDB(const String& strPartition){ return db::CDBAdapter::getDB(strPartition.c_str()); }

    void initProtocol();
    void processServerSources(String strSources);
    void checkSourceAssociations();

    boolean syncOneSource(int i);
    void syncAllSources();

    friend class CSyncSource;
};

}
}
