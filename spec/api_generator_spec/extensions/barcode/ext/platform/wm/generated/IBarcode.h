#pragma once

#include "common/RhoStd.h"
#include "api_generator/MethodResult.h"
#include "api_generator/BaseClasses.h"

///////////////////////////////////////////////////////////
struct IBarcode
{
    virtual ~IBarcode(){}

    virtual void enable( const rho::Hashtable<rho::StringW, rho::StringW>& propertyMap, CMethodResult& oResult) = 0;
    virtual void start(CMethodResult& oResult) = 0;
    virtual void stop(CMethodResult& oResult) = 0;
    virtual void disable(CMethodResult& oResult) = 0;
    virtual void take( const rho::Hashtable<rho::StringW, rho::StringW>& propertyMap, CMethodResult& oResult) = 0;
    virtual void getProperty( const rho::StringW& propertyName, CMethodResult& oResult) = 0;
    virtual void getProperties( const rho::Vector<rho::StringW>& arrayofNames, CMethodResult& oResult) = 0;
    virtual void getAllProperties(CMethodResult& oResult) = 0;
    virtual void setProperty( const rho::StringW& propertyName,  const rho::StringW& propertyValue, CMethodResult& oResult) = 0;
    virtual void setProperties( const rho::Hashtable<rho::StringW, rho::StringW>& propertyMap, CMethodResult& oResult) = 0;
    virtual void clearProps(CMethodResult& oResult) = 0;

};

struct IBarcodeSingleton
{
    virtual ~IBarcodeSingleton(){}

    virtual void enumerate(CMethodResult& oResult) = 0;


    virtual rho::StringW getDefaultID() = 0;
    virtual rho::StringW getInitialDefaultID() = 0;
    virtual void setDefaultID(const rho::StringW& strID) = 0;

    virtual void addCommandToQueue(rho::common::IRhoRunnable* pFunctor) = 0;
    virtual void callCommandInThread(rho::common::IRhoRunnable* pFunctor) = 0;
};

struct IBarcodeFactory
{
    virtual ~IBarcodeFactory(){}

    virtual IBarcodeSingleton* getModuleSingleton() = 0;


    virtual IBarcode* getModuleByID(const rho::StringW& strID) = 0;

};

class CBarcodeFactoryBase : public CModuleFactoryBase<IBarcode, IBarcodeSingleton, IBarcodeFactory>
{
protected:
    static rho::common::CAutoPtr<CBarcodeFactoryBase> m_pInstance;

public:

    static void setInstance(CBarcodeFactoryBase* pInstance){ m_pInstance = pInstance; }
    static CBarcodeFactoryBase* getInstance(){ return m_pInstance; }

    static IBarcodeSingleton* getBarcodeSingletonS(){ return getInstance()->getModuleSingleton(); }
};

extern "C" void Init_Barcode_API();
