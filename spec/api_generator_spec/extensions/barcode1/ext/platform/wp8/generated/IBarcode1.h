#pragma once

#include "common/RhoStd.h"
#include "api_generator/MethodResult.h"
#include "api_generator/BaseClasses.h"

// WP8 specific declaration
namespace Barcode1Runtime { interface class IBarcode1Impl; }

///////////////////////////////////////////////////////////

struct IBarcode1
{
    virtual ~IBarcode1(){}

    virtual void enable( const rho::Hashtable<rho::StringW, rho::StringW>& propertyMap, rho::apiGenerator::CMethodResult& oResult) = 0;
	virtual void start(rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void stop(rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void disable(rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void take( const rho::Hashtable<rho::StringW, rho::StringW>& propertyMap, rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void getProperty( const rho::StringW& propertyName, rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void getProperties( const rho::Vector<rho::StringW>& arrayofNames, rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void getAllProperties(rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void setProperty( const rho::StringW& propertyName,  const rho::StringW& propertyValue, rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void setProperties( const rho::Hashtable<rho::StringW, rho::StringW>& propertyMap, rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void clearProps(rho::apiGenerator::CMethodResult& oResult) = 0;

	// WP8 specific method
	virtual void registerRuntime(Barcode1Runtime::IBarcode1Impl^ runtime) = 0;
};

struct IBarcode1Singleton
{
    virtual ~IBarcode1Singleton(){}

    virtual void enumerate(rho::apiGenerator::CMethodResult& oResult) = 0;


    virtual rho::StringW getDefaultID() = 0;
    virtual rho::StringW getInitialDefaultID() = 0;
    virtual void setDefaultID(const rho::StringW& strID) = 0;

    virtual void addCommandToQueue(rho::common::IRhoRunnable* pFunctor) = 0;
    virtual void callCommandInThread(rho::common::IRhoRunnable* pFunctor) = 0;
};

struct IBarcode1Factory
{
    virtual ~IBarcode1Factory(){}

    virtual IBarcode1Singleton* getModuleSingleton() = 0;


    virtual IBarcode1* getModuleByID(const rho::StringW& strID) = 0;

};

class CBarcode1FactoryBase : public rho::apiGenerator::CModuleFactoryBase<IBarcode1, IBarcode1Singleton, IBarcode1Factory>
{
protected:
    static rho::common::CAutoPtr<CBarcode1FactoryBase> m_pInstance;

public:

    static void setInstance(CBarcode1FactoryBase* pInstance){ m_pInstance = pInstance; }
    static CBarcode1FactoryBase* getInstance(){ return m_pInstance; }

    static IBarcode1Singleton* getBarcode1SingletonS(){ return getInstance()->getModuleSingleton(); }
};

extern "C" void Init_Barcode1_API();
