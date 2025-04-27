/*------------------------------------------------------------------------
* (The MIT License)
* 
* Copyright (c) 2008-2011 Rhomobile, Inc.
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
* 
* http://rhomobile.com
*------------------------------------------------------------------------*/

#pragma once

#include "DBResult.h"
#include "DBAttrManager.h"
#include "logging/RhoLog.h"
#include "common/IRhoCrypt.h"
#include "DBLock.h"

extern "C" void rhoInitSQLitePageSize(unsigned int);

namespace rho{

namespace db{
	

class CDBAdapter
{
    sqlite3* m_dbHandle;
    String   m_strDbPath, m_strDbVer, m_strDbVerPath;
    String   m_strDbPartition;

    Hashtable<String,sqlite3_stmt*> m_mapStatements;
    CRubyMutex m_mxRuby;
    common::CMutex m_mxDB;
    boolean m_bUIWaitDB;
    unsigned m_nTransactionCounter;
    CDBAttrManager m_attrMgr;
    static HashtablePtr<String,CDBAdapter*> m_mapDBPartitions;
    common::CAutoPtr<common::IRhoCrypt> m_ptrCrypt;
    String m_strCryptKey;

    struct CDBVersion
    {
    	String m_strRhoVer;
    	String m_strAppVer;
        boolean m_bEncrypted;

        boolean isRhoVerChanged(CDBVersion& dbNewVer)
    	{
    		return m_strRhoVer.compare(dbNewVer.m_strRhoVer) != 0;
    	}
    	boolean isAppVerChanged(CDBVersion& dbNewVer)
    	{
    		return m_strAppVer.compare(dbNewVer.m_strAppVer) != 0;
    	}
    	
    	boolean isDbFormatChanged(CDBVersion& dbNewVer)
    	{
    		return m_bEncrypted != dbNewVer.m_bEncrypted;// || 
    			//m_bSqlite != dbNewVer.m_bSqlite;
    	}
        void fromFile(const String& strFilePath);//throws Exception
        void toFile(const String& strFilePath)const;//throws Exception

        CDBVersion() : m_bEncrypted(false){}
    	CDBVersion( String strRhoVer, String strAppVer )
    	{
    		m_strRhoVer = strRhoVer;
    		m_strAppVer = strAppVer;
    	}
    };

public:
    static const char* USER_PARTITION_NAME(){return "user";}

    DEFINE_LOGCLASS;

    CDBAdapter(const char* szDBPartition, boolean bNoRubyLock) : m_dbHandle(0), m_strDbPath(""), m_strDbPartition(szDBPartition),
        m_mxRuby(bNoRubyLock), m_bUIWaitDB(false), m_nTransactionCounter(0) {
            if (usingDeprecatedPageSize()){
                rhoInitSQLitePageSize(1024);
            }else{
                rhoInitSQLitePageSize(4096);
            }            
        }
    ~CDBAdapter(void){}
    bool usingDeprecatedPageSize(){
        const char* depEncrypt = get_app_build_config_item("use_deprecated_encryption");
        return (depEncrypt && strcmp(depEncrypt, "1") == 0);
    }
    int checkoutRealPageSize(String &strDbPath, bool isExist, bool isEncrypted);
    void open (String strDbPath, String strVer, boolean bTemp, boolean checkImportState);
    void close(boolean bCloseRubyMutex = true);
    sqlite3* getDbHandle(){ return m_dbHandle; }
    CDBAttrManager& getAttrMgr(){ return m_attrMgr; }

    boolean isUIWaitDB()const{ return m_bUIWaitDB; }
    void Lock();
    void Unlock();
    boolean isInsideTransaction(){ return m_nTransactionCounter > 0; }
    const String& getDBPath(){ return m_strDbPath; }
    common::IRhoCrypt* getCrypt(){ return m_ptrCrypt; }
    void setCryptKey(String& strKey){ m_strCryptKey = strKey; }
	const String& getPartitionName() const { return m_strDbPartition; }
	const String& getDBVersion() const { return m_strDbVer; }

    static HashtablePtr<String,CDBAdapter*>& getDBPartitions(){ return  m_mapDBPartitions; }
    static void closeAll();
    static void initAttrManager();
    static boolean isAnyInsideTransaction();
    static CDBAdapter& getUserDB();
    static CDBAdapter& getDBByHandle(sqlite3* db);
    static CDBAdapter& getDB(const char* szPartition);
    static Vector<String> getDBAllPartitionNames();
    //static void destroy_tables_allpartitions(const rho::Vector<rho::String>& arIncludeTables, const rho::Vector<rho::String>& arExcludeTables);

    boolean isTableExist(String strTableName);
    int prepareSqlStatement(const char* szSql, int nByte, sqlite3_stmt **ppStmt);

    void bind(sqlite3_stmt* st, int nPos, int val)
    {
        sqlite3_bind_int(st, nPos, val);
    }
    void bind(sqlite3_stmt* st, int nPos, const String& val)
    {
        sqlite3_bind_text(st, nPos, val.c_str(), val.length(), SQLITE_TRANSIENT);
    }
    void bind(sqlite3_stmt* st, int nPos, const char* val)
    {
        sqlite3_bind_text(st, nPos, val, -1, SQLITE_TRANSIENT);
    }
    void bind(sqlite3_stmt* st, int nPos, uint64 val)
    {
        sqlite3_bind_int64(st, nPos, val);
    }
    void bind(sqlite3_stmt* st, int nPos, sqlite_int64 val)
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
        if ( res->getStatement() == NULL )
            return res;

        bind(res->getStatement(), 1, p1);
        bind(res->getStatement(), 2, p2);

        return executeStatement(res, szSt);
    }

    template<typename T1, typename T2, typename T3>
    DBResultPtr executeSQL( const char* szSt, T1 p1, T2 p2, T3 p3 )
    {
        DBResultPtr res = prepareStatement(szSt);
        if ( res->getStatement() == NULL )
            return res;

        bind(res->getStatement(), 1, p1);
        bind(res->getStatement(), 2, p2);
        bind(res->getStatement(), 3, p3);

        return executeStatement(res, szSt);
    }

    template<typename T1, typename T2, typename T3, typename T4>
    DBResultPtr executeSQL( const char* szSt, T1 p1, T2 p2, T3 p3, T4 p4 )
    {
        DBResultPtr res = prepareStatement(szSt);
        if ( res->getStatement() == NULL )
            return res;

        bind(res->getStatement(), 1, p1);
        bind(res->getStatement(), 2, p2);
        bind(res->getStatement(), 3, p3);
        bind(res->getStatement(), 4, p4);

        return executeStatement(res, szSt);
    }

    template<typename T1, typename T2, typename T3, typename T4, typename T5>
    DBResultPtr executeSQL( const char* szSt, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5 )
    {
        DBResultPtr res = prepareStatement(szSt);
        if ( res->getStatement() == NULL )
            return res;

        bind(res->getStatement(), 1, p1);
        bind(res->getStatement(), 2, p2);
        bind(res->getStatement(), 3, p3);
        bind(res->getStatement(), 4, p4);
        bind(res->getStatement(), 5, p5);

        return executeStatement(res, szSt);
    }

    template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
    DBResultPtr executeSQL( const char* szSt, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6 )
    {
        DBResultPtr res = prepareStatement(szSt);
        if ( res->getStatement() == NULL )
            return res;

        bind(res->getStatement(), 1, p1);
        bind(res->getStatement(), 2, p2);
        bind(res->getStatement(), 3, p3);
        bind(res->getStatement(), 4, p4);
        bind(res->getStatement(), 5, p5);
        bind(res->getStatement(), 6, p6);

        return executeStatement(res, szSt);
    }

    DBResultPtr executeSQLReportNonUniqueEx( const char* szSt, Vector<String>& arValues );

    template<typename T1, typename T2, typename T3, typename T4>
    DBResultPtr executeSQLReportNonUnique( const char* szSt, T1 p1, T2 p2, T3 p3, T4 p4 )
    {
        DBResultPtr res = prepareStatement(szSt);
        if ( res->getStatement() == NULL )
            return res;

        bind(res->getStatement(), 1, p1);
        bind(res->getStatement(), 2, p2);
        bind(res->getStatement(), 3, p3);
        bind(res->getStatement(), 4, p4);

        res->setReportNonUnique(true);
        return executeStatement(res, szSt);
    }

    template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
    DBResultPtr executeSQLReportNonUnique( const char* szSt, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7 )
    {
        DBResultPtr res = prepareStatement(szSt);
        if ( res->getStatement() == NULL )
            return res;

        bind(res->getStatement(), 1, p1);
        bind(res->getStatement(), 2, p2);
        bind(res->getStatement(), 3, p3);
        bind(res->getStatement(), 4, p4);
        bind(res->getStatement(), 5, p5);
        bind(res->getStatement(), 6, p6);
        bind(res->getStatement(), 7, p7);

        res->setReportNonUnique(true);
        return executeStatement(res, szSt);
    }

    template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
    DBResultPtr executeSQL( const char* szSt, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7 )
    {
        DBResultPtr res = prepareStatement(szSt);
        if ( res->getStatement() == NULL )
            return res;

        bind(res->getStatement(), 1, p1);
        bind(res->getStatement(), 2, p2);
        bind(res->getStatement(), 3, p3);
        bind(res->getStatement(), 4, p4);
        bind(res->getStatement(), 5, p5);
        bind(res->getStatement(), 6, p6);
        bind(res->getStatement(), 7, p7);

        return executeStatement(res, szSt);
    }

    template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
    DBResultPtr executeSQL( const char* szSt, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8 )
    {
        DBResultPtr res = prepareStatement(szSt);
        if ( res->getStatement() == NULL )
            return res;

        bind(res->getStatement(), 1, p1);
        bind(res->getStatement(), 2, p2);
        bind(res->getStatement(), 3, p3);
        bind(res->getStatement(), 4, p4);
        bind(res->getStatement(), 5, p5);
        bind(res->getStatement(), 6, p6);
        bind(res->getStatement(), 7, p7);
        bind(res->getStatement(), 8, p8);

        return executeStatement(res, szSt);
    }

    template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
    DBResultPtr executeSQL( const char* szSt, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9 )
    {
        DBResultPtr res = prepareStatement(szSt);
        if ( res->getStatement() == NULL )
            return res;

        bind(res->getStatement(), 1, p1);
        bind(res->getStatement(), 2, p2);
        bind(res->getStatement(), 3, p3);
        bind(res->getStatement(), 4, p4);
        bind(res->getStatement(), 5, p5);
        bind(res->getStatement(), 6, p6);
        bind(res->getStatement(), 7, p7);
        bind(res->getStatement(), 8, p8);
        bind(res->getStatement(), 9, p9);

        return executeStatement(res, szSt);
    }

    template<typename T1>
    DBResultPtr executeSQL( const char* szSt, T1 p1)
    {
        DBResultPtr res = prepareStatement(szSt);
        if ( res->getStatement() == NULL )
            return res;

        bind(res->getStatement(), 1, p1);
        return executeStatement(res, szSt);
    }

    DBResultPtr executeSQLEx( const char* szSt, Vector<String>& arValues);
    DBResultPtr executeSQL( const char* szSt);
	void        executeBatch(const char* szSql, CDBError& error);
	
    void startTransaction();
    void endTransaction();
    void rollback();
    void destroy_tables(const rho::Vector<rho::String>& arIncludeTables, const rho::Vector<rho::String>& arExcludeTables);
    void setBulkSyncDB(String fDataName, String strCryptKey);
	void setImportDB(String fDataName, String strCryptKey);

    void createTrigger(const String& strSQL);
    void dropTrigger(const String& strName);
	
    DBResultPtr prepareStatement( const char* szSt );
    DBResultPtr executeStatement(common::CAutoPtr<CDBResult>& res, const char* szSt);

    // unpack object in changed_values with update_type==create to set of objects's attribs
    void updateAllAttribChanges();
    
    // add file.blob attrib_type for blob attribs in changed_values
    void updateAllBlobAttribChanges();
    void updateFullUpdateChanges(int nSrcID);

	String exportDatabase();
	bool importDatabase( const String& zipName );
	
private:

    void checkDBVersion(String& strVer);
    void createSchema();
    String tauDecryptTextFile(const String fullPath);
    void createTriggers();
    boolean checkDbError(int rc);
    boolean checkDbErrorEx(int rc, rho::db::CDBResult& res);
    sqlite3_stmt* createInsertStatement(IDBResult& res, const String& tableName, CDBAdapter& db, String& strInsert);

    boolean migrateDB(const CDBVersion& dbVer, const CDBVersion& dbNewVer);
    void copyTable(String tableName, CDBAdapter& dbFrom, CDBAdapter& dbTo);
    void copyChangedValues(CDBAdapter& db);
};

}
}

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

void rho_db_init_attr_manager();
int rho_db_open(const char* szDBPath, const char* szDBPartition, void** ppDB);

#ifdef __cplusplus
};
#endif //__cplusplus
