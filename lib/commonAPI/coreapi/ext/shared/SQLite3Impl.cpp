#include "generated/cpp/SQLite3Base.h"
#include "ruby/ext/rho/rhoruby.h"
#include "json/JSONIterator.h"
#include "sqlite/sqlite3.h"

extern "C" {
int rho_db_open(const char* szDBPath, const char* szDBPartition, void** ppDB);
int rho_db_close(void* pDB);
int rho_db_startTransaction(void* pDB);
int rho_db_commitTransaction(void* pDB);
int rho_db_rollbackTransaction(void* pDB);
int rho_db_destroy_tables(void* pDB, unsigned long arInclude, unsigned long arExclude);
void* rho_db_get_handle(void* pDB);
int rho_db_is_ui_waitfordb(void* pDB);
int rho_db_prepare_statement(void* pDB, const char* szSql, int nByte, sqlite3_stmt **ppStmt);
void rho_db_lock(void* pDB);
void rho_db_unlock(void* pDB);
int  rho_db_is_table_exist(void* pDB, const char* szTableName);
VALUE ruby_db_execute(int argc, VALUE *argv, VALUE self);
VALUE rho_db_export( void* pDB );
int rho_db_import( void* pDB, const char* zipName );
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
	virtual void import(const rho::String& zipName, rho::apiGenerator::CMethodResult& oResult);
	virtual void export(rho::apiGenerator::CMethodResult& oResult);
    virtual void destroyTables(const rho::Vector<rho::String>& include, const rho::Vector<rho::String>& exclude, rho::apiGenerator::CMethodResult& oResult);
    virtual void isTableExist( const rho::String& tableName, rho::apiGenerator::CMethodResult& oResult);
    virtual void isUiWaitForDb(rho::apiGenerator::CMethodResult& oResult);
    virtual void execute( const rho::String& sqlStmt,  bool isBatch,  const rho::Vector<rho::String>& args, rho::apiGenerator::CMethodResult& oResult);
	void* getDb() const {return m_pDB;}

private:
	void* m_pDB;

};

void CSQLite3Impl::open( const rho::String& dbPath,  const rho::String& dbPartition, rho::apiGenerator::CMethodResult& oResult)
{
	const char *szDbName = NULL;
    const char *szDbPartition = NULL;
	m_pDB = NULL;
	int result;
	
	result = rho_db_open(dbPath.c_str(), dbPartition.c_str(), &m_pDB);
	oResult.set(result);
}

void CSQLite3Impl::close(rho::apiGenerator::CMethodResult& oResult)
{
	int result;
	
	result = rho_db_close(m_pDB);
	oResult.set(result);
}

void CSQLite3Impl::startTransaction(rho::apiGenerator::CMethodResult& oResult)
{
	int result;
	
	result = rho_db_startTransaction(m_pDB);
	oResult.set(result);
}

void CSQLite3Impl::commitTransaction(rho::apiGenerator::CMethodResult& oResult)
{
	int result;
	
	result = rho_db_commitTransaction(m_pDB);
	oResult.set(result);
}

void CSQLite3Impl::rollbackTransaction(rho::apiGenerator::CMethodResult& oResult)
{
	int result;
	
	result = rho_db_rollbackTransaction(m_pDB);
	oResult.set(result);
}

void CSQLite3Impl::lockDb(rho::apiGenerator::CMethodResult& oResult)
{
	rho_db_lock(m_pDB);
	oResult.set(0);
}

void CSQLite3Impl::unlockDb(rho::apiGenerator::CMethodResult& oResult)
{
	rho_db_unlock(m_pDB);
	oResult.set(0);
}

VALUE getRubyStringArrayFromVector(const rho::Vector<rho::String>& vec)
{
    VALUE ary = rho_ruby_create_array();
	for(int i = 0; i < vec.size(); ++i)
		rho_ruby_add_to_array(ary, rho_ruby_create_string(vec[i].c_str()));

	return ary;
}

void CSQLite3Impl::destroyTables(const rho::Vector<rho::String>& include, const rho::Vector<rho::String>& exclude, rho::apiGenerator::CMethodResult& oResult)
{
	int result;
	
	result = rho_db_destroy_tables(m_pDB, getRubyStringArrayFromVector(include), getRubyStringArrayFromVector(exclude));
	oResult.set(result);
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

void CSQLite3Impl::import(const rho::String& zipName, rho::apiGenerator::CMethodResult& oResult)
{
	int result;
	
	result = rho_db_import(m_pDB, zipName.c_str());
	oResult.set(result ? true : false);
}

void CSQLite3Impl::export(rho::apiGenerator::CMethodResult& oResult)
{
	VALUE val = rho_db_export(m_pDB);
	oResult.set(String(getStringFromValue(val)));
}

void CSQLite3Impl::execute( const rho::String& sqlStmt,  bool isBatch,  const rho::Vector<rho::String>& args, rho::apiGenerator::CMethodResult& oResult)
{
}

////////////////////////////////////////////////////////////////////////

class CSQLite3SingletonImpl: public CSQLite3SingletonBase
{
public:
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

extern "C" void Init_SQLite3()
{
    CSQLite3Factory::setInstance( new CSQLite3Factory() );
    Init_SQLite3_API();
}

}
}

#include "ruby.h"
extern "C" VALUE rb_impl_SQLite3_execute(int argc, VALUE *argv, rho::database::ISQLite3* pObj)
{
    sqlite3 * db = NULL;
	void* pDB =  dynamic_cast<rho::database::CSQLite3Impl*>(pObj)->getDb();
	void **ppDB = &pDB;		
	sqlite3_stmt *statement = NULL;
	const char* sql = NULL;
	VALUE arRes = rb_ary_new();
    VALUE* colNames = NULL;
	int nRes = 0;
    char * szErrMsg = 0;
    int is_batch = 0;

	if ((argc < 2) || (argc > 3))
		rb_raise(rb_eArgError, "wrong # of arguments(%d for 3)",argc);
	
	db = (sqlite3 *)rho_db_get_handle(*ppDB);
	sql = RSTRING_PTR(argv[0]);
    is_batch = argv[1] == Qtrue ? 1 : 0;

    RAWTRACE1("db_execute: %s", sql);

    if ( is_batch )
    {
        rho_db_lock(*ppDB);
        nRes = sqlite3_exec(db, sql,  NULL, NULL, &szErrMsg);
        rho_db_unlock(*ppDB);
    }
    else
    {
        rho_db_lock(*ppDB);
        nRes = rho_db_prepare_statement(*ppDB, sql, -1, &statement);
        //nRes = sqlite3_prepare_v2(db, sql, -1, &statement, NULL);
        if ( nRes != SQLITE_OK)
        {
            szErrMsg = (char *)sqlite3_errmsg(db);
            rho_db_unlock(*ppDB);

            rb_raise(rb_eArgError, "could not prepare statement: %d; Message: %s",nRes, (szErrMsg?szErrMsg:""));
        }

        if ( (argc > 2) && (TYPE(argv[2]) == T_ARRAY) )
        {
            int i = 0;
            VALUE args = argv[2];
            if ( RARRAY_LEN(args) > 0 && TYPE(RARRAY_PTR(args)[0]) == T_ARRAY )
                args = RARRAY_PTR(args)[0];

            for( ; i < RARRAY_LEN(args); i++ )
            {
                VALUE arg = RARRAY_PTR(args)[i];
                if (NIL_P(arg))
                {
                    sqlite3_bind_null(statement, i+1);
                    continue;
                }

                switch( TYPE(arg) )
                {
                case T_STRING:
                    sqlite3_bind_text(statement, i+1, RSTRING_PTR(arg), RSTRING_LEN(arg), SQLITE_TRANSIENT);
                    break;
                case T_FLOAT:
                    sqlite3_bind_double(statement, i+1, NUM2DBL(arg));
                    break;
                case T_FIXNUM:
                case T_BIGNUM:
                    sqlite3_bind_int64(statement, i+1, NUM2LL(arg));
                    break;
                default:
					{
						VALUE strVal = rb_funcall(arg, rb_intern("to_s"), 0);	
	                    sqlite3_bind_text(statement, i+1, RSTRING_PTR(strVal), -1, SQLITE_TRANSIENT);	
					}
					break;
                }
            }
        }

        nRes = sqlite3_step(statement);

	    while( nRes== SQLITE_ROW ) {
		    int nCount = sqlite3_data_count(statement);
		    int nCol = 0;
		    VALUE hashRec = rb_hash_new();

            //if ( !colNames )
            //    colNames = getColNames(statement, nCount);

		    for(;nCol<nCount;nCol++){
			    int nColType = sqlite3_column_type(statement,nCol);
			    const char* szColName = sqlite3_column_name(statement,nCol);
			    VALUE colName = rb_str_new2(szColName);
			    VALUE colValue = Qnil;
    			
			    switch(nColType){
				    case SQLITE_NULL:
					    break;
                    case SQLITE_FLOAT:
                    {
                        double dVal = sqlite3_column_double(statement, nCol);
                        colValue = DBL2NUM(dVal);
                        break;
                    }
                    case SQLITE_INTEGER:
                    {
                        sqlite_int64 nVal = sqlite3_column_int64(statement, nCol);
                        colValue = LL2NUM(nVal);
                        break;
                    }
				    default:{
                        sqlite3_value * sqlValue = sqlite3_column_value(statement, nCol);
                        int nLen = sqlite3_value_bytes(sqlValue);
                        const char*  szValue = (const char *)sqlite3_value_text(sqlValue);
					    //char *text = (char *)sqlite3_column_text(statement, nCol);
					    colValue = rb_str_new(szValue, nLen);
					    break;
				    }
			    }
    			
			    rb_hash_aset(hashRec, colName/*colNames[nCol]*/, colValue);
		    }
    		
		    rb_ary_push(arRes, hashRec);

            nRes = sqlite3_step(statement);

	    }

        rho_db_unlock(*ppDB);

    }

    if ( statement )
        //sqlite3_finalize(statement);
        sqlite3_reset(statement);

    if ( colNames )
        free(colNames);

    if ( nRes != SQLITE_OK && nRes != SQLITE_ROW && nRes != SQLITE_DONE )
    {
        if ( !szErrMsg )
            szErrMsg = (char*)sqlite3_errmsg(db);

        rb_raise(rb_eArgError, "could not execute statement: %d; Message: %s",nRes, (szErrMsg?szErrMsg:""));
    }

	return arRes;
}

rho::String js_SQLite3_execute(rho::json::CJSONArray& argv, const rho::String& strObjID)
{
    //TODO: js_Database_execute
    return "{}";
}