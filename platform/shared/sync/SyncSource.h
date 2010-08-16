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
    struct CMultipartItem;
}
namespace json {
    class CJSONEntry;
    class CJSONArrayIterator;
    class CJSONStructIterator;
}

namespace sync {
struct ISyncProtocol;

class CAttrValue
{
public:
	String m_strAttrib;
    String m_strValue;
    String m_strBlobSuffix;

    CAttrValue(const String& strAttrib, const String& strValue);
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
    boolean m_bSchemaSource;

    struct CAssociation
    {
        String m_strSrcName, m_strAttrib;
        CAssociation( String strSrcName, String strAttrib ){m_strSrcName = strSrcName; m_strAttrib = strAttrib; }
    };
    Vector<CAssociation> m_arAssociations;
    VectorPtr<net::CMultipartItem*> m_arMultipartItems;
    Vector<String>                  m_arBlobAttrs;

public:
    int m_nErrCode;
    String m_strError;

public:
    CSyncSource(int id, const String& strName, const String& strSyncType, db::CDBAdapter& db, CSyncEngine& syncEngine );
    virtual void sync();
    virtual boolean syncClientChanges();

    int getID()const { return m_nID; }
    String getName() { return m_strName; }
    String getSyncType(){ return m_strSyncType; }

    int getServerObjectsCount()const{ return m_nInserted+m_nDeleted; }

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

    void processToken(uint64 token);

    int getInsertedCount()const { return m_nInserted; }
    int getDeletedCount()const { return m_nDeleted; }
    void setCurPageCount(int nCurPageCount){m_nCurPageCount = nCurPageCount;}
    void setTotalCount(int nTotalCount){m_nTotalCount = nTotalCount;}
    int  getCurPageCount(){return m_nCurPageCount;}
    int  getTotalCount(){return m_nTotalCount;}

    void processServerResponse_ver3(json::CJSONArrayIterator& oJsonArr);
    void processServerCmd_Ver3(const String& strCmd, const String& strObject, const String& strAttrib, const String& strValue);//throws Exception

    String makeFileName(const CAttrValue& value);//throws Exception
    boolean downloadBlob(CAttrValue& value);//throws Exception
    boolean processBlob( const String& strCmd, const String& strObject, CAttrValue& oAttrValue );

    void setRefreshTime( int nRefreshTime ){ m_nRefreshTime = nRefreshTime;}

    db::CDBAdapter& getDB(){ return m_dbAdapter; }

    void processSyncCommand(const String& strCmd, json::CJSONEntry oCmdEntry);

    void processServerCmd_Ver3_Schema(const String& strCmd, const String& strObject, json::CJSONStructIterator& attrIter);//throws Exception

    void parseAssociations(const String& strAssociations);
    void processAssociations(const String& strOldObject, const String& strNewObject);
    void updateAssociation(const String& strOldObject, const String& strNewObject, const String& strAttrib);
private:
    CSyncEngine& getSync(){ return m_syncEngine; }
    CSyncNotify& getNotify();
    net::INetRequest& getNet();
    ISyncProtocol& getProtocol();

};

}
}
