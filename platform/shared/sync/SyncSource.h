#pragma once

#include "common/RhoStd.h"
#include "db/DBResult.h"
#include "logging/RhoLog.h"

namespace rho {

namespace db {
    class CDBAdapter;
}
namespace net {
    struct INetRequest;
}
namespace json {
    class CJSONEntry;
    class CJSONArrayIterator;
    class CJSONStructIterator;
}

namespace sync {
struct ISyncProtocol;

class CSyncBlob
{
   String m_strBody;
   String m_strFilePath;

public:
    CSyncBlob(const String& body, const String& filePath ) :
      m_strBody(body), m_strFilePath(filePath){}

    const String& getBody()const{ return m_strBody; }
    const String& getFilePath()const{ return m_strFilePath; }
};

class CValue
{
public:
    String m_strValue;
	String m_strAttrType;
	uint64 m_nID;
	
    CValue(json::CJSONEntry& oJsonEntry);//throws JSONException
    CValue(json::CJSONEntry& oJsonEntry, int nVer);//throws JSONException
};


class CSyncEngine;
class CSyncNotify;
class CSyncSource
{
    DEFINE_LOGCLASS;

    CSyncEngine& m_syncEngine;
    db::CDBAdapter& m_dbAdapter;

    int    m_nID;
    String m_strName;
    uint64 m_token;
    String m_strSyncType;
    boolean m_bTokenFromDB;

    int m_nCurPageCount, m_nInserted, m_nDeleted, m_nTotalCount;
    boolean m_bGetAtLeastOnePage;
    int m_nRefreshTime;
    int m_nProgressStep;

public:
    int m_nErrCode;
    String m_strError;
    boolean m_bIsSearch;
private:
    VectorPtr<CSyncBlob*> m_arSyncBlobs;

public:
    CSyncSource(int id, const String& strName, uint64 token, const String& strSyncType, db::CDBAdapter& db, CSyncEngine& syncEngine );
    virtual void sync();
    virtual boolean syncClientChanges();

    int getID()const { return m_nID; }
    String getName() { return m_strName; }
    String getSyncType(){ return m_strSyncType; }

    int getServerObjectsCount()const{ return m_nInserted+m_nDeleted; }
    boolean isSearch()const{ return m_bIsSearch;}

    uint64 getToken()const{ return m_token; }
    boolean isTokenFromDB(){ return m_bTokenFromDB; }
    void setToken(uint64 token){ m_token = token; m_bTokenFromDB = false; }
    virtual boolean isEmptyToken()
    {
        return m_token == 0;
    }

    int getProgressStep(){ return m_nProgressStep; }
    void setProgressStep(int nProgressStep){ m_nProgressStep = nProgressStep; }

    boolean getGetAtLeastOnePage(){ return m_bGetAtLeastOnePage; }
    int getRefreshTime(){ return m_nRefreshTime; }

//private:
    //CSyncSource();
    CSyncSource(CSyncEngine& syncEngine, db::CDBAdapter& db );

    void doSyncClientChanges();
    boolean isPendingClientChanges();

    void syncServerChanges();
    void makePushBody_Ver3(String& strBody, const String& strUpdateType);
    void afterSyncClientChanges(boolean arUpdateSent[]);

    void processToken(uint64 token);

    int getInsertedCount()const { return m_nInserted; }
    int getDeletedCount()const { return m_nDeleted; }
    void setCurPageCount(int nCurPageCount){m_nCurPageCount = nCurPageCount;}
    void setTotalCount(int nTotalCount){m_nTotalCount = nTotalCount;}
    int  getCurPageCount(){return m_nCurPageCount;}
    int  getTotalCount(){return m_nTotalCount;}

    void processServerResponse_ver3(json::CJSONArrayIterator& oJsonArr);
    void processServerCmd_Ver3(const String& strCmd, const String& strObject, const String& strAttrib, const String& strValue);//throws Exception

    VectorPtr<CSyncBlob*>& getSyncBlobs(){ return m_arSyncBlobs; }
    void syncClientBlobs(const String& strBaseQuery);

    String makeFileName(const CValue& value);//throws Exception
    boolean downloadBlob(CValue& value);//throws Exception

    void setRefreshTime( int nRefreshTime ){ m_nRefreshTime = nRefreshTime;}

    db::CDBAdapter& getDB(){ return m_dbAdapter; }

private:
    CSyncEngine& getSync(){ return m_syncEngine; }
    CSyncNotify& getNotify();
    net::INetRequest& getNet();
    ISyncProtocol& getProtocol();

};

}
}
