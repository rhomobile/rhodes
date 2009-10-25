#pragma once

#include "DBResult.h"
#include "DBAttrManager.h"
#include "logging/RhoLog.h"

namespace rho{
namespace db{

class CDBAdapter
{
    String   m_strDbPath, m_strDbVer, m_strDbVerPath;
    sqlite3* m_dbHandle;
    Hashtable<String,sqlite3_stmt*> m_mapStatements;
    common::CMutex m_mxDB;
    common::CMutex m_mxTransDB;
	boolean m_bInsideTransaction;
    boolean m_bUnlockDB;
    CDBAttrManager m_attrMgr;

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

public:
    DEFINE_LOGCLASS;

    CDBAdapter(void) : m_dbHandle(0), m_strDbPath(""), m_bUnlockDB(false), m_bInsideTransaction(false){}
    ~CDBAdapter(void){}

    void open (String strDbPath, String strVer, boolean bTemp);
    void close();
    sqlite3* getDbHandle(){ return m_dbHandle; }
    CDBAttrManager& getAttrMgr(){ return m_attrMgr; }

    boolean isUnlockDB()const{ return m_bUnlockDB; }
    void setUnlockDB(boolean b){ m_bUnlockDB = b; }
    void Lock(){ m_mxDB.Lock(); }
    void Unlock(){ setUnlockDB(false); m_mxDB.Unlock(); }
    boolean isInsideTransaction(){ return m_bInsideTransaction; }

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

    template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
    DBResultPtr executeSQLReportNonUnique( const char* szSt, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6 )
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

    DBResultPtr executeSQL( const char* szSt);

    void startTransaction();
    void endTransaction();
    void rollback();
    void destroy_table(String strTable);

    static String makeBlobFolderName();
//private:
    DBResultPtr executeStatement(common::CAutoPtr<CDBResult>& res);

    void checkDBVersion(String& strVer);
    CDBVersion readDBVersion();//throws Exception
    void       writeDBVersion(const CDBVersion& ver);//throws Exception
    void createSchema();
    boolean checkDbError(int rc);
    boolean checkDbErrorEx(int rc, rho::db::CDBResult& res);
    sqlite3_stmt* createInsertStatement(rho::db::CDBResult& res, const String& tableName, CDBAdapter& db, String& strInsert);

    virtual DBResultPtr prepareStatement( const char* szSt );
};

}
}

