#pragma once

#include "common/RhoStd.h"
#include "api_generator/MethodResult.h"
#include "api_generator/BaseClasses.h"


namespace rho {
///////////////////////////////////////////////////////////
struct INetworkAccess
{
//constants


//methods
    virtual ~INetworkAccess(){}


};

struct INetworkAccessSingleton
{
//constants

    static const char AUTH_BASIC[];// "basic" 

    virtual ~INetworkAccessSingleton(){}

//methods
    virtual void cancel(rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void downloadFile( const rho::Hashtable<rho::String, rho::String>& propertyMap, rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void get( const rho::Hashtable<rho::String, rho::String>& propertyMap, rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void post( const rho::Hashtable<rho::String, rho::String>& propertyMap, rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void uploadFile( const rho::Hashtable<rho::String, rho::String>& propertyMap, rho::apiGenerator::CMethodResult& oResult) = 0;


    virtual void addCommandToQueue(rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor) = 0;
    virtual void callCommandInThread(rho::common::IRhoRunnable* pFunctor) = 0;
};

struct INetworkAccessFactory
{
    virtual ~INetworkAccessFactory(){}

    virtual INetworkAccessSingleton* getModuleSingleton() = 0;


};


}
