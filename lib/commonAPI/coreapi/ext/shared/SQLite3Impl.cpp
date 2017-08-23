#include "generated/cpp/SQLite3Base.h"
#include "json/JSONIterator.h"
#include "db/DBAdapter.h"
#include "common/RhodesApp.h"
#include "statistic/RhoProfiler.h"
#include "common/StringConverter.h"

extern "C" {
int rho_db_open(const char* szDBPath, const char* szDBPartition, void** ppDB);
int rho_db_close(void* pDB);
int rho_db_startTransaction(void* pDB);
int rho_db_commitTransaction(void* pDB);
int rho_db_rollbackTransaction(void* pDB);
int rho_db_is_ui_waitfordb(void* pDB);
void rho_db_lock(void* pDB);
void rho_db_unlock(void* pDB);
int  rho_db_is_table_exist(void* pDB, const char* szTableName);
unsigned long rb_c_impl_SQLite3_execute(int argc, unsigned long *argv, void* pDB);

void* rho_db_get_handle(void* pDB);
int rho_db_prepare_statement(void* pDB, const char* szSql, int nByte, sqlite3_stmt **ppStmt);

};

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "SQLite3"

namespace rho {
namespace database {

using namespace rho::apiGenerator;
using namespace rho::common;

class CSQLite3Impl: public CSQLite3Base
{
public:

    virtual void open( const rho::String& dbPath,  const rho::String& dbPartition, rho::apiGenerator::CMethodResult& oResult);
    virtual void close(rho::apiGenerator::CMethodResult& oResult);
    virtual void startTransaction(rho::apiGenerator::CMethodResult& oResult);
    virtual void commitTransaction(rho::apiGenerator::CMethodResult& oResult);
    virtual void rollbackTransaction(rho::apiGenerator::CMethodResult& oResult);
    virtual void lockDb(rho::apiGenerator::CMethodResult& oResult);
    virtual void unlockDb(rho::apiGenerator::CMethodResult& oResult);
	virtual void doImport(const rho::String& zipName, rho::apiGenerator::CMethodResult& oResult);
	virtual void doExport(rho::apiGenerator::CMethodResult& oResult);
    virtual void destroyTables(const rho::Vector<rho::String>& include, const rho::Vector<rho::String>& exclude, rho::apiGenerator::CMethodResult& oResult);
    virtual void isTableExist( const rho::String& tableName, rho::apiGenerator::CMethodResult& oResult);
    virtual void isUiWaitForDb(rho::apiGenerator::CMethodResult& oResult);
	void* getDb() const {return m_pDB;}

private:
	void* m_pDB;

};

void CSQLite3Impl::open( const rho::String& dbPath,  const rho::String& dbPartition, rho::apiGenerator::CMethodResult& oResult)
{
	m_pDB = NULL;
	rho_db_open(dbPath.c_str(), dbPartition.c_str(), &m_pDB);
    //TODO: process errors
}

void CSQLite3Impl::close(rho::apiGenerator::CMethodResult& oResult)
{
	rho_db_close(m_pDB);
    //TODO: process errors
}

void CSQLite3Impl::startTransaction(rho::apiGenerator::CMethodResult& oResult)
{
	rho_db_startTransaction(m_pDB);
    //TODO: process errors
}

void CSQLite3Impl::commitTransaction(rho::apiGenerator::CMethodResult& oResult)
{
	rho_db_commitTransaction(m_pDB);
    //TODO: process errors
}

void CSQLite3Impl::rollbackTransaction(rho::apiGenerator::CMethodResult& oResult)
{
    rho_db_rollbackTransaction(m_pDB);
}

void CSQLite3Impl::lockDb(rho::apiGenerator::CMethodResult& oResult)
{
	rho_db_lock(m_pDB);
}

void CSQLite3Impl::unlockDb(rho::apiGenerator::CMethodResult& oResult)
{
	rho_db_unlock(m_pDB);
}

void CSQLite3Impl::destroyTables(const rho::Vector<rho::String>& include, const rho::Vector<rho::String>& exclude, rho::apiGenerator::CMethodResult& oResult)
{
    rho::db::CDBAdapter& db = *((rho::db::CDBAdapter*)m_pDB);
    db.destroy_tables(include,exclude);
}

void CSQLite3Impl::isTableExist( const rho::String& tableName, rho::apiGenerator::CMethodResult& oResult)
{
	int result;
	
	result = rho_db_is_table_exist(m_pDB, tableName.c_str());
	oResult.set(result ? true : false);
}

void CSQLite3Impl::isUiWaitForDb( rho::apiGenerator::CMethodResult& oResult)
{
	int result;
	
	result = rho_db_is_ui_waitfordb(m_pDB);
	oResult.set(result ? true : false);
}

void CSQLite3Impl::doImport(const rho::String& zipName, rho::apiGenerator::CMethodResult& oResult)
{
	rho::db::CDBAdapter& db = *((rho::db::CDBAdapter*)m_pDB);
	bool bRes = db.importDatabase(zipName);
    oResult.set(bRes);
}

void CSQLite3Impl::doExport(rho::apiGenerator::CMethodResult& oResult)
{
	rho::db::CDBAdapter& db = *((rho::db::CDBAdapter*)m_pDB);
    String strRes = db.exportDatabase();
    oResult.set(strRes);
}

////////////////////////////////////////////////////////////////////////

class CSQLite3SingletonImpl: public CSQLite3SingletonBase
{
public:

    virtual void isBlobAttr( const rho::String& partition,  int sourceID,  const rho::String& attrName, rho::apiGenerator::CMethodResult& oResult)
    {
        bool bRes = db::CDBAdapter::getDB( partition.c_str()).getAttrMgr().isBlobAttr((int)sourceID, attrName.c_str());
        oResult.set(bRes);
    }

};

class CSQLite3Factory: public CSQLite3FactoryBase
{
public:
    ~CSQLite3Factory(){}

    ISQLite3Singleton* createModuleSingleton()
    { 
        return new CSQLite3SingletonImpl(); 
    }

    virtual ISQLite3* createModuleByID(const rho::String& strID){ return new CSQLite3Impl(); };
};

}
}

extern "C" void Init_SQLite3()
{
    rho::database::CSQLite3Factory::setInstance( new rho::database::CSQLite3Factory() );

    RHODESAPP().getExtManager().requireRubyFile("RhoDatabaseApi");

    rho::database::Init_SQLite3_API();
}
#ifndef RHO_NO_RUBY_API
extern "C" unsigned long rb_impl_SQLite3_execute(int argc, unsigned long *argv, rho::database::ISQLite3* pObj)
{
    void* pDB =  static_cast<rho::database::CSQLite3Impl*>(pObj)->getDb();
    return rb_c_impl_SQLite3_execute(argc, argv, pDB);
}
#else
extern "C" unsigned long rb_impl_SQLite3_execute(int argc, unsigned long *argv, rho::database::ISQLite3* pObj)
{
    return 0;
}
#endif

using namespace rho;
using namespace rho::common;
using namespace rho::json;

rho::String js_SQLite3_execute(const rho::String& strObjID, CJSONArray& argv, const rho::String&, const rho::String&, const rho::String& )
{
    rho::database::ISQLite3* pObj = rho::database::CSQLite3FactoryBase::getInstance()->getModuleByID(strObjID);
    void* pDB =  static_cast<rho::database::CSQLite3Impl*>(pObj)->getDb();

    sqlite3 * db = NULL;
	void **ppDB = &pDB;		
	sqlite3_stmt *statement = NULL;
	const char* sql = NULL;
	Vector< rho::Hashtable<rho::String, rho::String> > arRes;
    rho::apiGenerator::CMethodResult oResult;

	int nRes = 0;
    char * szErrMsg = 0;
    int is_batch = 0;

	db = (sqlite3 *)rho_db_get_handle(*ppDB);
    sql = argv[0].getString();
    is_batch = argv[1].getBoolean();

    RAWTRACE1("db_execute: %s", sql);

    PROF_START_CREATED("SQLITE");
    if ( is_batch )
    {
        PROF_START_CREATED("SQLITE_EXEC");

        rho_db_lock(*ppDB);
        nRes = sqlite3_exec(db, sql,  NULL, NULL, &szErrMsg);
        rho_db_unlock(*ppDB);

        PROF_STOP("SQLITE_EXEC");
    }
    else
    {
        rho_db_lock(*ppDB);
        PROF_START_CREATED("SQLITE_PREPARE");
        nRes = rho_db_prepare_statement(*ppDB, sql, -1, &statement);
        PROF_STOP("SQLITE_PREPARE");
        if ( nRes != SQLITE_OK)
        {
            szErrMsg = (char *)sqlite3_errmsg(db);
            rho_db_unlock(*ppDB);

            oResult.setArgError( String("could not prepare statement: ") + convertToStringA(nRes) + "; Message: " + (szErrMsg?szErrMsg:""));
            return oResult.toJSON();
        }

        if ( argv.getSize() > 2 && argv[2].isArray() )
        {
            int i = 0;
            CJSONArray args( argv[2] );
                
            for( ; i < args.getSize(); i++ )
            {
                CJSONEntry arg = args[i];
                if ( arg.isNull() )
                {
                    sqlite3_bind_null(statement, i+1);
                    continue;
                }

                if ( arg.isString() )
                    sqlite3_bind_text(statement, i+1, arg.getString(), strlen(arg.getString()), SQLITE_TRANSIENT);
                else if (arg.isFloat())
                    sqlite3_bind_double(statement, i+1, arg.getDouble() );
                else if (arg.isInteger())
                    sqlite3_bind_int64(statement, i+1, arg.getUInt64() );
                else if (arg.isBoolean())
                    sqlite3_bind_int(statement, i+1, arg.getBoolean() ? 1 : 0 );
                else
				{
                    sqlite3_reset(statement);

                    oResult.setArgError( String("Unsupported argument type. Argument number: ") + convertToStringA(i));
                    rho_db_unlock(*ppDB);
                    return oResult.toJSON();
				}
            }
        }

        PROF_START_CREATED("SQLITE_EXEC");
        nRes = sqlite3_step(statement);
        PROF_STOP("SQLITE_EXEC");

	    while( nRes== SQLITE_ROW ) 
        {
		    int nCount = sqlite3_data_count(statement);
		    int nCol = 0;
            rho::Hashtable<rho::String, rho::String> hashRec;

		    for(;nCol<nCount;nCol++)
            {
			    int nColType = sqlite3_column_type(statement,nCol);
			    const char* szColName = sqlite3_column_name(statement,nCol);
                String colValue;
    			
			    switch(nColType)
                {
				    case SQLITE_NULL:
					    break;
                    case SQLITE_FLOAT:
                    {
                        double dVal = sqlite3_column_double(statement, nCol);
                        colValue = convertToStringA(dVal);
                        break;
                    }
                    case SQLITE_INTEGER:
                    {
                        sqlite_int64 nVal = sqlite3_column_int64(statement, nCol);
                        colValue = convertToStringA(nVal);
                        break;
                    }
				    default:{
                        sqlite3_value * sqlValue = sqlite3_column_value(statement, nCol);
                        int nLen = sqlite3_value_bytes(sqlValue);
                        const char*  szValue = (const char *)sqlite3_value_text(sqlValue);
					    colValue = String(szValue, nLen);
					    break;
				    }
			    }
    			
                hashRec[szColName] = colValue;
		    }

            arRes.addElement( hashRec );

            PROF_START_CREATED("SQLITE_EXEC");
            nRes = sqlite3_step(statement);
            PROF_STOP("SQLITE_EXEC");

	    }

        rho_db_unlock(*ppDB);

    }

    if ( statement )
        sqlite3_reset(statement);

    PROF_STOP("SQLITE");

    if ( nRes != SQLITE_OK && nRes != SQLITE_ROW && nRes != SQLITE_DONE )
    {
        if ( !szErrMsg )
            szErrMsg = (char*)sqlite3_errmsg(db);

        oResult.setError( String("could not execute statement: ") + convertToStringA(nRes) + "; Message: " + (szErrMsg?szErrMsg:""));
        return oResult.toJSON();
    }

    oResult.set(arRes);

    return oResult.toJSON();
}
