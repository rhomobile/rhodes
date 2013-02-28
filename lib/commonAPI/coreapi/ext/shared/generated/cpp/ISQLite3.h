#pragma once

#include "common/RhoStd.h"
#include "api_generator/MethodResult.h"
#include "api_generator/BaseClasses.h"


namespace rho {
namespace database {
///////////////////////////////////////////////////////////
struct ISQLite3
{
//constants


//methods
    virtual ~ISQLite3(){}

    virtual void open( const rho::String& dbPath,  const rho::String& dbPartition, rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void close(rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void startTransaction(rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void commitTransaction(rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void rollbackTransaction(rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void lockDb(rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void unlockDb(rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void destroyTables(rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void isTableExist( const rho::String& tableName, rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void isUiWaitForDb( const rho::String& tableName, rho::apiGenerator::CMethodResult& oResult) = 0;

};

struct ISQLite3Singleton
{
//constants


    virtual ~ISQLite3Singleton(){}

//methods


    virtual void addCommandToQueue(rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor) = 0;
    virtual void callCommandInThread(rho::common::IRhoRunnable* pFunctor) = 0;
};

struct ISQLite3Factory
{
    virtual ~ISQLite3Factory(){}

    virtual ISQLite3Singleton* getModuleSingleton() = 0;


};


}
}
