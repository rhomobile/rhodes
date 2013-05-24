#pragma once

#include "common/RhoStd.h"
#include "api_generator/MethodResult.h"
#include "api_generator/BaseClasses.h"


namespace rho {
namespace examples {
///////////////////////////////////////////////////////////
struct IMegamodule
{
//constants

    static const char CONST1_STRING[];// "value1" 
    static const int CONST2_INTEGER = 123; 
    static const char CONST_STRING_1[];// "const1" 
    static const char CONST_STRING_2[];// "const2" 

//methods
    virtual ~IMegamodule(){}

    virtual void getStringProperty(rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void getIntegerProperty(rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void setIntegerProperty( int IntegerProperty, rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void typesTest( const rho::String& paramStr,  bool paramBool,  int paramInt,  double paramFloat,  const rho::Vector<rho::String>& paramArray,  const rho::Hashtable<rho::String, rho::String>& paramHash, rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void produceArray(rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void produceHash(rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void produceComplicatedResult(rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void showAlertFromUIThread( const rho::String& message, rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void setPeriodicallyCallback( int periodInMilliseconds, rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void isPeriodicallyCallbackSetted(rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void stopPeriodicallyCallback(rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void complicatedTypesTest1( const rho::Vector<rho::String>& paramArray, rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void getProperty( const rho::String& propertyName, rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void getProperties( const rho::Vector<rho::String>& arrayofNames, rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void getAllProperties(rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void setProperty( const rho::String& propertyName,  const rho::String& propertyValue, rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void setProperties( const rho::Hashtable<rho::String, rho::String>& propertyMap, rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void clearAllProperties(rho::apiGenerator::CMethodResult& oResult) = 0;

};

struct IMegamoduleSingleton
{
//constants


    virtual ~IMegamoduleSingleton(){}

//methods
    virtual void getObjectsCount(rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void getObjectByIndex( int index, rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void enumerate(rho::apiGenerator::CMethodResult& oResult) = 0;


    virtual rho::String getDefaultID() = 0;
    virtual rho::String getInitialDefaultID() = 0;
    virtual void setDefaultID(const rho::String& strID) = 0;

    virtual void addCommandToQueue(rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor) = 0;
    virtual void callCommandInThread(rho::common::IRhoRunnable* pFunctor) = 0;
};

struct IMegamoduleFactory
{
    virtual ~IMegamoduleFactory(){}

    virtual IMegamoduleSingleton* getModuleSingleton() = 0;


    virtual IMegamodule* getModuleByID(const rho::String& strID) = 0;

};


}
}
