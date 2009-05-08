#pragma once

#include "common/RhoStd.h"
#include "db/DBResult.h"

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

class CSyncEngine;
class CSyncSource
{
    CSyncEngine& m_syncEngine;
    int m_nCurPageCount, m_nInserted, m_nDeleted;
    uint64 m_token;

public:
    CSyncSource(db::CDBResult& dbRes, CSyncEngine& syncEngine ) : m_syncEngine(syncEngine)
    {
        m_nCurPageCount = 0;
        m_nInserted = 0;
        m_nDeleted = 0;
    }

    virtual void sync();

    String getUrl()const { return ""; }
    int getID()const { return 0; }
    int getServerObjectsCount()const { return 0; }

    uint64 getToken()const{ return m_token; }
    void setToken(uint64 token){ m_token = token; }
    virtual boolean isEmptyToken()
    {
        return m_token == 0;
    }

//private:
    CSyncSource();

    void syncClientChanges();
    void syncServerChanges();
    void makePushBody(String& strBody, const char* szUpdateType);
    String getAndremoveAsk();
    void processToken(uint64 token);

    int getInsertedCount()const { return m_nInserted; }
    int getDeletedCount()const { return m_nDeleted; }
    void setCurPageCount(int nCurPageCount){m_nCurPageCount = nCurPageCount;}
    int  getCurPageCount()const{return m_nCurPageCount;}

    void processServerData(const String& strData);
    void processSyncObject(json::CJSONEntry& oJsonEntry);

private:
    CSyncEngine& getSync(){ return m_syncEngine; }
    db::CDBAdapter& getDB();
    net::INetRequest& getNet();

};

}
}