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
}

namespace sync {

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

class CSyncEngine;
class CSyncSource
{
    DEFINE_LOGCLASS;

    CSyncEngine& m_syncEngine;

    int    m_nID;
    String m_strUrl;
    uint64 m_token;

    int m_nCurPageCount, m_nInserted, m_nDeleted, m_nTotalCount;
    String m_strAskParams;
    VectorPtr<CSyncBlob*> m_arSyncBlobs;
    boolean m_bGetAtLeastOnePage;

public:
    CSyncSource(int id, const String& strUrl, uint64 token, CSyncEngine& syncEngine );
    virtual void sync();

    String getUrl()const { return m_strUrl; }
    int getID()const { return m_nID; }
    int getServerObjectsCount()const{ return m_nInserted+m_nDeleted; }

    uint64 getToken()const{ return m_token; }
    void setToken(uint64 token){ m_token = token; }
    virtual boolean isEmptyToken()
    {
        return m_token == 0;
    }

//private:
    CSyncSource();
    CSyncSource(CSyncEngine& syncEngine ) : m_syncEngine(syncEngine)
    {
        m_nID = 0;
        m_strUrl = "";
        m_token = 0;

        m_nCurPageCount = 0;
        m_nInserted = 0;
        m_nDeleted = 0;
    }

    void syncClientChanges();
    void syncServerChanges();
    void makePushBody(String& strBody, const char* szUpdateType);
    void getAndremoveAsk();
    void setAskParams(const String& ask){ m_strAskParams = ask;}
    String getAskParams()const{ return m_strAskParams;}
    void processToken(uint64 token);

    int getInsertedCount()const { return m_nInserted; }
    int getDeletedCount()const { return m_nDeleted; }
    void setCurPageCount(int nCurPageCount){m_nCurPageCount = nCurPageCount;}
    void setTotalCount(int nTotalCount){m_nTotalCount = nTotalCount;}
    int  getCurPageCount(){return m_nCurPageCount;}
    int  getTotalCount(){return m_nTotalCount;}

    void processServerData(const char* szData);
    void processSyncObject(json::CJSONEntry& oJsonEntry);

    VectorPtr<CSyncBlob*>& getSyncBlobs(){ return m_arSyncBlobs; }
    void syncClientBlobs(const String& strBaseQuery);

private:
    CSyncEngine& getSync(){ return m_syncEngine; }
    db::CDBAdapter& getDB();
    net::INetRequest& getNet();

};

}
}