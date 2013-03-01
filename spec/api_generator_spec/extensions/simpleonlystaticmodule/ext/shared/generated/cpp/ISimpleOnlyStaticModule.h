#pragma once

#include "common/RhoStd.h"
#include "api_generator/MethodResult.h"
#include "api_generator/BaseClasses.h"


namespace rho {
namespace examples {
///////////////////////////////////////////////////////////
struct ISimpleOnlyStaticModule
{
//constants


//methods
    virtual ~ISimpleOnlyStaticModule(){}


};

struct ISimpleOnlyStaticModuleSingleton
{
//constants


    virtual ~ISimpleOnlyStaticModuleSingleton(){}

//methods
    virtual void calcSumm( int x,  int y, rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void joinStrings( const rho::Vector<rho::String>& strings, rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void getPlatform(rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void showAlertFromUIThread(rho::apiGenerator::CMethodResult& oResult) = 0;


    virtual void addCommandToQueue(rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor) = 0;
    virtual void callCommandInThread(rho::common::IRhoRunnable* pFunctor) = 0;
};

struct ISimpleOnlyStaticModuleFactory
{
    virtual ~ISimpleOnlyStaticModuleFactory(){}

    virtual ISimpleOnlyStaticModuleSingleton* getModuleSingleton() = 0;


};


}
}
