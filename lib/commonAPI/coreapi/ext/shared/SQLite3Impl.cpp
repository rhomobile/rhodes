#include "generated/cpp/SQLite3Base.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "SQLite3"

namespace rho {
namespace database {

using namespace rho::apiGenerator;
using namespace rho::common;

class CSQLite3Impl: public CSQLite3Base
{
public:

    virtual void open( const rho::String& dbPath,  const rho::String& dbPartition, rho::apiGenerator::CMethodResult& oResult)
    {
        int i = 0;
    }

    virtual void close(rho::apiGenerator::CMethodResult& oResult){}
    virtual void startTransaction(rho::apiGenerator::CMethodResult& oResult){}
    virtual void commitTransaction(rho::apiGenerator::CMethodResult& oResult){}
    virtual void rollbackTransaction(rho::apiGenerator::CMethodResult& oResult){}
    virtual void lockDb(rho::apiGenerator::CMethodResult& oResult){}
    virtual void unlockDb(rho::apiGenerator::CMethodResult& oResult){}
    virtual void destroyTables(rho::apiGenerator::CMethodResult& oResult){}
    virtual void isTableExist( const rho::String& tableName, rho::apiGenerator::CMethodResult& oResult){}
    virtual void isUiWaitForDb( const rho::String& tableName, rho::apiGenerator::CMethodResult& oResult){}
    virtual void execute( const rho::String& sqlStmt,  bool isBatch,  const rho::Vector<rho::String>& args, rho::apiGenerator::CMethodResult& oResult){}

};


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