#include "generated/cpp/SQLite3Base.h"
#include "json/JSONIterator.h"
#include "db/DBAdapter.h"
#include "common/RhodesApp.h"

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

    RHODESAPP().getExtManager().requireRubyFile("Database");

    rho::database::Init_SQLite3_API();
}

extern "C" unsigned long rb_impl_SQLite3_execute(int argc, unsigned long *argv, rho::database::ISQLite3* pObj)
{
    void* pDB =  static_cast<rho::database::CSQLite3Impl*>(pObj)->getDb();
    return rb_c_impl_SQLite3_execute(argc, argv, pDB);
}

rho::String js_SQLite3_execute(rho::json::CJSONArray& argv, const rho::String& strObjID)
{
    //TODO: js_Database_execute
    return "{}";
}
