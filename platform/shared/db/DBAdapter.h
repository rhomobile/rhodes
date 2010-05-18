#pragma once

#include "DBResult.h"
#include "DBAttrManager.h"
#include "logging/RhoLog.h"

namespace rho{
namespace common{
class CRubyMutex
{
    int m_nLockCount;
    unsigned long m_valThread, m_valMutex;

public:
    CRubyMutex();
    ~CRubyMutex();

    boolean isMainRubyThread();
    void Lock();
    void Unlock();
};
}

namespace db{

class CDBAdapter
{
    sqlite3* m_dbHandle;
    String   m_strDbPath, m_strDbVer, m_strDbVerPath;
    String   m_strDbPartition;

    Hashtable<String,sqlite3_stmt*> m_mapStatements;
    common::CRubyMutex m_mxRuby;
    common::CMutex m_mxDB;
    boolean m_bUIWaitDB;
    int     m_nTransactionCounter;
    CDBAttrManager m_attrMgr;
    static HashtablePtr<String,CDBAdapter*> m_mapDBPartitions;

    struct CDBVersion
    {
    	String m_strRhoVer;
    	String m_strAppVer;
    	
        CDBVersion(){}
    	CDBVersion( String strRhoVer, String strAppVer )
    	{
    		m_strRhoVer = strRhoVer;
    		m_strAppVer = strAppVer;
    	}
    };

    static const char* USER_PARTITION_NAME(){return "user";}
public:
    DEFINE_LOGCLASS;

    CDBAdapter(const char* szDBPartition) : m_dbHandle(0), m_strDbPath(""), m_strDbPartition(szDBPartition),
        m_bUIWaitDB(false), m_nTransactionCounter(0) {}
    ~CDBAdapter(void){}

    void open (String strDbPath, String strVer, boolean bTemp);
    void close();
    sqlite3* getDbHandle(){ return m_dbHandle; }
    CDBAttrManager& getAttrMgr(){ return m_attrMgr; }

    boolean isUIWaitDB()const{ return m_bUIWaitDB; }
    void Lock();
    void Unlock();
    boolean isInsideTransaction(){ return m_nTransactionCounter > 0; }
    const String& getDBPath(){ return m_strDbPath; }

    static HashtablePtr<String,CDBAdapter*>& getDBPartitions(){ return  m_mapDBPartitions; }
    static void closeAll();
    static void initAttrManager();
    static boolean isAnyInsideTransaction();
    static CDBAdapter& getUserDB();
    static CDBAdapter& getDBByHandle(sqlite3* db);
    static CDBAdapter& getDB(const char* szPartition);

    boolean isTableExist(String strTableName);
    int prepareSqlStatement(const char* szSql, int nByte, sqlite3_stmt **ppStmt);

    void bind(sqlite3_stmt* st, int nPos, int val)
    {
        sqlite3_bind_int(st, nPos, val);
    }
    void bind(sqlite3_stmt* st, int nPos, const String& val)
    {
        sqlite3_bind_text(st, nPos, val.c_str(), -1, SQLITE_TRANSIENT);
    }
    void bind(sqlite3_stmt* st, int nPos, const char* val)
    {
        sqlite3_bind_text(st, nPos, val, -1, SQLITE_TRANSIENT);
    }
    void bind(sqlite3_stmt* st, int nPos, uint64 val)
    {
        sqlite3_bind_int64(st, nPos, val);
    }
    void bind(sqlite3_stmt* st, int nPos, unsigned long val)
    {
        sqlite3_bind_int(st, nPos, static_cast<int>(val));
    }

    template<typename T1, typename T2>
    DBResultPtr executeSQL( const char* szSt, T1 p1, T2 p2 )
    {
        DBResultPtr res = prepareStatement(szSt);
        if ( res->getStatement() == null )
            return res;

        bind(res->getStatement(), 1, p1);
        bind(res->getStatement(), 2, p2);

        return executeStatement(res);
    }

    template<typename T1, typename T2, typename T3>
    DBResultPtr executeSQL( const char* szSt, T1 p1, T2 p2, T3 p3 )
    {
        DBResultPtr res = prepareStatement(szSt);
        if ( res->getStatement() == null )
            return res;

        bind(res->getStatement(), 1, p1);
        bind(res->getStatement(), 2, p2);
        bind(res->getStatement(), 3, p3);

        return executeStatement(res);
    }

    template<typename T1, typename T2, typename T3, typename T4>
    DBResultPtr executeSQL( const char* szSt, T1 p1, T2 p2, T3 p3, T4 p4 )
    {
        DBResultPtr res = prepareStatement(szSt);
        if ( res->getStatement() == null )
            return res;

        bind(res->getStatement(), 1, p1);
        bind(res->getStatement(), 2, p2);
        bind(res->getStatement(), 3, p3);
        bind(res->getStatement(), 4, p4);

        return executeStatement(res);
    }

    template<typename T1, typename T2, typename T3, typename T4, typename T5>
    DBResultPtr executeSQL( const char* szSt, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5 )
    {
        DBResultPtr res = prepareStatement(szSt);
        if ( res->getStatement() == null )
            return res;

        bind(res->getStatement(), 1, p1);
        bind(res->getStatement(), 2, p2);
        bind(res->getStatement(), 3, p3);
        bind(res->getStatement(), 4, p4);
        bind(res->getStatement(), 5, p5);

        return executeStatement(res);
    }

    template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
    DBResultPtr executeSQL( const char* szSt, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6 )
    {
        DBResultPtr res = prepareStatement(szSt);
        if ( res->getStatement() == null )
            return res;

        bind(res->getStatement(), 1, p1);
        bind(res->getStatement(), 2, p2);
        bind(res->getStatement(), 3, p3);
        bind(res->getStatement(), 4, p4);
        bind(res->getStatement(), 5, p5);
        bind(res->getStatement(), 6, p6);

        return executeStatement(res);
    }

    DBResultPtr executeSQLReportNonUniqueEx( const char* szSt, Vector<String>& arValues );

    template<typename T1, typename T2, typename T3, typename T4>
    DBResultPtr executeSQLReportNonUnique( const char* szSt, T1 p1, T2 p2, T3 p3, T4 p4 )
    {
        DBResultPtr res = prepareStatement(szSt);
        if ( res->getStatement() == null )
            return res;

        bind(res->getStatement(), 1, p1);
        bind(res->getStatement(), 2, p2);
        bind(res->getStatement(), 3, p3);
        bind(res->getStatement(), 4, p4);

        res->setReportNonUnique(true);
        return executeStatement(res);
    }

    template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
    DBResultPtr executeSQL( const char* szSt, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7 )
    {
        DBResultPtr res = prepareStatement(szSt);
        if ( res->getStatement() == null )
            return res;

        bind(res->getStatement(), 1, p1);
        bind(res->getStatement(), 2, p2);
        bind(res->getStatement(), 3, p3);
        bind(res->getStatement(), 4, p4);
        bind(res->getStatement(), 5, p5);
        bind(res->getStatement(), 6, p6);
        bind(res->getStatement(), 7, p7);

        return executeStatement(res);
    }

    template<typename T1>
    DBResultPtr executeSQL( const char* szSt, T1 p1)
    {
        DBResultPtr res = prepareStatement(szSt);
        if ( res->getStatement() == null )
            return res;

        bind(res->getStatement(), 1, p1);
        return executeStatement(res);
    }

    DBResultPtr executeSQLEx( const char* szSt, Vector<String>& arValues);
    DBResultPtr executeSQL( const char* szSt);

    void startTransaction();
    void endTransaction();
    void rollback();
    void destroy_tables(const rho::Vector<rho::String>& arIncludeTables, const rho::Vector<rho::String>& arExcludeTables);
    void setBulkSyncDB(String fDataName);

    void createDeleteTrigger(const String& strTable);
private:
    DBResultPtr executeStatement(common::CAutoPtr<CDBResult>& res);

    void checkDBVersion(String& strVer);
    CDBVersion readDBVersion();//throws Exception
    void       writeDBVersion(const CDBVersion& ver);//throws Exception
    void createSchema();
    void createTriggers();
    boolean checkDbError(int rc);
    boolean checkDbErrorEx(int rc, rho::db::CDBResult& res);
    sqlite3_stmt* createInsertStatement(rho::db::CDBResult& res, const String& tableName, CDBAdapter& db, String& strInsert);

    virtual DBResultPtr prepareStatement( const char* szSt );

    boolean migrateDB(const CDBVersion& dbVer, const String& strRhoDBVer, const String& strAppDBVer);
    void copyTable(String tableName, CDBAdapter& dbFrom, CDBAdapter& dbTo);
};

}
}

extern "C" void rho_db_init_attr_manager();
